# API List

<!-- md-trans-meta sourceCommit=28b6cff9e78c6de0dfa0a7b19c256fb6ad06a312 translatedAt=2026-10-23T03:54:33.748650+00:00 pushedAt=2026-10-27T02:26:31.895Z nl=2 -->

The C API documentation is organized by the finest-grained public header files. Except for experimental APIs, you can include `c_api/asc_simd.h` for overall use.

## Basic Definitions

### Union

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_fill_value_config](defs/union/asc_fill_value_config.md)|The initialization parameter structure for fill_value, containing the various initialization parameters that need to be configured for the [asc_fill_l0a](cube_datamove/asc_fill_l0a.md)/[asc_fill_l0b](cube_datamove/asc_fill_l0b.md)/[asc_fill_l1](cube_datamove/asc_fill_l1.md) APIs.|`c_api/defs/union.h`|
|[asc_l13d_fmatrix_config](defs/union/asc_l13d_fmatrix_config.md)|Used to set the Feature map attribute parameters of the [asc_copy_l12l0a](cube_datamove/asc_copy_l12l0a/asc_copy_l12l0a.md)/[asc_copy_l12l0b](cube_datamove/asc_copy_l12l0b/asc_copy_l12l0b.md) 3D format data movement APIs.|`c_api/defs/union.h`|
|[asc_load3d_v2_config](defs/union/asc_load3d_v2_config.md)|The repeat parameter of the Load3Dv2 API.|`c_api/defs/union.h`|
|[asc_ndim_pad_count_config](defs/union/asc_ndim_pad_count_config.md)|Used in the [asc_set_ndim_pad_count](vector_datamove/asc_set_ndim_pad_count.md) API to set the number of padding elements on the left and right sides of each dimension of the [asc_ndim_copy_gm2ub](vector_datamove/asc_ndim_copy_gm2ub.md) API.|`c_api/defs/union.h`|
|[asc_store_atomic_config](defs/union/asc_store_atomic_config.md)|The values of the atomic operation enable bit and the atomic operation type.|`c_api/defs/union.h`|

### Enumeration

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_channel_pad_mode](defs/enum/asc_channel_pad_mode.md)|Indicates the padding or compression mode for each 32-byte channel block when moving data from GM to L1 Buffer.|`c_api/defs/enum.h`|
|[asc_dual_dst_mode](defs/enum/asc_dual_dst_mode.md)|Indicates the dual-destination split mode when moving data out of the L0C Buffer.|`c_api/defs/enum.h`|
|[asc_hf32_round_mode](defs/enum/asc_hf32_round_mode.md)|Indicates the rounding mode management policy for rounding from FP32 to HF32 when HF32 mode is enabled for Mmad computation.|`c_api/defs/enum.h`|
|[asc_l13d_repeat_direction](defs/enum/asc_l13d_repeat_direction.md)|Indicates the repeat iteration direction of 3D img2col data movement.|`c_api/defs/enum.h`|
|[asc_load_l2_cache_mode](defs/enum/asc_load_l2_cache_mode.md)|Indicates the L2 cache policy when moving data from GM to the UB, and can also be used for scalar load policy configuration.|`c_api/defs/enum.h`|
|[asc_mutex_execute_mode](defs/enum/asc_mutex_execute_mode.md)|Indicates the blocking or non-blocking execution mode of a mutex.|`c_api/defs/enum.h`|
|[asc_override_strategy](defs/enum/asc_override_strategy.md)|Indicates the saturation control policy.|`c_api/defs/enum.h`|
|[asc_position_mode](defs/enum/asc_position_mode.md)|Indicates the even or odd position of the source or destination operand in a register.|`c_api/defs/enum.h`|
|[asc_position_quarter_mode](defs/enum/asc_position_quarter_mode.md)|Indicates the quarter position of the source or destination operand in a register.|`c_api/defs/enum.h`|
|[asc_quant_mode](defs/enum/asc_quant_mode.md)|Indicates the type conversion or quantization mode when moving data out of the L0C Buffer.|`c_api/defs/enum.h`|
|[asc_relu_pre_mode](defs/enum/asc_relu_pre_mode.md)|Indicates the on-the-fly ReLU mode during L0C Buffer data move-out.|`c_api/defs/enum.h`|
|[asc_saturation_mode](defs/enum/asc_saturation_mode.md)|Indicates the saturation control mode.|`c_api/defs/enum.h`|
|[asc_store_l2_cache_mode](defs/enum/asc_store_l2_cache_mode.md)|Indicates the L2 cache policy when moving data from UB to GM. It can also be used to configure the scalar store policy.|`c_api/defs/enum.h`|
|[asc_unit_flag_mode](defs/enum/asc_unit_flag_mode.md)|Indicates the UnitFlag behavior after a Cube computation or Cube move-out API is executed.|`c_api/defs/enum.h`|

### Data Types

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[Reg data type definitions](defs/type/data_type_definition.md)|Describes the C API data types for vector data registers, mask registers, unaligned registers, and address registers.|`c_api/defs/type.h`|

### Macros

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[ASC_DEPRECATED](defs/macro/asc_deprecation.md)|Adds deprecation attributes and compilation warning information to C API declarations.|`c_api/defs/macro.h`|
|[ASC_IS_AIC/ASC_IS_AIV](defs/macro/asc_is_aic-asc_is_aiv.md)|Determines whether the currently compiled code path is the Cube Core (AIC) or Vector Core (AIV) code path.|`c_api/defs/macro.h`|

### Constants

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[ASC_C_API_MRGSORT_ELEMENT_LEN](defs/constant/asc_c_api_mrgsort_element_len.md)|Number of queues supported by the MrgSort4 operation.|`c_api/defs/constant.h`|
|[ASC_VDEQ_SIZE](defs/constant/asc_vdeq_size.md)|Number of elements in the quantization parameter array of the asc_set_deq_scale API.|`c_api/defs/constant.h`|

## Utility APIs

### System Initialization

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_init](utils/sys_init/asc_init.md)|Initializes the NPU state.|`c_api/utils/sys_init.h`|

### Other System Operations

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_nop](utils/sys_misc/asc_nop.md)|Generates a no-operation instruction without performing actual computation.|`c_api/utils/sys_misc.h`|

### System Resources and Variables

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_get_arch_ver](utils/sys_var/asc_get_arch_ver.md)|Obtains the current AI processor architecture version number.|`c_api/utils/sys_var.h`|
|[asc_get_core_id](utils/sys_var/asc_get_core_id.md)|Obtains the ID of the current core.|`c_api/utils/sys_var.h`|
|[asc_get_ffts_base_addr](utils/sys_var/asc_get_ffts_base_addr.md)|Returns the base address of the inter-core synchronization register.|`c_api/utils/sys_var.h`|
|[asc_get_phy_buf_addr](utils/sys_var/asc_get_phy_buf_addr.md)|Obtains the actual physical address on-chip based on the offset.|`c_api/utils/sys_var.h`|
|[asc_get_phy_stack_base](utils/sys_var/asc_get_phy_stack_base.md)|Obtains the physical stack base address.|`c_api/utils/sys_var.h`|
|[asc_get_program_counter](utils/sys_var/asc_get_program_counter.md)|Obtains the pointer to the program counter, which records the current execution position of the program.|`c_api/utils/sys_var.h`|
|[asc_get_smmu_tag_version](utils/sys_var/asc_get_smmu_tag_version.md)|Obtains the System Memory Management Unit (SMMU) version information.|`c_api/utils/sys_var.h`|
|[asc_get_status](utils/sys_var/asc_get_status.md)|Obtains the status information.|`c_api/utils/sys_var.h`|
|[asc_get_sub_block_id](utils/sys_var/asc_get_sub_block_id.md)|Obtains the ID of the Vector core on the AI Core.|`c_api/utils/sys_var.h`|
|[asc_get_sub_block_num](utils/sys_var/asc_get_sub_block_num.md)|In separate mode, obtains the number of Cube Cores (AIC) or Vector Cores (AIV) on an AI Core.|`c_api/utils/sys_var.h`|
|[asc_get_sys_virtual_base](utils/sys_var/asc_get_sys_virtual_base.md)|Obtains the system virtual base address.|`c_api/utils/sys_var.h`|
|[asc_get_system_cycle](utils/sys_var/asc_get_system_cycle.md)|Obtains the current system cycle count.|`c_api/utils/sys_var.h`|
|[asc_get_system_clock](utils/sys_var/asc_get_system_clock.md)|Obtains the current system clock time in microseconds.|`c_api/utils/sys_var.h`|
|[asc_get_vf_len](utils/sys_var/asc_get_vf_len.md)|Obtains the size of the Tensor bit width VL (Vector Length).|`c_api/utils/sys_var.h`|
|[asc_set_ffts_base_addr](utils/sys_var/asc_set_ffts_base_addr.md)|Used before [asc_sync_block_arrive](sync/inter_core_sync/asc_sync_block_arrive.md) and [asc_sync_block_wait](sync/inter_core_sync/asc_sync_block_wait.md) to set the base address of the inter-core synchronization register.|`c_api/utils/sys_var.h`|

## Cube Computation

### MMAD

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_enable_hf32](cube_compute/asc_enable_hf32.md)|Enables HF32 mode for Mmad computation. After this mode is enabled, the FP32 data in the L0A Buffer/L0B Buffer is rounded to HF32 before participating in Mmad computation.|`c_api/cube_compute/cube_compute.h`|
|[asc_enable_hif8](cube_compute/asc_enable_hif8.md)|Enables HiF8 mode. When both left matrix A and right matrix B are of the fp8_e4m3fn_t type, the data is converted to the hifloat8_t type before participating in matrix multiplication.|`c_api/cube_compute/cube_compute.h`|
|[asc_disable_hif8](cube_compute/asc_disable_hif8.md)|Disables HiF8 mode for Mmad computation. When both left matrix A and right matrix B are of the fp8_e4m3fn_t type, the data is not converted to the hifloat8_t type and directly participates in matrix multiplication as fp8_e4m3fn_t.|`c_api/cube_compute/cube_compute.h`|
|[asc_disable_hf32](cube_compute/asc_disable_hf32.md)|Disables HF32 mode for Mmad computation. After this mode is set, FP32 data in the L0A Buffer/L0B Buffer is not rounded before participating in Mmad computation.|`c_api/cube_compute/cube_compute.h`|
|[asc_mmad](cube_compute/asc_mmad.md)|Performs matrix multiply-accumulate operations.|`c_api/cube_compute/cube_compute.h`|
|[asc_mmad_mx](cube_compute/asc_mmad_mx.md)|Performs a matrix multiply-add operation with scaling.|`c_api/cube_compute/cube_compute.h`|
|[asc_mmad_sparse](cube_compute/asc_mmad_sparse.md)|Performs matrix multiply-accumulate operations, where the input left matrix A is a sparse matrix and the right matrix B is a dense matrix.|`c_api/cube_compute/cube_compute.h`|
|[asc_set_hf32_round_mode](cube_compute/asc_set_hf32_round_mode.md)|Sets the rounding method for HF32 mode. This configuration takes effect only while HF32 mode is enabled.|`c_api/cube_compute/cube_compute.h`|
|[asc_set_mmad_direction_m](cube_compute/asc_set_mmad_direction_m.md)|Sets the Mmad computation to preferentially generate results along the M direction of M/N, and then produce results along the N direction, where M is the number of rows of the matrix and N is the number of columns of the matrix.|`c_api/cube_compute/cube_compute.h`|
|[asc_set_mmad_direction_n](cube_compute/asc_set_mmad_direction_n.md)|Sets the Mmad computation to preferentially generate results along the N direction of M/N, and then generate results along the M direction, where M is the number of rows of the matrix and N is the number of columns of the matrix.|`c_api/cube_compute/cube_compute.h`|

## Cube Computation Data Movement

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_copy_gm2l0a](cube_datamove/asc_copy_gm2l0a.md)|Copies data from GM to L0A.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_copy_gm2l0b](cube_datamove/asc_copy_gm2l0b.md)|Copies data from GM to L0B.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_copy_gm2l1_align](cube_datamove/asc_copy_gm2l1_align.md)|Copies data from GM to L1 after padding.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_copy_gm2l1_dn2nz](cube_datamove/asc_copy_gm2l1_dn2nz.md)|Copies data from GM to L1, performing DN->NZ/NCHW->NC1HWC0/NCHW->C1HWNC0 operations during the process.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_copy_l0c2ub](cube_datamove/asc_copy_l0c2ub.md)|Copies data from L0C to UB.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_copy_l12gm](cube_datamove/asc_copy_l12gm.md)|Copies data from L1 to GM.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_copy_l12l0a_mx](cube_datamove/asc_copy_l12l0a_mx.md)|Copies the Mx scale matrix from the L1 Buffer to the L0A Buffer.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_copy_l12l0a_trans](cube_datamove/asc_copy_l12l0a_trans.md)|This API loads 2D data with transpose from the L1 Buffer to the L0A Buffer.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_copy_l12l0b_mx](cube_datamove/asc_copy_l12l0b_mx.md)|Copies the Mx scale matrix from L1 Buffer to L0B Buffer.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_copy_l12l0b_sparse](cube_datamove/asc_copy_l12l0b_sparse.md)|Copies a 512B dense weight matrix stored in the L1 Buffer to the L0B Buffer, and simultaneously reads a 128B index matrix to sparsify the dense matrix.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_copy_l12l0c](cube_datamove/asc_copy_l12l0c.md)|Copies a matrix from the L1 Buffer to the L0C Buffer.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_copy_l12ub](cube_datamove/asc_copy_l12ub.md)|Copies data from the L1 Buffer to UB.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_fill_l0a](cube_datamove/asc_fill_l0a.md)|Initializes the Local Memory of the L0A Buffer to a specific value.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_fill_l0b](cube_datamove/asc_fill_l0b.md)|Initializes the Local Memory of the L0B Buffer to a specific value.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_fill_l1](cube_datamove/asc_fill_l1.md)|Initializes the Local Memory of the L1 Buffer to a specific value.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_get_l0c2gm_prequant](cube_datamove/asc_get_l0c2gm_prequant.md)|During on-the-fly quantization in the data movement process, call this API to obtain the start address of the vector before the quantization operation.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_get_l0c2gm_relu](cube_datamove/asc_get_l0c2gm_relu.md)|During on-the-fly quantization in data movement, call this API to obtain the start address of the vector before the ReLU operation.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_get_l0c2gm_unitflag](cube_datamove/asc_get_l0c2gm_unitflag.md)|When on-the-fly quantization is performed during data movement, call this API to obtain the unit_flag setting.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_load_image_to_cbuf](cube_datamove/asc_load_image_to_cbuf.md)|Transfers image data from Global Memory to L1 Buffer.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_set_gm2l1_loop1_stride](cube_datamove/asc_set_gm2l1_loop1_stride.md)|Sets the stride between adjacent iteration data blocks in the inner loop when moving data from GM to the L1 Buffer.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_set_gm2l1_loop2_stride](cube_datamove/asc_set_gm2l1_loop2_stride.md)|Sets the stride between adjacent iteration data blocks in the outer loop when moving data from GM to the L1 Buffer.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_set_gm2l1_loop_size](cube_datamove/asc_set_gm2l1_loop_size.md)|When transferring data from GM to the L1 Buffer, this API sets the number of loops in the data movement process.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_set_gm2l1_nz_para](cube_datamove/asc_set_gm2l1_nz_para.md)|Sets the value of the MTE2_NZ_PARA register.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_set_gm2l1_padding](cube_datamove/asc_set_gm2l1_padding.md)|Sets the padding value of [asc_copy_gm2l1_arch_3510 (high-dimensional slicing data movement)](cube_datamove/asc_copy_gm2l1/asc_copy_gm2l1_highdim_split_arch_3510.md).|`c_api/cube_datamove/cube_datamove.h`|
|[asc_set_gm2l1_pad](cube_datamove/asc_set_gm2l1_pad.md)|Sets the constant padding value of the [asc_copy_gm2l1_align](cube_datamove/asc_copy_gm2l1_align.md) API.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_set_l0c_copy_nz_para](cube_datamove/asc_set_l0c_copy_nz_para.md)|During data movement, when performing on-the-fly format conversion (converting the NZ format to the ND format), this API sets the related format conversion configuration.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_set_l0c_copy_channel_para](cube_datamove/asc_set_l0c_copy_channel_para.md)|Configures the address offset between adjacent rows of the source Nz matrix during Nz2DN format conversion.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_set_l0c_copy_config](cube_datamove/asc_set_l0c_copy_config.md)|Sets the related configuration for on-the-fly tensor quantization, on-the-fly tensor activation, and UnitFlag functions in the L0C Buffer move-out scenario.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_set_l0c_copy_lrelu_alpha](cube_datamove/asc_set_l0c_copy_lrelu_alpha.md)|When performing on-the-fly scalar activation during L0C Buffer move-out, configures the scaling coefficient required for activation computation.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_set_l0c_copy_relu_alpha](cube_datamove/asc_set_l0c_copy_relu_alpha.md)|When performing on-the-fly scalar activation during L0C Buffer move-out, configures the scaling coefficient required for activation computation.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_set_l0c_copy_params](cube_datamove/asc_set_l0c_copy_params.md)|Sets format conversion configurations when DataCopy performs on-the-fly format conversion (NZ to ND) while moving data from L0C Buffer to Global Memory or L1 Buffer.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_set_l0c_copy_prequant](cube_datamove/asc_set_l0c_copy_prequant.md)|When on-the-fly quantization is performed during data movement, call this API to set the scalar quantization parameters used in the quantization process.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_set_l13d_fmatrix](cube_datamove/asc_set_l13d_fmatrix.md)|Configures the Feature Map attribute description parameters of the left matrix in 3D img2col data movement mode.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_set_l13d_fmatrix_b](cube_datamove/asc_set_l13d_fmatrix_b.md)|Configures the Feature Map attribute description parameters of the right matrix in 3D img2col data movement mode.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_set_l12l0a_3d_padding](cube_datamove/asc_set_l12l0a_3d_padding.md)|Sets the padding value for 3D left matrix data movement.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_set_l12l0b_3d_padding](cube_datamove/asc_set_l12l0b_3d_padding.md)|Sets the padding value for 3D right matrix data movement.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_set_l13d_rpt](cube_datamove/asc_set_l13d_rpt.md)|Configures the parameters required for moving the left matrix in repeat mode under 3D img2col data movement mode.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_set_l13d_rpt_b](cube_datamove/asc_set_l13d_rpt_b.md)|Configures the parameters required for moving the right matrix in repeat mode under 3D img2col data movement mode.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_set_l13d_size](cube_datamove/asc_set_l13d_size.md)|Sets the boundary value of the 3D format data transfer API of [asc_copy_l12l0a](cube_datamove/asc_copy_l12l0a/asc_copy_l12l0a_arch_2201.md)/[asc_copy_l12l0b](cube_datamove/asc_copy_l12l0b/asc_copy_l12l0b_arch_2201.md) in the L1 Buffer.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_set_l3d_rpt_b](cube_datamove/asc_set_l3d_rpt_b.md)|Sets the parameters required for moving the right matrix in repeat mode under 3D img2col data movement mode through a 64-bit configuration value.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_copy_gm2l1](cube_datamove/asc_copy_gm2l1/asc_copy_gm2l1.md)|Copies data from GM to L1.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_copy_gm2l1_nd2nz](cube_datamove/asc_copy_gm2l1_nd2nz/asc_copy_gm2l1_nd2nz.md)|Copies data from GM to L1.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_copy_l0c2gm](cube_datamove/asc_copy_l0c2gm/asc_copy_l0c2gm.md)|Copies data from L0C to GM.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_copy_l0c2l1](cube_datamove/asc_copy_l0c2l1/asc_copy_l0c2l1.md)|After the Cube computation is complete, quantizes the result and then copies the processed result to L1.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_copy_l12bt](cube_datamove/asc_copy_l12bt/asc_copy_l12bt.md)|Copies the Bias data of the MMAD instruction from the L1 Buffer to the BiasTable Buffer.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_copy_l12fb](cube_datamove/asc_copy_l12fb/asc_copy_l12fb.md)|Copies data from the L1 Buffer to the Fixpipe Buffer, which is used to store quantization parameters.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_copy_l12l0a](cube_datamove/asc_copy_l12l0a/asc_copy_l12l0a.md)|Copies a 512B matrix stored in the L1 Buffer to the L0A Buffer.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_copy_l12l0b](cube_datamove/asc_copy_l12l0b/asc_copy_l12l0b.md)|Copies a 512B matrix stored in the L1 Buffer to the L0B Buffer.|`c_api/cube_datamove/cube_datamove.h`|
|[asc_copy_l12l0b_trans](cube_datamove/asc_copy_l12l0b_trans/asc_copy_l12l0b_trans.md)|This API loads 2D data with transpose from the L1 Buffer to the L0B Buffer.|`c_api/cube_datamove/cube_datamove.h`|

## Memory Vector Computation

### Basic Arithmetic

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_abs](vector_compute/vector_arith/asc_abs.md)|Computes the absolute value element-wise.|`c_api/vector_compute/compute/vector_arith.h`|
|[asc_add](vector_compute/vector_arith/asc_add.md)|Performs element-wise summation.|`c_api/vector_compute/compute/vector_arith.h`|
|[asc_add_scalar](vector_compute/vector_arith/asc_add_scalar.md)|Sums each element in the vector with a scalar.|`c_api/vector_compute/compute/vector_arith.h`|
|[asc_div](vector_compute/vector_arith/asc_div.md)|Performs element-wise division.|`c_api/vector_compute/compute/vector_arith.h`|
|[asc_exp](vector_compute/vector_arith/asc_exp.md)|Computes the natural exponential element-wise.|`c_api/vector_compute/compute/vector_arith.h`|
|[asc_log](vector_compute/vector_arith/asc_log.md)|Computes the natural logarithm element-wise.|`c_api/vector_compute/compute/vector_arith.h`|
|[asc_max](vector_compute/vector_arith/asc_max.md)|Computes the maximum value element-wise.|`c_api/vector_compute/compute/vector_arith.h`|
|[asc_max_scalar](vector_compute/vector_arith/asc_max_scalar.md)|Computes the maximum value between each element in the vector and a scalar.|`c_api/vector_compute/compute/vector_arith.h`|
|[asc_min](vector_compute/vector_arith/asc_min.md)|Computes the minimum value element-wise.|`c_api/vector_compute/compute/vector_arith.h`|
|[asc_min_scalar](vector_compute/vector_arith/asc_min_scalar.md)|Compares each element of the source operand vector with a scalar element-wise and takes the smaller value.|`c_api/vector_compute/compute/vector_arith.h`|
|[asc_mul](vector_compute/vector_arith/asc_mul.md)|Performs element-wise multiplication.|`c_api/vector_compute/compute/vector_arith.h`|
|[asc_mul_scalar](vector_compute/vector_arith/asc_mul_scalar.md)|Multiplies each element in the vector by a scalar.|`c_api/vector_compute/compute/vector_arith.h`|
|[asc_rcp](vector_compute/vector_arith/asc_rcp.md)|Performs the reciprocal operation on a vector.|`c_api/vector_compute/compute/vector_arith.h`|
|[asc_rsqrt](vector_compute/vector_arith/asc_rsqrt.md)|Computes the square root and then the reciprocal element-wise.|`c_api/vector_compute/compute/vector_arith.h`|
|[asc_sqrt](vector_compute/vector_arith/asc_sqrt.md)|Computes the square root of elements.|`c_api/vector_compute/compute/vector_arith.h`|
|[asc_sub](vector_compute/vector_arith/asc_sub.md)|Performs element-wise subtraction.|`c_api/vector_compute/compute/vector_arith.h`|
|[asc_sub_scalar](vector_compute/vector_arith/asc_sub_scalar.md)|Subtracts a scalar from each element in the vector.|`c_api/vector_compute/compute/vector_arith.h`|

### Logical Computation

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_and](vector_compute/vector_logic/asc_and.md)|Performs the AND operation on a vector.|`c_api/vector_compute/compute/vector_logic.h`|
|[asc_not](vector_compute/vector_logic/asc_not.md)|Performs bitwise NOT on each element. The calculation formula is as follows.|`c_api/vector_compute/compute/vector_logic.h`|
|[asc_or](vector_compute/vector_logic/asc_or.md)|Performs a bitwise OR operation on each pair of elements.|`c_api/vector_compute/compute/vector_logic.h`|
|[asc_shiftleft](vector_compute/vector_logic/asc_shiftleft.md)|Shifts all elements left by distance bits.|`c_api/vector_compute/compute/vector_logic.h`|
|[asc_shiftright](vector_compute/vector_logic/asc_shiftright.md)|Performs a right shift on each element in the source operand.|`c_api/vector_compute/compute/vector_logic.h`|

### Reduction Computation

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_datablock_reduce_max](vector_compute/vector_reduce/asc_datablock_reduce_max.md)|Computes the maximum value of all elements in each DataBlock.|`c_api/vector_compute/compute/vector_reduce.h`|
|[asc_datablock_reduce_min](vector_compute/vector_reduce/asc_datablock_reduce_min.md)|Computes the minimum value of all elements in each DataBlock.|`c_api/vector_compute/compute/vector_reduce.h`|
|[asc_datablock_reduce_sum](vector_compute/vector_reduce/asc_datablock_reduce_sum.md)|Sums all elements in each DataBlock.|`c_api/vector_compute/compute/vector_reduce.h`|
|[asc_get_reduce_max_cnt](vector_compute/vector_reduce/asc_get_reduce_max_cnt.md)|Obtains the maximum value after the asc_repeat_reduce_max operation, as well as the index of the first maximum value.|`c_api/vector_compute/compute/vector_reduce.h`|
|[asc_get_reduce_min_cnt](vector_compute/vector_reduce/asc_get_reduce_min_cnt.md)|Obtains the minimum value after the asc_repeat_reduce_min operation, as well as the index of the first minimum value.|`c_api/vector_compute/compute/vector_reduce.h`|
|[asc_get_rsvd_count](vector_compute/vector_reduce/asc_get_rsvd_count.md)|Obtains the number of remaining elements after a GatherMask operation.|`c_api/vector_compute/compute/vector_reduce.h`|
|[asc_pair_reduce_sum](vector_compute/vector_reduce/asc_pair_reduce_sum.md)|Performs a reduction operation on the input data to obtain the total sum.|`c_api/vector_compute/compute/vector_reduce.h`|
|[asc_reduce](vector_compute/vector_reduce/asc_reduce.md)|Selects elements from the source operand and writes them into the destination operand based on the gather mask (data collection mask) corresponding to the built-in fixed pattern in binary form or the user-defined input value.|`c_api/vector_compute/compute/vector_reduce.h`|
|[asc_repeat_reduce_max](vector_compute/vector_reduce/asc_repeat_reduce_max.md)|Computes the maximum value of all elements within each Repeat.|`c_api/vector_compute/compute/vector_reduce.h`|
|[asc_repeat_reduce_min](vector_compute/vector_reduce/asc_repeat_reduce_min.md)|Computes the minimum value of all elements within each Repeat.|`c_api/vector_compute/compute/vector_reduce.h`|
|[asc_repeat_reduce_sum](vector_compute/vector_reduce/asc_repeat_reduce_sum.md)|Sums all elements within each Repeat.|`c_api/vector_compute/compute/vector_reduce.h`|

### Broadcast Operation

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_brcb](vector_compute/vector_broadcast/asc_brcb.md)|Fills each value in the source operand into one DataBlock of the destination operand.|`c_api/vector_compute/compute/vector_broadcast.h`|
|[asc_duplicate](vector_compute/vector_broadcast/asc_duplicate.md)|Fills a variable or an immediate value into a vector.|`c_api/vector_compute/compute/vector_broadcast.h`|

### Composite Computation

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_add_relu](vector_compute/vector_fused/asc_add_relu.md)|Performs element-wise summation followed by Relu computation (comparing the result with 0 and taking the larger value), and provides the capability to convert the data type of the final result (s162s8, f322f16, f162s8).|`c_api/vector_compute/compute/vector_fused.h`|
|[asc_axpy](vector_compute/vector_fused/asc_axpy.md)|The product of each element in the source operand src and the scalar value is added to the corresponding element in the destination operand dst.|`c_api/vector_compute/compute/vector_fused.h`|
|[asc_deq_int162b8](vector_compute/vector_fused/asc_deq_int162b8.md)|Converts int16_t data to int8_t or uint8_t type, and stores the data in the upper or lower half of each DataBlock.|`c_api/vector_compute/compute/vector_fused.h`|
|[asc_deq_int322half](vector_compute/vector_fused/asc_deq_int322half.md)|Quantizes the input int32_t data element-wise and converts it to the half type.|`c_api/vector_compute/compute/vector_fused.h`|
|[asc_fma](vector_compute/vector_fused/asc_fma.md)|Multiplies src0 and src1 element-wise, adds the result to dst, and stores the final result in dst.|`c_api/vector_compute/compute/vector_fused.h`|
|[asc_leakyrelu](vector_compute/vector_fused/asc_leakyrelu.md)|Performs the Leaky Relu operation on a vector.|`c_api/vector_compute/compute/vector_fused.h`|
|[asc_mul_add](vector_compute/vector_fused/asc_mul_add.md)|Performs the multiply-add operation on a vector.|`c_api/vector_compute/compute/vector_fused.h`|
|[asc_mul_add_relu](vector_compute/vector_fused/asc_mul_add_relu.md)|Multiplies src0 and dst element-wise, adds src1, and then performs Relu computation (comparing the result with 0 and taking the larger value). The final result is stored in dst.|`c_api/vector_compute/compute/vector_fused.h`|
|[asc_mul_cast_half2int8](vector_compute/vector_fused/asc_mul_cast_half2int8.md)|Performs element-wise multiplication and converts the result to the int8_t type.|`c_api/vector_compute/compute/vector_fused.h`|
|[asc_mul_cast_half2uint8](vector_compute/vector_fused/asc_mul_cast_half2uint8.md)|Performs element-wise multiplication and converts the result to the uint8_t type.|`c_api/vector_compute/compute/vector_fused.h`|
|[asc_relu](vector_compute/vector_fused/asc_relu.md)|Performs linear rectification (Relu) element-wise.|`c_api/vector_compute/compute/vector_fused.h`|
|[asc_set_deq_scale](vector_compute/vector_fused/asc_set_deq_scale.md)|Sets the value of the DEQSCALAR register.|`c_api/vector_compute/compute/vector_fused.h`|
|[asc_sub_relu](vector_compute/vector_fused/asc_sub_relu.md)|Computes the element-wise difference and then applies Relu (taking the larger value between the result and 0), and provides the capability to convert the data type of the final result (s162s8, f322f16, f162s8).|`c_api/vector_compute/compute/vector_fused.h`|
|[asc_vdeq_int162b8](vector_compute/vector_fused/asc_vdeq_int162b8.md)|Converts int16_t data to int8_t or uint8_t type, and stores the data in the upper or lower half of each DataBlock.|`c_api/vector_compute/compute/vector_fused.h`|

### Comparison Computation

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_eq](vector_compute/vector_compare/asc_eq.md)|Compares the element values of src0 and src1 at corresponding index positions. If the comparison result is true, the corresponding bit of the output result is set to 1; otherwise, it is set to 0.|`c_api/vector_compute/compute/vector_compare.h`|
|[asc_eq_scalar](vector_compute/vector_compare/asc_eq_scalar.md)|Performs a comparison operation between a vector and a scalar. If the values are equal, the output is 1; otherwise, it is 0.|`c_api/vector_compute/compute/vector_compare.h`|
|[asc_ge](vector_compute/vector_compare/asc_ge.md)|Ge (greater than or equal to) performs an element-wise comparison to determine whether src0 >= src1 holds. If it holds, the output result is 1; otherwise, the output result is 0. The comparison result of each element occupies one bit.|`c_api/vector_compute/compute/vector_compare.h`|
|[asc_ge_scalar](vector_compute/vector_compare/asc_ge_scalar.md)|Determines element-wise whether src >= value holds. If it holds, the output result is 1; otherwise, it is 0.|`c_api/vector_compute/compute/vector_compare.h`|
|[asc_get_cmp_mask](vector_compute/vector_compare/asc_get_cmp_mask.md)|Obtains the comparison result of a Compare operation.|`c_api/vector_compute/compute/vector_compare.h`|
|[asc_gt](vector_compute/vector_compare/asc_gt.md)|Compares the magnitudes of two vectors element by element. If the comparison result is true, the corresponding bit of the output result is 1; otherwise, it is 0.|`c_api/vector_compute/compute/vector_compare.h`|
|[asc_gt_scalar](vector_compute/vector_compare/asc_gt_scalar.md)|Each element in src is compared with the scalar value one by one. If the element at a certain position is greater than value, the corresponding bit of the output result dst is set to 1; otherwise, it is set to 0.|`c_api/vector_compute/compute/vector_compare.h`|
|[asc_le](vector_compute/vector_compare/asc_le.md)|Determines element-wise whether src0 <= src1 holds. If it holds, the output result is 1; otherwise, it is 0.|`c_api/vector_compute/compute/vector_compare.h`|
|[asc_le_scalar](vector_compute/vector_compare/asc_le_scalar.md)|Determines element-wise whether src <= value holds. If it holds, the output result is 1; otherwise, it is 0.|`c_api/vector_compute/compute/vector_compare.h`|
|[asc_lt](vector_compute/vector_compare/asc_lt.md)|Determines element-wise whether src0 < src1 holds. If it holds, the corresponding bit in the output result is set to 1; otherwise, it is set to 0.|`c_api/vector_compute/compute/vector_compare.h`|
|[asc_lt_scalar](vector_compute/vector_compare/asc_lt_scalar.md)|Compares each position in the vector with a scalar. If the value is less than the scalar value, the result is 1; otherwise, it is 0. The result is arranged in little-endian order for each bit.|`c_api/vector_compute/compute/vector_compare.h`|
|[asc_ne](vector_compute/vector_compare/asc_ne.md)|Determines element-wise whether src0 != src1 holds. If it holds, the output result is 1; otherwise, it is 0.|`c_api/vector_compute/compute/vector_compare.h`|
|[asc_ne_scalar](vector_compute/vector_compare/asc_ne_scalar.md)|Determines element-wise whether each element is not equal to the input scalar. If it holds, the corresponding bit in the output result is set to 1; otherwise, it is set to 0.|`c_api/vector_compute/compute/vector_compare.h`|
|[asc_set_cmp_mask](vector_compute/vector_compare/asc_set_cmp_mask.md)|Sets the mask used for selection in a Select operation.|`c_api/vector_compute/compute/vector_compare.h`|

### Layout Transformation

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_select](vector_compute/vector_permute_sel/asc_select.md)|Selects elements from two source operands based on the mask and outputs them to the destination operand.|`c_api/vector_compute/compute/vector_permute_sel.h`|
|[asc_set_va_reg](vector_compute/vector_permute_sel/asc_set_va_reg.md)|Sets the address for transpose and associates the operand address sequence with the address register.|`c_api/vector_compute/compute/vector_permute_sel.h`|
|[asc_squeeze](vector_compute/vector_permute_sel/asc_squeeze.md)|Compresses the vector in src0 into a shorter vector by comparing it with the mask src1, and stores the result in dst.|`c_api/vector_compute/compute/vector_permute_sel.h`|
|[asc_transpose](vector_compute/vector_permute_sel/asc_transpose.md)|Used to implement the transpose of a 16*16 two-dimensional matrix data block.|`c_api/vector_compute/compute/vector_permute_sel.h`|
|[asc_transto5hd](vector_compute/vector_permute_sel/asc_transto5hd.md)|Data format conversion, generally used to convert the NCHW format to the NC1HWC0 format.|`c_api/vector_compute/compute/vector_permute_sel.h`|

### Sort Computation

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_bitsort](vector_compute/vector_sort/asc_bitsort.md)|Score and Index are stored in src0 and src1, respectively. The elements are sorted by Score (elements with larger Score values are placed first), and the sorted Score together with its corresponding Index are stored in dst as a (Score, Index) structure.|`c_api/vector_compute/compute/vector_sort.h`|
|[asc_get_vms4_sr](vector_compute/vector_sort/asc_get_vms4_sr.md)|Obtains the number of Region Proposals that have been processed in each queue after the asc_mrgsort4 operation.|`c_api/vector_compute/compute/vector_sort.h`|
|[asc_mrgsort4](vector_compute/vector_sort/asc_mrgsort4.md)|Merges up to 4 sorted queues into 1 queue, with the results sorted by the score field in descending order.|`c_api/vector_compute/compute/vector_sort.h`|

### Aggregation Operation

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_gather](vector_compute/vector_gather/asc_gather.md)|Gathers the source operand into the destination operand element-wise according to the given offsets.|`c_api/vector_compute/compute/vector_gather.h`|
|[asc_gather_datablock](vector_compute/vector_gather/asc_gather_datablock.md)|Gathers the source operand into the destination operand at the granularity of DataBlock according to the offset addresses.|`c_api/vector_compute/compute/vector_gather.h`|

### Type Conversion

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_bfloat162float](vector_compute/vector_convert/asc_bfloat162float.md)|Data type conversion. Converts bfloat16_t data to the float type.|`c_api/vector_compute/compute/vector_convert.h`|
|[asc_bfloat162int32](vector_compute/vector_convert/asc_bfloat162int32.md)|Data type conversion. Converts bfloat16_t data to the int32_t type.|`c_api/vector_compute/compute/vector_convert.h`|
|[asc_float2bfloat16](vector_compute/vector_convert/asc_float2bfloat16.md)|Data type conversion. Converts float data to the bfloat16_t type.|`c_api/vector_compute/compute/vector_convert.h`|
|[asc_float2float](vector_compute/vector_convert/asc_float2float.md)|Data type conversion. Converts float data to the float type.|`c_api/vector_compute/compute/vector_convert.h`|
|[asc_float2half](vector_compute/vector_convert/asc_float2half.md)|Data type conversion. Converts float data to the half type.|`c_api/vector_compute/compute/vector_convert.h`|
|[asc_float2int16](vector_compute/vector_convert/asc_float2int16.md)|Converts float data to the int16_t type.|`c_api/vector_compute/compute/vector_convert.h`|
|[asc_float2int32](vector_compute/vector_convert/asc_float2int32.md)|Converts float data to the int32_t type.|`c_api/vector_compute/compute/vector_convert.h`|
|[asc_float2int64](vector_compute/vector_convert/asc_float2int64.md)|Converts float data to the int64_t type.|`c_api/vector_compute/compute/vector_convert.h`|
|[asc_half2float](vector_compute/vector_convert/asc_half2float.md)|Data type conversion. Converts half data to the float type.|`c_api/vector_compute/compute/vector_convert.h`|
|[asc_half2int16](vector_compute/vector_convert/asc_half2int16.md)|Data type conversion. Converts half data to the int16_t type.|`c_api/vector_compute/compute/vector_convert.h`|
|[asc_half2int32](vector_compute/vector_convert/asc_half2int32.md)|Data type conversion. Converts half data to the int32_t type.|`c_api/vector_compute/compute/vector_convert.h`|
|[asc_half2int4](vector_compute/vector_convert/asc_half2int4.md)|Data type conversion. Converts half data to the int4b_t type.|`c_api/vector_compute/compute/vector_convert.h`|
|[asc_half2int8](vector_compute/vector_convert/asc_half2int8.md)|Data type conversion. Converts half data to the int8_t type.|`c_api/vector_compute/compute/vector_convert.h`|
|[asc_half2uint8](vector_compute/vector_convert/asc_half2uint8.md)|Converts half data to the uint8_t type, supporting multiple rounding modes.|`c_api/vector_compute/compute/vector_convert.h`|
|[asc_int162float](vector_compute/vector_convert/asc_int162float.md)|Data type conversion. Converts int16_t data to the float type.|`c_api/vector_compute/compute/vector_convert.h`|
|[asc_int162half](vector_compute/vector_convert/asc_int162half.md)|Data type conversion. Converts int16_t data to the half type.|`c_api/vector_compute/compute/vector_convert.h`|
|[asc_int322float](vector_compute/vector_convert/asc_int322float.md)|Converts int32_t data to the float type.|`c_api/vector_compute/compute/vector_convert.h`|
|[asc_int322int16](vector_compute/vector_convert/asc_int322int16.md)|Data type conversion. Converts int32_t data to the int16_t type.|`c_api/vector_compute/compute/vector_convert.h`|
|[asc_int322int64](vector_compute/vector_convert/asc_int322int64.md)|Data type conversion. Converts int32_t data to the int64_t type.|`c_api/vector_compute/compute/vector_convert.h`|
|[asc_int42half](vector_compute/vector_convert/asc_int42half.md)|Data type conversion. Converts int4b_t data to the half type.|`c_api/vector_compute/compute/vector_convert.h`|
|[asc_int642float](vector_compute/vector_convert/asc_int642float.md)|Converts int64_t data to the float type.|`c_api/vector_compute/compute/vector_convert.h`|
|[asc_int642int32](vector_compute/vector_convert/asc_int642int32.md)|Data type conversion. Converts int64_t data to the int32_t type.|`c_api/vector_compute/compute/vector_convert.h`|
|[asc_int82half](vector_compute/vector_convert/asc_int82half.md)|Data type conversion. Converts int8_t data to the half type.|`c_api/vector_compute/compute/vector_convert.h`|
|[asc_uint82half](vector_compute/vector_convert/asc_uint82half.md)|Data type conversion. Converts uint8_t data to the half type.|`c_api/vector_compute/compute/vector_convert.h`|

### Mask Operation

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_set_mask_count](vector_compute/vector_mask_config/asc_set_mask_count.md)|Sets the Mask mode to Counter mode.|`c_api/vector_compute/compute/vector_mask_config.h`|
|[asc_set_mask_norm](vector_compute/vector_mask_config/asc_set_mask_norm.md)|Sets the Mask mode to Normal mode.|`c_api/vector_compute/compute/vector_mask_config.h`|
|[asc_set_vector_mask](vector_compute/vector_mask_config/asc_set_vector_mask.md)|Sets the Mask value.|`c_api/vector_compute/compute/vector_mask_config.h`|

## Vector Data Movement

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_copy](vector_datamove/asc_copy.md)|Copies data from the UB to the UB. Supports high-dimensional slicing.|`c_api/vector_datamove/vector_datamove.h`|
|[asc_copy_ub2l1](vector_datamove/asc_copy_ub2l1.md)|Copies data from the UB to the L1 Buffer.|`c_api/vector_datamove/vector_datamove.h`|
|[asc_copy_ub2ub](vector_datamove/asc_copy_ub2ub.md)|Copies data from the UB to the UB.|`c_api/vector_datamove/vector_datamove.h`|
|[asc_ndim_copy_gm2ub](vector_datamove/asc_ndim_copy_gm2ub.md)|Multi-dimensional data movement API that copies data from Global Memory (GM) to UB.|`c_api/vector_datamove/vector_datamove.h`|
|[asc_ndim_copy_dci](vector_datamove/asc_ndim_copy_dci.md)|Performs the Data Cache Invalidate (DCI) operation required for N-dimensional transfer scenarios.|`c_api/vector_datamove/vector_datamove.h`|
|[asc_set_copy_pad_val](vector_datamove/asc_set_copy_pad_val.md)|Used together with the asc_copy_gm2ub_align or asc_copy_ub2gm_align API to set the padding values on the left and right sides of the consecutively copied data blocks.|`c_api/vector_datamove/vector_datamove.h`|
|[asc_set_gm2ub_loop1_stride](vector_datamove/asc_set_gm2ub_loop1_stride.md)|Sets the data block interval between adjacent iterations in the inner loop when using [asc_copy_gm2ub_align](vector_datamove/asc_copy_gm2ub_align/asc_copy_gm2ub_align.md) to copy data from Global Memory (GM) to UB.|`c_api/vector_datamove/vector_datamove.h`|
|[asc_set_gm2ub_loop2_stride](vector_datamove/asc_set_gm2ub_loop2_stride.md)|Sets the data block interval between adjacent iterations in the outer loop when using [asc_copy_gm2ub_align](vector_datamove/asc_copy_gm2ub_align/asc_copy_gm2ub_align.md) to copy data from Global Memory (GM) to UB.|`c_api/vector_datamove/vector_datamove.h`|
|[asc_set_gm2ub_loop_size](vector_datamove/asc_set_gm2ub_loop_size.md)|Sets the number of loops in the data movement process when using [asc_copy_gm2ub_align](vector_datamove/asc_copy_gm2ub_align/asc_copy_gm2ub_align.md) to copy data from Global Memory (GM) to UB.|`c_api/vector_datamove/vector_datamove.h`|
|[asc_set_ndim_loop_stride](vector_datamove/asc_set_ndim_loop_stride.md)|Sets the interval between the elements of the source operand and the destination operand in each dimension when [asc_ndim_copy_gm2ub](vector_datamove/asc_ndim_copy_gm2ub.md) performs multi-dimensional data movement, with up to 5 dimensions configurable.|`c_api/vector_datamove/vector_datamove.h`|
|[asc_set_ndim_pad_count](vector_datamove/asc_set_ndim_pad_count.md)|Sets the number of padding elements on the left and right sides of each dimension of the [asc_ndim_copy_gm2ub](vector_datamove/asc_ndim_copy_gm2ub.md) API.|`c_api/vector_datamove/vector_datamove.h`|
|[asc_set_ndim_pad_value](vector_datamove/asc_set_ndim_pad_value.md)|Sets the fixed padding value of the [asc_ndim_copy_gm2ub](vector_datamove/asc_ndim_copy_gm2ub.md) API.|`c_api/vector_datamove/vector_datamove.h`|
|[asc_set_ub2gm_loop1_stride](vector_datamove/asc_set_ub2gm_loop1_stride.md)|Sets the data block interval of the source operand between adjacent iterations in the inner loop, and the data block interval of the destination operand between adjacent iterations, when using [asc_copy_ub2gm_align](vector_datamove/asc_copy_ub2gm_align/asc_copy_ub2gm_align.md) to copy data from UB to Global Memory (GM).|`c_api/vector_datamove/vector_datamove.h`|
|[asc_set_ub2gm_loop2_stride](vector_datamove/asc_set_ub2gm_loop2_stride.md)|Sets the data block interval of the source operand between adjacent iterations in the outer loop, and the data block interval of the destination operand between adjacent iterations, when using [asc_copy_ub2gm_align](vector_datamove/asc_copy_ub2gm_align/asc_copy_ub2gm_align.md) to copy data from UB to Global Memory (GM).|`c_api/vector_datamove/vector_datamove.h`|
|[asc_set_ub2gm_loop_size](vector_datamove/asc_set_ub2gm_loop_size.md)|Sets the number of inner loops and outer loops when using [asc_copy_ub2gm_align](vector_datamove/asc_copy_ub2gm_align/asc_copy_ub2gm_align.md) to copy data from UB to Global Memory (GM).|`c_api/vector_datamove/vector_datamove.h`|
|[asc_copy_gm2ub](vector_datamove/asc_copy_gm2ub/asc_copy_gm2ub.md)|Copies data from Global Memory to UB.|`c_api/vector_datamove/vector_datamove.h`|
|[asc_copy_gm2ub_align](vector_datamove/asc_copy_gm2ub_align/asc_copy_gm2ub_align.md)|Supports unaligned data copying from Global Memory to UB for 8-bit/16-bit/32-bit data types.|`c_api/vector_datamove/vector_datamove.h`|
|[asc_copy_ub2gm](vector_datamove/asc_copy_ub2gm/asc_copy_ub2gm.md)|Copies data from UB to Global Memory.|`c_api/vector_datamove/vector_datamove.h`|
|[asc_copy_ub2gm_align](vector_datamove/asc_copy_ub2gm_align/asc_copy_ub2gm_align.md)|Copies data from UB to Global Memory, supporting 8-bit/16-bit/32-bit block copy operations.|`c_api/vector_datamove/vector_datamove.h`|

## Reg Vector Computation

### Reg Data Load

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_loadalign](reg_compute/load/asc_loadalign.md)|An aligned data movement API that performs a contiguous aligned load from UB to the destination operand in NORM load mode.|`c_api/reg_compute/load/loadalign.h`|
|[asc_loadalign_datablock_strided](reg_compute/load/asc_loadalign_datablock_strided.md)|Aligned data movement API that performs a non-contiguous aligned load of 8 DataBlocks from UB and returns the vector data register through the function return value.|`c_api/reg_compute/load/loadalign.h`|
|[asc_loadalign_mask](reg_compute/load/asc_loadalign_mask.md)|Aligned data movement API that performs a contiguous aligned load of mask data from UB and returns the mask register through the function return value.|`c_api/reg_compute/load/loadalign.h`|
|[asc_loadalign_brc_datablock](reg_compute/load/asc_loadalign_brc_datablock.md)|Aligned data movement API that performs a contiguous aligned load of the destination operand from UB, reads one DataBlock, and broadcasts it to VL.|`c_api/reg_compute/load/loadalign.h`|
|[asc_loadalign_brc_datablock_postupdate](reg_compute/load/asc_loadalign_brc_datablock_postupdate.md)|Aligned data movement API that reads one DataBlock and broadcasts it to VL, with Post Update enabled.|`c_api/reg_compute/load/loadalign.h`|
|[asc_loadalign_brc_elem](reg_compute/load/asc_loadalign_brc_elem.md)|Aligned data movement API that performs a contiguous aligned load of the destination operand from UB, loads one element, and broadcasts it to VL.|`c_api/reg_compute/load/loadalign.h`|
|[asc_loadalign_brc_elem2datablock](reg_compute/load/asc_loadalign_brc_elem2datablock.md)|Aligned data movement API that performs a contiguous aligned load of the destination operand from UB and broadcasts each element to one DataBlock.|`c_api/reg_compute/load/loadalign.h`|
|[asc_loadalign_brc_elem2datablock_postupdate](reg_compute/load/asc_loadalign_brc_elem2datablock_postupdate.md)|Aligned data movement API that broadcasts each element to one DataBlock, with Post Update enabled.|`c_api/reg_compute/load/loadalign.h`|
|[asc_loadalign_brc_elem_postupdate](reg_compute/load/asc_loadalign_brc_elem_postupdate.md)|Aligned data movement API that loads one element and broadcasts it to VL, with Post Update enabled.|`c_api/reg_compute/load/loadalign.h`|
|[asc_loadalign_deintlv](reg_compute/load/asc_loadalign_deintlv.md)|Aligned data movement API. It performs a contiguous aligned load of data from UB into the destination operand and completes the on-the-fly deinterleave operation.|`c_api/reg_compute/load/loadalign.h`|
|[asc_loadalign_deintlv_postupdate](reg_compute/load/asc_loadalign_deintlv_postupdate.md)|Aligned data movement API. It performs a contiguous aligned load of data from UB into the destination operand, completes the on-the-fly deinterleave operation, enables Post Update mode, and automatically updates the UB address parameters.|`c_api/reg_compute/load/loadalign.h`|
|[asc_loadalign_downsample](reg_compute/load/asc_loadalign_downsample.md)|Aligned data movement API that performs a contiguous aligned load of the destination operand from UB in 2x downsampling mode.|`c_api/reg_compute/load/loadalign.h`|
|[asc_loadalign_mask_downsample](reg_compute/load/asc_loadalign_mask_downsample.md)|Aligned data movement API that performs a contiguous aligned load of mask data from UB in 2x downsampling mode and returns the mask register through the function return value.|`c_api/reg_compute/load/loadalign.h`|
|[asc_loadalign_downsample_postupdate](reg_compute/load/asc_loadalign_downsample_postupdate.md)|Aligned data movement API in 2x downsampling mode, with Post Update enabled.|`c_api/reg_compute/load/loadalign.h`|
|[asc_loadalign_postupdate](reg_compute/load/asc_loadalign_postupdate.md)|Loads data from UB into MaskReg and enables post mod.|`c_api/reg_compute/load/loadalign.h`|
|[asc_loadalign_unpack](reg_compute/load/asc_loadalign_unpack.md)|Aligned data movement API that performs a contiguous aligned load of the destination operand from UB in unpacking mode.|`c_api/reg_compute/load/loadalign.h`|
|[asc_loadalign_unpack4](reg_compute/load/asc_loadalign_unpack4.md)|Aligned data movement API that performs a contiguous aligned load of the destination operand from UB in unpacking mode.|`c_api/reg_compute/load/loadalign.h`|
|[asc_loadalign_unpack4_postupdate](reg_compute/load/asc_loadalign_unpack4_postupdate.md)|Aligned data movement API in unpacking mode, with Post Update enabled.|`c_api/reg_compute/load/loadalign.h`|
|[asc_loadalign_unpack_postupdate](reg_compute/load/asc_loadalign_unpack_postupdate.md)|Aligned data movement API in unpacking mode, with Post Update enabled.|`c_api/reg_compute/load/loadalign.h`|
|[asc_loadalign_upsample](reg_compute/load/asc_loadalign_upsample.md)|Aligned data movement API that performs a contiguous aligned load of the destination operand from UB in 2x upsampling mode.|`c_api/reg_compute/load/loadalign.h`|
|[asc_loadalign_mask_upsample](reg_compute/load/asc_loadalign_mask_upsample.md)|Aligned data movement API that performs a contiguous aligned load of mask data from UB in 2x upsampling mode and returns the mask register through the function return value.|`c_api/reg_compute/load/loadalign.h`|
|[asc_loadalign_upsample_postupdate](reg_compute/load/asc_loadalign_upsample_postupdate.md)|Aligned data movement API in 2x upsampling mode with Post Update enabled.|`c_api/reg_compute/load/loadalign.h`|
|[asc_load](reg_compute/load/asc_load.md)|A Reg computation data movement API that loads data from src, a source address in UB that is not aligned to 32 bytes, to a vector data register. The amount of data loaded is VL.|`c_api/composite/loadunalign_composite.h`|
|[asc_loadunalign](reg_compute/load/asc_loadunalign.md)|A Reg computation data movement API, applicable to scenarios where data is contiguously loaded into vector data registers from a start address in UB that is not aligned to 32 bytes.|`c_api/reg_compute/load/loadunalign.h`|
|[asc_loadunalign_postupdate](reg_compute/load/asc_loadunalign_postupdate.md)|Data movement API, suitable for contiguous loading starting from an address in UB that is not aligned to 32 bytes, with automatic source operand address update.|`c_api/reg_compute/load/loadunalign.h`|
|[asc_loadunalign_pre](reg_compute/load/asc_loadunalign_pre.md)|Used to initialize before loading unaligned data, and must be used together with the [asc_loadunalign](reg_compute/load/asc_loadunalign.md) API.|`c_api/reg_compute/load/loadunalign.h`|

### Reg Gather Load

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_gather](reg_compute/ub_gather/asc_gather.md)|Collects the source operand src into the destination operand dst element-wise based on the index position index (UB source gather mode).|`c_api/reg_compute/gather/ub_gather.h`|
|[asc_gather_datablock](reg_compute/ub_gather/asc_gather_datablock.md)|Given the base address of the source operand in UB and the index, collects the source operand into the destination operand by DataBlock based on the index position.|`c_api/reg_compute/gather/ub_gather.h`|

### Reg Data Store

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_storealign](reg_compute/store/asc_storealign.md)|A Reg computation data movement API that stores data contiguously in an aligned manner from vector data registers to UB in NORM store mode.|`c_api/reg_compute/store/storealign.h`|
|[asc_storealign_1st](reg_compute/store/asc_storealign_1st.md)|A Reg computation data movement API that stores the first element of src to dst, ignoring mask.|`c_api/reg_compute/store/storealign.h`|
|[asc_storealign_1st_postupdate](reg_compute/store/asc_storealign_1st_postupdate.md)|A Reg computation data movement API that stores the first element of src to dst, ignoring mask, with Post Update enabled.|`c_api/reg_compute/store/storealign.h`|
|[asc_storealign_intlv](reg_compute/store/asc_storealign_intlv.md)|A Reg computation data movement API that interleaves elements from src0 and src1 and stores them to UB.|`c_api/reg_compute/store/storealign.h`|
|[asc_storealign_pack](reg_compute/store/asc_storealign_pack.md)|A Reg computation data movement API that contiguously stores the lower half of the valid elements in src into dst based on mask.|`c_api/reg_compute/store/storealign.h`|
|[asc_storealign_pack_postupdate](reg_compute/store/asc_storealign_pack_postupdate.md)|A Reg computation data movement API that performs compressed store based on mask, with Post Update enabled.|`c_api/reg_compute/store/storealign.h`|
|[asc_storealign_pack_quarter](reg_compute/store/asc_storealign_pack_quarter.md)|A Reg computation data movement API that contiguously stores the lower 8 bits of the valid elements in src into dst based on mask.|`c_api/reg_compute/store/storealign.h`|
|[asc_storealign_pack_quarter_postupdate](reg_compute/store/asc_storealign_pack_quarter_postupdate.md)|A Reg computation data movement API that contiguously stores the lower 8 bits of the valid elements in src into dst based on mask, with Post Update enabled.|`c_api/reg_compute/store/storealign.h`|
|[asc_storealign_postupdate](reg_compute/store/asc_storealign_postupdate.md)|A Reg computation data movement API that stores data contiguously in an aligned manner from vector data registers to UB in NORM store mode, with Post Update enabled.|`c_api/reg_compute/store/storealign.h`|
|[asc_storealign_squeeze_status](reg_compute/store/asc_storealign_squeeze_status.md)|Writes the valid data length stored in the AR special register after the squeeze operation to UB.|`c_api/reg_compute/store/storealign.h`|
|[asc_storealign_squeeze_status_postupdate](reg_compute/store/asc_storealign_squeeze_status_postupdate.md)|Writes the valid data length stored in the AR special register after the squeeze operation to UB, with automatic destination address update.|`c_api/reg_compute/store/storealign.h`|
|[asc_store](reg_compute/store/asc_store.md)|A Reg computation data movement API. It is suitable for the scenario of storing data from a vector data register to UB, regardless of alignment. When pursuing ultimate performance, avoid using this API as much as possible.|`c_api/composite/storeunalign_composite.h`|
|[asc_storeunalign](reg_compute/store/asc_storeunalign.md)|A Reg computation data movement API, applicable to scenarios where data is contiguously stored from a vector data register to a UB address that is not aligned to 32 bytes.|`c_api/reg_compute/store/storeunalign.h`|
|[asc_storeunalign_post](reg_compute/store/asc_storeunalign_post.md)|A Reg computation data movement API, applicable to the tail block scenario where data is stored from a vector data register to a UB start address that is not aligned to 32 bytes.|`c_api/reg_compute/store/storeunalign.h`|
|[asc_storeunalign_post_postupdate](reg_compute/store/asc_storeunalign_post_postupdate.md)|A Reg computation data movement API, applicable to the tail block scenario where data is stored from a vector data register to a UB start address that is not aligned to 32 bytes, with automatic destination operand address update.|`c_api/reg_compute/store/storeunalign.h`|
|[asc_storeunalign_postupdate](reg_compute/store/asc_storeunalign_postupdate.md)|A Reg computation data movement API, applicable to scenarios where data is contiguously stored from a vector data register to a UB address that is not aligned to 32 bytes.|`c_api/reg_compute/store/storeunalign.h`|

### Reg Scatter Store

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_scatter](reg_compute/scatter/asc_scatter.md)|Scatters the elements of the source operand src into the destination operand dst based on the index value index.|`c_api/reg_compute/scatter/ub_scatter.h`|

### Reg-to-Reg Transfer

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_copy](reg_compute/reg_copy/asc_copy.md)|Copies data within registers.|`c_api/reg_compute/reg_copy.h`|
|[asc_extract_mask](reg_compute/reg_copy/asc_extract_mask.md)|Extracts the mask register from a vector data register and returns the result through the function return value.|`c_api/reg_compute/reg_copy.h`|
|[asc_mask](reg_compute/reg_copy/asc_mask.md)|Returns the mask copy result of a vector data register or mask register through the function return value.|`c_api/reg_compute/reg_copy.h`|

### Basic Arithmetic

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_abs](reg_compute/reg_arith/asc_abs.md)|Computes the absolute value element-wise.|`c_api/reg_compute/compute/reg_arith.h`|
|[asc_add](reg_compute/reg_arith/asc_add.md)|Performs addition element-wise.|`c_api/reg_compute/compute/reg_arith.h`|
|[asc_add_scalar](reg_compute/reg_arith/asc_add_scalar.md)|Performs addition of a vector and a scalar element-wise.|`c_api/reg_compute/compute/reg_arith.h`|
|[asc_addc](reg_compute/reg_arith/asc_addc.md)|Performs addition with carry element-wise.|`c_api/reg_compute/compute/reg_arith.h`|
|[asc_div](reg_compute/reg_arith/asc_div.md)|Performs division element-wise.|`c_api/reg_compute/compute/reg_arith.h`|
|[asc_exp](reg_compute/reg_arith/asc_exp.md)|Computes the exponent of the source operand element-wise.|`c_api/reg_compute/compute/reg_arith.h`|
|[asc_ln](reg_compute/reg_arith/asc_ln.md)|Computes the natural logarithm of the source operand element-wise.|`c_api/reg_compute/compute/reg_arith.h`|
|[asc_log](reg_compute/reg_arith/asc_log.md)|Computes the natural logarithm of the source operand element-wise.|`c_api/reg_compute/compute/reg_arith.h`|
|[asc_max](reg_compute/reg_arith/asc_max.md)|Computes the maximum value element-wise.|`c_api/reg_compute/compute/reg_arith.h`|
|[asc_max_scalar](reg_compute/reg_arith/asc_max_scalar.md)|Computes the maximum value of a vector and a scalar element-wise.|`c_api/reg_compute/compute/reg_arith.h`|
|[asc_min](reg_compute/reg_arith/asc_min.md)|Performs an element-wise minimum operation on the source operands src0 and src1 based on mask, and writes the result to the destination operand dst.|`c_api/reg_compute/compute/reg_arith.h`|
|[asc_min_scalar](reg_compute/reg_arith/asc_min_scalar.md)|Compares each element in the source operand vector with a scalar and takes the smaller value.|`c_api/reg_compute/compute/reg_arith.h`|
|[asc_mul](reg_compute/reg_arith/asc_mul.md)|Performs a multiplication operation on the source operands src0 and src1, and writes the result to the destination operand dst.|`c_api/reg_compute/compute/reg_arith.h`|
|[asc_mul_scalar](reg_compute/reg_arith/asc_mul_scalar.md)|Performs an element-wise multiplication operation on the source operands src and value, and writes the result to the destination operand dst.|`c_api/reg_compute/compute/reg_arith.h`|
|[asc_mull](reg_compute/reg_arith/asc_mull.md)|Performs element-wise multiplication on the input data src0 and src1 based on mask, writes the result to dst0, and writes the overflow portion to dst1.|`c_api/reg_compute/compute/reg_arith.h`|
|[asc_neg](reg_compute/reg_arith/asc_neg.md)|Performs a negation operation on the source operand src based on mask, and writes the result to the destination operand dst.|`c_api/reg_compute/compute/reg_arith.h`|
|[asc_sqrt](reg_compute/reg_arith/asc_sqrt.md)|Computes the square root element-wise.|`c_api/reg_compute/compute/reg_arith.h`|
|[asc_sub](reg_compute/reg_arith/asc_sub.md)|Performs subtraction element-wise.|`c_api/reg_compute/compute/reg_arith.h`|
|[asc_subc](reg_compute/reg_arith/asc_subc.md)|Performs subtraction with borrow element-wise.|`c_api/reg_compute/compute/reg_arith.h`|

### Logical Computation

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_and](reg_compute/reg_logic/asc_and.md)|Operates on the mask register: performs a logical AND operation on the valid bits of the source operands src0 and src1 based on mask to obtain a new mask register.|`c_api/reg_compute/compute/reg_logic.h`|
|[asc_not](reg_compute/reg_logic/asc_not.md)|Performs a vector NOT operation.|`c_api/reg_compute/compute/reg_logic.h`|
|[asc_or](reg_compute/reg_logic/asc_or.md)|Performs an OR (\|) operation on the source operands src0 and src1, and writes the result to the destination operand dst.|`c_api/reg_compute/compute/reg_logic.h`|
|[asc_shiftleft](reg_compute/reg_logic/asc_shiftleft.md)|Performs a left shift on input data src0 according to the corresponding elements of src1 based on the mask, and writes the result to dst.|`c_api/reg_compute/compute/reg_logic.h`|
|[asc_shiftleft_scalar](reg_compute/reg_logic/asc_shiftleft_scalar.md)|Performs a left shift on the source operand src based on mask. The number of bits to shift is determined by the input parameter value, and the result is written to the destination operand dst.|`c_api/reg_compute/compute/reg_logic.h`|
|[asc_shiftright](reg_compute/reg_logic/asc_shiftright.md)|Performs a right shift on input data src0 according to the corresponding elements of src1 based on the mask, and writes the result to dst.|`c_api/reg_compute/compute/reg_logic.h`|
|[asc_shiftright_scalar](reg_compute/reg_logic/asc_shiftright_scalar.md)|Performs a right shift on the source operand src based on mask, with the number of shift bits determined by the input parameter value, and writes the result to the destination operand dst.|`c_api/reg_compute/compute/reg_logic.h`|
|[asc_xor](reg_compute/reg_logic/asc_xor.md)|Performs an element-wise XOR (^) operation on the input src0 and src1 based on mask, and writes the result to dst.|`c_api/reg_compute/compute/reg_logic.h`|

### Reduction Computation

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_pair_reduce_sum](reg_compute/reg_reduce/asc_pair_reduce_sum.md)|PairReduceSum: sums two adjacent (odd-even) elements and writes the result to dst.|`c_api/reg_compute/compute/reg_reduce.h`|
|[asc_reduce_max](reg_compute/reg_reduce/asc_reduce_max.md)|Performs a reduction maximum operation on the source operand src based on mask, and writes the result to the destination operand dst.|`c_api/reg_compute/compute/reg_reduce.h`|
|[asc_reduce_max_datablock](reg_compute/reg_reduce/asc_reduce_max_datablock.md)|Finds the maximum value in the DataBlock and stores the final computation results sequentially starting from the least significant bit of dst.|`c_api/reg_compute/compute/reg_reduce.h`|
|[asc_reduce_min](reg_compute/reg_reduce/asc_reduce_min.md)|Performs a reduction minimum operation on the source operand src based on mask, and writes the result to the destination operand dst.|`c_api/reg_compute/compute/reg_reduce.h`|
|[asc_reduce_min_datablock](reg_compute/reg_reduce/asc_reduce_min_datablock.md)|Based on mask, stores the minimum value in each DataBlock (32 bytes) sequentially starting from the least significant bit of dst.|`c_api/reg_compute/compute/reg_reduce.h`|
|[asc_reduce_sum](reg_compute/reg_reduce/asc_reduce_sum.md)|Performs reduction sum, which sums all elements in src that participate in the computation and stores the result in dst.|`c_api/reg_compute/compute/reg_reduce.h`|
|[asc_reduce_sum_datablock](reg_compute/reg_reduce/asc_reduce_sum_datablock.md)|Performs reduction sum, which sums the elements in each DataBlock (32B) of src that participate in the computation and stores the results in dst in sequence.|`c_api/reg_compute/compute/reg_reduce.h`|

### Aggregation Operation

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_gather](reg_compute/reg_gather/asc_gather.md)|Collects the source operand src into the destination operand dst element-wise based on the index position index (register source gather mode).|`c_api/reg_compute/compute/reg_gather.h`|

### Broadcast Operation

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_duplicate](reg_compute/reg_broadcast/asc_duplicate.md)|Pads the destination operand dst with the lowest-position element of the source operand src based on mask.|`c_api/reg_compute/compute/reg_broadcast.h`|
|[asc_duplicate_highest](reg_compute/reg_broadcast/asc_duplicate_highest.md)|Pads the destination operand dst with the highest-position element of the source operand src based on mask, and sets the elements in dst not selected by mask to 0.|`c_api/reg_compute/compute/reg_broadcast.h`|
|[asc_duplicate_highest_merge](reg_compute/reg_broadcast/asc_duplicate_highest_merge.md)|Pads the destination operand dst with the highest-position element of the source operand src based on mask, and retains the original values of the elements in dst not selected by mask.|`c_api/reg_compute/compute/reg_broadcast.h`|
|[asc_duplicate_merge](reg_compute/reg_broadcast/asc_duplicate_merge.md)|Pads the destination operand dst with the lowest-position element of the source operand src based on mask, and retains the original values of the elements in dst not selected by mask.|`c_api/reg_compute/compute/reg_broadcast.h`|
|[asc_duplicate_scalar](reg_compute/reg_broadcast/asc_duplicate_scalar.md)|Pads dst with value according to mask.|`c_api/reg_compute/compute/reg_broadcast.h`|
|[asc_duplicate_scalar_merge](reg_compute/reg_broadcast/asc_duplicate_scalar_merge.md)|Pads the destination operand dst with value based on mask, and retains the original values of the elements in dst not selected by mask.|`c_api/reg_compute/compute/reg_broadcast.h`|

### Composite Computation

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_abs_sub](reg_compute/reg_fused/asc_abs_sub.md)|Computes the absolute value of the difference element-wise.|`c_api/reg_compute/compute/reg_fused.h`|
|[asc_axpy](reg_compute/reg_fused/asc_axpy.md)|Performs a multiply-accumulate operation on the source operands src and value element-wise based on mask, and writes the result to the destination operand dst.|`c_api/reg_compute/compute/reg_fused.h`|
|[asc_fma](reg_compute/reg_fused/asc_fma.md)|Element-wise multiply-add operation.|`c_api/reg_compute/compute/reg_fused.h`|
|[asc_leakyrelu](reg_compute/reg_fused/asc_leakyrelu.md)|Performs the Leaky ReLU (Leaky Rectified Linear Unit) operation element-wise.|`c_api/reg_compute/compute/reg_fused.h`|
|[asc_madd](reg_compute/reg_fused/asc_madd.md)|madd (multiply-add): performs element-wise multiplication and addition on the source operands.|`c_api/reg_compute/compute/reg_fused.h`|
|[asc_mul_scalar_float2half_rn](reg_compute/reg_fused/asc_mul_scalar_float2half_rn.md)|Multiplies float source operands element-wise by a scalar, then converts the result to the half type using the RINT rounding mode.|`c_api/reg_compute/compute/reg_fused.h`|
|[asc_mula](reg_compute/reg_fused/asc_mula.md)|Multiplies two source registers element-wise according to the mask and accumulates the result with the destination register.|`c_api/reg_compute/compute/reg_fused.h`|
|[asc_prelu](reg_compute/reg_fused/asc_prelu.md)|If the source operand src0 is greater than 0, this API writes src0 directly to the destination operand dst; otherwise, it writes the result of src0 * src1 to dst.|`c_api/reg_compute/compute/reg_fused.h`|
|[asc_relu](reg_compute/reg_fused/asc_relu.md)|Performs the ReLU operation element-wise.|`c_api/reg_compute/compute/reg_fused.h`|
|[asc_exp_sub](reg_compute/reg_fused/asc_exp_sub.md)|Subtracts src1 from src0 element-wise according to mask, and computes the natural exponential function of the difference.|`c_api/reg_compute/compute/reg_fused.h`|

### Comparison Computation

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_eq](reg_compute/reg_compare/asc_eq.md)|Compares the source operands element-wise for equality.|`c_api/reg_compute/compute/reg_compare.h`|
|[asc_eq_scalar](reg_compute/reg_compare/asc_eq_scalar.md)|Compares the source operand element-wise for equality with a scalar.|`c_api/reg_compute/compute/reg_compare.h`|
|[asc_ge](reg_compute/reg_compare/asc_ge.md)|ge (greater than or equal to): performs an element-wise comparison on the source operands.|`c_api/reg_compute/compute/reg_compare.h`|
|[asc_ge_scalar](reg_compute/reg_compare/asc_ge_scalar.md)|ge (greater than or equal to): performs an element-wise comparison between the source operand and a scalar.|`c_api/reg_compute/compute/reg_compare.h`|
|[asc_gt](reg_compute/reg_compare/asc_gt.md)|gt (greater than): performs an element-wise comparison on the source operands.|`c_api/reg_compute/compute/reg_compare.h`|
|[asc_gt_scalar](reg_compute/reg_compare/asc_gt_scalar.md)|gt (greater than): performs an element-wise comparison between the source operand and a scalar.|`c_api/reg_compute/compute/reg_compare.h`|
|[asc_le](reg_compute/reg_compare/asc_le.md)|le (less than or equal to): performs an element-wise comparison on the source operands.|`c_api/reg_compute/compute/reg_compare.h`|
|[asc_le_scalar](reg_compute/reg_compare/asc_le_scalar.md)|le (less than or equal to): performs an element-wise comparison between the source operand and a scalar.|`c_api/reg_compute/compute/reg_compare.h`|
|[asc_lt](reg_compute/reg_compare/asc_lt.md)|lt (less than): performs an element-wise comparison on the source operands.|`c_api/reg_compute/compute/reg_compare.h`|
|[asc_lt_scalar](reg_compute/reg_compare/asc_lt_scalar.md)|lt (less than): performs an element-wise comparison between the source operand and a scalar.|`c_api/reg_compute/compute/reg_compare.h`|
|[asc_ne](reg_compute/reg_compare/asc_ne.md)|ne (not equal): performs an element-wise comparison on the source operands.|`c_api/reg_compute/compute/reg_compare.h`|
|[asc_ne_scalar](reg_compute/reg_compare/asc_ne_scalar.md)|Compares each element in src with the scalar value one by one. If src_i != value, the corresponding bit of the output result dst is 1; otherwise, it is 0.|`c_api/reg_compute/compute/reg_compare.h`|

### Layout Transformation

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_deintlv](reg_compute/reg_permute_sel/asc_deintlv.md)|Given the source operands src0 and src1, deinterleaves the elements in src0 and src1 and stores them into the result operands dst0 and dst1.|`c_api/reg_compute/compute/reg_permute_sel.h`|
|[asc_intlv](reg_compute/reg_permute_sel/asc_intlv.md)|Interleaves the elements in the source operands src0 and src1 and stores them into the destination operands dst0 and dst1.|`c_api/reg_compute/compute/reg_permute_sel.h`|
|[asc_pack_to_high](reg_compute/reg_permute_sel/asc_pack_to_high.md)|Selects the low 8 bits (b16) or low 16 bits (b32) of each element in the source operand and writes them to the upper half of the destination operand (for mask register operands, selects the even-indexed bits).|`c_api/reg_compute/compute/reg_permute_sel.h`|
|[asc_pack_to_low](reg_compute/reg_permute_sel/asc_pack_to_low.md)|Selects the low 8 bits (b16) or low 16 bits (b32) of each element in the source operand and writes them to the lower half of the destination operand (for mask register operands, selects the even-indexed bits).|`c_api/reg_compute/compute/reg_permute_sel.h`|
|[asc_select](reg_compute/reg_permute_sel/asc_select.md)|Selects elements from source operands src0 and src1 based on the bit values of mask to obtain destination operand dst.|`c_api/reg_compute/compute/reg_permute_sel.h`|
|[asc_squeeze](reg_compute/reg_permute_sel/asc_squeeze.md)|Copies the valid elements selected by mask from src to dst in sequence, with the valid elements arranged contiguously from low to high and the remaining positions set to 0.|`c_api/reg_compute/compute/reg_permute_sel.h`|
|[asc_squeeze_and_storeunalign](reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign.md)|A Reg computation data movement API, applicable to scenarios where valid elements are filtered from a vector data register and then contiguously stored to UB starting at an address that is not aligned to 32 bytes.|`c_api/reg_compute/compute/reg_permute_sel.h`|
|[asc_squeeze_and_storeunalign_finalize](reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign_finalize.md)|A Reg computation data movement API, applicable to the tail block scenario where valid elements are filtered from a vector data register and then contiguously stored to UB starting at an address that is not aligned to 32 bytes.|`c_api/reg_compute/compute/reg_permute_sel.h`|
|[asc_squeeze_and_storeunalign_init](reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign_init.md)|Clears the AR special register. The AR register is used together with [asc_squeeze_and_storeunalign](reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign.md) and [asc_squeeze_and_storeunalign_finalize](reg_compute/reg_permute_sel/asc_squeeze_and_storeunalign_finalize.md).|`c_api/reg_compute/compute/reg_permute_sel.h`|
|[asc_unpack](reg_compute/reg_permute_sel/asc_unpack.md)|Performs vector unpacking.|`c_api/reg_compute/compute/reg_permute_sel.h`|
|[asc_unsqueeze](reg_compute/reg_permute_sel/asc_unsqueeze.md)|Performs decompression based on mask and outputs the generated data to dst.|`c_api/reg_compute/compute/reg_permute_sel.h`|

### Index Operations

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_arange](reg_compute/reg_index/asc_arange.md)|Generates incrementing/decrementing indices starting from the passed value, and stores the generated indices in dst.|`c_api/reg_compute/compute/reg_index.h`|

### Histogram

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_cumulative_histogram](reg_compute/reg_histogram/asc_cumulative_histogram.md)|Performs cumulative statistics on histogram data.|`c_api/reg_compute/compute/reg_histogram.h`|
|[asc_frequency_histogram](reg_compute/reg_histogram/asc_frequency_histogram.md)|Performs frequency statistics on histogram data.|`c_api/reg_compute/compute/reg_histogram.h`|

### Mask Register Operations

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_create_mask](reg_compute/reg_mask/asc_create_mask.md)|Generates the corresponding mask register based on the input parameters.|`c_api/reg_compute/compute/reg_mask.h`|
|[asc_get_mask_spr](reg_compute/reg_mask/asc_get_mask_spr.md)|Reads the mask value from the special registers SPR{MASK1, MASK0} and returns the corresponding mask data according to the data type format. Both MASK0 and MASK1 are 64-bit registers.|`c_api/reg_compute/compute/reg_mask.h`|
|[asc_update_mask](reg_compute/reg_mask/asc_update_mask.md)|Generates the value in the corresponding mask register based on the size of value.|`c_api/reg_compute/compute/reg_mask.h`|

### Address Register Operations

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_update_addr_reg](reg_compute/reg_addr_reg/asc_update_addr_reg.md)|The address register is initialized through this API, and then the address register is used to store the address offset within the loop.|`c_api/reg_compute/compute/reg_addr_reg.h`|

### Type Conversion

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_bfloat162e1m2x2](reg_compute/reg_convert/asc_bfloat162e1m2x2.md)|Converts the bfloat16_t type to the fp4x2_e1m2_t type and supports specifying the write position of the conversion result.|`c_api/reg_compute/reg_convert.h`|
|[asc_bfloat162e2m1x2](reg_compute/reg_convert/asc_bfloat162e2m1x2.md)|Converts the bfloat16_t type to the fp4x2_e2m1_t type and supports specifying the write position of the conversion result.|`c_api/reg_compute/reg_convert.h`|
|[asc_bfloat162float](reg_compute/reg_convert/asc_bfloat162float.md)|Converts the bfloat16_t type to the float type without a rounding mode.|`c_api/reg_compute/reg_convert.h`|
|[asc_bfloat162half](reg_compute/reg_convert/asc_bfloat162half.md)|Converts the bfloat16_t type to the half type and supports multiple rounding modes as well as saturation and non-saturation modes.|`c_api/reg_compute/reg_convert.h`|
|[asc_bfloat162int32](reg_compute/reg_convert/asc_bfloat162int32.md)|Converts data of the bfloat16_t type to the int32_t type and supports multiple rounding modes.|`c_api/reg_compute/reg_convert.h`|
|[asc_ceil](reg_compute/reg_convert/asc_ceil.md)|Rounds the floating-point elements in the source operand to integer values using the CEIL (round toward positive infinity) mode, with the result retaining the original floating-point data type.|`c_api/reg_compute/reg_convert.h`|
|[asc_e1m2x22bfloat16](reg_compute/reg_convert/asc_e1m2x22bfloat16.md)|Converts the fp4x2_e1m2_t type to the bfloat16_t type and supports specifying the read position of the source data.|`c_api/reg_compute/reg_convert.h`|
|[asc_e2m1x22bfloat16](reg_compute/reg_convert/asc_e2m1x22bfloat16.md)|Converts the fp4x2_e2m1_t type to the bfloat16_t type and supports specifying the read position of the source data.|`c_api/reg_compute/reg_convert.h`|
|[asc_e4m32float](reg_compute/reg_convert/asc_e4m32float.md)|Converts the fp8_e4m3fn_t type to the float type and supports specifying the read position of the source data.|`c_api/reg_compute/reg_convert.h`|
|[asc_e5m22float](reg_compute/reg_convert/asc_e5m22float.md)|Converts the fp8_e5m2_t type to the float type and supports specifying the read position of the source data.|`c_api/reg_compute/reg_convert.h`|
|[asc_float2bfloat16](reg_compute/reg_convert/asc_float2bfloat16.md)|Converts the float type to the bfloat16_t type and supports multiple rounding modes.|`c_api/reg_compute/reg_convert.h`|
|[asc_float2e4m3](reg_compute/reg_convert/asc_float2e4m3.md)|Converts the float type to the fp8_e4m3fn_t type and supports specifying the write position of the conversion result.|`c_api/reg_compute/reg_convert.h`|
|[asc_float2e5m2](reg_compute/reg_convert/asc_float2e5m2.md)|Converts the float type to the fp8_e5m2_t type and supports specifying the write position of the conversion result.|`c_api/reg_compute/reg_convert.h`|
|[asc_float2half](reg_compute/reg_convert/asc_float2half.md)|Converts the float type to the half type and supports multiple rounding modes.|`c_api/reg_compute/reg_convert.h`|
|[asc_float2hif8](reg_compute/reg_convert/asc_float2hif8.md)|Converts the float type to the hifloat8_t type and supports specifying the write position of the conversion result.|`c_api/reg_compute/reg_convert.h`|
|[asc_float2int16](reg_compute/reg_convert/asc_float2int16.md)|Converts the float type to the int16_t type and supports multiple rounding modes.|`c_api/reg_compute/reg_convert.h`|
|[asc_float2int32](reg_compute/reg_convert/asc_float2int32.md)|Converts the float type to the int32_t type and supports multiple rounding modes.|`c_api/reg_compute/reg_convert.h`|
|[asc_float2int64](reg_compute/reg_convert/asc_float2int64.md)|Converts the float type to the int64_t type and supports multiple rounding modes.|`c_api/reg_compute/reg_convert.h`|
|[asc_floor](reg_compute/reg_convert/asc_floor.md)|Rounds the floating-point elements in the source operand to integer values using the FLOOR (round toward negative infinity) mode, with the result retaining the original floating-point data type.|`c_api/reg_compute/reg_convert.h`|
|[asc_half2bfloat16](reg_compute/reg_convert/asc_half2bfloat16.md)|Converts data of the half type to the bfloat16_t type and supports multiple rounding modes.|`c_api/reg_compute/reg_convert.h`|
|[asc_half2float](reg_compute/reg_convert/asc_half2float.md)|Converts data of the half type to the float type.|`c_api/reg_compute/reg_convert.h`|
|[asc_half2hif8](reg_compute/reg_convert/asc_half2hif8.md)|Converts the half type to the hifloat8_t type and supports multiple rounding modes.|`c_api/reg_compute/reg_convert.h`|
|[asc_half2int16](reg_compute/reg_convert/asc_half2int16.md)|Converts data of the half type to the int16_t type and supports multiple rounding modes.|`c_api/reg_compute/reg_convert.h`|
|[asc_half2int32](reg_compute/reg_convert/asc_half2int32.md)|Converts data of the half type to the int32_t type and supports multiple rounding modes.|`c_api/reg_compute/reg_convert.h`|
|[asc_half2int4x2](reg_compute/reg_convert/asc_half2int4x2.md)|Converts the half type to the int4x2_t type and supports multiple rounding modes as well as saturation and non-saturation modes.|`c_api/reg_compute/reg_convert.h`|
|[asc_half2int8](reg_compute/reg_convert/asc_half2int8.md)|Converts the half type to the int8_t type and supports multiple rounding modes.|`c_api/reg_compute/reg_convert.h`|
|[asc_half2uint8](reg_compute/reg_convert/asc_half2uint8.md)|Converts the half type to the uint8_t type and supports multiple rounding modes.|`c_api/reg_compute/reg_convert.h`|
|[asc_hif82float](reg_compute/reg_convert/asc_hif82float.md)|Converts the hifloat8_t type to the float type and supports specifying the read position of the source data.|`c_api/reg_compute/reg_convert.h`|
|[asc_hif82half](reg_compute/reg_convert/asc_hif82half.md)|Converts data of the hifloat8_t type to the half type.|`c_api/reg_compute/reg_convert.h`|
|[asc_int162float](reg_compute/reg_convert/asc_int162float.md)|Converts the int16_t type to the float type.|`c_api/reg_compute/reg_convert.h`|
|[asc_int162half](reg_compute/reg_convert/asc_int162half.md)|Converts the int16_t type to the half type.|`c_api/reg_compute/reg_convert.h`|
|[asc_int162int32](reg_compute/reg_convert/asc_int162int32.md)|Converts the int16_t type to the int32_t type.|`c_api/reg_compute/reg_convert.h`|
|[asc_int162uint32](reg_compute/reg_convert/asc_int162uint32.md)|Converts the int16_t type to the uint32_t type.|`c_api/reg_compute/reg_convert.h`|
|[asc_int162uint8](reg_compute/reg_convert/asc_int162uint8.md)|Converts the int16_t type to the uint8_t type.|`c_api/reg_compute/reg_convert.h`|
|[asc_int322float](reg_compute/reg_convert/asc_int322float.md)|Converts the int32_t type to the float type and supports multiple rounding modes.|`c_api/reg_compute/reg_convert.h`|
|[asc_int322int16](reg_compute/reg_convert/asc_int322int16.md)|Converts the int32_t type to the int16_t type.|`c_api/reg_compute/reg_convert.h`|
|[asc_int322int64](reg_compute/reg_convert/asc_int322int64.md)|Converts the int32_t type to the int64_t type.|`c_api/reg_compute/reg_convert.h`|
|[asc_int322uint16](reg_compute/reg_convert/asc_int322uint16.md)|Converts the int32_t type to the uint16_t type.|`c_api/reg_compute/reg_convert.h`|
|[asc_int4x22bfloat16](reg_compute/reg_convert/asc_int4x22bfloat16.md)|Converts the int4x2_t type to the bfloat16_t type.|`c_api/reg_compute/reg_convert.h`|
|[asc_int4x22half](reg_compute/reg_convert/asc_int4x22half.md)|Converts the int4x2_t type to the half type.|`c_api/reg_compute/reg_convert.h`|
|[asc_int4x22int16](reg_compute/reg_convert/asc_int4x22int16.md)|Converts the int4x2_t type to the int16_t type.|`c_api/reg_compute/reg_convert.h`|
|[asc_int642float](reg_compute/reg_convert/asc_int642float.md)|Converts data of the int64_t type to the float type and supports multiple rounding modes.|`c_api/reg_compute/reg_convert.h`|
|[asc_int642int32](reg_compute/reg_convert/asc_int642int32.md)|Converts the int64_t type to the int32_t type.|`c_api/reg_compute/reg_convert.h`|
|[asc_int82half](reg_compute/reg_convert/asc_int82half.md)|Converts the int8_t type to the half type.|`c_api/reg_compute/reg_convert.h`|
|[asc_int82int16](reg_compute/reg_convert/asc_int82int16.md)|Converts the int8_t type to the int16_t type.|`c_api/reg_compute/reg_convert.h`|
|[asc_int82int32](reg_compute/reg_convert/asc_int82int32.md)|Converts data of the int8_t type to the int32_t type and supports specifying the read position of the source data.|`c_api/reg_compute/reg_convert.h`|
|[asc_rint](reg_compute/reg_convert/asc_rint.md)|Rounds the floating-point elements in the source operand to integer values in RINT (round half to even) mode, with the result retaining the original floating-point data type.|`c_api/reg_compute/reg_convert.h`|
|[asc_round](reg_compute/reg_convert/asc_round.md)|Rounds the floating-point elements in the source operand to integer values in ROUND (round half away from zero) mode, with the result retaining the original floating-point data type.|`c_api/reg_compute/reg_convert.h`|
|[asc_trunc](reg_compute/reg_convert/asc_trunc.md)|Truncates the floating-point elements in the source operand to integer values in TRUNC (truncate toward zero) mode, with the result retaining the original floating-point data type.|`c_api/reg_compute/reg_convert.h`|
|[asc_uint162uint32](reg_compute/reg_convert/asc_uint162uint32.md)|Converts data of the uint16_t type to the uint32_t type.|`c_api/reg_compute/reg_convert.h`|
|[asc_uint162uint8](reg_compute/reg_convert/asc_uint162uint8.md)|Converts data of the uint16_t type to the uint8_t type.|`c_api/reg_compute/reg_convert.h`|
|[asc_uint322int16](reg_compute/reg_convert/asc_uint322int16.md)|Converts data of the uint32_t type to the int16_t type.|`c_api/reg_compute/reg_convert.h`|
|[asc_uint322uint16](reg_compute/reg_convert/asc_uint322uint16.md)|Converts data of the uint32_t type to the uint16_t type.|`c_api/reg_compute/reg_convert.h`|
|[asc_uint322uint8](reg_compute/reg_convert/asc_uint322uint8.md)|Converts data of the uint32_t type to the uint8_t type and supports specifying the write position of the conversion result.|`c_api/reg_compute/reg_convert.h`|
|[asc_uint82half](reg_compute/reg_convert/asc_uint82half.md)|Converts data of the uint8_t type to the half type.|`c_api/reg_compute/reg_convert.h`|
|[asc_uint82uint16](reg_compute/reg_convert/asc_uint82uint16.md)|Converts data of the uint8_t type to the uint16_t type.|`c_api/reg_compute/reg_convert.h`|
|[asc_uint82uint32](reg_compute/reg_convert/asc_uint82uint32.md)|Converts data of the uint8_t type to the uint32_t type and supports specifying the read position of the source data.|`c_api/reg_compute/reg_convert.h`|

### Synchronization Control

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_mem_bar](reg_compute/reg_sync/asc_mem_bar.md)|Synchronization instruction between different pipelines within a Reg computation macro function.|`c_api/reg_compute/reg_sync.h`|

## Scalar Computation

### Bitwise Operations

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_clear_nthbit](scalar_compute/scalar_bit/asc_clear_nthbit.md)|A bit manipulation function that sets the idx-th bit of a uint64_t integer bits to 0.|`c_api/scalar_compute/compute/scalar_bit.h`|
|[asc_clz](scalar_compute/scalar_bit/asc_clz.md)|Computes the number of leading zeros of the parameter (the number of 0s from the most significant bit to the first 1 in binary).|`c_api/scalar_compute/compute/scalar_bit.h`|
|[asc_ffs](scalar_compute/scalar_bit/asc_ffs.md)|The FindFirstSet API searches the binary representation of the input data from the least significant bit to the most significant bit for the first bit whose value is 1, and returns its position. If no such bit is found, it returns -1.|`c_api/scalar_compute/compute/scalar_bit.h`|
|[asc_ffz](scalar_compute/scalar_bit/asc_ffz.md)|Obtains the position of the first 0, starting from the least significant bit, in the binary representation of a uint64_t number. Returns -1 if none is found.|`c_api/scalar_compute/compute/scalar_bit.h`|
|[asc_popc](scalar_compute/scalar_bit/asc_popc.md)|Obtains the number of 1s in the binary representation of a uint64_t number.|`c_api/scalar_compute/compute/scalar_bit.h`|
|[asc_set_nthbit](scalar_compute/scalar_bit/asc_set_nthbit.md)|Sets the specified bit of a uint64_t value to 1 while leaving the other bits unchanged.|`c_api/scalar_compute/compute/scalar_bit.h`|
|[asc_sflbits](scalar_compute/scalar_bit/asc_sflbits.md)|Computes the number of consecutive bits, starting from the most significant value bit, that are identical to the sign bit in the binary representation of an int64_t number.|`c_api/scalar_compute/compute/scalar_bit.h`|
|[asc_zero_bits_cnt](scalar_compute/scalar_bit/asc_zero_bits_cnt.md)|Obtains the number of 0s in the binary representation of a uint64_t value.|`c_api/scalar_compute/compute/scalar_bit.h`|

### Type Conversion

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_float2half](scalar_compute/scalar_convert/asc_float2half.md)|Converts a float scalar to the half type, with the rounding mode fixed to ODD (round to nearest odd).|`c_api/scalar_compute/compute/scalar_convert.h`|
|[asc_float2int32](scalar_compute/scalar_convert/asc_float2int32.md)|Converts float data to int32_t and supports multiple rounding modes.|`c_api/scalar_compute/compute/scalar_convert.h`|

### Data Load

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_load_dev](scalar_compute/scalar_load/asc_load_dev.md)|Reads integer data directly from a GM address without going through the DCache.|`c_api/scalar_compute/scalar_load.h`|

### Data Store

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_store_dev](scalar_compute/scalar_store/asc_store_dev.md)|Writes data to a GM address without going through the DCache.|`c_api/scalar_compute/scalar_store.h`|

## Atomic Operations

### Data Movement Atomic Operations

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_set_atomic_add](atomic/datamove_atomic/asc_set_atomic_add.md)|Enables atomic accumulation for subsequent data transfers from UB/L0C Buffer/L1 Buffer to Global Memory. The accumulation data type is int16_t.|`c_api/atomic/datamove_atomic.h`|
|[asc_set_atomic_max](atomic/datamove_atomic/asc_set_atomic_max.md)|Transfers the computation result to GM in an atomic comparison manner. Before copying, compares the int16_t data to be transferred with the existing data in GM element-wise, and writes the maximum value to GM.|`c_api/atomic/datamove_atomic.h`|
|[asc_set_atomic_min](atomic/datamove_atomic/asc_set_atomic_min.md)|Transfers the computation result to GM in an atomic comparison manner. Before copying, compares the int16_t data to be transferred with the existing data in GM element-wise, and writes the minimum value to GM.|`c_api/atomic/datamove_atomic.h`|
|[asc_disable_dma_atomic](atomic/datamove_atomic/asc_disable_dma_atomic.md)|Clears the atomic operation state.|`c_api/atomic/datamove_atomic.h`|

### Scalar Atomic Operations

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_atomic_add](atomic/scalar_atomic/asc_atomic_add.md)|Performs an atomic addition operation on the data in Global Memory and the specified data.|`c_api/atomic/scalar_atomic.h`|
|[asc_atomic_and](atomic/scalar_atomic/asc_atomic_and.md)|Performs an atomic AND operation on the data in Global Memory and the specified data.|`c_api/atomic/scalar_atomic.h`|
|[asc_atomic_cas](atomic/scalar_atomic/asc_atomic_cas.md)|Performs an atomic compare-and-swap operation on the element pointed to by address in Global Memory. If the value at address equals the specified value compare, address is assigned the specified value val; otherwise, the value at address remains unchanged.|`c_api/atomic/scalar_atomic.h`|
|[asc_atomic_dec](atomic/scalar_atomic/asc_atomic_dec.md)|Performs an atomic decrement operation on the counter pointed to by address in Global Memory. If the value at address equals 0 or is greater than the specified value val, val is assigned to address; otherwise, the value at address is decremented by 1.|`c_api/atomic/scalar_atomic.h`|
|[asc_atomic_exch](atomic/scalar_atomic/asc_atomic_exch.md)|Performs an atomic assignment operation on the element pointed to by address in Global Memory.|`c_api/atomic/scalar_atomic.h`|
|[asc_atomic_inc](atomic/scalar_atomic/asc_atomic_inc.md)|Performs an atomic increment operation on the counter pointed to by address in Global Memory. If the value at address is greater than or equal to the specified value val, address is set to 0; otherwise, the value at address is incremented by 1.|`c_api/atomic/scalar_atomic.h`|
|[asc_atomic_max](atomic/scalar_atomic/asc_atomic_max.md)|Performs an atomic maximum operation on the data in Global Memory and the specified data.|`c_api/atomic/scalar_atomic.h`|
|[asc_atomic_min](atomic/scalar_atomic/asc_atomic_min.md)|Performs an atomic minimum operation on the data in Global Memory and the specified data.|`c_api/atomic/scalar_atomic.h`|
|[asc_atomic_or](atomic/scalar_atomic/asc_atomic_or.md)|Performs an atomic OR operation on the data in Global Memory and the specified data.|`c_api/atomic/scalar_atomic.h`|
|[asc_atomic_sub](atomic/scalar_atomic/asc_atomic_sub.md)|Performs an atomic subtraction operation on the data in Global Memory and the specified data.|`c_api/atomic/scalar_atomic.h`|
|[asc_atomic_xor](atomic/scalar_atomic/asc_atomic_xor.md)|Performs an atomic XOR operation on the data in Global Memory and the specified data.|`c_api/atomic/scalar_atomic.h`|
|[asc_get_store_atomic_config](atomic/scalar_atomic/asc_get_store_atomic_config.md)|Obtains the values of the atomic operation enable bit and the atomic operation type.|`c_api/atomic/scalar_atomic.h`|
|[asc_set_store_atomic_config_v1](atomic/scalar_atomic/asc_set_store_atomic_config_v1.md)|Sets the atomic operation enable bit and the atomic operation type value. This applies to Atlas A3 products and Atlas A2 products.|`c_api/atomic/scalar_atomic.h`|

## Cache Control

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_datacache_preload](cache_ctrl/asc_datacache_preload.md)|Preloads data from the specific GM address where the source address is located into the Data Cache.|`c_api/cache_ctrl/cache_ctrl.h`|
|[asc_dcci](cache_ctrl/asc_dcci.md)|Flushes the cache to ensure cache coherence.|`c_api/cache_ctrl/cache_ctrl.h`|
|[asc_dci](cache_ctrl/asc_dci.md)|Invalidates the data cache, making the entire data cache invalid.|`c_api/cache_ctrl/cache_ctrl.h`|
|[asc_get_icache_preload_status](cache_ctrl/asc_get_icache_preload_status.md)|Obtains the Preload status of the ICache.|`c_api/cache_ctrl/cache_ctrl.h`|
|[asc_get_scalar_load_cache_mode](cache_ctrl/asc_get_scalar_load_cache_mode.md)|Obtains the L2 Cache management policy of the scalar load path.|`c_api/cache_ctrl/cache_ctrl.h`|
|[asc_get_scalar_store_cache_mode](cache_ctrl/asc_get_scalar_store_cache_mode.md)|Obtains the L2 Cache management policy of the scalar store path.|`c_api/cache_ctrl/cache_ctrl.h`|
|[asc_icache_preload](cache_ctrl/asc_icache_preload.md)|Preloads data from the GM address specified by the instruction into the corresponding cacheline.|`c_api/cache_ctrl/cache_ctrl.h`|
|[asc_set_scalar_cache_mode](cache_ctrl/asc_set_scalar_cache_mode.md)|Configures the L2 Cache management policy when the scalar unit accesses Global Memory.|`c_api/cache_ctrl/cache_ctrl.h`|

## Synchronization Control

### Intra-Core Synchronization

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_lock](sync/intra_core_sync/asc_lock.md)|API used for asynchronous pipeline synchronization inside the AI Core.|`c_api/sync/sync.h`|
|[asc_sync](sync/intra_core_sync/asc_sync.md)|Waits for all pipeline operations to complete.|`c_api/sync/sync.h`|
|[asc_sync_data_barrier](sync/intra_core_sync/asc_sync_data_barrier.md)|Blocks the execution of subsequent instructions until all preceding memory access instructions (the memory locations to wait for can be controlled by parameters) have completed.|`c_api/sync/sync.h`|
|[asc_sync_mte2](sync/intra_core_sync/asc_sync_mte2.md)|Waits for the PIPE_MTE2 pipeline to complete.|`c_api/sync/sync.h`|
|[asc_sync_mte3](sync/intra_core_sync/asc_sync_mte3.md)|Waits for the PIPE_MTE3 pipeline to complete.|`c_api/sync/sync.h`|
|[asc_sync_notify](sync/intra_core_sync/asc_sync_notify.md)|Sets the synchronization flag.|`c_api/sync/sync.h`|
|[asc_sync_pipe](sync/intra_core_sync/asc_sync_pipe.md)|Waits for the specified pipeline operation to complete.|`c_api/sync/sync.h`|
|[asc_sync_vec](sync/intra_core_sync/asc_sync_vec.md)|Synchronizes all pipelines.|`c_api/sync/sync.h`|
|[asc_sync_wait](sync/intra_core_sync/asc_sync_wait.md)|Waits for the synchronization flag.|`c_api/sync/sync.h`|
|[asc_unlock](sync/intra_core_sync/asc_unlock.md)|Releases the cache.|`c_api/sync/sync.h`|

### Inter-Core Synchronization

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_sync_block_arrive](sync/inter_core_sync/asc_sync_block_arrive.md)|Sends synchronization information data to the inter-core synchronization register to set a synchronization point.|`c_api/sync/sync.h`|
|[asc_sync_block_wait](sync/inter_core_sync/asc_sync_block_wait.md)|Used together with [asc_sync_block_arrive](sync/inter_core_sync/asc_sync_block_arrive.md) (associated through flag_id) to wait for all synchronization objects to arrive at the synchronization point corresponding to flag_id.|`c_api/sync/sync.h`|
|[asc_sync_inter_arrive](sync/inter_core_sync/asc_sync_inter_arrive.md)|Used together with [asc_sync_inter_wait](sync/inter_core_sync/asc_sync_inter_wait.md) (associated through flag_id) for signal synchronization between blocks in a group.|`c_api/sync/sync.h`|
|[asc_sync_inter_wait](sync/inter_core_sync/asc_sync_inter_wait.md)|Waits for the synchronization flag within a block.|`c_api/sync/sync.h`|
|[asc_sync_intra_arrive](sync/inter_core_sync/asc_sync_intra_arrive.md)|Sends a synchronization signal to the inter-core synchronization register.|`c_api/sync/sync.h`|
|[asc_sync_intra_wait](sync/inter_core_sync/asc_sync_intra_wait.md)|Waits for the synchronization flag of the inter-core synchronization register.|`c_api/sync/sync.h`|
|[asc_sync_subblock_arrive](sync/inter_core_sync/asc_sync_subblock_arrive.md)|Used together with [asc_sync_subblock_wait](sync/inter_core_sync/asc_sync_subblock_wait.md) (associated through flag_id) for signal synchronization between subblocks in a group.|`c_api/sync/sync.h`|
|[asc_sync_subblock_wait](sync/inter_core_sync/asc_sync_subblock_wait.md)|Waits for the synchronization flag between subblocks.|`c_api/sync/sync.h`|

## Other Operations

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[Instruction issue queue idle slot count query](experimental/asc_get_pipe_idle_slot_count.md)|Obtains the current number of idle slots in the issue queue corresponding to the specified pipeline.|`c_api/experimental/asc_get_pipe_idle_slot_count.h`|

## Special Register Access

|API|Description|Finest-Grained Public Header File|
|---|---|---|
|[asc_get_ctrl](spr/asc_get_ctrl.md)|Reads the value of the CTRL register (control register).|`c_api/spr/spr.h`|
|[asc_get_saturation_flag](spr/asc_get_saturation_flag.md)|Reads the on/off state of the specified saturation mode.|`c_api/spr/spr.h`|
|[asc_get_saturation_strategy](spr/asc_get_saturation_strategy.md)|Reads the saturation control policy.|`c_api/spr/spr.h`|
|[asc_get_squeeze_status](spr/asc_get_squeeze_status.md)|Reads the valid data length value saved to the AR special register after the squeeze operation.|`c_api/spr/spr.h`|
|[asc_set_ctrl](spr/asc_set_ctrl.md)|Sets the value of the CTRL register (control register).|`c_api/spr/spr.h`|
|[asc_set_saturation_flag](spr/asc_set_saturation_flag.md)|Sets the on/off state of the specified saturation mode.|`c_api/spr/spr.h`|
|[asc_set_saturation_strategy](spr/asc_set_saturation_strategy.md)|Sets the saturation control policy.|`c_api/spr/spr.h`|

## Deprecated APIs

| Deprecated APIs | Description |
| --- | --- |
| [asc_abs_sync (deprecated)](deprecated_interface/asc_abs_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_add_relu_sync (deprecated)](deprecated_interface/asc_add_relu_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_add_scalar_sync (deprecated)](deprecated_interface/asc_add_scalar_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_add_sync (deprecated)](deprecated_interface/asc_add_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_and (deprecated)](deprecated_interface/asc_and_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_and_sync (deprecated)](deprecated_interface/asc_and_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_axpy_sync (deprecated)](deprecated_interface/asc_axpy_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_bfloat162e1m2x2 (deprecated)](deprecated_interface/asc_bfloat162e1m2x2_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_bfloat162e2m1x2 (deprecated)](deprecated_interface/asc_bfloat162e2m1x2_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_bfloat162float (deprecated)](deprecated_interface/asc_bfloat162float_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_bfloat162float_sync (deprecated)](deprecated_interface/asc_bfloat162float_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_bfloat162int32 (deprecated)](deprecated_interface/asc_bfloat162int32_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_bfloat162int32_sync (deprecated)](deprecated_interface/asc_bfloat162int32_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_bitsort_sync (deprecated)](deprecated_interface/asc_bitsort_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_brcb_sync (deprecated)](deprecated_interface/asc_brcb_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_clear_ar_spr (deprecated)](deprecated_interface/asc_clear_ar_spr_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_gm2l0a_sync (deprecated)](deprecated_interface/asc_copy_gm2l0a_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_gm2l0b_sync (deprecated)](deprecated_interface/asc_copy_gm2l0b_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_gm2l1_align_sync (deprecated)](deprecated_interface/asc_copy_gm2l1_align_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_gm2l1_dn2nz_sync (deprecated)](deprecated_interface/asc_copy_gm2l1_dn2nz_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_gm2l1_nd2nz_sync_arch_2201 (deprecated)](deprecated_interface/asc_copy_gm2l1_nd2nz_sync_arch_2201_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_gm2l1_nd2nz_sync_arch_3510 (deprecated)](deprecated_interface/asc_copy_gm2l1_nd2nz_sync_arch_3510_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_gm2l1_sync_2d_arch_3510 (deprecated)](deprecated_interface/asc_copy_gm2l1_sync_2d_arch_3510_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_gm2l1_sync_arch_2201 (deprecated)](deprecated_interface/asc_copy_gm2l1_sync_arch_2201_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_gm2l1_sync_highdim_split_arch_3510 (deprecated)](deprecated_interface/asc_copy_gm2l1_sync_highdim_split_arch_3510_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_gm2ub_align_arch_3510 (deprecated)](deprecated_interface/asc_copy_gm2ub_align_arch_3510_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_gm2ub_align_sync_arch_2201 (deprecated)](deprecated_interface/asc_copy_gm2ub_align_sync_arch_2201_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_gm2ub_align_sync_arch_3510 (deprecated)](deprecated_interface/asc_copy_gm2ub_align_sync_arch_3510_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_gm2ub_sync_arch_2201 (deprecated)](deprecated_interface/asc_copy_gm2ub_sync_arch_2201_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_gm2ub_sync_arch_3510 (deprecated)](deprecated_interface/asc_copy_gm2ub_sync_arch_3510_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_l0c2gm_sync_arch_2201 (deprecated)](deprecated_interface/asc_copy_l0c2gm_sync_arch_2201_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_l0c2l1_sync_arch_2201 (deprecated)](deprecated_interface/asc_copy_l0c2l1_sync_arch_2201_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_l0c2l1_sync_arch_3510 (deprecated)](deprecated_interface/asc_copy_l0c2l1_sync_arch_3510_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_l12bt_sync_arch_2201 (deprecated)](deprecated_interface/asc_copy_l12bt_sync_arch_2201_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_l12bt_sync_arch_3510 (deprecated)](deprecated_interface/asc_copy_l12bt_sync_arch_3510_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_l12fb_sync_arch_2201 (deprecated)](deprecated_interface/asc_copy_l12fb_sync_arch_2201_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_l12fb_sync_arch_3510 (deprecated)](deprecated_interface/asc_copy_l12fb_sync_arch_3510_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_l12gm_sync (deprecated)](deprecated_interface/asc_copy_l12gm_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_l12l0a_mx_sync (deprecated)](deprecated_interface/asc_copy_l12l0a_mx_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_l12l0a_sync_2d_arch_3510 (deprecated)](deprecated_interface/asc_copy_l12l0a_sync_2d_arch_3510_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_l12l0a_sync_3d_arch_3510 (deprecated)](deprecated_interface/asc_copy_l12l0a_sync_3d_arch_3510_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_l12l0a_sync_arch_2201 (deprecated)](deprecated_interface/asc_copy_l12l0a_sync_arch_2201_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_l12l0a_trans_sync (deprecated)](deprecated_interface/asc_copy_l12l0a_trans_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_l12l0b_mx_sync (deprecated)](deprecated_interface/asc_copy_l12l0b_mx_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_l12l0b_sparse_sync (deprecated)](deprecated_interface/asc_copy_l12l0b_sparse_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_l12l0b_sync_2d_arch_3510 (deprecated)](deprecated_interface/asc_copy_l12l0b_sync_2d_arch_3510_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_l12l0b_sync_3d_arch_3510 (deprecated)](deprecated_interface/asc_copy_l12l0b_sync_3d_arch_3510_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_l12l0b_sync_arch_2201 (deprecated)](deprecated_interface/asc_copy_l12l0b_sync_arch_2201_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_l12l0b_trans_sync_arch_2201 (deprecated)](deprecated_interface/asc_copy_l12l0b_trans_sync_arch_2201_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_l12l0b_trans_sync_arch_3510 (deprecated)](deprecated_interface/asc_copy_l12l0b_trans_sync_arch_3510_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_l12l0c_sync (deprecated)](deprecated_interface/asc_copy_l12l0c_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_l12ub_sync (deprecated)](deprecated_interface/asc_copy_l12ub_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_sync (deprecated)](deprecated_interface/asc_copy_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_ub2gm_align_arch_3510 (deprecated)](deprecated_interface/asc_copy_ub2gm_align_arch_3510_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_ub2gm_align_sync_arch_2201 (deprecated)](deprecated_interface/asc_copy_ub2gm_align_sync_arch_2201_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_ub2gm_align_sync_arch_3510 (deprecated)](deprecated_interface/asc_copy_ub2gm_align_sync_arch_3510_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_ub2gm_sync_arch_2201 (deprecated)](deprecated_interface/asc_copy_ub2gm_sync_arch_2201_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_ub2gm_sync_arch_3510 (deprecated)](deprecated_interface/asc_copy_ub2gm_sync_arch_3510_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_ub2l1_sync (deprecated)](deprecated_interface/asc_copy_ub2l1_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_copy_ub2ub_sync (deprecated)](deprecated_interface/asc_copy_ub2ub_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_create_iter_reg (deprecated)](deprecated_interface/asc_create_iter_reg_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_datablock_reduce_max_sync (deprecated)](deprecated_interface/asc_datablock_reduce_max_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_datablock_reduce_min_sync (deprecated)](deprecated_interface/asc_datablock_reduce_min_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_datablock_reduce_sum_sync (deprecated)](deprecated_interface/asc_datablock_reduce_sum_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_deq_int162b8_sync (deprecated)](deprecated_interface/asc_deq_int162b8_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_deq_int322half_sync (deprecated)](deprecated_interface/asc_deq_int322half_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_div_sync (deprecated)](deprecated_interface/asc_div_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_duplicate_sync (deprecated)](deprecated_interface/asc_duplicate_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_e1m2x22bfloat16 (deprecated)](deprecated_interface/asc_e1m2x22bfloat16_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_e2m1x22bfloat16 (deprecated)](deprecated_interface/asc_e2m1x22bfloat16_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_e4m32float (deprecated)](deprecated_interface/asc_e4m32float_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_e5m22float (deprecated)](deprecated_interface/asc_e5m22float_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_eq_scalar_sync (deprecated)](deprecated_interface/asc_eq_scalar_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_eq_sync (deprecated)](deprecated_interface/asc_eq_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_exp_sub (deprecated)](deprecated_interface/asc_exp_sub_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_exp_sync (deprecated)](deprecated_interface/asc_exp_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_fill_l0a_sync (deprecated)](deprecated_interface/asc_fill_l0a_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_fill_l0b_sync (deprecated)](deprecated_interface/asc_fill_l0b_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_fill_l1_sync (deprecated)](deprecated_interface/asc_fill_l1_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_float2bfloat16 (deprecated)](deprecated_interface/asc_float2bfloat16_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_float2bfloat16_sync (deprecated)](deprecated_interface/asc_float2bfloat16_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_float2e4m3 (deprecated)](deprecated_interface/asc_float2e4m3_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_float2e5m2 (deprecated)](deprecated_interface/asc_float2e5m2_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_float2float_sync (deprecated)](deprecated_interface/asc_float2float_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_float2half (deprecated)](deprecated_interface/asc_float2half_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_float2half_sync (deprecated)](deprecated_interface/asc_float2half_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_float2hif8 (deprecated)](deprecated_interface/asc_float2hif8_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_float2int16 (deprecated)](deprecated_interface/asc_float2int16_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_float2int16_sync (deprecated)](deprecated_interface/asc_float2int16_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_float2int32_sync (deprecated)](deprecated_interface/asc_float2int32_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_float2int64 (deprecated)](deprecated_interface/asc_float2int64_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_float2int64_sync (deprecated)](deprecated_interface/asc_float2int64_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_fma_sync (deprecated)](deprecated_interface/asc_fma_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_gather_datablock_sync (deprecated)](deprecated_interface/asc_gather_datablock_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_gather_sync (deprecated)](deprecated_interface/asc_gather_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_ge_scalar_sync (deprecated)](deprecated_interface/asc_ge_scalar_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_ge_sync (deprecated)](deprecated_interface/asc_ge_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_get_ar_spr (deprecated)](deprecated_interface/asc_get_ar_spr_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_get_block_idx (deprecated)](deprecated_interface/asc_get_block_idx_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_get_block_num (deprecated)](deprecated_interface/asc_get_block_num_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_gt_scalar_sync (deprecated)](deprecated_interface/asc_gt_scalar_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_gt_sync (deprecated)](deprecated_interface/asc_gt_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_half2float (deprecated)](deprecated_interface/asc_half2float_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_half2float_sync (deprecated)](deprecated_interface/asc_half2float_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_half2hif8 (deprecated)](deprecated_interface/asc_half2hif8_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_half2int16_sync (deprecated)](deprecated_interface/asc_half2int16_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_half2int32 (deprecated)](deprecated_interface/asc_half2int32_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_half2int32_sync (deprecated)](deprecated_interface/asc_half2int32_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_half2int4_sync (deprecated)](deprecated_interface/asc_half2int4_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_half2int4x2 (deprecated)](deprecated_interface/asc_half2int4x2_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_half2int8 (deprecated)](deprecated_interface/asc_half2int8_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_half2int8_sync (deprecated)](deprecated_interface/asc_half2int8_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_half2uint8 (deprecated)](deprecated_interface/asc_half2uint8_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_half2uint8_sync (deprecated)](deprecated_interface/asc_half2uint8_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_hif82float (deprecated)](deprecated_interface/asc_hif82float_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_hif82half (deprecated)](deprecated_interface/asc_hif82half_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_int162float (deprecated)](deprecated_interface/asc_int162float_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_int162float_sync (deprecated)](deprecated_interface/asc_int162float_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_int162half_sync (deprecated)](deprecated_interface/asc_int162half_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_int162int32 (deprecated)](deprecated_interface/asc_int162int32_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_int162uint32 (deprecated)](deprecated_interface/asc_int162uint32_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_int162uint8 (deprecated)](deprecated_interface/asc_int162uint8_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_int322float_sync (deprecated)](deprecated_interface/asc_int322float_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_int322int16 (deprecated)](deprecated_interface/asc_int322int16_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_int322int16_sync (deprecated)](deprecated_interface/asc_int322int16_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_int322int64 (deprecated)](deprecated_interface/asc_int322int64_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_int322int64_sync (deprecated)](deprecated_interface/asc_int322int64_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_int322uint16 (deprecated)](deprecated_interface/asc_int322uint16_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_int322uint8 (deprecated)](deprecated_interface/asc_int322uint8_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_int42half_sync (deprecated)](deprecated_interface/asc_int42half_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_int4x22bfloat16 (deprecated)](deprecated_interface/asc_int4x22bfloat16_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_int4x22half (deprecated)](deprecated_interface/asc_int4x22half_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_int4x22int16 (deprecated)](deprecated_interface/asc_int4x22int16_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_int642float (deprecated)](deprecated_interface/asc_int642float_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_int642float_sync (deprecated)](deprecated_interface/asc_int642float_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_int642int32 (deprecated)](deprecated_interface/asc_int642int32_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_int642int32_sync (deprecated)](deprecated_interface/asc_int642int32_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_int82half (deprecated)](deprecated_interface/asc_int82half_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_int82half_sync (deprecated)](deprecated_interface/asc_int82half_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_int82int16 (deprecated)](deprecated_interface/asc_int82int16_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_int82int32 (deprecated)](deprecated_interface/asc_int82int32_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_le_scalar_sync (deprecated)](deprecated_interface/asc_le_scalar_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_le_sync (deprecated)](deprecated_interface/asc_le_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_leakyrelu_sync (deprecated)](deprecated_interface/asc_leakyrelu_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_load_image_to_cbuf_sync (deprecated)](deprecated_interface/asc_load_image_to_cbuf_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_loadalign (deprecated)](deprecated_interface/asc_loadalign_v1_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_loadalign (deprecated)](deprecated_interface/asc_loadalign_v2_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_loadalign (deprecated)](deprecated_interface/asc_loadalign_v3_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_loadalign (deprecated)](deprecated_interface/asc_loadalign_v4_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_log_sync (deprecated)](deprecated_interface/asc_log_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_lt_scalar_sync (deprecated)](deprecated_interface/asc_lt_scalar_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_lt_sync (deprecated)](deprecated_interface/asc_lt_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_max_scalar_sync (deprecated)](deprecated_interface/asc_max_scalar_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_max_sync (deprecated)](deprecated_interface/asc_max_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_min_scalar_sync (deprecated)](deprecated_interface/asc_min_scalar_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_min_sync (deprecated)](deprecated_interface/asc_min_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_mmad (deprecated)](deprecated_interface/asc_mmad_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_mmad_mx (deprecated)](deprecated_interface/asc_mmad_mx_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_mmad_sparse (deprecated)](deprecated_interface/asc_mmad_sparse_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_mrgsort4_sync (deprecated)](deprecated_interface/asc_mrgsort4_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_mul_add_relu_sync (deprecated)](deprecated_interface/asc_mul_add_relu_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_mul_add_sync (deprecated)](deprecated_interface/asc_mul_add_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_mul_cast_half2int8_sync (deprecated)](deprecated_interface/asc_mul_cast_half2int8_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_mul_cast_half2uint8_sync (deprecated)](deprecated_interface/asc_mul_cast_half2uint8_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_mul_scalar_sync (deprecated)](deprecated_interface/asc_mul_scalar_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_mul_sync (deprecated)](deprecated_interface/asc_mul_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_muls (deprecated)](deprecated_interface/asc_muls_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_ndim_copy_gm2ub (deprecated)](deprecated_interface/asc_ndim_copy_gm2ub_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_ne_scalar_sync (deprecated)](deprecated_interface/asc_ne_scalar_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_ne_sync (deprecated)](deprecated_interface/asc_ne_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_not_sync (deprecated)](deprecated_interface/asc_not_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_or (deprecated)](deprecated_interface/asc_or_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_or_sync (deprecated)](deprecated_interface/asc_or_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_pack (deprecated)](deprecated_interface/asc_pack_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_pair_reduce_sum_sync (deprecated)](deprecated_interface/asc_pair_reduce_sum_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_rcp_sync (deprecated)](deprecated_interface/asc_rcp_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_reduce_sync (deprecated)](deprecated_interface/asc_reduce_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_relu_sync (deprecated)](deprecated_interface/asc_relu_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_repeat_reduce_max_sync (deprecated)](deprecated_interface/asc_repeat_reduce_max_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_repeat_reduce_min_sync (deprecated)](deprecated_interface/asc_repeat_reduce_min_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_repeat_reduce_sum_sync (deprecated)](deprecated_interface/asc_repeat_reduce_sum_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_rsqrt_sync (deprecated)](deprecated_interface/asc_rsqrt_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_scatter (deprecated)](deprecated_interface/asc_scatter_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_select_sync (deprecated)](deprecated_interface/asc_select_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_set_atomic_add_int (deprecated)](deprecated_interface/asc_set_atomic_add_int_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_set_atomic_max_int (deprecated)](deprecated_interface/asc_set_atomic_max_int_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_set_atomic_min_int (deprecated)](deprecated_interface/asc_set_atomic_min_int_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_set_atomic_none (deprecated)](deprecated_interface/asc_set_atomic_none_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_set_gm2ub_pad (deprecated)](deprecated_interface/asc_set_gm2ub_pad_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_set_store_atomic_config_v2 (deprecated)](deprecated_interface/asc_set_store_atomic_config_v2_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_shiftleft_sync (deprecated)](deprecated_interface/asc_shiftleft_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_shiftright_sync (deprecated)](deprecated_interface/asc_shiftright_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_sqrt_sync (deprecated)](deprecated_interface/asc_sqrt_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_squeeze_v2 (deprecated)](deprecated_interface/asc_squeeze_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_squeeze_with_status (deprecated)](deprecated_interface/asc_squeeze_with_status_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_storealign_pack_v2 (deprecated)](deprecated_interface/asc_storealign_v1_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_storealign_pack_v2 (deprecated)](deprecated_interface/asc_storealign_v2_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_storealign_pack_v2 (deprecated)](deprecated_interface/asc_storealign_v3_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_storealign_pack_postupdate_v2 (deprecated)](deprecated_interface/asc_storealign_v4_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_storeunalign_post (deprecated)](deprecated_interface/asc_storeunalign_post_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_storeunalign_postupdate (deprecated)](deprecated_interface/asc_storeunalign_postupdate_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_sub_relu_sync (deprecated)](deprecated_interface/asc_sub_relu_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_sub_scalar_sync (deprecated)](deprecated_interface/asc_sub_scalar_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_sub_sync (deprecated)](deprecated_interface/asc_sub_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_transpose_sync (deprecated)](deprecated_interface/asc_transpose_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_transto5hd_sync (deprecated)](deprecated_interface/asc_transto5hd_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_uint162uint32 (deprecated)](deprecated_interface/asc_uint162uint32_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_uint162uint8 (deprecated)](deprecated_interface/asc_uint162uint8_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_uint322int16 (deprecated)](deprecated_interface/asc_uint322int16_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_uint322uint16 (deprecated)](deprecated_interface/asc_uint322uint16_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_uint322uint8 (deprecated)](deprecated_interface/asc_uint322uint8_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_uint82half (deprecated)](deprecated_interface/asc_uint82half_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_uint82half_sync (deprecated)](deprecated_interface/asc_uint82half_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_uint82uint16 (deprecated)](deprecated_interface/asc_uint82uint16_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_uint82uint32 (deprecated)](deprecated_interface/asc_uint82uint32_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
| [asc_vdeq_int162b8_sync (deprecated)](deprecated_interface/asc_vdeq_int162b8_sync_deprecated.md) | This API is deprecated. For replacement relationships and migration methods, see the API documentation. |
