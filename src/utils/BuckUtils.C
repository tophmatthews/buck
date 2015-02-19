#include "BuckUtils.h"
#include "mooseError.h"

namespace Buck {

	void avgSizeFromGroup(std::vector<Real> & avgs, const Real G, const Real N_max, const int N_nuc, const bool log)
  {
    // Determines maximum cluster size for a given group.
    // G = Max number of groups
    // N_max = largest cluster size
    // N_nuc = largest cluster size from nucleation model

    // First setup nucleation clusters, which consist of N_nuc groups with one atom each
    
    std::vector<Real> sizes;
    
    for ( int g=1; g<N_nuc+1; ++g )
      sizes.push_back(g);

    Real m; // slope
    Real b; // intercept
    if ( !log )
    {
      // Linear separation between max atoms and group number
      b = N_nuc;
      m = (N_max - N_nuc) / (G - N_nuc);

      for ( int g=N_nuc; g<G+1; ++g )
        sizes.push_back( b + m * (g-N_nuc+1) );
    }
    else
    {
      // roughly log separation
      b = N_nuc - 1;
      m = std::log10(N_max - N_nuc +1) / (G - N_nuc);

      for ( int g=N_nuc; g<G+1; ++g )
        sizes.push_back( b+ std::pow( 10, m * (g-N_nuc+1) ) );
    }

	  avgs.push_back(1.0);
	  for ( int g=0; g<N_max-1; ++g )
	  {
	  	Real tempavg = 0.5*sizes[g] + 0.5*sizes[g+1];
		  avgs.push_back( ceil(tempavg) );
		}
	}
}
