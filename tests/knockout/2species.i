# This tests the Knockout kernel.
#
# The exact solution is based on
# dc1/dt = -k*c1^2 + b*c2
#
# where k is the rate constant to form a dimer
# and b is the re-solution rate
#
# After a long time, c2 = k/b * c1^2
#
# For this input, k=0.1 and b = 0.1
#
# c1 -> 0.5
# c2 -> 0.25

[GlobalParams]
  coupled_conc = 'c1 c2'
  coupled_conc_atoms = '1 2'
  fission_rate = 0.1
  N_nuc = 2
[]


[Mesh]
  type = GeneratedMesh
  dim = 1
[]


[Variables]
  [./c1]
    initial_condition = 1
  [../]
  [./c2]
  [../]
  [./dummy]
  [../]
[]


[Kernels]
  [./diff_dummy]
    type = Diffusion
    variable = dummy
  [../]
  [./dummy_time]
    type = TimeDerivative
    variable = dummy
  [../]
  
  [./c1_time]
    type = TimeDerivative
    variable = c1
  [../]
  [./c2_time]
    type = TimeDerivative
    variable = c2
  [../]

  [./c1_nucleation]
    type = HomNucleation
    variable = c1
    g = 1
  [../]
  [./c2_nucleation]
    type = HomNucleation
    variable = c2
    g = 2
  [../]

  [./c1_knockout]
    type = Knockout
    variable = c1
    g = 1
    parameter = 1
  [../]
  [./c2_knockout]
    type = Knockout
    variable = c2
    g = 2
    parameter = 1
  [../]
[]


[Materials]
  [./c1_diff]
    type = AtomicDiffusionCoef
    temp = 1
    D0 = 0.1
    Q = 0
    model = 0
    factor = 1
    block = 0
  [../]
  [./cN_coeffs]
    type = HomNucleationMaterial
    block = 0
    c1_rx_coeffs = '1 0'
    omega = 1
    a = 1
    cluster_diffusion = false
    N_nuc = 2
  [../]
[]

[BCs]
  [./dummy_bc]
    type = FunctionDirichletBC
    variable = dummy
    function = fcn
    boundary = left
  [../]
[]

[Functions]
  [./fcn]
    type = PiecewiseLinear
    x = '0 1e6'
    y = '0 1'
  [../]
[]



[Executioner]
  # type = Steady
  type = Transient

  solve_type = PJFNK

  num_steps = 3
  dt = 1e3
[]


[Postprocessors]
  [./c1_conc]
    type = ElementAverageValue
    variable = c1
  [../]
  [./c2_conc]
    type = ElementAverageValue
    variable = c2
  [../]
  [./sum]
    type = SumOfPostprocessors
    postprocessors = 'c1_conc c2_conc'
    factors = '1 2'
  [../]
[]

[Outputs]
  console = true
  exodus = true
[]
