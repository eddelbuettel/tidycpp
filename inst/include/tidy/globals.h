// globals.h: some generally useful settings

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

#ifndef tidyCpp_globals_h
#define tidyCpp_globals_h

// explicit prefixes are clearer
#define STRICT_R_HEADERS

// include R headers, but set R_NO_REMAP and access everything via Rf_ prefixes
#define R_NO_REMAP

// also do not remap for the math function
//#define R_NO_REMAP_RMATH

// this was useful in the Rcpp context
#define MAXELTSIZE 8192

#endif
