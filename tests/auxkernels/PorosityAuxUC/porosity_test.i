# Tests material model SolidSwellingUC for the calculation of swelling due to 
#  gaseous fission products
#
# The test is a single element unit cube with a fission rate of 1e20. The simulation is
#  run for 70 1e6 timesteps.

# The porosity should equal the (volume - 1 ) + 0.05
#
#| time           | burnup         | porosity_pp    | volume         |
#|   1.000000e+06 |   3.654370e-03 |   5.000000e-02 |   1.000000e+00 |
#|   2.000000e+06 |   7.308740e-03 |   5.000000e-02 |   1.000000e+00 |
#|   3.000000e+06 |   1.096311e-02 |   5.085066e-02 |   1.000851e+00 |
#|   4.000000e+06 |   1.461748e-02 |   5.293067e-02 |   1.002931e+00 |
#|   5.000000e+06 |   1.827185e-02 |   5.516473e-02 |   1.005165e+00 |
#|   6.000000e+06 |   2.192622e-02 |   5.749509e-02 |   1.007495e+00 |
#|   7.000000e+06 |   2.558059e-02 |   5.991290e-02 |   1.009913e+00 |

[GlobalParams]
  density = 12267.0
  initial_porosity = 0.05
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
  [./porosity]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[SolidMechanics]
  [./solid]
    disp_x = disp_x
    disp_y = disp_y
    disp_z = disp_z
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
  [./por_aux]
    type = PorosityAuxUC
    variable = porosity
    block = 1
    #initial_porosity = 0.0
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
  [./VSwelling]
    type = VSwellingUC
    block = 1
    burnup = burnup
    temp = temp
    calculate_gas_swelling = true
    solid_factor = 0
    save_gas_swell = true
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
    outputs = all
    disp_x = disp_x
    disp_y = disp_y
    disp_z = disp_z
  [../]
  [./zone_mat]
    type = ZonalUC
    block = 1
    temp = temp
    fission_rate = fission_rate
    burnup = burnup
    outputs = all
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
  num_steps = 70
  dt = 1e5

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
  [./porosity_pp]
    type = ElementAverageValue
    block = 1
    variable = porosity
  [../]
[]


[Outputs]
  file_base = porosity_test_out
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
