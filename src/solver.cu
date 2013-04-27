#include "mesh.h"
#include "constant.h"

__device__ double
source(int i, int j, double t){
    return (j==200)?8*sin((t*1e10*2*pi)):0;
}

//H
__global__ void
updateH(mesh* m, int W, int H)
{
	int idx = blockIdx.x * blockDim.x + threadIdx.x;
	int idy = blockIdx.y * blockDim.y + threadIdx.y;
	int k = idy*W+idx;
	//update
	if(idx<W-1 && idy<H-1){
		m[k].Hzx = m[k].DHx1*m[k].Hzx - m[k].DHx2 * (m[k+H].Ey - m[k].Ey);
		m[k].Hzy = m[k].DHy1*m[k].Hzy + m[k].DHy2 * (m[k+1].Ex - m[k].Ex);
	}
	//source
	__syncthreads();
}

//E
__global__ void
updateE(mesh *m, int W, int H)
{
	int idx = blockIdx.x * blockDim.x + threadIdx.x;
	int idy = blockIdx.y * blockDim.y + threadIdx.y;
	int k = idy*W+idx;
	//update
	if(idx<W && idx>0 && idy>0 && idy<H){
		m[k].Ex = m[k].CEx1*m[k].Ex + m[k].CEx2 * (m[k].Hzx + m[k].Hzy - m[k-1].Hzx - m[k-1].Hzy);
		m[k].Ey = m[k].CEy1*m[k].Ey - m[k].CEy2 * (m[k].Hzx + m[k].Hzy - m[k-H].Hzx - m[k-H].Hzy);
	}
	//source
	__syncthreads();
}

__global__ void 
updateSource(mesh *m, int W, int H, double time){
	int idx = blockIdx.x * blockDim.x + threadIdx.x;
	int idy = blockIdx.y * blockDim.y + threadIdx.y;
	int k = idy*W+idx;
	if(idx<W && idx>0 && idy>0 && idy<H){
		double Hs = source(idx, idy, time);
		double Es = source(idx, idy, time);
		m[k].Hzy+= m[k].DHx2*	s;
		m[k+1].Ey	+= m[k+1].CEy2*s/(120*pi);
	}
	__syncthreads();
}

extern "C"
void cudaUpdateKernel(mesh* d_m, int Nx, int Ny, double t){
	dim3 dimBlock(32,32);
	dim3 dimGrid(ceil(Nx/32), ceil(Ny/32));
	updateH<<<dimGrid, dimBlock>>>(d_m, Nx, Ny);
	updateE<<<dimGrid, dimBlock>>>(d_m, Nx, Ny);
	updateSource<<<dimGrid, dimBlock>>>(d_m, Nx, Ny, t);
}

