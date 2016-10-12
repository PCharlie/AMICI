#include <include/udata.h>
#include <string.h>
#include <include/symbolic_functions.h>

UserDataBase::~UserDataBase(){
    if(m_qpositivex) delete[] m_qpositivex;
    if(m_plist) delete[] m_plist;
    if(m_p) delete[] m_p;
    if(m_k) delete[] m_k;
    if(m_ts) delete[] m_ts;
    if(m_pbar) delete[] m_pbar;
    if(m_xbar) delete[] m_xbar;
    if(m_idlist) delete[] m_idlist;
    if(m_sx0data) delete m_sx0data;
    if(m_z2event) delete[] m_z2event;
    if(m_h) delete[] m_h;
    if(m_dxdotdp) delete[] m_dxdotdp;
    if(m_w) delete[] m_w;
    if(m_dwdx) delete[] m_dwdx;
    if(m_dwdp) delete[] m_dwdp;
    if(m_M) delete[] m_M;
    if(m_dfdx) delete[] m_dfdx;
    if(m_stau) delete[] m_stau;
    SparseDestroyMat(m_J);
}

template<typename T>
void UserDataBase::getUserData(const T container) {
    /**
     * @brief readData reads options from the provided container object which can be an matlab mxArray or the file_id of an HDF5 file
     * @param[in] container: pointer to the array of input arguments @type void
     * @return udata: struct containing all provided user data @type UserData
     */
    
    int length;
    
    double *ptr_dbl = new double[1];
    int *ptr_int = new int[1];
    
    init_modeldims();
    
    /*readAttribute(container, TOUT, "ts", DOUBLE, &m_ts, &length, 2);
    m_nt = length;
    
    readAttribute(container, THETA, "p", DOUBLE, &m_p, &length, 2);
#ifdef AMICI_WITHOUT_MATLAB
    m_np = length;
#endif
    
    readAttribute(container, KAPPA, "kappa", DOUBLE, &m_k, &length, 2);
    
    readAttribute(container, PLIST, "plist", INT, &m_plist, &length, 2);
#ifdef AMICI_WITHOUT_MATLAB
    m_plist = new int[m_np]();
    for (int i = 0; i < m_np; i++) {
        m_plist[i] = i;
    }
#else
    m_np = length;
#endif
    
    
    m_nmaxevent = readAttribute(container, OPTIONS, "nmaxevent", INT, &ptr_int, &length, 1);
    m_tstart = readAttribute(container, OPTIONS, "tstart", DOUBLE, &ptr_dbl, &length, 1);
    m_atol = readAttribute(container, OPTIONS, "atol", DOUBLE, &ptr_dbl, &length, 1);
    m_rtol = readAttribute(container, OPTIONS, "rtol", DOUBLE, &ptr_dbl, &length, 1);
    m_maxsteps = readAttribute(container, OPTIONS, "maxsteps", INT, &ptr_int, &length, 1);
    m_lmm = readAttribute(container, OPTIONS, "lmm", INT, &ptr_int, &length, 1);
    m_iter = readAttribute(container, OPTIONS, "iter", INT, &ptr_int, &length, 1);
    m_interpType = readAttribute(container, OPTIONS, "interptype", INT, &ptr_int, &length, 1);
    m_linsol = readAttribute(container, OPTIONS, "linsol", INT, &ptr_int, &length, 1);
    m_stldet = readAttribute(container, OPTIONS, "stldet", INT, &ptr_int, &length, 1);
    
    readAttribute(container, OPTIONS, "id", DOUBLE, &m_idlist, &length, 2);
#ifdef AMICI_WITHOUT_MATLAB
    m_idlist = new realtype[m_np]();
    for(int i = 0; i < m_np; ++i)
        m_idlist[i] = 0;
#endif
    readAttribute(container, OPTIONS, "z2event", DOUBLE, &m_z2event, &length, 2);
#ifdef AMICI_WITHOUT_MATLAB
    m_z2event = new realtype[m_nz]();
    for(int i = 0; i < m_nz; ++i)
        m_z2event[i] = i;
#endif
    
    m_sensi = readAttribute(container, OPTIONS, "sensi", INT, &ptr_int, &length, 1);
    m_ism = readAttribute(container, OPTIONS, "ism", INT, &ptr_int, &length, 1);
    m_sensi_meth = readAttribute(container, OPTIONS, "sensi_meth", INT, &ptr_int, &length, 1);
    m_ordering = readAttribute(container, OPTIONS, "ordering", INT, &ptr_int, &length, 1);
    
    readAttribute(container, PBAR, "pbar", DOUBLE, &m_idlist, &length, 2);
#ifdef AMICI_WITHOUT_MATLAB
    m_pbar = new realtype[m_np]();
    ones(m_pbar, m_np);
#endif
    readAttribute(container, XSCALE, "xscale", DOUBLE, &m_z2event, &length, 2);
#ifdef AMICI_WITHOUT_MATLAB
    m_xbar = new double[m_nx]();
#endif
    
    
    readAttribute(container, OPTIONS, "qpositivex", DOUBLE, &m_qpositivex, &length, 2);*/
    
    processUserData();
    
    
}

void UserDataBase::processUserData() {
    int nx = m_nx;
    int np = m_np;
    if (m_nx>0) {
        /* initialise temporary jacobian storage */
        m_J = SparseNewMat(nx,nx,m_nnz,CSC_MAT);
        m_M = new realtype[nx*nx]();
        m_dfdx = new realtype[nx*nx]();
    }
    if (m_sensi>0) {
        /* initialise temporary dxdotdp storage */
        m_dxdotdp = new realtype[nx*np]();
    }
    if (m_ne>0) {
        /* initialise temporary stau storage */
        m_stau = new realtype[np]();
    }
    
    
    m_w = new realtype[m_nw]();
    m_dwdx = new realtype[m_ndwdx]();
    m_dwdp = new realtype[m_ndwdp]();
}

#ifdef AMICI_WITHOUT_MATLAB
UserDataHDF5::UserDataHDF5(const char* fileName) {
    hid_t file_id = H5Fopen(fileName, H5F_ACC_RDONLY, H5P_DEFAULT);
    getUserData(&file_id);
    H5Fclose(file_id);
}
#else
UserDataMATLAB::UserDataMATLAB(const mxArray *prhs[]) {
    getUserData(file_id);
}
#endif

#ifdef AMICI_WITHOUT_MATLAB
template<typename T, typename S> T UserDataHDF5::readAttribute(void *container, OptsObj optionsObject, const char* attributeName, AttrType attributeType, T **ptr_attribute, S *length, int ndims) {
    const char* Obj;
    int *plist;
    realtype *pbar;
    double *xbar;
    switch(optionsObject) {
        case TOUT:
            Obj = "/data";
            break;
        case THETA:
            Obj = "/data";
            break;
        case KAPPA:
            Obj = "/data";
            break;
        case PLIST:
            /*plist = new int[m_np]();
             for (int i = 0; i < m_np; i++) {
             plist[i] = i;
             }
             *length = m_np;
             ptr_attribute = plist;*/
            return;
        case PBAR:
            /*pbar = new realtype[m_np]();
             ones(pbar, m_np);
             *length = m_np;
             ptr_attribute = pbar;*/
            return;
        case XSCALE:
            /*xbar = new double[m_nx]();
             *length = 1;
             ptr_attribute = xbar;*/
            return;
        case DATA:
            Obj = "/data";
            break;
        case INIT:
            m_bsx0 = FALSE;
            m_bx0 = FALSE;
            return;
        case OPTIONS:
            Obj = "/options";
            break;
    }
    if(strcmp(attributeName,"z2event")) {
        /*realtype *z2event = new realtype[m_nz]();
        for(int i = 0; i < m_nz; ++i)
            z2event[i] = i;
        *length = m_nz;
        ptr_attribute = z2event;*/
        return;
    }
    if(strcmp(attributeName,"idlist")) {
        /*realtype *idlist = new realtype[m_np]();
        for(int i = 0; i < m_np; ++i)
            idlist[i] = 0;
        *length = m_np;
        ptr_attribute = idlist;*/
        return;
    }
    
    hid_t *file_id = (hid_t *) container;
    getTemplateArrayAttribute_HDF5(*file_id, Obj, attributeName, ptr_attribute, length);
    return(**ptr_attribute);
}
#else
template<typename T, typename S> T UserDataMATLAB::readAttribute(void *container, OptsObj optionsObject, const char* attributeName, AttrType attributeType, T **ptr_attribute, S *length, int ndims) {
    switch(attributeType) {
        case INT:
            switch(ndims){
                case 1:
                    *length = 1;
                    *attribute = getIntScalarAttribute_MATLAB(container, Obj, attributeName);
                case 2:
                    getIntArrayAttribute_MATLAB(container, optionsObject, attributeName, ptr_attribute, length);
            }
        case DOUBLE:
            switch(ndims){
                case 1:
                    *length = 1;
                    *attribute = getDoubleScalarAttribute_MATLAB(container, Obj, attributeName);
                case 2:
                    getDoubleArrayAttribute_MATLAB(container, optionsObject, attributeName, ptr_attribute, length);
            }
            
    }
    
    return(**ptr_attribute);
}
#endif
