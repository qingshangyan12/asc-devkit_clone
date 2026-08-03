/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning                                                                                                               \
    "impl/tensor_api/algorithm/copy_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
* \file copy_impl.h
* \brief
*/
#ifndef IMPL_TENSOR_API_ALGORITHM_COPY_IMPL_H
#define IMPL_TENSOR_API_ALGORITHM_COPY_IMPL_H

#include "impl/tensor_api/atom/copy_atom_impl.h"
#include "impl/tensor_api/algorithm/transform_impl.h"

namespace AscendC {
namespace Te {

template <typename QuantParam>
constexpr bool IsQuantScalarV =
    !IsAttrTensorV<QuantParam> && Std::is_convertible_v<Std::remove_cvref_t<QuantParam>, uint64_t>;

template <typename QuantParam>
constexpr bool IsCopyQuantParamV = IsQuantScalarV<QuantParam> || IsAttrTensorV<QuantParam>;

template <typename Coord>
constexpr bool IsCopyCoordParamV = Std::is_tuple_v<Std::remove_cvref_t<Coord>> ||
    Std::is_same_v<Std::remove_cvref_t<Coord>, ZeroCoordType>;

template <typename Coord, typename Shape>
constexpr bool IsCopyCoordShapeCompatibleV =
    Std::is_same_v<Std::remove_cvref_t<Coord>, ZeroCoordType> || IsSameStructureV<Coord, Shape>;

template <typename CoordDst, typename CoordSrc, typename Shape>
constexpr bool IsCopyRegionParamV = IsCopyCoordParamV<CoordDst> && IsCopyCoordParamV<CoordSrc> &&
    Std::is_tuple_v<Std::remove_cvref_t<Shape>>;

template <typename Pattern>
constexpr bool IsConvLayoutPatternV = Std::is_same_v<Pattern, NCHWLayoutPtn> ||
    Std::is_same_v<Pattern, NHWCLayoutPtn> || Std::is_same_v<Pattern, NC1HWC0LayoutPtn> ||
    Std::is_same_v<Pattern, NCDHWLayoutPtn> || Std::is_same_v<Pattern, NDC1HWC0LayoutPtn>;

template <typename SrcTensor, typename Shape>
struct IsCopyShapeCompatibleWithSrcLayout {
private:
    using LayoutType = typename Std::remove_cvref_t<SrcTensor>::layoutType;
    using SrcShape = Std::remove_cvref_t<decltype(Std::declval<LayoutType>().Shape())>;
    using ShapeType = Std::remove_cvref_t<Shape>;
    using Pattern = GetLayoutPattern<LayoutType>;

    static constexpr size_t srcDepth = NestingDepthV<SrcShape>;
    static constexpr size_t shapeDepth = NestingDepthV<ShapeType>;
    static constexpr size_t srcRank = Std::tuple_size_v<SrcShape>;
    static constexpr size_t shapeRank = Std::tuple_size_v<ShapeType>;
    static constexpr size_t batchNum = srcRank - 1;

    static constexpr bool isSameStructure = IsSameStructureV<SrcShape, ShapeType>;
    static constexpr bool isLogicalMatrix = !IsConvLayoutPatternV<Pattern> && srcRank == TWO_DIM_DATA &&
        srcDepth == FOUR_DIM_DATA && shapeDepth == TWO_DIM_DATA;
    static constexpr bool isLogicalFlatBatch = !IsConvLayoutPatternV<Pattern> && srcRank == shapeRank &&
        srcRank >= TWO_DIM_DATA && srcDepth == batchNum + FOUR_DIM_DATA &&
        shapeDepth == batchNum + TWO_DIM_DATA;

public:
    static constexpr bool value = isSameStructure || isLogicalMatrix || isLogicalFlatBatch;
};

template <typename SrcTensor, typename Shape>
constexpr bool IsCopyShapeCompatibleWithSrcLayoutV = IsCopyShapeCompatibleWithSrcLayout<SrcTensor, Shape>::value;

template <typename SrcTensor, typename CoordSrc, typename Shape>
__aicore__ inline constexpr void CheckCopyRegionParams()
{
    static_assert(IsCopyCoordShapeCompatibleV<CoordSrc, Shape>,
        "Explicit coordSrc and shape must have the same tuple structure.");
    static_assert(IsCopyShapeCompatibleWithSrcLayoutV<SrcTensor, Shape>,
        "Shape must match the source Layout shape, or use a supported logical matrix/batch shape.");
}

template <typename AtomType, typename DstTensor, typename SrcTensor>
__aicore__ inline void Copy(const CopyAtom<AtomType>& atomCopy, const DstTensor& dst, const SrcTensor& src)
{
    atomCopy.Call(dst, src);
}

template <typename AtomType, typename DstTensor, typename SrcTensor, typename QuantParam,
    Std::enable_if_t<IsCopyQuantParamV<QuantParam>, int> = 0>
__aicore__ inline void Copy(const CopyAtom<AtomType>& atomCopy, const DstTensor& dst, const SrcTensor& src,
    const QuantParam& quant)
{
    atomCopy.Call(dst, src, quant);
}

template <typename AtomType, typename DstTensor, typename SrcTensor, typename CoordDst, typename CoordSrc,
    typename Shape, Std::enable_if_t<IsCopyRegionParamV<CoordDst, CoordSrc, Shape>, int> = 0>
__aicore__ inline void Copy(const CopyAtom<AtomType>& atomCopy, const DstTensor& dst, const SrcTensor& src,
    const CoordDst& coordDst, const CoordSrc& coordSrc, const Shape& shape)
{
    CheckCopyRegionParams<SrcTensor, CoordSrc, Shape>();
    atomCopy.Call(dst, src, coordDst, coordSrc, shape);
}

template <typename AtomType, typename DstTensor, typename SrcTensor, typename QuantParam, typename CoordDst,
    typename CoordSrc, typename Shape,
    Std::enable_if_t<IsCopyQuantParamV<QuantParam> && IsCopyRegionParamV<CoordDst, CoordSrc, Shape>, int> = 0>
__aicore__ inline void Copy(const CopyAtom<AtomType>& atomCopy, const DstTensor& dst, const SrcTensor& src,
    const QuantParam& quant, const CoordDst& coordDst, const CoordSrc& coordSrc, const Shape& shape)
{
    CheckCopyRegionParams<SrcTensor, CoordSrc, Shape>();
    atomCopy.Call(dst, src, quant, coordDst, coordSrc, shape);
}

template <typename CopyOperationType>
__aicore__ inline constexpr auto MakeCopy(const CopyOperationType& copyOperation)
{
    return CopyAtom<CopyTraits<CopyOperationType>>{};
}

template <typename CopyOperationType, typename CopyTraitType>
__aicore__ inline constexpr auto MakeCopy(const CopyOperationType& copyOperation, const CopyTraitType& copyTrait)
{
    return CopyAtom<CopyTraits<CopyOperationType, CopyTraitType>>{};
}

}
}

#endif // IMPL_TENSOR_API_ALGORITHM_COPY_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
