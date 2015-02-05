# This tests the postprocessor MaterialXeBubbleTester that in turn tests the namespace MaterialXeBubbleTester
# The order of magnitude of atoms (i.e. 10^m) is varied across a simple line. The output radius, given the 
# temperature and stress. The values are double checked against the formulation for atomic density defined
# in MaterialXeBubble, as well as a separate calculation method. The output should be as given below,
# and the "% diff" output at each timestep should be small (~1e-8):

# +----------------+------------------------+----------------+
# | time           | MaterialXeBubbleTester | m_mag          |
# +----------------+------------------------+----------------+
# |   1.000000e+00 |           1.218719e-09 |   1.900000e+00 |
# |   2.000000e+00 |           2.471825e-09 |   2.800000e+00 |
# |   3.000000e+00 |           5.004088e-09 |   3.700000e+00 |
# |   4.000000e+00 |           1.008877e-08 |   4.600000e+00 |
# |   5.000000e+00 |           2.025854e-08 |   5.500000e+00 |
# |   6.000000e+00 |           4.056540e-08 |   6.400000e+00 |
# |   7.000000e+00 |           8.109169e-08 |   7.300000e+00 |
# |   8.000000e+00 |           1.619570e-07 |   8.200000e+00 |
# |   9.000000e+00 |           3.233068e-07 |   9.100000e+00 |
# |   1.000000e+01 |           6.452434e-07 |   1.000000e+01 |
# +----------------+------------------------+----------------+


[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 1
  xmax = 1
  xmin = 0
[]


[Variables]
  [./m]
    initial_condition = 10
  [../]
[]


[Kernels]
  [./u_time]
    type = TimeDerivative
    variable = m
  [../]
[]


[BCs]
  [./bcs]
    type = FunctionDirichletBC
    variable = m
    boundary = 'right left'
    function = bc_fcn
  [../]
[]


[Functions]
  [./bc_fcn]
    type = PiecewiseLinear
    x = '0 10'
    y = '1 10'
  [../]
[]


[Executioner]
  type = Transient

  solve_type = PJFNK

  num_steps = 10
  dt = 1
[]


[Postprocessors]
  [./m_mag]
    type = SideAverageValue
    variable = m
    boundary = left
  [../]
  [./radius]
    type = MaterialXeBubbleTester
    temp = 2000
    sigma = 1e9
    testing = true
    m_mag = m_mag
  [../]
[]


[Outputs]
  console = true
  exodus = true
[]
