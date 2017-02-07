//
//  itkBSplineReg.cpp
//  itkBSplineReg
//
//  Created by Konrad Werys on 2/6/17.
//
//

#include "itkDaemonReg.h"

int itkDaemonReg(ImageType::Pointer fixedImage, ImageType::Pointer movingImage, ImageType::Pointer registeredImage, DisplacementFieldImageType::Pointer displacementField){

    typedef itk::HistogramMatchingImageFilter<
    ImageType,
    ImageType >   MatchingFilterType;
    MatchingFilterType::Pointer matcher = MatchingFilterType::New();
    
    matcher->SetInput( movingImage );
    matcher->SetReferenceImage( fixedImage );
    matcher->SetNumberOfHistogramLevels( 1024 );
    matcher->SetNumberOfMatchPoints( 7 );
    matcher->ThresholdAtMeanIntensityOn();
    
    typedef itk::SymmetricForcesDemonsRegistrationFilter<
    ImageType,
    ImageType,
    DisplacementFieldImageType> RegistrationFilterType;
    RegistrationFilterType::Pointer filter = RegistrationFilterType::New();
    
    filter->SetFixedImage( fixedImage );
    filter->SetMovingImage( matcher->GetOutput() );
    filter->SetNumberOfIterations( 1000 );
    filter->SetStandardDeviations( 3.0 );
    filter->Update();
    
    typedef itk::WarpImageFilter<
    ImageType,
    ImageType,
    DisplacementFieldImageType  >     WarperType;
    typedef itk::LinearInterpolateImageFunction<
    ImageType,
    double          >  InterpolatorType;
    WarperType::Pointer warper = WarperType::New();
    InterpolatorType::Pointer interpolator = InterpolatorType::New();
    
    warper->SetInput( movingImage );
    warper->SetInterpolator( interpolator );
    warper->SetOutputSpacing( fixedImage->GetSpacing() );
    warper->SetOutputOrigin( fixedImage->GetOrigin() );
    warper->SetOutputDirection( fixedImage->GetDirection() );
    warper->SetDisplacementField( filter->GetOutput() );
    warper->Update();
    
    filterToImage<ImageType,WarperType>(registeredImage, warper);
    filterToImage<DisplacementFieldImageType,RegistrationFilterType>(displacementField, filter);
    
    return EXIT_SUCCESS;

}
