# Tests Bounds action. Should pass.

[GlobalParams]
  N = 7
  N_nuc = 5
  temp = temp
  block = 0
[]


[Mesh]
  type = GeneratedMesh
  dim = 1
  nx = 2
[]


[Clusters]
  [./Nucleation]
  [../]
  [./Growth]
  [../]
  [./PPs]
    concentrations = 'console'
  [../]
  [./Bounds]
  [../]
[]


[Variables]
  [./c1]
  [../]
[]


[Kernels]
  [./c1_time]
    type = TimeDerivative
    variable = c1
  [../]
  [./c1_source]
    type = VariableScaledSource
    variable = c1
    factor = 6.6e-8
  [../]
[]


[AuxVariables]
  [./temp]
  [../]
[]


[AuxKernels]
  [./temp_constant]
    type = ConstantAux
    variable = temp
    value = 2000
  [../]
[]


[Materials]
  [./c1_diff]
    type = AtomicDiffusionCoef
    model = 1
    factor = 1
  [../]
  [./cN_coeffs]
    type = HomNucleationMaterial
    c1_rx_coeffs = '84 20 12 15 18'
    omega = 3.0e4
    a = 0.5
    cluster_diffusion = false
  [../]
[]


[Executioner]
  type = Transient

  solve_type = PJFNK

  petsc_options_iname = '-snes_type'
  petsc_options_value = 'vinewtonrsls'

  dt = 1e1
  num_steps = 10
[]


[Outputs]
  console = true
[]
