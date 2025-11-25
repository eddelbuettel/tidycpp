// motivated by a StackOverflow question
// updated to use R::Protect in lieu of R::Shield

#include <tidyCpp>

// [[Rcpp::depends(tidyCpp)]]
// We use Rcpp here because it has cppFunction() and sourceCpp(). We do not use any Rcpp headers.

// just to be cute and to not write SEXP below -- example uses only numeric vectors (aka REAL)
// with a dimension attribute making it a numeric matrix
typedef SEXP NumMat;

// [[Rcpp::export]]
NumMat make_matrix(int n) {
    // allocate a numeric (square( matrix and fill it with (trivial) values
    R::Protect mat( R::allocVectorNumeric(n*n) );
    for (auto i=0; i<n*n; i++) {
        R::numericPointer(mat)[i] = 1.0*i;
    }

    // allocate a dimension vector of size two and set it
    // this is what makes a matrix from a vector
    R::Protect d( R::allocVectorInteger(2) );
    R::integerPointer(d)[0] = n;
    R::integerPointer(d)[1] = n;
    mat = setDim(mat, d);

    // allocate a list for column and rownames, allocate those
    // (trivially) set them
    R::Protect lst( R::allocVectorList(2) );
    R::Protect rown( R::allocVectorCharacter(n) );
    R::Protect coln( R::allocVectorCharacter(n) );
    for (auto i=0; i<n; i++) {
        char buf[16];
        sprintf(buf, "row%1d", i);
        R::stringPointer(rown)[i] = R::mkChar(buf);
        sprintf(buf, "col%1d", i);
        R::stringPointer(coln)[i] = R::mkChar(buf);
    }
    R::setVectorElement(lst, 0, rown);
    R::setVectorElement(lst, 1, coln);
    setDimNames(mat, lst);

    // return the matrix
    return(mat);
}
