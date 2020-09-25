\name{tidyCpp-package}
\alias{tidyCpp-package}
\alias{tidyCpp}
\docType{package}
\title{\packageTitle{tidyCpp}}
\description{\packageDescription{tidyCpp}}
\section{Package Content}{\packageIndices{tidyCpp}}
\author{\packageAuthor{tidyCpp}}
\section{Maintainer}{\packageMaintainer{tidyCpp}}
\keyword{package}
\examples{
## see the source files in the snippets/ directory of the package
## check for (optional, only in Suggests:) Rcpp, and ensure we are
## but not on Windows as this runs up to the ten second time limit
if (requireNamespace("Rcpp", quietly=TRUE) && (.Platform$OS.type != "windows")) {
    Rcpp::sourceCpp(system.file("snippets", "dimnamesExample.cpp", package="tidyCpp"))
}
}
