# Concentration of c1,c2 -> 0
# Concentration of c3 -> 1/3
# Sum should always be 100
#
# This should be the exact same as tests/homnucleation/3species.i
#
# +----------------+----------------+----------------+----------------+----------------+
# | time           | c1_conc        | c2_conc        | c3_conc        | sum            |
# +----------------+----------------+----------------+----------------+----------------+
# |   1.000000e+03 |   7.962168e-01 |   7.943038e-02 |   3.301497e+01 |   1.000000e+02 |
# |   2.000000e+03 |   6.493922e-02 |   8.583795e-03 |   3.330596e+01 |   1.000000e+02 |
# |   3.000000e+03 |   1.643290e-02 |   2.367890e-03 |   3.332628e+01 |   1.000000e+02 |
# |   4.000000e+03 |   7.141588e-03 |   1.063926e-03 |   3.333024e+01 |   1.000000e+02 |
# |   5.000000e+03 |   4.083686e-03 |   6.176921e-04 |   3.333156e+01 |   1.000000e+02 |
# |   6.000000e+03 |   2.721206e-03 |   4.148869e-04 |   3.333215e+01 |   1.000000e+02 |
# |   7.000000e+03 |   1.990640e-03 |   3.049028e-04 |   3.333247e+01 |   1.000000e+02 |
# |   8.000000e+03 |   1.548188e-03 |   2.378210e-04 |   3.333266e+01 |   1.000000e+02 |
# |   9.000000e+03 |   1.256503e-03 |   1.933892e-04 |   3.333279e+01 |   1.000000e+02 |
# |   1.000000e+04 |   1.051941e-03 |   1.621257e-04 |   3.333287e+01 |   1.000000e+02 |
# +----------------+----------------+----------------+----------------+----------------+

[GlobalParams]
  N_nuc = 3
[]

[Mesh]
  type = GeneratedMesh
  dim = 1
[]

[Clusters]
  [./Nucleation]
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
  [../]
[]


[Executioner]
  type = Transient

  solve_type = PJFNK

  num_steps = 10
  dt = 1000
[]


[Postprocessors]
  [./c1_conc]
    type = ElementIntegralVariablePostprocessor
    variable = c1
  [../]
  [./c2_conc]
    type = ElementIntegralVariablePostprocessor
    variable = c2
  [../]
  [./c3_conc]
    type = ElementIntegralVariablePostprocessor
    variable = c3
  [../]
  [./sum]
    type = SumOfPostprocessors
    postprocessors = 'c1_conc c2_conc c3_conc'
    factors = '1 2 3'
  [../]
[]

[Outputs]
  console = true
  exodus = true
[]
