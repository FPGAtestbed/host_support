#include <host_support.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <stdexcept>

static std::vector<cl::Device> obtainPlatformDevices(const std::string&);
static char* read_binary_file(const std::string&, uint32_t&);

void initialiseDevice(const std::string & vendorName, const std::string & deviceName, const std::string & binaryName,
    cl::Context * context, cl::Program * program) {

  cl_int err;
  std::vector<cl::Device> matchingDevices=obtainMatchingDevices(vendorName, deviceName);
  OCL_CHECK(err, context=new cl::Context(matchingDevices, NULL, NULL, NULL, &err));
  program=programDevice(binaryName, matchingDevices, *context);
}

std::vector<cl::Device> obtainMatchingDevices(const std::string & vendorName, const std::string & deviceName) {
  std::vector<cl::Device> matchingDevices;
  std::vector<cl::Device> platformDevices;
  try {
    platformDevices=obtainPlatformDevices(vendorName);
  } catch(const std::exception& e) {
    throw;
  }
  for (cl::Device device : platformDevices) {
    cl_int err;
    std::string specificDevName=device.getInfo<CL_DEVICE_NAME>(&err);
    if (specificDevName.find(deviceName) != std::string::npos) {
      matchingDevices.push_back(device);
    }
  }
  return matchingDevices;
}

cl::Program* programDevice(const std::string & binaryName, cl::Device & device, cl::Context & context) {
  std::vector<cl::Device> progdevices(1, device);
  return programDevice(binaryName, device, context);
}

cl::Program* programDevice(const std::string & binaryName, std::vector<cl::Device> & devices, cl::Context & context) {
  cl::Program * program;
  cl_int err;
  // Read the binary file
  uint32_t fileBufSize;
  char* fileBuf;
  try {
    fileBuf = read_binary_file(binaryName, fileBufSize);
  } catch(const std::exception& e) {
    throw;
  }
  cl::Program::Binaries bins{{fileBuf, fileBufSize}};

  // Create the program object from the binary and program the FPGA device with it
  OCL_CHECK(err, program=new cl::Program(context, devices, bins, NULL, &err));
  return program;
}

static std::vector<cl::Device> obtainPlatformDevices(const std::string & vendorName) {
  cl_int err;
  std::vector<cl::Platform> platforms;
  OCL_CHECK(err, err = cl::Platform::get(&platforms));
  cl::Platform platform;
  for (int i=0;i<platforms.size();i++) {
    platform=platforms[i];
    OCL_CHECK(err, std::string platformName = platform.getInfo<CL_PLATFORM_NAME>(&err));
    if (platformName == vendorName){
      std::vector<cl::Device> devices;
      OCL_CHECK(err, err = platform.getDevices(CL_DEVICE_TYPE_ACCELERATOR, &devices));
      return devices;
    }
  }
  std::throw_with_nested(HostSupportException("No platform with name '"+vendorName+"' found"));
}

static char* read_binary_file(const std::string &xclbin_file_name, uint32_t & nb) {
  if(access(xclbin_file_name.c_str(), R_OK) != 0) {
    std::throw_with_nested(HostSupportException("File '"+xclbin_file_name+"' not found when trying to load xclbin"));
  }
  //Loading XCL Bin into char buffer
  std::ifstream bin_file(xclbin_file_name.c_str(), std::ifstream::binary);
  bin_file.seekg (0, bin_file.end);
  nb = bin_file.tellg();
  bin_file.seekg (0, bin_file.beg);
  char *buf = new char[nb];
  bin_file.read(buf, nb);
  return buf;
}
