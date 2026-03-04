/**
 	 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 	 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 	 * CANN Open Software License Agreement Version 2.0 (the "License").
 	 * Please refer to the License for details. You may not use this file except in compliance with the License.
 	 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 	 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 	 * See LICENSE in the root of the software repository for the full text of the License.
 	 */
 	 
 	 /*!
 	  * \file instruction.h
 	  * \brief
 	  */
 	 #ifndef IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_LOAD_DATA_NPU_ARCH_3510_INSTRUCTION_H
 	 #define IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_LOAD_DATA_NPU_ARCH_3510_INSTRUCTION_H
 	 
 	 #include "impl/experimental/tensor_api/tensor/pointer_impl.h"
 	 #include "impl/experimental/tensor_api/tensor/local_tensor_impl.h"
 	 #include "impl/experimental/tensor_api/arch/arch_utils.h"
 	 
 	 namespace AscendC {
 	 namespace Te {
 	 
 	 class LoadDataFourDim3510L12L0ABase {
 	 
 	 public:
 	     template<typename T>
 	     __aicore__ inline constexpr void CheckNZTemplate()
 	     {
 	         using type = typename T::elementType;
 	         using ShapeRow0 = typename GetFourDimType<T, AttrInfo::SHAPE, AttrInfo::ROW, 0>::type;
 	         using ShapeColumn0 = typename GetFourDimType<T, AttrInfo::SHAPE, AttrInfo::COLUMN, 0>::type;
 	         static_assert(Std::is_same_v<ShapeRow0, Std::Int<FRACTAL_FIXED>>,
 	             "LoadDataFourDim3510L12L0A Layout->Shape->Row->ZeroDim is not Std::Int<16> type!");
 	         static_assert(Std::is_same_v<ShapeColumn0, Std::Int<C0_ELEMS<type>>>,
 	             "LoadDataFourDim3510L12L0A Layout->Shape->Column->ZeroDim is not Std::Int<C0Size/Type> type!"); 
 	 
 	         using StrideRow0 = typename GetFourDimType<T, AttrInfo::STRIDE, AttrInfo::ROW, 0>::type;
 	         using StrideRow1 = typename GetFourDimType<T, AttrInfo::STRIDE, AttrInfo::ROW, 1>::type;
 	         using StrideColumn0 = typename GetFourDimType<T, AttrInfo::STRIDE, AttrInfo::COLUMN, 0>::type;
 	         static_assert(Std::is_same_v<StrideRow0, Std::Int<C0_ELEMS<type>>>,
 	             "LoadDataFourDim3510L12L0A Layout->Stride->Row-ZeroDim is not Std::Int<C0Size/Type> type!");
 	         static_assert(Std::is_same_v<StrideRow1, Std::Int<FRACTAL_FIXED * C0_ELEMS<type>>>,
 	             "LoadDataFourDim3510L12L0A Layout->Stride->Row-ZeroDim is not Std::Int<16*C0Size/Type> type!");
 	         static_assert(Std::is_same_v<StrideColumn0, Std::Int<1>>,
 	             "LoadDataFourDim3510L12L0A Layout->Stride->Column->ZeroDim is not Std::Int<1> type!");
 	     }
 	 
 	     template<typename T>
 	     __aicore__ inline constexpr void CheckZNTemplate()
 	     {
 	         using type = typename T::elementType;
 	         using ShapeRow0 = typename GetFourDimType<T, AttrInfo::SHAPE, AttrInfo::ROW, 0>::type;
 	         using ShapeColumn0 = typename GetFourDimType<T, AttrInfo::SHAPE, AttrInfo::COLUMN, 0>::type;
 	         static_assert(Std::is_same_v<ShapeRow0, Std::Int<C0_ELEMS<type>>>,
 	             "LoadDataFourDim3510L12L0A Layout->Shape->Row->ZeroDim is not Std::Int<C0Size/Type> type!");
 	         static_assert(Std::is_same_v<ShapeColumn0, Std::Int<FRACTAL_FIXED>>,
 	             "LoadDataFourDim3510L12L0A Layout->Shape->Column->ZeroDim is not Std::Int<16> type!"); 
 	 
 	         using StrideRow0 = typename GetFourDimType<T, AttrInfo::STRIDE, AttrInfo::ROW, 0>::type;
 	         using StrideColumn0 = typename GetFourDimType<T, AttrInfo::STRIDE, AttrInfo::COLUMN, 0>::type;
 	         using StrideColumn1 = typename GetFourDimType<T, AttrInfo::STRIDE, AttrInfo::COLUMN, 1>::type;
 	         static_assert(Std::is_same_v<StrideRow0, Std::Int<1>>,
 	             "LoadDataFourDim3510L12L0A Layout->Stride->Row-ZeroDim is not Std::Int<1> type!");
 	         static_assert(Std::is_same_v<StrideColumn0, Std::Int<C0_ELEMS<type>>>,
 	             "LoadDataFourDim3510L12L0A Layout->Stride->Column->ZeroDim is not Std::Int<C0Size/Type> type!");
 	         static_assert(Std::is_same_v<StrideColumn1, Std::Int<FRACTAL_FIXED * C0_ELEMS<type>>>,
 	             "LoadDataFourDim3510L12L0A Layout->Stride->Column->OneDim is not Std::Int<16*C0Size/Type> type!");
 	     }
 	 
 	     template <const LoadDataTrait& trait, typename T, typename U>
 	     __aicore__ inline constexpr void CheckTemplate()
 	     {
 	         using srcType = typename U::elementType;
 	         using dstType = typename T::elementType;
 	         CheckNZTemplate<T>();
 	         if constexpr (!trait.transposed) {
 	             CheckNZTemplate<U>();
 	         } else {
 	             CheckZNTemplate<U>();
 	         }
 	 #if defined(__NPU_ARCH__ ) && __NPU_ARCH__ == 3510
 	         static_assert(Std::is_one_of_v<Std::tuple<dstType, srcType>, Std::tuple<__ca__ half, __cbuf__ half>, 
 	             Std::tuple<__ca__ int16_t, __cbuf__ int16_t>, Std::tuple<__ca__ uint16_t, __cbuf__ uint16_t>,
 	             Std::tuple<__ca__ bfloat16_t, __cbuf__ bfloat16_t>, Std::tuple<__ca__ uint32_t, __cbuf__ uint32_t>, 
 	             Std::tuple<__ca__ int32_t, __cbuf__ int32_t>, Std::tuple<__ca__ float, __cbuf__ float>, 
 	             Std::tuple<__ca__ uint8_t, __cbuf__ uint8_t>, Std::tuple<__ca__ int8_t, __cbuf__ int8_t>, 
 	             Std::tuple<__ca__ fp8_e4m3fn_t, __cbuf__ fp8_e4m3fn_t>, Std::tuple<__ca__ fp8_e5m2_t, __cbuf__ fp8_e5m2_t>, 
 	             Std::tuple<__ca__ hifloat8_t, __cbuf__ hifloat8_t>>, "The data type is not supported.");
 	 #endif
 	     }
 	 
 	     template <const LoadDataTrait& trait, typename T, typename U, typename V, size_t... Is>
 	     __aicore__ inline void LoadDataAlignV2Impl(const T& dst, const U& src, const V& tupleParams, Std::index_sequence<Is...>)
 	     {
 	         // MTE2
 	         LoadL1ToL0AAlignV2<trait.transposed>(dst.Data().Get(), src.Data().Get(),
 	             Std::get<Is>(tupleParams)...);
 	     }
 	 
 	     template <bool transpose, typename T>
 	     __aicore__ inline void LoadL1ToL0AAlignV2(__ca__ T* dst, __cbuf__ T* src, uint16_t mStartPosition,
 	         uint16_t kStartPosition, uint8_t mStep, uint8_t kStep, int16_t srcStride, uint16_t dstStride)
 	     {
 	         if ASCEND_IS_AIV {
 	             return;
 	         }
 	 
 	         if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
                 if (is_tensor_type_v<T, fp4x2_e2m1_t>) {
 	                load_cbuf_to_ca_s4(dst, src, mStartPosition, kStartPosition, mStep, kStep, srcStride, dstStride, transpose);
                 } else {
 	             	load_cbuf_to_ca(dst, src, mStartPosition, kStartPosition, mStep, kStep, srcStride, dstStride, transpose);
				 }
 	         }
 	     }
 	 };
 	 
 	 class LoadDataFourDim3510L12L0BBase {
 	 
 	 public:
 	     template<typename T>
 	     __aicore__ inline constexpr void CheckNZTemplate()
 	     {
 	         using type = typename T::elementType;
 	         using ShapeRow0 = typename GetFourDimType<T, AttrInfo::SHAPE, AttrInfo::ROW, 0>::type;
 	         using ShapeColumn0 = typename GetFourDimType<T, AttrInfo::SHAPE, AttrInfo::COLUMN, 0>::type;
 	         static_assert(Std::is_same_v<ShapeRow0, Std::Int<FRACTAL_FIXED>>,
 	             "LoadDataFourDim3510L12L0B Layout->Shape->Row->ZeroDim is not Std::Int<16> type!");
 	         static_assert(Std::is_same_v<ShapeColumn0, Std::Int<C0_ELEMS<type>>>,
 	             "LoadDataFourDim3510L12L0B Layout->Shape->Column->ZeroDim is not Std::Int<C0Size/Type> type!");
 	 
 	         using StrideRow0 = typename GetFourDimType<T, AttrInfo::STRIDE, AttrInfo::ROW, 0>::type;
 	         using StrideColumn0 = typename GetFourDimType<T, AttrInfo::STRIDE, AttrInfo::COLUMN, 0>::type;
 	         static_assert(Std::is_same_v<StrideRow0, Std::Int<C0_ELEMS<type>>>,
 	             "LoadDataFourDim3510L12L0B Layout->Stride->Row-ZeroDim is not Std::Int<C0Size/Type> type!");
 	         static_assert(Std::is_same_v<StrideColumn0, Std::Int<1>>,
 	             "LoadDataFourDim3510L12L0B Layout->Stride->Column->ZeroDim is not Std::Int<1> type!");
 	     }
 	     
 	     template<typename T>
 	     __aicore__ inline constexpr void CheckZNTemplate()
 	     {
 	         using type = typename T::elementType;
 	         using ShapeRow0 = typename GetFourDimType<T, AttrInfo::SHAPE, AttrInfo::ROW, 0>::type;
 	         using ShapeColumn0 = typename GetFourDimType<T, AttrInfo::SHAPE, AttrInfo::COLUMN, 0>::type;
 	         static_assert(Std::is_same_v<ShapeColumn0, Std::Int<FRACTAL_FIXED>>,
 	             "LoadDataFourDim3510L12L0B Layout->Shape->Column->ZeroDim is not Std::Int<16> type!");
 	         static_assert(Std::is_same_v<ShapeRow0, Std::Int<C0_ELEMS<type>>>,
 	             "LoadDataFourDim3510L12L0B Layout->Shape->Row->ZeroDim is not Std::Int<C0Size/Type> type!"); 
 	 
 	         using StrideRow0 = typename GetFourDimType<T, AttrInfo::STRIDE, AttrInfo::ROW, 0>::type;
 	         using StrideColumn0 = typename GetFourDimType<T, AttrInfo::STRIDE, AttrInfo::COLUMN, 0>::type;
 	         static_assert(Std::is_same_v<StrideColumn0, Std::Int<C0_ELEMS<type>>>,
 	             "LoadDataFourDim3510L12L0B Layout->Stride->Column-ZeroDim is not Std::Int<C0Size/Type> type!");
 	         static_assert(Std::is_same_v<StrideRow0, Std::Int<1>>,
 	             "LoadDataFourDim3510L12L0B Layout->Stride->Row->ZeroDim is not Std::Int<1> type!");
 	     }
 	 
 	     template <const LoadDataTrait& trait, typename T, typename U>
 	     __aicore__ inline constexpr void CheckTemplate()
 	     {
 	         using srcType = typename U::elementType;
 	         using dstType = typename T::elementType;
 	         CheckZNTemplate<T>();
 	         if constexpr (!trait.transposed) {
 	             CheckZNTemplate<U>();
 	         } else {
 	             CheckNZTemplate<U>();
 	         }
 	 #if defined(__NPU_ARCH__ ) && __NPU_ARCH__ == 3510
 	         static_assert(Std::is_one_of_v<Std::tuple<dstType, srcType>, Std::tuple<__cb__ half, __cbuf__ half>, 
 	             Std::tuple<__cb__ int16_t, __cbuf__ int16_t>, Std::tuple<__cb__ uint16_t, __cbuf__ uint16_t>,
 	             Std::tuple<__cb__ bfloat16_t, __cbuf__ bfloat16_t>, Std::tuple<__cb__ uint32_t, __cbuf__ uint32_t>, 
 	             Std::tuple<__cb__ int32_t, __cbuf__ int32_t>, Std::tuple<__cb__ float, __cbuf__ float>, 
 	             Std::tuple<__cb__ uint8_t, __cbuf__ uint8_t>, Std::tuple<__cb__ int8_t, __cbuf__ int8_t>, 
 	             Std::tuple<__cb__ fp8_e4m3fn_t, __cbuf__ fp8_e4m3fn_t>, Std::tuple<__cb__ fp8_e5m2_t, __cbuf__ fp8_e5m2_t>, 
 	             Std::tuple<__cb__ hifloat8_t, __cbuf__ hifloat8_t>>, "The data type is not supported.");
 	 #endif
 	     }
 	 
 	     template <const LoadDataTrait& trait, typename T, typename U, typename V, size_t... Is>
 	     __aicore__ inline void LoadDataAlignV2Impl(const T& dst, const U& src, const V& tupleParams, Std::index_sequence<Is...>)
 	     {
 	         // MTE2
 	         LoadL1ToL0BAlignV2<trait.transposed>(dst.Data().Get(), src.Data().Get(),
 	             Std::get<Is>(tupleParams)...);
 	     }
 	 
 	     template <bool transpose, typename T>
 	     __aicore__ inline void LoadL1ToL0BAlignV2(__cb__ T* dst, __cbuf__ T* src, uint16_t mStartPosition,
 	         uint16_t kStartPosition, uint8_t mStep, uint8_t kStep, int16_t srcStride, uint16_t dstStride)
 	     {
 	         if ASCEND_IS_AIV {
 	             return;
 	         }
 	         if constexpr (CURRENT_ARCH_VERSION == ArchVersion::V3510) {
                 if (is_tensor_type_v<T, fp4x2_e2m1_t>) {
 	                load_cbuf_to_cb_s4(dst, src, mStartPosition, kStartPosition, mStep, kStep, srcStride, dstStride, transpose);
                 } else {
 	                load_cbuf_to_cb(dst, src, mStartPosition, kStartPosition, mStep, kStep, srcStride, dstStride, transpose);
                 }
 	         }
 	     }
 	 };
 	 } // namespace Te
 	 } // namespace AscendC
 	 
 	 #endif // IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_LOAD_DATA_NPU_ARCH_3510_INSTRUCTION_H