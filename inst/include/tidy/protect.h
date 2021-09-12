//                                               Emacs make this -*- mode: C++; -*-
// tidyCpp: wrapping parts of the C API of R
//
// protect.h: a simple wrapper for PROTECT / UNPROTECT

// Copyright (C) 2020 Dirk Eddelbuettel
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

#ifndef tidyCpp_Protect_h
#define tidyCpp_Protect_h

namespace R {

    class Protect {
    public:
        Protect(SEXP obj): sx_(obj) {                   // constructor from SEXP assign
            if (obj != R_NilValue)                      //   incoming non-null objects
                PROTECT(obj);                           //   are protected
        }
        ~Protect() {                                    // destructor
            if (sx_ != R_NilValue)                      //   the protected non-null element
                UNPROTECT(1);                           //   is unprotected at end of scope
        }

        operator SEXP() const { return sx_; }           // default access via SEXP operator

        Protect& operator=(const Protect&) = default;   // copy assignment
        Protect(const Protect&) = default;              // copy constructor
        Protect(Protect&& p) = default;                 // move constructor
        Protect& operator=(Protect&&) = default;        // move copy assignment operator

    private:
        SEXP sx_;
    };

    typedef Protect Shield;                             // cover for Rcpp::Shield in version 0.0.1
}

#endif
