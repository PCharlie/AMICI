#ifndef AMI_MATLAB_H
#define AMI_MATLAB_H

#include <mex.h>

#undef AMI_HDF5_H_DEBUG

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif


EXTERNC double getDoubleScalarAttribute_MATLAB(mxArray *prhs, const char* optionsObject, const char* attributeName);
EXTERNC int getIntScalarAttribute_MATLAB(mxArray *prhs, const char* optionsObject, const char* attributeName);
EXTERNC void getDoubleArrayAttribute_MATLAB(mxArray *prhs, const char* optionsObject, const char* attributeName, double **destination, hsize_t *length);
EXTERNC void getIntArrayAttribute_MATLAB(mxArray *prhs, const char* optionsObject, const char* attributeName, int **destination, hsize_t *length);
EXTERNC void getDoubleArrayAttribute2D_MATLAB(mxArray *prhs, const char* optionsObject, const char* attributeName, double **destination, hsize_t *m, hsize_t *n);


EXTERNC void createAndWriteDouble2DAttribute_MATLAB(mxArray *plhs, const char *attributeName, const double *buffer, hsize_t m, hsize_t n);
EXTERNC void createAndWriteDouble3DAttribute_MATLAB(mxArray *plhs, const char *attributeName, const double *buffer, hsize_t m, hsize_t n, hsize_t o);
EXTERNC void setAttributeIntFromDouble_MATLAB(mxArray *plhs, const char *obj_name, const char *attr_name, const double *bufferDouble, size_t size );

#endif
