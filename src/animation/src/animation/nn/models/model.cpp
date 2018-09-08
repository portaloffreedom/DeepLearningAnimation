#include "model.h"

#include <iostream>
#include <fstream>
#include "errors/fileerror.h"

using namespace animation::nn;

Model::Model(unsigned int x_dim, unsigned int y_dim, unsigned int styles_dim, unsigned int trajectory_samples_n)
    : _x_dim(x_dim)
    , _y_dim(y_dim)
    , _styles_dim(styles_dim)
    , _trajectory_samples_n(trajectory_samples_n)
{

}

Eigen::MatrixXf Model::load_binary_matrix(const std::string& file_path,unsigned const rows, unsigned const cols)
{
    Eigen::MatrixXf matrix(rows, cols);

    std::ifstream file(file_path, std::ifstream::in | std::ifstream::binary);

    // Test if file open was successful
    if (!file) {
        std::stringstream error_message;
        error_message << "failed to open binary matrix file: " << strerror(errno) 
                      << "\nfile: " << file_path;
        throw FileError(error_message.str(), FileError::ErrorType::FILE_OPEN_ERROR);
    }

    // Throw exception if some read fails
    std::ios_base::iostate exceptionMask = file.exceptions() | std::ios::failbit;
    file.exceptions(exceptionMask);

    float* features = new float[rows * cols];
    try {
        //WARNING assumptions on endianess of files and CPU are made here
        file.read((char*)features, rows * cols * sizeof(float));

    } catch (std::ios_base::failure& e) {
        std::stringstream error_message;
        error_message << "fail while reading binary matrix file: " << e.what()
                      << "\nerrno: " << strerror(errno)
                      << "\nfile: " << file_path;
        throw FileError(error_message.str(), FileError::ErrorType::FILE_READ_ERROR);
    }
    
    //TODO It's maybe possible to make this read way faster. For example reading all the matrix at once instead of 4 bytes at the time.
    for (int i=0; i<rows; i++) {
        for (int j=0; j<cols; j++) {
            matrix(i,j) = features[i*cols + j];
        }
    }

    delete[] features;
    return matrix;
}
