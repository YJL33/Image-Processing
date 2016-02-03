//
//  Pre-treatment.cpp
//  HW4_P1.b.0.1
//
//  Label each connected objects (characters) in Test_ideal1.raw
//  Extract the characters from the labeled image,
//  and Output as different images in same size (Black character, 224x245) for further usage
//
//  Steps:
//  0. Read the image (620x759 24-bit)
//  1. Distinguish the background and object/character
//  2. Label the pixels according to the connectivity.
//    i. Label all pixels
//    ii. For each object, merge all labels on its' pixels into one
//    iii. Re-organize the labels
//  3. Specify the rectangle area of each object/character
//  4. Extract all characters
//  5. Put all characters in images with identical desired dimension (boundary box, 224x245)
//     (Note that here "enclosed area" and "background" are outputed as 254 and 255)
//  6. Output all character images
//
//  Difference b/w 1.a.0
//  1. The image size is different
//  2. Set a threshold (interested area) when converting the image to grey-level
//  3. Re-label only characters, based on not only labels but also on the object actual size (count the pixel, threshold obtained from observation)
//
//  Input: Training image (training.raw)
//  Output: Character images (8-bit white background image.raw, 224x245)
//
//  Created by Yun-Jun Lee.
//  Copyright (c) 2015 USC. All rights reserved.

#include <iostream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#ifndef LABELCARACTERS
#define LABELCARACTERS

using namespace std;

int main(int argc, const char * argv[])
{
    clock_t begin = clock();
    FILE *file;
    int BytesPerPixel=3, Height = 759, Width = 620;               // Define the variables
    int boundary_box_width=224, boundary_box_height=245;            // The size of desired size of boundary box

    //argv[1] = "/Users/YJLee/Desktop/P4_Output/P1/Test_ideal1.raw"
    //argv[2] = "/Users/YJLee/Desktop/P4_Output/P1/test1/"
    //argv[3] = "/Users/YJLee/Desktop/P4_Output/P1/test1/Labeled.raw"  (if necessary)
    
    cout << "Syntax: program_name [input_image.raw] [directory for output_image.raw] [labeled_image.raw (if desired)]" << endl;
    cout << "Please add the slash in the end, e.g. '/Users/Desktop/characters/' " << endl;
    printf("\nDesired output (unknown) Character image: %d*%d (pixels), 8-bit white background image\n\n", boundary_box_width, boundary_box_height);

    
    if (argc < 3){                                                // Check for proper syntax
        cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
        cout << "program_name [input_image.raw] [directory for output images] [labeled_image.raw (if desired)]" << endl;
        return 0;
    }
    
    unsigned char Imagedata[Height][Width][BytesPerPixel];        // Read the image contents by fread(ptr,size,count,fp)
    
    if (!(file=fopen(argv[1],"rb"))) {
        cout << "Error: unable to open file" <<endl;
        exit(1);
    }
    fread(Imagedata, sizeof(unsigned char), Height*Width*BytesPerPixel, file);
    fclose(file);
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // Step 1. Convert to grey level
    unsigned char GreyImage[Height][Width][1];                    // Convert to grey image
    float convert_ratio[3] = {0.2989, 0.5870, 0.1140};            // RGB coefficient for converting
    
    for (int y = 0; y < Height; ++y)
    {
        for (int x = 0; x < Width; ++x)
        {
            double sum=0;
            for (int ch=0; ch < BytesPerPixel; ++ch) {
                sum += convert_ratio[ch]*Imagedata[y][x][ch];
            }
            GreyImage[y][x][0] = 255-sum;                         // Here we want object !=0, background=0 (change background back to 255 later before step 4),
        }
    }
    for (int y = 0; y < Height; ++y)
    {
        for (int x = 0; x < Width; ++x)
        {
            if ((x<60) || (x>Width-60) || (y<60) || (y>Height-60)) {
                GreyImage[y][x][0] = 0;                          // Here we eliminate the undesired boundaries
            }
        }
    }


    // Step 2. label all pixels, then make all pixels belong to one single connected object all share one one label

    unsigned char Label[Height][Width][1];        // Label map
    bool LabeledOrNot[Height][Width][1];          // Whether this pixel is already labeled or not
    unsigned char NextLabel = 1;                  // Label of Connectivity

    unsigned char Linked[512*256] = {0};          // The smallest number of each label can be replaced with
                                                  // e.g. ( Linked[5]=1, means label "5" is can be replaced with label 1)

    for (int y=0; y < Height; ++y)                // Label all pixels
    {
        for (int x=0; x < Width; ++x)
        {
            if (GreyImage[y][x][0] < 128)                  // This pixel is background
            {
                Label[y][x][0] = 0;                        // Label the background as 0
                LabeledOrNot[y][x][0] = true;
            }
            if (GreyImage[y][x][0] > 128)                  // This pixel is target
            {
                unsigned char neighbor_label[3*3] = {0};   // This pixel's neighborhood (including itself)
                int neighbor_count = 0;
            
                for (int j = y - 1; j <= y + 1; ++j){      // Get this pixel's neighborhood
                    for (int i = x - 1; i <= x + 1; ++i){
                        if ((j >= 0) && (j < Height) && (i >= 0) && (i < Width)){
                            if ((GreyImage[j][i][0] > 128) && (LabeledOrNot[j][i][0] == true)) {      // This neighbor is target
                                neighbor_label[neighbor_count] = Label[j][i][0];
                                neighbor_count++;
                                //printf("neighbor #%d: (%d, %d)= %d\n", neighbor_count, i, j, Label[j][i][0]);
                                //printf("get its value: %d\n",  neighbor_label[neighbor_count-1]);
                            }
                        }
                    }
                }
                
                if (neighbor_count == 0)                   // All neighbors are empty
                {
                    Linked[NextLabel] = NextLabel;
                    Label[y][x][0] = NextLabel;            // Label this pixel (using new label)
                    LabeledOrNot[y][x][0] = true;
                    
                    //printf("empty - \nLabeling (%d, %d) as: %d\n\n", x,y, Label[y][x][0]);
                    NextLabel++;
                }
                
                else                                       // Some neighbors are labeled
                {
                    int MinLabel = 255;                    // Get the smallest neighbor's label
                    for (int i =0; i<=neighbor_count-1; i++) {
                        if (MinLabel > neighbor_label[i]) {
                            MinLabel = neighbor_label[i];
                        }
                    }
                    //printf("not empty, neighbor count = %d, Labeling (%d, %d) as %d\n", neighbor_count, x, y, MinLabel);

                    Label[y][x][0] = MinLabel;             // Label this pixel (using smallest neighbor's label)
                    LabeledOrNot[y][x][0] = true;
                    
                    for (int i =0; i<=neighbor_count; i++) {
                        int lvl = (int)neighbor_label[i];
                        if (Linked[lvl] > MinLabel)
                        {
                            Linked[lvl] = MinLabel;        // Mark all other labels in this neighborhood, since they can be replaced with "the smallest one".
                            //printf("Current position: (%d, %d); Neighbor_count: %d; Neighbor: %d; Neighbor's label: %d\n", x, y, neighbor_count,i, neighbor_label[i]);
                            //printf("********Updating info: Now label %d is linked with %d********\n\n", lvl ,MinLabel);
                        }
                    }
                }
            }
            //printf("(%d, %d) is labeled as %d\n", x, y, Label[y][x][0]);
        }
    }
    
    
    for (int y=0; y<Height; ++y)                           // Combine multiple labels within one object by replacing all labels with the minimum one.
    {
        for (int x=0; x<Width; ++x)
        {
            int lvl = Label[y][x][0];
            int old_lvl = lvl;
            if ((Label[y][x][0] != 0) && (Label[y][x][0] != Linked[lvl]))    // This non-background pixel having a label can be replaced with a smaller one
            {
                //printf("(%d, %d)= %d\n", x,y, Label[y][x][0]);
                while (Linked[lvl] != lvl) {                                 // Keep trace the root of the smaller label, until it can not replaced by other one.
                    //printf("Label: %d, Should be: %d, ", lvl, Linked[lvl]);
                    lvl = Linked[lvl];
                }
                Linked[old_lvl] = Linked[lvl];
                Label[y][x][0] = Linked[lvl];                                // Label this pixel with the smallest one (root)
                //printf("Updated and Re-labeled as: %d\n", Linked[lvl]);
            }
        }
    }
    
    for (int x=0; x<Width; ++x) {
        for (int y=0; y<Height; ++y) {
            if (Label[y][x][0] == 0)
            {
                Label[y][x][0] = 255;                                // Restore the Background to 255
            }
        }
    }
    
    int object_count=0;                                              // Count the object/character (background doesnt count, beginning from -1)
    
    for (int i=0; i<NextLabel; ++i) {                                // Check all labels
        if (Linked[i] == i) {                                        // If this label have no root => it's a connected object.
            int pixel_count = 0;                                     // However, connected object != character, but character = connected object
            for (int y=0; y<Height; ++y) {
                for (int x=0; x<Width; ++x) {
                    if (Label[y][x][0] == i) {
                        pixel_count++;                               // Count the size of this label
                    }
                }
            }
            if ((pixel_count > 1000) && (pixel_count < 33910)) {     // Determine whether this is a character or not: based on actual size (pixel count)
                object_count++;                                      // If size is correct => count it as object (threshold is obtained from observation)
                if ((object_count != i) && (pixel_count > 1000) && (pixel_count < 33910)) {      // If this object has a wrong number of label
                    printf("Remaining Label: %d, Relabel as: %d\n", i, object_count);
                    for (int x=0; x<Width; ++x) {
                        for (int y=0; y<Height; ++y) {
                            if (Label[y][x][0] == i) {               // Re-organize its label
                                Label[y][x][0] = object_count;       // Make the remaining labeling continuous: relabel all pixel as count number
                            }
                        }
                    }
                }
                else {printf("Remaining Label: %d\n", i);}           // If this object has a correct number of label
            }
            else
            {
                for (int y=0; y<Height; ++y) {
                    for (int x=0; x<Width; ++x) {
                        if (Label[y][x][0] == i) {
                            Label[y][x][0] = 255;                    // Eliminate this label
                        }
                    }
                }
                printf("Remaining Label: %d, incorrect size\n", i);
            }
        }
    }
    printf("\nTotal Character Counts: %d, background (label=0) are excluded\n\n", object_count);
    
    // Step 3. Specify the rectangle area of each object
    
    int min_x, max_x, min_y, max_y;                                    // boundary of each object (character)
    
    for (int object_id=1; object_id<=object_count; ++object_id) {
        min_x=Width, max_x=0, min_y=Height, max_y=0;                   // Initialize
        for (int x=0; x<Width; ++x) {
            for (int y=0; y<Height; ++y) {
                if (Label[y][x][0] == object_id)                       // Find the boundary of this character
                {
                    if (x < min_x) {
                        min_x = x;
                    };
                    if (x > max_x) {
                        max_x = x;
                    }
                    if (y < min_y) {
                        min_y = y;
                    };
                    if (y > max_y) {
                        max_y = y;
                    }
                }
            }
        }
        
        // Step 4. Extract each character
        
        int object_height = (max_y-min_y+1) , object_width = (max_x-min_x+1);
        unsigned char old_character[object_height][object_width][1];           // The rectangle area where character is in training image
        
        printf("For character %d, Boundary box Area = %dx%d, ", object_id, object_width, object_height);
        
        for (int x=0; x<=object_width; ++x) {
            for (int y=0; y<=object_height; ++y) {
                int old_y = y+min_y;
                int old_x = x+min_x;
                if (Label[old_y][old_x][0] == object_id) {
                    old_character[y][x][0] = 0;
                }
                else
                {
                    old_character[y][x][0] = 254;                                  // The background and enclosed area is now 254
                }
            }
        }
        
        // Step 5. "Paste" character inside desired dimension

        unsigned char normalized_character[boundary_box_height][boundary_box_width][1];  // The desired output retangle area of each character
        int top=floor((boundary_box_height-object_height)/2);                            // blank area around the character
        int left=floor((boundary_box_width-object_width)/2);
        
        for (int y=0; y<boundary_box_height; y++){
            for (int x=0; x<boundary_box_width; x++){
                normalized_character[y][x][0] = 255;                               // Initialize the desired output image as blank (255)
            }
        }
        
        for (int y=0; y<object_height; y++){
            for (int x=0; x<object_width; x++){
                normalized_character[y+top][x+left][0] = old_character[y][x][0];   // "Paste" the character in the blank image
            }
        }
        
        int wrong_background_count = 1;                                            // Now, some background pixels from old characters are 254
        while (wrong_background_count != 0) {
            wrong_background_count = 0;
            for (int y=0; y<boundary_box_height; y++){
                for (int x=0; x<boundary_box_width; x++){
                    if (normalized_character[y][x][0] == 254) {                    // Not sure whether its background or enclosed area
                        for (int j = y - 1; j <= y + 1; j++){
                            for (int i = x - 1; i <= x + 1; i++){
                                if (j >= 0 && j < boundary_box_height && i >= 0 && i < boundary_box_width){
                                    if (normalized_character[j][i][0] == 255) {    // It has a neighbor = 255 (background)
                                        wrong_background_count++;                  // Count it
                                        normalized_character[y][x][0] = 255;       // And correct it to 255
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        
        /////////////////////////////////////////////////////////////////////////////////////////////////
        
        // Step 6. Save the object in boundary box
        
        char filepath[] = {0};
        char filename[] = "%02d.raw";           // Combine the directory and filename
        strcat(filepath, argv[2]);
        strcat(filepath, filename);
        sprintf(filepath, filepath, (char)(object_id));
        
        if (!(file=fopen(filepath,"wb"))) {
            cout << "Error: unable to save character image" << endl;
            exit(1);
        }
        
        fwrite(normalized_character, sizeof(unsigned char), boundary_box_height*boundary_box_width*1, file);
        fclose(file);
        printf("Saved as: %s (%dx%d)\n", filepath, boundary_box_width, boundary_box_height);          // Output the character image
    }
    
    if (argc == 4){
        if (!(file=fopen(argv[3],"wb"))) {
            cout << "Error: unable to save labeled image" << endl;
            exit(1);
        }
        
        fwrite(Label, sizeof(unsigned char), Height*Width*1, file);
        fclose(file);
        printf("\nLabeled image Saved as: %s (%dx%d)\n", argv[3], Width, Height);                       // Output the Label image
    
    }
    
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("elapsed time: %f sec\n", elapsed_secs);
    return 0;
}
#endif