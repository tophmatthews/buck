CDF   
   
      
len_string     !   len_line   Q   four      	time_step          len_name   !   num_dim       	num_nodes         num_elem      
num_el_blk        num_node_sets         num_side_sets         num_el_in_blk1        num_nod_per_el1       num_side_ss1      num_side_ss2      num_nod_ns1       num_nod_ns2       num_nod_var       num_glo_var       num_info  �         api_version       @�
=   version       @�
=   floating_point_word_size            	file_size               int64_status             title         3species_out.e     maximum_name_length                    
time_whole                            �    	eb_status                             x   eb_prop1               name      ID              |   	ns_status         	                    �   ns_prop1      	         name      ID              �   	ss_status         
                    �   ss_prop1      
         name      ID              �   coordx                            �   coordy                            �   coordz                            �   eb_names                       $      �   ns_names      	                 D      �   ss_names      
                 D      	8   
coor_names                         d      	|   node_num_map                          	�   connect1                  	elem_type         EDGE2               	�   elem_num_map                          	�   elem_ss1                          	�   side_ss1                          	�   elem_ss2                          	�   side_ss2                          
    node_ns1                          
   node_ns2                          
   vals_nod_var1                                �   vals_nod_var2                                �   vals_nod_var3                                �(   name_nod_var                       d      
   name_glo_var                       �      
p   vals_glo_var                              �8   info_records                      �      
�                                         ?�                                                                          left                             right                              left                             right                                                                                                                                                                   c1                               c2                               c3                                c1_conc                          c2_conc                          c3_conc                          sum                              ####################                                                             # Created by MOOSE #                                                             ####################                                                             ### Command Line Arguments ###                                                   -i                                                                               3species.i                                                                                                                                                        ### Version Info ###                                                             Framework Information:                                                           MOOSE version:           git commit a18a738 on 2015-02-09                        PETSc Version:           3.5.1                                                   Current Time:            Fri Feb 13 14:22:33 2015                                Executable Timestamp:    Fri Feb 13 14:21:11 2015                                                                                                                                                                                                  ### Input File ###                                                                                                                                                []                                                                                 initial_from_file_timestep     = 2                                               initial_from_file_var          = INVALID                                         block                          = INVALID                                         coord_type                     = XYZ                                             fe_cache                       = 0                                               kernel_coverage_check          = 1                                               name                           = 'MOOSE Problem'                                 rz_coord_axis                  = Y                                               type                           = FEProblem                                       use_legacy_uo_aux_computation  = INVALID                                         use_legacy_uo_initialization   = INVALID                                         element_order                  = AUTO                                            order                          = AUTO                                            side_order                     = AUTO                                            active_bcs                     = INVALID                                         active_kernels                 = INVALID                                         inactive_bcs                   = INVALID                                         inactive_kernels               = INVALID                                         start                          = 0                                               dimNearNullSpace               = 0                                               dimNullSpace                   = 0                                               error_on_jacobian_nonzero_reallocation = 0                                       long_name                      =                                                 solve                          = 1                                               use_nonlinear                  = 1                                             []                                                                                                                                                                [Clusters]                                                                                                                                                          [./Nucleation]                                                                     N_nuc                        = 3                                                 transient                    = 1                                                 use_displaced_mesh           = 0                                                 var_name_base                = c                                                 family                       = LAGRANGE                                          order                        = FIRST                                             scaling                      = 1                                               [../]                                                                          []                                                                                                                                                                [Executioner]                                                                      type                           = Transient                                       l_abs_step_tol                 = -1                                              l_max_its                      = 10000                                           l_tol                          = 1e-05                                           line_search                    = default                                         nl_abs_step_tol                = 1e-50                                           nl_abs_tol                     = 1e-50                                           nl_max_funcs                   = 10000                                           nl_max_its                     = 50                                              nl_rel_step_tol                = 1e-50                                           nl_rel_tol                     = 1e-08                                           no_fe_reinit                   = 0                                               petsc_options                  = INVALID                                         petsc_options_iname            = INVALID                                         petsc_options_value            = INVALID                                         solve_type                     = PJFNK                                           abort_on_solve_fail            = 0                                               dt                             = 1000                                            dtmax                          = 1e+30                                           dtmin                          = 2e-14                                           end_time                       = 1e+30                                           long_name                      = Executioner                                     n_startup_steps                = 0                                               num_steps                      = 10                                              picard_abs_tol                 = 1e-50                                           picard_max_its                 = 1                                               picard_rel_tol                 = 1e-08                                           predictor_scale                = INVALID                                         reset_dt                       = 0                                               restart_file_base              =                                                 scheme                         = implicit-euler                                  splitting                      = INVALID                                         ss_check_tol                   = 1e-08                                           ss_tmin                        = 0                                               start_time                     = 0                                               time_period_ends               = INVALID                                         time_period_starts             = INVALID                                         time_periods                   = INVALID                                         timestep_tolerance             = 2e-14                                           trans_ss_check                 = 0                                               use_multiapp_dt                = 0                                               verbose                        = 0                                             []                                                                                                                                                                [Executioner]                                                                      _fe_problem                    = 0x7fb3aa018200                                []                                                                                                                                                                [GlobalParams]                                                                     N_nuc                          = 3                                             []                                                                                                                                                                [Kernels]                                                                                                                                                           [./c1_time]                                                                        type                         = TimeDerivative                                    block                        = INVALID                                           diag_save_in                 = INVALID                                           long_name                    = Kernels/c1_time                                   lumping                      = 0                                                 save_in                      = INVALID                                           seed                         = 0                                                 use_displaced_mesh           = 0                                                 variable                     = c1                                              [../]                                                                          []                                                                                                                                                                [Materials]                                                                                                                                                         [./c1_diff]                                                                        type                         = AtomicDiffusionCoef                               D0                           = INVALID                                           Q                            = INVALID                                           R                            = 8.31446                                           block                        = 0                                                 boundary                     = INVALID                                           factor                       = 1                                                 long_name                    = Materials/c1_diff                                 model                        = 1                                                 output_properties            = INVALID                                           outputs                      = none                                              temp                         =                                                   use_displaced_mesh           = 0                                               [../]                                                                                                                                                             [./cN_coeffs]                                                                      type                         = HomNucleationMaterial                             N_nuc                        = 3                                                 a                            = 0.5                                               block                        = 0                                                 boundary                     = INVALID                                           c1_rx_coeffs                 = '1 10 0'                                          c2_rx_coeffs                 = INVALID                                           c3_rx_coeffs                 = INVALID                                           c4_rx_coeffs                 = INVALID                                           c5_rx_coeffs                 = INVALID                                           c6_rx_coeffs                 = INVALID                                           c7_rx_coeffs                 = INVALID                                           c8_rx_coeffs                 = INVALID                                           c9_rx_coeffs                 = INVALID                                           cluster_diffusion            = 0                                                 diffusivity_multipliers      = INVALID                                           long_name                    = Materials/cN_coeffs                               omega                        = 30000                                             output_properties            = INVALID                                           outputs                      = none                                              use_displaced_mesh           = 0                                               [../]                                                                          []                                                                                                                                                                [Mesh]                                                                             displacements                  = INVALID                                         block_id                       = INVALID                                         block_name                     = INVALID                                         boundary_id                    = INVALID                                         boundary_name                  = INVALID                                         construct_side_list_from_node_list = 0                                           ghosted_boundaries             = INVALID                                         ghosted_boundaries_inflation   = INVALID                                         patch_size                     = 40                                              second_order                   = 0                                               type                           = GeneratedMesh                                   uniform_refine                 = 0                                               centroid_partitioner_direction = INVALID                                         dim                            = 1                                               distribution                   = DEFAULT                                         elem_type                      = INVALID                                         long_name                      = Mesh                                            nemesis                        = 0                                               nx                             = 1                                               ny                             = 1                                               nz                             = 1                                               partitioner                    = default                                         patch_update_strategy          = never                                           xmax                           = 1                                               xmin                           = 0                                               ymax                           = 1                                               ymin                           = 0                                               zmax                           = 1                                               zmin                           = 0                                             []                                                                                                                                                                [Outputs]                                                                          checkpoint                     = 0                                               color                          = 1                                               console                        = 1                                               csv                            = 0                                               dofmap                         = 0                                               exodus                         = 1                                               file_base                      = INVALID                                         gmv                            = 0                                               gnuplot                        = 0                                               hide                           = INVALID                                         interval                       = 1                                               nemesis                        = 0                                               output_final                   = 0                                               output_if_base_contains        = INVALID                                         output_initial                 = 0                                               output_intermediate            = 1                                               output_on                      = 'timestep_end timestep_end timestep_end'        output_timestep_end            = 1                                               print_linear_residuals         = 0                                               print_perf_log                 = 0                                               show                           = INVALID                                         solution_history               = 0                                               sync_times                     =                                                 tecplot                        = 0                                               vtk                            = 0                                               xda                            = 0                                               xdr                            = 0                                                                                                                                [./console]                                                                        type                         = Console                                           additional_output_on         = INVALID                                           all_variable_norms           = 0                                                 append_displaced             = 0                                                 append_restart               = 0                                                 end_time                     = INVALID                                           file_base                    = INVALID                                           fit_mode                     = ENVIRONMENT                                       hide                         = INVALID                                           interval                     = 1                                                 linear_residual_dt_divisor   = 1000                                              linear_residual_end_time     = INVALID                                           linear_residual_start_time   = INVALID                                           linear_residuals             = 0                                                 long_name                    = Outputs/console                                   max_rows                     = 15                                                nonlinear_residual_dt_divisor = 1000                                             nonlinear_residual_end_time  = INVALID                                           nonlinear_residual_start_time = INVALID                                          nonlinear_residuals          = 0                                                 outlier_multiplier           = '0.8 2'                                           outlier_variable_norms       = 1                                                 output_elemental_variables   = 1                                                 output_failed                = 0                                                 output_file                  = 0                                                 output_final                 = 0                                                 output_if_base_contains      =                                                   output_initial               = 0                                                 output_input                 = 1                                                 output_input_on              = INVALID                                           output_intermediate          = 1                                                 output_linear                = 0                                                 output_nodal_variables       = 1                                                 output_nonlinear             = 0                                                 output_on                    = 'timestep_end nonlinear failed'                   output_postprocessors        = 1                                                 output_postprocessors_on     = timestep_end                                      output_scalar_variables      = 1                                                 output_scalars_on            = timestep_end                                      output_screen                = 1                                                 output_system_information    = 1                                                 output_system_information_on = initial                                           output_timestep_end          = 1                                                 output_vector_postprocessors = 1                                                 output_vector_postprocessors_on = INVALID                                        padding                      = 4                                                 perf_header                  = INVALID                                           perf_log                     = 0                                                 scientific_time              = 0                                                 setup_log                    = INVALID                                           setup_log_early              = 0                                                 show                         = INVALID                                           show_multiapp_name           = 0                                                 solve_log                    = INVALID                                           start_time                   = INVALID                                           sync_only                    = 0                                                 sync_times                   =                                                   time_precision               = INVALID                                           time_tolerance               = 1e-14                                             use_displaced                = 0                                                 verbose                      = 0                                               [../]                                                                                                                                                             [./exodus]                                                                         type                         = Exodus                                            additional_output_on         = INVALID                                           append_displaced             = 0                                                 append_oversample            = 0                                                 elemental_as_nodal           = 0                                                 end_time                     = INVALID                                           file                         = INVALID                                           file_base                    = INVALID                                           hide                         = INVALID                                           interval                     = 1                                                 linear_residual_dt_divisor   = 1000                                              linear_residual_end_time     = INVALID                                           linear_residual_start_time   = INVALID                                           linear_residuals             = 0                                                 long_name                    = Outputs/exodus                                    nonlinear_residual_dt_divisor = 1000                                             nonlinear_residual_end_time  = INVALID                                           nonlinear_residual_start_time = INVALID                                          nonlinear_residuals          = 0                                                 output_elemental_on          = INVALID                                           output_elemental_variables   = 1                                                 output_failed                = 0                                                 output_final                 = 0                                                 output_if_base_contains      =                                                   output_initial               = 0                                                 output_input                 = 1                                                 output_input_on              = initial                                           output_intermediate          = 1                                                 output_linear                = 0                                                 output_material_properties   = 0                                                 output_nodal_on              = INVALID                                           output_nodal_variables       = 1                                                 output_nonlinear             = 0                                                 output_on                    = 'timestep_end timestep_end timestep_end'          output_postprocessors        = 1                                                 output_postprocessors_on     = INVALID                                           output_scalar_variables      = 1                                                 output_scalars_on            = INVALID                                           output_system_information    = 1                                                 output_timestep_end          = 1                                                 output_vector_postprocessors = 1                                                 oversample                   = 0                                                 padding                      = 3                                                 position                     = INVALID                                           refinements                  = 0                                                 scalar_as_nodal              = 0                                                 sequence                     = INVALID                                           show                         = INVALID                                           show_material_properties     = INVALID                                           start_time                   = INVALID                                           sync_only                    = 0                                                 sync_times                   =                                                   time_tolerance               = 1e-14                                             use_displaced                = 0                                               [../]                                                                          []                                                                                                                                                                [Postprocessors]                                                                                                                                                    [./c1_conc]                                                                        type                         = ElementIntegralVariablePostprocessor              block                        = INVALID                                           execute_on                   = timestep_end                                      long_name                    = Postprocessors/c1_conc                            outputs                      = INVALID                                           seed                         = 0                                                 use_displaced_mesh           = 0                                                 variable                     = c1                                              [../]                                                                                                                                                             [./c2_conc]                                                                        type                         = ElementIntegralVariablePostprocessor              block                        = INVALID                                           execute_on                   = timestep_end                                      long_name                    = Postprocessors/c2_conc                            outputs                      = INVALID                                           seed                         = 0                                                 use_displaced_mesh           = 0                                                 variable                     = c2                                              [../]                                                                                                                                                             [./c3_conc]                                                                        type                         = ElementIntegralVariablePostprocessor              block                        = INVALID                                           execute_on                   = timestep_end                                      long_name                    = Postprocessors/c3_conc                            outputs                      = INVALID                                           seed                         = 0                                                 use_displaced_mesh           = 0                                                 variable                     = c3                                              [../]                                                                                                                                                             [./sum]                                                                            type                         = SumOfPostprocessors                               execute_on                   = timestep_end                                      factors                      = '1 2 3'                                           long_name                    = Postprocessors/sum                                outputs                      = INVALID                                           postprocessors               = 'c1_conc c2_conc c3_conc'                         use_displaced_mesh           = 0                                               [../]                                                                          []                                                                                                                                                                [Variables]                                                                                                                                                         [./c1]                                                                             block                        = INVALID                                           eigen                        = 0                                                 family                       = LAGRANGE                                          initial_condition            = 100                                               order                        = FIRST                                             scaling                      = 1                                                 initial_from_file_timestep   = 2                                                 initial_from_file_var        = INVALID                                         [../]                                                                          []                                                                                @�@     ?���-�L?���V}�?�b����?�b���J@@�xI�Cp@@�xE��m?���B|?�b����@@�xG�o@Y   A$@�@     ?}
�$EyE?}
�%��?N۱U��2?N۱Wʇ�@@�H��zn@@�H����?}
�$��n?N۱V�<@@�H��9�@Y   @�@�p     ?T����x?T����K?(F��SJ�?(F��G{@@���[ŭ@@���qH\?T����=,?(F�ֱ��@@���f�@Y   @�@�@     ?>�&5��?>�&��?����?���@@���U}�@@���k �?>�&k)�?���X@@���`?,@Y   @�@��     ?0I�2v�c?0I�2��M?�Ź��?�Ź�{Y@@��"���@@���|�?0I�2�X?�Ź�Kv@@�� ��$@Y   @�@�p     ?$�����?$���	c!>�O�q`�>�O�q�Ns@@��lM�b@@��hc_?$������>�O�qy��@@��jX��@Y   @�@�X     ?`�[�[?`�[:��>�
��.�>�
��I7�@@��\�@@��0߿?`�[(�&>�
��;�@@��&0@Y   @�@�@     ?e4R��?e4h��>��r��>��rΩ�@@��tGU�@@��p\��?e4]Ǽ>��r�ak@@��rRW@Y   @�@��     ?���*Wd?���8�o>���]�>���C@@�����^@@����z~?���1{�>��� @@������@Y   @�@È     ?�'�e
�?�'�x��>�T����>�T���Ǌ@@���nX�@@��ك۶?�'�o�>�T���e@@���y&@Y   @�