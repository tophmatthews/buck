# This tests the grain_gas_flux postprocessor. Grain_gas_flux calculates the mass flux
#  across the right side of a 1D line. Grain_gas_total linearlly adds all the values as
#  time continues. Sum takes the sum of grain_gas_total and c1_num. Sum should equal close 
#  to the initial_condition of c1 = 100*L = 50.
#
# +----------------+----------------+----------------+----------------+----------------+
# | time           | c1_num         | grain_gas_flux | grain_gas_total| sum            |
# +----------------+----------------+----------------+----------------+----------------+
# |   2.000000e+02 |   4.038676e+01 |   2.540033e-02 |   9.324032e+00 |   4.971079e+01 |
# |   4.000000e+02 |   3.631963e+01 |   1.757182e-02 |   1.348016e+01 |   4.979980e+01 |
# |   6.000000e+02 |   3.321019e+01 |   1.424497e-02 |   1.662748e+01 |   4.983767e+01 |
# |   8.000000e+02 |   3.059347e+01 |   1.228568e-02 |   1.926652e+01 |   4.985998e+01 |
# |   1.000000e+03 |   2.829321e+01 |   1.094074e-02 |   2.158210e+01 |   4.987531e+01 |
# +----------------+----------------+----------------+----------------+----------------+

[GlobalParams]
  nucleation_conc_vars = 'c1'
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
    type = SpeciesDiffusion
    variable = c1
    m = 1
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
  [./coeffs]
    type = HomNucleationMaterial
    block = 0
    diffusivity_multipliers = '1'
    c1_rx_coeffs = '0'
    temp = 1200
    D0 = 1.7e5
    Q = 2.3
    k = 8.617e-5
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
