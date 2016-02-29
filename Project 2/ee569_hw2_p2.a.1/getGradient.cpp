//
//  getGradient.h
//  ee569_hw2_p2.a.1
//
//  Created by Yun-Jun Lee on 9/28/15.
//  Copyright (c) 2015 USC. All rights reserved.
//
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "getGradient.h"

// This function applies specific Sobel Detector on the image
double getGradient(double arr[], int len, bool k)
{
    int horizontal[9] =
    {
        -1, 0, 1, -2, 0, 2, -1, 0, 1,
    };
    
    int vertical[9] =
    {
        -1, -2, -1, 0, 0, 0, 1, 2, 1,
    };
    
    // Apply Sobel
    
    double sum=0;
    //double num_x=0, num_y=0;
    
    if (k == true)
    {
        for (int i=0; i<len; ++i) {
            sum += horizontal[i]*arr[i];
            //num_x ++;
        }
    }
    
    else
    {
        for (int i=0; i<len; ++i) {
            sum += vertical[i]*arr[i];
            //num_y ++;
        }
    }
    
    return sum;
}

