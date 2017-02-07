//
//  itkBSplineReg.cpp
//  itkBSplineReg
//
//  Created by Konrad Werys on 2/6/17.
//
//

#include "itkFemReg.h"

int itkFemReg(ImageType::Pointer fixedImage, ImageType::Pointer movingImage, ImageType::Pointer registeredImage, DisplacementFieldImageType::Pointer displacementField){

    RegistrationType::Pointer registrationFilter = RegistrationType::New();
    registrationFilter->SetMaxLevel(1);
    registrationFilter->SetUseNormalizedGradient( true );
    registrationFilter->ChooseMetric( 0 );
    
    unsigned int maxiters = 20;
    float        E = 100;
    float        p = 1;
    registrationFilter->SetElasticity(E, 0);
    registrationFilter->SetRho(p, 0);
    registrationFilter->SetGamma(1., 0);
    registrationFilter->SetAlpha(1.);
    registrationFilter->SetMaximumIterations( maxiters, 0 );
    registrationFilter->SetMeshPixelsPerElementAtEachResolution(4, 0);
    registrationFilter->SetWidthOfMetricRegion(1, 0);
    registrationFilter->SetNumberOfIntegrationPoints(2, 0);
    registrationFilter->SetDoLineSearchOnImageEnergy( 0 );
    registrationFilter->SetTimeStep(1.);
    registrationFilter->SetEmployRegridding(false);
    registrationFilter->SetUseLandmarks(false);
    
    // Rescale the image intensities so that they fall between 0 and 255
    typedef itk::RescaleIntensityImageFilter<ImageType,ImageType> FilterType;
    FilterType::Pointer movingrescalefilter = FilterType::New();
    FilterType::Pointer fixedrescalefilter = FilterType::New();
    
    movingrescalefilter->SetInput(movingImage);
    fixedrescalefilter->SetInput(fixedImage);
    
    const double desiredMinimum =  0.0;
    const double desiredMaximum =  255.0;
    
    movingrescalefilter->SetOutputMinimum( desiredMinimum );
    movingrescalefilter->SetOutputMaximum( desiredMaximum );
    movingrescalefilter->UpdateLargestPossibleRegion();
    fixedrescalefilter->SetOutputMinimum( desiredMinimum );
    fixedrescalefilter->SetOutputMaximum( desiredMaximum );
    fixedrescalefilter->UpdateLargestPossibleRegion();
    
    // Histogram match the images
    typedef itk::HistogramMatchingImageFilter<ImageType,ImageType> HEFilterType;
    HEFilterType::Pointer IntensityEqualizeFilter = HEFilterType::New();
    
    IntensityEqualizeFilter->SetReferenceImage( fixedrescalefilter->GetOutput() );
    IntensityEqualizeFilter->SetInput( movingrescalefilter->GetOutput() );
    IntensityEqualizeFilter->SetNumberOfHistogramLevels( 100);
    IntensityEqualizeFilter->SetNumberOfMatchPoints( 15);
    IntensityEqualizeFilter->ThresholdAtMeanIntensityOn();
    IntensityEqualizeFilter->Update();
    
    // Set the images for registration filter
    registrationFilter->SetFixedImage(fixedrescalefilter->GetOutput());
    registrationFilter->SetMovingImage(IntensityEqualizeFilter->GetOutput());
    
    itk::fem::MaterialLinearElasticity::Pointer m;
    m = itk::fem::MaterialLinearElasticity::New();
    m->SetGlobalNumber(0);
    // Young's modulus of the membrane
    m->SetYoungsModulus(registrationFilter->GetElasticity());
    m->SetCrossSectionalArea(1.0);  // Cross-sectional area
    m->SetThickness(1.0);           // Thickness
    m->SetMomentOfInertia(1.0);     // Moment of inertia
    m->SetPoissonsRatio(0.);        // Poisson's ratio -- DONT CHOOSE 1.0!!
    m->SetDensityHeatProduct(1.0);  // Density-Heat capacity product
    
    // Create the element type
    ElementType::Pointer e1=ElementType::New();
    e1->SetMaterial(m.GetPointer());
    registrationFilter->SetElement(e1.GetPointer());
    registrationFilter->SetMaterial(m);
    
    registrationFilter->RunRegistration();
    
    typedef itk::ImageDuplicator< ImageType > ImageDuplicatorType;
    ImageDuplicatorType::Pointer imageDuplicator = ImageDuplicatorType::New();
    imageDuplicator->SetInputImage(registrationFilter->GetWarpedImage());
    imageDuplicator->Update();
    
    filterToImage<ImageType,ImageDuplicatorType>(registeredImage, imageDuplicator);
    
    typedef itk::ImageRegionConstIterator< RegistrationType::FieldType > ConstIteratorType;
    typedef itk::ImageRegionIterator< DisplacementFieldImageType > IteratorType;
    ConstIteratorType inputIt( registrationFilter->GetDisplacementField(), registrationFilter->GetDisplacementField()->GetLargestPossibleRegion() );
    IteratorType outputIt( displacementField, displacementField->GetLargestPossibleRegion() );
    
    inputIt.GoToBegin();
    outputIt.GoToBegin();
    
    while( !inputIt.IsAtEnd() ){
        outputIt.Set( inputIt.Get() );
        ++inputIt;
        ++outputIt;
    }
    displacementField->Update();
    
    return EXIT_SUCCESS;

}
