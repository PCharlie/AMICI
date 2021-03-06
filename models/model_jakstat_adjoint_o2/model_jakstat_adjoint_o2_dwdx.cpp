
#include <include/symbolic_functions.h>
#include <string.h>
#include <include/udata.h>
#include <include/udata_accessors.h>
#include "model_jakstat_adjoint_o2_w.h"

int dwdx_model_jakstat_adjoint_o2(realtype t, N_Vector x, N_Vector dx, void *user_data) {
int status = 0;
UserData *udata = (UserData*) user_data;
realtype *x_tmp = N_VGetArrayPointer(x);
memset(dwdx_tmp,0,sizeof(realtype)*2);
status = w_model_jakstat_adjoint_o2(t,x,NULL,user_data);
  dwdx_tmp[0] = x_tmp[1]*2.0;
  dwdx_tmp[1] = 2.0;
return(status);

}


