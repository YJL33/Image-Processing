%  HW3_P1b
%  
%  Generate the cube with correct location/info of 5 images 
%  (The input image PATH is at Line 19, 37, 55, 73, 91 for each 5 image)
%  
%  And then generate the perspective view.
%  (The code is after Line 121)
%  
%
%  Input: Raw square image *5
%  Output: Perspective view
%
%  Created by Yun-Jun Lee on 10/20/15.
%  Copyright (c) 2015 USC. All rights reserved.

    x_size = 200;
    y_size = 200;
    
% Get Baby Human image:
    % readraw - read RAW format RGB image of 200x200 square size into matrix I
    disp(['	Retrieving Image ' '~/HW3_images/P2/baby.raw' ' ...'])
    % Get file ID for file
    f_orig=fopen('/Users/YJLee/Documents/MATLAB/ee569_hw3/HW3_images/P2/baby.raw','rb');
    % Check if file exists
    if (f_orig == -1)
        error('can not open input image file press CTRL-C to exit \n');
        pause
    end
    % Get all the pixels from the image
    OrigRGBImage = fread(f_orig, (x_size * y_size * 3), '*uint8');
    OrigRedChannel = reshape(OrigRGBImage(1:3:end), [y_size, x_size]);
    OrigGreenChannel = reshape(OrigRGBImage(2:3:end), [y_size, x_size]);
    OrigBlueChannel = reshape(OrigRGBImage(3:3:end), [y_size, x_size]);
    Baby = cat(3, OrigRedChannel, OrigGreenChannel, OrigBlueChannel); 
    %%%%%%

% Get Baby Cat image:
    %readraw - read RAW format RGB image of 200x200 square size into matrix I
    disp(['	Retrieving Image ' '~/HW3_images/P2/baby_cat.raw' ' ...']);
    % Get file ID for file
    f_orig=fopen('/Users/YJLee/Documents/MATLAB/ee569_hw3/HW3_images/P2/baby_cat.raw','rb');
    % Check if file exists
    if (f_orig == -1)
        error('can not open input image file press CTRL-C to exit \n');
        pause
    end
    % Get all the pixels from the image
    OrigRGBImage = fread(f_orig, (x_size * y_size * 3), '*uint8');
    OrigRedChannel = reshape(OrigRGBImage(1:3:end), [y_size, x_size]);
    OrigGreenChannel = reshape(OrigRGBImage(2:3:end), [y_size, x_size]);
    OrigBlueChannel = reshape(OrigRGBImage(3:3:end), [y_size, x_size]);
    Baby_cat = cat(3, OrigRedChannel, OrigGreenChannel, OrigBlueChannel); 
    %%%%%%

% Get Baby Dog image:
    %readraw - read RAW format RGB image of 200x200 square size into matrix I
    disp(['	Retrieving Image ' '~/HW3_images/P2/baby_dog.raw' ' ...']);
    % Get file ID for file
    f_orig=fopen('/Users/YJLee/Documents/MATLAB/ee569_hw3/HW3_images/P2/baby_dog.raw','rb');
    % Check if file exists
    if (f_orig == -1)
        error('can not open input image file press CTRL-C to exit \n');
        pause
    end
    % Get all the pixels from the image
    OrigRGBImage = fread(f_orig, (x_size * y_size * 3), '*uint8');
    OrigRedChannel = reshape(OrigRGBImage(1:3:end), [y_size, x_size]);
    OrigGreenChannel = reshape(OrigRGBImage(2:3:end), [y_size, x_size]);
    OrigBlueChannel = reshape(OrigRGBImage(3:3:end), [y_size, x_size]);
    Baby_dog = cat(3, OrigRedChannel, OrigGreenChannel, OrigBlueChannel); 
    %%%%%%

% Get Baby Panda image:
    %readraw - read RAW format RGB image of 200x200 square size into matrix I
    disp(['	Retrieving Image ' '~/HW3_images/P2/baby_panda.raw' ' ...']);
    % Get file ID for file
    f_orig=fopen('/Users/YJLee/Documents/MATLAB/ee569_hw3/HW3_images/P2/baby_panda.raw','rb');
    % Check if file exists
    if (f_orig == -1)
        error('can not open input image file press CTRL-C to exit \n');
        pause
    end
    % Get all the pixels from the image
    OrigRGBImage = fread(f_orig, (x_size * y_size * 3), '*uint8');
    OrigRedChannel = reshape(OrigRGBImage(1:3:end), [y_size, x_size]);
    OrigGreenChannel = reshape(OrigRGBImage(2:3:end), [y_size, x_size]);
    OrigBlueChannel = reshape(OrigRGBImage(3:3:end), [y_size, x_size]);
    Baby_panda = cat(3, OrigRedChannel, OrigGreenChannel, OrigBlueChannel); 
    %%%%%%
% Get Baby Bear image:
    %readraw - read RAW format RGB image of 200x200 square size into matrix I
    disp(['	Retrieving Image ' '~/HW3_images/P2/baby_bear.raw' ' ...']);
    % Get file ID for file
    f_orig=fopen('/Users/YJLee/Documents/MATLAB/ee569_hw3/HW3_images/P2/baby_bear.raw','rb');
    % Check if file exists
    if (f_orig == -1)
        error('can not open input image file press CTRL-C to exit \n');
        pause
    end
    % Get all the pixels from the image
    OrigRGBImage = fread(f_orig, (x_size * y_size * 3), '*uint8');
    OrigRedChannel = reshape(OrigRGBImage(1:3:end), [y_size, x_size]);
    OrigGreenChannel = reshape(OrigRGBImage(2:3:end), [y_size, x_size]);
    OrigBlueChannel = reshape(OrigRGBImage(3:3:end), [y_size, x_size]);
    Baby_bear = cat(3, OrigRedChannel, OrigGreenChannel, OrigBlueChannel); 
    %%%%%%

% Put the image on cube surface:    
    % front
    surface([-1 1; -1 1], [-1 -1; -1 -1], [-1 -1; 1 1], 'FaceColor', 'texturemap', 'CData', Baby_bear );
    % back
    surface([-1 1; -1 1], [1 1; 1 1], [-1 -1; 1 1], 'FaceColor', 'texturemap', 'CData', Baby_dog );
    % left
    surface([-1 -1; -1 -1], [-1 1; -1 1], [-1 -1; 1 1], 'FaceColor', 'texturemap', 'CData', Baby_panda );
    % right
    surface([1 1; 1 1], [-1 1; -1 1], [-1 -1; 1 1], 'FaceColor', 'texturemap', 'CData', Baby_cat );
    % top
    surface([-1 1; -1 1], [-1 -1; 1 1], [1 1; 1 1], 'FaceColor', 'texturemap', 'CData', Baby );
    % bottom
    %surface([-1 1; -1 1], [-1 -1; 1 1], [-1 -1; -1 -1], 'FaceColor', 'texturemap', 'CData', cdatar );
    
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % Get Perspective view
    %camproj perspective
    % Set camera position
    campos('manual');
    campos([5,5,5]);
    % Set camera target
    camtarget('manual');
    camtarget([0,0,0]);
    %
    view();


    % debug:
    % figure();
    % imshow(p);
    
    % Create surfaces on cube
    
    %cdata = Baby; % flipdim( imread('peppers.png'), 1 );
    %%%%%
    fclose(f_orig);
%   fclose(f_comp);

