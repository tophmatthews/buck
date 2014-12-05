# Tests material model SolidSwellingUC for the calculation of swelling due to 
#  solid fission products
#
# The mesh is a cube with 7 blocks.

# Burnup    temp [K]  BUCK vol  EXCEL vol % diff
# 0.000E+00 9.000E+02 1.000E+00 1.000E+00 0.000E+00
# 1.000E-02 9.500E+02 1.015E+00 1.019E+00 3.637E-01
# 2.000E-02 1.000E+03 1.031E+00 1.035E+00 3.600E-01
# 3.000E-02 1.050E+03 1.048E+00 1.052E+00 3.561E-01
# 4.000E-02 1.100E+03 1.071E+00 1.075E+00 3.500E-01
# 5.000E-02 1.150E+03 1.107E+00 1.111E+00 3.373E-01
# 6.000E-02 1.200E+03 1.148E+00 1.156E+00 7.483E-01
# 7.000E-02 1.250E+03 1.190E+00 1.204E+00 1.168E+00
# 8.000E-02 1.300E+03 1.233E+00 1.253E+00 1.600E+00
# 9.000E-02 1.350E+03 1.278E+00 1.304E+00 2.045E+00
# 1.000E-01 1.400E+03 1.324E+00 1.358E+00 2.507E+00

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
    initial_condition = 900.0
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
  [./temp_ramp]
    type = PiecewiseLinear
    x = '0 100'
    y = '900 1400'
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
  [./temp]
    type = FunctionDirichletBC
    boundary = '1 2 3 4 5 6'
    function = temp_ramp
    variable = temp
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

  [./VSwelling]
    type = VSwellingUr
    block = '1 2 3 4 5 6 7'
    burnup = burnup
    temp = temp
    save_solid_swell = true
    save_gas_swell = true
    solid_factor = 0.0
    calculate_gas_swelling = true
    porosity = 0.04
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
  [./volume]
    type = VolumePostprocessor
    use_displaced_mesh = true
    execute_on = timestep
  [../]
  [./temp]
    type = ElementAverageValue
    blcok = 1
    variable = temp
  [../]
[]


[Outputs]
  file_base = gas_hight_out
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
