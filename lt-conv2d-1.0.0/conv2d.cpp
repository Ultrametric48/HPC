//@Author Dominic Gastaldo
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "Eigen/Dense"

#define matrix_len 3

using Eigen::Matrix; 
using namespace Eigen;
typedef Matrix<int, Dynamic, Dynamic> Matrix8b; //define 8-bit matrix


//function prototypes
Matrix8b conv2d(Matrix8b input, Matrix8b kernel);



Matrix8b conv2d(Matrix8b input, Matrix8b kernel){
    /*
     * 2D image convolution with the Eigen matrix template
     */
    
    //Start convolution algorithm
    
    /*
     * Check for kernel matrix seprability
     * The kernel is seprable if it can be written as an outer product of two vectors, and
     * a seprable kernel can be computed faster then its non-seprable counterpart.
     * 
     * 2D Convolution is seprable
     */
    
    //Define pivot elements that map convolution element to image element
    int p = 0;
    int e = 0;
    
    
    Matrix8b conv2d = Matrix8b::Zero(matrix_len, matrix_len); //resultant convolution matrix
    
    //resultant matrix loops
    for(int conv2d_row=0; conv2d_row < matrix_len; conv2d_row++){ 
        for(int conv2d_col=0; conv2d_col < matrix_len; conv2d_col++){ 
            
            //Image/kernel loops
            for(int i=0; i < matrix_len; i++){      
                for(int j=0; j < matrix_len; j++){
                       
                    
                    /*
                     * Boundary conditions:
                     * Kernel operations must be handled at the boundary, where the kernel can be disregarded.
                     */
                    
                    if((conv2d_row + i - p) < 0 ||  (conv2d_row + i - p) >= matrix_len || (conv2d_col + j - e) < 0 ||  (conv2d_col + j - e) >= matrix_len){
                        printf("Skipping (%d,%d)\n", (conv2d_row + i - p), (conv2d_col + j - e));
                        continue;
                    }
                    
                    
                    //compute each entry of resultant matrix
                        
                    std::cout << "(i,j)=" << "(" << conv2d_row + i - p << ", " << conv2d_col + j - e << ")" << std::endl;
                    std::cout << "Input matrix element " << "(" << conv2d_row + i - p << ", " << conv2d_col + j - e << ") "  << input(conv2d_row + i - p, conv2d_col + j - e) << std::endl;
                    std::cout << "Kernel matrix element" << "(" << i << "," << j << ") " << kernel(i,j) << std::endl;
                        
                    conv2d(conv2d_row, conv2d_col) = input(conv2d_row + i - p, conv2d_col + j - e) * kernel(i,j);
                
                            // In line - (matrix_len-1) flips horizontallyconv2d_col - (matrix_len-1)\  // In line - (matrix_len-1) flips vertically
                    
                
                }

            }
            

        }
    
        
    }
    
    
  return conv2d;
    
}




int main(int argc, char **argv){
    
    Matrix8b image = Matrix8b::Identity(matrix_len, matrix_len);
    Matrix8b filter = Matrix8b::Identity(matrix_len, matrix_len);
    
    filter(0,0) = 1;
    filter(0,1) = 0;
    filter(0,2) = -1;
    filter(1,0) = 2;
    filter(1,1) = 0;
    filter(1,2) = -2;
    filter(2,0) = 1;
    filter(2,1) = 0;
    filter(2,2) = -1;
    
     
    

    Matrix8b mat = conv2d(image, filter);
    
    puts("");
    puts("");
    puts("");
    puts("");
    puts("End of program data...\n");
    

    std::cout << "Here's element zero: " << mat(0,0) << std::endl;
    std::cout << "Here is the 2d-convolution m:\n" << mat << std::endl;

}