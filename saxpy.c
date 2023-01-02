#include<stdio.h>
#include<stdlib.h>
#include<CL/cl.h>

const char* saxpy_kernel = "__kernel                                  "
                           "void saxpy_kernel(float alpha,            "
                           "                  __global float *A,      "
                           "                  __global float *B,      "
                           "                  __global float *C)      "
                           "{                                         "
                           "    //Get the index of the work-item      \n"
                           "    int index = get_global_id(0);         "
                           "    C[index] = alpha* A[index] + B[index];"
                           "}                                         ";

int main(int argc, char* argv)
{
    // get arguments
    int VECTOR_SIZE = atoi(argv[1]);
    int LOCAL_SIZE  = atoi(argv[2]);
    int alpha = 0.2;

    // allocate vectors A,B,C
    float* A = (float*)malloc(VECTOR_SIZE*sizeof(float));
    float* B = (float*)malloc(VECTOR_SIZE*sizeof(float));
    float* C = (float*)malloc(VECTOR_SIZE*sizeof(float));
    // initialize them
    for(int i=0; i<VECTOR_SIZE; i++)
    {
        A[i] = i;
        B[i] = VECTOR_SIZE-i;
    }

    // get platform and device information
    cl_platform_id* platforms = NULL;
    cl_device_id* devices = NULL;
    cl_uint num_platforms;
    cl_uint num_devices;
    cl_int clstat=0;

    clGetPlatformIDs(0, NULL, &num_platforms);
    platforms = (cl_platform_id*)malloc(num_platforms*sizeof(cl_platform_id));
    clGetPlatformIDs(1, platforms, NULL);

    clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, 0, NULL, &num_devices);
    devices = (cl_device_id*)malloc(num_devices*sizeof(cl_device_id));
    clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, num_devices, devices, NULL);

    // create OpenCL context on device
    cl_context context = clCreateContext(NULL, num_devices, devices, NULL, NULL, &clstat);

    // create command queue
    cl_command_queue cmd_queue = clCreateCommandQueueWithProperties(context, devices[0], NULL, &clstat);

    // create memory buffers on device for each vector
    cl_mem A_clmem = clCreateBuffer(context, CL_MEM_READ_ONLY, VECTOR_SIZE*sizeof(float), NULL, &clstat);
    cl_mem B_clmem = clCreateBuffer(context, CL_MEM_READ_ONLY, VECTOR_SIZE*sizeof(float), NULL, &clstat);
    cl_mem C_clmem = clCreateBuffer(context, CL_MEM_WRITE_ONLY, VECTOR_SIZE*sizeof(float), NULL, &clstat);

    // copy buffer of A and B to device
    clEnqueueWriteBuffer(cmd_queue, A_clmem, CL_TRUE, 0, VECTOR_SIZE*sizeof(float), A, 0, NULL, NULL);
    clEnqueueWriteBuffer(cmd_queue, B_clmem, CL_TRUE, 0, VECTOR_SIZE*sizeof(float), B, 0, NULL, NULL);

    // create a program with kernel source
    cl_program prg = clCreateProgramWithSource(context, 1, (const char**)&saxpy_kernel, NULL, &clstat);

    // build the program
    clBuildProgram(prg, 1, devices, NULL, NULL, NULL);

    // create the OpenCL kernel
    cl_kernel kernel = clCreateKernel(prg, "saxpy_kernel", &clstat);

    // set arguments of kernel
    clSetKernelArg(kernel, 0, sizeof(float), (void*)&alpha);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)&A_clmem);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), (void*)&B_clmem);
    clSetKernelArg(kernel, 3, sizeof(cl_mem), (void*)&C_clmem);

    // execute kernel on list
    int gsize = VECTOR_SIZE;
    clEnqueueNDRangeKernel(cmd_queue, C_clmem, CL_TRUE, 0, VECTOR_SIZE*sizeof(float), C, 0, NULL, NULL);

    // clean up and wait for all commands to complete
    clFlush(cmd_queue);
    clFinish(cmd_queue);

    // display result to screen
    for(int i=0; i<VECTOR_SIZE; i++)
        printf("%f * %f + %f = %f,\n", alpha, A[i], B[i], C[i]);
    
    // release all allocated objects and buffers
    clReleaseKernel(kernel);
    clReleaseProgram(prg);
    clReleaseMemObject(A_clmem);
    clReleaseMemObject(B_clmem);
    clReleaseMemObject(C_clmem);
    clReleaseCommandQueue(cmd_queue);
    clReleaseContext(context);
    free(A);
    free(B);
    free(C);
    free(platforms);
    free(devices);
    return 0;
}