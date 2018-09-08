#include "tensor.h"

#include <iostream>
#include <Eigen/Dense>

using namespace animation::nn;

Tensor::Tensor(unsigned int rows, unsigned int cols)
    : internal_matrix(new Eigen::MatrixXf(Eigen::MatrixXf::Zero(rows, cols)))
{
}

Tensor::Tensor(std::unique_ptr<Eigen::MatrixXf>& internal_matrix)
    : internal_matrix(std::move(internal_matrix))
{
}

Tensor::Tensor(const Eigen::MatrixXf& internal_matrix)
    : internal_matrix(new Eigen::MatrixXf(internal_matrix))
{
}


Tensor::Tensor(const Tensor& other)
    : internal_matrix(new Eigen::MatrixXf(*other.internal_matrix))
{
}

Tensor::Tensor(Tensor && other)
    : Tensor(other.internal_matrix)
{
}


Tensor::~Tensor()
{

}

void Tensor::set_value(unsigned int row, unsigned int col, float value)
{
    assert(row < rows());
    assert(col < cols());

    (*internal_matrix)(row, col) = (float) value;
}

const float & Tensor::get_value(unsigned int row, unsigned int col)
{
    assert(row < rows());
    assert(col < cols());

    return (*internal_matrix)(row, col);
}

const Tensor Tensor::operator+(const Tensor& rhs) const
{
    assert(this->rows() == rhs.rows());
    assert(this->cols() == rhs.cols());

    return Tensor((*this->internal_matrix) + (*rhs.internal_matrix));
}

const Tensor Tensor::operator-(const Tensor& rhs) const
{
    assert(this->rows() == rhs.rows());
    assert(this->cols() == rhs.cols());

    return Tensor((*this->internal_matrix) - (*rhs.internal_matrix));
}

const Tensor Tensor::operator*(const Tensor& rhs) const
{
    assert(this->cols() == rhs.rows());

    return Tensor((*this->internal_matrix) * (*rhs.internal_matrix));
}

const Tensor Tensor::operator*(const float value) const
{
    return Tensor((*internal_matrix) * value);
}

Tensor & Tensor::operator=(const Tensor& rhs)
{
    (*this->internal_matrix) = *rhs.internal_matrix;

    return *this;
}

Tensor & Tensor::operator+=(const Tensor& rhs)
{
    assert(this->rows() == rhs.rows());
    assert(this->cols() == rhs.cols());

    (*this->internal_matrix) *= (*rhs.internal_matrix);
    return *this;
}

const Tensor Tensor::point_wise_product(const Tensor& rhs) const
{
    assert(this->rows() == rhs.rows());
    assert(this->cols() == rhs.cols());

    return Tensor(this->internal_matrix->cwiseProduct(*rhs.internal_matrix));
}

const Tensor Tensor::point_wise_quotient(const Tensor& rhs) const
{
    assert(this->rows() == rhs.rows());
    assert(this->cols() == rhs.cols());

    return Tensor(this->internal_matrix->cwiseQuotient(*rhs.internal_matrix));
}

const Tensor Tensor::point_wise_absolute() const
{
    return Tensor(this->internal_matrix->cwiseAbs());
}

Tensor & Tensor::ELU()
{
    int rows = internal_matrix->rows();
    for (int i = 0; i<rows; i++) {
        (*internal_matrix)(i, 0) = (std::max)((*internal_matrix)(i, 0), 0.0f) + std::exp((std::min)((*internal_matrix)(i, 0), 0.0f)) - 1.0f;
    }

    return *this;
}

Tensor & Tensor::sigmoid()
{
    int rows = internal_matrix->rows();
    for (int i = 0; i<rows; i++) {
        (*internal_matrix)(i, 0) = 1.0f / (1.0f + std::exp(-(*internal_matrix)(i, 0)));
    }

    return *this;
}

Tensor & Tensor::tanh()
{
    int rows = internal_matrix->rows();
    for (int i = 0; i<rows; i++) {
        (*internal_matrix)(i, 0) = std::tanh((*internal_matrix)(i, 0));
    }

    return *this;
}

Tensor & Tensor::softmax()
{
    float frac = 0.0f;
    int rows = internal_matrix->rows();
    for (int i = 0; i<rows; i++) {
            (*internal_matrix)(i, 0) = std::exp((*internal_matrix)(i, 0));
            frac += (*internal_matrix)(i, 0);
    }
    for (int i = 0; i<rows; i++) {
            (*internal_matrix)(i, 0) /= frac;
    }

    return *this;
}

Tensor & Tensor::set_zero()
{
    internal_matrix->setZero();
    //*internal_matrix = (*internal_matrix).Zero((*internal_matrix).rows(), (*internal_matrix).cols());
    return *this;
}


const float Tensor::row_sum(unsigned int row) const
{
    assert(row < this->rows());

    return internal_matrix->row(row).sum();
}

const float Tensor::col_sum(unsigned int col) const
{
    assert(col < this->cols());

    return internal_matrix->col(col).sum();
}
