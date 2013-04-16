#include <iostream>
#include <cstdio>
#include <cmath>
#include <cassert>

#include <string>
#include <fstream>

#define VERBOSE false
#define SAVE_RESULTS true

// To measure the execution time
#include <sys/time.h>

// The code for the convolutions
#include "convolution_gsl.h"

#define NB_REPETITIONS 20

/******************************************************/
/*                      Main                          */
/******************************************************/

int main(int argc, char * argv[])
{
    if(argc != 3)
    {
        printf("Usage : linear_convolution_gsl_benchmark <img_size> <kernel_size>\n");
        printf(" It performs a linear convolution using : \n");
        printf(" 1 - Nested for loops \n");
        printf(" 2 - The GSL without padding \n");
        printf(" 3 - The gsl with padding \n");
        printf(" 4 - The two previous versions with a small kernel of size 3x3\n");
        return -1;
    }

    std::ofstream results;
    if(SAVE_RESULTS) results.open("../Data/benchmarks_LinearConvolution_gsl.txt", std::ios::app);

    struct timeval before,after;
    double sbefore, safter, total;
    int h_src, w_src;
    int h_kernel, w_kernel;

    h_src = w_src = atoi(argv[1]);
    h_kernel = w_kernel = atoi(argv[2]);

    if(VERBOSE) printf("Image size : %i x %i \n", h_src, w_src);
    if(VERBOSE) printf("Kernel size : %i x %i\n ", h_kernel, w_kernel);
    if(SAVE_RESULTS) results << std::scientific << h_src << " " << h_kernel << " ";

    gsl_matrix * src = gsl_matrix_alloc(h_src, w_src);
    for(int i = 0 ; i < h_src ; ++i)
        for(int j = 0 ; j < w_src ; ++j)
            gsl_matrix_set(src,i ,j, rand()/double(RAND_MAX));

    gsl_matrix * kernel = gsl_matrix_alloc(h_kernel, w_kernel);
    for(int i = 0 ; i < h_kernel ; ++i)
        for(int j = 0 ; j < w_kernel ; ++j)
            gsl_matrix_set(kernel,i ,j, rand()/double(RAND_MAX));

    gsl_matrix * dst = gsl_matrix_alloc(h_src, w_src);
    gsl_matrix * dst_optimal = gsl_matrix_alloc(h_src, w_src);

    // And compute the linear convolution
    if(VERBOSE) printf("Execution times : \n");

    gettimeofday(&before, NULL);
    /*for(int i = 0 ; i < NB_REPETITIONS ; ++i)
        linear_convolution_fft_gsl(src, kernel, dst);
    */
    gettimeofday(&after, NULL);
    sbefore = before.tv_sec + before.tv_usec * 1E-6;
    safter =after.tv_sec + after.tv_usec * 1E-6;
    total = safter - sbefore;
    if(VERBOSE) printf("Unpadded GSL : %e s. \n", total/NB_REPETITIONS);
    if(SAVE_RESULTS) results << total/NB_REPETITIONS << " ";

    // And compute the linear convolution with an optimal size
    gettimeofday(&before, NULL);
    for(int i = 0 ; i < NB_REPETITIONS ; ++i)
        linear_convolution_fft_gsl_optimal(src, kernel, dst_optimal);
    gettimeofday(&after, NULL);
    sbefore = before.tv_sec + before.tv_usec * 1E-6;
    safter =after.tv_sec + after.tv_usec * 1E-6;
    total = safter - sbefore;
    if(VERBOSE) printf("Padded GSL : %e s. \n", total/NB_REPETITIONS);
    if(SAVE_RESULTS) results << total/NB_REPETITIONS << " ";


    if(SAVE_RESULTS) results << std::endl;

    // Free the memory
    gsl_matrix_free(src);
    gsl_matrix_free(kernel);
    gsl_matrix_free(dst);
    gsl_matrix_free(dst_optimal);
}
