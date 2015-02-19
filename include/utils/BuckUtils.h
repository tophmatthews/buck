#ifndef BUCKUTILS_H
#define BUCKUTILS_H

#include "Moose.h"
#include "MooseTypes.h"
#include "Conversion.h"

namespace Buck
{
  // template <typename T, typename T2>
  // inline void atomsFromN(std::vector<T> &atoms, const T2 N, const int N_min=1)
  // {
  //   for ( int i=N_min; i<N+1; ++i )
  //     atoms.push_back(i);
  // }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////

  template <typename T>
  inline void iterateAndDisplay(std::string name, std::vector<T> &show)
  {
    // Iterates through vector of values and dispalys them
    std::cout << std::endl;
    for ( int i=0; i<show.size(); ++i)
      std::cout << name << "[" << i << "]: " << show[i] << std::endl;
    std::cout << std::endl;
  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////
}

#endif //BUCKUTILS_H
