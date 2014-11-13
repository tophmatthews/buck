# Tests material model SolidSwellingUC for the calculation of swelling due to 
#  solid fission products in RZ geometry
#
# The volume at the final timestep should be 
# ( Pi(3.14159) + solid_factor(0.5) * burnup(0.1) ) = 3.29867

[GlobalParams]
  density = 10000.0
[]

[Problem]
  coord_type = RZ
[]

[Mesh]
  file = square.e
  displacements = 'disp_x disp_y'
[]

[Variables]
  [./disp_x]
  [../]
  [./disp_y]
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
  [./burnup]
  [../]
[]

[SolidMechanics]
  # Specify that we need solid mechanics (divergence of stress)
  [./solid]
    disp_r = disp_x
    disp_z = disp_y
    temp = temp
  [../]
[]

[AuxKernels]
  [./burnup]
    type = FunctionAux
    variable = burnup
    block = 1
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
  [./center_fix_]
    type = DirichletBC
    variable = disp_x
    boundary = left
    value = 0.0
  [../]
[]

[Materials]
  [./mechUC]
    type = Elastic
    block = 1
    disp_r = disp_x
    disp_z = disp_y
    youngs_modulus = 2.0
    poissons_ratio = .3
    thermal_expansion = 0
  [../]
  [./VSwelling]
    type = VSwellingUC
    block = 1
    burnup = burnup
    temp = temp
    calculate_gas_swelling = false
    solid_factor = 0.5
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
    disp_r = disp_x
    disp_z = disp_y
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
  nl_rel_tol = 1e-5
  nl_abs_tol = 1e-8
  l_tol = 1e-6
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
[]


[Outputs]
  file_base = rz_out
  output_initial = false
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
