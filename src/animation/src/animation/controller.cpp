#include "controller.h"

#include <Eigen/Dense>
#include <animation/nn/models/pfnnmodel.h>

using namespace animation;

void Controller::update()
{
    float rest = set_inputs();
    update_model();
    //TODO check if this is the right place to check trajector onto the terrain
    //trajectory->post_process(*terrain); //useless on flat terrain
    output_to_skeleton(rest);
}

float Controller::set_inputs()
{
    //TODO to finish

    //Calculate Root
    //TODO initialize root_point_index correctly
    Matrix4f current_root = trajectory->root_point().get_transformation(); //copy matrix
    current_root(1,3) = 0.0; //For flat terrain
    unsigned int start = 0;

    //Input trajectory Positions / Directions / Velocities / Styles
    auto point_samples = trajectory->samples(model->trajectory_samples_n());
    unsigned int styles_dim = model->get_styles_dim();
    unsigned int trajectory_dim_in = 7+styles_dim;
    unsigned int i=0;
    for(auto sample = point_samples.cbegin(); sample != point_samples.cend(); ++sample) {
        Eigen::Vector3f pos = sample->get_position().get_relative_position_to(current_root);
        Eigen::Vector3f dir = sample->get_direction().get_relative_direction_to(current_root);
        Eigen::Vector3f vel = sample->get_velocity().get_relative_direction_to(current_root);
        float speed = sample->get_speed();
        set_input(start + i*trajectory_dim_in + 0, pos.x());
        set_input(start + i*trajectory_dim_in + 1, pos.z());
        set_input(start + i*trajectory_dim_in + 2, dir.x());
        set_input(start + i*trajectory_dim_in + 3, dir.z());
        set_input(start + i*trajectory_dim_in + 4, vel.x());
        set_input(start + i*trajectory_dim_in + 5, vel.z());
        set_input(start + i*trajectory_dim_in + 6, speed);

        unsigned int style_input_slot = start
                + i*trajectory_dim_in
                 + (trajectory_dim_in - styles_dim); //This should always be 7

        for(unsigned int j=0; j<styles_dim; j++) {
            set_input(style_input_slot + j, sample->get_style(j));
        }

        i++;
    }

    start += trajectory_dim_in * point_samples.size();

    Eigen::Matrix4f previousRoot = trajectory->root_point(-1)
                                    .get_transformation();
    previousRoot(1,3) = 0.0; //For flat terrain

    unsigned const joint_dim_in = 12;
    //Input Previous Bone Positions / Velocities
     unsigned int bones_n = this->skeleton->bones_n();
     for(i=0; i<bones_n; i++) {
//         if (start + i*joint_dim_in + 11 > this->X->rows()) {
//             break;
//         }
         auto &bone = skeleton->bone(i);
         Vector3f pos = bone.get_transform_position().get_relative_position_to(previousRoot);
         Vector3f forward = bone.get_transform_position().get_relative_direction_to(previousRoot);
         Vector3f up = bone.get_transform_up().get_relative_direction_to(previousRoot);
         Vector3f vel = bone.get_velocity().get_relative_direction_to(previousRoot);
         set_input(start + i*joint_dim_in + 0, pos.x());
         set_input(start + i*joint_dim_in + 1, pos.y());
         set_input(start + i*joint_dim_in + 2, pos.z());
         set_input(start + i*joint_dim_in + 3, forward.x());
         set_input(start + i*joint_dim_in + 4, forward.y());
         set_input(start + i*joint_dim_in + 5, forward.z());
         set_input(start + i*joint_dim_in + 6, up.x());
         set_input(start + i*joint_dim_in + 7, up.y());
         set_input(start + i*joint_dim_in + 8, up.z());
         set_input(start + i*joint_dim_in + 9, vel.x());
         set_input(start + i*joint_dim_in + 10, vel.y());
         set_input(start + i*joint_dim_in + 11, vel.z());
     }
     start += joint_dim_in * bones_n;

    // DAMPING
    //TODO this is only for PFNNModel
    float rest = std::pow(
            1.0f - trajectory->root_point().get_style(0),
            0.25f
    );
    float damping = 1.0f - (rest * 0.9f + 0.1f);
    reinterpret_cast<nn::PFNNModel*>(model.get())->set_damping(damping);

    return rest;
}

void Controller::update_model()
{
    model->predict(X, Y);
}

void Controller::output_to_skeleton(float rest)
{
    //TODO remove hardcoded values
    unsigned const trajectory_dim_out = 3;
    unsigned const joint_dim_out = 3;
    unsigned const bones_n = skeleton->bones_n();
    const float framerate = 60.0;

    Eigen::Vector3f root_motion(
            get_output(trajectory_dim_out*6 + joint_dim_out*bones_n + 0),
            get_output(trajectory_dim_out*6 + joint_dim_out*bones_n + 1),
            get_output(trajectory_dim_out*6 + joint_dim_out*bones_n + 2)
    );
    root_motion /= framerate;
    trajectory->update_trajectory(root_motion, rest, framerate);

    //TODO continue
//    throw std::runtime_error( "Controller::apply_skeleton() is unsupported" );
//
    // Update Trajectory - part 2
    unsigned int start = 0;
    for(unsigned int index = trajectory->get_root_point_index()+1; index<trajectory->points().size(); index++) {
        //ROOT	1		2		3		4		5
        //.x....x.......x.......x.......x.......x
//        int prevSampleIndex = get_previous_sample(index).get_index() / Trajectory::POINT_DENSITY;
//        int nextSampleIndex = get_next_sample(index).get_index() / Trajectory::POINT_DENSITY;
//        float factor = (float)(index % Trajectory::POINT_DENSITY) / Trajectory::POINT_DENSITY;
//
//
//        Vector3f prev_pos = Vector3f(
//                get_output(start + (prevSampleIndex-6)*trajectory_dim_out + 0),
//                0.0,
//                get_output(start + (prevSampleIndex-6)*trajectory_dim_out + 1)
//        ).get_relative_position_from(next_root);
//        Vector3f prev_dir = Vector3f(
//                get_output(start + (prevSampleIndex-6)*trajectory_dim_out + 2),
//                0.0,
//                get_output(start + (prevSampleIndex-6)*trajectory_dim_out + 3)
//        ).normalized().get_relative_direction_from(next_root);
//        Vector3f prev_vel = Vector3f(
//                get_output(start + (prevSampleIndex-6)*trajectory_dim_out + 4),
//                0.0,
//                get_output(start + (prevSampleIndex-6)*trajectory_dim_out + 5)
//        ).get_relative_direction_from(next_root);
//
//        Vector3f next_pos = Vector3f(
//                get_output(start + (nextSampleIndex-6)*trajectory_dim_out + 0),
//                0.0,
//                get_output(start + (nextSampleIndex-6)*trajectory_dim_out + 1)
//        ).get_relative_position_from(next_root);
//        Vector3f next_dir = Vector3f(
//                get_output(start + (nextSampleIndex-6)*trajectory_dim_out + 2),
//                0.0,
//                get_output(start + (nextSampleIndex-6)*trajectory_dim_out + 3)
//        ).normalized().get_relative_direction_from(next_root);
//        Vector3f next_vel = Vector3f(
//                get_output(start + (nextSampleIndex-6)*trajectory_dim_out + 4),
//                0.0,
//                get_output(start + (nextSampleIndex-6)*trajectory_dim_out + 5)
//        ).get_relative_direction_from(next_root);
//
//        Vector3f pos = (1.0 - factor) * prev_pos + factor * next_pos;
//        Vector3f dir = ((1.0 - factor) * prev_dir + factor * next_dir).normalized();
//        Vector3f vel = (1.0 - factor) * prev_vel + factor * next_vel;
//
//        pos = Vector3::Lerp(trajectory->get_point(index).GetPosition() + vel / framerate, pos, 0.5f);
//
//        trajectory->get_point(index).set_position(
//                Utility.Interpolate(
//                        trajectory->get_point(index).get_position(),
//                        pos,
//                        trajectory_correction
//                )
//        );
//        trajectory->get_point(index).set_direction(
//                Utility.Interpolate(
//                        trajectory->get_point(index).get_direction(),
//                        dir,
//                        trajectory_correction
//                )
//        );
//        trajectory->get_point(index).set_velocity(
//                Utility.Interpolate(
//                        trajectory->get_point(index).get_velocity(),
//                        vel,
//                        trajectory_correction
//                )
//        );
    }
    start += trajectory_dim_out*6;

    // Compute Posture
    for(int i=0; i<bones_n; i++) {
        Vector3f position = Vector3f(
                get_output(start + i*joint_dim_out + 0),
                get_output(start + i*joint_dim_out + 1),
                get_output(start + i*joint_dim_out + 2)
        ).get_relative_position_from(current_root);

        Vector3f forward = Vector3f(
                get_output(start + i*joint_dim_out + 3),
                get_output(start + i*joint_dim_out + 4),
                get_output(start + i*joint_dim_out + 5)
        ).normalized().get_relative_direction_from(current_root);

        Vector3f up = Vector3f(
                get_output(start + i*joint_dim_out + 6),
                get_output(start + i*joint_dim_out + 7),
                get_output(start + i*joint_dim_out + 8)
        ).normalized().get_relative_direction_from(currentRoot);

        Vector3f velocity = Vector3f(
                get_output(start + i*joint_dim_out + 9),
                get_output(start + i*joint_dim_out + 10),
                get_output(start + i*joint_dim_out + 11)
        ).get_relative_direction_from(current_root);

        positions[i] = Vector3f::Lerp(positions[i] + velocity / framerate, position, 0.5f);
        forwards[i] = forward;
        ups[i] = up;
        velocities[i] = velocity;
    }
    start += joint_dim_out*bones_n;

    //Assign Posture
    transform = root_object_transform();
    transform.position = next_root.get_position();
    transform.rotation = next_root.get_rotation();
    for(unsigned int i=0; i<bones_n; i++) {
        skeleton->bone(i).set_position(positions[i]);
        skeleton->bone(i).set_rotation(Quaternionf::LookRotation(forwards[i], ups[i]));
    }
}

void Controller::set_input(unsigned int index, float value)
{
    this->X->set_value(index, 0, value);
}

float Controller::get_output(unsigned int index) const
{
    return this->Y->get_value(index, 0);
}

unsigned int Controller::get_input_dim() const
{
    return this->model->x_dim();
}

unsigned int Controller::get_output_dim() const
{
    return this->model->y_dim();
}

Trajectory::Point& Controller::get_sample(unsigned int index) const
{
    return trajectory->get_point(std::clamp(index*10, 0, trajectory->points().size()-1));
}

Trajectory::Point& Controller::get_previous_sample(unsigned int index) const
{
    return get_sample(index / 10);
}

Trajectory::Point& Controller::get_next_sample(unsigned int index) const
{
    if(index % 10 == 0) {
        return get_sample(index / 10);
    } else {
        return get_sample(index / 10 + 1);
    }
}