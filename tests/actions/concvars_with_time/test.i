# Tests nucleation of bubbles. Total should equal 1e5 at all times.
# Values should be exactly the same as tests/bubblenucleation/test.i
#
# +----------------+----------------+----------------+----------------+
# | time           | c1             | c2             | total          |
# +----------------+----------------+----------------+----------------+
# |   1.000000e+07 |   9.369567e+02 |   3.152164e+01 |   1.000000e+03 |
# |   2.000000e+07 |   8.813889e+02 |   5.930556e+01 |   1.000000e+03 |
# |   3.000000e+07 |   8.320415e+02 |   8.397927e+01 |   1.000000e+03 |
# |   4.000000e+07 |   7.879255e+02 |   1.060372e+02 |   1.000000e+03 |
# |   5.000000e+07 |   7.482511e+02 |   1.258744e+02 |   1.000000e+03 |
# |   6.000000e+07 |   7.123798e+02 |   1.438101e+02 |   1.000000e+03 |
# |   7.000000e+07 |   6.797898e+02 |   1.601051e+02 |   1.000000e+03 |
# |   8.000000e+07 |   6.500505e+02 |   1.749747e+02 |   1.000000e+03 |
# |   9.000000e+07 |   6.228038e+02 |   1.885981e+02 |   1.000000e+03 |
# |   1.000000e+08 |   5.977488e+02 |   2.011256e+02 |   1.000000e+03 |
# +----------------+----------------+----------------+----------------+

[GlobalParams]
  coupled_conc = 'c1 c2'
  coupled_rad = 'c1 c2'
  coupled_atoms = '1 2'
  coupled_widths = '1 2'
  temp = 1000
[]


[Mesh]
  type = GeneratedMesh
  dim = 1
[]


[Bubbles]
  [./Conc]
    N = 2
    s = 2
    c1_initial_condition = 1e3
    c2_initial_condition = 0
    initial_condition = 0
  [../]
[]


[Kernels]
  [./c1_nucleation]
    type = BubbleNucleation
    variable = c1
  [../]
  [./c2_nucleation]
    type = BubbleNucleation
    variable = c2
  [../]
[]


[Materials]
  [./diff]
    type = GasAtomDiffusivity
    model = 4
    factor = 1
    block = 0
  [../]
[]


[Executioner]
  type = Transient

  solve_type = PJFNK

  end_time = 1e8
  dt = 1e5
[]


[Postprocessors]
  [./c1]
    type = ElementAverageValue
    variable = c1
  [../]
  [./c2]
    type = ElementAverageValue
    variable = c2
  [../]
  [./total]
    type = SumOfPostprocessors
    postprocessors = 'c1 c2'
    factors = '1 2'
  [../]
[]


[Outputs]
  interval = 100
  exodus = true
[]
