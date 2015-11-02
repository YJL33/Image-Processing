//
//  hitormiss_1k.cpp
//  ee569_hw3_p3.b.1.5
//
//  Pick and mark the candidate from input data
//
//  Created by Yun-Jun Lee on 10/28/15.
//  Copyright (c) 2015 USC. All rights reserved.
//

#include "hitormiss_1k.h"

// This function applies stage 1 hit or miss filter.
unsigned char HitOrMiss1k(unsigned char arr[], int len)
{
    if (len <9) {
        // Edge point => not interested in, just return 0.
        return 0;
    }
    
    // Calculate rank
    unsigned char result=0;
    int rank=-1;
    
    for (int i = 0; i < len; ++i)
    {
        rank += arr[i]* (1 + (i%2));
    }
    // Debug:
    // printf("rank = %d => ", rank);
    
    // Apply filter
    if (rank == 4)
    {
        int filter_number = 8;        // Shrink = 4
        unsigned char filter[][9] =
        {
            {0,1,0, 0,1,1, 0,0,0},{0,1,0, 1,1,0, 0,0,0},
            {0,0,0, 0,1,1, 0,1,0},{0,0,0, 1,1,0, 0,1,0},     // Additional 4 Thinning
            {0,0,1, 0,1,1, 0,0,1},{1,1,1, 0,1,0, 0,0,0},
            {1,0,0, 1,1,0, 1,0,0},{0,0,0, 0,1,0, 1,1,1}
        };
        
        // Compare the elements
        int match=0;
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
            }
        }
        result = match;
    }
    
    // Apply filter        (HERES THE DIFFERENCE B/W THINNING AND SKELETON)
    else if (rank == 6)
    {
        int filter_number = 8;
        unsigned char filter[][9] =
        {
            {1,1,1, 0,1,1, 0,0,0},{0,1,1, 0,1,1, 0,0,1},{1,1,1, 1,1,0, 0,0,0},{1,1,0, 1,1,0, 1,0,0},
            {1,0,0, 1,1,0, 1,1,0},{0,0,0, 1,1,0, 1,1,1},{0,0,0, 0,1,1, 1,1,1},{0,0,1, 0,1,1, 0,1,1}
        };
        
        // Compare the elements
        int match=0;
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
            }
        }
        result = match;
    }
    
    // Apply filter
    else if (rank == 7)
    {
        int filter_number = 4;
        unsigned char filter[][9] =
        {
            {1,1,1,0,1,1,0,0,1},{1,1,1,1,1,0,1,0,0},
            {1,0,0,1,1,0,1,1,1},{0,0,1,0,1,1,1,1,1}
        };
        
        // Compare the elements
        int match=0;
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
            }
        }
        result = match;
    }
    
    // Apply filter
    else if (rank == 8)
    {
        int filter_number = 4;
        unsigned char filter[][9] =
        {
            {1,1,1,1,1,1,0,0,0},{1,1,0,1,1,0,1,1,0},
            {0,0,0,1,1,1,1,1,1},{0,1,1,0,1,1,0,1,1}
        };
        
        // Compare the elements
        int match=0;
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
            }
        }
        result = match;
    }
    
    // Apply filter
    else if (rank == 9)
    {
        int filter_number = 8;
        unsigned char filter[][9] =
        {
            {1,1,1,1,1,1,1,0,0},{1,1,1,1,1,0,1,1,0},
            {1,1,1,1,1,1,0,0,1},{1,1,0,1,1,0,1,1,1},
            {1,0,0,1,1,1,1,1,1},{1,1,1,0,1,1,0,1,1},
            {0,0,1,1,1,1,1,1,1},{0,1,1,0,1,1,1,1,1}
        };
        
        // Compare the elements
        int match=0;
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
            }
        }
        result = match;
    }
    
    // Apply filter
    else if (rank == 10)
    {
        int filter_number = 4;
        unsigned char filter[][9] =
        {
            {1,1,1,1,1,1,1,0,1},{1,1,1,1,1,0,1,1,1},
            {1,0,1,1,1,1,1,1,1},{1,1,1,0,1,1,1,1,1},
        };
        
        // Compare the elements
        int match=0;
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
            }
        }
        result = match;
    }
    
    // Apply filter
    else if (rank == 11)
    {
        int filter_number = 4;
        unsigned char filter[][9] =
        {
            {1,1,1,1,1,1,1,1,0},{1,1,1,1,1,1,0,1,1},
            {0,1,1,1,1,1,1,1,1},{1,1,0,1,1,1,1,1,1},
        };
        
        // Compare the elements
        int match=0;
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
            }
        }
        result = match;
    }
    
    return result;
}
