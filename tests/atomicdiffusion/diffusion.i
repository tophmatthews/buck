# Tests SpeciesDiffusion against analytical solution.
#  Model is of a 100 nm line with 1000 segements.
#  The left side of the model has a constant BC of 1
#  The initial condition is 0.
#  The sample point is 0.5 nm from edge.
#  Analytical solution is c(x,t) = c0(1-erf(x/sqrt(4Dt)))
#
# +----------------+----------------+ ----------------+
# | time           | point          | analytical
# +----------------+----------------+ ----------------+
# |   1.000000e+05 |   8.987268e-02 | 0.09005328
# |   2.000000e+05 |   2.262948e-01 | 0.230672419
# |   3.000000e+05 |   3.236715e-01 | 0.327740046
# |   4.000000e+05 |   3.933281e-01 | 0.396683971
# |   5.000000e+05 |   4.456521e-01 | 0.448403543
# +----------------+----------------+ ----------------+

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
    D0 = 1.7e5
    Q = 2.3
    k = 8.617e-5
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
[]

[Outputs]
  output_on = 'initial linear nonlinear failed'
  interval = 10
  exodus = true
[]
