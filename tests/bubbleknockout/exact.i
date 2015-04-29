# Tests Knockout of bubbles. Check excel file for exact solution.
# Total should equal 3e3 at all times.
#
# +----------------+----------------+----------------+----------------+----------------+
# | time           | c1             | c2             | c3             | total          |
# +----------------+----------------+----------------+----------------+----------------+
# |   1.000000e+04 |   5.197438e+01 |   4.913527e+01 |   9.499184e+02 |   3.000000e+03 |
# |   2.000000e+04 |   1.049698e+02 |   9.399777e+01 |   9.023449e+02 |   3.000000e+03 |
# |   3.000000e+04 |   1.588015e+02 |   1.348683e+02 |   8.571540e+02 |   3.000000e+03 |
# |   4.000000e+04 |   2.132988e+02 |   1.720112e+02 |   8.142263e+02 |   3.000000e+03 |
# |   5.000000e+04 |   2.683045e+02 |   2.056750e+02 |   7.734485e+02 |   3.000000e+03 |
# |   6.000000e+04 |   3.236736e+02 |   2.360938e+02 |   7.347129e+02 |   3.000000e+03 |
# |   7.000000e+04 |   3.792731e+02 |   2.634875e+02 |   6.979173e+02 |   3.000000e+03 |
# |   8.000000e+04 |   4.349806e+02 |   2.880630e+02 |   6.629645e+02 |   3.000000e+03 |
# |   9.000000e+04 |   4.906840e+02 |   3.100148e+02 |   6.297621e+02 |   3.000000e+03 |
# |   1.000000e+05 |   5.462808e+02 |   3.295257e+02 |   5.982226e+02 |   3.000000e+03 |
# +----------------+----------------+----------------+----------------+----------------+

[GlobalParams]
  coupled_conc = 'c1 c2 c3'
  coupled_rad = 'r1 r2 r3'
  coupled_atoms = '1 2 3'
  coupled_widths = '1 1 1'
  fission_rate = 10
[]


[Mesh]
  type = GeneratedMesh
  dim = 1
[]


[Variables]
  [./c1]
  [../]
  [./c2]
  [../]
  [./c3]
    initial_condition = 1e3
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
  [./c3_time]
    type = TimeDerivative
    variable = c3
  [../]

  [./c1_Knockout]
    type = BubbleKnockout
    variable = c1
  [../]
  [./c2_Knockout]
    type = BubbleKnockout
    variable = c2
  [../]
  [./c3_Knockout]
    type = BubbleKnockout
    variable = c3
  [../]
[]

[AuxVariables]
  [./r1]
  [../]
  [./r2]
  [../]
  [./r3]
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
[]

[Executioner]
  type = Transient

  solve_type = PJFNK

  num_steps = 1000
  dt = 100
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
  [./total]
    type = SumOfPostprocessors
    postprocessors = 'c1 c2 c3'
    factors = '1 2 3'
  [../]
[]

[Outputs]
  interval = 100
  exodus = true
[]
