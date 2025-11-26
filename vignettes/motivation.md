<!--
%\VignetteIndexEntry{tidyCpp Motivation}
%\VignetteEngine{simplermarkdown::mdweave_to_html}
%\VignetteEncoding{UTF-8}
-->
---
title: "tidyCpp Motivation"
author: "Dirk Eddelbuettel"
date: "Initial version November 2020; Updated August 2021 and November 2025"
css: "water.css"
---

## Introducing tidyCpp

The `tidyCpp` package offers a simple, small and clean C++ layer over the C API offered by R.
As of version 0.0.4, it also adds a (truly minimal) numeric vector class for C++.
This vignette highlights a few usage examples, often taken from the [_Writing R Extensions_ vignette that comes
with R](https://cran.r-project.org/doc/manuals/r-release/R-exts.html), to highlight some features.

`tidyCpp` has no further dependencies on any other package.  It can however be used with
[Rcpp](https://www.rcpp.org) simply to take advantage of its helper functions `cppFunction()` or
`sourceCpp()`.

`tidyCpp` is still a fairly small package.  Please free to contribute by make suggestions, or
sending bugfixes or extension proposals.


### Snippet One: dimnames

This example comes from [Writing R Extension, Section
5.9.4](https://cran.r-project.org/doc/manuals/r-release/R-exts.html#Attributes) which highlights
attribute setting from the C API.

It takes two (named) numeric vectors, computes the outer product matrix and uses the names to set
row- and column names. Note that we modified the existing example ever so slight by ensuring (as is
frequently done) remapping of symbols. For example, `length` (which can clash easily with existing
symbols in the global namespace) is now `Rf_length`.  We also added an `export` tag for `Rcpp`
simply to facilitate integration into R.  No Rcpp header or data structures are used; we simply rely
on its logic in getting C or C++ source into R.

:::::: {.columns}
::: {.column width="49.75%"}

#### Using the C API for R

```c++
#define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>

// [[Rcpp::export]]
SEXP out(SEXP x, SEXP y)
{
  int nx = Rf_length(x), ny = Rf_length(y);
  SEXP ans =
    PROTECT(Rf_allocMatrix(REALSXP, nx, ny));
  double *rx = REAL(x),
    *ry = REAL(y),
    *rans = REAL(ans);

  for(int i = 0; i < nx; i++) {
    double tmp = rx[i];
    for(int j = 0; j < ny; j++)
      rans[i + nx*j] = tmp * ry[j];
  }

  SEXP dimnames =
    PROTECT(Rf_allocVector(VECSXP, 2));
  SET_VECTOR_ELT(dimnames, 0,
                 Rf_getAttrib(x,R_NamesSymbol));
  SET_VECTOR_ELT(dimnames, 1,
                 Rf_getAttrib(y,R_NamesSymbol));
  Rf_setAttrib(ans, R_DimNamesSymbol, dimnames);

  UNPROTECT(2);
  return ans;
}
```

:::
::: {.column width="0.5%"}
&nbsp;
:::
::: {.column width="49.75%"}

#### Using tidyCpp


```c++
#include <tidyCpp>
// [[Rcpp::depends(tidyCpp)]]


// [[Rcpp::export]]
SEXP out(SEXP x, SEXP y)
{
  int nx = R::length(x), ny = R::length(y);
  R::Protect ans(R::allocMatrixReal(nx, ny));

  double *rx = R::numericPointer(x),
    *ry = R::numericPointer(y),
    *rans = R::numericPointer(ans);

  for(int i = 0; i < nx; i++) {
    double tmp = rx[i];
    for(int j = 0; j < ny; j++)
      rans[i + nx*j] = tmp * ry[j];
  }

  R::Protect dimnames(R::allocVectorList(2));

  R::setVectorElement(dimnames, 0,
                      R::getNames(x));
  R::setVectorElement(dimnames, 1,
                      R::getNames(y));
  R::setDimNames(ans, dimnames);

  return ans;
}
```
:::
::::::

Some key differences:

- a single header `tidyCpp`: simple and clean;
- no `PROTECT` and `UNPROTECT` with manual calling of the number of calls made: C++ takes care of
  that for us via `Protect` which is a modernized (and simplified) version of
  class `Shield` in Rcpp (and see below for more discussion of `Protect`);
- no `Rf_*` calls: everything used comes from a clean new namespace `R` and is easily identified;
- types are made explicit in the name of the called function sequence rather than enum;
- consistent naming that aligns with language convention:
    - _types_ such as `Protect` are capitalized, and
    - _verbs_ such as the allocators or converters are camelCase;
- overall less wordy and shorter, _e.g._, `R::getNames(x)` instead of `Rf_getAttrib(x, R_NamesSymbol)`;
- no macros are being used.

Note that the use of `Rcpp::export` does not imply use of Rcpp data structures.  We simply take
advantage of the tried and true code generation to make it easy to call the example from R. You can
copy either example into a temporary file and use `Rcpp::sourceCpp("filenameHere")` on it to run the
example.



### Snippet Two: convolution

This example comes from [Writing R Extension, Section
5.10.1](https://cran.r-project.org/doc/manuals/r-release/R-exts.html#Calling-_002eCall) which
introduces the `.Call()` interface of the C API for R.

It takes two numeric vectors and computes a convolution. Note that as above we modified the existing
example ever so slight by ensuring (as is frequently done) remapping of symbols, once again added an
`export` tag for `Rcpp` simply to facilitate integration into R, and changing whitespace.  No Rcpp
header or data structures are used; we simply rely on its logic in getting C or C++ source into R.

:::::: {.columns}
::: {.column width="49.75%"}

#### Using the C API for R

```c++
#define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>

// [[Rcpp::export]]
SEXP convolve2(SEXP a, SEXP b)
{
  int na, nb, nab;
  double *xa, *xb, *xab;
  SEXP ab;

  a = PROTECT(Rf_coerceVector(a, REALSXP));
  b = PROTECT(Rf_coerceVector(b, REALSXP));
  na = Rf_length(a);
  nb = Rf_length(b);
  nab = na + nb - 1;
  ab = PROTECT(Rf_allocVector(REALSXP, nab));
  xa = REAL(a);
  xb = REAL(b);
  xab = REAL(ab);
  for(int i = 0; i < nab; i++)
    xab[i] = 0.0;
  for(int i = 0; i < na; i++)
    for(int j = 0; j < nb; j++)
      xab[i + j] += xa[i] * xb[j];
  UNPROTECT(3);
  return ab;
}
```

:::
::: {.column width="0.5%"}
&nbsp;
:::
::: {.column width="49.75%"}

#### Using tidyCpp


```c++
#include <tidyCpp>
// [[Rcpp::depends(tidyCpp)]]


// [[Rcpp::export]]
SEXP convolve2(SEXP a, SEXP b)
{
  int na, nb, nab;
  double *xa, *xb, *xab;


  R::Protect pa(R::coerceVectorNumeric(a));
  R::Protect pb(R::coerceVectorNumeric(b));
  na = R::length(pa);
  nb = R::length(pb);
  nab = na + nb - 1;
  R::Protect ab(R::allocVectorNumeric(nab));
  xa = R::numericPointer(pa);
  xb = R::numericPointer(pb);
  xab = R::numericPointer(ab);
  for(int i = 0; i < nab; i++)
    xab[i] = 0.0;
  for(int i = 0; i < na; i++)
    for(int j = 0; j < nb; j++)
      xab[i + j] += xa[i] * xb[j];
  return ab;
}
```
:::
::::::

Like the previous example, the new version operates without macros, does not require manual counting
in `PROTECT` and `UNPROTECT` and is, to our eyes, a little more readable.

As the existing example from [Writing R Extension, Section
5.10.1](https://cran.r-project.org/doc/manuals/r-release/R-exts.html#Calling-_002eCall)
used `PROTECT` on the two incoming `SEXP` objects (whereas the previous
example, from the same source, does not) we need to allocate two temporary
objects `pa` and `pb` with the explicit C++ ownership providing the protect
and unprotect pairing.  Because `pa` and `pb` go out of scope at the end of
the function, the destructor will then unprotect correctly.

(And as above, the two 'tags' for Rcpp use are present only to facilitate use
via the `Rcpp::sourceCpp()` package.)


### Snippet Three: uchardet

For the third example, we use an unrelated package:
[uchardet](https://cran.r-project.org/package=uchardet) which provides R bindings to the eponymous
C++ library to detect character encodings. This example cannot be sourced simply into R as it
requires the underlying C++ library.  One can, however, download the R package and then replace
the file `src/detect.cpp` with the content below, and add `LinkingTo: tidyCpp` to the `DESCRIPTION`
file.

```c++
#include <tidyCpp>
#include <R_ext/Visibility.h>
#include <fstream>
#include <uchardet.h>

#define BUFFER_SIZE 65536
char buffer[BUFFER_SIZE];

SEXP attribute_hidden get_charset(uchardet_t handle) {
  const char* ans = uchardet_get_charset(handle);
  if (strlen(ans) == 0) {
    // Rf_warning("Can not detect encoding.");
    return NA_STRING;
  }
  return R::mkChar(ans);
}

SEXP attribute_hidden do_detect_sexp(SEXP x, uchardet_t handle) {
  R_xlen_t x_len = R::xlength(x);
  if (x_len == 0) {
    return NA_STRING;
  }
  const char* x_data;
  switch(R::Typeof(x)) {
    case CHARSXP: {
      if (x == NA_STRING) {
        return NA_STRING;
      }
      x_data = R::charPointer(x);
      break;
    }
    case RAWSXP: {
      x_data = (const char*) R::rawPointer(x);
      break;
    }
    default: {
      R::warning("Unsupported data type '%s'.", R::type2char(R::Typeof(x)));
      return NA_STRING;
    }
  }
  int retval = uchardet_handle_data(handle, x_data, x_len);
  uchardet_data_end(handle);
  if (retval != 0) {
    // Rf_warning("Can not handling data.");
    return NA_STRING;
  }

  return get_charset(handle);
}

SEXP attribute_hidden do_detect_file(SEXP x, uchardet_t handle) {
  if (x == NA_STRING) {
    return NA_STRING;
  }

  const char* fname = R::charPointer(x);
  std::ifstream fs(R_ExpandFileName(fname), std::ios::binary);
  if (!fs.is_open()) {
    R::warning("Can not open file '%s'.", fname);
    return NA_STRING;
  }

  while (!fs.eof()) {
    fs.read(buffer, BUFFER_SIZE);
    std::size_t len = fs.gcount();
    uchardet_handle_data(handle, buffer, len);
  }
  uchardet_data_end(handle);
  fs.close();

  return get_charset(handle);
}

template<typename T>
SEXP attribute_hidden do_detect_vec(SEXP x, T fun) {
  if (R::Typeof(x) != STRSXP) {
    R::error("'x' must be character vector.");
  }
  R_xlen_t n = R::xlength(x);
  R::Protect res(R::allocVectorCharacter(n));
  uchardet_t handle = uchardet_new();
  for (R_len_t i = 0; i < n; ++i) {
    R::setStringElement(res, i, fun(R::stringElement(x, i), handle));
    uchardet_reset(handle);
  }
  uchardet_delete(handle);
  return res;
}

SEXP detect_character(SEXP x) {
  return do_detect_vec(x, do_detect_sexp);
}

SEXP detect_file(SEXP x) {
  return do_detect_vec(x, do_detect_file);
}

SEXP detect_raw(SEXP x) {
  if (R::Typeof(x) != RAWSXP) {
    R::error("'x' must be raw vector.");
  }
  R::Protect res(R::allocVectorCharacter(1));
  uchardet_t handle = uchardet_new();
  R::setStringElement(res, 0, do_detect_sexp(x, handle));
  uchardet_delete(handle);
  return res;
}

```

### Snippet Four: Rolling min and max

For the fourth example, we modified a function from an older version of package
[ichimoku](https://cran.r-project.org/package=uchardet). It implements a rolling minimum and maximum
operator (in an earlier version of the package, see _e.g._ [for the
file](https://github.com/shikokuchuo/ichimoku/blob/6462c5ee65642f8018bd51ae529499c705a15c95/src/windowfns.cpp).
Its use of Rcpp is fairly standard, however the package chose to make changes based on the
compilation time so we took a look too. The version below deploys `tidyCpp` and its new `numvec`
header and class instead (and we tested this in a local fork of the package).

To make the code fit in the dual display below, we added linebreaks on the left, and adjusted
whitespace. Our code on the right also slightly changes the interface by simplyfing the
implementation: without the `enum` and `struct Args` and dual callers, we add a third argument
to determine whether we operate as min or max which saves the two extra functions at the bottom.
We also altered whitespace away from our preferred use of four spaces; see the original function
(also containing full copyright headers and more) [here](https://github.com/eddelbuettel/dang/blob/master/src/rollMinMax.cpp).

The key point, however, is immediately apparent.  The two versions are essentially identical (though
the Rcpp version will have more type checks, exception handling, wrapper generation and all the
other reasons why we often use Rcpp).

:::::: {.columns}
::: {.column width="49.75%"}

#### Using the Rcpp version

```c++
#include <deque>
#include <utility>
#include <Rcpp.h>
using namespace Rcpp;

// types of calculations
enum CalcType {MIN, MAX};

// function arguments for non-data
struct Args {
  int window;
  CalcType ctype;
};

// calculates rolling window for {min, max}
NumericVector
roll_minmax(const NumericVector& x, Args a) {

  int n  = x.length();
  NumericVector rollx(n);

  std::deque<std::pair<long double, int>> deck;
  for (int i = 0; i < x.size(); ++i) {
    if(a.ctype == MIN) {
      while (!deck.empty() &&
             deck.back().first >= x[i])
        deck.pop_back();
    } else {
      while (!deck.empty() &&
             deck.back().first <= x[i])
        deck.pop_back();
    }
    deck.push_back(std::make_pair(x[i], i));

    while(deck.front().second <= i - a.window)
      deck.pop_front();

    long double min = deck.front().first;
    if (i < a.window - 1) {
      rollx[i] = NA_REAL;
    } else {
      rollx[i] = min;
    }
  }
  return rollx;
}

// [[Rcpp::export]]
NumericVector maxOver(const SEXP& x,
                      int window) {
  Args a;
  a.window = window;
  a.ctype = MAX;
  return roll_minmax(x, a);
}

// [[Rcpp::export]]
NumericVector minOver(const SEXP& x,
                      int window) {
  Args a;
  a.window = window;
  a.ctype = MIN;
  return roll_minmax(x, a);
}
```

:::
::: {.column width="0.5%"}
&nbsp;
:::
::: {.column width="49.75%"}

#### Using tidyCpp

```c++
#include <deque>
#include <utility>
#include <tidyCpp>

extern "C" {
// forward declaration
tidy::NumVec rollMinMax(tidy::NumVec x,
                        int window, bool isMin);
// this SEXP variant is referenced from init.c
SEXP _rollMinMax(SEXP x, SEXP win, SEXP isMin){
  return rollMinMax(x, R::asInteger(win),
                    R::asLogical(isMin)));
}

// Calculates rolling window for {min, max}
tidy::NumVec
rollMinMax(tidy::NumVec x, int win, bool isMin){

  int n  = R::length(x);
  tidy::NumVec rollx(n);

  std::deque<std::pair<long double, int>> deck;
  for (int i = 0; i < n; ++i) {
    if (isMin) {
      while (!deck.empty() &&
             deck.back().first >= x[i])
        deck.pop_back();
    } else {
      while (!deck.empty() &&
             deck.back().first <= x[i])
        deck.pop_back();
    }
    deck.push_back(std::make_pair(x[i], i));

    while(deck.front().second <= i - win)
      deck.pop_front();

    long double min = deck.front().first;
    if (i < win - 1) {
      rollx[i] = NA_REAL;
    } else {
      rollx[i] = min;
    }
  }
  return rollx;
}

} // extern "C"
```

:::
::::::

### PROTECT and UNPROTECT

The `R::Protect()` class ensures proper `PROTECT` wrapping for the lifetime of an object. A very
important, yet easy-to-overlook, detail is that the form of assigning to a `SEXP` variable _looks_
correct, but is in fact incorrect. For example in `SEXP ans = R::Protect(R::allocMatrixReal(nx,
ny));`, the class will correctly construct around the result from `allocMatrix()`. But as it
assigned to a `SEXP` variable, the compiler realizes that it is a temporary object and after calling
`operator SEXP()` the destructor is called---essentially immediately.

So the correct use is to write `R::Protect ans(R::allocMatrixReal(nx, ny));` which turns this into
an instance of the the `Protect` which will live to the end of the scope and only `UNRPROTECT` via
the destructor at the end of the scope.

The example file `snippets/protectExamples.cpp` illustrates this by using
[RcppSpdlog](https://cran.r-project.org/package=RcppSpdlog) to log invocation of the three relevant
parts constructor, destructor and `operator SEXP()` for both the correct and incorrect form of the
`convolution()` example. When `sourceCpp()`-ed into an R session, and skipping the first example in
the file, we may see the following result (with of course different timestamps).

```r
R> convolveIncorrect(c(1, 2, 3), c(4, 5, 6))
[11:25:56.418260] starting convolveIncorrect
[11:25:56.418286] entered ctor
[11:25:56.418288] entered SEXP()
[11:25:56.418290] entered dtor
[11:25:56.418291] entered ctor
[11:25:56.418293] entered SEXP()
[11:25:56.418295] entered dtor
[11:25:56.418297] entered ctor
[11:25:56.418298] entered SEXP()
[11:25:56.418300] entered dtor
[11:25:56.418302] ending convolveIncorrect
[1]  4 13 28 27 18

R> convolveCorrect(c(1, 2, 3), c(4, 5, 6))
[11:25:56.418431] starting convolveCorrect
[11:25:56.418434] entered ctor
[11:25:56.418435] entered ctor
[11:25:56.418436] entered SEXP()
[11:25:56.418438] entered SEXP()
[11:25:56.418439] entered ctor
[11:25:56.418441] entered SEXP()
[11:25:56.418442] entered SEXP()
[11:25:56.418444] entered SEXP()
[11:25:56.418445] ending convolveCorrect
[11:25:56.418447] entered SEXP()
[11:25:56.418449] entered dtor
[11:25:56.418450] entered dtor
[11:25:56.418452] entered dtor
[1]  4 13 28 27 18
```

In the first example, we see that the triplet constructor/SEXP()/destructor is called twice when the
two vectors are coerced, and then a third time when the result vector is allocated. This shows the
incorrect behavior: destructors essentially immediately after constructors, leaving the object
unprotected---which is not what was intended.

The second example shows the correct behavior. Two constructor calls, then two `SEXP()` calls (when
the length are determined), another constructor followed by three `SEXP()` calls from the three
`numericPointer()` calls---as well as a final `SEXP()` call for the return result and three
destructors at end. This is the intended behavior of protecting the three objects during their
lifetime.

### Discussion

`tidyCpp` provides a cleaner layer on top of the C API for R (as well as a so-far still minimal C++
class layer). That has its advantages: we find it more readable. It conceivably has possible
disadvantages. Those familiar with the C API for R may not need this, and may find it an unnecessary
new dialect. Time will tell if new adoption and use may outweigh possible hesitation by other.  In the
meantime, the package "does no harm", has no further dependencies and can be used, or dropped, at
will.


### Summary

The `tidyCpp` package provides a simplifying layer on top of the time-tested but somewhat crusty C
API for R.
