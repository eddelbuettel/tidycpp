
## tidyCpp: Tidy C++ wrapping of the C API for R

[![CI](https://github.com/eddelbuettel/tidycpp/workflows/ci/badge.svg)](https://github.com/eddelbuettel/tidycpp/actions?query=workflow%3Aci)
[![License](https://img.shields.io/badge/license-GPL%20%28%3E=%202%29-brightgreen.svg?style=flat)](https://www.r-project.org/Licenses/GPL-2)
[![CRAN](https://www.r-pkg.org/badges/version/tidyCpp)](https://cran.r-project.org/package=tidyCpp)
[![r-universe](https://eddelbuettel.r-universe.dev/badges/tidyCpp)](https://eddelbuettel.r-universe.dev/tidyCpp)
[![Dependencies](https://tinyverse.netlify.app/badge/tidyCpp)](https://cran.r-project.org/package=tidyCpp)
[![Downloads](https://cranlogs.r-pkg.org/badges/tidyCpp?color=brightgreen)](https://www.r-pkg.org/pkg/tidyCpp)
[![Last Commit](https://img.shields.io/github/last-commit/eddelbuettel/tidycpp)](https://github.com/eddelbuettel/tidycpp)

### About

The C API of the R language and environment is robust, yet a little awkward for C++ programmers.
This package wraps a tidy layer of header functions around it.

R Core is moving towards a more explicit classification of which functions are officially part of
the API for R, and which ones are 'experimental'.  A [very helpful contributed
page](https://aitap.codeberg.page/R-api) summarizes the status.

### Examples

A recent motivating example is from the
[dang](https://github.com/eddelbuettel/dang/blob/master/src/checkNonAscii.cpp) package which
includes one unexported Base R function written in C. The original version is
[here](https://github.com/wch/r-source/blob/trunk/src/library/tools/src/text.c#L140-L184). While
short enough, we found working with a _tidier_ version easier and more enjoyable. We include an
abbreviated version (minus the copyright header) here:

```c++
#include <tidyCpp>

extern "C" {

// cf src/library/tools/src/text.c
SEXP _check_nonASCII(SEXP text, SEXP ignore_quotes) {
    /* Check if all the lines in 'text' are ASCII, after removing
       comments and ignoring the contents of quotes (unless ignore_quotes)
       (which might span more than one line and might be escaped).

       This cannot be entirely correct, as quotes and \ might occur as
       part of another character in a MBCS: but this does not happen
       in UTF-8.
    */
    int i, nbslash = 0; /* number of preceding backslashes */
    const char *p;
    char quote= '\0';
    Rboolean ign, inquote = FALSE;

    if (!R::isString(text)) R::error("invalid input");
    ign = (Rboolean) R::asLogical(ignore_quotes);
    if (ign == NA_LOGICAL) R::error("'ignore_quotes' must be TRUE or FALSE");

    for (i = 0; i < R::length(text); i++) {
        p = R::charPointer(R::stringElement(text, i)); // ASCII or not not affected by charset
        inquote = FALSE; /* avoid runaway quotes */
        for (; *p; p++) {
            if (!inquote && *p == '#') break;
            if (!inquote || ign) {
                if ((unsigned int) *p > 127) {
                    Rprintf("%s\n", R::charPointer(R::stringElement(text, i)));
                    Rprintf("found %x\n", (unsigned int) *p);
                    return R::scalarLogical(TRUE);
                }
            }
            if ((nbslash % 2 == 0) && (*p == '"' || *p == '\'')) {
                if (inquote && *p == quote) {
                    inquote = FALSE;
                } else if(!inquote) {
                    quote = *p;
                    inquote = TRUE;
                }
            }
            if (*p == '\\') nbslash++; else nbslash = 0;
        }
    }
    return R::scalarLogical(FALSE);
}

} // extern "C"
```

Additions from the `tidyCpp` wrapper can be identified easily via the `R::` namespace. While the
code is not hiding its C heritage, we find it overall a little easier to work with. Other examples
(see below) show how using `tidyCpp` can help avoid the very manual and error-prone `PROTECT` and
`UNPROTECT` and pairs.

For additional examples, please see the
[Motivation](https://cran.r-project.org/package=tidyCpp/vignettes/motivation.html) vignette for a
longer discussion with more examples, and the files in the
[snippets/](https://github.com/eddelbuettel/tidycpp/tree/master/inst/snippets) directory for
directly callable code examples.

### Author

Dirk Eddelbuettel

### License

GPL (>= 2)
