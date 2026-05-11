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
    "impl/tensor_api/tensor/local_tensor_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
* \file local_tensor_impl.h
* \brief
*/
#ifndef IMPL_TENSOR_API_TENSOR_LOCAL_TENSOR_IMPL_H
#define IMPL_TENSOR_API_TENSOR_LOCAL_TENSOR_IMPL_H

#include "impl/tensor_api/utils/utils_impl.h"
#include "impl/tensor_api/tensor/layout_impl.h"
#include "impl/tensor_api/tensor/engine_impl.h"
#include "impl/tensor_api/tensor/pointer_pattern.h"
#include "include/basic_api/kernel_tensor.h"

namespace AscendC {
namespace Te {

template <typename EngineType, typename LayoutType>
struct TensorAttribute {};

template <typename T>
struct BaseTensor {};

template <typename LocationType, typename TensorType>
struct MakeTensorResult {
    static_assert(!Std::is_same_v<LocationType, Location::INVALID>,
        "MakeTensor does not support Location::INVALID.");
    using type = typename Std::conditional<Std::is_same_v<LocationType, Location::GM>,
        AscendC::GlobalTensor<TensorType>, AscendC::LocalTensor<TensorType>>::type;
};

template <typename LocationType>
struct TensorPosition;

template <>
struct TensorPosition<Location::UB> {
    static constexpr TPosition value = TPosition::VECIN;
};

template <>
struct TensorPosition<Location::L1> {
    static constexpr TPosition value = TPosition::A1;
};

template <>
struct TensorPosition<Location::L0A> {
    static constexpr TPosition value = TPosition::A2;
};

template <>
struct TensorPosition<Location::L0B> {
    static constexpr TPosition value = TPosition::B2;
};

template <>
struct TensorPosition<Location::L0C> {
    static constexpr TPosition value = TPosition::CO1;
};

template <>
struct TensorPosition<Location::FIXBUF> {
    static constexpr TPosition value = TPosition::C2PIPE2GM;
};

template <>
struct TensorPosition<Location::BIAS> {
    static constexpr TPosition value = TPosition::C2;
};

template <typename Pointer>
struct MemPtrValue {
    using type = Std::remove_cv_t<Std::remove_pointer_t<Pointer>>;
};

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wignored-attributes"
template <typename T>
struct MemPtrValue<__gm__ T*> {
    using type = Std::remove_cv_t<T>;
};

template <typename T>
struct MemPtrValue<__ubuf__ T*> {
    using type = Std::remove_cv_t<T>;
};

template <typename T>
struct MemPtrValue<__cbuf__ T*> {
    using type = Std::remove_cv_t<T>;
};

template <typename T>
struct MemPtrValue<__ca__ T*> {
    using type = Std::remove_cv_t<T>;
};

template <typename T>
struct MemPtrValue<__cb__ T*> {
    using type = Std::remove_cv_t<T>;
};

template <typename T>
struct MemPtrValue<__cc__ T*> {
    using type = Std::remove_cv_t<T>;
};

template <typename T>
struct MemPtrValue<__fbuf__ T*> {
    using type = Std::remove_cv_t<T>;
};

template <typename T>
struct MemPtrValue<__biasbuf__ T*> {
    using type = Std::remove_cv_t<T>;
};
#pragma clang diagnostic pop

template <typename Iterator>
struct LegacyIteratorValue {
    using type = typename Iterator::valueType;
};

template <typename PtrPattern, typename Pointer>
struct LegacyIteratorValue<HardwareMemPtr<PtrPattern, Pointer>> {
    using type = typename MemPtrValue<Pointer>::type;
};

template <typename EngineType>
using LegacyTensorValueT = typename LegacyIteratorValue<typename EngineType::iterator>::type;

template <typename EngineType, typename LayoutType>
struct BaseTensor<TensorAttribute<EngineType, LayoutType>> {
    using iterator = typename EngineType::iterator;
    using valueType = typename EngineType::valueType;
    using elementType = typename EngineType::elementType;
    using reference = typename EngineType::reference;

    using engineType  = EngineType;
    using layoutType  = LayoutType;

    __aicore__ inline BaseTensor() {}
    __aicore__ inline BaseTensor(const EngineType& engine, const LayoutType& layout) : rep(layout, engine) {}

    static constexpr int rank  = LayoutType::rank; // tuple size

    __aicore__ inline constexpr decltype(auto) Tensor() const {
        return *this;
    }

    __aicore__ inline constexpr decltype(auto) Engine() const {
        return Std::get<1>(rep);
    }

    __aicore__ inline constexpr decltype(auto) Engine() {
        return Std::get<1>(rep);
    }

    __aicore__ inline constexpr decltype(auto) Layout() const {
        return Std::get<0>(rep);
    }

    __aicore__ inline constexpr decltype(auto) Data() const {
        return Engine().Begin();
    }

    __aicore__ inline constexpr decltype(auto) Data() {
        return Engine().Begin();
    }

    __aicore__ inline constexpr decltype(auto) Shape() const {
        return Layout().Shape();
    }

    __aicore__ inline constexpr decltype(auto) Stride() const {
        return Layout().Stride();
    }

    __aicore__ inline constexpr auto Size() const {
        return Layout().Size();
    }

    __aicore__ inline constexpr auto Capacity() const {
        return Layout().Capacity();
    }

    template <typename Coord>
    __aicore__ inline constexpr decltype(auto) operator[](const Coord& coord) {
        return Data()[Layout()(coord)];
    }

    template <typename Coord>
    __aicore__ inline constexpr decltype(auto) operator[](const Coord& coord) const {
        return Data()[Layout()(coord)];
    }

    template <typename Coord>
    __aicore__ inline constexpr decltype(auto) operator()(const Coord& coord) {
        auto sliceEngine = Engine() + Layout()(coord);
 	    auto coordLayout = MakeCoordLayout(coord, Layout());
        using Location = GetMemLocation<decltype(sliceEngine)>;
        using AttrTensor = TensorAttribute<decltype(sliceEngine), decltype(coordLayout)>;
        using ResultTensor = typename MakeTensorResult<Location, AttrTensor>::type;
        return ResultTensor{sliceEngine, coordLayout};
    }

    template <typename Coord>
    __aicore__ inline constexpr decltype(auto) operator()(const Coord& coord) const {
        auto sliceEngine = Engine() + Layout()(coord);
 	    auto coordLayout = MakeCoordLayout(coord, Layout());
        using Location = GetMemLocation<decltype(sliceEngine)>;
        using AttrTensor = TensorAttribute<decltype(sliceEngine), decltype(coordLayout)>;
        using ResultTensor = typename MakeTensorResult<Location, AttrTensor>::type;
        return ResultTensor{sliceEngine, coordLayout};
    }

    template <typename Coord0, typename Coord1, typename... Coords>
    __aicore__ inline constexpr decltype(auto) operator()(const Coord0& c0, const Coord1& c1, const Coords&... cs) {
        return operator()(MakeCoord(c0,c1,cs...));
    }

    template <typename Coord0, typename Coord1, typename... Coords>
    __aicore__ inline constexpr decltype(auto) operator()(const Coord0& c0, const Coord1& c1, const Coords&... cs) const {
        return operator()(MakeCoord(c0,c1,cs...));
    }

    template <typename Coord, typename Info>
  	__aicore__ inline constexpr decltype(auto) Slice(const Coord& coord, const Info& info) {
        auto sliceEngine = Engine() + Layout()(coord);
 	    auto coordLayout = MakeSliceLayout(coord, Layout(), info);
        using Location = GetMemLocation<decltype(sliceEngine)>;
        using AttrTensor = TensorAttribute<decltype(sliceEngine), decltype(coordLayout)>;
        using ResultTensor = typename MakeTensorResult<Location, AttrTensor>::type;
 	    return ResultTensor{sliceEngine, coordLayout};
  	}

    template <typename Coord, typename Info>
  	__aicore__ inline constexpr decltype(auto) Slice(const Coord& coord, const Info& info) const{
        auto sliceEngine = Engine() + Layout()(coord);
 	    auto coordLayout = MakeSliceLayout(coord, Layout(), info);
        using Location = GetMemLocation<decltype(sliceEngine)>;
        using AttrTensor = TensorAttribute<decltype(sliceEngine), decltype(coordLayout)>;
        using ResultTensor = typename MakeTensorResult<Location, AttrTensor>::type;
 	    return ResultTensor{sliceEngine, coordLayout};
  	}

    template <typename... Layouts>
    __aicore__ inline constexpr auto Compose(const Layouts&... layouts) {
        return MakeTensor(Data(), Layout().Compose(layouts...));
    }

    template <typename... Layouts>
    __aicore__ inline constexpr auto Compose(const Layouts&... layouts) const {
        return MakeTensor(Data(), Layout().Compose(layouts...));
    }

    template <typename... Layouts>
    __aicore__ inline constexpr auto Tile(const Layouts&... layouts) {
        return MakeTensor(Data(), Layout().Tile(layouts...));
    }

    template <typename... Layouts>
    __aicore__ inline constexpr auto Tile(const Layouts&... layouts) const {
        return MakeTensor(Data(), Layout().Tile(layouts...));
    }

    __aicore__ inline constexpr void SetL2CacheHint(CacheMode mode) {
        Engine().SetCacheMode(mode);
    }

private:
    Std::tuple<layoutType, engineType> rep;
};

template <typename T>
struct IsAttrTensor : Std::false_type {};

template <template <typename> class TensorType, typename Engine, typename Layout>
struct IsAttrTensor<TensorType<TensorAttribute<Engine, Layout>>> : Std::true_type {};

template <typename T>
constexpr bool IsAttrTensorV = IsAttrTensor<Std::remove_cvref_t<T>>::value;

// tensor construction
template <typename T, typename = void>
struct HasDereference : Std::false_type {};

template <typename T>
struct HasDereference<T, void_t<decltype(*Std::declval<T&>())>> : Std::true_type {};

template <typename T>
struct MakeTensorImpl {
template <typename Arg0, typename... Args>
    __aicore__ inline constexpr auto operator()(const Arg0& arg0, const Args&... args) const {
        if constexpr (HasDereference<Arg0>::value) {
            using Engine = ViewEngine<Arg0>;
            if constexpr (sizeof...(Args) == 1 && (IsLayoutV<Args> && ...)) {
                using Layout = typename Std::tuple_element<0, Std::tuple<Args...>>::type;
                using AttrTensor = TensorAttribute<Engine, Layout>;
                using Location = GetMemLocation<Engine>;
                using ResultTensor = typename MakeTensorResult<Location, AttrTensor>::type;
                return ResultTensor{Engine{arg0}, args...};
            } else {
                using Layout = decltype(MakeLayout(args...));
                using AttrTensor = TensorAttribute<Engine, Layout>;
                using Location = GetMemLocation<Engine>;
                using ResultTensor = typename MakeTensorResult<Location, AttrTensor>::type;
                return ResultTensor{Engine{arg0}, MakeLayout(args...)};
            }
        }
    }
};

template <typename Iterator, typename... Args>
__aicore__ inline constexpr auto MakeTensor(const Iterator& iter, const Args&... args)
{
    static_assert(HasDereference<Iterator>::value, "Expected iterator iter in MakeTensorImpl(iter, args...)");
    static_assert(!(HasDereference<Args>::value && ...), "Expected layout args... in MakeTensorImpl(iter, args...)");
    return MakeTensorImpl<Iterator>{}(iter, args...);
}

} // namespace Te

template <typename EngineType, typename LayoutType>
struct GlobalTensor<Te::TensorAttribute<EngineType, LayoutType>>
    : public Te::BaseTensor<Te::TensorAttribute<EngineType, LayoutType>>,
      public AscendC::GlobalTensor<Te::LegacyTensorValueT<EngineType>> {
    using TensorApiBase = Te::BaseTensor<Te::TensorAttribute<EngineType, LayoutType>>;
    using LegacyValue = Te::LegacyTensorValueT<EngineType>;
    using BasicTensorBase = AscendC::GlobalTensor<LegacyValue>;

    using TensorApiBase::TensorApiBase;
    using BasicTensorBase::BasicTensorBase;

    __aicore__ inline GlobalTensor() = default;

    __aicore__ inline GlobalTensor(const EngineType& engine, const LayoutType& layout)
        : TensorApiBase(engine, layout), BasicTensorBase()
    {
        this->SetGlobalBuffer(reinterpret_cast<__gm__ LegacyValue*>(engine.Begin().Get()), layout.Size());
    }
};

template <typename EngineType, typename LayoutType>
struct LocalTensor<Te::TensorAttribute<EngineType, LayoutType>>
    : public Te::BaseTensor<Te::TensorAttribute<EngineType, LayoutType>>,
      public AscendC::LocalTensor<Te::LegacyTensorValueT<EngineType>> {
    using TensorApiBase = Te::BaseTensor<Te::TensorAttribute<EngineType, LayoutType>>;
    using LegacyValue = Te::LegacyTensorValueT<EngineType>;
    using BasicTensorBase = AscendC::LocalTensor<LegacyValue>;

    using TensorApiBase::TensorApiBase;
    using BasicTensorBase::BasicTensorBase;

    __aicore__ inline LocalTensor() = default;

    __aicore__ inline LocalTensor(const EngineType& engine, const LayoutType& layout)
        : TensorApiBase(engine, layout), BasicTensorBase(GetTensorPosition(), GetLocalOffset(engine), layout.Size())
    {}

private:
    __aicore__ inline static constexpr TPosition GetTensorPosition()
    {
        using Location = Te::GetMemLocation<EngineType>;
        static_assert(!Std::is_same_v<Location, Te::Location::GM>, "Global memory should use GlobalTensor");
        return Te::TensorPosition<Location>::value;
    }

    __aicore__ inline static uint32_t GetLocalOffset(const EngineType& engine)
    {
        return static_cast<uint32_t>(reinterpret_cast<uint64_t>(engine.Begin().Get()) -
                                     reinterpret_cast<uint64_t>(asc_get_phy_buf_addr(0)));
    }
};

} // namespace AscendC

#endif // IMPL_TENSOR_API_TENSOR_LOCAL_TENSOR_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
