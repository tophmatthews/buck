# Tests ClusterPPAction at building postprocessors. Total_atoms should equal 100

[Mesh]
  type = GeneratedMesh
  dim = 1
[]

[Clusters]
  [./Nucleation]
    N = 3
  [../]
  [./Growth]
    N_min = 3
    N_max = 5
  [../]
  [./PPs]
    concentrations = true
    total_concentrations = true
    total_atoms = true
    N = 5
  [../]
[]

[Variables]
  [./c1]
    initial_condition = 100
  [../]
[]


[Kernels]
  [./c1_time]
    type = TimeDerivative
    variable = c1
  [../]
[]

[AuxVariables]
  [./temp]
    order = FIRST
    family = LAGRANGE
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
    temp = 1000
    D0 = 1.7e5
    Q = 2.3
    k = 8.617e-5
    factor = 1
    block = 0
  [../]
  [./cN_coeffs]
    type = HomNucleationMaterial
    block = 0
    c1_rx_coeffs = '1 10 0'
    omega = 3.0e4
    a = 0.5
    cluster_diffusion = false
    nucleation_conc_vars = 'c1 c2 c3'
  [../]
[]


[Executioner]
  type = Transient

  solve_type = PJFNK

  num_steps = 10
  dt = 1000
[]


[Outputs]
  console = true
  exodus = true
[]
