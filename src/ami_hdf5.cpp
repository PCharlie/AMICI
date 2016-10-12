#include<assert.h>

#include <src/ami_hdf5.h>
#include "wrapfunctions.h"
#include "include/symbolic_functions.h"

#include "include/rdata_accessors.h"
#include "include/edata_accessors.h"
#include "include/udata_accessors.h"

template<typename T>
void getTemplateArrayAttribute_HDF5(hid_t file_id, const char* optionsObject, const char* attributeName, T **destination, hsize_t *length) {
    H5T_class_t type_class;
    size_t type_size;
    H5LTget_attribute_info(file_id, optionsObject, attributeName, length, &type_class, &type_size);
#ifdef AMI_HDF5_H_DEBUG
    printf("%s: %d: ", attributeName, *length);
#endif
    *destination = new T[*length]; // vs. type_size
    H5LTget_attribute_template(file_id, optionsObject, attributeName, *destination);
#ifdef AMI_HDF5_H_DEBUG
    printfArray(*destination, *length, "%e ");
    printf("\n");
#endif
}

template<> void H5LTget_attribute_template<double>(hid_t file_id, const char* optionsObject, const char* attributeName, double **destination) { H5LTget_attribute_double(file_id, optionsObject, attributeName, *destination);};
template<> void H5LTget_attribute_template<int>(hid_t file_id, const char* optionsObject, const char* attributeName, int **destination) { H5LTget_attribute_int(file_id, optionsObject, attributeName, *destination);};


EXTERNC void getDoubleArrayAttribute2D_HDF5(hid_t file_id, const char* optionsObject, const char* attributeName, double **destination, hsize_t *m, hsize_t *n) {
    int rank;
    H5LTget_attribute_ndims(file_id, optionsObject, attributeName, &rank);
    assert(rank <= 2);

    hsize_t dims[2];
    H5T_class_t type_class;
    size_t type_size;
    H5LTget_attribute_info(file_id, optionsObject, attributeName, dims, &type_class, &type_size);

    if(rank == 1) {
        *m = 1;
        *n = dims[0];
        getTemplateArrayAttribute_HDF5(file_id, optionsObject, attributeName, &destination, m);
    } else {
#ifdef AMI_HDF5_H_DEBUG
        printf("%s: %d x %d: ", attributeName, dims[0], dims[1]);
#endif
        *m = dims[0];
        *n = dims[1];

        *destination = new double[(*m) * (*n)];
        H5LTget_attribute_double(file_id, optionsObject, attributeName, *destination);
#ifdef AMI_HDF5_H_DEBUG
        printfArray(*destination, (*m) * (*n), "%e ");
        printf("\n");
#endif
    }
}

// option for overwrite
EXTERNC void createAndWriteDouble2DAttribute_HDF5(hid_t dataset, const char *attributeName, const double *buffer, hsize_t m, hsize_t n) {
    const hsize_t adims[] = {m, n};

    if(H5Aexists(dataset, attributeName) > 0) {
        H5Adelete(dataset, attributeName);
    }

    hid_t space = H5Screate_simple(2, adims, NULL);
    hid_t attr = H5Acreate2(dataset, attributeName, H5T_NATIVE_DOUBLE, space, H5P_DEFAULT, H5P_DEFAULT);
    herr_t status = H5Awrite(attr, H5T_NATIVE_DOUBLE, buffer);
}

EXTERNC void createAndWriteDouble3DAttribute_HDF5(hid_t dataset, const char *attributeName, const double *buffer, hsize_t m, hsize_t n, hsize_t o) {
    const hsize_t adims[] = {m, n, o};

    if(H5Aexists(dataset, attributeName) > 0) {
        H5Adelete(dataset, attributeName);
    }

    hid_t space = H5Screate_simple(3, adims, NULL);
    hid_t attr = H5Acreate2(dataset, attributeName, H5T_NATIVE_DOUBLE, space, H5P_DEFAULT, H5P_DEFAULT);
    herr_t status = H5Awrite(attr, H5T_NATIVE_DOUBLE, buffer);
}


EXTERNC void setAttributeIntFromDouble_HDF5(hid_t file_id, const char *obj_name, const char *attr_name, const double *bufferDouble, size_t size)
{
    int intBuffer[size];
    for(int i = 0; i < size; ++i)
        intBuffer[i] = bufferDouble[i];

    H5LTset_attribute_int(file_id, obj_name, attr_name, intBuffer, size);
}
