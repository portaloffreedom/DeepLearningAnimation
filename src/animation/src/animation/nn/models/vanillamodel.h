#ifndef ANIMATION_NN_VANILLAMODEL_H
#define ANIMATION_NN_VANILLAMODEL_H

#include "animation/nn/models/model.h"
#include "animation/nn/tensor.h"

namespace animation {
namespace nn {

/**
 * @todo write docs
 */
class VanillaModel : public Model
{
public:
    VanillaModel(unsigned int x_dim, unsigned int h_dim, unsigned int y_dim, unsigned int styles_dim);

    virtual void load_parameters(const std::string &folder_path) override;

    virtual void predict(const std::unique_ptr<Tensor> &X, std::unique_ptr<Tensor> &Y) override;
    
private:
    const unsigned int h_dim;

    std::unique_ptr<Tensor> Xmean, Xstd, Ymean, Ystd;
    std::unique_ptr<Tensor> W0, W1, W2, b0, b1, b2;
    std::unique_ptr<Tensor> X, Y;
};

}
}

#endif // ANIMATION_NN_VANILLAMODEL_H
