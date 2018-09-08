#ifndef QUATERNION_H
#define QUATERNION_H

#include <Eigen/Dense>
#include "vector3f.h"
class Vector3f;

/**
 * @todo write docs
 */
class Quaternionf : public Eigen::Quaternionf
{
public:
    Quaternionf()
        : Eigen::Quaternionf()
    {}

    explicit Quaternionf(const Eigen::Quaternionf &other)
        : Eigen::Quaternionf(other)
    {}

    explicit Quaternionf(const Eigen::AngleAxisf &axis)
        : Eigen::Quaternionf(axis)
    {}

    static Quaternionf FromEuler(float roll, float picth, float yaw);
    /**
     * Creates Quaterion from axis and rotation angle
     * @param angle
     * @param axis The axis vector must be normalized.
     */
    static Quaternionf AngleAxis(float angle, const Eigen::Vector3f &axis);

    Vector3f operator*(const Eigen::Vector3f &rhs) const;

    Vector3f get_right() const;
    Vector3f get_up() const;
    Vector3f get_forward() const;
};

#endif // QUATERNION_H
