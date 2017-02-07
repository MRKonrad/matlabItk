#ifndef MIUTIL_H
#define MIUTIL_H

#include "itkImage.h"
#include "itkImageRegionIterator.h"

#include "mex.h"
#include "matrix.h"

#include <iostream>

const   unsigned int ImageDimension = 2;
typedef double PixelType;
typedef itk::Image< PixelType, ImageDimension > ImageType;

typedef itk::Vector< double, ImageDimension > VectorPixelType;
typedef itk::Image< VectorPixelType, ImageDimension > DisplacementFieldImageType;

typedef itk::ImageRegionIterator< ImageType > IteratorType;
typedef itk::ImageRegionIterator< DisplacementFieldImageType > IteratorDispFieldType;


ImageType::Pointer emptyImage(int nRows, int nCols);
ImageType::Pointer getItkImageFromMatlabPtr2d(const mxArray *ptr);
void fillOutputWithItkImage2d(mxArray *ptr, ImageType::Pointer image);
void fillOutputWithItkDispField2d(mxArray *ptr, DisplacementFieldImageType::Pointer image);

#endif
