#ifndef _am_model_jakstat_adjoint_dJydp_h
#define _am_model_jakstat_adjoint_dJydp_h

int dJydp_model_jakstat_adjoint(realtype t, int it, realtype *dJydp, realtype *y, N_Vector x, realtype *dydp, realtype *my, realtype *sigma_y, realtype *dsigma_ydp, void *user_data);


#endif /* _am_model_jakstat_adjoint_dJydp_h */
