# Tests material model MechUC for call to VSwellingUC for the calculation of
#  swelling due to solid fission products. The result should be the same as 
#  tests/materials/VSwellingUC/gas_swelling.i
#
# The test is a single element unit cube with a fission rate of 1e20. The simulation is
#  run for 60 1e6 timesteps. The calculated volume should be the same as the Buck volume
#  EXCEPT in zone 1. The swelling should remain the same after zone 1 is crossed.
#
# The following is a comparison of BUCK to a excel hand calc:
#            
# Step  zone  BUCK (m3)  Calc (m3)  diff [%]
# 10    4     1.02E+00   1.017636   6.9847E-04
# 20    4     1.08E+00   1.076851   6.1716E-03
# 30    3     1.18E+00   1.175542   1.9130E-02
# 40    3     1.29E+00   1.291589   3.3258E-02
# 50    3     1.42E+00   1.416426   4.6295E-02
# 60    1     1.44E+00    
# 70    1     1.44E+00    


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
    calculate_gas_swelling = true
    solid_factor = 0
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
  [../]
  [./zone_mat] # Required for VSwellingUC
    type = ZonalUC
    block = 1
    temp = temp
    fission_rate = fission_rate
    burnup = burnup
    testing = false
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
  nl_abs_tol = 1e-3
  l_tol = 1e-4
  start_time = 0.0
  num_steps = 70
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
[]

[Outputs]
  file_base = gas_swelling_out
  output_initial = false
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
