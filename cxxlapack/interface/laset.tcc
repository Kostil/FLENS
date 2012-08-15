/*
 *   Copyright (c) 2012, Michael Lehn, Klaus Pototzky
 *
 *   All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *   1) Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2) Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in
 *      the documentation and/or other materials provided with the
 *      distribution.
 *   3) Neither the name of the FLENS development group nor the names of
 *      its contributors may be used to endorse or promote products derived
 *      from this software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef CXXLAPACK_INTERFACE_LASET_TCC
#define CXXLAPACK_INTERFACE_LASET_TCC 1

#include <cxxlapack/netlib/netlib.h>

namespace cxxlapack {

template <typename IndexType>
void
laset(char                  uplo,
      IndexType             m,
      IndexType             n,
      float                 alpha,
      float                 beta,
      float                 *A,
      IndexType             ldA)
{
    CXXLAPACK_DEBUG_OUT("slaset");
    
    LAPACK_IMPL(slaset)(&uplo,
                        &m,
                        &n,
                        &alpha,
                        &beta,
                        A,
                        &ldA);
}


template <typename IndexType>
void
laset(char                  uplo,
      IndexType             m,
      IndexType             n,
      double                alpha,
      double                beta,
      double                *A,
      IndexType             ldA)
{
    CXXLAPACK_DEBUG_OUT("dlaset");
   
    LAPACK_IMPL(dlaset)(&uplo,
                        &m,
                        &n,
                        &alpha,
                        &beta,
                        A,
                        &ldA);
}


template <typename IndexType>
void
laset(char                  uplo,
      IndexType             m,
      IndexType             n,
      std::complex<float >  alpha,
      std::complex<float >  beta,
      std::complex<float >  *A,
      IndexType             ldA)
{
    CXXLAPACK_DEBUG_OUT("claset");
   
    LAPACK_IMPL(claset)(&uplo,
                        &m,
                        &n,
                        reinterpret_cast<const float  *>(&alpha),
                        reinterpret_cast<const float  *>(&beta),
                        reinterpret_cast<float  *>(A),
                        &ldA);
}

template <typename IndexType>
void
laset(char                  uplo,
      IndexType             m,
      IndexType             n,
      std::complex<double>  alpha,
      std::complex<double>  beta,
      std::complex<double>  *A,
      IndexType             ldA)
{
    CXXLAPACK_DEBUG_OUT("zlaset");
   
    LAPACK_IMPL(zlaset)(&uplo,
                        &m,
                        &n,
                        reinterpret_cast<const double *>(&alpha),
                        reinterpret_cast<const double *>(&beta),
                        reinterpret_cast<double *>(A),
                        &ldA);
}

} // namespace cxxlapack

#endif // CXXLAPACK_INTERFACE_LASET_TCC
