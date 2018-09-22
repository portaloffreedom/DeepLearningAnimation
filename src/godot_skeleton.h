#pragma once
#include <animation/engine/skeleton.h>
#include <Godot.hpp>
#include <Skeleton.hpp>

/**
 * @todo write docs
 */
class GodotSkeleton : public animation::engine::Skeleton
{
public:
    /**
     * Default constructor
     */
    GodotSkeleton(godot::Skeleton &godot_skeleton);

    /**
     * @todo write docs
     *
     * @param index TODO
     * @return TODO
     */
    virtual const animation::engine::Skeleton::Bone& bone(unsigned int index) const override;

    /**
     * @todo write docs
     *
     * @param index TODO
     * @return TODO
     */
    virtual animation::engine::Skeleton::Bone& bone(unsigned int index) override;

    /**
     * @todo write docs
     *
     * @return TODO
     */
    virtual unsigned int bones_n() const override;

private:
    class GodotBone : public animation::engine::Skeleton::Bone {
    private:
        const unsigned int index;
        const std::string _name;
        godot::Skeleton &skeleton;
    public:
        GodotBone(unsigned int index, godot::Skeleton &skeleton);
        virtual const std::string &name() const override;
        virtual Vector3f get_transform_position() const override;
        virtual Vector3f get_transform_forward() const override;
        virtual Vector3f get_transform_up() const override;
        virtual void set_transform_position(const Vector3f &position) override;
        virtual void set_rotation(const Quaternionf &rotation) override;
        virtual void set_rotation(const Vector3f &direction, const Vector3f &axis) override;
    };

    godot::Skeleton &_skeleton;
    std::vector<GodotBone> bones;
};
