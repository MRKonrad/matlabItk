#include "util/MIUtil.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]){

    if(nrhs != 2)  {
        mexErrMsgIdAndTxt("MyToolbox:ABC:nrhs", "Two inputs required.");
    }

    if(nlhs != 1)  {
        mexErrMsgIdAndTxt("MyToolbox:ABC:nlhs", "One output required.");
    }

    ImageType::Pointer image0 = getItkImageFromMatlabPtr2d(prhs[0]);
    ImageType::Pointer image1 = getItkImageFromMatlabPtr2d(prhs[1]);

    ImageType::SizeType size = image1->GetLargestPossibleRegion().GetSize();
    plhs[0] = mxCreateDoubleMatrix((mwSize)size[0], (mwSize)size[1], mxREAL);
    fillOutputWithItkImage2d(plhs[0], image1);
}
