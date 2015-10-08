//
//  nms.cpp
//  ee569_hw2_p2.a.3
//
//  Created by Yun-Jun Lee on 9/28/15.
//  Copyright (c) 2015 USC. All rights reserved.
//

#include "nms.h"

// This function returns the local maximal from input array and input length.

unsigned char NMS(unsigned char arr[], int len)
{
    unsigned char max=0;
    for (int i = 0; i < len; i++)
    {
    	if (arr[i] > max)
    	{
    		max = arr[i];
    	}
    }
    return max;
}
