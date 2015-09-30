 EE569 Homework Assignment #2
 Date: Sep 30, 2015 
 Name: Yun-Jun Lee 
 USCID: 6742472861 
 E-mail: yunjunle@usc.edu 
 Compiled on OS X 10.9.5 with gcc / Xcode
 
 OpenCV 3.0.0 is required.

 Problem. 1 Texture analysis and classification
 
  1.1 Feature extraction:                          
   FeatureExtraction.cpp  [the full description of sets of files]........... e.g. "../part_a/unknown_%02d.raw"
                          [number of samples]............................... 36 labeled samples or 24 unlabeled samples
                          [output "feature array"].......................... 2-D array: [25 Law's features] * [number of samples (24 or 36)]
   get_localmean.cpp
   get_localmean.h
   apply_Lawsfilter.cpp
   apply_Lawsfilter.h
   calc_LocalEnergy.cpp
   calc_LocalEnergy.h
 
  1.2 Minimum mean distance classifier:
   MinMeanDistClassifier.cpp  [class A (grass) mean-features].................. 2-D array obtained from 1.1
                              [class B (straw) mean-features].................. 2-D array obtained from 1.1
                              [unknown images' features]....................... 2-D array obtained from 1.1


 
 demosaicing - bilinear interpolation:        Demosaic_BL.cpp
 demosaicing - MHC interpolation:             Demosaic_MHC.cpp
 
 Problem. 2
 Equalization - transfer function:            Equalization_TransferFunction.cpp
 Equalization - bucket filling method:        Equalization_BucketFilling.cpp
 
 Quantize the image(4 or 8):                  QuantizeRGB.cpp
 Impose the oil-paint filter:                 OilPaint.cpp
 
 Problem. 3
 Get PSNR value:                              getPSNR.cpp
 Remove impulse noise:                        RemoveNoise_LinearFilter.cpp
 Remove additive noise:                       RemoveNoise_MedianFilter.cpp
 Remove gaussian noise:                       RemoveNoise_BilateralFilter.cpp
 
 Apply Guided filter:                         convertColorToGray.cpp
                                              guidedfilter.m
                                              RmNoise_GuidedFilter.m
                                              ssim.m
