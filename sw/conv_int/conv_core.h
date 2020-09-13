#ifndef __CONV_CORE_H__
#define __CONV_CORE_H__

#include <iostream>

using namespace std;

//typedef ap_int<8> Dtype_f;
//typedef ap_int<8> Dtype_w;
//typedef ap_int<15> Dtype_mul;
//typedef ap_int<32> Dtype_acc;

typedef float Dtype_f;
typedef float Dtype_w;
typedef float Dtype_mul;
typedef float Dtype_acc;


void Conv(int CHin,int Hin,int Win,int CHout,
		int Kx,int Ky,int Sx,int Sy,int mode,int relu_en,
		Dtype_f feature_in[],Dtype_w W[],Dtype_w bias[],Dtype_f feature_out[]
	);//mode: 0:VALID, 1:SAME

#endif
