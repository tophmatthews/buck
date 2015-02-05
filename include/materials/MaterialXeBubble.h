#ifndef MATERIALXEBUBBLE_H
#define MATERIALXEBUBBLE_H

namespace MaterialXeBubble {

  double VDW_RtoRho(double R, double T, double sigma);
  double VDW_MtoR(double m, double T, double sigma, bool testing=false);
}

#endif // MATERIALXEBUBBLE_H
