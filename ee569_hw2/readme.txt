 EE569 Homework Assignment #2
 Date: Sep 30, 2015 
 Name: Yun-Jun Lee 
 USCID: 6742472861 
 E-mail: yunjunle@usc.edu 
 Compiled on OS X 10.9.5 with gcc

 
 OpenCV 3.0.0 is required.
 (1.b.2) libsvm tools are required, contributed by Chih-Chung Chang and Chih-Jen Lin, LIBSVM.

 g++ -o xxx xxx.cpp ooo.cpp
 ./program_name [input_parameters]...


 Problem. 1 Texture analysis and classification
 (a)
  1.1 Feature extraction:                          
   FeatureExtraction.cpp  
                          [the full description of sets of files] .................. e.g. "/part_a/unknown_%02d.raw"
                          [number of samples] ...................................... 24 or 36
                          [output "feature array" for all samples] ................. (25xN) array: [25 Law's filters] * [number of samples (24 or 36)]
   get_localmean.cpp
   get_localmean.h
   apply_Lawsfilter.cpp
   apply_Lawsfilter.h
   calc_LocalEnergy.cpp
   calc_LocalEnergy.h
  
  (for both 1.a and 1.b)

  1.2 Minimum mean distance classifier:
   MinMeanDistClassifier.cpp
                              [unknown images' feature array]....................... 25x24 array obtained from 1.1
                              [class A (grass) feature array]....................... 25x36 array obtained from 1.1
                              [class B (straw) feature array]....................... 25x36 array obtained from 1.1

  1.3 Feature reduction (apply PCA/LDA then apply M-distance classifier):
   FeatureReduction.cpp
                              [unknown images' feature array]....................... 25x24 array obtained from 1.1
                              [class A (grass) feature array]....................... 25x36 array obtained from 1.1
                              [class B (straw) feature array]....................... 25x36 array obtained from 1.1
  
  (b)
  1.b.0 Sample preparation (for PCA/LDA):
   SamplePreparation.cpp 
                              [training Negative sample set 1] ..................... 25x12 array obtained from 1.1
                              [training Negative sample set 2] ..................... 25x12 array obtained from 1.1
                              [training Negative sample set 3] ..................... 25x12 array obtained from 1.1
                              [output training Negative sample set] ................ 25x36 array obtained from 1.1

                              [testing sample set 1] ............................... 25x12 array obtained from 1.1 (if necessary)
                              [testing sample set 2] ............................... 25x12 array obtained from 1.1 (if necessary)
                              [testing sample set 3] ............................... 25x12 array obtained from 1.1 (if necessary)
                              [testing sample set 4] ............................... 25x12 array obtained from 1.1 (if necessary)
                              [output testing sample set] .......................... 25x48 array obtained from 1.1 (if necessary)


  1.b.1 Feature reduction ver.2 (apply PCA/LDA then apply M-distance classifier):
   FeatReduction_ver2.cpp 
                              [Test samples]........................................ 25x48 array obtained from 1.b.0
                              [Positive training image samples]..................... 25x36 array obtained from 1.b.0
                              [Negative training image samples]..................... 25x36 array obtained from 1.b.0
                              
  1.b.2 Sample preparation (for libsvm format):
   SamplePreparationSVM.cpp
                              [Positive training image samples]..................... 25x36 array obtained from 1.b.0
                              [Negative training image samples]..................... 25x36 array obtained from 1.b.0
                              [Test samples]........................................ 25x48 array obtained from 1.b.0 (LABELING is Required)

                              [output Training data] ............................... required format for libsvm/tool
                              [output Testing data] ................................ required format for libsvm/tool

 
 Problem. 2 Edge Detection
 (a) Sobel Edge Detector and Non Maximal Suppression:
  convertColorToGray.cpp     ./program_name input_image.raw gray_image.raw [BytesPerPixel = 3] [Width = 481] [Height = 321]
  
  getSobelImage.cpp          ./program_name gray_image.raw  gradient_image.raw
  getGradient.cpp
  
  SobelThreshold.cpp         ./program_name  gradient_image.raw  [threshold=0~255]  contour_threshold_image.raw
  
  applynms.cpp               ./program_name  contour_threshold_image.raw   contour_nms_image.raw
  nms.cpp
  nms.hpp
   
 (b) Canny Edge Detector
  applyCanny.cpp  (need opencv)          ./program_name 
 (c) Structured Edge
 (d) Performance Evaluation

 Problem. 3 Image Segmentation
 (a) Mean Shift + Superpixel Segmentation
 (b) Color Palettes Generation
 (c) Segmentation Result Evaluation
 
