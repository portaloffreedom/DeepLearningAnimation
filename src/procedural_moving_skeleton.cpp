#include "procedural_moving_skeleton.h"
#include <iostream>

#include "godot_skeleton.h"
#include "godot_terrain.h"
#include "animation/nn/models/pfnnmodel.h"

void ProceduralMovingSkeleton::_register_methods()
{
    godot::register_method("_process", &ProceduralMovingSkeleton::_process);
    godot::register_method("_ready", &ProceduralMovingSkeleton::_ready);
}

ProceduralMovingSkeleton::ProceduralMovingSkeleton()
    : anim_controller(nullptr)
    , time_passed(0)
    , bone_id(-1)
{}

ProceduralMovingSkeleton::~ProceduralMovingSkeleton()
{
    delete anim_controller;
}

void ProceduralMovingSkeleton::_ready()
{
    std::unique_ptr<animation::engine::Skeleton> skeleton(
            new GodotSkeleton(*owner));
    std::shared_ptr<animation::engine::Terrain> terrain(
            new GodotTerrain());

    auto pfnn_model =
            new animation::nn::PFNNModel(468, 512, 376, 4, 10, 375);
    pfnn_model->load_parameters( ASSETS_FOLDER "/adam/nn/" );

    std::shared_ptr<animation::nn::Model> model(pfnn_model);
    anim_controller = new animation::Controller(
            skeleton,
            terrain,
            model
    );

    std::cout << "ProceduralMovingSkeleton init" << std::endl;
}

void ProceduralMovingSkeleton::_process(float delta)
{
    time_passed += delta;

    godot::Transform transform;
    transform.translate(0, time_passed * 10, 0);

    if (bone_id < 0) {
        bone_id = owner->find_bone("neck01");
    }
    // owner->set_bone_pose(bone_id, transform);

    anim_controller->update();
}
