# Tests SpeciesDiffusion against analytical solution.
#  Model is of a 1 um line with 100 segements.
#  The left side of the model has a constant BC of 1
#  The initial condition is 0.
#  The sample point is 0.1 um from left edge.
#  Analytical solution is c(x,t) = c0(1-erf(x/sqrt(4Dt)))
#  See corresponding excel file
#
# t           EXCEL       BUCK        % diff
# 1.0000E+05  8.0939E-01  8.0870E-01  8.5181E-02
# 2.0000E+05  8.6457E-01  8.6432E-01  2.8850E-02

[GlobalParams]
  order = SECOND
  family = LAGRANGE
[]


[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 100
  xmax = 1
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
    type = BasicDiffusion
    diffusivity = gas_diffusivity
    variable = c1
  [../]
[]


[BCs]
  [./left]
    type = DirichletBC
    variable = c1
    value = 1
    boundary = left
  [../]
[]


[Materials]
  [./diff]
    type = GasAtomDiffusivity
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

  num_steps = 200
  dt = 1e3
[]

[Postprocessors]
  [./point]
    type = PointValue
    variable = c1
    point = '0.01 0 0'
  [../]
[]

[Outputs]
  interval = 100
  exodus = true
[]
