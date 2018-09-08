#ifndef ANIMATION_NN_MODEL_H
#define ANIMATION_NN_MODEL_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <Eigen/Dense>
#include "animation/nn/types.h"
#include "animation/nn/tensor.h"

namespace animation {
namespace nn {

class Tensor;

/**
 * @todo write docs
 */
class Model
{
public:
    /**
     * Default constructor
     */
    Model(unsigned int x_dim, unsigned int y_dim, unsigned int styles_dim, unsigned int trajectory_samples_n);

    virtual ~Model() {
//         for (auto tensor: this->tensors) {
//             delete tensor;
//         }
    }
    
    unsigned int x_dim() const { return _x_dim; }
    unsigned int y_dim() const { return _y_dim; }
    unsigned int get_styles_dim() const { return _styles_dim; }
    unsigned int trajectory_samples_n() const { return _trajectory_samples_n; }

    virtual void load_parameters(const std::string &folder_path) = 0;
    virtual void predict(const std::unique_ptr<Tensor> &X, std::unique_ptr<Tensor> &Y) = 0;

    
    
protected:
    
    Tensor* create_tensor(int rows, int cols, const std::string &id) {
        if(is_identifier_present(id)) {
            std::cout << "Tensor with ID " << id << " already contained." << std::endl;
            return nullptr;
        }
        Tensor *t = new Tensor(rows, cols);
//         tensors.push_back(t);
        identifiers.push_back(id);
        return t;
    }

    Tensor* create_tensor(const Eigen::MatrixXf &matrix, const std::string &id) {
        if(is_identifier_present(id)) {
            std::cout << "Tensor with ID " << id << " already contained." << std::endl;
            return nullptr;
        }
        Tensor *t = new Tensor(matrix);
//         tensors.push_back(t);
        identifiers.push_back(id);
        return t;
    }

//     void DeleteTensor(const Tensor* t) {
//         int index = tensors.IndexOf(t);
//         if(index == -1) {
//             std::clog << "Tensor not found." << std::endl;
//             return;
//         }
//         tensors.RemoveAt(index);
//         identifiers.RemoveAt(index);
//         delete t;
//     }
//
//     Tensor* GetTensor(const std::string &id) {
//         int index = identifiers.IndexOf(id);
//         if(index == -1) {
//             return nullptr;
//         }
//         return tensors[index];
//     }
//
//     const std::string& GetID(const Tensor* t) const {
//         int index = tensors.IndexOf(t);
//         if(index == -1) {
//             return nullptr;
//         }
//         return identifiers[index];
//     }

    /**
     * Load a `Eigen::MatrixXf` in memory by reading a binary file
     * 
     * @throws FileError if error occour while reading the file
     */
    static Eigen::MatrixXf load_binary_matrix(const std::string& file_path, unsigned const rows, unsigned const cols);

protected:
    const unsigned int _x_dim;
    const unsigned int _y_dim;
    const unsigned int _styles_dim;
    const unsigned int _trajectory_samples_n;

private:
//     std::vector<Tensor*> tensors;
    std::vector<std::string> identifiers;

    bool is_identifier_present(const std::string &id) const {
        return std::find(identifiers.begin(), identifiers.end(), id) != identifiers.end();
    }
};

}
}

#endif // ANIMATION_NN_MODEL_H
