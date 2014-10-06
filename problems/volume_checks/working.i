[GlobalParams]
  #density = 12267. # Initial density only
  disp_x = disp_x
  disp_y = disp_y
  disp_z = disp_z
[]

[Mesh]
  file = cube.e
  displacements = 'disp_x disp_y disp_z'
  #second_order = true
[]

[Variables]
  [./temp]
    initial_condition = 293.0
  [../]
  [./disp_x]
  [../]
  [./disp_y]
  [../]
  [./disp_z]
  [../]
[]

[AuxVariables]
  [./fission_rate]
  [../]
  [./burnup]
  [../]
#  [./gas_rel_3]
#    order = CONSTANT
#    family = MONOMIAL
#  [../]
  [./porosity]
    order = CONSTANT
    family = MONOMIAL
    initial_condition = 0.05
  [../]
  [./density]
    order = CONSTANT
    family = MONOMIAL
  [../]
#  [./deltav_v0_bd]
#    order = CONSTANT
#    family = MONOMIAL
#  [../]
#  [./deltav_v0_sl]
#    order = CONSTANT
#    family = MONOMIAL
#  [../]
#  [./deltav_v0_dn]
#    order = CONSTANT
#    family = MONOMIAL
#  [../]
#  [./deltav_v0_swe]
#    order = CONSTANT
#    family = MONOMIAL
#  [../]
#  [./fabrication_porosity]
#    order = CONSTANT
#    family = MONOMIAL
#    block = 1
#  [../]
#  [./gaseous_porosity]
#    order = CONSTANT
#    family = MONOMIAL
#  [../]
#  [./sintering_porosity]
#    order = CONSTANT
#    family = MONOMIAL
#  [../]
  [./stress_xx]      # stress aux variables are defined for output; this is a way to get integration point variables to the output file
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./stress_yy]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./stress_zz]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./vonmises]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./hydrostatic_stress]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./creep_strain_mag]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./creep_strain_hoop]
    order = CONSTANT
    family = MONOMIAL
  [../]
[]

[Functions]
  [./power_density]
    #type = ConstantFunction
    #value = 1 # [W/m3]
    type = PiecewiseLinear
    x = '0 1 1e10'
    y = '0 2.83e8 2.83e8' # [W/m3]
  [../]
[]

[SolidMechanics]
  [./solid]
    temp = temp
  [../]
[]

[Kernels]
  [./conduction]
    type = HeatConduction
    variable = temp
  [../]
  [./heat_source]
   type = NeutronHeatSource
   variable = temp 
   block = 1
   fission_rate = fission_rate
  [../]
  [./heat_ie]
    type = HeatConductionTimeDerivative
    variable = temp
  [../]
[]

[AuxKernels]
  [./hydrostatic_stress]
    block = 1
    type = MaterialTensorAux
    tensor = stress
    variable = hydrostatic_stress
    quantity = hydrostatic
    execute_on = timestep
  [../]
  [./stress_xx]               # computes stress components for output
    type = MaterialTensorAux
    tensor = stress
    variable = stress_xx
    index = 0
    execute_on = timestep     # for efficiency, only compute at the end of a timestep
  [../]
  [./stress_yy]
    type = MaterialTensorAux
    tensor = stress
    variable = stress_yy
    index = 1
    execute_on = timestep
  [../]
  [./stress_zz]
    type = MaterialTensorAux
    tensor = stress
    variable = stress_zz
    index = 2
    execute_on = timestep
  [../]
  [./vonmises]
    type = MaterialTensorAux
    tensor = stress
    variable = vonmises
    quantity = vonmises
    execute_on = timestep
  [../]
  [./fsn_rt]
    type = FissionRateFromPowerDensity
    block = 1
    variable = fission_rate
    function = power_density
    energy_per_fission = 3.2e-11 # [J/fsn]
  [../]
  [./burnup]
    type = BurnupAux
    variable = burnup
    block = 1
    fission_rate = fission_rate
    molecular_weight = .250 # [kg/mol]
    density = 12267.0 # Initial denstiy used for FIMA calcualtion [kg/m3]
  [../]
#  [./por]
#    type = PorosityAuxUO2
#    block = 1
#    variable = porosity
#    execute_on = residual
#  [../]
  [./rho]
    type = MaterialRealAux
    variable = density
    property = density
  [../]
#  [./gas_rel]
#    type = MaterialRealAux
#    variable = gas_rel_3
#    property = gas_rel_3
#  [../]
#  [./dvv0bd]
#    type = MaterialRealAux
#    variable = deltav_v0_bd
#    property = deltav_v0_bd
#  [../]
#  [./dvv0sl]
#    type = MaterialRealAux
#    variable = deltav_v0_sl
#    property = deltav_v0_sl
#  [../]
#  [./dvv0dn]
#    type = MaterialRealAux
#    variable = deltav_v0_dn
#    property = deltav_v0_dn
#  [../]
#  [./dvv0swe]
#    type = MaterialRealAux
#    variable = deltav_v0_swe
#    property = deltav_v0_swe
#  [../]
#  [./fab_por]
#    type = MaterialRealAux
#    variable = fabrication_porosity
#    property = fabrication_porosity
#  [../]
#  [./gaspor]
#    type = MaterialRealAux
#    variable = gaseous_porosity
#    property = gaseous_porosity
#  [../]
#  [./sinpor]
#    type = MaterialRealAux
#    variable = sintering_porosity
#    property = sintering_porosity
#  [../]
#  [./thcond]
#    type = MaterialRealAux
#    variable = thermal_conductivity
#    property = thermal_conductivity
#  [../]
[]

[BCs]
  [./heat_boundary]
    type = DirichletBC
    variable = temp
    boundary = '1 4'
    value = 1500.0
  [../]
  [./fix_x]
    type = DirichletBC
    variable = disp_x
    boundary = '2'
    value = 0.0
  [../]
  [./fix_y]
    type = DirichletBC
    variable = disp_y
    boundary = '4'
    value = 0.0
  [../]
  [./fix_z]
    type = DirichletBC
    variable = disp_z
    boundary = '6'
    value = 0.0
  [../]
[]

[Materials]
  [./fuel_thermal]
    type = ThermalUC
    burnup = burnup
    initial_porosity = 0.05
    porosity = porosity
    temp = temp
    block = 1
  [../]
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
    thermal_expansion = 1e-5
    model_thermal_expansion = false
    model_swelling = false
    model_gas_swelling = false
    model_creep = false
    calc_elastic_modulus = true
  [../]
  [./VSwellingUC]
    type = VSwellingUC
    block = 1
    burnup = burnup
    initial_density = 12667.0
  [../]
  [./fuel_density]
    type = Density
    block = 1
    density = 12267.0
  [../]
#  [./SifgrsUC]
#    type = SifgrsUC
#    block = 1
#    density = 12267  # initial density, [kg/m3]
#    burnup = burnup
#    fission_rate = fission_rate
#    temp = temp
#    initial_grain_radius = 25.0e-6
#    initial_porosity = 0.05
#    porosity = porosity
#    compute_swelling = true
#    total_densification = 0
#    compute_swelling = true
#  [../]
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
  dt = 1e6
  end_time = 1e7

[]

[Postprocessors]
  [./temperature (K)]
    type = NodalVariableValue
    nodeid = 5
    variable = temp
  [../]
  [./dt (s)]
    type = TimestepSize
  [../]
  [./burnup (FIMA)]
    type = ElementAverageValue
    block = 1
    variable = burnup
  [../]
#  [./fis_gas_generated]
#    type = ElementIntegralFisGasGeneratedSifgrs
#    variable = temp
#    block = 1
#  [../]
#  [./fis_gas_released]
#    type = ElementIntegralFisGasReleasedSifgrs
#    variable = temp
#    block = 1
#  [../]
#  [./fis_gas_grain]
#    type = ElementIntegralFisGasGrainSifgrs
#    variable = temp
#    block = 1
#    output = file
#  [../]
#  [./fis_gas_boundary]
#    type = ElementIntegralFisGasBoundarySifgrs
#    variable = temp
#    block = 1
#    output = file
#  [../]
#  [./local_gas_release]
#    type = ElementalVariableValue
#    elementid = 1
#    variable = gas_rel_3
#  [../]
  [./volume (m3)]
    type = VolumePostprocessor
    block = 1
    use_displaced_mesh = 1
  [../]
  [./mass (kg)]
    type = Mass
    block = 1
    use_displaced_mesh = 1
    variable = temp
  [../]

[]

[Outputs]
  file_base = working.i
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
