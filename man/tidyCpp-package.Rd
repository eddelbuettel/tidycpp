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
\dontrun{
    ## see the source files in the snippets/ directory of the package
    ## check for (optional, only in Suggests:) Rcpp, and also wrapped in a
    ## dontrun as it takes 10s at CRAN (yet only 3.5 here) yielding a NOTE
    if (requireNamespace("Rcpp", quietly=TRUE)) {
        Rcpp::sourceCpp(system.file("snippets", "convolveExample.cpp", package="tidyCpp"))
    }
}
}
