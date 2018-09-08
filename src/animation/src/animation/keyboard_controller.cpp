#include "keyboard_controller.h"

using namespace animation;

KeyboardController::KeyboardController(unsigned int style_count)
    : styles(0)
    , output_styles(style_count)
    , output_move(0,0,0)
{
    styles.reserve(style_count);
    for (int i=0; i<style_count; i++) {
        styles.push_back(std::unique_ptr<Style>(new Style("")));
    }
}

KeyboardController::KeyboardController(const std::vector<std::string> &style_names)
    : styles(0)
    , output_styles(style_names.size())
    , output_move(0,0,0)
{
    styles.reserve(style_names.size());
    for (const std::string& style_name: style_names) {
        styles.push_back(std::unique_ptr<Style>(new Style(style_name)));
    }
}

KeyboardController::~KeyboardController()
{

}

std::vector<float> & KeyboardController::get_style()
{
    output_styles.resize(styles.size());
    assert(output_styles.size() == styles.size());

    for(int i=0; i<styles.size(); i++) {
        output_styles[i] = styles[i]->query() ? 1.0 : 0.0;
    }
    return output_styles;
}

Eigen::Vector3f & KeyboardController::query_move()
{
    output_move(2) = forward;
    //output_move(1) = 0.0
    output_move(0) = right_strafe;

    return output_move;
}

float KeyboardController::query_turn()
{
    return right_turn;
}

KeyboardController::Style::Style(const std::string& name)
    : name(name)
    , bias(1)
    , transition(0.1)
    , keys(0)
    , negations(0)
    , multipliers(0)
{
}

bool KeyboardController::Style::query()
{
    if (keys.size() == 0) {
        return false;
    }

    //TODO query all pressed keys and return true if one is active

    bool active = false;

//     for(int i=0; i<keys.size(); i++) {
//         if(!negations[i]) {
//             if(keys[i] == 0) {
//                 if(!Input.anyKey) {
//                     active = true;
//                 }
//             } else {
//                 if(Input.GetKey(Keys[i])) {
//                     active = true;
//                 }
//             }
//         }
//     }
//
//     for(int i=0; i<Keys.Length; i++) {
//         if(negations[i]) {
//             if(keys[i] == 0) {
//                 if(!Input.anyKey) {
//                     active = false;
//                 }
//             } else {
//                 if(Input.GetKey(Keys[i])) {
//                     active = false;
//                 }
//             }
//         }
//     }

    return active;

}

void KeyboardController::Style::set_key_count(int count)
{
    count = std::max(count, 0);
    if(keys.size() != count) {
        keys.resize(count);
        negations.resize(count);
    }
}

void KeyboardController::Style::add_multiplier()
{
    multipliers.push_back(Multiplier());
}
void KeyboardController::Style::remove_multiplier()
{
    multipliers.pop_back();
}
