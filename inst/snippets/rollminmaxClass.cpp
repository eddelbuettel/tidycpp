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

class NumVec {
public:
    NumVec(SEXP s_): s(s_) {};
    NumVec(const int n): s(R::allocVectorNumeric(n)) {};
    NumVec(const NumVec &) = default;						// copy ctor
    NumVec(NumVec &&) noexcept = default;					// move ctor
    NumVec& operator=(const NumVec &) = default;            // copy assignment
    NumVec& operator=(NumVec &&) noexcept = default;        // move assignment
    ~NumVec() noexcept = default;                           // dtor

    operator SEXP() const { return s; };

    class Proxy {
    public:
        Proxy(SEXP s_, int index_) : s(s_), index(index_) {}
        Proxy& operator=(double x) { R::numericPointer(s)[index] = x; return *this; }
        inline operator double() { return R::numericPointer(s)[index]; }
        SEXP s;
        int index;
    };
    inline Proxy operator[](int i) { return Proxy(s, i); }

private:
    SEXP s;
};

// calculates rolling window for {minimum, maximum}
NumVec roll_minmax(NumVec x, Args a) {
    int n  = R::length(x);
    NumVec rollx(n);

    std::deque< std::pair<long double, int> > deck;
    for (int i = 0; i < n; ++i) {
        double xv = x[i];
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
            //R::numericPointer(rollx)[i] = NA_REAL;
            rollx[i] = NA_REAL;
        } else {
            R::numericPointer(rollx)[i] = min;
            rollx[i] = min;
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
