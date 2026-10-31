# Tensor API List

<!-- md-trans-meta sourceCommit=0c697a9252886e83ab05f536c267cd58e7c9d89b translatedAt=2026-10-27T11:11:21.839Z pushedAt=2026-10-27T13:04:46.180Z nl=2 -->

## Basic Data Structures

**Table 1** Basic data structure API list

| Type or API Name | Function Description |
| --- | --- |
| [layout](layout/layout.md) | Uses Shape and Stride to describe the logical shape and memory layout of data. |
| [Shape](layout/shape.md) | Describes the logical shape of each dimension of a Tensor, supporting plain integers and nested tuples. |
| [Stride](layout/stride.md) | Describes the address stride between adjacent elements of each dimension of a Tensor in memory, supporting plain integers and nested tuples. |
| [Coord](layout/coord.md) | Describes the logical coordinates of a Tensor in each dimension. |
| [global_tensor and local_tensor](tensor/tensor.md) | Describe Tensor objects in Global Memory and on-chip Local Memory, respectively. |
| [Pointer](tensor/pointer.md) | Describes the access address of a Tensor and its corresponding storage location. |
| [make_layout](layout/make_layout.md) | Constructs a plain Layout based on Shape and Stride. |
| [make_pattern_layout](layout/make_pattern_layout.md) | Constructs a Layout with layout type information based on Layout Pattern, Shape, and Stride. |
| [make_frame_layout](layout/make_frame_layout.md) | Constructs a Layout based on the Layout Pattern and shape parameters of a convolution feature map or cube fractal. |
| [make_shape](layout/make_shape.md) | Constructs a Shape object. |
| [make_stride](layout/make_stride.md) | Constructs a Stride object. |
| [make_coord](layout/make_coord.md) | Constructs a Coord object. |
| [make_mem_ptr](tensor/make_mem_ptr.md) | Constructs a Tensor API memory pointer object based on a pointer and storage location. |
| [make_tensor](tensor/make_tensor.md) | Binds a memory pointer and a Layout to construct a global_tensor or local_tensor. |
| [slice](tensor/slice.md) | Creates a sub-Tensor of the original Tensor based on the start coordinates and a Shape or Layout description. |

## Layout Operations

**Table 2** Layout operation API list

| API Name | Function Description |
| --- | --- |
| [capacity](layout/capacity.md) | Gets the actual memory capacity corresponding to the Layout or a specified sub-dimension. |
| [coshape](layout/coshape.md) | Gets the Shape structure of a specified sub-dimension of the Layout at compile time. |
| [cosize](layout/cosize.md) | Gets the capacity or number of elements of a specified sub-dimension of the Layout at compile time. |
| [crd2idx](layout/crd2idx.md) | Converts multi-dimensional coordinates into a linear memory index according to the Layout. |
| [get](layout/get.md) | Gets a specified element in a tuple, supporting single or multiple compile-time indexes. |
| [get_shape](layout/get_shape.md) | Gets the Shape of the Layout or of a specified sub-dimension. |
| [get_stride](layout/get_stride.md) | Gets the Stride of the Layout or of a specified sub-dimension. |
| [rank](layout/rank.md) | Gets the hierarchy or dimension information of the Layout. |
| [select](layout/select.md) | Selects a substructure from the Shape and Stride of the Layout by a specified index. |
| [size](layout/size.md) | Gets the number of logical elements of the Layout. |
| [squeeze](layout/squeeze.md) | Removes the specified dimension of size 1 from the top-level structure of the Shape. |

## Algorithm

**Table 3** Algorithm API and type list

| Type or API Name | Function Description |
| --- | --- |
| [Operation, Trait, and Atom](algorithm.md) | Describes how Operation, Trait, Params, and Atom are combined in movement and cube computation, as well as the unified call entries of `copy` and `mmad`. |

## Movement

**Table 4** Movement path and API list

| Movement Path or API Name | Function Description |
| --- | --- |
| [Global Memory to Unified Buffer](reg_vector_compute/vector_compute_load/copy_gm_to_ub.md) | Uses `copy` to move data from Global Memory to the Unified Buffer, supporting coordinate region movement. |
| [Unified Buffer to Global Memory](reg_vector_compute/vector_compute_store/copy_ub_to_gm.md) | Uses `copy` to move data from the Unified Buffer to Global Memory, supporting coordinate region movement. |
| [Global Memory to L1 Buffer](matrix_compute/cube_compute_load/copy_gm_to_l1.md) | Uses `copy` to move data from Global Memory to the L1 Buffer, supporting multiple cube format conversions and batch movement. |
| [Unified Buffer to L1 Buffer](reg_vector_compute/vector_compute_load/copy_ub_to_l1.md) | Uses `copy` to move data from the Unified Buffer to the L1 Buffer. |
| [L1 Buffer to Unified Buffer](matrix_compute/cube_compute_load/copy_l1_to_ub.md) | Uses `copy` to move data from the L1 Buffer to the Unified Buffer. |
| [L1 Buffer to L0A Buffer](matrix_compute/cube_compute_load/copy_l1_to_l0a.md) | Uses `copy` to move the left cube data from the L1 Buffer to the L0A Buffer, supporting cube format conversion and convolution feature map movement. |
| [L1 Buffer to L0B Buffer](matrix_compute/cube_compute_load/copy_l1_to_l0b.md) | Uses `copy` to move the right cube data from the L1 Buffer to the L0B Buffer, supporting cube format conversion and batch movement. |
| [L1 Buffer to L0ScaleA Buffer](matrix_compute/cube_compute_load/copy_l1_to_l0scalea.md) | Uses `copy` to move the ScaleA data from the L1 Buffer to the L0ScaleA Buffer. |
| [L1 Buffer to L0ScaleB Buffer](matrix_compute/cube_compute_load/copy_l1_to_l0scaleb.md) | Uses `copy` to move the ScaleB data from the L1 Buffer to the L0ScaleB Buffer. |
| [L1 Buffer to BiasTable Buffer](matrix_compute/cube_compute_load/copy_l1_to_biastable.md) | Uses `copy` to move the Bias data from the L1 Buffer to the BiasTable Buffer. |
| [L1 Buffer to Fixpipe Buffer](matrix_compute/cube_compute_load/copy_l1_to_fixbuf.md) | Uses `copy` to move the quantization parameters from the L1 Buffer to the Fixpipe Buffer. |
| [L0C Buffer to Global Memory](matrix_compute/cube_compute_store/copy_l0c_to_gm.md) | Uses `copy` to move the cube result from the L0C Buffer to Global Memory, supporting format conversion and quantized output. |
| [L0C Buffer to Unified Buffer](matrix_compute/cube_compute_store/copy_l0c_to_ub.md) | Uses `copy` to move the cube result from the L0C Buffer to the Unified Buffer, supporting format conversion and quantized output. |
| [L0C Buffer to L1 Buffer](matrix_compute/cube_compute_store/copy_l0c_to_l1.md) | Uses `copy` to move the cube result from the L0C Buffer to the L1 Buffer. |
| [Unified Buffer Internal Movement](reg_vector_compute/vector_compute_load/copy_ub_to_ub.md) | Uses `copy` to move data between Tensors within the Unified Buffer. |

## Cube Computation

**Table 5** Cube computation API and feature list

| API or Topic | Function Description |
| --- | --- |
| [Cube Computation Overview](matrix_compute/overview.md) | Describes the basic process and API composition of Tensor API cube computation. |
| [Cube Computation Fractal Introduction](matrix_compute/cube_compute_fractal_intro.md) | Describes the commonly used data fractal formats in cube computation and their Layout expressions. |
| [mmad](matrix_compute/mmad_compute/mmad.md) | Uses L0A, L0B, and optional Bias data to perform cube multiply-accumulate computation and writes the result to L0C. |
| [Key Features of mmad Computation](matrix_compute/mmad_compute_key_features/mmad_compute_key_features.md) | Describes key cube multiply-accumulate features such as GEMV, HF32, and unit_flag. |
| [GEMV](matrix_compute/mmad_compute_key_features/gemv.md) | Describes the GEMV computation mode in the scenario where M is 1. |
| [HF32](matrix_compute/mmad_compute_key_features/hf32.md) | Describes the HF32 precision mode in cube computation. |
| [unit_flag](matrix_compute/mmad_compute_key_features/unit_flag.md) | Describes the fine-grained parallel control between mmad and the subsequent cube data move-out. |
| [Auxiliary Configuration for mmad Computation](matrix_compute/mmad_compute_aux_config.md) | Lists the C API auxiliary configuration APIs required for mmad computation. |
| [Overall Description of Cube Computation Move-In](matrix_compute/cube_compute_load/overall_description.md) | Describes the paths for moving cube data from Global Memory or the L1 Buffer into the cube computation buffers at each level. |
| [Overall Description of Cube Computation Move-Out](matrix_compute/cube_compute_store/overall_description.md) | Describes the paths for moving the cube result from the L0C Buffer to Global Memory, the Unified Buffer, or the L1 Buffer. |
| [Key Features of Cube Move-Out](matrix_compute/cube_store_key_features/cube_store_key_features.md) | Describes real-time quantization, real-time ReLU, channel split, channel merge, batch movement, and dual-destination mode. |
| [Real-Time Quantization](matrix_compute/cube_store_key_features/quant_pre.md) | Describes the real-time quantization capability during cube result move-out. |
| [Real-Time ReLU](matrix_compute/cube_store_key_features/relu_pre.md) | Describes the real-time ReLU capability during cube result move-out. |
| [F32 Channel Split](matrix_compute/cube_store_key_features/f32_channel_split.md) | Describes the channel split capability during F32 result move-out. |
| [Int8 Channel Merge](matrix_compute/cube_store_key_features/int8_channel_merge.md) | Describes the channel merge capability during Int8 result move-out. |
| [Batch Copy](matrix_compute/cube_store_key_features/batch_copy.md) | Describes the multi-batch move-out capability of the cube result. |
| [L0C to Unified Buffer Dual-Destination Mode](matrix_compute/cube_store_key_features/l0c_to_ub_dual_dst.md) | Introduces the dual-destination movement-out mode from L0C to the Unified Buffer. |

## Utility APIs

**Table 6**  Utility API list

| Type or API Name | Function Description |
| --- | --- |
| [Compile-time Integer Aliases](utils/compile_time_integer_aliases.md) | Provides type aliases for expressing compile-time integer values. |
| [Dimension Keep Mark](utils/dimension_keep_mark.md) | Provides types and constants for marking dimension-keeping behavior. |
| [get_layout_pattern](utils/get_layout_pattern.md) | Extracts Layout Pattern type information from a Layout type. |
| [Type Traits](utils/type_traits.md) | Provides type determination utilities for Layout, Tensor, Coord, Shape, quantization parameters, and others. |

## Deprecated APIs

**Table 7**  Deprecated API list

| API Name | Function Description |
| --- | --- |
| [Deprecated API description](deprecated_interface/deprecated_interface.md) | Summarizes the deprecated APIs in the Tensor API used for compatibility with legacy PascalCase naming. |
| [layout::Capacity](deprecated_interface/layout_capacity_deprecated.md) | Legacy Layout capacity query API. |
| [layout::Shape](deprecated_interface/layout_shape_deprecated.md) | Legacy Layout Shape query API. |
| [layout::Stride](deprecated_interface/layout_stride_deprecated.md) | Legacy Layout Stride query API. |
| [layout::Rank](deprecated_interface/layout_rank_deprecated.md) | Legacy Layout rank query API. |
| [layout::Size](deprecated_interface/layout_size_deprecated.md) | Legacy Layout logical element count query API. |
| [layout::Get](deprecated_interface/layout_get_deprecated.md) | Legacy Layout element get API. |
| [base_tensor::Tensor](deprecated_interface/tensor_tensor_deprecated.md) | Legacy Tensor object access API. |
| [base_tensor::Engine](deprecated_interface/tensor_engine_deprecated.md) | Legacy Tensor Engine access API. |
| [base_tensor::Layout](deprecated_interface/tensor_layout_deprecated.md) | Legacy Tensor Layout access API. |
| [base_tensor::Data](deprecated_interface/tensor_data_deprecated.md) | Legacy Tensor data address access API. |
| [base_tensor::Shape](deprecated_interface/tensor_shape_deprecated.md) | Legacy Tensor Shape access API. |
| [base_tensor::Stride](deprecated_interface/tensor_stride_deprecated.md) | Legacy Tensor Stride access API. |
| [base_tensor::Size](deprecated_interface/tensor_size_deprecated.md) | Legacy Tensor logical element count query API. |
| [base_tensor::Capacity](deprecated_interface/tensor_capacity_deprecated.md) | Legacy Tensor capacity query API. |
| [base_tensor::Slice](deprecated_interface/tensor_slice_deprecated.md) | Legacy Tensor slice API. |
| [global_tensor::SetL2CacheHint](deprecated_interface/set_l2_cache_hint_deprecated.md) | Legacy Global Tensor L2 Cache configuration API. |
