#pragma once

#include <Godot.hpp>
#include <Skeleton.hpp>

class ProceduralMovingSkeleton: public godot::GodotScript<godot::Skeleton>
{
    GODOT_CLASS(ProceduralMovingSkeleton)

private:
    float time_passed;
    int bone_id;

public:
    static void _register_methods();

    ProceduralMovingSkeleton();
    ~ProceduralMovingSkeleton();

    void _process(float delta);
};
