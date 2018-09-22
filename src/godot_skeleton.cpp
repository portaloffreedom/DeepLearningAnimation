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
    godot::Transform tr = skeleton.get_bone_pose(index);
    return Vector3f(tr.origin.x, tr.origin.y, tr.origin.z);
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
    tr.set_origin(godot::Vector3(position.x(), position.y(), position.z()));
    skeleton.set_bone_pose(index, tr);
}

void GodotSkeleton::GodotBone::set_rotation(const Quaternionf &rotation)
{
    godot::Transform tr = skeleton.get_bone_pose(index);
    godot::Quat quat = godot::Quat(rotation.x(), rotation.y(), rotation.z(), rotation.w());
    tr.basis = godot::Basis(quat);
    skeleton.set_bone_pose(index, tr);
}

void GodotSkeleton::GodotBone::set_rotation(const Vector3f &direction, const Vector3f &axis)
{
    godot::Transform tr = skeleton.get_bone_pose(index);
//    var lookDir = get_node(lookTarget).get_transform().origin - t.origin
    godot::Vector3 g_direction(direction.x(), direction.y(), direction.z());
    godot::Vector3 g_axis(axis.x(), axis.y(), axis.z());
    godot::Transform rotation_transform = tr.looking_at(g_direction, g_axis);
    auto this_rotation = godot::Quat(rotation_transform.basis);
    skeleton.set_bone_pose(index, godot::Transform(this_rotation,tr.origin));
}
