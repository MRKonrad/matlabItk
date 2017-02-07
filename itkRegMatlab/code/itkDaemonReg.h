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

#include "itkSymmetricForcesDemonsRegistrationFilter.h"
#include "itkHistogramMatchingImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkWarpImageFilter.h"

//#include "itkImageRegistrationMethodv4.h"
//#include "itkMeanSquaresImageToImageMetricv4.h"
//
//#include "itkBSplineTransform.h"
//#include "itkLBFGSOptimizerv4.h"
//#include "itkBSplineTransformParametersAdaptor.h"
//
//#include "itkResampleImageFilter.h"
//#include "itkCastImageFilter.h"
//#include "itkSquaredDifferenceImageFilter.h"
//
//#include "itkIdentityTransform.h"
//
//#include "itkBSplineTransformInitializer.h"
//#include "itkTransformToDisplacementFieldFilter.h"

#include "MIUtil.h"

int itkDaemonReg(const ImageType::Pointer fixedImage, const ImageType::Pointer movingImage, ImageType::Pointer registeredImage, DisplacementFieldImageType::Pointer displacementField);

#endif /* itkBSplineReg_h */
