//
//  itkBSplineReg.cpp
//  itkBSplineReg
//
//  Created by Konrad Werys on 2/6/17.
//
//

#include "itkBSplineReg.h"

#include "itkAddImageFilter.h"
#include "itkImageToImageFilter.h"
#include "itKImageToImageFilter.h"

int itkBSplineReg(const ImageType::Pointer fixedImage, const ImageType::Pointer movingImage, ImageType::Pointer registeredImage, DisplacementFieldImageType::Pointer displacementField){

    typedef itk::AddImageFilter <ImageType, ImageType > AddImageFilterType;
    AddImageFilterType::Pointer addFilter = AddImageFilterType::New();
    addFilter->SetInput1(fixedImage);
    addFilter->SetInput2(movingImage);

    addFilter->Update();

    std::cout << "eee" << std::endl;
    filterToImage(registeredImage, (itkImageToImageFilterType::Pointer) addFilter);
    
    return 0;
}

int filterToImage(ImageType::Pointer image, itkImageToImageFilterType::Pointer filter ){
    
    typedef itk::ImageRegionConstIterator< ImageType > ConstIteratorType;
    typedef itk::ImageRegionIterator< ImageType > IteratorType;
    ConstIteratorType inputIt( filter->GetOutput(), filter->GetOutput()->GetLargestPossibleRegion() );
    IteratorType outputIt( image, image->GetLargestPossibleRegion() );
    
    inputIt.GoToBegin();
    outputIt.GoToBegin();
    
    while( !inputIt.IsAtEnd() ){
        outputIt.Set( inputIt.Get() );
        ++inputIt;
        ++outputIt;
    }
    image->Update();
    
    return 0;
}

int itkBSplineRegMulti(ImageType::Pointer fixedImage, ImageType::Pointer movingImage, ImageType::Pointer registeredImage, DisplacementFieldImageType::Pointer &displacementField){

    const unsigned int SpaceDimension = ImageDimension;
    const unsigned int SplineOrder = 3;
    typedef double CoordinateRepType;
    
    typedef itk::BSplineTransform<
    CoordinateRepType,
    SpaceDimension,
    SplineOrder > TransformType;
    
    typedef itk::LBFGSOptimizerv4 OptimizerType;
    
    typedef itk::MeanSquaresImageToImageMetricv4<
    ImageType,
    ImageType >    MetricType;
    
    typedef itk::ImageRegistrationMethodv4<
    ImageType,
    ImageType >    RegistrationType;
    
    MetricType::Pointer         metric        = MetricType::New();
    OptimizerType::Pointer      optimizer     = OptimizerType::New();
    RegistrationType::Pointer   registration  = RegistrationType::New();
    
    
    registration->SetMetric(        metric        );
    registration->SetOptimizer(     optimizer     );
    
    registration->SetFixedImage ( fixedImage   );
    registration->SetMovingImage( movingImage  );
    
    TransformType::Pointer  outputBSplineTransform = TransformType::New();
    
    // Initialize the fixed parameters of transform (grid size, etc).
    //
    typedef itk::BSplineTransformInitializer<
    TransformType,
    ImageType> InitializerType;
    
    InitializerType::Pointer transformInitializer = InitializerType::New();
    
    unsigned int numberOfGridNodesInOneDimension = 8;
    
    TransformType::MeshSizeType meshSize;
    meshSize.Fill( numberOfGridNodesInOneDimension - SplineOrder );
    
    transformInitializer->SetTransform( outputBSplineTransform );
    transformInitializer->SetImage( fixedImage );
    transformInitializer->SetTransformDomainMeshSize( meshSize );
    transformInitializer->InitializeTransform();
    
    // Set transform to identity
    //
    typedef TransformType::ParametersType     ParametersType;
    const unsigned int numberOfParameters =
    outputBSplineTransform->GetNumberOfParameters();
    ParametersType parameters( numberOfParameters );
    parameters.Fill( 0.0 );
    outputBSplineTransform->SetParameters( parameters );
    
    registration->SetInitialTransform( outputBSplineTransform );
    registration->InPlaceOn();

    const unsigned int numberOfLevels = 3;
    
    RegistrationType::ShrinkFactorsArrayType shrinkFactorsPerLevel;
    shrinkFactorsPerLevel.SetSize( numberOfLevels );
    shrinkFactorsPerLevel[0] = 3;
    shrinkFactorsPerLevel[1] = 2;
    shrinkFactorsPerLevel[2] = 1;
    
    RegistrationType::SmoothingSigmasArrayType smoothingSigmasPerLevel;
    smoothingSigmasPerLevel.SetSize( numberOfLevels );
    smoothingSigmasPerLevel[0] = 2;
    smoothingSigmasPerLevel[1] = 1;
    smoothingSigmasPerLevel[2] = 0;
    
    registration->SetNumberOfLevels( numberOfLevels );
    registration->SetSmoothingSigmasPerLevel( smoothingSigmasPerLevel );
    registration->SetShrinkFactorsPerLevel( shrinkFactorsPerLevel );

    RegistrationType::TransformParametersAdaptorsContainerType adaptors;
    
    // First, get fixed image physical dimensions
    TransformType::PhysicalDimensionsType             fixedPhysicalDimensions;
    for( unsigned int i=0; i< SpaceDimension; i++ )
    {
        fixedPhysicalDimensions[i] = fixedImage->GetSpacing()[i] *
        static_cast<double>(
                            fixedImage->GetLargestPossibleRegion().GetSize()[i] - 1 );
    }
    
    // Create the transform adaptors specific to B-splines
    for( unsigned int level = 0; level < numberOfLevels; level++ )
    {
        typedef itk::ShrinkImageFilter<
        ImageType,
        ImageType> ShrinkFilterType;
        ShrinkFilterType::Pointer shrinkFilter = ShrinkFilterType::New();
        shrinkFilter->SetShrinkFactors( shrinkFactorsPerLevel[level] );
        shrinkFilter->SetInput( fixedImage );
        shrinkFilter->Update();
        
        // A good heuristic is to double the b-spline mesh resolution at each level
        //
        TransformType::MeshSizeType requiredMeshSize;
        for( unsigned int d = 0; d < ImageDimension; d++ )
        {
            requiredMeshSize[d] = meshSize[d] << level;
        }
        
        typedef itk::BSplineTransformParametersAdaptor<TransformType>
        BSplineAdaptorType;
        BSplineAdaptorType::Pointer bsplineAdaptor = BSplineAdaptorType::New();
        bsplineAdaptor->SetTransform( outputBSplineTransform );
        bsplineAdaptor->SetRequiredTransformDomainMeshSize( requiredMeshSize );
        bsplineAdaptor->SetRequiredTransformDomainOrigin(
                                                         shrinkFilter->GetOutput()->GetOrigin() );
        bsplineAdaptor->SetRequiredTransformDomainDirection(
                                                            shrinkFilter->GetOutput()->GetDirection() );
        bsplineAdaptor->SetRequiredTransformDomainPhysicalDimensions(
                                                                     fixedPhysicalDimensions );
        
        adaptors.push_back( bsplineAdaptor.GetPointer() );
    }
    
    registration->SetTransformParametersAdaptorsPerLevel( adaptors );
    
    // Scale estimator
    typedef itk::RegistrationParameterScalesFromPhysicalShift<MetricType> ScalesEstimatorType;
    ScalesEstimatorType::Pointer scalesEstimator = ScalesEstimatorType::New();
    scalesEstimator->SetMetric( metric );
    scalesEstimator->SetTransformForward( true );
    scalesEstimator->SetSmallParameterVariation( 1.0 );
    
    // Set Optimizer
    optimizer->SetScalesEstimator( scalesEstimator );
    optimizer->SetGradientConvergenceTolerance( 10 );
    optimizer->SetLineSearchAccuracy( 0.9 );
    optimizer->SetDefaultStepLength( 5 );
    optimizer->TraceOn();
    optimizer->SetMaximumNumberOfFunctionEvaluations( 10 );
    
    std::cout << "Starting Registration "<< std::endl;
    
    try
    {
        registration->Update();
        std::cout << "Optimizer stop condition = "
        << registration->GetOptimizer()->GetStopConditionDescription()
        << std::endl;
    }
    catch( itk::ExceptionObject & err )
    {
        std::cerr << "ExceptionObject caught !" << std::endl;
        std::cerr << err << std::endl;
        return EXIT_FAILURE;
    }
    
    // Finally we use the last transform in order to resample the image.
    //
    typedef itk::ResampleImageFilter<
    ImageType,
    ImageType >    ResampleFilterType;
    
    ResampleFilterType::Pointer resample = ResampleFilterType::New();
    
    resample->SetTransform( outputBSplineTransform );
    resample->SetInput( movingImage );
    
    resample->SetSize(    fixedImage->GetLargestPossibleRegion().GetSize() );
    resample->SetOutputOrigin(  fixedImage->GetOrigin() );
    resample->SetOutputSpacing( fixedImage->GetSpacing() );
    resample->SetOutputDirection( fixedImage->GetDirection() );
    resample->SetDefaultPixelValue( 100 );
    resample->Update();
    filterToImage(registeredImage, (itkImageToImageFilterType::Pointer) resample);
    //registeredImage -> Update();
    
    // Generate the explicit deformation field resulting from
    // the registration.

    typedef itk::TransformToDisplacementFieldFilter<
    DisplacementFieldImageType,
    CoordinateRepType >             DisplacementFieldGeneratorType;
    
    /** Create an setup displacement field generator. */
    DisplacementFieldGeneratorType::Pointer dispfieldGenerator =
    DisplacementFieldGeneratorType::New();
    dispfieldGenerator->UseReferenceImageOn();
    dispfieldGenerator->SetReferenceImage( fixedImage );
    dispfieldGenerator->SetTransform( outputBSplineTransform );
    try
    {
        dispfieldGenerator->Update();
    }
    catch ( itk::ExceptionObject & err )
    {
        std::cerr << "Exception detected while generating deformation field";
        std::cerr << " : "  << err << std::endl;
        return EXIT_FAILURE;
    }
    
    displacementField = dispfieldGenerator->GetOutput();
    
    return EXIT_SUCCESS;

}
