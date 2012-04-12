/*
 *   Copyright (c) 2012, Michael Lehn
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

#ifndef FLENS_BLAS_CLOSURES_MMSWITCH_TCC
#define FLENS_BLAS_CLOSURES_MMSWITCH_TCC 1

#include <flens/aux/aux.h>
#include <flens/blas/closures/debugclosure.h>
#include <flens/blas/closures/prune.h>
#include <flens/blas/level1/level1.h>
#include <flens/blas/level2/level2.h>
#include <flens/typedefs.h>

#ifdef FLENS_DEBUG_CLOSURES
#   include <flens/blas/blaslogon.h>
#else
#   include <flens/blas/blaslogoff.h>
#endif

namespace flens { namespace blas {

//
//  This switch evaluates closures of the form
//
//      C = beta*C + A*B
//
//  If B is a closure then it gets evaluated and a temporary gets created to
//  store the result.  For matrix A we distinguish between three cases:
//  case 1: A is no closure
//  case 2: A is a scaling closure (i.e. scale*A)
//  case 3: A is some other closure

//
//  Entry point for mmSwitch
//
template <typename ALPHA, typename MA, typename MB, typename BETA, typename MC>
typename RestrictTo<IsSame<MA, typename MA::Impl>::value
                 && IsSame<MB, typename MB::Impl>::value
                 && IsSame<MC, typename MC::Impl>::value,
         void>::Type
mmSwitch(Transpose transA, Transpose transB, const ALPHA &alpha,
         const MA &A, const MB &B, const BETA &beta, MC &C)
{
    ASSERT(alpha==ALPHA(1) || alpha==ALPHA(-1));
//
//  If A is a closure then prune arbitrary many OpTrans/OpConj
//
    typedef typename PruneConjTrans<MA>::Remainder RMA;

    transA = Transpose(transA^PruneConjTrans<MA>::trans);
    const RMA  &_A = PruneConjTrans<MA>::remainder(A);
//
//  If B is a closure then prune arbitrary many OpTrans/OpConj
//
    transB = Transpose(transB^PruneConjTrans<MB>::trans);
//
//  If the remainder B is a closure it gets evaluated.  In this case a temporary
//  gets created.  Otherwise we only keep a reference
//
    FLENS_BLASLOG_TMP_TRON;
    typedef typename PruneConjTrans<MB>::Remainder RMB;
    const typename Result<RMB>::Type &_B = PruneConjTrans<MB>::remainder(B);
    FLENS_BLASLOG_TMP_TROFF;
//
//  Call mm implementation
//
    mmCase(transA, transB, alpha, _A, _B, beta, C);
//
//  If a temporary was created and registered before we now unregister it
//
#   ifdef FLENS_DEBUG_CLOSURES
    if (!IsSame<RMB, typename Result<RMB>::Type>::value) {
        FLENS_BLASLOG_TMP_REMOVE(_B, PruneConjTrans<MB>::remainder(B));
    }
#   else
    const bool check = IsSame<RMB, typename Result<RMB>::Type>::value;
    if (!check) {
        std::cerr << "ERROR: Temporary required." << std::endl;
    }
    ASSERT(check);
#   endif
}

//
//  case 1: A is no closure
//
template <typename ALPHA, typename MA, typename MB, typename BETA, typename MC>
typename RestrictTo<!IsClosure<MA>::value,
         void>::Type
mmCase(Transpose transA, Transpose transB, const ALPHA &alpha,
       const MA &A, const MB &B, const BETA &beta, MC &C)
{
    mm(transA, transB, alpha, A, B, beta, C);
}

//
//  case 2: A is closure of type scale*A
//
template <typename ALPHA, typename T, typename MA, typename MB, typename BETA,
          typename MC>
void
mmCase(Transpose transA, Transpose transB, const ALPHA &alpha,
       const MatrixClosure<OpMult, ScalarValue<T>, MA> &scale_A,
       const MB &B, const BETA &beta, MC &C)
{
//
//  If A is a closure then prune arbitrary many OpTrans/OpConj
//
    typedef typename PruneConjTrans<MA>::Remainder  RMA;
    typedef typename Result<RMA>::Type              ResultRMA;

    transA = Transpose(transA^PruneConjTrans<MA>::trans);
//
//  If the remaining A is a closure it gets evaluated.  In this case
//  a temporary gets created.  Otherwise we only keep a reference
//
    FLENS_BLASLOG_TMP_TRON;
    const RMA &_A      = PruneConjTrans<MA>::remainder(scale_A.right());
    const ResultRMA &A = _A;
    FLENS_BLASLOG_TMP_TROFF;

    mm(transA, transB, alpha*scale_A.left().value(), A, B, beta, C);

//
//  If a temporary was created and registered before we now unregister it
//
#   ifdef FLENS_DEBUG_CLOSURES
    if (!IsSame<RMA, ResultRMA>::value) {
        FLENS_BLASLOG_TMP_REMOVE(A, _A);
    }
#   else
    const bool check = IsSame<RMA, typename Result<RMA>::Type>::value;
    if (!check) {
        std::cerr << "ERROR: Temporary required." << std::endl;
    }
    ASSERT(check);
#   endif
}

//
//  case 3: A is some other closure
//
template <typename ALPHA, typename Op, typename L, typename R, typename MB,
          typename BETA, typename MC>
void
mmCase(Transpose transA, Transpose transB, const ALPHA &alpha,
       const MatrixClosure<Op, L, R> &A, const MB &B, const BETA &beta, MC &C)
{
    typedef MatrixClosure<Op, L, R>  ClosureType;

//
//  Create (most certainly) temporary for the result of A
//
    FLENS_BLASLOG_TMP_TRON;
    typedef typename Result<ClosureType>::Type  MA;
    const MA &_A = A;
    FLENS_BLASLOG_TMP_TROFF;

    mm(transA, transB, alpha, _A, B, beta, C);

//
//  If a temporary was created and registered before we now unregister it
//
#   ifdef FLENS_DEBUG_CLOSURES
    if (!IsSame<ClosureType, MA>::value) {
        FLENS_BLASLOG_TMP_REMOVE(_A, A);
    }
#   else
    const bool check = IsSame<ClosureType, MA>::value;
    if (!check) {
        std::cerr << "ERROR: Temporary required." << std::endl;
    }
    ASSERT(check);
#   endif
}

} } // namespace blas, flens

#endif // FLENS_BLAS_CLOSURES_MMSWITCH_TCC
