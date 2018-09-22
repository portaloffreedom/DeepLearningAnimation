#include "vector3f.h"

Vector3f Vector3f::Lerp(const Eigen::Vector3f &a, const Eigen::Vector3f &b, float ratio)
{
    Eigen::Vector3f scaled_a = a*ratio;
    Eigen::Vector3f scaled_b = b*(1.f-ratio);
    return Vector3f(scaled_a + scaled_b);
}

Vector3f Vector3f::get_relative_position_from(const Matrix4f& from) const
{
    return from.multiply_point(*this);
}

Vector3f Vector3f::get_relative_position_to(const Matrix4f &to) const {
    return Matrix4f(to.inverse()).multiply_point(*this);
}

Vector3f Vector3f::get_relative_direction_from(const Matrix4f &from) const {
    return from.multiply_vector(*this);
}

Vector3f Vector3f::get_relative_direction_to(const Matrix4f &to) const {
    return Matrix4f(to.inverse()).multiply_vector(*this);
}

Vector3f Vector3f::normalized() const
{
    return Vector3f(Eigen::Vector3f::normalized());
}