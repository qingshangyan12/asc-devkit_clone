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
 * \file kernel_ptr_utils.h
 * \brief Shared pointer/tensor operand traits used by the pointer-extended DMA Basic APIs.
 *        A single set of APIs accepts either a LocalTensor / GlobalTensor operand or the equivalent
 *        hardware address-space pointer (__gm__ / __ubuf__ / __cbuf__ / __cc__ / ...).
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/utils/kernel_ptr_utils.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_operator_data_copy_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_PTR_UTILS_H__
#endif

#ifndef ASCENDC_MODULE_KERNEL_PTR_UTILS_H
#define ASCENDC_MODULE_KERNEL_PTR_UTILS_H

#include <cstdint>

#include "../../../include/basic_api/kernel_tensor.h"
#include "../../../include/utils/std/type_traits.h"
#include "kernel_utils_macros.h"

namespace AscendC {

/*!
 * \brief Physical address space of a DMA operand, resolved at compile time.
 *        Tensor operands carry no compile-time position information (TPosition is runtime metadata), so they
 *        are reported as kUnknown and keep their original runtime dispatch.
 */
enum class DmaAddrSpace : uint8_t {
    kUnknown = 0,
    kGm,
    kUb,
    kL1,
    kL0A,
    kL0B,
    kL0C,
    kFb,
};

/*!
 * \brief Address space of an address-space qualified pointer. An unqualified pointer (private/local address)
 *        stays kUnknown and therefore never matches a pointer overload.
 */
template <typename T>
struct DmaAddrSpaceOf {
    static constexpr DmaAddrSpace value = DmaAddrSpace::kUnknown;
};

#if !defined(ASCENDC_CPU_DEBUG) || ASCENDC_CPU_DEBUG != 1
// The CPU twin-debug framework defines __gm__ / __ubuf__ / __cbuf__ / __cc__ as empty, which would fold every
// specialization below into the same `T*` and produce a redefinition error. Address-space qualified pointers
// only exist in device compilation, so the mapping is compiled for the NPU path exclusively. In CPU twin-debug
// every pointer operand degrades to kUnknown, which makes the pointer overloads non-viable instead of
// redefining them -- CPU twin-debug keeps calling the original Tensor overloads.
// The specializations match the bare address-space qualified type (no pointer), because
// DmaAddrSpaceQuery strips the pointer before the lookup. An unqualified type falls through to the
// kUnknown primary template (private / local address).
template <typename T>
struct DmaAddrSpaceOf<__gm__ T> {
    static constexpr DmaAddrSpace value = DmaAddrSpace::kGm;
};

template <typename T>
struct DmaAddrSpaceOf<__ubuf__ T> {
    static constexpr DmaAddrSpace value = DmaAddrSpace::kUb;
};

template <typename T>
struct DmaAddrSpaceOf<__cbuf__ T> {
    static constexpr DmaAddrSpace value = DmaAddrSpace::kL1;
};

template <typename T>
struct DmaAddrSpaceOf<__ca__ T> {
    static constexpr DmaAddrSpace value = DmaAddrSpace::kL0A;
};

template <typename T>
struct DmaAddrSpaceOf<__cb__ T> {
    static constexpr DmaAddrSpace value = DmaAddrSpace::kL0B;
};

template <typename T>
struct DmaAddrSpaceOf<__cc__ T> {
    static constexpr DmaAddrSpace value = DmaAddrSpace::kL0C;
};

template <typename T>
struct DmaAddrSpaceOf<__fbuf__ T> {
    static constexpr DmaAddrSpace value = DmaAddrSpace::kFb;
};
#endif // !ASCENDC_CPU_DEBUG

/*!
 * \brief Compile-time query of the address space of an operand type.
 */
template <typename T, bool = Std::is_pointer_v<Std::remove_cv_t<T>>>
struct DmaAddrSpaceQuery {
    // Pointer operand: strip the (possibly address-space qualified) pointer, drop the pointee cv-qualifiers and
    // look the bare pointee type up. remove_cv on an address-space qualified type only strips the pointee
    // cv-qualifiers, so the address space itself survives and stays distinguishable.
    using Pointee = Std::remove_cv_t<Std::remove_pointer_t<Std::remove_cv_t<T>>>;
    static constexpr DmaAddrSpace value = DmaAddrSpaceOf<Pointee>::value;
};

template <typename T>
struct DmaAddrSpaceQuery<T, false> {
    static constexpr DmaAddrSpace value = DmaAddrSpace::kUnknown;
};

template <typename T>
constexpr DmaAddrSpace DmaAddrSpaceOfV = DmaAddrSpaceQuery<T>::value;

template <typename T>
constexpr bool IsDmaGmPtr = DmaAddrSpaceOfV<T> == DmaAddrSpace::kGm;
template <typename T>
constexpr bool IsDmaUbPtr = DmaAddrSpaceOfV<T> == DmaAddrSpace::kUb;
template <typename T>
constexpr bool IsDmaL1Ptr = DmaAddrSpaceOfV<T> == DmaAddrSpace::kL1;
template <typename T>
constexpr bool IsDmaL0CPtr = DmaAddrSpaceOfV<T> == DmaAddrSpace::kL0C;

/*!
 * \brief `true` when the operand is a raw pointer in a hardware address space.
 */
template <typename T>
constexpr bool IsDmaPtr = DmaAddrSpaceOfV<T> != DmaAddrSpace::kUnknown;

/*!
 * \brief `true` when the operand is a LocalTensor / GlobalTensor.
 */
template <typename T>
struct IsDmaTensor : Std::false_type {};

template <typename T>
struct IsDmaTensor<LocalTensor<T>> : Std::true_type {};

template <typename T>
struct IsDmaTensor<GlobalTensor<T>> : Std::true_type {};

template <typename T>
constexpr bool IsDmaTensorV = IsDmaTensor<Std::remove_cv_t<T>>::value;

template <typename T>
struct IsDmaGlobalTensor : Std::false_type {};

template <typename T>
struct IsDmaGlobalTensor<GlobalTensor<T>> : Std::true_type {};

template <typename T>
constexpr bool IsDmaGlobalTensorV = IsDmaGlobalTensor<Std::remove_cv_t<T>>::value;

template <typename T>
struct IsDmaLocalTensor : Std::false_type {};

template <typename T>
struct IsDmaLocalTensor<LocalTensor<T>> : Std::true_type {};

template <typename T>
constexpr bool IsDmaLocalTensorV = IsDmaLocalTensor<Std::remove_cv_t<T>>::value;

/*!
 * \brief Drops the hardware address space qualifier of a pointee type. An address space qualifier survives
 *        remove_cv, so it has to be peeled off explicitly before the pointee can be used as a template
 *        argument (for instance DataCopyPadExtParams<...> must receive `float`, never `__gm__ float`).
 *        An unqualified type falls through untouched.
 */
template <typename T>
struct DmaStripAddrSpace {
    using type = T;
};

#if !defined(ASCENDC_CPU_DEBUG) || ASCENDC_CPU_DEBUG != 1
template <typename T>
struct DmaStripAddrSpace<__gm__ T> {
    using type = T;
};

template <typename T>
struct DmaStripAddrSpace<__ubuf__ T> {
    using type = T;
};

template <typename T>
struct DmaStripAddrSpace<__cbuf__ T> {
    using type = T;
};

template <typename T>
struct DmaStripAddrSpace<__ca__ T> {
    using type = T;
};

template <typename T>
struct DmaStripAddrSpace<__cb__ T> {
    using type = T;
};

template <typename T>
struct DmaStripAddrSpace<__cc__ T> {
    using type = T;
};

template <typename T>
struct DmaStripAddrSpace<__fbuf__ T> {
    using type = T;
};
#endif // !ASCENDC_CPU_DEBUG

template <typename T>
using DmaStripAddrSpaceT = typename DmaStripAddrSpace<T>::type;

/*!
 * \brief Element type of a DMA operand: the pointee type for a raw pointer, PrimType for a Tensor. Both
 *        reduce to the same primitive type the original Tensor-only signature used as its `T`.
 */
template <typename T, bool = IsDmaTensorV<T>>
struct DmaOperandElemTImpl {
    using type = DmaStripAddrSpaceT<Std::remove_cv_t<Std::remove_pointer_t<Std::remove_cv_t<T>>>>;
};

template <typename T>
struct DmaOperandElemTImpl<T, true> {
    using type = typename Std::remove_cv_t<T>::PrimType;
};

template <typename T>
struct DmaOperandElemT {
    using type = typename DmaOperandElemTImpl<T>::type;
};

template <typename T>
using DmaOperandElem = typename DmaOperandElemT<T>::type;

/*!
 * \brief Primitive element type behind the address-space casts made at the call sites. For a Tensor this is
 *        the tensor's own PrimType, which is exactly the expression the original code used; for a pointer it
 *        is the bare pointee type. Both branches yield the same type for equivalent operands.
 */
template <typename T, bool = IsDmaTensorV<T>>
struct DmaPrimTImpl {
    using type = DmaOperandElem<T>;
};

template <typename T>
struct DmaPrimTImpl<T, true> {
    using type = typename Std::remove_cv_t<T>::PrimType;
};

template <typename T>
using DmaPrimT = typename DmaPrimTImpl<T>::type;

/*!
 * \brief Operand shape predicates for the address-space generic overloads.
 *
 * Disambiguation never relies on the address space: every generic family is separated by the operand slot
 * (GlobalTensor vs LocalTensor, checked by type) and by the parameter types. The address space is only used
 * to accept or reject a pointer in a slot, which is what the following predicates express.
 *
 * In CPU twin-debug the qualifiers are empty, so the space of a pointer is unrecoverable at compile time and
 * every raw pointer is accepted in either slot; the address space check then degrades to the caller, which is
 * the documented behaviour of the pointer paths. This keeps the pointer overloads viable -- and therefore
 * executable in CPU twin-debug tests -- instead of removing them from that build.
 */
#if !defined(ASCENDC_CPU_DEBUG) || ASCENDC_CPU_DEBUG != 1
template <typename T>
constexpr bool IsDmaPtrOperand = IsDmaPtr<T>;

template <typename T>
constexpr bool IsDmaGmPtrOperand = IsDmaGmPtr<T>;

template <typename T>
constexpr bool IsDmaLocalPtrOperand = IsDmaPtr<T> && !IsDmaGmPtr<T>;
#else
template <typename T>
constexpr bool IsDmaPtrOperand = Std::is_pointer_v<Std::remove_cv_t<T>>;

template <typename T>
constexpr bool IsDmaGmPtrOperand = Std::is_pointer_v<Std::remove_cv_t<T>>;

template <typename T>
constexpr bool IsDmaLocalPtrOperand = Std::is_pointer_v<Std::remove_cv_t<T>>;
#endif // !ASCENDC_CPU_DEBUG

// GM slot: a GlobalTensor, or a GM pointer. Never a LocalTensor, so the GM and the local slot of a family can
// not both be satisfied by the same Tensor operand.
template <typename T>
constexpr bool IsDmaGmOperand = IsDmaGlobalTensorV<T> || IsDmaGmPtrOperand<T>;

// Local slot: a LocalTensor, or a non-GM hardware pointer. Never a GlobalTensor.
template <typename T>
constexpr bool IsDmaLocalOperand = IsDmaLocalTensorV<T> || IsDmaLocalPtrOperand<T>;

/*!
 * \brief `true` when at least one operand is a raw pointer. The generic overloads are only enabled by this
 *        clause, which guarantees that a Tensor/Tensor call keeps resolving to the original Tensor-only
 *        signatures: every generic candidate is SFINAE-removed for Tensor/Tensor, so no ambiguity is
 *        introduced and the original overload resolution is untouched.
 */
template <typename DstT, typename SrcT>
constexpr bool IsDmaPtrInvolved = IsDmaPtrOperand<DstT> || IsDmaPtrOperand<SrcT>;

/*!
 * \brief Slot predicates used by the generic movement families. A family is identified by the operand slots;
 *        these predicates come in two strengths, and the distinction is what keeps the directional overloads of
 *        one parameter list mutually exclusive in both compilation regimes.
 *
 *        "Acceptable": the operand may occupy the slot. A raw pointer is acceptable in the local slot even in
 *        CPU twin-debug, because a Tensor on the other side already pins the direction there, so accepting the
 *        pointer introduces no ambiguity.
 *
 *        "Proven": the slot is pinned by the operand alone. A Tensor always proves its slot. A pointer proves
 *        it on device, where the address space says the slot. In CPU twin-debug a pointer proves nothing -- but
 *        since a pointer in a GM-ward call is taken as the GM operand (see DmaProveGm below), taking the pair
 *        as GM -> local is the only consistent reading there.
 */
template <typename T>
constexpr bool DmaAcceptLocal = IsDmaLocalTensorV<T> || IsDmaPtrOperand<T>;

#if !defined(ASCENDC_CPU_DEBUG) || ASCENDC_CPU_DEBUG != 1
template <typename T>
constexpr bool DmaProveGm = IsDmaGlobalTensorV<T> || IsDmaGmPtr<T>;

template <typename T>
constexpr bool DmaProveLocal = IsDmaLocalTensorV<T> || IsDmaLocalPtrOperand<T>;
#else
// In CPU twin-debug a raw pointer carries no address space, so it can not prove a local slot: a pointer paired
// with a LocalTensor is read as the GM operand (GM -> local), and a pointer paired with a GlobalTensor as the
// local operand (local -> GM). Both readings are consistent with the two mixed forms the task requires, and a
// pointer whose simulated buffer turns out to be local still fails the position check of the body instead of
// silently moving data, so the misreading can not go unnoticed.
template <typename T>
constexpr bool DmaProveGm = IsDmaGlobalTensorV<T> || IsDmaPtrOperand<T>;

template <typename T>
constexpr bool DmaProveLocal = IsDmaLocalTensorV<T>;
#endif // !ASCENDC_CPU_DEBUG

/*!
 * \brief Whether the operand pair may enter a generic overload at all: every pair involving a raw pointer on
 *        device, and the Tensor / pointer mixed pairs in CPU twin-debug.
 *
 *        A pair of raw pointers pins no direction in CPU twin-debug, because the address-space qualifiers are
 *        empty there, so it is excluded in that build -- a pointer / pointer call has to be compiled for the
 *        device. The mixed pairs stay viable in both regimes. This is the §2.1.4.1 guard, kept in one place so
 *        that it can be applied uniformly to every generic declaration.
 */
#if !defined(ASCENDC_CPU_DEBUG) || ASCENDC_CPU_DEBUG != 1
template <typename DstT, typename SrcT>
constexpr bool DmaPtrShapeAllowed = IsDmaPtrOperand<DstT> || IsDmaPtrOperand<SrcT>;
#else
template <typename DstT, typename SrcT>
constexpr bool DmaPtrShapeAllowed = IsDmaPtrOperand<DstT> != IsDmaPtrOperand<SrcT>;
#endif // !ASCENDC_CPU_DEBUG

/*!
 * \brief The three operand shapes of a movement family, expressed purely in terms of the slots:
 *
 *          GM -> local   : the source proves GM, the destination may take the local slot
 *          local -> GM   : the destination proves GM, the source may take the local slot
 *          local -> local: both operands prove the local slot
 *
 *        These carry the slot logic only; every generic declaration combines them with IsDmaPtrInvolved and
 *        DmaPtrShapeAllowed, which is what removes the Tensor / Tensor calls (so the original overloads keep
 *        their resolution) and the pointer / pointer pairs in CPU twin-debug.
 *
 *        Combined that way the three are mutually exclusive in both regimes, which is what lets the three
 *        directions of one parameter list coexist as separate overloads -- necessary because the pipe attribute
 *        (MTE2 for the loads, MTE3 for the stores, none for local -> local) belongs to the direction, not to the
 *        operand shapes. The exclusivity was checked exhaustively over the operand matrix in both regimes.
 */
template <typename DstT, typename SrcT>
constexpr bool DmaGmToLocalShape = DmaAcceptLocal<DstT> && DmaProveGm<SrcT>;

template <typename DstT, typename SrcT>
constexpr bool DmaLocalToGmShape = DmaProveGm<DstT> && DmaAcceptLocal<SrcT>;

template <typename DstT, typename SrcT>
constexpr bool DmaLocalToLocalShape = DmaProveLocal<DstT> && DmaProveLocal<SrcT>;

/*! * \brief Compile-time hardware position of a local-memory operand, used to reproduce the runtime
 *        GetPosition() dispatch of the Tensor path inside an `if constexpr`.
 *
 *        It resolves to Hardware::MAX for anything that is not a hardware-address-space pointer, which is
 *        exactly what an implementation wants from a Tensor operand: keep dispatching on it at run time. In CPU
 *        twin-debug the address qualifiers are empty, so every pointer also resolves to Hardware::MAX and the
 *        runtime dispatch is used, which is the documented degradation of the pointer path there.
 */
template <typename T, DmaAddrSpace space = (IsDmaPtr<T> ? DmaAddrSpaceOfV<T> : DmaAddrSpace::kUnknown)>
struct DmaHardwarePos {
    static constexpr Hardware value = Hardware::MAX;
};

template <typename T>
struct DmaHardwarePos<T, DmaAddrSpace::kGm> {
    static constexpr Hardware value = Hardware::GM;
};

template <typename T>
struct DmaHardwarePos<T, DmaAddrSpace::kUb> {
    static constexpr Hardware value = Hardware::UB;
};

template <typename T>
struct DmaHardwarePos<T, DmaAddrSpace::kL1> {
    static constexpr Hardware value = Hardware::L1;
};

template <typename T>
struct DmaHardwarePos<T, DmaAddrSpace::kL0A> {
    static constexpr Hardware value = Hardware::L0A;
};

template <typename T>
struct DmaHardwarePos<T, DmaAddrSpace::kL0B> {
    static constexpr Hardware value = Hardware::L0B;
};

template <typename T>
struct DmaHardwarePos<T, DmaAddrSpace::kL0C> {
    static constexpr Hardware value = Hardware::L0C;
};

template <typename T>
struct DmaHardwarePos<T, DmaAddrSpace::kFb> {
    static constexpr Hardware value = Hardware::FIXBUF;
};

template <typename T>
constexpr Hardware DmaHardwarePosV = DmaHardwarePos<T>::value;

/*!
 * \brief True when the runtime position of a Tensor operand must still be consulted. A Tensor operand has no
 *        compile-time position, so the dispatcher keeps its original runtime branch for it.
 */
template <typename T>
constexpr bool IsDmaPosRuntime = DmaHardwarePosV<T> == Hardware::MAX;

/*!
 * \brief Directional operand shapes of a data movement family. A family keeps one generic overload per
 *        direction because the pipe attribute belongs to the direction (MTE2 for the loads, MTE3 for the
 *        stores, none for the local -> local family), not to the operand shapes. See DmaGmToLocalShape for how
 *        the three directions are kept mutually exclusive.
 */

/*! \brief The two shapes handled by the MTE3 store family: local -> GM and the local -> local copies. */
template <typename DstT, typename SrcT>
constexpr bool DmaLocalToGmOrLocalShape = DmaLocalToGmShape<DstT, SrcT> || DmaLocalToLocalShape<DstT, SrcT>;
/*!
 * \brief Pointer extraction: a raw pointer is passed through unchanged (no reinterpret, no offset fixup), a
 *        Tensor degrades to its physical address. Fully resolved at compile time, so no runtime instruction is
 *        generated for either branch.
 */
template <typename T>
__aicore__ inline constexpr auto GetUnderlyingPtr(const T& val)
{
    if constexpr (Std::is_pointer_v<Std::remove_cv_t<T>>) {
        return val;
    } else {
        return val.GetPhyAddr();
    }
}

#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
/*!
 * \brief CPU twin-debug only: recover the hardware position of a raw address by locating it inside one of the
 *        simulated buffers.
 *
 *        This is what keeps the Tensor / pointer mixed DataCopy forms executable in CPU twin-debug. The
 *        address-space qualifiers are empty there, so a pointer operand carries no compile-time position and
 *        its dispatch has to fall back to a runtime lookup. A Tensor keeps resolving its position from its
 *        TPosition, exactly as the previous Tensor-only code did.
 */
__aicore__ inline Hardware GetDmaHardwarePosByAddress(const void* addr)
{
    const uintptr_t target = reinterpret_cast<uintptr_t>(addr);
    const auto& definer = ConstDefiner::Instance();
    for (const auto& entry : definer.hardwareCpuBufferMap) {
        const auto lenIt = definer.bufferInitLen.find(entry.first);
        if (lenIt == definer.bufferInitLen.end()) {
            continue;
        }
        const uintptr_t base = reinterpret_cast<uintptr_t>(entry.second);
        if (target >= base && target < base + lenIt->second) {
            return entry.first;
        }
    }
    // The simulated GM window is not part of hardwareCpuBufferMap: cpuGM is allocated on its own and never
    // inserted there. Since the only meaningful raw pointer outside a local buffer is a GM one, an address
    // that matches no local buffer is GM. Answering MAX instead would misreport every GM pointer of a mixed
    // call as an unsupported position.
    return Hardware::GM;
}
#endif // ASCENDC_CPU_DEBUG

/*!
 * \brief Hardware position of an operand of a generic movement family.
 *
 *        A Tensor keeps its original runtime resolution from its TPosition -- literally
 *        `GetPhyType((TPosition)dst.GetPosition())` of the Tensor-only code. A pointer resolves at compile time
 *        from its address space on device. In CPU twin-debug a pointer falls back to the simulated buffers,
 *        which is the documented degradation there (on device such a lookup is not needed and not compiled).
 */
template <typename T>
__aicore__ inline Hardware GetDmaOperandHardwarePos(const T& operand)
{
    if constexpr (!IsDmaPosRuntime<T>) {
        return DmaHardwarePosV<T>;
    } else if constexpr (Std::is_pointer_v<Std::remove_cv_t<T>>) {
#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
        return GetDmaHardwarePosByAddress(GetUnderlyingPtr(operand));
#else
        return Hardware::MAX;
#endif
    } else {
        return GetPhyType((TPosition)operand.GetPosition());
    }
}

/*!
 * \brief A representative TPosition of a hardware position, used for diagnostics only.
 */
constexpr TPosition DmaHardwareToPosition(Hardware hardPos)
{
    return (hardPos == Hardware::GM)   ? TPosition::GM :
           (hardPos == Hardware::UB)   ? TPosition::VECCALC :
           (hardPos == Hardware::L1)   ? TPosition::A1 :
           (hardPos == Hardware::L0A)  ? TPosition::A2 :
           (hardPos == Hardware::L0B)  ? TPosition::B2 :
           (hardPos == Hardware::L0C)  ? TPosition::CO1 :
           (hardPos == Hardware::BIAS) ? TPosition::C2 :
                                         TPosition::MAX;
}

/*!
 * \brief Position of an operand as a TPosition, for the position diagnostics of the generic families.
 *
 *        A Tensor reports its own TPosition, which is what the Tensor-only code passed to GetPositionDisplay. A
 *        pointer reports the representative TPosition of its hardware position. The value is only consumed as an
 *        argument of the check macros, whose arguments are dropped entirely on device, so a pointer operand
 *        costs nothing there.
 */
template <typename T>
__aicore__ inline TPosition GetDmaOperandPosition(const T& operand)
{
    if constexpr (Std::is_pointer_v<Std::remove_cv_t<T>>) {
        return DmaHardwareToPosition(GetDmaOperandHardwarePos(operand));
    } else {
        return static_cast<TPosition>(operand.GetPosition());
    }
}
} // namespace AscendC

#endif // ASCENDC_MODULE_KERNEL_PTR_UTILS_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_PTR_UTILS_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_PTR_UTILS_H__
#endif
