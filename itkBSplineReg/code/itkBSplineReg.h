//
//  itkBSplineReg.hpp
//  itkBSplineReg
//
//  Created by Konrad Werys on 2/6/17.
//
//

#ifndef itkBSplineReg_h
#define itkBSplineReg_h

#include <stdio.h>

#include "itkImageRegistrationMethodv4.h"
#include "itkMeanSquaresImageToImageMetricv4.h"

#include "itkBSplineTransform.h"
#include "itkLBFGSOptimizerv4.h"
#include "itkBSplineTransformParametersAdaptor.h"

#include "itkResampleImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkSquaredDifferenceImageFilter.h"

#include "itkIdentityTransform.h"

#include "itkBSplineTransformInitializer.h"
#include "itkTransformToDisplacementFieldFilter.h"

#include "../util/MIUtil.h"

//const   unsigned int ImageDimension = 2;

//typedef double PixelType;
//typedef itk::Vector< double, ImageDimension > VectorPixelType;
//
//typedef itk::Image< PixelType, ImageDimension >       ImageType;
//typedef itk::Image< VectorPixelType, ImageDimension > DisplacementFieldImageType;

typedef itk::ImageToImageFilter <ImageType, ImageType> itkImageToImageFilterType;
int filterToImage(ImageType::Pointer image, itkImageToImageFilterType::Pointer filter );

int itkBSplineReg(const ImageType::Pointer fixedImage, const ImageType::Pointer movingImage, ImageType::Pointer registeredImage, DisplacementFieldImageType::Pointer displacementField);
int itkBSplineRegMulti(const ImageType::Pointer fixedImage, const ImageType::Pointer movingImage, ImageType::Pointer registeredImage, DisplacementFieldImageType::Pointer &displacementField);

#endif /* itkBSplineReg_h */
