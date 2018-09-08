#include "fakeskeleton.h"

const animation::engine::Skeleton::Bone& FakeSkeleton::bone(unsigned int index) const
{
    assert(index < bones_n());
    return fake_bone;
}

animation::engine::Skeleton::Bone& FakeSkeleton::bone(unsigned int index)
{
    assert(index < bones_n());
    return fake_bone;
}

unsigned int FakeSkeleton::bones_n() const
{
    return 1;
}

const std::string & FakeSkeleton::FakeBone::name() const
{
    static const std::string fake_bone_name("fake_bone");
    return fake_bone_name;
}

Vector3f FakeSkeleton::FakeBone::get_transform_forward() const
{
    return Vector3f(0,0,0);
}

Vector3f FakeSkeleton::FakeBone::get_transform_position() const
{
    return Vector3f(0,0,0);
}

Vector3f FakeSkeleton::FakeBone::get_transform_up() const
{
    return Vector3f(0,0,0);
}

void FakeSkeleton::FakeBone::set_rotation(const Quaternionf& rotation)
{
    //TODO print in chart
}

void FakeSkeleton::FakeBone::set_transform_position(const Vector3f& position)
{
    //TODO print in chart
}
