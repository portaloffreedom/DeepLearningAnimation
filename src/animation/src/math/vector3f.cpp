#include "vector3f.h"

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
