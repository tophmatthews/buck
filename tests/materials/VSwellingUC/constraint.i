# Tests material model SolidSwellingUC for the calculation of swelling due to 
#  solid fission products when constrained on 5 sides
#
# The test is a single element unit cube with a fission rate of 1e20. The simulation is
#  run for 50 1e6 timesteps. 
#
# Volume should equal (1-disp_x)
#
#
# The following is a comparison of BUCK to a excel hand calc:
#
# Step  burnup    disp_x    BUCK vol  EXCEL vol
# 10    0.000000  0.000000  1.000000  1.000000
# 20    0.036544  -0.011287 1.011287  1.011287
# 30    0.073087  -0.022523 1.022523  1.022523
# 40    0.109631  -0.033710 1.033710  1.033710
# 50    0.146175  -0.044847 1.044847  1.044847
# 60    0.182719  -0.055938 1.055938  1.055938


[GlobalParams]
  density = 12267.0
[]

[Mesh]
  file = cube.e
  displacements = 'disp_x disp_y disp_z'
[]

[Variables]

  [./disp_x]
    order = FIRST
    family = LAGRANGE
  [../]

  [./disp_y]
    order = FIRST
    family = LAGRANGE
  [../]

  [./disp_z]
    order = FIRST
    family = LAGRANGE
  [../]

  [./temp]
    order = FIRST
    family = LAGRANGE
    initial_condition = 500.0
  [../]

[]


[AuxVariables]

  [./fission_rate]
    order = FIRST
    family = LAGRANGE
  [../]

  [./burnup]
    order = FIRST
    family = LAGRANGE
  [../]

[]


[SolidMechanics]
  [./solid]
    disp_x = disp_x
    disp_y = disp_y
    disp_z = disp_z
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


[AuxKernels]

  [./fsnrt]
    type = FissionRateAux
    block = 1
    variable = fission_rate
    value = 1e20
  [../]

  [./burnup]
    type = BurnupAux
    variable = burnup
    block = 1
    fission_rate = fission_rate
  [../]

[]


[BCs]

  [./bottom_fix_y]
    type = DirichletBC
    variable = disp_y
    boundary = '4 3'
    value = 0.0
  [../]

  [./fix_z]
    type = DirichletBC
    variable = disp_z
    boundary = '5 6'
    value = 0.0
  [../]

  [./fix_x]
    type = DirichletBC
    variable = disp_x
    boundary = '1'
    value = 0.0
  [../]

[]


[Materials]

  [./mechUC]
    type = Elastic
    block = 1
    disp_x = disp_x
    disp_y = disp_y
    disp_z = disp_z
    youngs_modulus = 2.e11
    poissons_ratio = .3
    thermal_expansion = 0
  [../]

  [./VSwellingMX]
    type = VSwellingUC
    block = 1
    burnup = burnup
    temp = temp
    save_solid_swell = true
    solid_factor = 0.5
    calculate_gas_swelling = false
    total_densification = 0
  [../]

  [./thermal]
    type = HeatConductionMaterial
    block = 1
    specific_heat = 1.0
    thermal_conductivity = 100.
  [../]

  [./density]
    type = Density
    block = 1
    disp_x = disp_x
    disp_y = disp_y
    disp_z = disp_z
  [../]

  [./zone_mat] # required for VSwellingUC
    type = ZonalUC
    block = 1
    temp = temp
    fission_rate = fission_rate
    burnup = burnup
    testing = false
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
  num_steps = 50
  dt = 1e6

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
 [./disp_x]
   type = SideAverageValue
   variable = disp_x
   boundary = 2
  [../]
[]


[Outputs]
  file_base = constraint_out
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
