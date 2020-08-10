// Shield: A simple PROTECT wrapper
// With thanks to Rcpp and its file inst/include/Rcpp/protection/Shield.h

// Copyright (C) 2013 Romain Francois and Kevin Ushey
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

#ifndef tidyCpp_Shield_h
#define tidyCpp_Shield_h

namespace R {

    inline SEXP protect(SEXP x); 	//  forward declarations, see below
    inline void unprotect(int i);

    // template < type name T >
    class Shield {
    public:
        Shield(SEXP t_) : t(protect(t_)) {}
        ~Shield() {
            if (t != R_NilValue) unprotect(1);
        }

        operator SEXP() const { return t; }
        SEXP t;

    private:
        Shield (const Shield&);
        Shield& operator=(const Shield&);
    };


    inline SEXP protect(SEXP x) {
        if (x != R_NilValue) PROTECT(x);
        return x;
    }

    inline void unprotect(int i) {
        // Prefer this function over UNPROTECT() so that all balance checks
        // errors by rchk are contained at one location
        UNPROTECT(i);
    }

}

#endif
