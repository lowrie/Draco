//----------------------------------*-C++-*----------------------------------//
/*!
 * \file   c4/C4_MPI_free_inherited_comm.cc
 * \author Thomas M. Evans
 * \date   Thu Mar 21 16:56:17 2002
 * \brief  C4 MPI implementation.
 * \note   Copyright (C) 2016-2017 Los Alamos National Security, LLC.
 *         All rights reserved.
 */
//---------------------------------------------------------------------------//
// $Id: C4_MPI_free_inherited_comm.cc 7388 2015-01-22 16:02:07Z kellyt $
//---------------------------------------------------------------------------//

#include "c4/config.h"
#include <vector>

#ifdef C4_MPI

#include "C4_MPI.hh"

namespace rtt_c4 {
//---------------------------------------------------------------------------//
// MPI COMMUNICATOR
//---------------------------------------------------------------------------//

extern MPI_Comm communicator;

//---------------------------------------------------------------------------//

void free_inherited_comm() {
  if (communicator != MPI_COMM_WORLD) {
    MPI_Comm_free(&communicator);
    communicator = MPI_COMM_WORLD;
  }
  return;
}

} // end namespace rtt_c4

#endif // C4_MPI

//---------------------------------------------------------------------------//
// end of C4_MPI_free_inherited_comm.cc
//---------------------------------------------------------------------------//
