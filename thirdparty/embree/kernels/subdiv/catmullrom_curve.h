// Copyright 2009-2021 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "../common/default.h"
#include "bezier_curve.h"
#include "../common/scene_curves.h"

/*

  Implements Catmul Rom curves with control points p0, p1, p2, p3. At
  t=0 the curve goes through p1, with tangent (p2-p0)/2, and for t=1
  the curve goes through p2 with tangent (p3-p2)/2.

 */

namespace embree
{
  class CatmullRomBasis
  {
  public:

    template<typename T>
      static __forceinline Vec4<T> eval(const T& u) 
    {
      const T t  = u;
      const T s  = T(1.0f) - u;
      const T n0 = - t * s * s;
      const T n1 = 2.0f + t * t * (3.0f * t - 5.0f);
      const T n2 = 2.0f + s * s * (3.0f * s - 5.0f);
      const T n3 = - s * t * t;
      return T(0.5f) * Vec4<T>(n0, n1, n2, n3);
    }
    
    template<typename T>
      static __forceinline Vec4<T>  derivative(const T& u)
    {
      const T t  =  u;
      const T s  =  1.0f - u;
      const T n0 =  - s * s + 2.0f * s * t;
      const T n1 =  2.0f * t * (3.0f * t - 5.0f) + 3.0f * t * t;
      const T n2 =  2.0f * s * (3.0f * t + 2.0f) - 3.0f * s * s;
      const T n3 = -2.0f * s * t + t * t;
      return T(0.5f) * Vec4<T>(n0, n1, n2, n3);
    }

    template<typename T>
      static __forceinline Vec4<T>  derivative2(const T& u)
    {
      const T t  =  u;
      const T n0 = -3.0f * t + 2.0f;
      const T n1 =  9.0f * t - 5.0f;
      const T n2 = -9.0f * t + 4.0f;
      const T n3 =  3.0f * t - 1.0f;
      return Vec4<T>(n0, n1, n2, n3);
    }
  };
  
  struct PrecomputedCatmullRomBasis
  {
    enum { N = 16 };
  public:
    PrecomputedCatmullRomBasis() {}
    PrecomputedCatmullRomBasis(int shift);

    /* basis for bspline evaluation */
  public:
    float c0[N+1][N+1];
    float c1[N+1][N+1];
    float c2[N+1][N+1];
    float c3[N+1][N+1];
    
    /* basis for bspline derivative evaluation */
  public:
    float d0[N+1][N+1];
    float d1[N+1][N+1];
    float d2[N+1][N+1];
    float d3[N+1][N+1];
  };
  extern PrecomputedCatmullRomBasis catmullrom_basis0;
  extern PrecomputedCatmullRomBasis catmullrom_basis1;

  template<typename Vertex>
    struct CatmullRomCurveT
    {
      Vertex v0,v1,v2,v3;
      
      __forceinline CatmullRomCurveT() {}
      
      __forceinline CatmullRomCurveT(const Vertex& v0, const Vertex& v1, const Vertex& v2, const Vertex& v3)
        : v0(v0), v1(v1), v2(v2), v3(v3) {}

      __forceinline Vertex begin() const {
        return v1;
      }

      __forceinline Vertex end() const {
        return v2;
      }

      __forceinline Vertex center() const {
        return 0.5f*(v0+v1);
      }

      __forceinline BBox<Vertex> bounds() const {
        return merge(BBox<Vertex>(v0),BBox<Vertex>(v1),BBox<Vertex>(v2),BBox<Vertex>(v3));
      }

      __forceinline friend CatmullRomCurveT operator -( const CatmullRomCurveT& a, const Vertex& b ) {
        return CatmullRomCurveT(a.v0-b,a.v1-b,a.v2-b,a.v3-b);
      }

      __forceinline CatmullRomCurveT<Vec3ff> xfm_pr(const LinearSpace3fa& space, const Vec3fa& p) const
      {
        const Vec3ff q0(xfmHector(space,v0-p), v0.w);
        const Vec3ff q1(xfmHector(space,v1-p), v1.w);
        const Vec3ff q2(xfmHector(space,v2-p), v2.w);
        const Vec3ff q3(xfmHector(space,v3-p), v3.w);
        return CatmullRomCurveT<Vec3ff>(q0,q1,q2,q3);
      }
      
      __forceinline Vertex eval(const float t) const 
      {
        const Vec4<float> b = CatmullRomBasis::eval(t);
        return madd(b.x,v0,madd(b.y,v1,madd(b.z,v2,b.w*v3)));
      }
      
      __forceinline Vertex eval_du(const float t) const
      {
        const Vec4<float> b = CatmullRomBasis::derivative(t);
        return madd(b.x,v0,madd(b.y,v1,madd(b.z,v2,b.w*v3)));
      }
      
      __forceinline Vertex eval_dudu(const float t) const 
      {
        const Vec4<float> b = CatmullRomBasis::derivative2(t);
        return madd(b.x,v0,madd(b.y,v1,madd(b.z,v2,b.w*v3)));
      }

      __forceinline void eval(const float t, Vertex& p, Vertex& dp) const
      {
        p = eval(t);
        dp = eval_du(t);
      }
      
      __forceinline void eval(const float t, Vertex& p, Vertex& dp, Vertex& ddp) const
      {
        p = eval(t);
        dp = eval_du(t);
        ddp = eval_dudu(t);
      }

      template<int M>
      __forceinline Vec4vf<M> veval(const vfloat<M>& t) const 
      {
        const Vec4vf<M> b = CatmullRomBasis::eval(t);
        return madd(b.x, Vec4vf<M>(v0), madd(b.y, Vec4vf<M>(v1), madd(b.z, Vec4vf<M>(v2), b.w * Vec4vf<M>(v3))));
      }

      template<int M>
      __forceinline Vec4vf<M> veval_du(const vfloat<M>& t) const 
      {
        const Vec4vf<M> b = CatmullRomBasis::derivative(t);
        return madd(b.x, Vec4vf<M>(v0), madd(b.y, Vec4vf<M>(v1), madd(b.z, Vec4vf<M>(v2), b.w * Vec4vf<M>(v3))));
      }

      template<int M>
      __forceinline Vec4vf<M> veval_dudu(const vfloat<M>& t) const 
      {
        const Vec4vf<M> b = CatmullRomBasis::derivative2(t);
        return madd(b.x, Vec4vf<M>(v0), madd(b.y, Vec4vf<M>(v1), madd(b.z, Vec4vf<M>(v2), b.w * Vec4vf<M>(v3))));
      }

      template<int M>
      __forceinline void veval(const vfloat<M>& t, Vec4vf<M>& p, Vec4vf<M>& dp) const
      {
        p = veval<M>(t);
        dp = veval_du<M>(t);
      }
      
      template<int M>
      __forceinline Vec4vf<M> eval0(const int ofs, const int size) const
      {
        assert(size <= PrecomputedCatmullRomBasis::N);
        assert(ofs <= size);
        return madd(vfloat<M>::loadu(&catmullrom_basis0.c0[size][ofs]), Vec4vf<M>(v0),
                    madd(vfloat<M>::loadu(&catmullrom_basis0.c1[size][ofs]), Vec4vf<M>(v1),
                         madd(vfloat<M>::loadu(&catmullrom_basis0.c2[size][ofs]), Vec4vf<M>(v2),
                              vfloat<M>::loadu(&catmullrom_basis0.c3[size][ofs]) * Vec4vf<M>(v3))));
      }
      
      template<int M>
      __forceinline Vec4vf<M> eval1(const int ofs, const int size) const
      {
        assert(size <= PrecomputedCatmullRomBasis::N);
        assert(ofs <= size);
        return madd(vfloat<M>::loadu(&catmullrom_basis1.c0[size][ofs]), Vec4vf<M>(v0), 
                    madd(vfloat<M>::loadu(&catmullrom_basis1.c1[size][ofs]), Vec4vf<M>(v1),
                         madd(vfloat<M>::loadu(&catmullrom_basis1.c2[size][ofs]), Vec4vf<M>(v2),
                              vfloat<M>::loadu(&catmullrom_basis1.c3[size][ofs]) * Vec4vf<M>(v3))));
      }
      
      template<int M>
      __forceinline Vec4vf<M> derivative0(const int ofs, const int size) const
      {
        assert(size <= PrecomputedCatmullRomBasis::N);
        assert(ofs <= size);
        return madd(vfloat<M>::loadu(&catmullrom_basis0.d0[size][ofs]), Vec4vf<M>(v0),
                    madd(vfloat<M>::loadu(&catmullrom_basis0.d1[size][ofs]), Vec4vf<M>(v1),
                         madd(vfloat<M>::loadu(&catmullrom_basis0.d2[size][ofs]), Vec4vf<M>(v2),
                              vfloat<M>::loadu(&catmullrom_basis0.d3[size][ofs]) * Vec4vf<M>(v3))));
      }
      
      template<int M>
      __forceinline Vec4vf<M> derivative1(const int ofs, const int size) const
      {
        assert(size <= PrecomputedCatmullRomBasis::N);
        assert(ofs <= size);
        return madd(vfloat<M>::loadu(&catmullrom_basis1.d0[size][ofs]), Vec4vf<M>(v0),
                    madd(vfloat<M>::loadu(&catmullrom_basis1.d1[size][ofs]), Vec4vf<M>(v1),
                         madd(vfloat<M>::loadu(&catmullrom_basis1.d2[size][ofs]), Vec4vf<M>(v2),
                              vfloat<M>::loadu(&catmullrom_basis1.d3[size][ofs]) * Vec4vf<M>(v3))));
      }
      
      /* calculates bounds of catmull-rom curve geometry */
      __forceinline BBox3fa accurateRoundBounds() const
      {
        const int N = 7;
        const float scale = 1.0f/(3.0f*(N-1));
        Vec4vfx pl(pos_inf), pu(neg_inf);
        for (int i=0; i<=N; i+=VSIZEX)
        {
          vintx vi = vintx(i)+vintx(step);
          vboolx valid = vi <= vintx(N);
          const Vec4vfx p  = eval0<VSIZEX>(i,N);
          const Vec4vfx dp = derivative0<VSIZEX>(i,N);
          const Vec4vfx pm = p-Vec4vfx(scale)*select(vi!=vintx(0),dp,Vec4vfx(zero));
          const Vec4vfx pp = p+Vec4vfx(scale)*select(vi!=vintx(N),dp,Vec4vfx(zero));
          pl = select(valid,min(pl,p,pm,pp),pl); // FIXME: use masked min
          pu = select(valid,max(pu,p,pm,pp),pu); // FIXME: use masked min
        }
        const Vec3fa lower(reduce_min(pl.x),reduce_min(pl.y),reduce_min(pl.z));
        const Vec3fa upper(reduce_max(pu.x),reduce_max(pu.y),reduce_max(pu.z));
        const float r_min = reduce_min(pl.w);
        const float r_max = reduce_max(pu.w);
        const Vec3fa upper_r = Vec3fa(max(abs(r_min),abs(r_max)));
        return enlarge(BBox3fa(lower,upper),upper_r);
      }
      
      /* calculates bounds when tessellated into N line segments */
      __forceinline BBox3fa accurateFlatBounds(int N) const
      {
        if (likely(N == 4))
        {
          const Vec4vf4 pi = eval0<4>(0,4);
          const Vec3fa lower(reduce_min(pi.x),reduce_min(pi.y),reduce_min(pi.z));
          const Vec3fa upper(reduce_max(pi.x),reduce_max(pi.y),reduce_max(pi.z));
          const Vec3fa upper_r = Vec3fa(reduce_max(abs(pi.w)));
          const Vec3ff pe = end();
          return enlarge(BBox3fa(min(lower,pe),max(upper,pe)),max(upper_r,Vec3fa(abs(pe.w))));
        } 
        else
        {
          Vec3vfx pl(pos_inf), pu(neg_inf); vfloatx ru(0.0f);
          for (int i=0; i<=N; i+=VSIZEX)
          {
            vboolx valid = vintx(i)+vintx(step) <= vintx(N);
            const Vec4vfx pi = eval0<VSIZEX>(i,N);
            
            pl.x = select(valid,min(pl.x,pi.x),pl.x); // FIXME: use masked min
            pl.y = select(valid,min(pl.y,pi.y),pl.y); 
            pl.z = select(valid,min(pl.z,pi.z),pl.z); 
            
            pu.x = select(valid,max(pu.x,pi.x),pu.x); // FIXME: use masked min
            pu.y = select(valid,max(pu.y,pi.y),pu.y); 
            pu.z = select(valid,max(pu.z,pi.z),pu.z); 
            
            ru = select(valid,max(ru,abs(pi.w)),ru); 
          }
          const Vec3fa lower(reduce_min(pl.x),reduce_min(pl.y),reduce_min(pl.z));
          const Vec3fa upper(reduce_max(pu.x),reduce_max(pu.y),reduce_max(pu.z));
          const Vec3fa upper_r(reduce_max(ru));
          return enlarge(BBox3fa(lower,upper),upper_r);
        }
      }
      
      friend __forceinline embree_ostream operator<<(embree_ostream cout, const CatmullRomCurveT& curve) {
        return cout << "CatmullRomCurve { v0 = " << curve.v0 << ", v1 = " << curve.v1 << ", v2 = " << curve.v2 << ", v3 = " << curve.v3 << " }";
      }
    };

  template<typename Vertex>
    __forceinline void convert(const CatmullRomCurveT<Vertex>& icurve, BezierCurveT<Vertex>& ocurve)
  {
    const Vertex v0 = icurve.v1;
    const Vertex v1 = icurve.v1+(icurve.v2-icurve.v0)*(1.0f/6.0f);
    const Vertex v2 = icurve.v2+(icurve.v1-icurve.v3)*(1.0f/6.0f);
    const Vertex v3 = icurve.v2;
    ocurve = BezierCurveT<Vertex>(v0,v1,v2,v3);
  }

  template<typename CurveGeometry>
  __forceinline CatmullRomCurveT<Vec3ff> enlargeRadiusToMinWidth(const RayQueryContext* context, const CurveGeometry* geom, const Vec3fa& ray_org, const CatmullRomCurveT<Vec3ff>& curve)
  {
    return CatmullRomCurveT<Vec3ff>(enlargeRadiusToMinWidth(context,geom,ray_org,curve.v0),
                                    enlargeRadiusToMinWidth(context,geom,ray_org,curve.v1),
                                    enlargeRadiusToMinWidth(context,geom,ray_org,curve.v2),
                                    enlargeRadiusToMinWidth(context,geom,ray_org,curve.v3));
  }
  
  typedef CatmullRomCurveT<Vec3fa> CatmullRomCurve3fa;
}

