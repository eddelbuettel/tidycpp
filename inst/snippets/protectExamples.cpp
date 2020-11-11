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
    R::Protect ab( R::allocVectorNumeric(2) );
    R::numericPointer(ab)[0] = 123.45;
    R::numericPointer(ab)[1] = 67.89;
    return ab;
}

/*** R
vectorExample()
*/


// We use RcppSpdlog to get some insight into the protection under two scenarios
// [[Rcpp::depends(RcppSpdlog)]]

// define a new 'logging Protect', for simplicity _outside_ the R namespace

#include <RcppSpdlog>

class Protect {
public:
    Protect(SEXP obj): sx_(obj) { 					// construct protected SEXP
        spdlog::info("entered ctor");
        if (obj != R_NilValue)
            PROTECT(obj);
    }
    ~Protect() {            						// destruct protected SEXP
        spdlog::info("entered dtor");
        if (sx_ != R_NilValue)
            UNPROTECT(1);
    }

    operator SEXP() const { 						// default access via SEXP operator
        spdlog::info("entered SEXP()");
        return sx_;
    }

    // omitting four delete line here for compactness

    private:
        SEXP sx_;
};


// We redo the convolution example

// [[Rcpp::export]]
SEXP convolveIncorrect(SEXP a, SEXP b)
{
    auto sp = spdlog::get("R");         					// retrieve existing one
    if (sp == nullptr) sp = spdlog::r_sink_mt("R");     	// or create new one if needed
    spdlog::set_default_logger(sp);                         // and set as default
    spdlog::set_pattern("[%H:%M:%S.%f] %v");
    spdlog::info("starting convolveIncorrect");

    int na, nb, nab;
    double *xa, *xb, *xab;
    SEXP ab;

    a = Protect(R::coerceVectorNumeric(a));
    b = Protect(R::coerceVectorNumeric(b));
    na = R::length(a); nb = R::length(b); nab = na + nb - 1;
    ab = Protect(R::allocVectorNumeric(nab));
    xa = R::numericPointer(a); xb = R::numericPointer(b); xab = R::numericPointer(ab);
    for(int i = 0; i < nab; i++) xab[i] = 0.0;
    for(int i = 0; i < na; i++)
        for(int j = 0; j < nb; j++) xab[i + j] += xa[i] * xb[j];
    spdlog::info("ending convolveIncorrect");
    return ab;
}

/*** R
convolveIncorrect(c(1,2,3), c(4,5,6))
*/

// [[Rcpp::export]]
SEXP convolveCorrect(SEXP a, SEXP b)
{
    auto sp = spdlog::get("R");         					// retrieve existing one
    if (sp == nullptr) sp = spdlog::r_sink_mt("R");     	// or create new one if needed
    spdlog::set_default_logger(sp);                         // and set as default
    spdlog::set_pattern("[%H:%M:%S.%f] %v");
    spdlog::info("starting convolveCorrect");

    int na, nb, nab;
    double *xa, *xb, *xab;

    Protect pa(R::coerceVectorNumeric(a));
    Protect pb(R::coerceVectorNumeric(b));
    na = R::length(pa); nb = R::length(pb); nab = na + nb - 1;
    Protect ab(R::allocVectorNumeric(nab));
    xa = R::numericPointer(pa); xb = R::numericPointer(pb); xab = R::numericPointer(ab);
    for(int i = 0; i < nab; i++) xab[i] = 0.0;
    for(int i = 0; i < na; i++)
        for(int j = 0; j < nb; j++) xab[i + j] += xa[i] * xb[j];
    spdlog::info("ending convolveCorrect");
    return ab;
}

/*** R
convolveCorrect(c(1,2,3), c(4,5,6))
*/
