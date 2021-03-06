# ComputerVision-Opencv
Opencv Projects

## histogram specification

## bitplanes.cpp
### Splits an 8-bit grayscale image into 8 seperate bit plane images and displays each bit plane plus the source image.
- Language: c++
- Required Arguments: image file 
- Usage Example: ./bitplanes path/to/image

## blurborder.cpp
### Blurs the border of a color image by making the colors pastel. The border blur starts at the distance specified by a trackbar.
- Language: C++
- Required Arguments: image file
- Usage Example: ./blurborder path/to/color/image

## enhance.cpp
### Enhances the quality of an image using a 3x3 Laplacian spatial filter.
- Language: C++
- Required Arguments: image file
- Usage Example: ./enhance path/to/image

## smooth.cpp
### Uses a circular kernel to smooth an image. The kernel's coefficients are based on the kernel's resolution and distance from the center.
- Language: C++
- Required Arguments: image file
- Optional Arguments: 
 - -r [int]: changes resolution (default: 1024)
 - -k [int]: changes kernel size (default: 5)
- Usage Example:
 - ./smooth_filter path/to/image
 - ./smooth_filter -r 64 -k 7 path/to/image

## zoom.cpp
### Uses a mouse callback function to specify an image ROI to zoom in on, and then puts the zoomed ROI back into the source image.
- Language: C++
- Required Arguments: image file
- Optional arguments: -z [int]: changes zoom factor (default: 5)
- Usage Example: ./zoom -z 3 path/to/image
