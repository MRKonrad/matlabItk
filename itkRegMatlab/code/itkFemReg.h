//
//  itkBSplineReg.hpp
//  itkBSplineReg
//
//  Created by Konrad Werys on 2/6/17.
//
//

#ifndef itkBSplineReg_h
#define itkBSplineReg_h

#include "MIUtil.h"

#include "itkRescaleIntensityImageFilter.h"
#include "itkHistogramMatchingImageFilter.h"
#include "itkFEMRegistrationFilter.h"
#include "itkImageDuplicator.h"

//typedef itk::Image<unsigned char, 2>                     DiskImageType;
//typedef itk::Image<float, 2>                             ImageType;
typedef itk::fem::Element2DC0LinearQuadrilateralMembrane ElementType;
typedef itk::fem::Element2DC0LinearTriangularMembrane    ElementType2;
typedef itk::fem::FEMObject<2>                           FEMObjectType;
typedef itk::fem::FEMRegistrationFilter<ImageType,ImageType,FEMObjectType>
RegistrationType;

int itkFemReg(const ImageType::Pointer fixedImage, const ImageType::Pointer movingImage, ImageType::Pointer registeredImage, DisplacementFieldImageType::Pointer displacementField);

#endif /* itkBSplineReg_h */
