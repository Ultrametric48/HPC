
#include <stdio.h>
#include <stdlib.h>
#include <math.h>   
#include <OpenCL/OpenCL.h>
 


class first_order_diff_eq_solver{
    
    float init_t, init_y;
    
    
public:
    first_order_diff_eq_solver(float t, float y){
        this->init_t = t;
        this->init_y = y;
    }
    first_order_diff_eq_solver(){};

  
    
    float Runge_Kutta_4(float (*func)(float t, float y), float h){
        
        float k1, k2, k3, k4;
        float t = this->init_t;
        float y = this->init_y;
        
        k1 = h*func(t,y);
        k2 = h*func(t + h/2.0, y + k1/2.0);
        k3 = h*func(t + h/2.0, y + k2/2.0);
        k4 = h*func(t + h, y + k3);
        
        return y + (k1 + k4)/6.0 + (k2 + k3)/3.0;
    }
    
    float Runge_Kutta_4x(float (*func)(float t, float x, float y, float z), float t, float x, float y, float z, float h){
        
        float k1, k2, k3, k4;
        
        k1 = h*func(t,x,y,z);
        k2 = h*func(t + h/2.0, x + k1/2.0, y, z);
        k3 = h*func(t + h/2.0, x + k2/2.0, y, z);
        k4 = h*func(t + h, x + k3, y, z);
        
        return x + (k1 + k4)/6.0 + (k2 + k3)/3.0;
    }
    float Runge_Kutta_4y(float (*func)(float t, float x, float y, float z), float t, float x, float y, float z, float h){
        
        float k1, k2, k3, k4;
        
        k1 = h*func(t,x,y,z);
        k2 = h*func(t + h/2.0, x, y + k1/2.0, z);
        k3 = h*func(t + h/2.0, x, y + k2/2.0, z);
        k4 = h*func(t + h, x, y + k3, z);
        
        return y + (k1 + k4)/6.0 + (k2 + k3)/3.0;
    }
    float Runge_Kutta_4z(float (*func)(float t, float x, float y, float z), float t, float x, float y, float z, float h){
        
        float k1, k2, k3, k4;
        
        k1 = h*func(t,x,y,z);
        k2 = h*func(t + h/2.0, x, y, z + k1/2.0);
        k3 = h*func(t + h/2.0, x, y, z + k2/2.0);
        k4 = h*func(t + h, x, y, z + k3);
        
        return z + (k1 + k4)/6.0 + (k2 + k3)/3.0;
    }
    
};



float function(float t, float y){return y;}
        

//Rossler System
static float a = 0.1;
static float b = 0.1;
static float c = 18.0;

float eqn_one(float t,  float x, float y, float z){return -y - z;}
float eqn_two(float t,  float x, float y, float z){return x + a*y;}
float eqn_three(float t,  float x, float y, float z){return b + z*(x - c);}


//Lorentz system
static float s = 10.0;
static float rho = 28.0;
static float B = 8.0/3.0;

float Leqn_one(float t,  float x, float y, float z){return s*(y - x);}
float Leqn_two(float t,  float x, float y, float z){return x*(rho - z) - y;}
float Leqn_three(float t,  float x, float y, float z){return x*y - B*z;}
        

//########################START KERNEL SOURCE####################################
// OpenCL kernel. Each work item takes care of one element of c
const char *kernelSource =   "\n" \
"#pragma OPENCL EXTENSION cl_khr_fp64 : enable\n" \
"#pragma OPENCL EXTENSION cl_intel_printf : enable\n" \
"__kernel void vecAdd(  __global double *a,\n" \
"__global double *b,\n" \
"__global double *c,\n" \
"const unsigned int n)\n" \
"{\n" \
"//Get our global thread ID\n" \
"int id = get_global_id(0);\n" \
"\n" \
"//Make sure we do not go out of bounds\n" \
"if (id < 100 )\n" \
"printf(\"In kernel %d \\n \", id);\n" \
"}\n" \
"\n" \
"\n" \
"\n" \
; 

//########################END KERNEL SOURCE####################################                                                               

 
int main( int argc, char* argv[] )
{
    // Length of vectors
    unsigned int n = 100000;
 
    // Host input vectors
    double *h_a;
    double *h_b;
    // Host output vector
    double *h_c;
 
    //Device input buffers
    cl_mem d_a;
    cl_mem d_b;
    //Device output buffer
    cl_mem d_c;
    //cl Device info 
    char device_info[1024];

    
    cl_platform_id cpPlatform;        // OpenCL platform
    cl_device_id device_id;           // device ID
    cl_context context;               // context
    cl_command_queue queue;           // command queue
    cl_program program;               // program
    cl_kernel kernel;                 // kernel
 
    //Size, in bytes, of each vector
    size_t bytes = n*sizeof(double);
    
    //Function
    
    
    /*
    
    
    puts("x,y,z,time,iteration");
    #pragma unroll
    for(int k = 0; k < iteration_number; k++){
        x_temp = exp.Runge_Kutta_4x(Leqn_one, t, x, y, z, h);
        y_temp = exp.Runge_Kutta_4y(Leqn_two, t, x, y, z, h);
        z_temp = exp.Runge_Kutta_4z(Leqn_three, t, x, y, z, h);
        t += h;
        x = x_temp;
        y = y_temp;
        z = z_temp;
        x_solutions[iteration_number] = x;
        y_solutions[iteration_number] = y;
        z_solutions[iteration_number] = z;
        printf("%f,%f,%f,%f,%d\n", x, y, z, t, k);
    }
    
    */
     
    //exit(0);
 
    // Allocate memory for each vector on host
    h_a = (double*)malloc(bytes);
    h_b = (double*)malloc(bytes);
    h_c = (double*)malloc(bytes);
 
    // Initialize vectors on host
    int i;
    for( i = 0; i < n; i++ )
    {
        h_a[i] = sinf(i)*sinf(i);
        h_b[i] = cosf(i)*cosf(i);
    }
 
    size_t globalSize, localSize;
    cl_int err;
 
    // Number of work items in each local work group
    localSize = 64;
 
    // Number of total work items - localSize must be devisor
    globalSize = ceil(n/(float)localSize)*localSize;
 
    // Bind to platform
    err = clGetPlatformIDs(1, &cpPlatform, NULL);
 
    // Get ID for the device
    err = clGetDeviceIDs(cpPlatform, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
    

    //print graphics card informaton    
    err = clGetDeviceInfo(device_id, CL_DEVICE_NAME,sizeof(device_info),&device_info, NULL);
    printf("Graphics Card:  %s \n", device_info);

 
    // Create a context 
    context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
 
    // Create a command queue
    queue = clCreateCommandQueue(context, device_id, 0, &err);
 
    // Create the compute program from the source buffer
    program = clCreateProgramWithSource(context, 1,
                            (const char **) & kernelSource, NULL, &err);
 
    // Build the program executable
    clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
 
    // Create the compute kernel in the program we wish to run
    kernel = clCreateKernel(program, "vecAdd", &err);
 
    // Create the input and output arrays in device memory for our calculation
    d_a = clCreateBuffer(context, CL_MEM_READ_ONLY, bytes, NULL, NULL);
    d_b = clCreateBuffer(context, CL_MEM_READ_ONLY, bytes, NULL, NULL);
    d_c = clCreateBuffer(context, CL_MEM_WRITE_ONLY, bytes, NULL, NULL);
 
    // Write our data set into the input array in device memory
    err = clEnqueueWriteBuffer(queue, d_a, CL_TRUE, 0,
                                   bytes, h_a, 0, NULL, NULL);
    err |= clEnqueueWriteBuffer(queue, d_b, CL_TRUE, 0,
                                   bytes, h_b, 0, NULL, NULL);
 
    // Set the arguments to our compute kernel
    err  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_a);
    err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_b);
    err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &d_c);
    err |= clSetKernelArg(kernel, 3, sizeof(unsigned int), &n);
 
    // Execute the kernel over the entire range of the data set 
    err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &globalSize, &localSize,
                                                              0, NULL, NULL);
 
    // Wait for the command queue to get serviced before reading back results
    clFinish(queue);
 
    // Read the results from the device
    clEnqueueReadBuffer(queue, d_c, CL_TRUE, 0,
                                bytes, h_c, 0, NULL, NULL );
 
    //Sum up vector c and print result divided by n, this should equal 1 within error
    double sum = 0;
    for(i=0; i<n; i++)
        sum += h_c[i];
    printf("final result: %f\n", sum/n);
 
    // release OpenCL resources
    clReleaseMemObject(d_a);
    clReleaseMemObject(d_b);
    clReleaseMemObject(d_c);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
 
    //release host memory
    free(h_a);
    free(h_b);
    free(h_c);
    
    return 0;
}

