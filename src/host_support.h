#define CL_HPP_CL_1_2_DEFAULT_BUILD
#define CL_HPP_TARGET_OPENCL_VERSION 120
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_ENABLE_PROGRAM_CONSTRUCTION_FROM_ARRAY_COMPATIBILITY 1
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS

#include <vector>
#include <exception>
#include <CL/cl2.hpp>

#define OCL_CHECK(error,call)                                       \
    call;                                                           \
    if (error != CL_SUCCESS) {                                      \
      printf("%s:%d Error calling " #call ", error code is: %d\n",  \
              __FILE__,__LINE__, error);                            \
      exit(EXIT_FAILURE);                                           \
    }

class HostSupportException : virtual public std::exception {
  std::string error_message;
    public:
  HostSupportException(const std::string& msg) : error_message(msg) { }

  virtual ~HostSupportException() throw () {}

  virtual const char* what() const throw () {
    return error_message.c_str();
  }
};

std::vector<cl::Device> obtainMatchingDevices(const std::string&, const std::string&);
cl::Program programDevice(const std::string&, cl::Device&, cl::Context&);
cl::Program programDevice(const std::string&, std::vector<cl::Device>&, cl::Context&);
void initialiseDevice(const std::string&, const std::string&, const std::string&, cl::Context&, cl::Program&);
