# Concentration of c1 -> 0
# Concentration of c2 -> 1/2
# Sum should always be 100
#
# +----------------+----------------+----------------+----------------+
# | time           | c1_conc        | c2_conc        | sum            |
# +----------------+----------------+----------------+----------------+
# |   1.000000e+01 |   9.319568e+00 |   4.534022e+01 |   1.000000e+02 |
# |   2.000000e+01 |   2.546934e+00 |   4.872653e+01 |   1.000000e+02 |
# |   3.000000e+01 |   1.154749e+00 |   4.942263e+01 |   1.000000e+02 |
# |   4.000000e+01 |   6.766808e-01 |   4.966166e+01 |   1.000000e+02 |
# |   5.000000e+01 |   4.578345e-01 |   4.977108e+01 |   1.000000e+02 |
# |   6.000000e+01 |   3.383270e-01 |   4.983084e+01 |   1.000000e+02 |
# |   7.000000e+01 |   2.650054e-01 |   4.986750e+01 |   1.000000e+02 |
# |   8.000000e+01 |   2.162027e-01 |   4.989190e+01 |   1.000000e+02 |
# |   9.000000e+01 |   1.817242e-01 |   4.990914e+01 |   1.000000e+02 |
# |   1.000000e+02 |   1.562385e-01 |   4.992188e+01 |   1.000000e+02 |
# +----------------+----------------+----------------+----------------+

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
  [./c1_nucleation]
    type = HomNucleation
    variable = c1
    m = 1
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
