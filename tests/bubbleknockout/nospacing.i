# Tests Knockout of bubbles. Total should equal 5 at all times.
# 
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+
# | time           | c1             | c2             | c3             | c4             | c5             | total          |
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+
# |   1.000000e+05 |   3.112952e-01 |   2.716684e-03 |   3.384583e-02 |   2.348322e-01 |   7.284810e-01 |   5.000000e+00 |
# |   2.000000e+05 |   6.049506e-01 |   1.519453e-02 |   1.000913e-01 |   3.527409e-01 |   5.306846e-01 |   5.000000e+00 |
# |   3.000000e+05 |   8.834352e-01 |   3.913819e-02 |   1.717628e-01 |   3.975080e-01 |   3.865936e-01 |   5.000000e+00 |
# |   4.000000e+05 |   1.148979e+00 |   7.251293e-02 |   2.348839e-01 |   3.983033e-01 |   2.816261e-01 |   5.000000e+00 |
# |   5.000000e+05 |   1.403280e+00 |   1.118464e-01 |   2.833850e-01 |   3.742688e-01 |   2.051593e-01 |   5.000000e+00 |
# |   6.000000e+05 |   1.647461e+00 |   1.535002e-01 |   3.157973e-01 |   3.377185e-01 |   1.494546e-01 |   5.000000e+00 |
# |   7.000000e+05 |   1.882127e+00 |   1.943045e-01 |   3.331481e-01 |   2.963613e-01 |   1.088749e-01 |   5.000000e+00 |
# |   8.000000e+05 |   2.107493e+00 |   2.318144e-01 |   3.376541e-01 |   2.548373e-01 |   7.931327e-02 |   5.000000e+00 |
# |   9.000000e+05 |   2.323504e+00 |   2.643500e-01 |   3.319394e-01 |   2.157716e-01 |   5.777821e-02 |   5.000000e+00 |
# |   1.000000e+06 |   2.529949e+00 |   2.909262e-01 |   3.185921e-01 |   1.804926e-01 |   4.209033e-02 |   5.000000e+00 |
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+

[GlobalParams]
  coupled_conc = 'c1 c2 c3 c4 c5'
  coupled_rad = 'r1 r2 r3 r4 r5'
  coupled_atoms = '1 2 3 4 5'
  coupled_widths = '1 1 1 1 1'
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
  [../]
  [./c4]
  [../]
  [./c5]
    initial_condition = 1
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
  [./c4_time]
    type = TimeDerivative
    variable = c4
  [../]
  [./c5_time]
    type = TimeDerivative
    variable = c5
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
  [./c4_Knockout]
    type = BubbleKnockout
    variable = c4
  [../]
  [./c5_Knockout]
    type = BubbleKnockout
    variable = c5
  [../]
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
  [./r5]
  [../]
[]

[AuxKernels]
  [./r1_aux]
    type = ConstantAux
    variable = r1
    value = 1
  [../]
  [./r2_aux]
    type = ConstantAux
    variable = r2
    value = 2
  [../]
  [./r3_aux]
    type = ConstantAux
    variable = r3
    value = 3
  [../]
  [./r4_aux]
    type = ConstantAux
    variable = r4
    value = 4
  [../]
  [./r5_aux]
    type = ConstantAux
    variable = r5
    value = 5
  [../]
[]


[Executioner]
  type = Transient

  solve_type = PJFNK

  num_steps = 100
  dt = 10000
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
  [./c5]
    type = ElementAverageValue
    variable = c5
  [../]
  [./total]
    type = SumOfPostprocessors
    postprocessors = 'c1 c2 c3 c4 c5'
    factors = '1 2 3 4 5'
  [../]
[]

[Outputs]
  interval = 10
  exodus = true
[]
