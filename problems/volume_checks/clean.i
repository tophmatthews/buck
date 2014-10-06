
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
    initial_condition = 200.0
  [../]
[]


[AuxVariables]
  [./porosity]
    order = CONSTANT
    family = MONOMIAL
    initial_condition = 0.05
  [../]
[]

[Functions]
  [./top_pull]
    type = PiecewiseLinear
    x = '0 1e7'
    y = '1 1'
  [../]
  [./temp_ramp]
    type = PiecewiseLinear
    x = '0.0 1e7'
    y = '300 1500'
  [../]
  [./porosity_ramp]
    type = PiecewiseLinear
    x = '0.0 1e7'
    y = '0.05 0.3'
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
  [./por]
    type = FunctionAux
    block = 1
    variable = porosity
    function = porosity_ramp
  [../]
[]


[BCs]
  [./top_pull]
    type = Pressure
    variable = disp_y
    component = 1
    boundary = 3
    factor = -1e9
    function = top_pull
  [../]
  [./bottom_fix_y]
    type = DirichletBC
    variable = disp_y
    boundary = 4
    value = 0.0
  [../]
  [./fix_y]
    type = DirichletBC
    variable = disp_z
    boundary = 6
    value = 0.0
  [../]
  [./fix_x]
    type = DirichletBC
    variable = disp_x
    boundary = 2
    value = 0.0
  [../]
  [./heatup]
     type = FunctionDirichletBC
     boundary = 1
     variable = temp
     function = temp_ramp
   [../]
[]


[Materials]

  [./mech]
    type = MechUC
    block = 1
    disp_x = disp_x
    disp_y = disp_y
    disp_z = disp_z
    temp = temp
    porosity = porosity
    initial_porosity = 0.05
    youngs_modulus = 2.e11
    poissons_ratio = .3
    thermal_expansion = 0
    model_thermal_expansion = false
    model_swelling = false
    model_gas_swelling = false
    model_creep = false
    calc_elastic_modulus = true
  [../]
  [./thermal]
    type = HeatConductionMaterial
    block = 1
    specific_heat = 1   
    thermal_conductivity = 100
  [../]
  [./density]
    type = Density
    block = 1
    disp_x = disp_x
    disp_y = disp_y
    disp_z = disp_z
    outputs = all
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
  nl_rel_tol = 1e-6
  nl_abs_tol = 1e-8
  l_tol = 1e-5
  start_time = 0.0
  dt = 2e6
  end_time = 1e7

[]


[Postprocessors]
  [./temperature (K)]
    type = ElementAverageValue
    block = 1
    variable = temp
  [../]
  [./porosity (/)]
    type = ElementAverageValue
    block = 1
    variable = porosity
  [../]
  [./node_y (dL_y/L0)]
    type = NodalVariableValue
    nodeid = 1
    variable = disp_y
  [../]
  [./node_x (dL_x/L0)]
    type = NodalVariableValue
    nodeid = 4
    variable = disp_x
  [../]
  [./Volume (m^3)]
    type = VolumePostprocessor
    use_displaced_mesh = 1
  [../]
  [./Mass (g)]
    type = Mass
    variable = temp # empty variable
  [../]
[]


[Outputs]
  file_base = clean
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
