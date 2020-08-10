#include <tidyCpp>

// We use Rcpp here because it has cppFunction() and sourceCpp(). We do not use any Rcpp headers.

// [[Rcpp::depends(tidyCpp)]]

// basic example from WRE:
//   Here is a small example of creating an R numeric vector in C code:

//     #include <R.h>
//     #include <Rinternals.h>

//         SEXP ab;
//           ....
//         ab = PROTECT(allocVector(REALSXP, 2));
//         REAL(ab)[0] = 123.45;
//         REAL(ab)[1] = 67.89;
//         UNPROTECT(1);


// [[Rcpp::export]]
SEXP vectorExample() {
    SEXP ab = R::Shield( R::allocVectorNumeric(2) );
    R::ptrNumeric(ab)[0] = 123.45;
    R::ptrNumeric(ab)[1] = 67.89;
    return ab;
}

/*** R
vectorExample()
*/
