#include "gdexample.h"
#include <iostream>

using namespace godot;

void GDExample::_register_methods()
{
    register_method((char *)"_process", &GDExample::_process);
}

GDExample::GDExample()
    : time_passed(0)
{
    std::cout << "GDExample init" << std::endl;
}

GDExample::~GDExample()
{}

void GDExample::_process(float delta)
{
    time_passed += delta;

    Vector2 new_position = Vector2(
        10.0 + (10.0 * sin(time_passed * 2.0)), 
        10.0 + (10.0 * cos(time_passed * 1.5))
    );

    owner->set_position(new_position);
}
