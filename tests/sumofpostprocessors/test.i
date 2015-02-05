# This tests the SumPostprocessor.
#   The sum_factors should 0, or just about ( u_left*1 + u_right*2)
#   The sum should be sum of u_left and u_right
#
# +----------------+----------------+----------------+----------------+----------------+
# | time           | sum            | sum_factors    | u_left         | u_right        |
# +----------------+----------------+----------------+----------------+----------------+
# |   1.000000e+00 |  -9.999969e-02 |   4.722475e-07 |  -1.999998e-01 |   1.000002e-01 |
# |   2.000000e+00 |  -2.000000e-01 |   2.220446e-16 |  -4.000000e-01 |   2.000000e-01 |
# |   3.000000e+00 |  -3.000000e-01 |   0.000000e+00 |  -6.000000e-01 |   3.000000e-01 |
# |   4.000000e+00 |  -4.000000e-01 |   0.000000e+00 |  -8.000000e-01 |   4.000000e-01 |
# |   5.000000e+00 |  -5.000000e-01 |   0.000000e+00 |  -1.000000e+00 |   5.000000e-01 |
# |   6.000000e+00 |  -6.000000e-01 |   0.000000e+00 |  -1.200000e+00 |   6.000000e-01 |
# |   7.000000e+00 |  -7.000000e-01 |   0.000000e+00 |  -1.400000e+00 |   7.000000e-01 |
# |   8.000000e+00 |  -8.000000e-01 |   0.000000e+00 |  -1.600000e+00 |   8.000000e-01 |
# |   9.000000e+00 |  -9.000000e-01 |   0.000000e+00 |  -1.800000e+00 |   9.000000e-01 |
# |   1.000000e+01 |  -1.000000e+00 |   0.000000e+00 |  -2.000000e+00 |   1.000000e+00 |
# +----------------+----------------+----------------+----------------+----------------+

[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 1
  xmax = 1
  xmin = 0
[]


[Variables]
  [./u]
    initial_condition = 100
  [../]
[]


[Kernels]
  [./u_time]
    type = TimeDerivative
    variable = u
  [../]
[]


[BCs]
  [./right1]
    type = FunctionDirichletBC
    variable = u
    boundary = 'right'
    function = bc_fcn_right
  [../]
  [./left1]
    type = FunctionDirichletBC
    variable = u
    boundary = 'left'
    function = bc_fcn_left
  [../]
[]


[Functions]
  [./bc_fcn_right]
    type = PiecewiseLinear
    x = '0 10'
    y = '0 1'
  [../]
  [./bc_fcn_left]
    type = PiecewiseLinear
    x = '0 10'
    y = '0 -2'
  [../]
[]


[Executioner]
  type = Transient

  solve_type = PJFNK

  num_steps = 10
  dt = 1
[]


[Postprocessors]
  [./u_right]
    type = SideAverageValue
    variable = u
    boundary = right
  [../]
  [./u_left]
    type = SideAverageValue
    variable = u
    boundary = left
  [../]
  [./sum_factors]
    type = SumOfPostprocessors
    postprocessors = 'u_left u_right'
    factors = '1 2'
  [../]
  [./sum]
    type = SumOfPostprocessors
    postprocessors = 'u_left u_right'
  [../]
[]


[Outputs]
  console = true
  exodus = true
[]
