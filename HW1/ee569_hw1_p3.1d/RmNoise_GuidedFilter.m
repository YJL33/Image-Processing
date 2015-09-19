%  HW1_P3
%  Remove noise via Guided Filter
%  To remove noises in raw square RGB image (512x512 is defaulted according to the homework assignment)
%  The code applies a guided filter ().
%
%  Input: Raw square image, desired output file path, (squared-window size or cross-window arm length), (input size)
%  Output: De-noised square image
%
%  Created by Yun-Jun Lee on 9/13/15.
%  Copyright (c) 2015 USC. All rights reserved.

close all;

I = double(imread('/Users/YJLee/Documents/MATLAB/ee569_img/peppers_noisy.bmp')) / 255;
p = double(imread('/Users/YJLee/Documents/MATLAB/ee569_img/peppers_GF_rpt.bmp')) / 255;
O = double(imread('/Users/YJLee/Documents/MATLAB/ee569_img/peppers.bmp')) / 255;

r = 8; % try r=2, 4, or 8
eps = 0.4^2; % try eps=0.1^2, 0.2^2, 0.4^2

q = zeros(size(I));

q(:, :, 1) = guidedfilter(I(:, :, 1), p(:, :, 1), r, eps);
q(:, :, 2) = guidedfilter(I(:, :, 2), p(:, :, 2), r, eps);
q(:, :, 3) = guidedfilter(I(:, :, 3), p(:, :, 3), r, eps);


figure();
imshow(q);

imwrite(q,'/ee569_img/peppers_GF_rpt.bmp');

%Calc SSIM
K = [0.05 0.05];
window = ones(8);
L = 100;
ssim(I, O, K, window, L)
