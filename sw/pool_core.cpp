#include "pool_core.h"

#define max(a,b) ((a>b)?a:b)
#define min(a,b) ((a>b)?b:a)

void Pool(int CHin,int Hin,int Win,
		int Kx,int Ky,int mode,
		Dtype_f feature_in[],Dtype_f feature_out[]
	)//mode: 0:MEAN, 1:MIN, 2:MAX
{
	/*
	#pragma HLS INTERFACE m_axi depth=4294967295 port=feature_out offset=slave
	#pragma HLS INTERFACE m_axi depth=4294967295 port=feature_in offset=slave
	*/
/*
	#pragma HLS INTERFACE m_axi depth=2147483647 port=feature_out offset=slave
	#pragma HLS INTERFACE m_axi depth=2147483647 port=feature_in offset=slave
	#pragma HLS INTERFACE s_axilite port=Win
	#pragma HLS INTERFACE s_axilite port=Kx
	#pragma HLS INTERFACE s_axilite port=Hin
	#pragma HLS INTERFACE s_axilite port=mode
	#pragma HLS INTERFACE s_axilite port=Ky
	#pragma HLS INTERFACE s_axilite port=CHin
	#pragma HLS INTERFACE s_axilite port=return
	*/
	int Hout,Wout;
	Wout=Win/Kx;//池化在做缩小，所以是直接除去这种核的大小
	Hout=Hin/Ky;

	for(int c=0;c<CHin;c++)//遍历通道
		for(int i=0;i<Hout;i++)//i,j遍历整个输入矩阵的高和宽
			for(int j=0;j<Wout;j++)
			{
				Dtype_f sum;
				if(mode==0)//三种池化模式对应给sum赋初值，0时候均值；求最小时候取个大数，求最大时候取个很小的数，
					sum=0;
				else
					if(mode==1)
						sum=99999999999999999;
					else
						sum=-99999999999999999;
				for(int ii=0;ii<Ky;ii++)
					for(int jj=0;jj<Kx;jj++)//ii,jj遍历概率矩阵，在这里也是Kx Ky表达
					{
						ap_int<16> h=i*Ky+ii;
						ap_int<16> w=j*Kx+jj;
						switch(mode)
						{//三种池化模式
							case 0:{sum+=feature_in[h*CHin*Win+w*CHin+c];break;}//求出整个池化范围的和
							case 1:{sum=min(sum,feature_in[h*CHin*Win+w*CHin+c]);break;}
							case 2:{sum=max(sum,feature_in[h*CHin*Win+w*CHin+c]);break;}
							default:break;
						}
					}
				if(mode==0)
					sum=sum/(Kx*Ky);//求均值
				feature_out[i*Wout*CHin+j*CHin+c]=sum;//结果给输出
			}
}
