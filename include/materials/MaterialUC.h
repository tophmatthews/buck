#ifndef MATERIALUC_H
#define MATERIALUC_H

namespace UCThermal{

const double TMELT_UC(2638.0); // Melting point (K) of fresh UC

}

namespace UCMechanical {

double FALPHA(double T);
double YOUNGS(double T, double p);
double POISSONS(double T);

}

#endif
