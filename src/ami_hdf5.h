#ifndef AMI_HDF5_H
#define AMI_HDF5_H

#include <hdf5.h>
#include <hdf5_hl.h>
#include <include/rdata.h>
#include <include/udata.h>

#undef AMI_HDF5_H_DEBUG

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif


template<typename T>
void getTemplateArrayAttribute_HDF5(hid_t file_id, const char* optionsObject, const char* attributeName, T **destination, hsize_t *length);

template<typename T>
void H5LTget_attribute_template(hid_t file_id, const char* optionsObject, const char* attributeName, T **destination);

EXTERNC void getDoubleArrayAttribute2D_HDF5(hid_t file_id, const char* optionsObject, const char* attributeName, double **destination, hsize_t *m, hsize_t *n);

EXTERNC void setAttributeIntFromDouble_HDF5(hid_t file_id, const char *obj_name, const char *attr_name, const double *bufferDouble, size_t size );
EXTERNC void createAndWriteDouble2DAttribute_HDF5(hid_t dataset, const char *attributeName, const double *buffer, hsize_t m, hsize_t n);
EXTERNC void createAndWriteDouble3DAttribute_HDF5(hid_t dataset, const char *attributeName, const double *buffer, hsize_t m, hsize_t n, hsize_t o);

#endif
