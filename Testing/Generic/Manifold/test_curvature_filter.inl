// Copyright (c) 2012-2019 University of Lyon and CNRS (France).
// All rights reserved.
//
// This file is part of MEPP2; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 3 of
// the License, or (at your option) any later version.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
#include "FEVV/Filters/Generic/Manifold/Curvature/curvature.hpp"
    // for calculate_curvature()
#include "FEVV/Filters/Generic/calculate_face_normals.hpp"
    // for calculate_face_normals
#include "FEVV/Filters/Generic/generic_reader.hpp" // for read_mesh()


// code from Visualization/PluginFilters/curvature/CurvaturePlugin.h
template< typename HalfedgeGraph >
void
curvature(HalfedgeGraph *mesh)
{
  std::cout << "Asking to Curvature mesh ! " << std::endl;

  auto pm = get(boost::vertex_point, *mesh);

  // ---

  //using Vector = typename FEVV::Geometry_traits< HalfedgeGraph >::Vector;

  // Face normal map
  // this one is a standard property map
  auto f_nm = make_property_map(FEVV::face_normal, *mesh);

  FEVV::Filters::calculate_face_normals(*mesh, pm, f_nm);

  // ---

  // Vertex curvature map
  // this one is a NON-standard property map!
  auto v_cm =
      FEVV::make_vertex_property_map< HalfedgeGraph,
                                      FEVV::Filters::v_Curv< HalfedgeGraph > >(
          *mesh);

  /*! \brief Minimum and maximum values of the minimum and maximum curvature
   * fields (usefull for color rendering)*/
  double min_nrm_min_curvature, max_nrm_min_curvature, min_nrm_max_curvature,
      max_nrm_max_curvature;

  bool value_is_geod = true; //=false
  double value_radius = 0.003; //=0.001
  FEVV::Filters::calculate_curvature( // B) call the filter corresponding to
                                      // your operation
      *mesh,
      v_cm,
      pm,
      f_nm,
      value_is_geod,
      value_radius,
      min_nrm_min_curvature,
      max_nrm_min_curvature,
      min_nrm_max_curvature,
      max_nrm_max_curvature);

  // ---

  std::cout << "Curvature mesh, isGeod: " << value_is_geod
            << " - radius: " << value_radius << "." << std::endl;
}

//------------------------------------------------------------------------------

template< typename MeshT >
int
test_curvature_filter(int narg, const char **argv)
{
  if(narg < 2)
  {
    std::cout << "Usage: " << argv[0] << " mesh_filename" << std::endl;
    exit(EXIT_FAILURE);
  }
  std::string filename(argv[1]);

  MeshT m;
  FEVV::PMapsContainer pmaps_bag;
  FEVV::Filters::read_mesh(filename, m, pmaps_bag);

  curvature(&m);

  std::cout << "Done." << std::endl;

  return 0;
}
