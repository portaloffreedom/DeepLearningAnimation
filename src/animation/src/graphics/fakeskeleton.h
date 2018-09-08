#ifndef FAKESKELETON_H
#define FAKESKELETON_H

#include <animation/engine/skeleton.h>

/**
 * @todo write docs
 */
class FakeSkeleton : public animation::engine::Skeleton
{
public:
    /**
     * Default constructor
     */
    FakeSkeleton() {}

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
    class FakeBone : public animation::engine::Skeleton::Bone {
    public:
        FakeBone() {}
        virtual const std::string &name() const override;
        virtual Vector3f get_transform_position() const override;
        virtual Vector3f get_transform_forward() const override;
        virtual Vector3f get_transform_up() const override;
        virtual void set_transform_position(const Vector3f &position) override;
        virtual void set_rotation(const Quaternionf &rotation) override;
    };

    FakeBone fake_bone;
};

#endif // FAKESKELETON_H
