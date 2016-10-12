#include <assert.h>

#include "ami_MATLAB.h"
#include "wrapfunctions.h"
#include "include/symbolic_functions.h"

#include "include/rdata_accessors.h"
#include "include/edata_accessors.h"
#include "include/udata_accessors.h"


double getDoubleScalarAttribute_MATLAB(mxArray prhs, const char* optionsObject, const char* attributeName) {
    if(mxGetProperty(prhs,0,optionsObject)) {
        return((double)mxGetScalar(mxGetProperty(prhs,0,optionsObject)));
    } else {
        warnMsgIdAndTxt("AMICI:mex:OPTION",strcat(strcat("Could not read field ",optionsObject)," from provided input!"));
        return(NULL);
    }
}

int getIntScalarAttribute_MATLAB(mxArray plhs, const char* optionsObject, const char* attributeName) {
    if(mxGetProperty(prhs,0,optionsObject)) {
        return((int)mxGetScalar(mxGetProperty(prhs,0,optionsObject)));
    } else {
        warnMsgIdAndTxt("AMICI:mex:OPTION",strcat(strcat("Could not read field ",optionsObject)," from provided input!"));
        return(NULL);
    }
}

void getDoubleArrayAttribute_MATLAB(mxArray plhs, const char* optionsObject, const char* attributeName, double **destination, int *length) {
    if(mxGetProperty(prhs,0,optionsObject)) {
        return((double *)mxGetData(mxGetProperty(prhs,0,optionsObject)));
    } else {
        warnMsgIdAndTxt("AMICI:mex:OPTION",strcat(strcat("Could not read field ",optionsObject)," from provided input!"));
        *length = 0;
        return(NULL);
    }
}

void getIntArrayAttribute_MATLAB(mxArray plhs, const char* optionsObject, const char* attributeName, int **destination, int *length) {
    if(mxGetProperty(prhs,0,optionsObject)) {
        return((int *)mxGetData(mxGetProperty(prhs,0,optionsObject)));
    } else {
        warnMsgIdAndTxt("AMICI:mex:OPTION",strcat(strcat("Could not read field ",optionsObject)," from provided input!"));
        *length = 0;
        return(NULL);
    }
}

