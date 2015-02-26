# This tests the grain_gas_flux postprocessor. Grain_gas_flux calculates the mass flux
#  across the right side of a 1D line. Grain_gas_total linearlly adds all the values as
#  time continues. Sum takes the sum of grain_gas_total and c1_num. Sum should equal close
#  to the initial_condition of c1 = 100*L = 50. It is not exact due to the BC of 0.
#
# +----------------+----------------+----------------+----------------+----------------+
# | time           | c1_num         | grain_gas_flux | grain_gas_total| sum            |
# +----------------+----------------+----------------+----------------+----------------+
# |   2.000000e+00 |   4.951253e+01 |   1.033049e-01 |   1.033049e-01 |   4.961584e+01 |
# |   4.000000e+00 |   4.935127e+01 |   8.698558e-02 |   2.935954e-01 |   4.964487e+01 |
# |   6.000000e+00 |   4.921390e+01 |   7.532367e-02 |   4.559046e-01 |   4.966980e+01 |
# |   8.000000e+00 |   4.909272e+01 |   6.676865e-02 |   5.979969e-01 |   4.969071e+01 |
# |   1.000000e+01 |   4.898350e+01 |   6.023091e-02 |   7.249965e-01 |   4.970850e+01 |
# +----------------+----------------+----------------+----------------+----------------+

[GlobalParams]
  order = SECOND
  family = LAGRANGE
[]


[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 10
  xmax = 1
  xmin = 0
  second_order = true
[]


[Variables]
  [./c1]
  [../]
  [./c2]
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
  [./c1_source]
    type = BodyForce
    variable = c1
    value = 1
  [../]

  [./c2_time]
    type = TimeDerivative
    variable = c2
  [../]
  [./c2_source]
    type = BodyForce
    variable = c2
    value = 1
  [../]
[]


[BCs]
  [./right1]
    type = VacuumBC
    variable = c1
    boundary = right
  [../]
[]


[Materials]
  [./diff]
    type = AtomicDiffusionCoef
    temp = 1
    D0 = 1
    Q = 0
    model = 0
    factor = 1
    block = 0
  [../]
[]


[Executioner]
  type = Transient

  solve_type = PJFNK

  num_steps = 50
  dt = 1
[]


[Postprocessors]
  [./c1_num]
    type = ElementIntegralVariablePostprocessor
    # type = ElementAverageValue
    variable = c1
  [../]
  [./c2_num]
    type = ElementIntegralVariablePostprocessor
    # type = ElementAverageValue
    variable = c2
  [../]
  [./grain_gas_flux]
    type = SideFluxIntegral
    diffusivity = atomic_diffusivity
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
