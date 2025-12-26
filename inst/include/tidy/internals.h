//                                               Emacs make this -*- mode: C++; -*-
// tidyCpp: wrapping parts of the C API of R
//
// internals.h: tidying some parts of Rinternals.h

// Copyright (C) 2020 - 2025  Dirk Eddelbuettel
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

#if defined(__HAVE_NIL_OBJECT)
    extern SEXP Nil = R_NilValue;	    // The nil object as convenience, user declared
#endif

    typedef struct SEXPREC *sexp; 		// R::sexp instead of SEXP

    // the following is from the 'internal use only' section
    //inline SEXP Attrib(SEXP x)               { return (ATTRIB(x));           }
    inline unsigned int Object(SEXP x)       { return (OBJECT(x));           }
    //inline unsigned int Mark(SEXP x)         { return (MARK(x));             }
    inline unsigned int Typeof(SEXP x)       { return (TYPEOF(x));           }
    inline unsigned int Named(SEXP x)        { return (NAMED(x));            }
    //inline unsigned int Rtrace(SEXP x)       { return (RTRACE(x));           }
    inline unsigned int Levels(SEXP x)       { return (LEVELS(x));           }
    inline void setObject(SEXP x, unsigned int v)     { SET_OBJECT(x, v);    }
    inline void setTypeof(SEXP x, unsigned int v)     { SET_TYPEOF(x, v);    }
    //inline void setRtrace(SEXP x, unsigned int v)     { SET_RTRACE(x, v);    }
    inline void setLevels(SEXP x, unsigned int v)     { SETLEVELS(x, v);     }
    inline unsigned int Altrep(SEXP x)       { return (ALTREP(x));           }
    //inline void setAltrep(SEXP x, unsigned int v)     { SETALTREP(x, v);     }
    //inline void setScalar(SEXP x, unsigned int v)     { SETSCALAR(x, v);     }

    inline Rboolean (isNull)(SEXP s)         { return (Rf_isNull)(s);        }
    inline Rboolean (isSymbol)(SEXP s)       { return (Rf_isSymbol)(s);      }
    //inline Rboolean (isLogical)(SEXP s)      { return (Rf_isLogical)(s);     }
    inline Rboolean (isReal)(SEXP s)         { return (Rf_isReal)(s);        }
    //inline Rboolean (isComplex)(SEXP s)      { return (Rf_isComplex)(s);     }
    inline Rboolean (isExpression)(SEXP s)   { return (Rf_isExpression)(s);  }
    inline Rboolean (isEnvironment)(SEXP s)  { return (Rf_isEnvironment)(s); }
    inline Rboolean (isString)(SEXP s)       { return (Rf_isString)(s);      }
    inline Rboolean (isObject)(SEXP s)       { return (Rf_isObject)(s);      }

    //inline bool isSimpleScalar(SEXP x, int type) { return (IS_SCALAR(x, type) && ATTRIB(x) == R_NilValue); }
    //inline int  simpleScalarType(SEXP x)         { return (((x)->sxpinfo.scalar && ATTRIB(x) == R_NilValue) ? TYPEOF(x) : 0); }

    inline bool maybeShared(SEXP x)     { return(MAYBE_SHARED(x));  }
    inline bool notShared(SEXP x)       { return(!maybeShared(x));  }
    inline bool noReferences(SEXP x)    { return(NO_REFERENCES(x)); }
    inline bool maybeReferenced(SEXP x) { return(!noReferences(x)); }

    inline int length(SEXP x)           { return(LENGTH(x));        }
    inline R_xlen_t xlength(SEXP x)     { return(XLENGTH(x));       }
    // inline R_xlen_t truelength(SEXP x)  { return(TRUELENGTH(x));    }

    inline SEXP stringElement(SEXP x, R_xlen_t i) { return STRING_ELT(x, i); }

    inline SEXP scalarComplex(Rcomplex s)   { return Rf_ScalarComplex(s); }
    inline SEXP scalarInteger(int x)        { return Rf_ScalarInteger(x); }
    inline SEXP scalarLogical(int x)        { return Rf_ScalarLogical(x); }
    inline SEXP scalarRaw(Rbyte r)          { return Rf_ScalarRaw(r);     }
    inline SEXP scalarReal(double d)        { return Rf_ScalarReal(d);    }
    inline SEXP scalarString(SEXP s)        { return Rf_ScalarString(s);  }

    inline SEXP allocMatrix(SEXPTYPE s, int n, int m) { return Rf_allocMatrix(s, n, m);       }
    inline SEXP allocMatrixLogical(int n, int m)      { return Rf_allocMatrix(LGLSXP, n, m);  }
    inline SEXP allocMatrixInteger(int n, int m)      { return Rf_allocMatrix(INTSXP, n, m);  }
    inline SEXP allocMatrixReal(int n, int m)         { return Rf_allocMatrix(REALSXP, n, m); }
    inline SEXP allocMatrixCharacter(int n, int m)    { return Rf_allocMatrix(STRSXP, n, m);  }
    inline SEXP allocMatrixComplex(int n, int m)      { return Rf_allocMatrix(CPLXSXP, n, m); }


    inline SEXP mkChar(const char *c)                         { return Rf_mkChar(c);            }
    inline SEXP mkCharLen(const char *c, int n)               { return Rf_mkCharLen(c, n);      }
    inline SEXP mkCharCE(const char *c, cetype_t t)           { return Rf_mkCharCE(c, t);       }
    inline SEXP mkCharLenCE(const char *c, int n, cetype_t t) { return Rf_mkCharLenCE(c, n, t); }

    inline const char * type2char(unsigned int sx)            { return Rf_type2char(sx); }

    inline void setStringElement(SEXP x, R_xlen_t i, SEXP v)  {        SET_STRING_ELT(x, i, v); }
    //inline SEXP setVectorElement(SEXP x, R_xlen_t i, SEXP v) { return SET_VECTOR_ELT(x, i, v); }


    // cf R_ext/Error.h
    template <typename... Args>
    inline void error(const char *msg, Args... args)   { Rf_error(msg, args...);   }
    // Version above tickles R-devel warning for format string, need to unroll Args... here.
    // Can be used with
    //   template <typename... Args>
    //   inline void errro(const char* fmt, Args&&... args ) {
    //     Rf_error("%s", tfm::format(fmt, std::forward<Args>(args)... ).c_str());
    // if tinyformat headers are available (as in Rcpp)
    // Version below is an alternative for pre-formed strings
    inline void error(const char *msg)   			   { Rf_error("%s", msg);      }

    template <typename... Args>
    inline void warning(const char *msg, Args... args) { Rf_warning(msg, args...); }
    // See error() above, same comment applies here
    inline void warning(const char *msg)               { Rf_warning("%s", msg);    }

    inline void message(const char *s)                 { R_ShowMessage(s);         }

}

#endif
