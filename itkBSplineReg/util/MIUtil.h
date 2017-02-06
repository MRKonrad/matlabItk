#ifndef MIUTIL_H
#define MIUTIL_H

#include "itkImage.h"
#include "itkImageRegionIterator.h"

#include "mex.h"
#include "matrix.h"

#include <iostream>

typedef itk::Image< double, 2 > ImageType;
typedef itk::ImageRegionIterator< ImageType > IteratorType;

ImageType::Pointer emptyImage(int nRows, int nCols);
ImageType::Pointer getItkImageFromMatlabPtr2d(const mxArray *ptr);
void fillOutputWithItkImage2d(mxArray *ptr, ImageType::Pointer image);

#endif
