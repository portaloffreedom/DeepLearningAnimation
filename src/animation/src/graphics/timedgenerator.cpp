#include "timedgenerator.h"
#include <cmath>
#include <iomanip>
#include <algorithm>

#include "animation/nn/models/pfnnmodel.h"

TimedGenerator::TimedGenerator(unsigned int timer_wait,
                               animation::Controller* anim_controller,
                               std::unique_ptr<animation::KeyboardController> &controller,
                               QObject* parent)
    : QObject(parent)
    , timer(nullptr)
    , interval(timer_wait)
    , anim_controller(anim_controller)
    , trajectory(new animation::Trajectory(points, controller->get_styles_dim()))
    , key_controller(std::move(controller))
    , trajectory_dim_in(TRAJECTORY_DIM_IN)
    , trajectory_dim_out(TRAJECTORY_DIM_OUT)
    , joint_dim_in(JOINT_DIM_IN)
    , joint_dim_out(JOINT_DIM_OUT)
{
    this->timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, &TimedGenerator::run_cycle);
    timer->setInterval(interval);
}

TimedGenerator::~TimedGenerator()
{}

void TimedGenerator::start()
{
    timer->start();
    beg_ = clock_::now();
}

void TimedGenerator::setUpdateTimerDelay(int delay)
{
    timer->setInterval(delay);
}

void TimedGenerator::run_cycle()
{
    assert(anim_controller != nullptr);

    // input -> network
    set_inputs();

    // update network
    float ms_for_nn = update_network();

    // output <- network
    auto output = get_outputs();

    // measure time delta
    double delta_time = std::chrono::duration_cast<second_>
                        (clock_::now() - beg_).count();
    beg_ = clock_::now();

    emit neuron_output(output, ms_for_nn);
}

const animation::TrajectoryPoint& TimedGenerator::get_sample(unsigned int index) const
{
    //auto new_index = std::clamp(index*10, 0, trajectory->points().size()-1);
    unsigned int new_index = std::min(index*10, static_cast<unsigned int>(trajectory->points().size()-1));
    return trajectory->points()[new_index];
}

void TimedGenerator::set_inputs()
{
//     //TODO
//     nn->set_input(0, 0);
//
//     //Calculate Root
//     //TODO initialize root_point_index correctly
//     Matrix4f current_root = trajectory->points()[root_point_index].get_transformation(); //copy matrix
//     current_root(1,3) = 0.0; //For flat terrain
//     int start = 0;
//     //Input trajectory Positions / Directions / Velocities / Styles
//     for(int i=0; i<point_samples; i++) {
//         Eigen::Vector3f pos = get_sample(i).get_position().get_relative_position_to(current_root);
//         Eigen::Vector3f dir = get_sample(i).get_direction().get_relative_direction_to(current_root);
//         Eigen::Vector3f vel = get_sample(i).get_velocity().get_relative_direction_to(current_root);
//         float speed = get_sample(i).get_speed();
//         nn->set_input(start + i*trajectory_dim_in + 0, pos.x());
//         nn->set_input(start + i*trajectory_dim_in + 1, pos.z());
//         nn->set_input(start + i*trajectory_dim_in + 2, dir.x());
//         nn->set_input(start + i*trajectory_dim_in + 3, dir.z());
//         nn->set_input(start + i*trajectory_dim_in + 4, vel.x());
//         nn->set_input(start + i*trajectory_dim_in + 5, vel.z());
//         nn->set_input(start + i*trajectory_dim_in + 6, speed);
//
//         unsigned int styles_dim = controller->get_styles_dim();
//         unsigned int style_input_slot = start
//                 + i*trajectory_dim_in
// //                 + (trajectory_dim_in - styles_dim); //WHATTHEFUCK? HOW IS THIS POSSIBLE? Maybe you meant 7?
//                 + 7;
//
//         for(int j=0; j<styles_dim; j++) {
//             nn->set_input(style_input_slot + j, get_sample(i).get_style(j));
//         }
//     }
//
//     //TODO check this start
//     start += trajectory_dim_in * point_samples;
//
//     Eigen::Matrix4f previousRoot = trajectory->get_point(root_point_index-1)
//                                     .get_transformation();
//     previousRoot(1,3) = 0.0; //For flat terrain
//
//     //Input Previous Bone Positions / Velocities
// //     unsigned int bones_n = this->skeleton->bones_n();
// //     for(int i=0; i<bones_n; i++) {
// //         Vector3f pos = positions[i].get_relative_position_to(previousRoot);
// //         Vector3f forward = forwards[i].get_relative_direction_to(previousRoot);
// //         Vector3f up = ups[i].get_relative_direction_to(previousRoot);
// //         Vector3f vel = velocities[i].get_relative_direction_to(previousRoot);
// //         nn->set_input(start + i*joint_dim_in + 0, pos.x);
// //         nn->set_input(start + i*joint_dim_in + 1, pos.y);
// //         nn->set_input(start + i*joint_dim_in + 2, pos.z);
// //         nn->set_input(start + i*joint_dim_in + 3, forward.x);
// //         nn->set_input(start + i*joint_dim_in + 4, forward.y);
// //         nn->set_input(start + i*joint_dim_in + 5, forward.z);
// //         nn->set_input(start + i*joint_dim_in + 6, up.x);
// //         nn->set_input(start + i*joint_dim_in + 7, up.y);
// //         nn->set_input(start + i*joint_dim_in + 8, up.z);
// //         nn->set_input(start + i*joint_dim_in + 9, vel.x);
// //         nn->set_input(start + i*joint_dim_in + 10, vel.y);
// //         nn->set_input(start + i*joint_dim_in + 11, vel.z);
// //     }
// //     start += joint_dim_in * bones_n;
//
//     float rest = std::pow(1.0f - trajectory->get_point(root_point_index)
//         .get_style(0), 0.25f);
//     reinterpret_cast<animation::nn::PFNNModel*>(nn->get_model())->set_damping(1.0 - (rest * 0.9 + 0.1));
}

float TimedGenerator::update_network()
{
    auto beginning = clock_::now();
    anim_controller->update();

    double delta_time = std::chrono::duration_cast<second_>
            (clock_::now() - beginning).count();

    return delta_time * 1000.0;
}

const float* TimedGenerator::get_outputs() const
{
    //TODO get real output from fake bones!
    static const float output[10] = {0,1,2,3,4,5,6,7,8,9};
    return output;
//     throw std::runtime_error( "TimedGenerator::get_outputs() is unsupported" );
}

