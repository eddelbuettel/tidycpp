//                                               Emacs make this -*- mode: C++; -*-
// tidyCpp: wrapping parts of the C API of R
//
// internals.h: tidying some parts of Rinternals.h

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

#ifndef tidyCpp_internals_h
#define tidyCpp_internals_h

#include <Rdefines.h>

namespace R {                   // we remain all tidied up in a namespace

#if 0
    inline Rboolean (isNull)(SEXP s)         { return (Rf_isNull)(s);        }
    inline Rboolean (isSymbol)(SEXP s)       { return (Rf_isSymbol)(s);      }
    inline Rboolean (isLogical)(SEXP s)      { return (Rf_isLogical)(s);     }
    inline Rboolean (isReal)(SEXP s)         { return (Rf_isReal)(s);        }
    inline Rboolean (isComplex)(SEXP s)      { return (Rf_isComplex)(s);     }
    inline Rboolean (isExpression)(SEXP s)   { return (Rf_isExpression)(s);  }
    inline Rboolean (isEnvironment)(SEXP s)  { return (Rf_isEnvironment)(s); }
    inline Rboolean (isString)(SEXP s)       { return (Rf_isString)(s);      }
    inline Rboolean (isObject)(SEXP s)       { return (Rf_isObject)(s);      }
#endif
    inline Rboolean (isString)(SEXP s)       { return (Rf_isString)(s);      }

    inline void error(const char *msg)       { return Rf_error(msg);         }

#if 0
    inline bool isSimpleScalar(SEXP x, int type) { return (IS_SCALAR(x, type) && ATTRIB(x) == R_NilValue); }
    inline int  simpleScalarType(SEXP x)         { return (((x)->sxpinfo.scalar && ATTRIB(x) == R_NilValue) ? TYPEOF(x) : 0); }
#endif

    inline bool maybeShared(SEXP x)     { return(MAYBE_SHARED(x));  }
    inline bool noReferences(SEXP x)    { return(REFCNT(x) == 0);   }
    inline bool maybeReferenced(SEXP x) { return(!noReferences(x)); }
    inline bool notShared(SEXP x)       { return(!maybeShared(x));  }

}

#endif