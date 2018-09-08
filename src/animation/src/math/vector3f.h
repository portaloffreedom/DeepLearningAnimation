#ifndef VECTOR3F_H
#define VECTOR3F_H

#include <Eigen/Dense>
#include "matrix4f.h"
class Matrix4f;

/**
 * @todo write docs
 */
class Vector3f : public Eigen::Vector3f
{
public:
    /**
     * Default constructor
     */
    Vector3f()
        : Eigen::Vector3f() {}

    /**
     * Copy Constructor
     */
    Vector3f(const Eigen::Vector3f& other)
        : Eigen::Vector3f(other) {}

    Vector3f(float x, float y, float z)
        : Eigen::Vector3f(x, y, z) {}

    static Vector3f Right()   { return Vector3f(1, 0, 0); }
    static Vector3f Up()      { return Vector3f(0, 1, 0); }
    static Vector3f Forward() { return Vector3f(0, 0, 1); }

    Vector3f get_relative_position_from(const Matrix4f &from) const;
    Vector3f get_relative_position_to(const Matrix4f &to) const;
    Vector3f get_relative_direction_from(const Matrix4f &from) const;
    Vector3f get_relative_direction_to(const Matrix4f &to) const;

    enum MirrorAxis {
        RIGHT,
        UP,
        FORWARD
    };

    Vector3f get_mirror(MirrorAxis axis) const {
        Vector3f vector(*this);
        switch (axis) {
            case RIGHT:
                vector.x() *= -1.0; break;
            case UP:
                vector.y() *= -1.0; break;
            case FORWARD:
                vector.z() *= -1.0; break;
        }
        return vector;
    }

};

#endif // VECTOR3F_H
