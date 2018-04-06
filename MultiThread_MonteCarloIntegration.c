
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: Dominic
 *
 * Created on October 23, 2016, 8:19 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include<math.h>
#include<omp.h>
#include<stdbool.h>

/*
 * 
 */

float RandfromNegativefivetofive()
{
    float r = 10.0*((float)rand()/RAND_MAX) - 5.0;
    return r;
}

float RandInRange(float a, float b)
{
    return (b-a)*((float)rand()/RAND_MAX) + a;
}

float * TenDimentionalRandomVectorInRange(float a, float b)
{
    
    float *randomValue = malloc(sizeof(float) * 10);
    int c;
    for (c = 0; c < 10; c++)
        randomValue[c] = RandInRange(a, b);
        
    
    return randomValue;
}



float * ArbitraryDimentionalRandomVectorInRange(float a, float b, int dim)
{
    
    float *randomValue = malloc(sizeof(float) * dim);
    
    
    for(int c = 0; c < dim; c++){
        
        randomValue[c] = RandInRange(a, b);
        
    }
    return randomValue;   
}


float LFunction(float * randPointer)
{
    float sum = 0;
    
    
    for(int c = 0; c < 10; c++){
        
        //printf("%f \n", randPointer[c]);
        
        sum += randPointer[c];
        
    }
    //printf("  \n");
    return sum;
}


float expLFunction(float * randPointer)
{
    
    double sum = 0;
    double partOne = 0;
    double partTwo = 0;
    
    
    for(int c = 1; c <= 9; c++){
        
        partOne = (1.0 - randPointer[c])*(1.0 - randPointer[c]);
        partTwo = 100.0*(randPointer[c+1] - randPointer[c]*randPointer[c])*(randPointer[c+1] - randPointer[c]*randPointer[c]);
        
        sum += partOne + partTwo;
        return exp(-sum);
        
    }
    
}



float MonteCarloIntegration(int iterationNumber)
{
    
    double sum = 0; //double
    long long Volume = pow(10,10);
    int threadNumber;
    int count = 0;
    int k = 1;
    int foo = pow(4,k);
    float integralValue_at_Iteration_c;
    float integralValue_at_Iteration_c_MinusOne;
    float error;
    double temp = 0;
    
    
    #pragma omp parallel for  
    for(int c = 1; c <= iterationNumber; c++)
    {
        
        //float * randVector = ArbitraryDimentionalRandomVectorInRange(-5, 5, 10);
        float * randVector = ArbitraryDimentionalRandomVectorInRange(-0.5, 0.5, 10);
        
        #pragma omp critical
        {
            
         
            //****************************************
            //Run for problem 1
            sum += LFunction(randVector);
        
        
            //****************************************
            //Run For problem 3.
            //sum += expLFunction(randVector);

            //use this for 4^k
        
        
            //rather than using master, thread zero is tasked with printing values
            //be careful of the master thread skipping powers of 4! Select out that thread for which count is a power of 4
        
            // local to critical scope
            int threadNum;
        
        
        
            if(count == foo)
            {
                //find the thread that's a power of four and only execute that 
                threadNum = omp_get_thread_num();
            
            
                if (omp_get_thread_num() == threadNum)    
                {
                    // if c is a power of 4, then print iteration number and integral value
            
                    integralValue_at_Iteration_c = (1.0/(float)count) * sum;
                    error = fabs(integralValue_at_Iteration_c - temp);
                    temp = integralValue_at_Iteration_c;
   
                    printf("At iteration %d, the integral value is %f \n", count, integralValue_at_Iteration_c);
                    printf("The error at iteration %d is %f \n", count, error);
            
                    foo = pow(4, ++k);
                }
        
            }
        
                count++;  
        }
        
        if (c > 1)
            continue;
        
        threadNumber = omp_get_num_threads();
    }
    
    
    printf("Total iterations: %d\n", count);
    printf("Total threads used: %d\n", threadNumber);
    
    //((float)Volume/(float)iterationNumber)
    
    
 return (1.0/((float)count)) * sum;   
 
}



int main(int argc, char** argv) {
    
    double start;
    double end;
    
    
    srand(time(NULL));
    
    float montecarlo;
    float *pointer;
    
    pointer = ArbitraryDimentionalRandomVectorInRange(0.0, 100.0, 30);
    
    //montecarlo = RandfromNegativefivetofive();
    
   
    printf("**************************************** \n");
    printf("Output \n");
    printf("**************************************** \n");
    printf("\n");
    start = omp_get_wtime();
    printf("Monte Carlo Integration Output: %f \n", MonteCarloIntegration(100000));
    end = omp_get_wtime();
    
    printf("\n");
    
    printf("Wall time: %f \n", end-start);
    
    
    
    
    free(pointer);
    
    return (EXIT_SUCCESS);
}


