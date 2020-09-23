//                                               Emacs make this -*- mode: C++; -*-
// tidyCpp: wrapping parts of the C API of R
//
// defines.h: tidying some parts of Rdefines.h

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

#ifndef tidyCpp_defines_h
#define tidyCpp_defines_h

#include <Rdefines.h>

namespace R {                   // we remain all tidied up in a namespace

    // maybe better an extern with global in src/init.c
    //#define Nil R_NilValue

    inline SEXP coerceVectorLogical(SEXP x)   { return Rf_coerceVector(x, LGLSXP);  }
    inline SEXP coerceVectorInteger(SEXP x)   { return Rf_coerceVector(x, INTSXP);  }
    inline SEXP coerceVectorNumeric(SEXP x)   { return Rf_coerceVector(x, REALSXP); }
    inline SEXP coerceVectorCharacter(SEXP x) { return Rf_coerceVector(x, STRSXP);  }
    inline SEXP coerceVectorComplex(SEXP x)   { return Rf_coerceVector(x, CPLXSXP); }
    inline SEXP coerceVector(SEXP x)          { return Rf_coerceVector(x, VECSXP);  }
    inline SEXP coerceList(SEXP x)            { return Rf_coerceVector(x, VECSXP);  }
    inline SEXP coerceVectorRaw(SEXP x)       { return Rf_coerceVector(x, RAWSXP);  }

    inline Rboolean isLogical(SEXP x)    { return Rf_isLogical(x); }
    inline Rboolean isInteger(SEXP x)    { return Rf_isInteger(x); }
    inline Rboolean isNumeric(SEXP x)    { return Rf_isReal(x);    }
    inline Rboolean isCharacter(SEXP x)  { return Rf_isString(x);  }
    inline Rboolean isComplex(SEXP x)    { return Rf_isComplex(x); }
    inline Rboolean isVector(SEXP x)     { return Rf_isVector(x);  }
    inline Rboolean isList(SEXP x)       { return Rf_isList(x);    }
    inline Rboolean isRaw(SEXP x)        { return Rboolean(TYPEOF(x) == RAWSXP); }

    inline SEXP allocVectorLogical(R_xlen_t n)   { return Rf_allocVector(LGLSXP, n);  }
    inline SEXP allocVectorInteger(R_xlen_t n)   { return Rf_allocVector(INTSXP, n);  }
    inline SEXP allocVectorNumeric(R_xlen_t n)   { return Rf_allocVector(REALSXP, n); }
    inline SEXP allocVectorCharacter(R_xlen_t n) { return Rf_allocVector(STRSXP, n);  }
    inline SEXP allocVectorComplex(R_xlen_t n)   { return Rf_allocVector(CPLXSXP, n); }
    inline SEXP allocVectorList(R_xlen_t n)      { return Rf_allocVector(VECSXP, n);  }
    inline SEXP allocString(R_xlen_t n)          { return NEW_CHARACTER(n);           }
    inline SEXP allocVectorRaw(R_xlen_t n)       { return Rf_allocVector(RAWSXP, n);  }

    inline int*      logicalPointer(SEXP x)   { return LOGICAL(x);    }
    inline int*      integerPointer(SEXP x)   { return INTEGER(x);    }
    inline double*   numericPointer(SEXP x)   { return REAL(x);       }
    inline SEXP*     stringPointer(SEXP x)    { return STRING_PTR(x); }
    inline Rcomplex* complexPointr(SEXP x)    { return COMPLEX(x);    }
    inline SEXP*     listPointer(SEXP x)      { return VECTOR_PTR(x); }  // need USE_RINTERNALS
    inline Rbyte*    rawPointer(SEXP x)       { return RAW(x);        }

    inline const char*  charPointer(SEXP x)   { return CHAR(x);       }

    inline void  setVectorElement(SEXP x, R_xlen_t i, SEXP val) { SET_VECTOR_ELT(x, i, val); }

    inline SEXP  getAttrib(SEXP x, SEXP what) { return(Rf_getAttrib(x, what));             }
    inline SEXP  getClass(SEXP x)             { return(Rf_getAttrib(x, R_ClassSymbol));    }
    inline SEXP  getDim(SEXP x)               { return(Rf_getAttrib(x, R_DimSymbol));      }
    inline SEXP  getDimNames(SEXP x)          { return(Rf_getAttrib(x, R_DimNamesSymbol)); }
    inline SEXP  getColNames(SEXP x)          { return(Rf_GetColNames(x)); }
    inline SEXP  getRowNames(SEXP x)          { return(Rf_GetRowNames(x)); }
    inline SEXP  getLevels(SEXP x)            { return(Rf_getAttrib(x, R_LevelsSymbol));   }
    inline SEXP  getTsp(SEXP x)               { return(Rf_getAttrib(x, R_TspSymbol));      }
    inline SEXP  getNames(SEXP x)             { return(Rf_getAttrib(x, R_NamesSymbol));    }

    inline SEXP  setAttrib(SEXP x, SEXP what, SEXP n) { return(Rf_setAttrib(x, what, n));  }
    inline SEXP  setClass(SEXP x, SEXP n)             { return(Rf_setAttrib(x, R_ClassSymbol, n));    }
    inline SEXP  setDim(SEXP x, SEXP n)               { return(Rf_setAttrib(x, R_DimSymbol, n));      }
    inline SEXP  setDimNames(SEXP x, SEXP n)          { return(Rf_setAttrib(x, R_DimNamesSymbol, n)); }
    inline SEXP  setLevels(SEXP x, SEXP n)            { return(Rf_setAttrib(x, R_LevelsSymbol, n));   }
    inline SEXP  setNames(SEXP x, SEXP n)             { return(Rf_setAttrib(x, R_NamesSymbol, n));    }

    // not for long vectors
    inline R_xlen_t getLength(SEXP x)               { return Rf_length(x); }
    inline SEXP     setLength(SEXP x, R_xlen_t n)   { x = Rf_lengthgets(x, n); return x; }

    inline SEXP getSlot(SEXP x, SEXP what)              { return R_do_slot(x, what);               }
    inline SEXP setSlot(SEXP x, SEXP what, SEXP value)  { return R_do_slot_assign(x, what, value); }

    inline SEXP makeClass(const char* what)             { return R_do_MAKE_CLASS(what);      }
    inline SEXP newObject(SEXP class_def)               { return R_do_new_object(class_def); }

    //inline SEXP createUserString(SEXP x)                { return mkChar(x); }
    //inline SEXP createStringVector(SEXP x)              { return mkChar(x); }
    //inline SEXP createFunctionCall(SEXP name, SEXP args) { return ::createFunctionCall(name, args); }


    inline int asLogical(SEXP x)                  { return Rf_asLogical(x);    }
    inline int asInteger(SEXP x)                  { return Rf_asInteger(x);    }
    inline double asNumeric(SEXP x)               { return Rf_asReal(x);       }
    inline const char* asCharacter(SEXP x)        { return CHAR(Rf_asChar(x)); }

}

#endif
