## Tests material model CreepUC for the calculation of thermal and irradiation creep
#
#  The test is a single element unit cube which is pulled in the y direction
#  with a pressure of 5.0e7 Pa. The temperature is held at 1350 K, while the fission
#  rate is held at 1e20 fissions/sec. The simulation is for 10 time steps of 1e4 dt.
#
#  The following compares the analytical values to BUCK's calculated values:
#
#      disp_y:		
# step BUCK       analytical  % diff
#  1   2.577E-04  2.577E-04   1.289E-02
#  2   2.654E-04  2.654E-04   1.327E-02
#  3   2.731E-04  2.731E-04   1.367E-02
#  4   2.808E-04  2.808E-04	  1.405E-02
#  5   2.885E-04  2.885E-04   1.444E-02
#  6   2.962E-04  2.962E-04   1.481E-02
#  7   3.039E-04  3.039E-04   1.519E-02
#  8   3.116E-04  3.116E-04   1.559E-02
#  9   3.193E-04  3.193E-04   1.597E-02
#  10  3.270E-04  3.270E-04   1.636E-02


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
    initial_condition = 1350.0
  [../]

[]


[AuxVariables]

  [./fission_rate]
    order = FIRST
    family = LAGRANGE
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
    y = '1350 1350'
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

[]


[BCs]

  [./top_pull]
    type = Pressure
    variable = disp_y
    component = 1
    boundary = 3
    factor = -0.5e8
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
    boundary = '5'
    value = 0.0
  [../]

  [./fix_x]
    type = DirichletBC
    variable = disp_x
    boundary = '1'
    value = 0.0
  [../]

  [./heatup]
     type = FunctionDirichletBC
     boundary = '1 2'
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
    fission_rate = fission_rate
    initial_porosity = 0.05
    youngs_modulus = 2.e11
    poissons_ratio = .3
    thermal_expansion = 0
    model_thermal_expansion = false
    model_swelling = false
    model_gas_swelling = false
    model_creep = true
    calc_elastic_modulus = false
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
  num_steps = 10
  dt = 10000

[]


#[Postprocessors]
#  [./temperature (K)]
#    type = ElementAverageValue
#    block = 1
#    variable = temp
#  [../]
#  [./node_y]
#    type = NodalVariableValue
#    nodeid = 2
#    variable = disp_y
#  [../]
#  [./node_x]
#    type = NodalVariableValue
#    nodeid = 2
#    variable = disp_x
#  [../]
#  [./node_z]
#    type = NodalVariableValue
#    nodeid = 2
#    variable = disp_z
#  [../]
#[]


[Outputs]
  file_base = creep_out
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
