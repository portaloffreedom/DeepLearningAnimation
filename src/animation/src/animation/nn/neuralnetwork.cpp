#include "neuralnetwork.h"
#include "animation/nn/models/pfnnmodel.h"

using namespace animation::nn;

NeuralNetwork::NeuralNetwork()
    : output(nullptr)
    , model(new PFNNModel(468, 512, 376, 4, 10, 375)) //ADAM VALUES
{
    auto output_size = this->output_size();

    this->output = new float[output_size];
    for (int i=0; i<output_size; i++) {
        this->output[i] = i/((float)output_size);
    }

    model->load_parameters( ASSETS_FOLDER "/adam/nn/" );
}

NeuralNetwork::~NeuralNetwork()
{
    delete[] this->output;
    this->output = nullptr;
}

float NeuralNetwork::update()
{
    throw std::runtime_error( "NeuralNetwork::update() is unsupported" );
    auto beginning = clock_::now();
//     model->predict();

    auto output_size = this->output_size();
    for (int i=0; i<output_size; i++) {
//         this->output[i] = model->get_output(i);
    }

    double delta_time = std::chrono::duration_cast<second_>
            (clock_::now() - beginning).count();

    return delta_time * 1000.0;
}

