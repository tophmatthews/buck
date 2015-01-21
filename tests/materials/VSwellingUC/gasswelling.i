# Tests material model SolidSwellingUC for the calculation of swelling due to 
#  gaseous fission products
#
# The test is a single element unit cube with a fission rate of 1e20. The simulation is
#  run for 60 1e6 timesteps. The calculated volume should be the same as the Buck volume
#  EXCEPT in zone 1. The swelling should remain the same after zone 1 is crossed.
#
# The following is a comparison of BUCK to a excel hand calc:
#            
# burnup  zone  volume  total diff [%]
# 0.03  4 1.0130  1.012947  0.00279
# 0.06  4 1.0570  1.056598  0.03371
# 0.09  3 1.1186  1.117567  0.08860
# 0.12  3 1.2098  1.207461  0.19457
# 0.15  3 1.3082  1.304277  0.29717
# 0.18  3 1.4124    
# 0.21  1 1.5217    
# 0.24  1 1.5217    
# 0.27  1 1.5217    
# 0.30  1 1.5217      


[GlobalParams]
  density = 12267.0
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
    initial_condition = 1130.0
  [../]
[]

[Kernels]
  [./heat]
    type = HeatConduction
    variable = temp
  [../]
  [./heat_ie]
    type = HeatConductionTimeDerivative
    variable = temp
  [../]
[]

[AuxVariables]
  [./fission_rate]
  [../]
  [./burnup]
  [../]
[]

[SolidMechanics]
  [./solid]
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
    y = '0 .3'
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
[]

[Materials]
  [./mechUC]
    type = Elastic
    block = '1 2 3 4 5 6 7'
    youngs_modulus = 2.e11
    poissons_ratio = .3
    thermal_expansion = 0
  [../]
  [./VSwelling]
    type = VSwellingUC
    block = '1 2 3 4 5 6 7'
    burnup = burnup
    temp = temp
    calculate_gas_swelling = true
    solid_factor = 0
    total_densification = 0
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
  [./zone_mat]
    type = ZonalUC
    block = '1 2 3 4 5 6 7'
    temp = temp
    fission_rate = fission_rate
    burnup = burnup
    outputs = all
    nitrogen_fraction = 0.5
    frac_rel_zone1 = 0.7
    frac_rel_zone3 = 0.15
    frac_rel_zone4 = 0.1
    burnup_threshold = 0.001
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
  nl_rel_tol = 1e-4
  nl_abs_tol = 1e-4
  l_tol = 1e-5
  start_time = 0.0
  num_steps = 10
  dt = 10

[]


[Postprocessors]
  [./burnup]
    type = ElementAverageValue
    block = 1
    variable = burnup
  [../]
  [./volume]
    type = VolumePostprocessor
    use_displaced_mesh = true
  [../]
  [./zone]
    type = ElementAverageValue
    block = 1
    variable = zone
  [../]
  [./T2]
    type = ElementAverageValue
    block = 1
    variable = T2
  [../]
  [./T3]
    type = ElementAverageValue
    block = 1
    variable = T3
  [../]
  [./temp]
    type = ElementAverageValue
    block = 1
    variable = temp
  [../]
[]


[Outputs]
  file_base = gasswelling_out
  output_initial = false
  csv = false
  interval = 1
  print_linear_residuals = true
  print_perf_log = true
  [./exodus]
    type = Exodus
    elemental_as_nodal = true
  [../]
[]
