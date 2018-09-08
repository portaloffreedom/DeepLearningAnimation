#ifndef ANIMATION_NN_TENSOR_H
#define ANIMATION_NN_TENSOR_H

#include <memory>
#include "types.h"
#include <Eigen/Dense>

namespace animation {
namespace nn {

/**
 * @todo write docs
 */
class Tensor
{
public:
    /**
     * Default constructor
     */
    Tensor(unsigned int rows, unsigned int cols);
    Tensor(const Tensor& other);
    Tensor(Tensor&& other);
    Tensor(const Eigen::MatrixXf &internal_matrix);

    /**
     * Destructor
     */
    ~Tensor();

    void set_value(unsigned int row, unsigned int col, float value);
    const float& get_value(unsigned int row, unsigned int col);

    const Tensor operator+(const Tensor& rhs) const;
    const Tensor operator-(const Tensor& rhs) const;
    const Tensor operator*(const Tensor& rhs) const;
    const Tensor operator*(const float value) const;

    Tensor& operator=(const Tensor& rhs);
    Tensor& operator+=(const Tensor &rhs);

    const Tensor point_wise_product(const Tensor& rhs) const;
    const Tensor point_wise_quotient(const Tensor& rhs) const;
    const Tensor point_wise_absolute() const;

    Tensor normalise(const Tensor &mean, const Tensor &std) const {
        return (*this - mean).point_wise_quotient(std);
    }

    Tensor renormalise(const Tensor &mean, const Tensor &std) const {
        return this->point_wise_product(std) + mean;
    }

    Tensor layer(const Tensor &w, const Tensor &b) const {
        return w * (*this) + b;
    }

    Tensor& blend(const Tensor &rhs, float w) {
        (*this) += rhs * w;

        return *this;
    }

    Tensor& ELU();
    Tensor& sigmoid();
    Tensor& tanh();
    Tensor& softmax();
    Tensor& set_zero();

    const float row_sum(unsigned int row) const;
    const float col_sum(unsigned int col) const;


    unsigned int rows() const
    { return internal_matrix->rows(); }
    unsigned int cols() const
    { return internal_matrix->cols(); }

private:
    Tensor(std::unique_ptr<Eigen::MatrixXf> &internal_matrix);

private:
    std::unique_ptr<Eigen::MatrixXf> internal_matrix;

};

}
}

#endif // ANIMATION_NN_TENSOR_H
