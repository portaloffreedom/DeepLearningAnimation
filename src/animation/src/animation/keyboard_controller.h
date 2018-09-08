#ifndef ANIMATION_KEYBOARD_CONTROLLER_H
#define ANIMATION_KEYBOARD_CONTROLLER_H

#include <vector>
#include <string>
#include <memory>
#include <Eigen/Dense>

namespace animation {

/**
 * @todo write docs
 */
class KeyboardController
{
public:
    KeyboardController(unsigned int style_count);
    KeyboardController(const std::vector< std::string >& style_names);

    /**
     * Destructor
     */
    ~KeyboardController();

    float query_turn();
    Eigen::Vector3f& query_move();

    std::vector<float>& get_style();
    unsigned int get_styles_dim() const
    { return styles.size(); }

    class Style {
    public:
        Style(const std::string& name);

        bool query();
        void set_key_count(int count);
        void add_multiplier();
        void remove_multiplier();

    protected:

        class Multiplier {
        public:
            char key;
            float value;
        };

        std::string name;
        float bias; // 1f;
        float transition; // 0.1f;
        std::vector<char> keys;
        std::vector<bool> negations;
        std::vector<Multiplier> multipliers;
    };

private:
    std::vector<std::unique_ptr<Style>> styles;
    std::vector<float> output_styles;
    Eigen::Vector3f    output_move;

    float forward;      // from -1.0 to 1.0
    float right_strafe; // from -1.0 to 1.0
    float right_turn;   // from -1.0 to 1.0

};

}

#endif // ANIMATION_KEYBOARD_CONTROLLER_H
