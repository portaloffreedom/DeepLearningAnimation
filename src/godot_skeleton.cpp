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
    return Vector3f(tr.origin.x, tr.origin.z, tr.origin.y);//change from right to left-handed
}

Vector3f GodotSkeleton::GodotBone::get_transform_forward() const
{
    godot::Transform tr = skeleton.get_bone_pose(index);
    tr.set_origin(godot::Vector3());
//    godot::Vector3 forward = tr.xform(godot::Vector3(0,0,1));
    godot::Vector3 forward = tr.xform_inv(godot::Vector3(0,0,1));
    return Vector3f(forward.x, forward.z, forward.y); //change from right to left-handed
//    return Vector3f(0, 1, 0);
}

Vector3f GodotSkeleton::GodotBone::get_transform_up() const
{
//    godot::Transform tr = skeleton.get_bone_pose(index);
//    godot::Quat quat(tr.basis);
//    godot::Vector3 axis;
//    real_t angle;
//    //change from right to left-handed
//    quat.get_axis_and_angle(axis, angle);
//    return Vector3f(axis.x, axis.z, axis.y);

//    godot::Transform tr = skeleton.get_bone_pose(index);
//    tr.set_origin(godot::Vector3());
////    godot::Vector3 up = tr.xform(godot::Vector3(0,1,0));
//    godot::Vector3 up = tr.xform_inv(godot::Vector3(0,1,0));
//    return Vector3f(up.x, up.z, up.y); //change from right to left-handed

    return Vector3f(0, 0, 1);
}

void GodotSkeleton::GodotBone::set_transform_position(const Vector3f &position)
{
    godot::Transform tr = skeleton.get_bone_pose(index);
    //change from left to right-handed
    tr.set_origin(godot::Vector3(position.x(), position.z(), position.y()));
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
    //change from left to right-handed
    godot::Vector3 g_direction(direction.x(), direction.z(), direction.y());
    godot::Vector3 g_axis(axis.x(), axis.z(), axis.y());
    godot::Transform tr = skeleton.get_bone_pose(index);

//    godot::Transform rotation_transform = tr.looking_at(g_direction, g_axis);
//    auto this_rotation = godot::Quat(rotation_transform.basis);
//    skeleton.set_bone_pose(index, godot::Transform(this_rotation,tr.origin));

    godot::Transform new_transform;
    new_transform.set_look_at(godot::Vector3(0,0,0), g_direction, g_axis);

    tr.basis = new_transform.basis;
    skeleton.set_bone_pose(index, tr);
}
