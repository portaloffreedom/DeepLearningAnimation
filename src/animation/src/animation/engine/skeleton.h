#ifndef ANIMATION_ENGINE_SKELETON_H
#define ANIMATION_ENGINE_SKELETON_H

#include <string>
#include "math/vector3f.h"
#include "math/quaternionf.h"

namespace animation {
namespace engine {

/**
 * @todo write docs
 */
class Skeleton
{
public:

    class Bone {
    public:
        virtual const std::string &name() const = 0;

        virtual Vector3f get_transform_position() const = 0;
        virtual Vector3f get_transform_forward() const = 0;
        virtual Vector3f get_transform_up() const = 0;
        const Vector3f& get_velocity() const
        { return velocity; }

        virtual void set_transform_position(const Vector3f &position) = 0;
        virtual void set_rotation(const Quaternionf &rotation) = 0;
        virtual void set_rotation(const Vector3f &direction, const Vector3f &axis) = 0;
        void set_velocity(const Vector3f &velocity)
        { this->velocity = velocity; }

    private:
        Vector3f velocity;
    };

    virtual unsigned int bones_n() const = 0;

    virtual Bone& bone(unsigned int index) = 0;
    virtual const Bone& bone(unsigned int index) const = 0;

};

}
}

#endif // ANIMATION_ENGINE_SKELETON_H
