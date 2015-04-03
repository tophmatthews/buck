# Tests Growth of bubbles. Total should equal 3e5 at all times.

# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+
# | time           | c1             | c2             | c3             | c4             | c5             | total          |
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+
# |   1.000000e+07 |   9.630403e+04 |   9.638858e+04 |   3.528406e+03 |   8.148382e+01 |   1.533488e+00 |   3.000000e+05 |
# |   2.000000e+07 |   9.272649e+04 |   9.303410e+04 |   6.668136e+03 |   2.879142e+02 |   9.849711e+00 |   3.000000e+05 |
# |   3.000000e+07 |   8.926549e+04 |   8.991471e+04 |   9.465217e+03 |   5.909242e+02 |   2.914842e+01 |   3.000000e+05 |
# |   4.000000e+07 |   8.591914e+04 |   8.701069e+04 |   1.195983e+04 |   9.674099e+02 |   6.206963e+01 |   3.000000e+05 |
# |   5.000000e+07 |   8.268547e+04 |   8.430421e+04 |   1.418712e+04 |   1.398591e+03 |   1.100766e+02 |   3.000000e+05 |
# |   6.000000e+07 |   7.956242e+04 |   8.177917e+04 |   1.617783e+04 |   1.869240e+03 |   1.737560e+02 |   3.000000e+05 |
# |   7.000000e+07 |   7.654779e+04 |   7.942095e+04 |   1.795895e+04 |   2.367050e+03 |   2.530525e+02 |   3.000000e+05 |
# |   8.000000e+07 |   7.363929e+04 |   7.721630e+04 |   1.955413e+04 |   2.882109e+03 |   3.474511e+02 |   3.000000e+05 |
# |   9.000000e+07 |   7.083449e+04 |   7.515320e+04 |   2.098420e+04 |   3.406477e+03 |   4.561178e+02 |   3.000000e+05 |
# |   1.000000e+08 |   6.813086e+04 |   7.322070e+04 |   2.226746e+04 |   3.933829e+03 |   5.780085e+02 |   3.000000e+05 |
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+

[GlobalParams]
  temp = temp
  coupled_conc = 'c1 c2 c3 c4 c5'
  coupled_rad = 'r1 r2 r3 r4 r5'
  coupled_atoms = '1 2 3 4 5'
  coupled_widths = '1 1 1 1 1'
  allow_loss = false
[]


[Mesh]
  type = GeneratedMesh
  dim = 1
[]


[Variables]
  [./c1]
    initial_condition = 1e5
  [../]
  [./c2]
    initial_condition = 1e5
  [../]
  [./c3]
  [../]
  [./c4]
  [../]
  [./c5]
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
  [./c5_time]
    type = TimeDerivative
    variable = c5
  [../]

  [./c1_Growth]
    type = BubbleGrowth
    variable = c1
  [../]
  [./c2_Growth]
    type = BubbleGrowth
    variable = c2
  [../]
  [./c3_Growth]
    type = BubbleGrowth
    variable = c3
  [../]
  [./c4_Growth]
    type = BubbleGrowth
    variable = c4
  [../]
  [./c5_Growth]
    type = BubbleGrowth
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
    type = EquilibriumRadiusAux
    variable = r1
    m = 1
  [../]
  [./r2_aux]
    type = EquilibriumRadiusAux
    variable = r2
    m = 2
  [../]
  [./r3_aux]
    type = EquilibriumRadiusAux
    variable = r3
    m = 3
  [../]
  [./r4_aux]
    type = EquilibriumRadiusAux
    variable = r4
    m = 4
  [../]
  [./r5_aux]
    type = EquilibriumRadiusAux
    variable = r5
    m = 5
  [../]
[]


[Materials]
  [./Dg]
    type = GasAtomDiffusivity
    model = 1
    factor = 1
    block = 0
  [../]
[]


[Executioner]
  type = Transient

  solve_type = PJFNK

  num_steps = 100
  dt = 1e6
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
