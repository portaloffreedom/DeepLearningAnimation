#include "matrix4f.h"

Vector3f Matrix4f::get_position() const
{
    return Vector3f(
               this->coeffRef(0,3),
               this->coeffRef(1,3),
               this->coeffRef(2,3)
           );
}

void Matrix4f::set_rotation(const Quaternionf& rotation)
{
    Eigen::Vector3f right = rotation.get_right();
    Eigen::Vector3f up = rotation.get_up();
    Eigen::Vector3f forward = rotation.get_forward();

    this->coeffRef(0,0) = right.x();
    this->coeffRef(1,0) = right.y();
    this->coeffRef(2,0) = right.z();
    this->coeffRef(0,1) = up.x();
    this->coeffRef(1,1) = up.y();
    this->coeffRef(2,1) = up.z();
    this->coeffRef(0,2) = forward.x();
    this->coeffRef(1,2) = forward.y();
    this->coeffRef(2,2) = forward.z();
}

Quaternionf Matrix4f::get_rotation() const {
    throw std::runtime_error( "Matrix4f::get_rotation() is unsupported" );
//         return Quaternion.LookRotation(matrix.GetColumn(2).normalized, matrix.GetColumn(1).normalized);
}

Vector3f Matrix4f::get_right() const
{
    return Vector3f(
               this->coeffRef(0,0),
               this->coeffRef(1,0),
               this->coeffRef(2,0)
           );
}

Vector3f Matrix4f::get_up() const
{
    return Vector3f(
               this->coeffRef(0,1),
               this->coeffRef(1,1),
               this->coeffRef(2,1)
           );
}

Vector3f Matrix4f::get_forward() const
{
    return Vector3f(
               this->coeffRef(0,2),
               this->coeffRef(1,2),
               this->coeffRef(2,2)
           );
}

Vector3f Matrix4f::multiply_point(const Vector3f& point) const
{

    Vector3f res;
    res.x() = this->coeffRef(0,0) * point.x()
              + this->coeffRef(0,1) * point.y()
              + this->coeffRef(0,2) * point.z()
              + this->coeffRef(0,3);
    res.y() = this->coeffRef(1,0) * point.x()
              + this->coeffRef(1,1) * point.y()
              + this->coeffRef(1,2) * point.z()
              + this->coeffRef(1,3);
    res.z() = this->coeffRef(2,0) * point.x()
              + this->coeffRef(2,1) * point.y()
              + this->coeffRef(2,2) * point.z()
              + this->coeffRef(2,3);
    float w = this->coeffRef(3,0) * point.x()
              + this->coeffRef(3,1) * point.y()
              + this->coeffRef(3,2) * point.z()
              + this->coeffRef(3,3);

    w = 1.0 / w;
    res.x() *= w;
    res.y() *= w;
    res.z() *= w;
    return res;

//     Eigen::Vector4f point4(
//         point.x(),
//         point.y(),
//         point.z(),
//         0
//     );
//
//     Eigen::Vector4f res = (*this) * point4;
//     float w = res(3);
//     Vector3f ret(
//         res.x(),
//         res.y(),
//         res.z()
//     );
//
//     ret /= w;
//     return ret;
}

Vector3f Matrix4f::multiply_vector(const Vector3f& vector) const
{
    Vector3f res;
    res.x() = this->coeffRef(0,0) * vector.x()
              + this->coeffRef(0,1) * vector.y()
              + this->coeffRef(0,2) * vector.z();
    res.y() = this->coeffRef(1,0) * vector.x()
              + this->coeffRef(1,1) * vector.y()
              + this->coeffRef(1,2) * vector.z();
    res.z() = this->coeffRef(2,0) * vector.x()
              + this->coeffRef(2,1) * vector.y()
              + this->coeffRef(2,2) * vector.z();
    return res;
}
