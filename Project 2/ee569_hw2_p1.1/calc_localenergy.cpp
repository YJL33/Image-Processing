//
//  calc_localenergy.cpp
//  ee569_hw2_p1.1
//
//  Created by Yun-Jun Lee on 9/28/15.
//  Copyright (c) 2015 USC. All rights reserved.
//

#include "calc_localenergy.h"

// This function returns the variance from input array and input length.
double calcLocalEnergy(double arr[], int len)
{
    double sum=0;
    for (int i = 0; i < len; i++)
    {
        sum += arr[i];
    }
    double mean = (sum/len);
    double tmp=0;
    for (int i = 0; i < len; i++) {
        tmp += (arr[i] - mean) * (arr[i] - mean);
    }
    return (tmp/len);
}
