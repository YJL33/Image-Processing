 EE569 Homework Assignment #4
 Date: Nov 13, 2015 
 Name: Yun-Jun Lee 
 USCID: 6742472861 
 E-mail: yunjunle@usc.edu 
 Compiled on OS X 10.9.5 with gcc


 g++ -o xxx xxx.cpp ooo.cpp
 ./program_name [input_parameters]...


 Problem. 1 Optical Character Recognition (OCR)
 (a) Minimum Mean Distance Classifier
  
  Pre-processing
   1.a.0.1 Pre-treatment.cpp            ./program name    [input image]    [directory of character image]    [labeled image(if needed)]
    Label each characters (connected objects) in training.raw,
    Extract the characters from the labeled image,
    and Output as different images in same size (Black character, 64x70) for further usage

   1.a.0.2 SmoothCharacters.cpp         ./program name    [directory of character image]   [directory of smoothed character image]

    In order to simulate a different font, use Structure Element to smooth characters.    

  Classification
   1.a.1 FeatureExtraction.cpp
   1.a.2 DecisionTree.cpp            Decision Tree: (Minimum Mean Distance Classifier is applied)

 (b) OCR Testing: Simple Cases

  Pre-processing
   1.b.0.1 Pre-treatment_(for Test_ideal1.raw).cpp
   1.b.0.2 Pre-treatment (for Test_ideal2.raw).cpp

  Classification
   1.b.1 DecisionTree (for Test_ideal1.raw).cpp
   1.b.2 DecisionTree (for Test_ideal2.raw).cpp

 (c) OCR Testing: Advanced Cases
  
  Pre-processing 
   1.c.0 Equalization_TransferFunction.cpp          Enhance the contrast of Test_night.raw
   1.c.0.1 Pre-treatment_(for Test_night.raw).cpp
   1.c.0.2 Pre-treatment_(for Test_shade.raw).cpp
  
  Classification
   1.c.1 DecisionTree (for Test_night.raw).cpp
   1.c.2 DecisionTree (for Test_shade.raw).cpp


 Problem. 2 Contour Modeling
   2.b Level-set
   ee569_hw4_p2.b.m      modified from Level-set demo

 Problem. 3 SIFT and Image Matching
   3.a SIFT
   ee569_hw4_p3.a.m      modified from SIFT demo



 
 
