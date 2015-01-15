# This test is used to verify the VariableScaledSource with an AuxVariable
# The mesh is a 1x1x1 cube (single element).
#
# The integral gas content should equal the time step.

[Mesh]
  type = GeneratedMesh
  dim = 3
[]

[Variables]
  [./c]
  [../]
[]

[AuxVariables]
  [./auxvar]
  []
[]

[Kernels]
  [./c_time]
    type = TimeDerivative
    variable = c
  [../]
  [./source]
    type = VariableScaledSource
    variable = c
    scaling_variable = auxvar
    factor = 1
  [../]
[]

[AuxKernels]
  [./var_value]
    type = ConstantAux
    variable = auxvar
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
  [./var_average]
    type = ElementAverageValue
    variable = auxvar
  [../]
[]

[Outputs]
  exodus = true
  file_base = auxvar_out
  [./console]
    type = Console
    perf_log = true
    interval = 10
  [../]
[]