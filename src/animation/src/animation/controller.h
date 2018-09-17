#ifndef ANIMATION_CONTROLLER_H
#define ANIMATION_CONTROLLER_H

#include <memory>
#include "animation/engine/skeleton.h"
#include "animation/engine/terrain.h"
#include "animation/nn/models/model.h"
#include "animation/trajectory/trajectory.h"

namespace animation {

/**
 * @todo write docs
 */
class Controller
{
public:
    /**
     * Default constructor
     */
    Controller(std::unique_ptr<engine::Skeleton> &skeleton,
               std::shared_ptr<engine::Terrain> &terrain,
               std::shared_ptr<nn::Model> &model)
        : skeleton(std::move(skeleton))
        , terrain(terrain)
        , model(model)
        , trajectory(new Trajectory(Trajectory::POINTS, model->get_styles_dim()))
    {
        X.reset(new nn::Tensor(this->model->x_dim(), 1));
        Y.reset(new nn::Tensor(this->model->y_dim(), 1));
        trajectory->post_process(*terrain); //make sure trajectory is processed
    }

    void update();

    unsigned int get_input_dim() const;
    unsigned int get_output_dim() const;

private:
    float set_inputs();
    void update_model();
    void output_to_skeleton(float rest);

    void set_input(unsigned int index, float value);
    float get_output(unsigned int index) const;

    const Trajectory::Point& get_sample(unsigned int index) const;
    const Trajectory::Point& get_previous_sample(unsigned int index) const;
    const Trajectory::Point& get_next_sample(unsigned int index) const;

private:
    std::unique_ptr<engine::Skeleton> skeleton;
    std::shared_ptr<engine::Terrain> terrain;
    std::shared_ptr<nn::Model> model;

    std::unique_ptr<Trajectory> trajectory;
    std::unique_ptr<nn::Tensor> X, Y;
};

}

#endif // ANIMATION_CONTROLLER_H
