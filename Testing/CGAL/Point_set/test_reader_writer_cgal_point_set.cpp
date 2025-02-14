// Copyright (c) 2012-2019 University of Lyon and CNRS (France).
// All rights reserved.
//
// This file is part of MEPP2; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published 
// by the Free Software Foundation; either version 3 of the License, 
// or (at your option) any later version.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#include "FEVV/DataStructures/DataStructures_cgal_point_set.h"
//#include "FEVV/Wrappings/Graph_traits_cgal_point_set.h"
#include "FEVV/Wrappings/Geometry_traits_cgal_point_set.h"
#include "FEVV/Wrappings/Graph_properties_cgal_point_set.h"
#include "FEVV/Wrappings/properties_cgal_point_set.h"
#include "FEVV/Wrappings/Graph_traits_extension_cgal_point_set.h"

#include "FEVV/Filters/CGAL/Point_set/cgal_point_set_reader.hpp"
#include "FEVV/Filters/CGAL/Point_set/cgal_point_set_writer.hpp"

#include "FEVV/Tools/IO/FileUtilities.hpp"
#include "Testing/Utils/utils_are_meshes_identical.hpp"
#include "Testing/Utils/utils_identical_text_based_files.hpp"

#include <iostream>
#include <string>


// main
int main(int argc, char *argv[])
{
  // parse arguments
  if(argc != 4)
  {
    std::cout << "Usage:  " << argv[0]
              << "  input_mesh_filename  output_mesh_filename  reference_mesh_filename" << std::endl;
    std::cout << "Example:  " << argv[0] << "  ../Testing/Data/tetra.xyz  tetra.out.ply  tetra.out.ref.ply"
              << std::endl;
    return EXIT_FAILURE;
  }
  std::string input_file = argv[1];
  std::string output_file = argv[2];
  std::string reference_file = argv[3];

  //----------------------------------

  // create point cloud
  typedef  FEVV::CGALPointSet  PointCloudT;
  PointCloudT pc;

  // load point cloud
  std::cout << "Reading file " << input_file << "..." << std::endl;
  FEVV::PMapsContainer pmaps_bag;
  FEVV::Filters::read_mesh(input_file, pc, pmaps_bag);

  std::cout << FEVV::size_of_vertices(pc) << " points were successfully read." << std::endl;

  //----------------------------------

  // save point cloud
  std::cout << "Saving to file " << output_file << "..." << std::endl;
  FEVV::Filters::write_mesh(output_file, pc, pmaps_bag);

  //----------------------------------

  // check output file
  std::cout << "Comparing output file '" << output_file
            << "' with reference file '" << reference_file << "'..."
            << std::endl;

  if(FEVV::FileUtils::has_extension(output_file, ".off") ||
     FEVV::FileUtils::has_extension(output_file, ".coff"))
  {
    // use OFF file comparator
    if(!are_meshes_equal(output_file, reference_file, false))
    {
      std::cout << "Files are different!" << std::endl;
      return EXIT_FAILURE;
    }
  }
  else
  {
    // use text file comparator
    if(!identical_text_based_files(output_file, reference_file))
    {
      std::cout << "Files are different!" << std::endl;
      return EXIT_FAILURE;
    }
  }

  std::cout << "Files are identical." << std::endl;
  
  return 0;
}
