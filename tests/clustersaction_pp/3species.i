# Tests ClusterPPAction at building postprocessors. Total_atoms should equal 100

[GlobalParams]
  N_nuc = 3
  N_max = 5
  G = 5
  log = false
  temp = temp
[]

[Mesh]
  type = GeneratedMesh
  dim = 1
[]

[Clusters]
  [./Nucleation]
  [../]
  [./Growth]
  [../]
  [./PPs]
    concentrations = 'exodus'
    total_concentrations = 'console'
    total_atoms = 'console exodus'
  [../]
[]

[ICs]
  [./c1]
    type = ConstantIC
    variable = c1
    value = 100
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
    model = 1
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
