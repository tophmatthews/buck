# Tests growth of bubbles. Solution should be the exact same as tests/bubbleffnucleation/exact.i
#
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+
# | time           | c1             | c2             | c3             | c4             | total          | total_conc     |
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+
# |   5.000000e+03 |   9.400000e+07 |   0.000000e+00 |   0.000000e+00 |   1.500000e+06 |   1.000000e+08 |   1.500000e+06 |
# |   1.000000e+04 |   8.800000e+07 |   0.000000e+00 |   0.000000e+00 |   3.000000e+06 |   1.000000e+08 |   3.000000e+06 |
# |   1.500000e+04 |   8.200000e+07 |   0.000000e+00 |   0.000000e+00 |   4.500000e+06 |   1.000000e+08 |   4.500000e+06 |
# |   2.000000e+04 |   7.600000e+07 |   0.000000e+00 |   0.000000e+00 |   6.000000e+06 |   1.000000e+08 |   6.000000e+06 |
# |   2.500000e+04 |   7.000000e+07 |   0.000000e+00 |   0.000000e+00 |   7.500000e+06 |   1.000000e+08 |   7.500000e+06 |
# |   3.000000e+04 |   6.400000e+07 |   0.000000e+00 |   0.000000e+00 |   9.000000e+06 |   1.000000e+08 |   9.000000e+06 |
# |   3.500000e+04 |   5.800000e+07 |   0.000000e+00 |   0.000000e+00 |   1.050000e+07 |   1.000000e+08 |   1.050000e+07 |
# |   4.000000e+04 |   5.200000e+07 |   0.000000e+00 |   0.000000e+00 |   1.200000e+07 |   1.000000e+08 |   1.200000e+07 |
# |   4.500000e+04 |   4.600000e+07 |   0.000000e+00 |   0.000000e+00 |   1.350000e+07 |   1.000000e+08 |   1.350000e+07 |
# |   5.000000e+04 |   4.000000e+07 |   0.000000e+00 |   0.000000e+00 |   1.500000e+07 |   1.000000e+08 |   1.500000e+07 |
# |   5.500000e+04 |   3.400000e+07 |   0.000000e+00 |   0.000000e+00 |   1.650000e+07 |   1.000000e+08 |   1.650000e+07 |
# |   6.000000e+04 |   2.800000e+07 |   0.000000e+00 |   0.000000e+00 |   1.800000e+07 |   1.000000e+08 |   1.800000e+07 |
# |   6.500000e+04 |   2.200000e+07 |   0.000000e+00 |   0.000000e+00 |   1.950000e+07 |   1.000000e+08 |   1.950000e+07 |
# |   7.000000e+04 |   1.600000e+07 |   0.000000e+00 |   0.000000e+00 |   2.100000e+07 |   1.000000e+08 |   2.100000e+07 |
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+

[GlobalParams]
  temp = temp
  fission_rate = fission_rate
[]


[Mesh]
  type = GeneratedMesh
  dim = 1
[]

[Bubbles]
  [./FFNucleation]
    N = 4
    s = 4
  [../]
[]


[Variables]
  [./c1]
    initial_condition = 1e8
  [../]
  [./c2]
  [../]
  [./c3]
  [../]
  [./c4]
  [../]
  [./temp]
    initial_condition = 1000
  [../]
[]


[Kernels]
  [./temp_time]
    type = TimeDerivative
    variable = temp
  [../]

  [./c1_time]
    type = TimeDerivative
    variable = c1
  [../]
  [./c2_time]
    type = TimeDerivative
    variable = c2
  [../]
  [./c3_time]
    type = TimeDerivative
    variable = c3
  [../]
  [./c4_time]
    type = TimeDerivative
    variable = c4
  [../]

  # [./c1_nuc]
  #   type = BubbleFFNucleation
  #   variable = c1
  # [../]
  # [./c2_nuc]
  #   type = BubbleFFNucleation
  #   variable = c2
  # [../]
  # [./c3_nuc]
  #   type = BubbleFFNucleation
  #   variable = c3
  # [../]
  # [./c4_nuc]
  #   type = BubbleFFNucleation
  #   variable = c4
  # [../]
[]

[AuxVariables]
  [./r1]
  [../]
  [./r2]
  [../]
  [./r3]
  [../]
  [./r4]
  [../]
  [./fission_rate]
  [../]
[]

[AuxKernels]
  [./r1_aux]
    type = ConstantAux
    variable = r1
    value = 1e-3
  [../]
  [./r2_aux]
    type = ConstantAux
    variable = r2
    value = 2e-3
  [../]
  [./r3_aux]
    type = ConstantAux
    variable = r3
    value = 3e-3
  [../]
  [./r4_aux]
    type = ConstantAux
    variable = r4
    value = 4e-3
  [../]
  [./fission_rate_aux]
    type = ConstantAux
    variable = fission_rate
    value = 60
  [../]
[]

[Dampers]
  [./c1_damper]
    type = PositiveDamper
    damping = 0.1
    variable = c1
  [../]
[]

[Materials]
  [./Dg]
    type = GasAtomDiffusivity
    model = 4
    factor = 1
    block = 0
  [../]
[]


[Executioner]
  type = Transient

  solve_type = PJFNK

  end_time = 7e4
  dt = 500
[]

[Postprocessors]
  [./c1]
    type = ElementAverageValue
    variable = c1
  [../]
  [./c2]
    type = ElementAverageValue
    variable = c2
  [../]
  [./c3]
    type = ElementAverageValue
    variable = c3
  [../]
  [./c4]
    type = ElementAverageValue
    variable = c4
  [../]
  [./total]
    type = SumOfPostprocessors
    postprocessors = 'c1 c2 c3 c4'
    factors = '1 2 3 4'
  [../]
  [./total_conc]
    type = SumOfPostprocessors
    postprocessors = 'c2 c3 c4'
  [../]
[]

[Outputs]
  interval = 10
  exodus = true
[]
