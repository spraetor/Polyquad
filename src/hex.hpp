/*
    This file is part of polyquad.
    Copyright (C) 2014  Freddie Witherden <freddie@witherden.org>

    Polyquad is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    Polyquad is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with polyquad.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef POLYQUAD_HEX_HPP_
#define POLYQUAD_HEX_HPP_

#include "base.hpp"
#include "jacobi_poly.hpp"
#include "util.hpp"

#include <Eigen/Dense>

#include <cassert>

namespace polyquad {

template<typename T>
class HexDomain : public BaseDomain<HexDomain<T>, T, 3, 7>
{
public:
    typedef BaseDomain<HexDomain<T>, T, 3, 7> Base;
    typedef typename Base::MatrixXT MatrixXT;
    typedef typename Base::VectorXT VectorXT;
    typedef typename Base::MatrixPtsT MatrixPtsT;
    typedef typename Base::VectorOrb VectorOrb;

    typedef Eigen::Matrix<T, 3, 1> Vector3T;

public:
    HexDomain() : Base(sqrt(T(8)))
    {}

    bool validate_orbit(const VectorOrb& orb) const
    { return orb(0) <= 1; }

private:
    friend class BaseDomain<HexDomain<T>, T, 3, 7>;

    int npts_for_orbit(int i) const;

    int narg_for_orbit(int i) const;

    int nbfn_for_qdeg(int qdeg) const;

    void expand_orbit(int i, int aoff, int poff,
                      const VectorXT& args,
                      MatrixPtsT& pts) const;

    void seed_orbit(int i, int aoff, VectorXT& args);

    template<typename D1, typename D2>
    void eval_orthob_block(const D1 pqr, D2 out) const;

    void clamp_arg(int i, int aoff, VectorXT& args) const;
};

template<typename T>
inline int
HexDomain<T>::npts_for_orbit(int i) const
{
    switch (i)
    {
        case 0:
            return 1;
        case 1:
            return 6;
        case 2:
            return 8;
        case 3:
            return 12;
        case 4:
        case 5:
            return 24;
        case 6:
            return 48;
        default:
            assert(0 && "Bad orbit"), abort();
    }
}

template<typename T>
inline int
HexDomain<T>::narg_for_orbit(int i) const
{
    switch (i)
    {
        case 0:
            return 0;
        case 1:
        case 2:
        case 3:
            return 1;
        case 4:
        case 5:
            return 2;
        case 6:
            return 3;
        default:
            assert(0 && "Bad orbit"), abort();
    }
}

template<typename T>
inline int
HexDomain<T>::nbfn_for_qdeg(int qdeg) const
{
    int n = 0;

    for (int i = 0; i <= qdeg; i += 2)
        for (int j = i; j <= qdeg - i; j += 2)
            for (int k = j; k <= qdeg - i - j; k += 2, ++n);

    return n;
}

template<typename T>
EIGEN_ALWAYS_INLINE void
HexDomain<T>::expand_orbit(int i, int aoff, int poff,
                            const VectorXT& args, MatrixPtsT& pts) const
{
    switch (i)
    {
        case 0:
            pts.row(poff) = Vector3T(0, 0, 0);
            break;
        case 1:
        {
            const T& a = args(aoff);
            pts.row(poff + 0) = Vector3T(-a, 0, 0);
            pts.row(poff + 1) = Vector3T(0, 0, a);
            pts.row(poff + 2) = Vector3T(0, a, 0);
            pts.row(poff + 3) = Vector3T(0, 0, -a);
            pts.row(poff + 4) = Vector3T(a, 0, 0);
            pts.row(poff + 5) = Vector3T(0, -a, 0);
            break;
        }
        case 2:
        {
            const T& a = args(aoff);
            pts.row(poff + 0) = Vector3T(a, -a, -a);
            pts.row(poff + 1) = Vector3T(-a, a, a);
            pts.row(poff + 2) = Vector3T(-a, a, -a);
            pts.row(poff + 3) = Vector3T(-a, -a, -a);
            pts.row(poff + 4) = Vector3T(-a, -a, a);
            pts.row(poff + 5) = Vector3T(a, a, -a);
            pts.row(poff + 6) = Vector3T(a, a, a);
            pts.row(poff + 7) = Vector3T(a, -a, a);
            break;
        }
        case 3:
        {
            const T& a = args(aoff);
            pts.row(poff + 0)  = Vector3T(-a, -a, 0);
            pts.row(poff + 1)  = Vector3T(a, 0, -a);
            pts.row(poff + 2)  = Vector3T(0, a, -a);
            pts.row(poff + 3)  = Vector3T(a, a, 0);
            pts.row(poff + 4)  = Vector3T(a, 0, a);
            pts.row(poff + 5)  = Vector3T(0, -a, a);
            pts.row(poff + 6)  = Vector3T(0, -a, -a);
            pts.row(poff + 7)  = Vector3T(-a, 0, a);
            pts.row(poff + 8)  = Vector3T(-a, a, 0);
            pts.row(poff + 9)  = Vector3T(a, -a, 0);
            pts.row(poff + 10) = Vector3T(0, a, a);
            pts.row(poff + 11) = Vector3T(-a, 0, -a);
            break;
        }
        case 4:
        {
            const T& a = args(aoff + 0);
            const T& b = args(aoff + 1);
            pts.row(poff + 0)  = Vector3T(0, a, -b);
            pts.row(poff + 1)  = Vector3T(-a, 0, -b);
            pts.row(poff + 2)  = Vector3T(0, b, a);
            pts.row(poff + 3)  = Vector3T(b, -a, 0);
            pts.row(poff + 4)  = Vector3T(0, a, b);
            pts.row(poff + 5)  = Vector3T(0, -a, b);
            pts.row(poff + 6)  = Vector3T(a, 0, -b);
            pts.row(poff + 7)  = Vector3T(0, -a, -b);
            pts.row(poff + 8)  = Vector3T(-b, 0, a);
            pts.row(poff + 9)  = Vector3T(-b, -a, 0);
            pts.row(poff + 10) = Vector3T(a, b, 0);
            pts.row(poff + 11) = Vector3T(b, 0, a);
            pts.row(poff + 12) = Vector3T(0, b, -a);
            pts.row(poff + 13) = Vector3T(b, 0, -a);
            pts.row(poff + 14) = Vector3T(0, -b, -a);
            pts.row(poff + 15) = Vector3T(a, -b, 0);
            pts.row(poff + 16) = Vector3T(-a, b, 0);
            pts.row(poff + 17) = Vector3T(a, 0, b);
            pts.row(poff + 18) = Vector3T(-b, a, 0);
            pts.row(poff + 19) = Vector3T(-b, 0, -a);
            pts.row(poff + 20) = Vector3T(b, a, 0);
            pts.row(poff + 21) = Vector3T(-a, 0, b);
            pts.row(poff + 22) = Vector3T(0, -b, a);
            pts.row(poff + 23) = Vector3T(-a, -b, 0);
            break;
        }
        case 5:
        {
            const T& a = args(aoff + 0);
            const T& b = args(aoff + 1);
            pts.row(poff + 0)  = Vector3T(a, b, -a);
            pts.row(poff + 1)  = Vector3T(-a, -b, a);
            pts.row(poff + 2)  = Vector3T(-a, b, -a);
            pts.row(poff + 3)  = Vector3T(-b, -a, a);
            pts.row(poff + 4)  = Vector3T(-a, -a, b);
            pts.row(poff + 5)  = Vector3T(b, a, -a);
            pts.row(poff + 6)  = Vector3T(a, -b, -a);
            pts.row(poff + 7)  = Vector3T(a, -b, a);
            pts.row(poff + 8)  = Vector3T(-a, -b, -a);
            pts.row(poff + 9)  = Vector3T(a, a, -b);
            pts.row(poff + 10) = Vector3T(-b, a, -a);
            pts.row(poff + 11) = Vector3T(-a, a, -b);
            pts.row(poff + 12) = Vector3T(-a, a, b);
            pts.row(poff + 13) = Vector3T(-a, -a, -b);
            pts.row(poff + 14) = Vector3T(a, -a, b);
            pts.row(poff + 15) = Vector3T(b, -a, -a);
            pts.row(poff + 16) = Vector3T(a, b, a);
            pts.row(poff + 17) = Vector3T(b, a, a);
            pts.row(poff + 18) = Vector3T(b, -a, a);
            pts.row(poff + 19) = Vector3T(-b, a, a);
            pts.row(poff + 20) = Vector3T(a, a, b);
            pts.row(poff + 21) = Vector3T(-a, b, a);
            pts.row(poff + 22) = Vector3T(a, -a, -b);
            pts.row(poff + 23) = Vector3T(-b, -a, -a);
            break;
        }
        case 6:
        {
            const T& a = args(aoff + 0);
            const T& b = args(aoff + 1);
            const T& c = args(aoff + 2);
            pts.row(poff + 0)  = Vector3T(-b, -c, a);
            pts.row(poff + 1)  = Vector3T(-b, a, -c);
            pts.row(poff + 2)  = Vector3T(-a, -c, -b);
            pts.row(poff + 3)  = Vector3T(a, c, b);
            pts.row(poff + 4)  = Vector3T(-c, b, -a);
            pts.row(poff + 5)  = Vector3T(c, -b, a);
            pts.row(poff + 6)  = Vector3T(c, b, a);
            pts.row(poff + 7)  = Vector3T(-b, c, a);
            pts.row(poff + 8)  = Vector3T(c, a, -b);
            pts.row(poff + 9)  = Vector3T(b, c, -a);
            pts.row(poff + 10) = Vector3T(-b, -a, -c);
            pts.row(poff + 11) = Vector3T(-c, -a, -b);
            pts.row(poff + 12) = Vector3T(a, -c, b);
            pts.row(poff + 13) = Vector3T(c, a, b);
            pts.row(poff + 14) = Vector3T(-a, -b, c);
            pts.row(poff + 15) = Vector3T(-a, c, b);
            pts.row(poff + 16) = Vector3T(b, -a, c);
            pts.row(poff + 17) = Vector3T(b, a, c);
            pts.row(poff + 18) = Vector3T(-c, -b, -a);
            pts.row(poff + 19) = Vector3T(-a, b, -c);
            pts.row(poff + 20) = Vector3T(-a, c, -b);
            pts.row(poff + 21) = Vector3T(c, b, -a);
            pts.row(poff + 22) = Vector3T(a, -c, -b);
            pts.row(poff + 23) = Vector3T(c, -a, b);
            pts.row(poff + 24) = Vector3T(-a, -c, b);
            pts.row(poff + 25) = Vector3T(-b, -c, -a);
            pts.row(poff + 26) = Vector3T(-b, c, -a);
            pts.row(poff + 27) = Vector3T(c, -b, -a);
            pts.row(poff + 28) = Vector3T(-c, -b, a);
            pts.row(poff + 29) = Vector3T(-b, a, c);
            pts.row(poff + 30) = Vector3T(c, -a, -b);
            pts.row(poff + 31) = Vector3T(a, b, -c);
            pts.row(poff + 32) = Vector3T(-a, b, c);
            pts.row(poff + 33) = Vector3T(a, -b, -c);
            pts.row(poff + 34) = Vector3T(b, a, -c);
            pts.row(poff + 35) = Vector3T(b, -a, -c);
            pts.row(poff + 36) = Vector3T(a, -b, c);
            pts.row(poff + 37) = Vector3T(-c, a, -b);
            pts.row(poff + 38) = Vector3T(-c, b, a);
            pts.row(poff + 39) = Vector3T(-a, -b, -c);
            pts.row(poff + 40) = Vector3T(b, -c, a);
            pts.row(poff + 41) = Vector3T(b, c, a);
            pts.row(poff + 42) = Vector3T(a, b, c);
            pts.row(poff + 43) = Vector3T(-c, -a, b);
            pts.row(poff + 44) = Vector3T(-b, -a, c);
            pts.row(poff + 45) = Vector3T(-c, a, b);
            pts.row(poff + 46) = Vector3T(b, -c, -a);
            pts.row(poff + 47) = Vector3T(a, c, -b);
            break;
        }
        default:
            assert(0 && "Bad orbit"), abort();
    }
}

template<typename T>
inline void
HexDomain<T>::seed_orbit(int i, int aoff, VectorXT& args)
{
    switch (i)
    {
        case 0:
            break;
        case 1:
        case 2:
        case 3:
            args(aoff) = this->rand(0.0, 1.0);
            break;
        case 4:
        case 5:
            args(aoff + 0) = this->rand(0.0, 1.0);
            args(aoff + 1) = this->rand(0.0, 1.0);
            break;
        case 6:
            args(aoff + 0) = this->rand(0.0, 1.0);
            args(aoff + 1) = this->rand(0.0, 1.0);
            args(aoff + 2) = this->rand(0.0, 1.0);
            break;
        default:
            assert(0 && "Bad orbit"), abort();
    }
}

template<typename T>
template<typename D1, typename D2>
inline void
HexDomain<T>::eval_orthob_block(const D1 pqr, D2 out) const
{
    typedef Eigen::Array<T, D1::RowsAtCompileTime, 1> ArrayT;

    const auto& p = pqr.col(0);
    const auto& q = pqr.col(1);
    const auto& r = pqr.col(2);

    const T half = 0.5;

    for (int i = 0, off = 0; i <= this->qdeg(); i += 2)
    {
        for (int j = i; j <= this->qdeg() - i; j += 2)
        {
            for (int k = j; k <= this->qdeg() - i - j; k += 2, ++off)
            {
                T cijk = sqrt((i + half)*(j + half)*(k + half));

                out.row(off) = cijk
                             * jacobi_poly(i, 0, 0, p)
                             * jacobi_poly(j, 0, 0, q)
                             * jacobi_poly(k, 0, 0, r);
            }
        }
    }
}

template<typename T>
inline void
HexDomain<T>::clamp_arg(int i, int aoff, VectorXT& args) const
{
    switch (i)
    {
        case 0:
            break;
        case 1:
        case 2:
        case 3:
            args(aoff) = clamp(0, args(aoff), 1);
            break;
        case 4:
        case 5:
            args(aoff + 0) = clamp(0, args(aoff + 0), 1);
            args(aoff + 1) = clamp(0, args(aoff + 1), 1);
            break;
        case 6:
            args(aoff + 0) = clamp(0, args(aoff + 0), 1);
            args(aoff + 1) = clamp(0, args(aoff + 1), 1);
            args(aoff + 2) = clamp(0, args(aoff + 2), 1);
            break;
        default:
            assert(0 && "Bad orbit"), abort();
    }
}

}

#endif /* POLYQUAD_HEX_HPP_ */
