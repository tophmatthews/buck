
[GlobalParams]
#  density = 12267.0
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
  [./total_porosity]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./density]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./P0_porosity]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./P1_porosity]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./P2_porosity]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./P3_porosity]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[Functions]
  [./top_pull]
    type = PiecewiseLinear
    x = '0 1e7'
    y = '0 0'
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
    variable = total_porosity
    function = porosity_ramp
  [../]
  [./density]
    type = MaterialRealAux
    property = density
    variable = density
  [../]
  [./PorosityAux]
    type = TotalPorosityAuxUC
    variable = total_porosity
  [../]

  [./P0_aux]
    type = MaterialRealAux
    property = P0_porosity
    variable = P0_porosity
  [../]
  [./P1_aux]
    type = MaterialRealAux
    property = P1_porosity
    variable = P1_porosity
  [../]
  [./P2_aux]
    type = MaterialRealAux
    property = P2_porosity
    variable = P2_porosity
  [../]
  [./P3_aux]
    type = MaterialRealAux
    property = P3_porosity
    variable = P3_porosity
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
    porosity = total_porosity
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
    density = 12200 # initial density
    disp_x = disp_x
    disp_y = disp_y
    disp_z = disp_z
  [../]
  [./BubblesUC]
    type = BubblesUC
    block = 1
    initial_P0_porosity = 0.01
    initial_P1_porosity = 0.01
    initial_P2_porosity = 0.01
    initial_P3_porosity = 0.01
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
  num_steps = 10
  dt = 2e6
  end_time = 1e7

[]


[Postprocessors]
  [./temperature (K)]
    type = ElementAverageValue
    block = 1
    variable = temp
  [../]
  [./total_porosity]
    type = ElementAverageValue
    block = 1
    variable = total_porosity
  [../]
  [./node_y]
    type = NodalVariableValue
    nodeid = 1
    variable = disp_y
  [../]
  [./node_x]
    type = NodalVariableValue
    nodeid = 4
    variable = disp_x
  [../]
  [./node_z]
    type = NodalVariableValue
    nodeid = 4
    variable = disp_z
  [../]
  [./volume (m3)]
    type = VolumePostprocessor
    use_displaced_mesh = 1
  [../]
  [./Mass (kg)]
    type = Mass
    variable = temp #empty variable
    use_displaced_mesh = 1
  [../]
  [./density]
    type = ElementAverageValue
    variable = density
  [../]
  [./P0_porosity]
    type = ElementAverageValue
    variable = P0_porosity
  [../]
  [./P1_porosity]
    type = ElementAverageValue
    variable = P1_porosity
  [../]
  [./P2_porosity]
    type = ElementAverageValue
    variable = P2_porosity
  [../]
  [./P3_porosity]
    type = ElementAverageValue
    variable = P3_porosity
  [../]
[]


[Outputs]
  file_base = test_out
  output_initial = true
  csv = true
  interval = 1
  [./exodus]
    type = Exodus
    elemental_as_nodal = true
  [../]
  [./console]
    type = Console
    perf_log = false
    linear_residuals = true
  [../]
[]
