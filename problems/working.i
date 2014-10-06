[GlobalParams]
  #density = 12267. # Initial density only
  disp_x = disp_x
  disp_y = disp_y
  disp_z = disp_z
[]

[Mesh]
  file = /Users/topher/projects/trunk/buck/meshes/kernel_new/d300_8th_x2.e
  #file = meshes/kernel/d300_8th_flat.e
  displacements = 'disp_x disp_y disp_z'
  #second_order = true
[]

[Variables]
  [./temp]
    initial_condition = 293.0
    #scaling = 1e5
    #order = SECOND
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

  [./gas_rel_3]
    order = CONSTANT
    family = MONOMIAL
  [../]

  [./porosity]
    order = CONSTANT
    family = MONOMIAL
    initial_condition = 0.05
  [../]
  [./density]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./deltav_v0_bd]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./deltav_v0_sl]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./deltav_v0_dn]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./deltav_v0_swe]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./fabrication_porosity]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./gaseous_porosity]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./sintering_porosity]
    order = CONSTANT
    family = MONOMIAL
  [../]
  [./thermal_conductivity]
    order = CONSTANT
    family = MONOMIAL
  [../]
  
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
  # Specify that we need solid mechanics (divergence of stress)
  [./solid]
    disp_x = disp_x
    disp_y = disp_y
    disp_z = disp_z
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

  [./por]
    type = PorosityAuxUO2
    block = 1
    variable = porosity
    execute_on = residual
  [../]
  [./rho]
    type = MaterialRealAux
    variable = density
    property = density
  [../]
  [./gas_rel]
    type = MaterialRealAux
    variable = gas_rel_3
    property = gas_rel_3
  [../]
  [./dvv0bd]
    type = MaterialRealAux
    variable = deltav_v0_bd
    property = deltav_v0_bd
  [../]
  [./dvv0sl]
    type = MaterialRealAux
    variable = deltav_v0_sl
    property = deltav_v0_sl
  [../]
  [./dvv0dn]
    type = MaterialRealAux
    variable = deltav_v0_dn
    property = deltav_v0_dn
  [../]
  [./dvv0swe]
    type = MaterialRealAux
    variable = deltav_v0_swe
    property = deltav_v0_swe
  [../]
  [./fabpor]
    type = MaterialRealAux
    variable = fabrication_porosity
    property = fabrication_porosity
  [../]
  [./gaspor]
    type = MaterialRealAux
    variable = gaseous_porosity
    property = gaseous_porosity
  [../]
  [./sinpor]
    type = MaterialRealAux
    variable = sintering_porosity
    property = sintering_porosity
  [../]
  [./thcond]
    type = MaterialRealAux
    variable = thermal_conductivity
    property = thermal_conductivity
  [../]
[]

[BCs]
  # kernel - 1(x) 3(y) 2(z) are symmetry planes, 4 is curved outside surface, 5 is flat outside surface
  # block - 1 2 are large flats, 3 4 5 6 are small flats

  [./heat_symmetry]
    type = NeumannBC
    variable = temp
    boundary = '1 3 2 11 13 12' 
    value = 0.0
  [../]

  [./heat_boundary]
    type = DirichletBC
    variable = temp
    boundary = '4 14'
    value = 1500.0
  [../]

  [./fix_x]
    type = DirichletBC
    variable = disp_x
    boundary = '1 11'
    value = 0.0
  [../]

  [./fix_y]
    type = DirichletBC
    variable = disp_y
    boundary = '3 13'
    value = 0.0
  [../]

  [./fix_z]
    type = DirichletBC
    variable = disp_z
    boundary = '2 12'
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

  [./fuel_elastic]
    type = MechUC
    block = 1
    disp_x = disp_x
    disp_y = disp_y
    disp_z = disp_z
    temp = temp
    porosity = porosity
    burnup = burnup
    calc_youngs = true
    youngs_modulus = 2.e11
    calc_poissons = true
    poissons_ratio = .3
    calc_alpha = true
    thermal_expansion = 0
    model_creep = true
    model_solid_swelling = true
    model_gas_swelling = true
  [../]

  [./SolidSwellingUC]
    type = SolidSwellingUC
    block = 1
    burnup = burnup
    initial_density = 12667
  [../]

  [./fuel_density]
    type = Density
    block = 1
    density = 12267.
  [../]

  [./SifgrsUC]
    type = SifgrsUC
    block = 1
    density = 12267  # initial density, [kg/m3]
    burnup = burnup
    fission_rate = fission_rate
    temp = temp
    initial_grain_radius = 25.0e-6
    initial_porosity = 0.05
    porosity = porosity
    compute_swelling = true
    total_densification = 0
    compute_swelling = true
  [../]
[]

[Preconditioning]
  [./SMP]
     type = SMP
     full = true
  [../]
[]

[Executioner]
  type = AdaptiveTransient
  #type = Transient
  petsc_options_iname = '-snes_type -snes_ls -ksp_gmres_restart -pc_type -pc_hypre_type -pc_hypre_boomeramg_max_iter'
  petsc_options_value = 'ls         basic    201                hypre    boomeramg      4'
  petsc_options = '-snes_mf_operator -ksp_monitor -snes_ksp_ew'

  nl_rel_tol = 1e-5
  nl_abs_tol = 1e-8
 
  l_tol = 1e-6
  l_max_its = 50

  start_time = 0.0
  #end_time = 8e6
  #end_time = 3.15e7 # 1 year
  end_time = 3.15e8 # 10 years
  #num_steps = 5
  dt = 1e6
  dtmax = 1e7
  optimal_iterations = 12
  iteration_window = 0.4
  linear_iteration_ratio = 100
[]

[Postprocessors]
  [./temperature (K)]
    type = NodalVariableValue
    nodeid = 5
    variable = temp
  [../]

  [./_dt (s)]
    type = PrintDT
  [../]

  [./burnup (FIMA)]
    type = ElementAverageValue
    block = 1
    variable = burnup
  [../]

  [./fis_gas_generated]
    type = ElementIntegralFisGasGeneratedSifgrs
    variable = temp
    block = 1
   [../]

   [./fis_gas_released]
     type = ElementIntegralFisGasReleasedSifgrs
     variable = temp
     block = 1
   [../]

   [./fis_gas_grain]
     type = ElementIntegralFisGasGrainSifgrs
     variable = temp
     block = 1
     output = file
   [../]

   [./fis_gas_boundary]
     type = ElementIntegralFisGasBoundarySifgrs
     variable = temp
     block = 1
     output = file
   [../]

  [./local_gas_release]
    type = ElementalVariableValue
    elementid = 1
    variable = gas_rel_3
  [../]

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

[Output]
  file_base = out
  interval = 1
  exodus = true
  perf_log = true
[]

[Debug]
  show_var_residual_norms = true
#  show_material_props = true
[]