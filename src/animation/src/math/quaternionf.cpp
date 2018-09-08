#include "quaternionf.h"

Quaternionf Quaternionf::FromEuler(float roll, float pitch, float yaw)
{
    Quaternionf q(
        Eigen::AngleAxisf(roll, Eigen::Vector3f::UnitX())
        * Eigen::AngleAxisf(pitch, Eigen::Vector3f::UnitY())
        * Eigen::AngleAxisf(yaw, Eigen::Vector3f::UnitZ())
    );

    return q;
}

Quaternionf Quaternionf::AngleAxis(float angle, const Eigen::Vector3f &axis)
{
    return Quaternionf(Eigen::AngleAxisf(angle, axis));
}

Vector3f Quaternionf::operator*(const Eigen::Vector3f& rhs) const
{
    return Vector3f(Eigen::Quaternionf::operator*(rhs));
//     throw std::runtime_error( "Quaternionf::operator*(Vector3f) is unsupported" );
}

Vector3f Quaternionf::get_right() const
{
    return this->operator*(Vector3f::Right());
}

Vector3f Quaternionf::get_up() const
{
    return this->operator*(Vector3f::Up());
}

Vector3f Quaternionf::get_forward() const
{
    return this->operator*(Vector3f::Forward());
}
