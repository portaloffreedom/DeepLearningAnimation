#ifndef ANIMATION_NN_NEURALNETWORK_H
#define ANIMATION_NN_NEURALNETWORK_H

#include <chrono>
#include "types.h"
#include "animation/nn/models/model.h"

namespace animation {
namespace nn {

/**
 * @todo write docs
 */
class NeuralNetwork
{
public:

    NeuralNetwork();
    virtual ~NeuralNetwork();

    /**
     * updates the neural NeuralNetwork
     *
     * @returns how many ms took to complete the update
     */
    virtual float update();

    virtual const float *const get_output() const {
        return this->output;
    }

    virtual const unsigned int output_size() const {
        return this->model->y_dim(); //NeuralNetwork::OUTPUT_SIZE;
    }

    virtual void set_input(unsigned int index, float value) {
        throw std::runtime_error( "NeuralNetwork::set_input() is unsupported" );
//         this->model->set_input(index, value);
    }

    Model* get_model() const {
        return model.get();
    }

private:
    static const unsigned int OUTPUT_SIZE = 8;


    float* output;
    std::unique_ptr<Model> model;

    // Delta time calculation required bits
    typedef std::chrono::high_resolution_clock clock_;
    typedef std::chrono::duration<double, std::ratio<1> > second_;
};

}
}

#endif // ANIMATION_NN_NEURALNETWORK_H
