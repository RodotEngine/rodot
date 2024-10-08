// Copyright 2009-2021 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "../common/scene.h"
#include "priminfo.h"
#include "priminfo_mb.h"
#include "bvh_builder_morton.h"

namespace embree
{ 
  namespace isa
  {
    PrimInfo createPrimRefArray(Geometry* geometry, unsigned int geomID, size_t numPrimitives, mHector<PrimRef>& prims, BuildProgressMonitor& progressMonitor);
   
    PrimInfo createPrimRefArray(Scene* scene, Geometry::GTypeMask types, bool mblur, size_t numPrimitives, mHector<PrimRef>& prims, BuildProgressMonitor& progressMonitor);

    PrimInfo createPrimRefArray(Scene* scene, Geometry::GTypeMask types, bool mblur, size_t numPrimitives, mHector<PrimRef>& prims, mHector<SubGridBuildData>& sgrids, BuildProgressMonitor& progressMonitor);
   
    PrimInfo createPrimRefArrayMBlur(Scene* scene, Geometry::GTypeMask types, size_t numPrimitives, mHector<PrimRef>& prims, BuildProgressMonitor& progressMonitor, size_t itime = 0);

    PrimInfoMB createPrimRefArrayMSMBlur(Scene* scene, Geometry::GTypeMask types, size_t numPrimitives, mHector<PrimRefMB>& prims, BuildProgressMonitor& progressMonitor, BBox1f t0t1 = BBox1f(0.0f,1.0f));

    PrimInfoMB createPrimRefArrayMSMBlur(Scene* scene, Geometry::GTypeMask types, size_t numPrimitives, mHector<PrimRefMB>& prims, mHector<SubGridBuildData>& sgrids, BuildProgressMonitor& progressMonitor, BBox1f t0t1 = BBox1f(0.0f,1.0f));

    template<typename Mesh>
      size_t createMortonCodeArray(Mesh* mesh, mHector<BVHBuilderMorton::BuildPrim>& morton, BuildProgressMonitor& progressMonitor);

    /* special variants for grids */
    PrimInfo createPrimRefArrayGrids(Scene* scene, mHector<PrimRef>& prims, mHector<SubGridBuildData>& sgrids); // FIXME: remove

    PrimInfo createPrimRefArrayGrids(GridMesh* mesh, mHector<PrimRef>& prims, mHector<SubGridBuildData>& sgrids);

    PrimInfoMB createPrimRefArrayMSMBlurGrid(Scene* scene, mHector<PrimRefMB>& prims, mHector<SubGridBuildData>& sgrids, BuildProgressMonitor& progressMonitor, BBox1f t0t1 = BBox1f(0.0f,1.0f));
  }
}
