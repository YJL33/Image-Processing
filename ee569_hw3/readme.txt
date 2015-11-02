 EE569 Homework Assignment #3
 Date: Oct 18, 2015 
 Name: Yun-Jun Lee 
 USCID: 6742472861 
 E-mail: yunjunle@usc.edu 
 Compiled on OS X 10.9.5 with gcc


 g++ -o xxx xxx.cpp ooo.cpp
 ./program_name [input_parameters]...


 Problem. 1 Image General Modification

 (a) Swirl Effect
  Swirl.cpp                       Apply swirl effect on 512x512 Kate.raw image.                ./program name    [imput image]     [output image]
  
 (b) Perspective Transformation & Imaging Geometry
  1.b Pre-Processing:
  getCubeAndPerspectiveView.m     Put 200x200 RGB different baby images on 5 sides of cube in assigned order, and then get perspective view. 
                                  [Input:  change the file path at Line 19, 37, 55, 73, 91]
                              
 
 Problem. 2 Digital Halftoning

 (a) Dithering
  1.a.1 Using Index = 4:
  Dithering_I4.cpp   Output dithered 512x512 mandrill.raw (0 & 255)          ./program name    [imput image]     [output image]

  1.a.2 Using Index = 8:
  Dithering_I8.cpp   Output dithered 512x512 mandrill.raw (0 & 255)          ./program name    [imput image]     [output image]

  1.a.3 Screen with 4 intensity levels:
  Dithering_T4.cpp   Output dithered 512x512 mandrill.raw (4 levels)         ./program name    [imput image]     [output image]

 (b) Error Diffusion
  1.b.1 Using Floyd-Steinberg's method:
  ErrorDiffusion_FS.cpp            Apply error-diffusion filter on mandrill.raw                ./program name    [imput image]     [output image]

  1.b.2 Using Jarvis, Judice, and Ninke's (JJN) method:
  ErrorDiffusion_JJN.cpp           Apply error-diffusion filter on mandrill.raw                ./program name    [imput image]     [output image]

  1.b.3 Using Stucki's method:
  ErrorDiffusion_Stucki.cpp        Apply error-diffusion filter on mandrill.raw                ./program name    [imput image]     [output image]

 (c) Scalar Color Halftoning
  ScalarHalftoning.cpp             Apply error-diffusion filter on sailboat.raw (color image)  ./program name    [imput image]     [output image]

 (d) Vector Color Halftoning
  VectorHalftoning.cpp             Apply error-diffusion filter on sailboat.raw (color image)  ./program name    [imput image]     [output image]

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




 
 
