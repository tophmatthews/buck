# Tests SpeciesDiffusion against analytical solution.
#  Model is of a 100 nm line with 1000 segements.
#  The left side of the model has a constant BC of 1
#  The initial condition is 0.
#  The sample point is 0.5 nm from edge.
#  Analytical solution is c(x,t) = c0(1-erf(x/sqrt(4Dt)))
#
# t         EXCEL       BUCK      % diff
# 1.00E+05  0.702916768 6.92E-01  1.49E+00
# 2.00E+05  0.787405907 7.84E-01  4.93E-01
# 3.00E+05  0.825720558 8.24E-01  2.60E-01
# 4.00E+05  0.848766081 8.47E-01  1.65E-01
# 5.00E+05  0.864568925 8.64E-01  1.16E-01

[GlobalParams]
  order = SECOND
  family = LAGRANGE
[]


[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 1000
  xmax = 100
  xmin = 0
  second_order = true
[]


[Variables]
  [./c1]
  [../]
[]


[Kernels]
  [./c1_time]
    type = TimeDerivative
    variable = c1
  [../]
  [./c1_diffusion]
    type = AtomicDiffusion
    variable = c1
  [../]
[]


[BCs]
  [./right1]
    type = DirichletBC
    variable = c1
    value = 1
    boundary = left
  [../]
[]


[Materials]
  [./diff]
    type = AtomicDiffusionCoef
    temp = 1000
    model = 1
    factor = 1
    block = 0
  [../]
[]


[Executioner]
  type = Transient

  solve_type = PJFNK

  petsc_options_iname = '-ksp_gmres_restart -pc_type -pc_hypre_type -pc_hypre_boomeramg_max_iter'
  petsc_options_value = '201                hypre    boomeramg      4'

  num_steps = 50
  dt = 1e4
[]

[Postprocessors]
  [./point]
    type = PointValue
    variable = c1
    point = '0.5 0 0'
  [../]
[]

[Outputs]
  output_on = 'initial linear nonlinear failed'
  interval = 10
  exodus = true
[]
