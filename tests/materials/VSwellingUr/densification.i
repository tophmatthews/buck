# Tests material model SolidSwellingUC for the calculation of swelling due to 
#  solid fission products
#
# The mesh is a cube with 7 blocks.
#
# burnup  BUCK vol  EXCEL vol   % Diff
# 0       1.00000   1.00000     0.00000
# 0.01    0.97255   0.97242     0.01268
# 0.02    0.96734   0.96721     0.01318
# 0.03    0.96636   0.96623     0.01321
# 0.04    0.96617   0.96604     0.01320
# 0.05    0.96614   0.96601     0.01320
# 0.06    0.96613   0.96600     0.01320
# 0.07    0.96613   0.96600     0.01320
# 0.08    0.96613   0.96600     0.01320
# 0.09    0.96613   0.96600     0.01321
# 0.1     0.96613   0.96600     0.01321

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
    initial_condition = 500.0
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
  [./heat_ie]
    type = HeatConductionTimeDerivative
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
    youngs_modulus = 2.
    poissons_ratio = .3
    thermal_expansion = 0
  [../]

  [./VSwellingMC]
    type = VSwellingUr
    block = '1 2 3 4 5 6 7'
    burnup = burnup
    temp = temp
    save_solid_swell = false
    solid_factor = 0.
    calculate_gas_swelling = false
    save_densification = true
    total_densification = 0.034
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
  #num_steps = 50
  end_time = 100
  dt = 10

[]


[Postprocessors]
  [./burnup]
    type = ElementAverageValue
    block = 1
    variable = burnup
    execute_on = timestep
    use_displaced_mesh = true
  [../]
  [./volume]
    type = VolumePostprocessor
    use_displaced_mesh = true
  [../]
[]


[Outputs]
  file_base = densification_out
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
