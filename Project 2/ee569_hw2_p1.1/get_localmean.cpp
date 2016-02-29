//
//  get_localmean.cpp
//  ee569_hw2_p1.1
//
//  Created by Yun-Jun Lee on 9/28/15.
//  Copyright (c) 2015 USC. All rights reserved.
//

#include "get_localmean.h"

// This function returns the local mean from input array and input length.
double getLocalMean(double arr[], int len)
{
    double sum=0;
    for (int i = 0; i < len; i++)
    {
        sum += arr[i];
    }
    return double(sum/len);
}
