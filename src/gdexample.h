#ifndef GDEXAMPLE_H
#define GDEXAMPLE_H

#include <Godot.hpp>
#include <Sprite.hpp>

namespace godot {

class GDExample: public godot::GodotScript<Sprite>
{
    GODOT_CLASS(GDExample)
    
private:
    float time_passed;

public:
    static void _register_methods();

    GDExample();
    ~GDExample();

    void _process(float delta);
};

}

#endif // GDEXAMPLE_H
