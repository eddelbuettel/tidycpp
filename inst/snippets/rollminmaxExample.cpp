// motivated from another package that had used Rcpp -- see commit 6462c5e
// of github repo shikokuchuo/ichimoku (released under GPL-3)
// original source included below, modified source follows

#include <deque>
#include <utility>
#include <tidyCpp>

// [[Rcpp::depends(tidyCpp)]]
// We use Rcpp here because it has cppFunction() and sourceCpp(). We do not use any Rcpp headers.

// types of calculations
enum CalcType { MIN, MAX };

// function arguments for non-data
struct Args {
    int window;
    CalcType ctype;
};

// just to be cute and to not write SEXP below -- example uses only numeric vectors (aka REAL)
typedef SEXP NumVec;

// calculates rolling window for {minimum, maximum}
NumVec roll_minmax(NumVec x, Args a) {

    int n  = R::length(x);
    R::Shield rollx( R::allocVectorNumeric(n) );

    std::deque< std::pair<long double, int> > deck;
    for (int i = 0; i < n; ++i) {
        double xv = R::numericPointer(x)[i];
        if (a.ctype == MIN) {
            while (!deck.empty() && deck.back().first >= xv)
                deck.pop_back();
        } else {
            while (!deck.empty() && deck.back().first <= xv)
                deck.pop_back();
        }
        deck.push_back(std::make_pair(xv, i));

        while(deck.front().second <= i - a.window)
            deck.pop_front();

        long double min = deck.front().first;
        if (i < a.window - 1) {
            R::numericPointer(rollx)[i] = NA_REAL;
        } else {
            R::numericPointer(rollx)[i] = min;
        }
    }
    return rollx;
}



//' Maximum Over a Rolling Window
//'
//' Used by \link{ichimoku} to calculate the maximum over a rolling window.
//'
//' @param x a vector.
//' @param window size of the rolling window.
//'
//' @return A vector of the same length as 'x' with elements 1 to
//'     (length(window) - 1) containing NAs.
//'
//' @details Fast implementation with no error checking or NA handling.
//'
//' @examples
//' maxOver(sample_ohlc_data$close[1:10], 3L)
//'
//' @export
// [[Rcpp::export]]
NumVec maxOver(NumVec x, int window) {
  Args a; a.window = window; a.ctype = MAX;
  return roll_minmax(x, a);
}

//' Minimum Over a Rolling Window
//'
//' Used by \link{ichimoku} to calculate the minimum over a rolling window.
//'
//' @param x a vector.
//' @param window size of the rolling window.
//'
//' @return A vector of the same length as 'x' with elements 1 to
//'     (length(window) - 1) containing NAs.
//'
//' @details Fast implementation with no error checking or NA handling.
//'
//' @examples
//' minOver(sample_ohlc_data$close[1:10], 3L)
//'
//' @export
// [[Rcpp::export]]
NumVec minOver(NumVec x, int window) {
    Args a; a.window = window; a.ctype = MIN;
    SEXP res = roll_minmax(x, a);
    return res;
}



// #include <Rcpp.h>
// using namespace Rcpp;

// // calculates rolling window for {minimum, maximum}
// NumericVector roll_minmax(const NumericVector& x, Args a) {

//   int n  = x.length();
//   NumericVector rollx(n);

//   std::deque< std::pair<long double, int> > deck;
//   for (int i = 0; i < x.size(); ++i) {
//     if(a.ctype == MIN) {
//       while (!deck.empty() && deck.back().first >= x[i])
//         deck.pop_back();
//     } else {
//       while (!deck.empty() && deck.back().first <= x[i])
//         deck.pop_back();
//     }
//     deck.push_back(std::make_pair(x[i], i));

//     while(deck.front().second <= i - a.window)
//       deck.pop_front();

//     long double min = deck.front().first;
//     if (i < a.window - 1) {
//       rollx[i] = NA_REAL;
//     } else {
//       rollx[i] = min;
//     }
//   }
//   return rollx;
// }

// // [[Rcpp::export]]
// NumericVector minOver(const SEXP& x, int window) {
//   Args a; a.window = window; a.ctype = MIN;
//   return roll_minmax(x, a);
// }

// // [[Rcpp::export]]
// NumericVector maxOver(const SEXP& x, int window) {
//   Args a; a.window = window; a.ctype = MAX;
//   return roll_minmax(x, a);
// }
