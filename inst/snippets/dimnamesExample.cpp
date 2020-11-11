
// #include <Rcpp.h>

// // [[Rcpp::export]]
// SEXP out(SEXP x, SEXP y)
// {
//     int nx = Rf_length(x), ny = Rf_length(y);
//     SEXP ans = PROTECT(Rf_allocMatrix(REALSXP, nx, ny));
//     double *rx = REAL(x), *ry = REAL(y), *rans = REAL(ans);

//     for(int i = 0; i < nx; i++) {
//         double tmp = rx[i];
//         for(int j = 0; j < ny; j++)
//             rans[i + nx*j] = tmp * ry[j];
//     }

//     SEXP dimnames = PROTECT(Rf_allocVector(VECSXP, 2));
//     SET_VECTOR_ELT(dimnames, 0, Rf_getAttrib(x, R_NamesSymbol));
//     SET_VECTOR_ELT(dimnames, 1, Rf_getAttrib(y, R_NamesSymbol));
//     Rf_setAttrib(ans, R_DimNamesSymbol, dimnames);

//     UNPROTECT(2);
//     return ans;
// }

#include <tidyCpp>
// [[Rcpp::depends(tidyCpp)]]

// We use Rcpp here because it has cppFunction() and sourceCpp(). We do not use any Rcpp headers.

// [[Rcpp::export]]
SEXP out(SEXP x, SEXP y)
{
    int nx = R::length(x), ny = R::length(y);
    R::Protect ans( R::allocMatrixReal(nx, ny) );
    double *rx = R::numericPointer(x), *ry = R::numericPointer(y), *rans = R::numericPointer(ans);

    for(int i = 0; i < nx; i++) {
        double tmp = rx[i];
        for(int j = 0; j < ny; j++)
            rans[i + nx*j] = tmp * ry[j];
    }

    R::Protect dimnames(R::allocVectorList(2));
    R::setVectorElement(dimnames, 0, R::getNames(x));
    R::setVectorElement(dimnames, 1, R::getNames(y));
    R::setDimNames(ans, dimnames);
    return ans;
}

/*** R
out(c(a=1,b=2,c=3), c(A=2,B=3,C=4))
*/
