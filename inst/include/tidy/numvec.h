
// truly minimal numeric vector class providing
//  - to/from SEXP use
//  - allocation of vectors
//  - indexing via x[i] on LHS and RHSll

#ifndef tidyCpp_numvec_h
#define tidyCpp_numvec_h

#include <Rdefines.h>

namespace tidy {                   // we remain all tidied up in a namespace

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
        Proxy(SEXP s_, int i_) : s(s_), i(i_) {}
        Proxy& operator=(double x) { R::numericPointer(s)[i] = x; return *this; }
        inline operator double() { return R::numericPointer(s)[i]; }
        SEXP s;
        int i;
    };
    inline Proxy operator[](int i) { return Proxy(s, i); }

private:
    SEXP s;
};

}

#endif
