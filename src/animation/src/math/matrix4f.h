#ifndef MATRIX4F_H
#define MATRIX4F_H

#include <Eigen/Dense>
#include "vector3f.h"
class Vector3f;
#include "quaternionf.h"
class Quaternionf;

class Matrix4f : public Eigen::Matrix4f
{
public:
    Matrix4f() : Eigen::Matrix4f() {}
    Matrix4f(const Eigen::Matrix4f& other) : Eigen::Matrix4f(other) {}

    void set_position(const Eigen::Vector3f &position)
    {
        this->coeffRef(0,3) = position.x();
        this->coeffRef(1,3) = position.y();
        this->coeffRef(2,3) = position.z();
    }

    Vector3f get_position() const;

    void set_rotation(const Quaternionf &rotation);
    Eigen::Quaternionf get_rotation() const
    {
        throw std::runtime_error( "Matrix4f::get_rotation() is unsupported" );
//         return Quaternion.LookRotation(matrix.GetColumn(2).normalized, matrix.GetColumn(1).normalized);
    }

    Vector3f get_right() const;
    Vector3f get_up() const;
    Vector3f get_forward() const;

    Vector3f multiply_point(const Vector3f &point) const;
    Vector3f multiply_vector(const Vector3f &vector) const;

};

#endif // MATRIX4F_H
