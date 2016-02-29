//
//  hitormiss_2t.cpp
//  ee569_hw3_p3.b.1
//
//  Pick and mark the desired pixels from marks
//
//  Created by Yun-Jun Lee on 10/28/15.
//  Copyright (c) 2015 USC. All rights reserved.
//

#include "hitormiss_2t.h"

// This function applies stage 1 hit or miss filter.
unsigned char HitOrMiss2t(unsigned char arr[], int len)
{
    if (len <9) {
        // Edge point => not interested in, just return 1. (kill it anyway)
        return 1;
    }
    
    unsigned char result;    // Since these are "unconditional pattern", therefore if any match, the candidate should be kept. => result = 1 (to be kill)
    
    bool match=0;     // if match any pattern = 1
    
    // Spot
    if ((match == 0) && (arr[0] == 0 && arr[1] == 0 && arr[2] == 0 && arr[3] == 0 && arr[5] == 0 && arr[6] == 0 && arr[7] == 0 && arr[8] == 0)) {
        match = 1;
        //printf("Spot\n");
    }
    
    // Spur
    if ((match == 0) && (arr[2] == 1) && (arr[0] == 0 && arr[1] == 0 && arr[3] == 0 && arr[5] == 0 && arr[6] == 0 && arr[7] == 0 && arr[8] == 0)) {
        match = 1;
        //printf("Spur\n");
    }
    if ((match == 0) && (arr[0] == 1) && (arr[1] == 0 && arr[2] == 0 && arr[3] == 0 && arr[5] == 0 && arr[6] == 0 && arr[7] == 0 && arr[8] == 0)) {
        match = 1;
        //printf("Spur\n");
    }
    
    // Single 4-connection
    if ((match == 0) && (arr[7] == 1) && (arr[0] == 0 && arr[1] == 0 && arr[2] == 0 && arr[3] == 0 && arr[5] == 0 && arr[6] == 0 && arr[8] == 0)) {
        match = 1;
        //printf("S-4\n");
    }
    if ((match == 0) && (arr[5] == 1) && (arr[0] == 0 && arr[1] == 0 && arr[2] == 0 && arr[3] == 0 && arr[6] == 0 && arr[7] == 0 && arr[8] == 0)) {
        match = 1;
        //printf("S-4\n");
    }

    // L-Cluster  (as same as hit-or-miss rank=3)
    if (match == 0)
    {
        int filter_number = 8;
        unsigned char filter[][9] =
        {
            {0,0,1,0,1,1,0,0,0},{0,1,1,0,1,0,0,0,0},
            {1,1,0,0,1,0,0,0,0},{1,0,0,1,1,0,0,0,0},
            {0,0,0,1,1,0,1,0,0},{0,0,0,0,1,0,1,1,0},
            {0,0,0,0,1,0,0,1,1},{0,0,0,0,1,1,0,0,1}
        };
        
        // Compare the elements
        
        bool keep_finding = true;
        for (int i=0; i<filter_number && keep_finding; ++i) {
            bool equal = true;
            for (int j=0; j<9 && equal; ++j) {
                if (filter[i][j] != arr[j]) {
                    equal = false;
                }
            }
            if (equal == true) {
                keep_finding = false;
                match = 1;
                //printf("L-Cluster\n");
            }
        }
    }
    
    // Four-connected  (as same as parts of hit-or-miss rank=5)
    if (match == 0)
    {
        int filter_number = 4;
        unsigned char filter[][9] =
        {
            {1,1,0,0,1,1,0,0,0},{0,1,0,0,1,1,0,0,1},
            {0,1,1,1,1,0,0,0,0},{0,0,1,0,1,1,0,1,0},
        };
        
        // Compare the elements
        
        bool keep_finding = true;
        for (int i=0; i<filter_number && keep_finding; ++i) {
            bool equal = true;
            for (int j=0; j<9 && equal; ++j) {
                if (filter[i][j] != arr[j]) {
                    equal = false;
                }
            }
            if (equal == true) {
                keep_finding = false;
                match = 1;
                //printf("4-Connected\n");
            }
        }
    }
    
    // Spur-corner-cluster
    if ((match == 0) && (arr[2] == 1 && arr[6] == 1) && (arr[1] == 1 || arr[5] == 1) && (arr[0] == 0 && arr[3] == 0 && arr[7] == 0 && arr[8] == 0)) {
        match = 1;
        //printf("Spur-Corner-Cluster\n");
    }
    if ((match == 0) && (arr[2] == 1 && arr[6] == 1) && (arr[3] == 1 || arr[7] == 1) && (arr[0] == 0 && arr[1] == 0 && arr[5] == 0 && arr[8] == 0)) {
        match = 1;
        //printf("Spur-Corner-Cluster\n");
    }
    if ((match == 0) && (arr[0] == 1 && arr[8] == 1) && (arr[1] == 1 || arr[3] == 1) && (arr[2] == 0 && arr[5] == 0 && arr[6] == 0 && arr[7] == 0)) {
        match = 1;
        //printf("Spur-Corner-Cluster\n");
    }
    if ((match == 0) && (arr[0] == 1 && arr[8] == 1) && (arr[5] == 1 || arr[7] == 1) && (arr[1] == 0 && arr[2] == 0 && arr[3] == 0 && arr[6] == 0)) {
        match = 1;
        //printf("Spur-Corner-Cluster\n");
    }
    
    // Corner-cluster
    if ((match == 0) && (arr[0] == 1 && arr[1] == 1 && arr[3] == 1)) {
        match = 1;
        //printf("Corner-Cluster\n");
    }
    
    // Tee branch
    if ((match == 0) && (arr[1] == 1 && arr[3] == 1 && arr[5] == 1) && (arr[2] == 0 && arr[7] == 0 && arr[8] == 0)) {
        match = 1;
        //printf("Tee-branch\n");
    }
    if ((match == 0) && (arr[1] == 1 && arr[3] == 1 && arr[5] == 1) && (arr[0] == 0 && arr[6] == 0 && arr[7] == 0)) {
        match = 1;
        //printf("Tee-branch\n");
    }
    if ((match == 0) && (arr[1] == 1 && arr[3] == 1 && arr[7] == 1) && (arr[5] == 0 && arr[6] == 0 && arr[8] == 0)) {
        match = 1;
        //printf("Tee-branch\n");
    }
    if ((match == 0) && (arr[1] == 1 && arr[3] == 1 && arr[7] == 1) && (arr[0] == 0 && arr[2] == 0 && arr[5] == 0)) {
        match = 1;
        //printf("Tee-branch\n");
    }
    if ((match == 0) && (arr[1] == 1 && arr[5] == 1 && arr[7] == 1) && (arr[0] == 0 && arr[2] == 0 && arr[3] == 0)) {
        match = 1;
        //printf("Tee-branch\n");
    }
    if ((match == 0) && (arr[1] == 1 && arr[5] == 1 && arr[7] == 1) && (arr[3] == 0 && arr[6] == 0 && arr[8] == 0)) {
        match = 1;
        //printf("Tee-branch\n");
    }
    if ((match == 0) && (arr[3] == 1 && arr[5] == 1 && arr[7] == 1) && (arr[0] == 0 && arr[1] == 0 && arr[6] == 0)) {
        match = 1;
        //printf("Tee-branch\n");
    }
    if ((match == 0) && (arr[3] == 1 && arr[5] == 1 && arr[7] == 1) && (arr[1] == 0 && arr[2] == 0 && arr[8] == 0)) {
        match = 1;
        //printf("Tee-branch\n");
    }
    
    // Vee branch
    if ((match == 0) && (arr[0] == 1 && arr[2] == 1) && (arr[6] == 1 || arr[7] == 1 || arr[8] ==1)) {
        match = 1;
        //printf("Vee-branch\n");
    }
    if ((match == 0) && (arr[0] == 1 && arr[6] == 1) && (arr[2] == 1 || arr[5] == 1 || arr[8] ==1)) {
        match = 1;
        //printf("Vee-branch\n");
    }
    if ((match == 0) && (arr[2] == 1 && arr[8] == 1) && (arr[0] == 1 || arr[3] == 1 || arr[6] ==1)) {
        match = 1;
        //printf("Vee-branch\n");
    }
    if ((match == 0) && (arr[6] == 1 && arr[8] == 1) && (arr[0] == 1 || arr[1] == 1 || arr[2] ==1)) {
        match = 1;
        //printf("Vee-branch\n");
    }
    
    // Diag. branch
    if ((match == 0) && (arr[1] == 1 && arr[5] == 1 && arr[6] == 1) && (arr[2] == 0 && arr[3] == 0 && arr[7] == 0)) {
        match = 1;
        //printf("Diag-branch\n");
    }
    if ((match == 0) && (arr[1] == 1 && arr[3] == 1 && arr[8] == 1) && (arr[0] == 0 && arr[5] == 0 && arr[7] == 0)) {
        match = 1;
        //printf("Diag-branch\n");
    }
    if ((match == 0) && (arr[1] == 0 && arr[5] == 0 && arr[6] == 0) && (arr[2] == 1 && arr[3] == 1 && arr[7] == 1)) {
        match = 1;
        //printf("Diag-branch\n");
    }
    if ((match == 0) && (arr[1] == 0 && arr[3] == 0 && arr[8] == 0) && (arr[0] == 1 && arr[5] == 1 && arr[7] == 1)) {
        match = 1;
        //printf("Diag-branch\n");
    }

    if (match == 1) {
        result = 0;
    }
    else
    {
        result = 1;
    }
    return result;
}
