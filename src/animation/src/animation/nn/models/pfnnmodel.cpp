#include "pfnnmodel.h"

#include <string>
#include <iostream>
#include <iomanip>
#define _USE_MATH_DEFINES
#include <cmath>

using namespace animation::nn;

//468, 512, 376, 375
PFNNModel::PFNNModel(unsigned int x_dim,
                     unsigned int h_dim,
                     unsigned int y_dim,
                     unsigned int styles_dim,
                     unsigned int trajectory_samples_n,
                     unsigned int phase_index,
                     float damping)
    : Model(x_dim, y_dim, styles_dim, trajectory_samples_n /*TODO read number from file*/)
    , h_dim(h_dim)
    , phase_index(phase_index)
    , phase(0)
    , damping(damping)
{

}

void PFNNModel::load_parameters(const std::string& folder_path)
{
    Xmean.reset(create_tensor(load_binary_matrix(folder_path+"/Xmean.bin", _x_dim, 1), "Xmean"));
    Xstd.reset(create_tensor(load_binary_matrix(folder_path+"/Xstd.bin", _x_dim, 1), "Xstd"));
    Ymean.reset(create_tensor(load_binary_matrix(folder_path+"/Ymean.bin", _y_dim, 1), "Ymean"));
    Ystd.reset(create_tensor(load_binary_matrix(folder_path+"/Ystd.bin", _y_dim, 1), "Ystd"));

    assert(TENSORS_N < 1000);

    for (int i=0; i<TENSORS_N; i++) {
        std::ostringstream i_str_stream;
        i_str_stream << std::setfill('0') << std::setw(3) << i;
        std::string i_str = i_str_stream.str();
        W0[i].reset(create_tensor(load_binary_matrix(folder_path+"/W0_"+i_str+".bin", h_dim, _x_dim), "W0_"+i_str));
        W1[i].reset(create_tensor(load_binary_matrix(folder_path+"/W1_"+i_str+".bin", h_dim, h_dim), "W1_"+i_str));
        W2[i].reset(create_tensor(load_binary_matrix(folder_path+"/W2_"+i_str+".bin", _y_dim, h_dim), "W2_"+i_str));
        b0[i].reset(create_tensor(load_binary_matrix(folder_path+"/b0_"+i_str+".bin", h_dim, 1), "b0_"+i_str));
        b1[i].reset(create_tensor(load_binary_matrix(folder_path+"/b1_"+i_str+".bin", h_dim, 1), "b1_"+i_str));
        b2[i].reset(create_tensor(load_binary_matrix(folder_path+"/b2_"+i_str+".bin", _y_dim, 1), "b2_"+i_str));
    }

    unsigned long memory_used = (
            _x_dim*2 +
            _y_dim*2 +
            50 * ( h_dim*_x_dim + h_dim*h_dim + _y_dim*h_dim + h_dim*2 + _y_dim ) +
            _x_dim +
            _y_dim
        ) * 4 /*syzeof(float)*/;

    std::cout << "LOADED "
     << (int) (memory_used / 1024.0 / 1024.0 + 0.5) // + 0.5 is to properly approximate
     << " MB IN RAM for PFNNModel" << std::endl;
}

float repeat_float(float input, float max)
{
    if (input <= 0) {
        return 0;
    } else {
        return std::fmod(input, max);
    }
}

void PFNNModel::predict(const std::unique_ptr<Tensor> &X, std::unique_ptr<Tensor> &Y)
{
    int index = (int)((phase / (2.0 * M_PI)) * (double) TENSORS_N );

    assert(index >= 0);
    assert(index < TENSORS_N);

    //Normalise Input
    (*Y) = X->normalise(*Xmean, *Xstd)
    //Process Vanilla Network
        .layer(*W0[index], *b0[index]).ELU()
        .layer(*W1[index], *b1[index]).ELU()
        .layer(*W2[index], *b2[index])
    //Renormalise Output
        .renormalise(*Ymean, *Ystd);

    //Update Phase
    auto phase_output = Y->get_value(phase_index, 0);
    assert(!std::isnan(phase_output));
    phase = repeat_float(phase + (1.0 + damping) * phase_output * 2.0 * M_PI, 2.0 * M_PI);
}
