# This tests HomNucleation, as well as HomNucleationMaterial
#  Exact solution in the accompanying excel spreadsheet
#
# +----------------+----------------+----------------+
# | time           | c1_num         | c2_num         |
# +----------------+----------------+----------------+
# |   1.000000e+00 |   1.000000e+02 |   5.220259e-04 |
# |   2.000000e+00 |   1.000000e+02 |   1.044052e-03 |
# |   3.000000e+00 |   1.000000e+02 |   1.566078e-03 |
# |   4.000000e+00 |   1.000000e+02 |   2.088104e-03 |
# |   5.000000e+00 |   1.000000e+02 |   2.610129e-03 |
# |   6.000000e+00 |   1.000000e+02 |   3.132155e-03 |
# |   7.000000e+00 |   1.000000e+02 |   3.654181e-03 |
# |   8.000000e+00 |   1.000000e+02 |   4.176207e-03 |
# |   9.000000e+00 |   1.000000e+02 |   4.698233e-03 |
# |   1.000000e+01 |   1.000000e+02 |   5.220259e-03 |
# +----------------+----------------+----------------+

[GlobalParams]
  nucleation_conc_vars = 'c1 c2'
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
    m = 2
  [../]
[]


[Materials]
  [./c1_diff]
    type = AtomicDiffusionCoef
    temp = 1000
    D0 = 1.7e5
    Q = 2.3
    k = 8.617e-5
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
