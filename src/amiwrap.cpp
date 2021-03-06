/**
 * @file   amiwrap.cpp
 * @brief  core routines for mex interface
 *
 * This file defines the fuction mexFunction which is executed upon calling the mex file from matlab
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _USE_MATH_DEFINES /* MS definition of PI and other constants */
#include <math.h>
#ifndef M_PI /* define PI if we still have no definition */
#define M_PI 3.14159265358979323846
#endif
#include <mex.h>
#include "wrapfunctions.h" /* user functions */
#include <include/amici_interface_matlab.h> /* amici functions */
#include <include/edata_accessors.h>
#include <include/udata_accessors.h>
#include <include/rdata_accessors.h>


/*!
 * mexFunction is the main function of the mex simulation file this function carries out all numerical integration and writes results into the sol struct.
 *
 * @param[in] nlhs number of output arguments of the matlab call @type int
 * @param[out] plhs pointer to the array of output arguments @type mxArray
 * @param[in] nrhs number of input arguments of the matlab call @type int
 * @param[in] prhs pointer to the array of input arguments @type mxArray
 * @return void
 */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
    
    UserData *udata = nullptr; /* user data */
    ReturnData *rdata = nullptr; /* return data */
    ExpData *edata = nullptr; /* experimental data */

    /* return status flag */
    int status;
    double *pstatus = (double *) mxMalloc(sizeof(double));

    udata = userDataFromMatlabCall(prhs);
    if (udata == NULL) {
        /* goto freturn will fail here as freeXXXXData routines will fail*/
        *pstatus = -98;
        return;
    }

    /* options */
    if (!prhs[3]) {
        mexErrMsgIdAndTxt("AMICI:mex:options","No options provided!");
    }

    rdata = setupReturnData(plhs, udata, pstatus);
    if (rdata == NULL) {
        status = -96;
        goto freturn;
    }

    if (nx>0) {
        edata = expDataFromMatlabCall(prhs, udata, &status);
        if (status != 0) {
            goto freturn;
        }
    }

    runAmiciSimulation(udata, edata, rdata, &status);

freturn:
    freeUserData(udata);
    delete edata;
    *pstatus = (double) status;
}
