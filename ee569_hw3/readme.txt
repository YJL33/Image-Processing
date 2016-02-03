 EE569 Homework Assignment #4
 Date: Nov 13, 2015 
 Name: Yun-Jun Lee 
 USCID: 6742472861 
 E-mail: yunjunle@usc.edu 
 Compiled on OS X 10.9.5 with gcc


 g++ -o xxx xxx.cpp ooo.cpp
 ./program_name [input_parameters]...


 Problem. 1 Optical Character Recognition (OCR)
 (a) Decision Tree
  Pre-processing.cpp               Identify and output the normalized character image                 ./program name    [imput image]     [output directory]


 (b) OCR Testing: Simple Cases
 (c) OCR Testing: Advanced Cases

 Problem. 3 Morphological Processing

 (a) Shrinking
  3.a.1 Shrinking
  Shrink.cpp                       Shrink the object (white/black) in the horseshoe.raw image         ./program name    [imput image]     [output image]
  hitormiss_1.cpp                  Pick and mark the candidate from input data (as erasing candidate)
  hitormiss_1.h
  hitormiss_2.cpp                  Pick and mark the desired pixels from marks (to prevent erase)
  hitormiss_2.h
  
  3.a.2 Shrinking with hole-filing pre-processing
  StructureElement.cpp             Apply "closing" technique by structure element as hole-filing pre-processing, then apply shrinking (3.a.1).
                                   ./program name    [imput image]     [output image]

  3.a.3 Object Counter
  CountObjects.cpp                 Count the number of "objects" in the input image.

 (b) Thinning and Skeletonizing
  3.b.1 Thinning
  Thinning.cpp                     Thinning the object in the horse1.raw image             ./program name    [imput image]     [output image]
  hitormiss_1t.cpp                 Pick and mark the candidate from input data (as erasing candidate)
  hitormiss_1t.h
  hitormiss_2t.cpp                 Pick and mark the desired pixels from marks (to prevent erase)
  hitormiss_2t.h

  3.b.1.5 Skeletoning
  Thinning.cpp                     Skeletoning the object in the horse1.raw image          ./program name    [imput image]     [output image]
  hitormiss_1t.cpp                 Pick and mark the candidate from input data (as erasing candidate)
  hitormiss_1t.h
  hitormiss_2t.cpp                 Pick and mark the desired pixels from marks (to prevent erase)
  hitormiss_2t.h
  
  3.b.2 Shrinking with hole-filing pre-processing
  StructureElement.cpp             Apply structure element pre-processing, then apply thinning(3.b.1) and skeletoning(3.b.1.5).
                                   ./program name    [imput image]     [output image]




 
 
