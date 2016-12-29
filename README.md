# ComputerVision-Opencv
Opencv Projects

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

## smooth_filter.cpp
### Uses a circular kernel to smooth an image. The kernel's coefficients are based on the resolution and distance from the center of the kernel.
- Language: C++
- Required Arguments: image file
- Optional Arguments: 
 - -r [int]: changes resolution (default: 1024)
 - -k [int]: changes kernel size (default: 5)
- Usage Example:
- - ./smooth_filter path/to/image
- - ./smooth_filter -r 64 -k 7 path/to/image
