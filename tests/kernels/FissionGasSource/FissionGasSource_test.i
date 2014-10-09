# This test is used to verify the fission gas source kernel
#
# The mesh is a 1x1x1 cube (single element).
# The fission rate is constant
# Yield is constant at 1
#
# The average gas content should equal the time step.

[Mesh]
  file = 1x1x1cube.e
[]

[Variables]
  [./gas]
    order = FIRST
    family = LAGRANGE
    initial_condition = 0
  [../]
[]


[AuxVariables]
  [./fission_rate]
    order = FIRST
    family = LAGRANGE
  []
[]


[Kernels]
  [./gas_time]
    type = TimeDerivative
    variable = gas
  [../]

  [./source]
    type = FGSource
    variable = gas
    fission_rate = fission_rate
    yield = 1
  [../]
[]


[AuxKernels]
  [./fissionrate]
    type = FissionRateAux
    variable = fission_rate
    value = 1
  [../]
[]

[Executioner]
  type = Transient

  solve_type = PJFNK

  petsc_options = '-snes_ksp_ew'
  petsc_options_iname = '-ksp_gmres_restart'
  petsc_options_value = '101'

  line_search = 'none'

  l_max_its = 100
  nl_max_its = 100
  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-10
  l_tol = 1e-5
  start_time = 0.0
  end_time = 100
  dt = 1
[]

# [Postprocessors]
#  [./Fission Gas]
#    type = ElementAverageValue
#    block = 1
#    variable = gas
#  [../]
# []

[Outputs]
  file_base = out
  output_initial = true
  csv = false
  interval = 10
  [./exodus]
    type = Exodus
    elemental_as_nodal = true
  [../]
  [./console]
    type = Console
    perf_log = true
    linear_residuals = true
  [../]
[]