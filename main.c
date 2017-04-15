/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: Dominic Gastaldo
 *
 * Created on October 1, 2016, 4:13 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/*
 * 
 */

float randomFloat()
{
    
    float r = (-1+2*((float)rand())/RAND_MAX) ; 
    
    //double r = 1.0 * rand() / (RAND_MAX / 2) - 1;
    return r;
     
}


float RandfromNegativefivetofive(){
   
    float r = 10.0*((float)rand()/RAND_MAX) - 5.0;
    
    return r;
   
}

float integrand(float x){
    
    return 1.0/(1.0 + powf(x,2));
    
}


float MonteCarlo(int interationNumber){
    
    
    //Random Numbers need to be uniformly distributed
    
    
    double I = M_PI/2.0;
    
    float sum = 0.0;
    
    float randNum;
    
   
    for(int i = 0; i < interationNumber; i++){
        
        randNum = randomFloat();
        sum += integrand(randNum);
       
    }
   
    
    //return (2.0/(float)interationNumber)*sum;
    return (2.0/(float)interationNumber)*sum;
}

long *MonteCarloError(int interationNumber){
    
    
    
    double I = M_PI/2.0;
    
    float sum = 0.0;
    
    
    long Error_At_Iteration[interationNumber];
    
    
    for(int i = 0; i < interationNumber; i++){
        
        sum += integrand(randomFloat());
        
        Error_At_Iteration[i] = fabs(I -(2.0/(float)i)*sum);
        
    }
   
    
    //return (2.0/(float)interationNumber)*sum;
    return Error_At_Iteration;
}


int IndicatorFunction(float pointx, float pointy){
    
    float isIncluded = pow(pointx,2) + pow(pointy,2);
    
    if(isIncluded <= 1)
        return 1.0;
    else
        return 0.0;
    
}


float MonteCarloPi(int iterationNumber){
    
    float sum = 0;   
    
    float randy;
    float randx;
    
    
            
    for(int c = 0; c < iterationNumber; c++){
        
        randx = randomFloat();
        randy = randomFloat();
        
        sum += IndicatorFunction(randx,randy);
        
        printf("%f\n",sum);
        
    }
     
    //return abs(M_PI - (4.0/iterationNumber)*sum);
    return fabs(M_PI - (4.0/iterationNumber)*sum)/(M_PI);
}

float LikelihoodFunction(float pointx, float pointy){
    
    float expon;
    
    expon = -pow((1-pointx),2) - 100*pow(pointy - pow(pointx,2),2);
    
    return(pow(exp(1),expon));
    
}

    
float MonteCarloModelSelction(int iterationNumber){
    
    float randx,randy;
   
    
   float sum =0;
    
    
    for(int c=0; c < iterationNumber; c++){
        
        randx = RandfromNegativefivetofive();
        randy = RandfromNegativefivetofive();
        
        
        sum += LikelihoodFunction(randx,  randy);
        
    }
    
   return (100.0/(float)iterationNumber)*sum;
    
}


int main(int argc, char** argv) {

    
    double temp;
    double a = -1.0;
    double b = 1.0;
    int itnum = 20;
    
    srand(time(NULL));
    
    float MonteCarloOutput = MonteCarlo(itnum);
    
    
    for(int c = 1; c < 12; c++){
        
        printf("%f \n",  MonteCarloModelSelction(pow(10,c)));
        
    }
    
    
  
    
    return (EXIT_SUCCESS);
}

