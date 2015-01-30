# Concentration of c1,c2 -> 0
# Concentration of c3 -> 1/3

[GlobalParams]
  nucleation_conc_vars = 'c1 c2 c3 c4 c5'
[]


[Mesh]
  type = GeneratedMesh
  dim = 1
  xmax = 100
[]


[Variables]
  [./c_tot]
  [../]
  [./c1]
  [../]
  [./c2]
  [../]
  [./c3]
  [../]
  [./c4]
  [../]
  [./c5]
  [../]
[]


[Kernels]
  [./c_tot_source]
    type = VariableScaledSource
    variable = c_tot
    factor = 6.6e-8
    # factor = 1
    scaling_variable = 1
  [../]
  [./c_tot_time]
    type = TimeDerivative
    variable = c_tot
  [../]

  [./c1_source]
    type = VariableScaledSource
    variable = c1
    factor = 6.6e-8
    # factor = 1
    scaling_variable = 1
  [../]
  [./c1_time]
    type = TimeDerivative
    variable = c1
  [../]
  [./c1_nucleation]
    type = HomNucleation
    variable = c1
    m = 1
  [../]
  [./c1_diffusion]
    type = SpeciesDiffusion
    variable = c1
    m = 1
  [../]

  [./c2_time]
    type = TimeDerivative
    variable = c2
  [../]
  [./c2_nucleation]
    type = HomNucleation
    variable = c2
    m = 2
  [../]
  [./c2_diffusion]
    type = SpeciesDiffusion
    variable = c2
    m = 2
  [../]

  [./c3_time]
    type = TimeDerivative
    variable = c3
  [../]
  [./c3_nucleation]
    type = HomNucleation
    variable = c3
    m = 3
  [../]
  [./c3_diffusion]
    type = SpeciesDiffusion
    variable = c3
    m = 3
  [../]

  [./c4_time]
    type = TimeDerivative
    variable = c4
  [../]
  [./c4_nucleation]
    type = HomNucleation
    variable = c4
    m = 4
  [../]
  [./c4_diffusion]
    type = SpeciesDiffusion
    variable = c4
    m = 4
  [../]

  [./c5_time]
    type = TimeDerivative
    variable = c5
  [../]
  [./c5_nucleation]
    type = HomNucleation
    variable = c5
    m = 5
  [../]
  [./c5_diffusion]
    type = SpeciesDiffusion
    variable = c5
    m = 5
  [../]
[]


[Materials]
  [./coeffs]
    type = HomNucleationMaterial
    block = 0
    temp = 923
    diffusivity_multipliers = '1 0 0 0 0'
    c1_rx_coeffs = '84 20 12 15 18'
    # c1_rx_coeffs = '84  0  0  0  0'
    c2_rx_coeffs = ' 0  0  0  0  0'
    c3_rx_coeffs = ' 0  0  0  0  0'
    c4_rx_coeffs = ' 0  0  0  0  0'
    c5_rx_coeffs = ' 0  0  0  0  0'
    D0 = 1.7e7
    Q = 2.3
    k = 8.617e-5
    a = 0.5
    omega = 6
  [../]
[]


[Executioner]
  type = Transient

  # scheme = 'rk-2'

  solve_type = PJFNK

  # petsc_options = '-snes_ksp_ew'
  # petsc_options_iname = '-ksp_gmres_restart'
  # petsc_options_value = '101'

  # num_steps = 250
  end_time = 2e6
  dt = 1e3
[]


[Postprocessors]
  # [./c_tot]
  #   type = ElementAverageValue
  #   variable = c_tot
  # [../]
  [./c1]
    type = ElementAverageValue
    variable = c1
  [../]
  [./c2]
    type = ElementAverageValue
    variable = c2
  [../]
  [./c3]
    type = ElementAverageValue
    variable = c3
  [../]
  [./c4]
    type = ElementAverageValue
    variable = c4
  [../]
  [./c5]
    type = ElementAverageValue
    variable = c5
  [../]
  [./sum]
    type = SumOfPostprocessors
    postprocessors = 'c2 c3 c4 c5'
  [../]
[]

[Outputs]
  console = true
  exodus = true
  interval = 1
[]
