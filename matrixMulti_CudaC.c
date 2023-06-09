#define BLOCK_SIZE 16

#include <stdio.h>
#include <stdlib.h>

__global__ void KernelMatrixMulti(int *a, int *b, int *c, int m, int n, int k)
{
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    int sum = 0;
    if (col < k && row < m)
    {
        for (int i = 0; i < n; i++)
        {
            sum += a[row * n + i] * b[i * k + col];
        }
        c[row * k + col] = sum;
    }
}

int main(int argc, char const *argv[])
{
    // Entering the dimensions of the matrices
    int m, n, k;
    printf("Input m n and k\n");
    scanf("%d %d %d", &m, &n, &k);

    int *h_a, *h_b, *h_c;
    cudaMallocHost((void **)&h_a, sizeof(int) * m * n);
    cudaMallocHost((void **)&h_b, sizeof(int) * n * k);
    cudaMallocHost((void **)&h_c, sizeof(int) * m * k);

    // Matrix A
    for (int i = 0; i < m; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            h_a[i * n + j] = rand() % 1024;
        }
    }

    // Matrix B
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < k; ++j)
        {
            h_b[i * k + j] = rand() % 1024;
        }
    }

    float gpu_elapsed_time_ms;

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start, 0);

    // Allocate memory space on the device
    int *d_a, *d_b, *d_c;
    cudaMalloc((void **)&d_a, sizeof(int) * m * n);
    cudaMalloc((void **)&d_b, sizeof(int) * n * k);
    cudaMalloc((void **)&d_c, sizeof(int) * m * k);

    // copy matrix A and B from host to device memory
    cudaMemcpy(d_a, h_a, sizeof(int) * m * n, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, h_b, sizeof(int) * n * k, cudaMemcpyHostToDevice);

    unsigned int grid_rows = (m + BLOCK_SIZE - 1) / BLOCK_SIZE;
    unsigned int grid_cols = (k + BLOCK_SIZE - 1) / BLOCK_SIZE;

    dim3 dimGrid(grid_cols, grid_rows);
    dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
    KernelMatrixMulti<<<dimGrid, dimBlock>>>(d_a, d_b, d_c, m, n, k);

    // Transefr results from device to host
    cudaMemcpy(h_c, d_c, sizeof(int) * m * k, cudaMemcpyDeviceToHost);
    cudaThreadSynchronize();

    cudaEventRecord(stop, 0);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&gpu_elapsed_time_ms, start, stop);

    printf("Time elapsed of %dx%d . %dx%d on the GPU: %f ms.\n\n", m, n, n, k, gpu_elapsed_time_ms);

    // free memory
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);
    cudaFreeHost(h_a);
    cudaFreeHost(h_b);
    cudaFreeHost(h_c);
    return 0;
}