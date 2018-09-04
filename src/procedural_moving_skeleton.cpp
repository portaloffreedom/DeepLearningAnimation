#include "procedural_moving_skeleton.h"
#include <iostream>

void ProceduralMovingSkeleton::_register_methods()
{
    godot::register_method((char *)"_process", &ProceduralMovingSkeleton::_process);
}

ProceduralMovingSkeleton::ProceduralMovingSkeleton()
    : time_passed(0)
    , bone_id(-1)
{
    std::cout << "ProceduralMovingSkeleton init" << std::endl;
}

ProceduralMovingSkeleton::~ProceduralMovingSkeleton()
{}

void ProceduralMovingSkeleton::_process(float delta)
{
    time_passed += delta;

    godot::Transform transform;
    transform.translate(0, time_passed * 10, 0);

    if (bone_id < 0) {
        bone_id = owner->find_bone("neck01");
    }
    owner->set_bone_pose(bone_id, transform); 
}