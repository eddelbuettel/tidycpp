
// #include <R.h>
// #include <Rinternals.h>

// SEXP convolve2(SEXP a, SEXP b)
// {
//     int na, nb, nab;
//     double *xa, *xb, *xab;
//     SEXP ab;

//     a = PROTECT(coerceVector(a, REALSXP));
//     b = PROTECT(coerceVector(b, REALSXP));
//     na = length(a); nb = length(b); nab = na + nb - 1;
//     ab = PROTECT(allocVector(REALSXP, nab));
//     xa = REAL(a); xb = REAL(b); xab = REAL(ab);
//     for(int i = 0; i < nab; i++) xab[i] = 0.0;
//     for(int i = 0; i < na; i++)
//         for(int j = 0; j < nb; j++) xab[i + j] += xa[i] * xb[j];
//     UNPROTECT(3);
//     return ab;
// }


#include <tidyCpp>

// [[Rcpp::depends(tidyCpp)]]

// We use Rcpp here because it has cppFunction() and sourceCpp(). We do not use any Rcpp headers.

// [[Rcpp::export]]
SEXP convolve2(SEXP a, SEXP b)
{
    int na, nb, nab;
    double *xa, *xb, *xab;

    R::Protect pa(R::coerceVectorNumeric(a));
    R::Protect pb(R::coerceVectorNumeric(b));
    na = R::length(pa); nb = R::length(pb); nab = na + nb - 1;
    R::Protect ab(R::allocVectorNumeric(nab));
    xa = R::numericPointer(pa); xb = R::numericPointer(pb); xab = R::numericPointer(ab);
    for(int i = 0; i < nab; i++) xab[i] = 0.0;
    for(int i = 0; i < na; i++)
        for(int j = 0; j < nb; j++) xab[i + j] += xa[i] * xb[j];
    return ab;
}

/*** R
convolve2(c(1,2,3), c(4,5,6))
*/
