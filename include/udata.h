#ifndef _MY_UDATA
#define _MY_UDATA

#include <nvector/nvector_serial.h>  /* defs. of serial NVECTOR fcts. and macros  */
#include <sundials/sundials_klu_impl.h> /* def. of type klu solver */
#include <sundials/sundials_sparse.h> /* def. of type sparse stuff */
#include <sundials/sundials_types.h> /* def. of type realtype */
#include <sundials/sundials_math.h>  /* definition of ABS */
#include <sundials/sundials_config.h>
#ifndef AMICI_WITHOUT_MATLAB
#include <src/ami_matlab.h>
#else
#include <src/ami_hdf5.h>
#endif
#include "wrapfunctions.h"

enum OptsObj { TOUT=0, THETA=1, KAPPA=2, OPTIONS=3, PLIST=4, PBAR=5, XSCALE=6, INIT=7 , DATA=8};
enum AttrType { INT, DOUBLE };

/** @brief struct that stores all user provided data */
class UserDataBase {
    void processUserData();
    void init_modeldims();
    template<typename T,typename S>
    T readAttribute(void *container, OptsObj optionsObject, const char* attributeName, AttrType attributeType, T **ptr_attribute, S *length, int ndims) {return(**ptr_attribute);};
    
    
public:
    /** positivity flag */
    double *m_qpositivex;
    
    /** parameter reordering */
    int    *m_plist;
    /** number of parameters */
    int    m_np;
    /** number of observables */
    int    m_ny;
    /** number of observables in the unaugmented system */
    int    m_nytrue;
    /** number of states */
    int    m_nx;
    /** number of states in the unaugmented system */
    int    m_nxtrue;
    /** number of event outputs */
    int    m_nz;
    /** number of event outputs in the unaugmented system */
    int    m_nztrue;
    /** number of events */
    int    m_ne;
    /** number of timepoints */
    int    m_nt;
    /** dimension of the augmented objective function for 2nd order ASA */
    int    m_ng;
    /** number of common expressions */
    int    m_nw;
    /** number of derivatives of common expressions wrt x */
    int    m_ndwdx;
    /** number of derivatives of common expressions wrt p */
    int    m_ndwdp;
    /** number of nonzero entries in jacobian */
    int    m_nnz;
    /** maximal number of events to track */
    int    m_nmaxevent;
    
    /** parameter array */
    double *m_p;
    /** constants array */
    double *m_k;
    
    /** starting time */
    double m_tstart;
    /** timepoints */
    double *m_ts;
    
    /** scaling of parameters */
    double *m_pbar;
    /** scaling of states */
    double *m_xbar;
    
    /** flag array for DAE equations */
    double *m_idlist;
    
    /** flag indicating whether sensitivities are supposed to be computed */
    int m_sensi;
    /** absolute tolerances for integration */
    double m_atol;
    /** relative tolerances for integration */
    double m_rtol;
    /** maximum number of allowed integration steps */
    int m_maxsteps;
    
    /** internal sensitivity method */
    /*!
     * a flag used to select the sensitivity solution method. Its value can be CV SIMULTANEOUS or CV STAGGERED. Only applies for Forward Sensitivities.
     */
    int m_ism;
    
    /** method for sensitivity computation */
    /*!
     * CW_FSA for forward sensitivity analysis, CW_ASA for adjoint sensitivity analysis
     */
    int m_sensi_meth;
    /** linear solver specification */
    int m_linsol;
    /** interpolation type */
    /*!
     * specifies the interpolation type for the forward problem solution which is then used for the backwards problem. can be either CV_POLYNOMIAL or CV_HERMITE
     */
    int m_interpType;
    
    /** linear multistep method */
    /*!
     * specifies the linear multistep method and may be one of two possible values: CV ADAMS or CV BDF.
     */
    int m_lmm;
    
    /** nonlinear solver */
    /*!
     * specifies the type of nonlinear solver iteration and may be either CV NEWTON or CV FUNCTIONAL.
     */
    int m_iter;
    
    /** flag controlling stability limit detection */
    booleantype m_stldet;
    
    /** upper bandwith of the jacobian */
    int m_ubw;
    /** lower bandwith of the jacobian */
    int m_lbw;
    
    /** flag for state initialisation */
    /*!
     * flag which determines whether analytic state initialisation or provided initialisation should be used
     */
    booleantype m_bx0;
    
    /** flag for sensitivity initialisation */
    /*!
     * flag which determines whether analytic sensitivities initialisation or provided initialisation should be used
     */
    booleantype m_bsx0;
    
    /** state initialisation */
    double *m_x0data;
    
    /** sensitivity initialisation */
    double *m_sx0data;
    
    
    /** state ordering */
    int m_ordering;
    
    /** index indicating to which event an event output belongs */
    double *m_z2event;
    
    /** flag indicating whether a certain heaviside function should be active or not */
    double *m_h;
    
    /** tempory storage of Jacobian data across functions */
    SlsMat m_J;
    /** tempory storage of dxdotdp data across functions */
    realtype *m_dxdotdp;
    /** tempory storage of w data across functions */
    realtype *m_w;
    /** tempory storage of dwdx data across functions */
    realtype *m_dwdx;
    /** tempory storage of dwdp data across functions */
    realtype *m_dwdp;
    /** tempory storage of M data across functions */
    realtype *m_M;
    /** tempory storage of dfdx data across functions */
    realtype *m_dfdx;
    /** tempory storage of stau data across functions */
    realtype *m_stau;
    
    /** flag indicating whether a NaN in dxdotdp has been reported */
    booleantype m_nan_dxdotdp;
    /** flag indicating whether a NaN in J has been reported */
    booleantype m_nan_J;
    /** flag indicating whether a NaN in JSparse has been reported */
    booleantype m_nan_JSparse;
    /** flag indicating whether a NaN in xdot has been reported */
    booleantype m_nan_xdot;
    /** flag indicating whether a NaN in xBdot has been reported */
    booleantype m_nan_xBdot;
    /** flag indicating whether a NaN in qBdot has been reported */
    booleantype m_nan_qBdot;

public:
    template<typename T>
    void getUserData(const T container);
    ~UserDataBase();
};

#ifdef AMICI_WITHOUT_MATLAB
class UserDataHDF5: public UserDataBase {
    template<typename T,typename S> T readAttribute(void *container, OptsObj optionsObject, const char* attributeName, AttrType attributeType, T **ptr_attribute, S *length, int ndims);
public:
    UserDataHDF5(const char* fileName);
};
typedef UserDataHDF5 UserData;
#else
class UserDataMATLAB: public UserDataBase {
    template<typename T,typename S> T readAttribute<T,S>(void *container, OptsObj optionsObject, const char* attributeName, AttrType attributeType, T **ptr_attribute, S *length, int ndims);
public:
        UserDataMATLAB(const mxArray *prhs[]);
};
typedef UserDataMATLAB UserData;
#endif

#endif /* _MY_UDATA */
