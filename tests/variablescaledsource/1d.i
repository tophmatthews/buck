# Tests VariableScaledsource in 1D
#
# c_total should equal time * 4/3 * Pi
#
# For last time step, c_total = 4.188790e+02

[Problem]
  coord_type = RSPHERICAL
[]

[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 10
  xmax = 1
  xmin = 0
[]

[Variables]
  [./c]
  [../]
[]

[AuxVariables]
  [./var]
  []
[]

[Kernels]
  [./c_time]
    type = TimeDerivative
    variable = c
  [../]
  [./c_source]
    type = VariableScaledSource
    variable = c
    scaling_variable = var
    factor = 1
  [../]
[]


[AuxKernels]
  [./var_value]
    type = ConstantAux
    variable = var
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

[Postprocessors]
  [./c_total]
    type = ElementIntegralVariablePostprocessor
    variable = c
  [../]
[]

[Outputs]
  exodus = true
  file_base = 1d_out
  print_perf_log = true
  [./console]
    type = Console
    interval = 10
  [../]
[]
