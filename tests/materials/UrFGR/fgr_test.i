# This tests the fission gas release model taken from the Uranus fuel
# performence code

# burnup    center_temp BUCK gas_gen  EXCEL gas_gen % diff    BUCK gas_rel  EXCEL gas_rel   % diff
# 0.00E+00  1.50E+03    0.00E+00      0             0         0             0               0
# 1.00E-02  1.50E+03    4.15E-04      0.000415144   7.16E-06  5.12E-07      5.11654E-07     2.25E-06
# 2.00E-02  1.90E+03    8.30E-04      0.000830289   4.88E-06  6.56E-06      6.5577E-06      2.30E-06
# 3.00E-02  1.90E+03    1.25E-03      0.001245433   3.30E-05  1.51E-05      1.51285E-05     3.59E-07
# 4.00E-02  1.90E+03    1.66E-03      0.001660578   7.16E-06  2.71E-05      2.71222E-05     1.37E-05
# 5.00E-02  1.90E+03    2.08E-03      2.0757E-03    1.69E-05  4.25E-05      4.2462E-05      3.08E-06
# 6.00E-02  2.10E+03    2.49E-03      2.4909E-03    7.16E-06  8.12E-05      8.1203E-05      3.40E-06
# 7.00E-02  2.10E+03    2.91E-03      2.9060E-03    1.00E-05  1.10E-04      1.1006E-04      2.48E-05
# 8.00E-02  1.80E+03    3.32E-03      3.3212E-03    7.16E-06  1.21E-04      1.2135E-04      1.19E-05
# 9.00E-02  1.80E+03    3.74E-03      3.7363E-03    6.22E-06  1.34E-04      1.3400E-04      6.08E-06
# 1.00E-01  1.80E+03    4.15E-03      4.1514E-03    7.16E-06  1.48E-04      1.4799E-04      1.93E-05

[GlobalParams]
  density = 10000.0
[]

[Mesh]
  file = patch.e
[]

[Variables]
  [./temp]
    initial_condition = 1500.0
  [../]
[]

[AuxVariables]
  [./burnup]
  [../]
[]

[Kernels]
  [./heat]
    type = HeatConduction
    variable = temp
  [../]
[]

[AuxKernels]
  [./burnup]
    type = FunctionAux
    variable = burnup
    block = '1 2 3 4 5 6 7'
    function = burnup_fcn
  [../]
[]

[Functions]
  [./burnup_fcn]
    type = PiecewiseLinear
    x = '0 100'
    y = '0 .1'
  [../]
  [./temp_fcn]
    type = PiecewiseLinear
    x = '0     49    50    69    70    100'
    y = '1900  1900  2100  2100  1800  1800'
  [../]
[]

[BCs]
  [./temp_change]
    type = FunctionDirichletBC
    variable = temp
    boundary = '14 10 9'
    function = temp_fcn
  [../]
[]

[Materials]
  [./thermal]
    type = HeatConductionMaterial
    block = '1 2 3 4 5 6 7'
    specific_heat = 1.0
    thermal_conductivity = 100.
  [../]
  [./density]
    type = Density
    block = '1 2 3 4 5 6 7'
  [../]
  [./fgr]
    type = UrFGR
    block = '1 2 3 4 5 6 7'
    burnup = burnup
    outputs = all
    fission_rate = 1e20
    center_temp = center_temp
    testing = true
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
  dt = 10
[]


[Postprocessors]
  [./burnup]
    type = ElementAverageValue
    block = 1
    variable = burnup
  [../]
  [./gas_gen]
    type = ElementAverageValue
    block = '1 2 3 4 5 6 7'
    variable = gas_gen
  [../]
  [./gas_rel]
    type = ElementAverageValue
    block = '1 2 3 4 5 6 7'
    variable = gas_rel
  [../]
  [./center_temp]
    type = ElementExtremeValue
    block = '1 2 3 4 5 6 7'
    variable = temp
    execute_on = timestep_begin
  [../]
[]


[Outputs]
  file_base = fgr_test_out
  output_initial = true
  csv = false
  interval = 1
  print_linear_residuals = true
  print_perf_log = true
  [./exodus]
    type = Exodus
    elemental_as_nodal = true
  [../]
[]
