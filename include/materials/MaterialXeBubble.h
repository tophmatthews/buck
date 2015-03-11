#ifndef MATERIALXEBUBBLE_H
#define MATERIALXEBUBBLE_H

namespace MaterialXeBubble {

  double VDW_RtoRho(double R, double T, double sigma);
  double VDW_RtoP(double R, double T, double m);
  double VDW_MtoR(double m, double T, double sigma, double gamma, double B, bool testing);

}

#endif // MATERIALXEBUBBLE_H
