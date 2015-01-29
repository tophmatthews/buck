# Concentration of c1,c2 -> 0
# Concentration of c3 -> 1/3

[GlobalParams]
  nucleation_conc_vars = 'c1 c2 c3'
[]


[Mesh]
  type = GeneratedMesh
  dim = 1
[]


[Variables]
  [./c1]
    initial_condition = 100
  [../]
  [./c2]
  [../]
  [./c3]
  [../]
[]


[Kernels]
  [./c1_time]
    type = TimeDerivative
    variable = c1
  [../]
  [./c1_nucleation]
    type = HomNucleation
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

  [./c3_time]
    type = TimeDerivative
    variable = c3
  [../]
  [./c3_nucleation]
    type = HomNucleation
    variable = c3
    m = 3
  [../]
[]


[Materials]
  [./coeffs]
    type = HomNucleationMaterial
    block = 0
    temp = 1000
    diffusivity_multipliers = '1 0 0'
    c1_rx_coeffs = '1 10 0'
    c2_rx_coeffs = '0 0 0'
    c3_rx_coeffs = '0 0 0'
    D0 = 1.7e5
    Q = 2.3
    k = 8.617e-5
    omega = 3.0e4
    a = 0.5
  [../]
[]


[Executioner]
  type = Transient

  solve_type = PJFNK

  num_steps = 10
  dt = 1000
[]


[Postprocessors]
  [./c1_num]
    type = ElementIntegralVariablePostprocessor
    variable = c1
  [../]
  [./c2_num]
    type = ElementIntegralVariablePostprocessor
    variable = c2
  [../]
  [./c3_num]
    type = ElementIntegralVariablePostprocessor
    variable = c3
  [../]
[]

[Outputs]
  console = true
  exodus = true
[]
