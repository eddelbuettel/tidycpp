\name{NEWS}
\title{News for Package \pkg{tidyCpp}}
\newcommand{\ghpr}{\href{https://github.com/eddelbuettel/tidycpp/pull/#1}{##1}}
\newcommand{\ghit}{\href{https://github.com/eddelbuettel/tidycpp/issues/#1}{##1}}

\section{Changes in tidyCpp version 0.0.5.2 (2021-11-20)}{
  \itemize{
    \item Assign nullptr in dtor for \code{Protect} class
    \item Switch vignette engine to \pkg{simplermarkdown}
  }
}

\section{Changes in tidyCpp version 0.0.5 (2021-09-16)}{
  \itemize{
    \item The \code{Protect} class uses default copy and move assignments
    and constructors
    \item The data object in \code{NumVec} is now a \code{Protect} object
  }
}

\section{Changes in tidyCpp version 0.0.4 (2021-09-05)}{
  \itemize{
    \item Minor updates to DESCRIPTION
    \item New snippet \code{rollminmaxExample} with simple vector use
    \item New class \code{NumVec} motivated from rolling min/max example
    \item Expand the vignette with C++ example based on NumVec
    \item Add a table of contents to the vignette
  }
}

\section{Changes in tidyCpp version 0.0.3 (2021-03-07)}{
  \itemize{
    \item Switch CI use to \href{https://eddelbuettel.github.io/r-ci/}{r-ci}
    \item Protect nil object definition
  }
}

\section{Changes in tidyCpp version 0.0.2 (2020-11-12)}{
  \itemize{
    \item Expanded definitions in \code{internals.h} to support new example.
    \item The vignette has been extended with an example based on
    package \pkg{uchardet}.
    \item Class \code{Shield} has been replaced by an new class
    \code{Protect}; a compatibility \code{typdef} has been added.
    \item The examples and vignette have been clarified with respect to
    proper ownership of protected objects; a new vignette section was added.
  }
}

\section{Changes in tidyCpp version 0.0.1 (2020-09-27)}{
  \itemize{
    \item Initial release with several basic headers and introductory vignette
  }
}
