#ifndef ANIMATION_NN_PFNNMODEL_H
#define ANIMATION_NN_PFNNMODEL_H

#include "animation/nn/models/model.h"

namespace animation {
namespace nn {

/**
 * @todo write docs
 */
class PFNNModel : public Model
{
public:
    PFNNModel(unsigned int x_dim,
              unsigned int h_dim,
              unsigned int y_dim,
              unsigned int styles_dim,
              unsigned int trajectory_samples_n,
              unsigned int phase_index,
              float damping = 0);

    /**
     * Load the matrices for the model from file in memory
     *
     * @throws FileError if error occours in reading files
     */
    virtual void load_parameters(const std::string &folder_path) override;

    virtual void predict(const std::unique_ptr<Tensor> &X, std::unique_ptr<Tensor> &Y) override;

    void set_damping(float damping) {
        this->damping = damping;
    }

private:
    static const unsigned int TENSORS_N = 50;

    const unsigned int h_dim;
    const unsigned int phase_index;
    double phase;
    double damping;


    std::unique_ptr<Tensor> Xmean, Xstd, Ymean, Ystd;
    std::unique_ptr<Tensor> W0[TENSORS_N], W1[TENSORS_N], W2[TENSORS_N], b0[TENSORS_N], b1[TENSORS_N], b2[TENSORS_N];
};

}
}

#endif // ANIMATION_NN_PFNNMODEL_H
