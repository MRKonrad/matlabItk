#include "MIUtil.h"

ImageType::Pointer getItkImageFromMatlabPtr2d(const mxArray *ptr){

    if( !mxIsDouble(ptr) ){
        mexErrMsgIdAndTxt("MyToolbox:ABC:notDouble", "Input must be type double.");
    }

    if( mxGetNumberOfDimensions(ptr) != 2){
        mexErrMsgIdAndTxt("MATLAB:mxgeteps:numDimensionsMismatch", "Input must be 2D.");
    }

    const mwSize *inputDism = mxGetDimensions(ptr);
    int nRows = inputDism[0];
    int nCols = inputDism[1];
    double* inputPtr = (double *)mxGetPr(ptr);

    ImageType::Pointer image = ImageType::New();
    ImageType::SizeType size;
    size[0] = nRows;
    size[1] = nCols;
    ImageType::IndexType start = {{ 0, 0}};

    ImageType::RegionType region;
    region.SetSize(size);
    region.SetIndex(start);
    image->SetRegions(region);
    image->Allocate();

    double spacing[] = { 1, 1};
    double origin[] = { 0, 0};
    image->SetSpacing(spacing);
    image->SetOrigin(origin);

    IteratorType it (image, region);
    it.GoToBegin();
    const double *data = inputPtr;
    while (!it.IsAtEnd()){
        it.Set(*data);
        ++it;
        ++data;
    }

    return image;
}

void fillOutputWithItkImage2d(mxArray *ptr, ImageType::Pointer image){
    ImageType::SizeType size = image->GetLargestPossibleRegion().GetSize();
    ImageType::RegionType region;
    region.SetSize(size);
    ImageType::IndexType start = {{ 0, 0}};
    region.SetIndex(start);

    IteratorType itOut (image, region);
    itOut.GoToBegin();
    double *dataOut = mxGetPr(ptr);
    while (!itOut.IsAtEnd()){
        *dataOut = (double)itOut.Get();
        ++itOut;
        ++dataOut;
    }
}
