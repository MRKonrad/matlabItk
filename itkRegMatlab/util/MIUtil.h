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
DisplacementFieldImageType::Pointer emptyDisplacementFieldImage(int nRows, int nCols);
ImageType::Pointer getItkImageFromMatlabPtr2d(const mxArray *ptr);
void fillOutputWithItkImage2d(mxArray *ptr, ImageType::Pointer image);
void fillOutputWithItkDispField2d(mxArray *ptr, DisplacementFieldImageType::Pointer image);

// templates
template< typename AnImageType, typename AFilterType >
int filterToImage(AnImageType* image, AFilterType* filter){
    typedef itk::ImageRegionConstIterator< AnImageType > ConstIteratorType;
    typedef itk::ImageRegionIterator< AnImageType > IteratorType;
    ConstIteratorType inputIt( filter->GetOutput(), filter->GetOutput()->GetLargestPossibleRegion() );
    IteratorType outputIt( image, image->GetLargestPossibleRegion() );

    inputIt.GoToBegin();
    outputIt.GoToBegin();

    while( !inputIt.IsAtEnd() ){
        outputIt.Set( inputIt.Get() );
        //outputIt.Value() = inputIt.Value();
        ++inputIt;
        ++outputIt;
    }
    image->Update();

    return 0;
}

#endif
