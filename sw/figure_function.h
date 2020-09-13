#pragma once

#define CL_HPP_CL_1_2_DEFAULT_BUILD
#define CL_HPP_TARGET_OPENCL_VERSION 120
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_ENABLE_PROGRAM_CONSTRUCTION_FROM_ARRAY_COMPATIBILITY 1

#include <CL/cl2.hpp>
#include <vector>
#include <unistd.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <cstdint>

#include "conv_core.h"
#include "pool_core.h"
#include "variables.h"


void reverse_gray_figure(uint8_t* ptr_buffer, size_t size);
size_t sizeoffile(const char* filepath);
uint8_t* readbmpfile(const char* filepath);
float* readfilterfile_to_float(const char* filepath, int hight, int width, int channel_in, int channel_out);
float* readbiasfile_to_float(const char* filepath, int channel_out);

char* read_binary_file(const std::string &xclbin_file_name, unsigned &nb);
std::vector<cl::Device> get_devices(const std::string& vendor_name);

constexpr auto BMP_OFFSET = 1078;

//OCL_CHECK doesn't work if call has templatized function call
#define OCL_CHECK(error,call)                                       \
    call;                                                           \
    if (error != CL_SUCCESS) {                                      \
      printf("%s:%d Error calling " #call ", error code is: %d\n",  \
              __FILE__,__LINE__, error);                            \
      exit(EXIT_FAILURE);                                           \
    }

//Customized buffer allocation for 4K boundary alignment
template <typename T>
struct aligned_allocator
{
  using value_type = T;
  T* allocate(std::size_t num)
  {
    void* ptr = nullptr;
    if (posix_memalign(&ptr,4096,num*sizeof(T)))
      throw std::bad_alloc();
    return reinterpret_cast<T*>(ptr);
  }
  void deallocate(T* p, std::size_t num)
  {
    free(p);
  }
};





