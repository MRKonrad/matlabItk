#include "util/MIUtil.h"
#include "code/itkBSplineReg.h"
#include "itkAddImageFilter.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){

    if(nrhs != 2)  {
        mexErrMsgIdAndTxt("MyToolbox:ABC:nrhs", "Two inputs required.");
    }

    if(nlhs != 2)  {
        mexErrMsgIdAndTxt("MyToolbox:ABC:nlhs", "Two outputs required.");
    }
    
    // input
    ImageType::Pointer fixedImage  = getItkImageFromMatlabPtr2d(prhs[0]);
    ImageType::Pointer movingImage = getItkImageFromMatlabPtr2d(prhs[1]);
    
    ImageType::SizeType itkSize = fixedImage->GetLargestPossibleRegion().GetSize();
    
    ImageType::Pointer registeredImage = emptyImage(itkSize[0], itkSize[1]);
    DisplacementFieldImageType::Pointer displacementField = DisplacementFieldImageType::New();
    // processing
    
    itkBSplineRegMulti(fixedImage, movingImage, registeredImage,  displacementField);
    
//    typedef itk::AddImageFilter <ImageType, ImageType, ImageType > AddImageFilterType;
//    AddImageFilterType::Pointer addFilter = AddImageFilterType::New();
//    addFilter->SetInput1(fixedImage);
//    addFilter->SetInput2(movingImage);
//    addFilter->Update();
//    registeredImage = addFilter->GetOutput();
//    registeredImage->Update();
    
    std::cout << "ggg" << std::endl;
    
    // output
    mwSize matlabSize[3];
    matlabSize[0] = itkSize[0];
    matlabSize[1] = itkSize[1];
    matlabSize[2] = 2;
    
    plhs[0] = mxCreateNumericArray(2, matlabSize, mxDOUBLE_CLASS, mxREAL);
    fillOutputWithItkImage2d(plhs[0], registeredImage);
    
    plhs[1] = mxCreateNumericArray(3, matlabSize, mxDOUBLE_CLASS, mxREAL);
    
}