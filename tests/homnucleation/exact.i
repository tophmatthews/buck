# This tests HomNucleation, as well as HomNucleationMaterial
#  Exact solution in the accompanying excel spreadsheet
#
# EXCEL         BUCK      % diff
# 0.000000E+00  0
# 1.031244E-02  1.03E-02  1.93893E-05
# 2.062488E-02  2.06E-02  1.93893E-05
# 3.093733E-02  3.09E-02  1.29341E-05
# 4.124977E-02  4.12E-02  4.85324E-06
# 5.156221E-02  5.16E-02  4.72353E-09
# 6.187465E-02  6.19E-02  3.22762E-06
# 7.218709E-02  7.22E-02  5.53643E-06
# 8.249954E-02  8.25E-02  4.85324E-06
# 9.281198E-02  9.28E-02  2.15962E-06
# 1.031244E-01  1.03E-01  1.93893E-05

[GlobalParams]
  coupled_conc = 'c1 c2'
  N_nuc = 2
[]


[Mesh]
  type = GeneratedMesh
  dim = 1
[]


[Variables]
  [./c1]
    initial_condition = 100
  [../]
  [./c2]
  [../]
[]


[Kernels]
  [./c1_time]
    type = TimeDerivative
    variable = c1
  [../]

  [./c2_time]
    type = TimeDerivative
    variable = c2
  [../]
  [./c2_nucleation]
    type = HomNucleation
    variable = c2
    g = 2
  [../]
[]


[Materials]
  [./c1_diff]
    type = AtomicDiffusionCoef
    temp = 1000
    model = 1
    factor = 1
    block = 0
  [../]
  [./cN_coeffs]
    type = HomNucleationMaterial
    block = 0
    c1_rx_coeffs = '1 0'
    omega = 3.0e-2
    a = 0.5
    cluster_diffusion = false
    N_nuc = 2
  [../]
[]


[Executioner]
  type = Transient

  solve_type = PJFNK

  num_steps = 10
  dt = 1
[]


[Postprocessors]
  [./c1_num]
    type = ElementIntegralVariablePostprocessor
    variable = c1
  [../]
  [./c2_num]
    type = ElementIntegralVariablePostprocessor
    variable = c2
  [../]
[]

[Outputs]
  console = true
  exodus = true
  interval = 1
[]
