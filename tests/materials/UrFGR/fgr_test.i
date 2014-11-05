# This tests the fission gas release model taken from the Uranus fuel
# performence code

# burnup  Buck gen    Excel gen   % diff      Buck rel    Excel rel   % diff
# 0       0           0           0           0           0           0
# 0.01    4.1514E-04  4.1514E-04  7.1600E-06  0           0           0
# 0.02    8.3029E-04  8.3029E-04  4.8840E-06  1.0802E-04  1.0802E-04  1.8342E-05
# 0.03    1.2454E-03  1.2454E-03  3.2987E-05  3.1946E-04  3.1946E-04  8.9928E-06
# 0.04    1.6606E-03  1.6606E-03  7.1600E-06  4.7278E-04  4.7278E-04  7.6033E-06
# 0.05    2.0757E-03  2.0757E-03  1.6928E-05  6.0404E-04  6.0404E-04  3.4549E-06
# 0.06    2.4909E-03  2.4909E-03  7.1600E-06  9.6133E-04  9.6133E-04  3.6610E-06
# 0.07    2.9060E-03  2.9060E-03  1.0046E-05  1.1222E-03  1.1222E-03  2.6532E-05
# 0.08    3.3212E-03  3.3212E-03  7.1600E-06  1.2243E-03  1.2243E-03  4.0243E-05
# 0.09    3.7363E-03  3.7363E-03  6.2222E-06  1.3265E-03  1.3265E-03  2.1345E-05
# 0.10    4.1514E-03  4.1514E-03  7.1600E-06  1.4287E-03  1.4287E-03  5.1503E-06

[GlobalParams]
  density = 10000.0
  disp_x = disp_x
  disp_y = disp_y
  disp_z = disp_z
[]

[Mesh]
  file = patch.e
  displacements = 'disp_x disp_y disp_z'
[]

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
  [./disp_z]
  [../]

  [./temp]
    initial_condition = 1500.0
  [../]
[]

[AuxVariables]
  [./burnup]
  [../]
[]

[SolidMechanics]
  [./solid]
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
    # x = '0    50    51    70    71   100'
    # y = '1750 1750  1850  1850  1650 1650'
    x = '0     49    50    69    70    100'
    y = '1900  1900  2100  2100  1800  1800'
  [../]
[]

[BCs]
  [./bottom_fix_x]
    type = DirichletBC
    variable = disp_x
    boundary = 10
    value = 0.0
  [../]
  [./fix_y]
    type = DirichletBC
    variable = disp_y
    boundary = 9
    value = 0.0
  [../]
  [./fix_z]
    type = DirichletBC
    variable = disp_z
    boundary = 14
    value = 0.0
  [../]
  [./temp_change]
    type = FunctionDirichletBC
    variable = temp
    boundary = '14 10 9'
    function = temp_fcn
  [../]
[]

[Materials]
  [./mechUC]
    type = Elastic
    block = '1 2 3 4 5 6 7'
    youngs_modulus = 2.0
    poissons_ratio = .3
    thermal_expansion = 0
  [../]
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
    temp = temp
    burnup = burnup
    outputs = all
    fission_rate = 1e20
    center_temp = center_temp
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
