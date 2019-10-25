
#pragma OPENCL EXTENSION cl_khr_fp64 : enable
__kernel void Dust(__global double *a,
__global double *b,
__global double *c,
const unsigned int n,
__global int *icds)
{
//Get our global thread ID
int id = get_global_id(0);

//Make sure we do not go out of bounds
if (id < 10){
printf(\"In kernel %d \\n Random initial condition: (%d, %d, %d) \\n \", id, icds[3*id],icds[3*id+1],icds[3*id+2]);


    }
}


                                 