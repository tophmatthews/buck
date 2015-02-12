# Test for SinkGrowth. See accompanying excel file for the exact solution
# Sum should always be 600
#
# This should be exactly the same as tests/sinkgrowth/3species.i
#
# +----------------+----------------+----------------+----------------+----------------+
# | time           | c1_conc        | c5_conc        | c6_conc        | sum            |
# +----------------+----------------+----------------+----------------+----------------+
# |   1.000000e+04 |   9.754341e-01 |   9.754341e-01 |   2.456588e-02 |   6.000000e+00 |
# |   2.000000e+04 |   9.520328e-01 |   9.520328e-01 |   4.796719e-02 |   6.000000e+00 |
# |   3.000000e+04 |   9.297158e-01 |   9.297158e-01 |   7.028424e-02 |   6.000000e+00 |
# |   4.000000e+04 |   9.084099e-01 |   9.084099e-01 |   9.159015e-02 |   6.000000e+00 |
# |   5.000000e+04 |   8.880484e-01 |   8.880484e-01 |   1.119516e-01 |   6.000000e+00 |
# |   6.000000e+04 |   8.685703e-01 |   8.685703e-01 |   1.314297e-01 |   6.000000e+00 |
# |   7.000000e+04 |   8.499197e-01 |   8.499197e-01 |   1.500803e-01 |   6.000000e+00 |
# |   8.000000e+04 |   8.320453e-01 |   8.320453e-01 |   1.679547e-01 |   6.000000e+00 |
# |   9.000000e+04 |   8.149000e-01 |   8.149000e-01 |   1.851000e-01 |   6.000000e+00 |
# |   1.000000e+05 |   7.984404e-01 |   7.984404e-01 |   2.015596e-01 |   6.000000e+00 |
# +----------------+----------------+----------------+----------------+----------------+

[GlobalParams]
  temp = temp
[]


[Mesh]
  type = GeneratedMesh
  dim = 1
[]


[Clusters]
  [./Growth]
    N = 6
    N_nuc = 5
    N_min_transient = true
  [../]
[]


[ICs]
  [./c5]
    type = ConstantIC
    variable = c5
    value = 1
  [../]
  [./c1]
    type = ConstantIC
    variable = c1
    value = 1
[]


[Variables]
  [./c1]
  [../]
[]


[Kernels]
  [./c1_time]
    type = TimeDerivative
    variable = c1
  [../]
[]


[AuxVariables]
  [./temp]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]


[AuxKernels]
  [./temp_aux]
    type = ConstantAux
    variable = temp
    value = 1000
  [../]
[]


[Materials]
  [./c1_diff]
    type = AtomicDiffusionCoef
    D0 = 1.7e5
    Q = 2.3
    k = 8.617e-5
    factor = 1
    block = 0
  [../]
[]


[Executioner]
  type = Transient

  solve_type = PJFNK

  num_steps = 10
  dt = 1e4
[]


[Postprocessors]
  [./c1_conc]
    type = ElementIntegralVariablePostprocessor
    variable = c1
  [../]
  [./c5_conc]
    type = ElementIntegralVariablePostprocessor
    variable = c5
  [../]
  [./c6_conc]
    type = ElementIntegralVariablePostprocessor
    variable = c6
  [../]
  [./sum]
    type = SumOfPostprocessors
    postprocessors = 'c1_conc c5_conc c6_conc'
    factors = '1 5 6'
  [../]
[]


[Outputs]
  console = true
  exodus = true
[]
