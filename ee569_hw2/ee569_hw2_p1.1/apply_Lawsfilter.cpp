//
//  apply_Lawsfilter.cpp
//  ee569_hw2_p1.1
//
//  Created by Yun-Jun Lee on 9/28/15.
//  Copyright (c) 2015 USC. All rights reserved.
//
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "apply_Lawsfilter.h"

// This function applies specific Law's filter on the image
double applyLawsFilter(double arr[], int len, int k)
{
    int kernels[25] =
    {
        1, 4, 6, 4, 1,
        -1, -2, 0, 2, 1,
        -1, 0, 2, 0, -1,
        -1, 2, 0, -2, 1,
        -1, -4, 6, -4, 1,
    };
    
    // Which kernel go to the tensor product
    int A[5] = {kernels[(int)(floor(k/5))*5], kernels[(int)(floor(k/5))*5+1], kernels[(int)(floor(k/5))*5+2], kernels[(int)(floor(k/5))*5+3], kernels[(int)(floor(k/5))*5+4]};
    int B[5] = {kernels[(k%5)*5], kernels[(k%5)*5+1], kernels[(k%5)*5+2], kernels[(k%5)*5+3], kernels[(k%5)*5+4]};
    //cout << A[0] << "," << A[1] << "," << A[2] << "," << A[3] << "," << A[4] << "," << endl;
    //cout << B[0] << "," << B[1] << "," << B[2] << "," << B[3] << "," << B[4] << "," << endl;
    
    // Calculate tensor product
    int LawsFilter[25];
    for (int j=0; j<5; j++)
    {
        for (int i=0; i<5; i++)
        {
            LawsFilter[j*5+i]=A[j]*B[i];
            //cout << LawsFilter[j*5+i] << endl;
        }
    }
    
    // Apply the filter
    double sum=0;
    double num=0;
    for (int i=0; i<len; i++) {
        sum += LawsFilter[i]*arr[i];
        num ++;
    }
    return double(sum/num);
}

