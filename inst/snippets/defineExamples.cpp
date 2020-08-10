
#include <tidyCpp>

// We use Rcpp here because it has cppFunction() and sourceCpp(). We do not use any Rcpp headers.

// [[Rcpp::depends(tidyCpp)]]

// [[Rcpp::export]]
bool isItLogical(SEXP x) {
    return R::isLogical(x);
}

/*** R
isItLogical(TRUE)
isItLogical(FALSE)
isItLogical(42)
*/
