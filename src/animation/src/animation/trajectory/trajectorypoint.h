#ifndef ANIMATION_TRAJECTORYPOINT_H
#define ANIMATION_TRAJECTORYPOINT_H

#include <vector>
#include <Eigen/Dense>
#include "trajectory.h"
#include "animation/engine/terrain.h"
#include "math/matrix4f.h"
class Matrix4f;
#include "math/quaternionf.h"
class Quaternionf;

namespace animation {
class Trajectory;

/**
 * @todo write docs
 */
class TrajectoryPoint
{
public:

    TrajectoryPoint(int index, int styles)
        : index(index)
        , transformation(Eigen::Matrix4f::Identity())
        , velocity(Eigen::Vector3f::Zero())
        , left_sample(Eigen::Vector3f::Zero())
        , right_sample(Eigen::Vector3f::Zero())
        , slope(0)
        , _styles(styles)
    {}

//  VALUE IS CONST, SO FUNCTION IS DISABLED
//     void set_index(int index) {
//         this->index = index;
//     }

    int get_index() const {
        return index;
    }

    void set_transformation(const Eigen::Matrix4f &matrix) {
        transformation = matrix;
    }

    const Matrix4f& get_transformation() const {
        return transformation;
    }

    void set_position(const Eigen::Vector3f &position) {
        transformation.set_position(position);
    }

    Vector3f get_position() const {
        return transformation.get_position();
    }

    void set_rotation(const Quaternionf &rotation) {
        transformation.set_rotation(rotation);
    }

    const Eigen::Quaternionf get_rotation() const {
        return transformation.get_rotation();
    }

    void set_direction(const Eigen::Vector3f &direction) {
        Eigen::Quaternionf rotation = Quaternionf::FromTwoVectors(direction, Vector3f::Up());
        set_rotation(Quaternionf(rotation));
//         throw std::runtime_error( "TrajectoryPoint::set_direction() is unsupported" );
//         set_rotation(Eigen::Quaternionf.LookRotation(direction == Eigen::Vector3f.zero ? Eigen::Vector3f.forward : direction, Eigen::Vector3f.up));
    }

    Vector3f get_direction() const {
        return transformation.get_forward();
    }

    void set_velocity(const Vector3f &velocity) {
        this->velocity = velocity;
    }

    void set_velocity(const Eigen::Vector3f &velocity) {
        this->velocity = Vector3f(velocity);
    }

    const Vector3f& get_velocity() const {
        return velocity;
    }

    void set_speed(float speed) {
        this->speed = speed;
    }

    float get_speed() const {
        return speed;
    }

    void set_left_sample(const Eigen::Vector3f &position) {
        left_sample = position;
    }

    const Eigen::Vector3f &get_left_sample() const {
        return left_sample;
    }

    void set_right_sample(const Eigen::Vector3f &position) {
        right_sample = position;
    }

    const Eigen::Vector3f &get_right_sample() const {
        return right_sample;
    }

    void set_slope(float slope) {
        this->slope = slope;
    }

    float get_slope() const {
        return slope;
    }

    void post_process(const engine::Terrain &terrain);

    float get_style(unsigned int index) const {
        assert(index < _styles.size());
        return _styles[index];
    }

    float& style(unsigned int index) {
        assert(index < _styles.size());
        return _styles[index];
    }

private:
    const int index;
    Matrix4f transformation;
    Vector3f velocity;
    float speed;
    Eigen::Vector3f left_sample;
    Eigen::Vector3f right_sample;
    float slope;
    std::vector<float> _styles;
};

}

#endif // ANIMATION_TRAJECTORYPOINT_H
