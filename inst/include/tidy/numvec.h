//                                               Emacs make this -*- mode: C++; -*-
// tidyCpp: wrapping parts of the C API of R
//
// numvec.h: A minimal numeric vector class

// Copyright (C) 2021 Dirk Eddelbuettel
//
// This file is part of tidyCpp
//
// tidyCpp is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// tidyCpp is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with tidyCpp.  If not, see <http://www.gnu.org/licenses/>.


// truly minimal numeric vector class providing
//  - to/from SEXP use
//  - allocation of vectors
//  - indexing via x[i] on LHS and RHS
// does not even do minimal testing (i.e. SEXP ctor could check type)
//
// Somewhat alpha quality, not widely tested, may set your hair on fire

#ifndef tidyCpp_numvec_h
#define tidyCpp_numvec_h

#include <Rdefines.h>           // for SEXP
#include <tidy/protect.h>       // for Protect

namespace tidy {                // we remain all tidied up in a namespace

class NumVec {
public:
    NumVec(SEXP s_): s(s_) {};                              // ctor from SEXP
    NumVec(const int n): s(R::allocVectorNumeric(n)) {};    // ctor from size
    NumVec(const NumVec &) = default;                       // copy ctor
    NumVec(NumVec &&) noexcept = default;                   // move ctor
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
    R::Protect s;
};

}

#endif
