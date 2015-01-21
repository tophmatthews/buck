# Tests material model MechUC for call to VSwellingUC for the calculation of
#  swelling due to solid fission products
#
# The test is a single element unit cube with a fission rate of 1e20. The simulation is
#  run for 50 1e6 timesteps. The accuracy of the simulation diverges at increasing burnup,
#  but stays below a percent up to 20% FIMA.
#
# V = (1 + solid_swelling * Bu [FIMA]) * V0
#
#
# The following is a comparison of BUCK to a excel hand calc:
#
# Burnup  Volume                % diff
#         Buck     Analytical	
# 0.0365  1.0183   1.0183       5.06E-04
# 0.0731  1.0366   1.0365       1.09E-03
# 0.1096  1.0548   1.0548       1.56E-03
# 0.1462  1.0731   1.0731       2.01E-03
# 0.1827  1.0914   1.0914       2.54E-03


[GlobalParams]
  density = 12267.0
  disp_x = disp_x
  disp_y = disp_y
  disp_z = disp_z
[]

[Mesh]
  file = cube.e
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
    initial_condition = 500.0
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
    order = FIRST
    family = LAGRANGE
  [../]
  [./burnup]
    order = FIRST
    family = LAGRANGE
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

[SolidMechanics]
  [./solid]
  [../]
[]

[BCs]
  [./bottom_fix_y]
    type = DirichletBC
    variable = disp_y
    boundary = 4
    value = 0.0
  [../]
  [./fix_z]
    type = DirichletBC
    variable = disp_z
    boundary = '5'
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
  [./mech]
    type = MechUC
    block = 1
    temp = temp
    porosity = 0
    fission_rate = 0
    youngs_modulus = 2.e11
    poissons_ratio = .3
    thermal_expansion = 0
    calc_elastic_modulus = false
    model_creep = false
    calc_alpha = false
    model_swelling = true
  [../]
  [./VSwellingUC]
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
[]


[Outputs]
  file_base = solid_swelling_out
  output_initial = true
  csv = false
  interval = 10
  print_linear_residuals = true
  print_perf_log = true
  [./exodus]
    type = Exodus
    elemental_as_nodal = true
  [../]
[]
