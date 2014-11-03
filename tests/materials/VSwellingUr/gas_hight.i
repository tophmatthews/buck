# Tests material model SolidSwellingUC for the calculation of swelling due to 
#  solid fission products
#
# The mesh is a cube with 7 blocks.

# Burnup  temp [K]  BUCK vol  Excel vol   % diff
# 0.00    873       1.0000    1.0000      0.0000
# 0.01    923       1.0192    1.0190      0.0196
# 0.02    973       1.0388    1.0384      0.0387
# 0.03    1023      1.0597    1.0590      0.0582
# 0.04    1073      1.0856    1.0848      0.0819
# 0.05    1123      1.1235    1.1222      0.1174
# 0.06    1173      1.1751    1.1681      0.6023
# 0.07    1223      1.2289    1.2158      1.0701
# 0.08    1273      1.2848    1.2656      1.5149
# 0.09    1323      1.3428    1.3174      1.9320
# 0.10    1373      1.4030    1.3713      2.3163

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
    initial_condition = 873.0
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
    y = '873 1373'
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
