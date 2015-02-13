# Concentration of c1,c2 -> 0
# Concentration of c3 -> 1/3
# Sum should always be 100
#
# This should be the exact same as tests/homnucleation/3species.i
#
# +----------------+----------------+----------------+----------------+----------------+
# | time           | c1_conc        | c2_conc        | c3_conc        | sum            |
# +----------------+----------------+----------------+----------------+----------------+
# |   1.000000e+03 |   1.796419e-01 |   1.795431e-02 |   3.326148e+01 |   1.000000e+02 |
# |   2.000000e+03 |   7.090517e-03 |   9.417167e-04 |   3.333034e+01 |   1.000000e+02 |
# |   3.000000e+03 |   1.278345e-03 |   1.852194e-04 |   3.333278e+01 |   1.000000e+02 |
# |   4.000000e+03 |   4.726737e-04 |   7.075089e-05 |   3.333313e+01 |   1.000000e+02 |
# |   5.000000e+03 |   2.485447e-04 |   3.773549e-05 |   3.333323e+01 |   1.000000e+02 |
# |   6.000000e+03 |   1.578561e-04 |   2.413830e-05 |   3.333326e+01 |   1.000000e+02 |
# |   7.000000e+03 |   1.120683e-04 |   1.720557e-05 |   3.333328e+01 |   1.000000e+02 |
# |   8.000000e+03 |   8.543138e-05 |   1.314835e-05 |   3.333330e+01 |   1.000000e+02 |
# |   9.000000e+03 |   6.836292e-05 |   1.053840e-05 |   3.333330e+01 |   1.000000e+02 |
# |   1.000000e+04 |   5.664047e-05 |   8.741095e-06 |   3.333331e+01 |   1.000000e+02 |
# +----------------+----------------+----------------+----------------+----------------+

[GlobalParams]
  N_nuc = 3
[]

[Mesh]
  type = GeneratedMesh
  dim = 1
[]

[Clusters]
  [./Nucleation]
  [../]
[]

[Variables]
  [./c1]
    initial_condition = 100
  [../]
[]


[Kernels]
  [./c1_time]
    type = TimeDerivative
    variable = c1
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
    c1_rx_coeffs = '1 10 0'
    omega = 3.0e4
    a = 0.5
    cluster_diffusion = false
  [../]
[]


[Executioner]
  type = Transient

  solve_type = PJFNK

  num_steps = 10
  dt = 1000
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
  [./c3_conc]
    type = ElementIntegralVariablePostprocessor
    variable = c3
  [../]
  [./sum]
    type = SumOfPostprocessors
    postprocessors = 'c1_conc c2_conc c3_conc'
    factors = '1 2 3'
  [../]
[]

[Outputs]
  console = true
  exodus = true
[]
