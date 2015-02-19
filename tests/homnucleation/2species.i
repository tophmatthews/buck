# Concentration of c1 -> 0
# Concentration of c2 -> 1/2
# Sum should always be 100
#
# +----------------+----------------+----------------+----------------+
# | time           | c1_conc        | c2_conc        | sum            |
# +----------------+----------------+----------------+----------------+
# |   1.000000e+01 |   2.177825e+00 |   4.891109e+01 |   1.000000e+02 |
# |   2.000000e+01 |   3.016099e-01 |   4.984920e+01 |   1.000000e+02 |
# |   3.000000e+01 |   9.909151e-02 |   4.995045e+01 |   1.000000e+02 |
# |   4.000000e+01 |   4.918880e-02 |   4.997541e+01 |   1.000000e+02 |
# |   5.000000e+01 |   3.027924e-02 |   4.998486e+01 |   1.000000e+02 |
# |   6.000000e+01 |   2.109831e-02 |   4.998945e+01 |   1.000000e+02 |
# |   7.000000e+01 |   1.589042e-02 |   4.999205e+01 |   1.000000e+02 |
# |   8.000000e+01 |   1.261053e-02 |   4.999369e+01 |   1.000000e+02 |
# |   9.000000e+01 |   1.038582e-02 |   4.999481e+01 |   1.000000e+02 |
# |   1.000000e+02 |   8.791659e-03 |   4.999560e+01 |   1.000000e+02 |
# +----------------+----------------+----------------+----------------+

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
  [./c1_nucleation]
    type = HomNucleation
    variable = c1
    g = 1
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
    omega = 3.0e4
    a = 0.5
    cluster_diffusion = false
    N_nuc = 2
  [../]
[]


[Executioner]
  type = Transient

  solve_type = PJFNK

  num_steps = 10
  dt = 10
[]


[Postprocessors]
  [./c1_conc]
    type = ElementIntegralVariablePostprocessor
    variable = c1
  [../]
  [./c2_conc]
    type = ElementIntegralVariablePostprocessor
    variable = c2
  [../]
  [./sum]
    type = SumOfPostprocessors
    postprocessors = 'c1_conc c2_conc'
    factors = '1 2'
  [../]
[]

[Outputs]
  console = true
  exodus = true
[]
