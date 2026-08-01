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
    "impl/tensor_api/tensor/tensor_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
* \file tensor_impl.h
* \brief
*/
#ifndef IMPL_TENSOR_API_TENSOR_TENSOR_IMPL_H
#define IMPL_TENSOR_API_TENSOR_TENSOR_IMPL_H

#include "impl/tensor_api/utils/utils_impl.h"
#include "impl/tensor_api/tensor/layout_impl.h"
#include "impl/tensor_api/tensor/engine_impl.h"
#include "impl/tensor_api/tensor/pointer_pattern.h"

namespace AscendC {

template <typename T>
class GlobalTensor;

template <typename T>
class LocalTensor;

template <typename EngineType, typename LayoutType>
struct tensor_attribute {};

template <typename EngineType, typename LayoutType>
using TensorAttribute = tensor_attribute<EngineType, LayoutType>;

} // namespace AscendC

namespace asc {
namespace te {

using AscendC::TensorAttribute;
using AscendC::tensor_attribute;

template <typename T>
using global_tensor = AscendC::GlobalTensor<T>;

template <typename T>
using local_tensor = AscendC::LocalTensor<T>;

template <typename LocationType, typename EngineType, typename LayoutType>
struct make_tensor_result {
    using type = typename Std::conditional<Std::is_same_v<LocationType, Location::GM>,
        AscendC::GlobalTensor<TensorAttribute<EngineType, LayoutType>>,
        AscendC::LocalTensor<TensorAttribute<EngineType, LayoutType>>>::type;
};

template <typename LocationType, typename EngineType, typename LayoutType>
using MakeTensorResult = make_tensor_result<LocationType, EngineType, LayoutType>;

template <typename EngineType, typename LayoutType>
struct base_tensor {
    using iterator = typename EngineType::iterator;
    using valueType = typename EngineType::valueType;
    using elementType = typename EngineType::elementType;
    using reference = typename EngineType::reference;

    using engineType  = EngineType;
    using layoutType  = LayoutType;

    __aicore__ inline base_tensor() {}
    __aicore__ inline base_tensor(const EngineType& engine, const LayoutType& layout) : rep(layout, engine) {}

    static constexpr int rank  = LayoutType::rank; // tuple size

    __aicore__ inline constexpr decltype(auto) tensor() const {
        return *this;
    }

    __aicore__ inline constexpr decltype(auto) engine() const {
        return Std::get<1>(rep);
    }

    __aicore__ inline constexpr decltype(auto) engine() {
        return Std::get<1>(rep);
    }

    __aicore__ inline constexpr decltype(auto) layout() const {
        return Std::get<0>(rep);
    }

    __aicore__ inline constexpr decltype(auto) data() const {
        return engine().begin();
    }

    __aicore__ inline constexpr decltype(auto) data() {
        return engine().begin();
    }

    __aicore__ inline constexpr decltype(auto) shape() const {
        return layout().shape();
    }

    __aicore__ inline constexpr decltype(auto) stride() const {
        return layout().stride();
    }

    __aicore__ inline constexpr auto size() const {
        return layout().size();
    }

    __aicore__ inline constexpr auto capacity() const {
        return layout().capacity();
    }

    template <typename Coord>
    __aicore__ inline constexpr decltype(auto) operator[](const Coord& coord) {
        return data()[layout()(coord)];
    }

    template <typename Coord>
    __aicore__ inline constexpr decltype(auto) operator[](const Coord& coord) const {
        return data()[layout()(coord)];
    }

    template <typename Coord>
    __aicore__ inline constexpr decltype(auto) operator()(const Coord& coord) {
        auto slice_engine = engine() + layout()(coord);
        auto coord_layout = make_coord_layout(coord, layout());
        return make_sub_tensor(slice_engine, coord_layout);
    }

    template <typename Coord>
    __aicore__ inline constexpr decltype(auto) operator()(const Coord& coord) const {
        auto slice_engine = engine() + layout()(coord);
        auto coord_layout = make_coord_layout(coord, layout());
        return make_sub_tensor(slice_engine, coord_layout);
    }

    template <typename Coord0, typename Coord1, typename... Coords>
    __aicore__ inline constexpr decltype(auto) operator()(const Coord0& c0, const Coord1& c1, const Coords&... cs) {
        return operator()(make_coord(c0,c1,cs...));
    }

    template <typename Coord0, typename Coord1, typename... Coords>
    __aicore__ inline constexpr decltype(auto) operator()(const Coord0& c0, const Coord1& c1, const Coords&... cs) const {
        return operator()(make_coord(c0,c1,cs...));
    }

    template <typename Coord, typename Info>
    __aicore__ inline constexpr decltype(auto) slice(const Coord& coord, const Info& info) {
        auto slice_engine = engine() + layout()(coord);
        auto coord_layout = make_slice_layout(coord, layout(), info);
        return make_sub_tensor(slice_engine, coord_layout);
  	}

    template <typename Coord, typename Info>
    __aicore__ inline constexpr decltype(auto) slice(const Coord& coord, const Info& info) const{
        auto slice_engine = engine() + layout()(coord);
        auto coord_layout = make_slice_layout(coord, layout(), info);
        return make_sub_tensor(slice_engine, coord_layout);
  	}

    __aicore__ inline constexpr decltype(auto) Tensor() const {
        return tensor();
    }

    __aicore__ inline constexpr decltype(auto) Engine() const {
        return engine();
    }

    __aicore__ inline constexpr decltype(auto) Engine() {
        return engine();
    }

    __aicore__ inline constexpr decltype(auto) Layout() const {
        return layout();
    }

    __aicore__ inline constexpr decltype(auto) Data() const {
        return data();
    }

    __aicore__ inline constexpr decltype(auto) Data() {
        return data();
    }

    __aicore__ inline constexpr decltype(auto) Shape() const {
        return shape();
    }

    __aicore__ inline constexpr decltype(auto) Stride() const {
        return stride();
    }

    __aicore__ inline constexpr auto Size() const {
        return size();
    }

    __aicore__ inline constexpr auto Capacity() const {
        return capacity();
    }

    template <typename Coord, typename Info>
    __aicore__ inline constexpr decltype(auto) Slice(const Coord& coord, const Info& info) {
        return slice(coord, info);
    }

    template <typename Coord, typename Info>
    __aicore__ inline constexpr decltype(auto) Slice(const Coord& coord, const Info& info) const {
        return slice(coord, info);
    }

private:
    template <typename SliceEngine, typename SliceLayout>
    __aicore__ inline static constexpr decltype(auto) make_sub_tensor(
        const SliceEngine& slice_engine, const SliceLayout& slice_layout)
    {
        using Location = GetMemLocation<SliceEngine>;
        using ResultTensor = typename MakeTensorResult<Location, SliceEngine, SliceLayout>::type;
        return ResultTensor{slice_engine, slice_layout};
    }

    Std::tuple<layoutType, engineType> rep;
};

template <typename EngineType, typename LayoutType>
using BaseTensor = base_tensor<EngineType, LayoutType>;

template <typename T>
struct is_attr_tensor : Std::false_type {};

template <template <typename> class TensorType, typename Engine, typename Layout>
struct is_attr_tensor<TensorType<TensorAttribute<Engine, Layout>>> : Std::true_type {};

template <typename T>
using IsAttrTensor = is_attr_tensor<T>;

template <typename T>
constexpr bool IsAttrTensorV = is_attr_tensor<Std::remove_cvref_t<T>>::value;

template <typename T>
struct make_tensor_builder {
template <typename Arg0, typename... Args>
    __aicore__ inline constexpr auto operator()(const Arg0& arg0, const Args&... args) const {
        using Engine = ViewEngine<Arg0>;
        if constexpr (sizeof...(Args) == 1 && (IsLayoutV<Args> && ...)) {
            using Layout = typename Std::tuple_element<0, Std::tuple<Args...>>::type;
            using Location = GetMemLocation<Engine>;
            using ResultTensor = typename MakeTensorResult<Location, Engine, Layout>::type;
            return ResultTensor{Engine{arg0}, args...};
        } else if constexpr (sizeof...(Args) == 2 && (Std::is_tuple_v<Std::remove_cvref_t<Args>> && ...)) {
            using Layout = decltype(make_layout(args...));
            using Location = GetMemLocation<Engine>;
            using ResultTensor = typename MakeTensorResult<Location, Engine, Layout>::type;
            return ResultTensor{Engine{arg0}, make_layout(args...)};
        } else {
            static_assert(sizeof...(Args) != sizeof...(Args), 
                "MakeTensor expected a hardware memory pointer and data structure like Layout or Shape and Stride");
        }
    }
};

template <typename T>
using MakeTensorBuilder = make_tensor_builder<T>;

template <typename Iterator, typename... Args>
__aicore__ inline constexpr auto make_tensor(const Iterator& iter, const Args&... args)
{
    static_assert(IsHardwareMemPtrV <Iterator>,
        "MakeTensor expects the first argument to be a memory pointer or iterator");
    return MakeTensorBuilder<Iterator>{}(iter, args...);
}

// Construct a single-batch sub-tensor from a batched tensor by offsetting the engine pointer
// and stripping the leading batch axis from the layout. Layout depth 5 -> 4 (e.g. NZ fractal)
// or 3 -> 2 (e.g. ND row/col). Other depths trigger a compile-time error.
template <typename Tensor>
__aicore__ inline constexpr auto make_single_batch_sub_tensor(const Tensor& t, uint32_t batch_idx)
{
    using LayoutType = typename Tensor::layoutType;
    static_assert(LayoutType::depth == THREE_DIM_DATA || LayoutType::depth == FIVE_DIM_DATA,
        "MakeSingleBatchSubTensor only supports batched layouts of depth 3 or 5.");
    auto layout = t.layout();
    auto batchStride = get<0>(layout.stride());
    auto subEngine = t.engine() + batchStride * batch_idx;
    auto subLayout = remove_batch_dim(layout);
    using NewEngine = Std::remove_cvref_t<decltype(subEngine)>;
    using NewLayout = Std::remove_cvref_t<decltype(subLayout)>;
    using Location = GetMemLocation<NewEngine>;
    using NewTensor = typename MakeTensorResult<Location, NewEngine, NewLayout>::type;
    return NewTensor{subEngine, subLayout};
}

// squeeze (mode 1): drop axes (by index) whose compile-time size is 1; non-one axes are kept (no error).
// Accepts either a Layout (returns a new Layout) or a Tensor (returns a new Tensor over the same engine).
template <
    size_t... SqueezeDims, typename T,
    typename = Std::enable_if_t<(IsLayoutV<T> || IsAttrTensorV<T>) && (sizeof...(SqueezeDims) > 0)>>
__aicore__ inline constexpr auto squeeze(const T& x)
{
    if constexpr (IsAttrTensorV<T>) {
        auto newLayout = squeeze_layout<SqueezeDims...>(x.layout());
        using NewLayout = Std::remove_cvref_t<decltype(newLayout)>;
        using NewEngine = typename T::engineType;
        using Location = GetMemLocation<NewEngine>;
        using NewTensor = typename MakeTensorResult<Location, NewEngine, NewLayout>::type;
        return NewTensor{x.engine(), newLayout};
    } else {
        static_assert(IsLayoutV<T>, "Squeeze input must be a Layout or Tensor");
        return squeeze_layout<SqueezeDims...>(x);
    }
}

// squeeze (mode 2): drop positions marked _1 in an isomorphic pattern tuple, when size is 1.
// Accepts either a Layout or a Tensor.
template <
    typename Pattern, typename T,
    typename = Std::enable_if_t<(IsLayoutV<T> || IsAttrTensorV<T>) && Std::is_tuple_v<Std::remove_cvref_t<Pattern>>>>
__aicore__ inline constexpr auto squeeze(const T& x, const Pattern& pattern)
{
    if constexpr (IsAttrTensorV<T>) {
        auto newLayout = squeeze_layout(x.layout(), pattern);
        using NewLayout = Std::remove_cvref_t<decltype(newLayout)>;
        using NewEngine = typename T::engineType;
        using Location = GetMemLocation<NewEngine>;
        using NewTensor = typename MakeTensorResult<Location, NewEngine, NewLayout>::type;
        return NewTensor{x.engine(), newLayout};
    } else {
        static_assert(IsLayoutV<T>, "Squeeze input must be a Layout or Tensor");
        return squeeze_layout(x, pattern);
    }
}

} // namespace te
} // namespace asc

namespace AscendC {

template <typename EngineType, typename LayoutType>
struct GlobalTensor<TensorAttribute<EngineType, LayoutType>>
    : public asc::te::BaseTensor<EngineType, LayoutType> {
    using TensorApiBase = asc::te::BaseTensor<EngineType, LayoutType>;

    using TensorApiBase::TensorApiBase;

    __aicore__ inline GlobalTensor() = default;

    __aicore__ inline constexpr void set_l2_cache_hint(asc::te::cache_mode mode) {
        this->engine().set_cache_mode(mode);
    }

    __aicore__ inline constexpr void SetL2CacheHint(asc::te::CacheMode mode) {
        set_l2_cache_hint(mode);
    }
};

template <typename EngineType, typename LayoutType>
struct LocalTensor<TensorAttribute<EngineType, LayoutType>>
    : public asc::te::BaseTensor<EngineType, LayoutType> {
    using TensorApiBase = asc::te::BaseTensor<EngineType, LayoutType>;

    using TensorApiBase::TensorApiBase;

    __aicore__ inline LocalTensor() = default;
};

} // namespace AscendC


// PascalCase compatibility wrappers
namespace asc {
namespace te {
template <typename Iterator, typename... Args>
__aicore__ inline constexpr auto MakeTensor(const Iterator& iter, const Args&... args)
{
    return make_tensor(iter, args...);
}

template <typename Tensor>
__aicore__ inline constexpr auto MakeSingleBatchSubTensor(const Tensor& t, uint32_t batch_idx)
{
    return make_single_batch_sub_tensor(t, batch_idx);
}

template <
    size_t... SqueezeDims, typename T,
    typename = Std::enable_if_t<(IsLayoutV<T> || IsAttrTensorV<T>) && (sizeof...(SqueezeDims) > 0)>>
__aicore__ inline constexpr auto Squeeze(const T& x)
{
    return squeeze<SqueezeDims...>(x);
}

template <
    typename Pattern, typename T,
    typename = Std::enable_if_t<(IsLayoutV<T> || IsAttrTensorV<T>) && Std::is_tuple_v<Std::remove_cvref_t<Pattern>>>>
__aicore__ inline constexpr auto Squeeze(const T& x, const Pattern& pattern)
{
    return squeeze(x, pattern);
}
} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_TENSOR_TENSOR_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
