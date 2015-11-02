//
//  StructureElement.cpp
//  HW3_P3.a.2
//
//  Count the objects in the image
//  Assume that objects are 8-connected
//
//  Steps: (refer to the psuedo code provided by TA)
//  0. Horizontally scan the image and label each pixel.
//  1. Vertically scan the image
//
//
//  Input: Treated image
//  Output: Counts
//
//  Created by Yun-Jun Lee.
//  Copyright (c) 2015 USC. All rights reserved.

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#ifndef COUNT_IMG
#define COUNT_IMG

using namespace std;

int main(int argc, const char * argv[])
{
    clock_t begin = clock();
    FILE *file;
    int BytesPerPixel=1, Height = 108, Width = 91;                      // Define the variables
    bool inverseImage=false;

    //argv[1] = "/Users/YJLee/Desktop/Horseshoe_shrink.raw";
    //argv[2] = "/Users/YJLee/Desktop/Count.raw";
    //argv[3] = 0;
    
    // Check for proper syntax
    if (argc < 3){
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name [input_image.raw] [output_image.raw]" << endl;
        return 0;
    }
    if ((argc = 4) && argv[3] !=0) {
        inverseImage=true;
        cout << "<<<<Black pixels as object in Horseshoe_shrink.raw>>>>" << endl;
    }
    else {
        cout << "<<<<White pixels as object in Horseshoe_shrink.raw>>>>" << endl;
    }
    
    // Read the image contents by fread(ptr,size,count,fp)
    unsigned char Imagedata[108][91][1];
    
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), 108*91*1, file);
    fclose(file);
    
    /////////////////////////////////////////////////////////////////////////////////////////////////
    
    unsigned char Label[108][91][1];
    bool LabeledOrNot[108][91][1];
    
    unsigned char Linked[108*91] = {0};
    // This is used to save the linking between labels, the order is the # of label, and the value is the minimum label it's relevant with
    // e.g. ( Linked[5]=1, means label "5" is has the minimum linked label 1)
    
    unsigned char NextLabel = 1;
    
    // First Pass
    for (int y=0; y < 108; ++y)
    {
        for (int x=0; x < 91; ++x)
        {
            if (Imagedata[y][x][0] == 0)
            {
                Label[y][x][0] = 0;
                LabeledOrNot[y][x][0] = true;
            }
            else
            {
                // Check if the neighbors are empty or not:
                // ( empty means that this pixel is surrounded by background, or surrounded by un-labeled object)
                // Make sure the neighbor is background, or its not labeled yet
                unsigned char neighbor_label[3*3];
                int neighbor_count = 0;
            
                for (int j = y - 1; j <= y + 1; ++j){
                    for (int i = x - 1; i <= x + 1; ++i){
                        if ((j >= 0) && (j < 108) && (i >= 0) && (i < 91)){
                            if ((Imagedata[j][i][0] != 0) && (LabeledOrNot[j][i][0] == true)) {
                                neighbor_label[neighbor_count] = Label[j][i][0];
                                neighbor_count++;
                                
                                //printf("neighbor #%d: (%d, %d)= %d\n", neighbor_count, i, j, Label[j][i][0]);
                                //printf("get its value: %d\n",  neighbor_label[neighbor_count-1]);
                            }
                        }
                    }
                }
                
                // If neighbors are empty
                if (neighbor_count == 0) 
                {
                    Linked[NextLabel] = NextLabel;
                    Label[y][x][0] = NextLabel;
                    LabeledOrNot[y][x][0] = true;
                    
                    //printf("empty - \nLabeling (%d, %d) as: %d\n\n", x,y, Label[y][x][0]);
                    NextLabel++;
                }
                
                // If neighbors are not empty
                else 
                {
                    int MinLabel = 255;          // Get the minimum neighbor label
                    for (int i =0; i<=neighbor_count-1; i++) {
                        if (MinLabel > neighbor_label[i]) {
                            MinLabel = neighbor_label[i];
                        }
                    }
                    // Label the pixel
                    Label[y][x][0] = MinLabel;
                    LabeledOrNot[y][x][0] = true;
                    //printf("not empty, neighbor count = %d, Labeling (%d, %d) as %d\n\n", neighbor_count, x, y, MinLabel);
                    
                    // Link the relevant labels
                    for (int i =0; i<=neighbor_count; i++) {
                        int lvl = (int)neighbor_label[i];
                        
                        if (Linked[lvl] > MinLabel)
                        {
                            Linked[lvl] = MinLabel;
                            //printf("********Updating info: Now label %d is linked with %d********\n", lvl ,MinLabel);
                        }
                    }
                }
            }
            //printf("(%d, %d) is labeled as %d\n", x, y, Label[y][x][0]);
        }
    }
    
    // Second pass
    for (int x=0; x<91; ++x)
    {
        for (int y=0; y<108; ++y)
        {
            // Re-labeling
            int lvl = Label[y][x][0];
            int old_lvl = lvl;
            if ((Label[y][x][0] != 0) && (Label[y][x][0] != Linked[lvl]))
            {
                //printf("(%d, %d)= %d\n", x,y, Label[y][x][0]);
                while (Linked[lvl] != lvl) {
                    //printf("Label: %d, Should be: %d, ", lvl, Linked[lvl]);
                    lvl = Linked[lvl];
                }
                Linked[old_lvl] = Linked[lvl];
                Label[y][x][0] = Linked[lvl];
                //printf("Updated and Re-labeled as: %d\n", Linked[lvl]);
            }
        }
    }
    
    // Debug:
    printf("\n********* Updated Label Table ***********\n");
    int label_count=-1;
    for (int i=0; i<NextLabel; ++i) {
        if (Linked[i] == i) {
            label_count++;
            printf("Label: %d, Value: %d\n", i, Linked[i]);
        }
    }
    printf("\n Total Counts: %d (background excluded)\n*********************************\n", label_count);
    
    // Shown in 1-255   (Background will be 255)
    for (int x=0; x<91; ++x) {
        for (int y=0; y<108; ++y) {
            if (Label[y][x][0] == 0) {
                Label[y][x][0] = 255;
            }
        }
    }
    
    
    /////////////////////////////////////////////////////////////////////////////////////////////////
  
    // Save the output_array into output image by fwrite(), the parameters are similar to fread()
    if (!(file=fopen(argv[2],"wb"))) {
        cout << "Error: unable to save file" << endl;
        exit(1);
    }
    
    fwrite(Label, sizeof(unsigned char), 108*91*1, file);
    fclose(file);
    cout << "\nCounting arrays successfully saved (background=255)" <<endl;

    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("elapsed time: %f sec\n", elapsed_secs);
    return 0;
}
#endif