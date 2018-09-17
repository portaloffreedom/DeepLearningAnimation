#include "godot_skeleton.h"

GodotSkeleton::GodotSkeleton(godot::Skeleton &godot_skeleton)
    : _skeleton(godot_skeleton)
    , bones()
{
    unsigned int bones_n = this->bones_n();
    bones.reserve(bones_n);

    for(unsigned int i=0; i< bones_n; i++)
    {
        bones.push_back(GodotBone(i, _skeleton));
    }
}

const animation::engine::Skeleton::Bone& GodotSkeleton::bone(unsigned int index) const
{
    assert(index < bones_n());
    return bones[index];
}

animation::engine::Skeleton::Bone& GodotSkeleton::bone(unsigned int index)
{
    assert(index < bones_n());
    return bones[index];
}

unsigned int GodotSkeleton::bones_n() const
{
    // The original model had only 29 bones, we have 137
    return 29;//_skeleton.get_bone_count();
}

// -- GODOT BONE -- //

GodotSkeleton::GodotBone::GodotBone(unsigned int index, godot::Skeleton &skeleton)
    : index(index)
    , _name(skeleton.get_bone_name(index).alloc_c_string())
    , skeleton(skeleton)
{}

const std::string& GodotSkeleton::GodotBone::name() const
{
    return _name;
}

Vector3f GodotSkeleton::GodotBone::get_transform_position() const
{
    //TODO skeleton.get_transform(index).
    return Vector3f(0,0,0);
}

Vector3f GodotSkeleton::GodotBone::get_transform_forward() const
{
    //TODO skeleton.get_transform(index).
    return Vector3f(0,0,0);
}

Vector3f GodotSkeleton::GodotBone::get_transform_up() const
{
    return Vector3f(0,0,0);
}

void GodotSkeleton::GodotBone::set_transform_position(const Vector3f &position)
{
    godot::Transform tr = skeleton.get_bone_pose(index);
    tr.translate(godot::Vector3(position.x(), position.y(), position.z()));
    skeleton.set_bone_pose(index, tr);
}

void GodotSkeleton::GodotBone::set_rotation(const Quaternionf &rotation)
{
    godot::Transform tr = skeleton.get_bone_pose(index);
    tr.rotate(godot::Vector3(rotation.x(), rotation.y(), rotation.z()), rotation.w());
    skeleton.set_bone_pose(index, tr);
}
