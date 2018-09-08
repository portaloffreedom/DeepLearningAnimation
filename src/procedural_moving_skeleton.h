#pragma once

#include <Godot.hpp>
#include <Skeleton.hpp>
#include "animation/controller.h"

class ProceduralMovingSkeleton: public godot::GodotScript<godot::Skeleton>
{
    GODOT_CLASS(ProceduralMovingSkeleton)

public:
    static void _register_methods();

    ProceduralMovingSkeleton();
    ~ProceduralMovingSkeleton();

    void _ready();
    void _process(float delta);

private:
    animation::Controller* anim_controller;
    double time_passed;
    int64_t bone_id;
};
