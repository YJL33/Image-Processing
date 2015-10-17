EE569 Homework Assignment #2 P3
 Date: Sep 30, 2015 
 Name: Yun-Jun Lee 
 USCID: 6742472861 
 E-mail: yunjunle@usc.edu 
 Compiled on OS X 10.9.5 with gcc / Matlab

 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Special Thanks to: 
   "Synergism in Low Level Vision" by Christopher M. Christoudias, Bogdan Georgescu and Peter Meer, in 16th International Conference on Pattern Recognition, 2002.
   Robust Image Understanding Lab (RIUL), from Department of Electrical and Computer Engineering, Rutgers University.
   
   "SLIC Superpixels" by Radhakrishna Achanta, Appu Shaji, Kevin Smith, Aurelien Lucchi, Pascal Fua, and Sabine Süsstrunk, in EPFL Technical Report no. 149300, June 2010."
   Image and Visual Representation Lab (IVRL), member of the National Competence Center in Research on Mobile Information and Communication Systems (NCCR-MICS), supported by the Swiss National Science Foundation
   
   "Robust Image Segmentation Using Contour-guided Color Palettes" by Xiang Fu, Chien-Yi Wang, Chen Chen, Changhu Wang and C.-C. Jay Kuo, in ICCV 2015.
   Media Communications Lab (MCL), Department of Electrical Engineering, University of Southern California

 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Problem. 3 Image Segmentation

 (a) Mean Shift + Superpixel Segmentation
  (EXE files, requires Microsoft Windows OS)

  Mean-shift: 2002_MS.zip  ........................................  from   http://coewww.rutgers.edu/riul/research/code/EDISON/      (Download Windows executable (GUI))

  Superpixel(Provided as reference): 2010_SLIC.zip  ...............  from   http://ivrl.epfl.ch/research/superpixels                  (Download Precompiled binaries)

 (b) Color Palettes Generation

  Contour-guided Color Palettes:
  CCPSegmentation.m            Line 34: Mean-shift parameter  .....  from   https://github.com/fuxiang87/MCL_CCP
                               Line 41: input image
 
 (c) Segmentation Result Evaluation
  
  BSDS500  ........................................................  (not included, Berkeley Segmentation Data Set and Benchmarks 500)
  SegmentationBenchmark ...........................................  Image Segmentation Benchmark Indices Package, University of California, Berkeley. 2007.
