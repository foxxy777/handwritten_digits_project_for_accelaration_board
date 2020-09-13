#ifndef __POOL_CORE_H__
#define __POOL_CORE_H__

#include <ap_int.h>
#include <iostream>

typedef float Dtype_f;

void Pool(int CHin,int Hin,int Win,
		int Kx,int Ky,int mode,
		Dtype_f feature_in[],Dtype_f feature_out[]
	);//mode: 0:MEAN, 1:MIN, 2:MAX

#endif
