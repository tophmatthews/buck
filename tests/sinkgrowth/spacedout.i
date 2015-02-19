# Tests for SinkGrowth with log spaced groupings.
# Sum should always be 400
#
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+
# | time           | c1_conc        | c2_conc        | c3_conc        | c4_conc        | c5_conc        | sum            |
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+
# |   1.000000e+02 |   9.908314e-01 |   0.000000e+00 |   9.957639e-01 |   4.214605e-03 |   2.147220e-05 |   4.000000e+00 |
# |   2.000000e+02 |   9.816081e-01 |   0.000000e+00 |   9.915849e-01 |   8.351485e-03 |   6.362455e-05 |   4.000000e+00 |
# |   3.000000e+02 |   9.723358e-01 |   0.000000e+00 |   9.874625e-01 |   1.241178e-02 |   1.256786e-04 |   4.000000e+00 |
# |   4.000000e+02 |   9.630203e-01 |   0.000000e+00 |   9.833965e-01 |   1.639663e-02 |   2.068700e-04 |   4.000000e+00 |
# |   5.000000e+02 |   9.536671e-01 |   0.000000e+00 |   9.793864e-01 |   2.030718e-02 |   3.064487e-04 |   4.000000e+00 |
# |   6.000000e+02 |   9.442816e-01 |   0.000000e+00 |   9.754317e-01 |   2.414458e-02 |   4.236794e-04 |   4.000000e+00 |
# |   7.000000e+02 |   9.348690e-01 |   0.000000e+00 |   9.715322e-01 |   2.790998e-02 |   5.578416e-04 |   4.000000e+00 |
# |   8.000000e+02 |   9.254344e-01 |   0.000000e+00 |   9.676872e-01 |   3.160452e-02 |   7.082301e-04 |   4.000000e+00 |
# |   9.000000e+02 |   9.159826e-01 |   0.000000e+00 |   9.638965e-01 |   3.522935e-02 |   8.741551e-04 |   4.000000e+00 |
# |   1.000000e+03 |   9.065184e-01 |   0.000000e+00 |   9.601595e-01 |   3.878560e-02 |   1.054942e-03 |   4.000000e+00 |
# +----------------+----------------+----------------+----------------+----------------+----------------+----------------+

[GlobalParams]
  coupled_conc = 'c1 c2 c3 c4 c5'
  coupled_maxsize = '1 2 3 6.24264 20'
  N_nuc = 3
  temp = 1000
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
  [./c3]
    initial_condition = 1
  [../]
  [./c4]
  [../]
  [./c5]
  [../]
[]


[Kernels]
  [./c1_time]
    type = TimeDerivative
    variable = c1
  [../]
  [./c1_growth]
    type = SinkGrowth
    variable = c1
    g = 1
  [../]

  [./c2_time]
    type = TimeDerivative
    variable = c2
  [../]

  [./c3_time]
    type = TimeDerivative
    variable = c3
  [../]
  [./c3_growth]
    type = SinkGrowth
    variable = c3
    g = 3
  [../]

  [./c4_time]
    type = TimeDerivative
    variable = c4
  [../]
  [./c4_growth]
    type = SinkGrowth
    variable = c4
    g = 4
  [../]

  [./c5_time]
    type = TimeDerivative
    variable = c5
  [../]
  [./c5_growth]
    type = SinkGrowth
    variable = c5
    g = 5
  [../]
[]


[Materials]
  [./c1_diff]
    type = AtomicDiffusionCoef
    model = 1
    factor = 1
    block = 0
  [../]
[]


[Executioner]
  type = Transient

  solve_type = PJFNK

  num_steps = 10
  dt = 100
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
  [./c4_conc]
    type = ElementIntegralVariablePostprocessor
    variable = c4
  [../]
  [./c5_conc]
    type = ElementIntegralVariablePostprocessor
    variable = c5
  [../]
  [./sum]
    type = SumOfPostprocessors
    postprocessors = 'c1_conc c2_conc c3_conc c4_conc c5_conc'
    factors = '1 2 3 5.12132 13.62132'
  [../]
[]

[Outputs]
  console = true
  exodus = true
  interval = 1
[]
