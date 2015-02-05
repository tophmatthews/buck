# This tests the grain_gas_flux postprocessor. Grain_gas_flux calculates the mass flux
#  across the right side of a 1D line. Grain_gas_total linearlly adds all the values as
#  time continues. Sum takes the sum of grain_gas_total and c1_num. Sum should equal close 
#  to the initial_condition of c1 = 100*L = 50.
#
# +----------------+----------------+----------------+----------------+----------------+
# | time           | c1_num         | grain_gas_flux | grain_gas_total| sum            |
# +----------------+----------------+----------------+----------------+----------------+
# |   2.000000e+02 |   4.896470e+01 |   2.956851e-03 |   7.763346e-01 |   4.974104e+01 |
# |   4.000000e+02 |   4.852598e+01 |   2.071211e-03 |   1.266018e+00 |   4.979200e+01 |
# |   6.000000e+02 |   4.818773e+01 |   1.653786e-03 |   1.634358e+00 |   4.982209e+01 |
# |   8.000000e+02 |   4.790344e+01 |   1.408331e-03 |   1.938727e+00 |   4.984217e+01 |
# |   1.000000e+03 |   4.765362e+01 |   1.245041e-03 |   2.203097e+00 |   4.985672e+01 |
# +----------------+----------------+----------------+----------------+----------------+

[GlobalParams]
  order = SECOND
  family = LAGRANGE
[]


[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 20
  xmax = 0.5
  xmin = 0
  second_order = true
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
  [./c1_diffusion]
    type = AtomicDiffusion
    variable = c1
  [../]
[]


[BCs]
  [./right1]
    type = DirichletBC
    variable = c1
    value = 0
    boundary = right
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

  num_steps = 50
  dt = 20
[]


[Postprocessors]
  [./c1_num]
    type = ElementIntegralVariablePostprocessor
    variable = c1
  [../]
  [./grain_gas_flux]
    type = GrainBoundaryGasFlux
    boundary = right
    variable = c1
  [../]
  [./grain_gas_total]
    type = TotalVariableValue
    value = grain_gas_flux
  [../]
  [./sum]
    type = SumOfPostprocessors
    postprocessors = 'grain_gas_total c1_num'
  [../]
[]


[Outputs]
  console = true
  exodus = true
  interval = 10
[]
