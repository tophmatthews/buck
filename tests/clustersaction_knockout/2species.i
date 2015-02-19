# This tests the KnockoutKernelsAction kernel.
#
# This should be exactly the same as tests/knockout/2species.i
#
# c1 -> 0.5
# c2 -> 0.25

[GlobalParams]
  coupled_conc = 'c1 c2'
  N_nuc = 2
  G = 2
  N_max = 2
  log = false
[]


[Mesh]
  type = GeneratedMesh
  dim = 1
[]


[Variables]
  [./dummy]
  [../]
[]

[Clusters]
  [./Knockout]
    parameter = .1
  [../]
[]

[ICs]
  [./c1]
    type = ConstantIC
    variable = c1
    value = 1
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
