/*
 *   Copyright (c) 2011, Michael Lehn
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

/* Based on
 *
      SUBROUTINE DHSEQR( JOB, COMPZ, N, ILO, IHI, H, LDH, WR, WI, Z,
     $                   LDZ, WORK, LWORK, INFO )
 *
 *  -- LAPACK computational routine (version 3.2.2) --
 *     Univ. of Tennessee, Univ. of California Berkeley,
 *     Univ. of Colorado Denver and NAG Ltd..
 *     June 2010
 */

#ifndef FLENS_LAPACK_IMPL_HSEQR_H
#define FLENS_LAPACK_IMPL_HSEQR_H 1

#include <flens/lapack/typedefs.h>
#include <flens/matrixtypes/matrixtypes.h>
#include <flens/vectortypes/vectortypes.h>

namespace flens { namespace lapack {

namespace HSEQR {

    enum Job {
        Eigenvalues = 'E', // compute eigenvalues only
        Schur       = 'S'  // compute eigenvalues and the Schur form T
    };

    enum ComputeZ {
        No     = 'N',   // no Schur vectors are computed;
        Init   = 'I',   // Z is initialized to the unit matrix and the matrix Z
                        // of Schur vectors of H is returned;
        NoInit = 'V',   // Z must contain an orthogonal matrix Q on entry, and
                        // the product Q*Z is returned.
    };

}

//== hseqr_wsq (worksize query) ================================================
template <typename IndexType, typename MH>
    IndexType
    hseqr_wsq(HSEQR::Job            job,
              HSEQR::ComputeZ       computeZ,
              IndexType             iLo,
              IndexType             iHi,
              const GeMatrix<MH>    &H);

//== hseqr =====================================================================
template <typename IndexType, typename MH, typename VWR, typename VWI,
          typename MZ, typename VWORK>
    IndexType
    hseqr(HSEQR::Job                job,
          HSEQR::ComputeZ           compZ,
          IndexType                 iLo,
          IndexType                 iHi,
          GeMatrix<MH>              &H,
          DenseVector<VWR>          &wr,
          DenseVector<VWI>          &wi,
          GeMatrix<MZ>              &Z,
          DenseVector<VWORK>        &work);


//-- forwarding ----------------------------------------------------------------
template <typename IndexType, typename MH>
    IndexType
    hseqr_wsq(HSEQR::Job        job,
              HSEQR::ComputeZ   computeZ,
              IndexType         iLo,
              IndexType         iHi,
              const MH          &&H);

template <typename IndexType, typename MH, typename VWR, typename VWI,
          typename MZ, typename VWORK>
    IndexType
    hseqr(HSEQR::Job            job,
          HSEQR::ComputeZ       compZ,
          IndexType             iLo,
          IndexType             iHi,
          MH                    &&H,
          VWR                   &&wr,
          VWI                   &&wi,
          MZ                    &&Z,
          VWORK                 &&work);

} } // namespace lapack, flens

#endif // FLENS_LAPACK_IMPL_HSEQR_H
