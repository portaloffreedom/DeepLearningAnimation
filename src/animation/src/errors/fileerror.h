#ifndef FILEERROR_H
#define FILEERROR_H

#include <exception>
#include <string>

/**
 * @todo write docs
 */
class FileError : public std::exception
{
public:
    enum ErrorType {
        FILE_OPEN_ERROR,
        FILE_READ_ERROR,
    };
    
    /**
     * Constructor (C strings).
     *  @param message C-style string error message.
     *                 The string contents are copied upon construction.
     *                 Hence, responsibility for deleting the char* lies
     *                 with the caller.
     */
    explicit FileError(const char* message, ErrorType error_type)
        : msg(message)
        , error_type(error_type)
    {}

    /**
     * Constructor (C++ STL strings).
     *  @param message The error message.
     */
    explicit FileError(const std::string& message, ErrorType error_type)
        : msg(message)
        , error_type(error_type)
    {}

    /**
     * Destructor.
     * Virtual to allow for subclassing.
     */
    virtual ~FileError() throw () {}

    /**
     * Returns a pointer to the (constant) error description.
     *  @return A pointer to a const char*. The underlying memory
     *          is in posession of the Exception object. Callers must
     *          not attempt to free the memory.
     */
    virtual const char* what() const throw ()
    {
        return msg.c_str();
    }

protected:
    static std::string To_string(ErrorType e)
    {
        switch(e) {
            case FILE_OPEN_ERROR:
                return "FILE_OPEN_ERROR";
            case FILE_READ_ERROR:
                return "FILE_READ_ERROR";
        }
    }
    
// VARIABLES
protected:
    /**
     * Error message.
     */
    const std::string msg;
    const ErrorType error_type;
        
};

#endif // FILEERROR_H
