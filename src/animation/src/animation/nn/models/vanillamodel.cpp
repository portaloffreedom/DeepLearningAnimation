#include "vanillamodel.h"

using namespace animation::nn;

VanillaModel::VanillaModel(unsigned int x_dim, unsigned int h_dim, unsigned int y_dim, unsigned int styles_dim)
    : Model(_x_dim, _y_dim, _styles_dim, 0 /*TODO read number from file*/)
    , h_dim(h_dim)
{

}

void VanillaModel::load_parameters(const std::string& folder_path)
{
    Xmean.reset(create_tensor(load_binary_matrix(folder_path+"/Xmean.bin", _x_dim, 1), "Xmean"));
    Xstd.reset(create_tensor(load_binary_matrix(folder_path+"/Xstd.bin", _x_dim, 1), "Xstd"));
    Ymean.reset(create_tensor(load_binary_matrix(folder_path+"/Ymean.bin", _y_dim, 1), "Ymean"));
    Ystd.reset(create_tensor(load_binary_matrix(folder_path+"/Ystd.bin", _y_dim, 1), "Ystd"));

    W0.reset(create_tensor(load_binary_matrix(folder_path+"/W0_000.bin", h_dim, _x_dim), "W0"));
    W1.reset(create_tensor(load_binary_matrix(folder_path+"/W1_000.bin", h_dim, h_dim), "W1"));
    W2.reset(create_tensor(load_binary_matrix(folder_path+"/W2_000.bin", _y_dim, h_dim), "W2"));
    b0.reset(create_tensor(load_binary_matrix(folder_path+"/b0_000.bin", h_dim, 1), "b0"));
    b1.reset(create_tensor(load_binary_matrix(folder_path+"/b1_000.bin", h_dim, 1), "b1"));
    b2.reset(create_tensor(load_binary_matrix(folder_path+"/b2_000.bin", _y_dim, 1), "b2"));

//     X.reset(create_tensor(x_dim, 1, "X"));
//     Y.reset(create_tensor(y_dim, 1, "Y"));
}

void VanillaModel::predict(const std::unique_ptr<Tensor> &X, std::unique_ptr<Tensor> &Y)
{
//     //Normalise Input
//     (*Y) = X->normalise(*Xmean, *Xstd);
//
//     //Process Vanilla Network
//     (*Y) = Y->layer(*W0, *b0).ELU();
//     (*Y) = Y->layer(*W1, *b1).ELU();
//     (*Y) = Y->layer(*W2, *b2);
//
//     //Renormalise Output
//     Y->renormalise(*Ymean, *Ystd);

    //Normalise Input
    (*Y) = X->normalise(*Xmean, *Xstd)
    //Process Vanilla Network
        .layer(*W0, *b0).ELU()
        .layer(*W1, *b1).ELU()
        .layer(*W2, *b2)
    //Renormalise Output
        .renormalise(*Ymean, *Ystd);
}
