[GlobalParams]
  N = 23
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
  [./bounds_dummy]
  [../]
[]


[AuxKernels]
  [./temp_constant]
    type = ConstantAux
    variable = temp
    value = 2000
  [../]
[]


[Bounds]
  [./c_bounds1]
    type = VectorBoundsAux
    variable = bounds_dummy
    bounded_variable = 'c1 c22'
    lower = 0
  [../]
[]


[Materials]
  [./c1_diff]
    type = AtomicDiffusionCoef
    temp = temp
    D0 = 1.7e5
    Q = 2.3
    k = 8.617e-5
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
