/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#include <vector>
#include <cstring>
#include <numeric>
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <cxxabi.h>
#include "mockcpp/mockcpp.hpp"
#include "tensor_api/stub/cce_stub.h"
#include "include/tensor_api/tensor.h"

class tensor_api_cube_copy_3510 : public testing::Test {
protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}

    void SetUp() override 
    {
        AscendC::SetGCoreType(1);
    }

    void TearDown() override 
    {
        AscendC::SetGCoreType(0);
    }
};

namespace {

template <typename location_tag, typename pointer_type, typename layout_type>
auto make_tensor_at(pointer_type ptr, const layout_type& layout)
{
    return asc::te::make_tensor(asc::te::make_mem_ptr<location_tag>(ptr), layout);
}

template <typename copy_operation, typename trait_type, typename dst_tensor_type, typename src_tensor_type>
void run_copy_call_paths(const dst_tensor_type& dst, const src_tensor_type& src)
{
    using namespace asc::te;

    auto atom = make_copy(copy_operation{}, trait_type{});
    atom.call(dst, src);

    copy_atom<copy_traits<copy_operation, trait_type>>{}.call(dst, src);
    copy(copy_atom<copy_traits<copy_operation, trait_type>>{}, dst, src);
}

template <typename copy_operation, typename trait_type, typename dst_tensor_type, typename src_tensor_type>
void run_copy_with_paths(const dst_tensor_type& dst, const src_tensor_type& src)
{
    using namespace asc::te;

    auto atom = copy_atom<copy_traits<copy_operation, trait_type>>{}.with();
    atom.call(dst, src);
    copy(atom, dst, src);
}

} // namespace

TEST_F(tensor_api_cube_copy_3510, copy_gm_to_l1_routes_to_cube_arch_copy)
{
    using namespace asc::te;

    constexpr uint32_t m = 32;
    constexpr uint32_t n = 32;
    __gm__ float src[m * n] = {0};
    __cbuf__ float dst[m * n] = {0};

    auto gm_tensor = make_tensor_at<location::gm>(src, make_frame_layout<nd_ext_layout_ptn>(m, n));
    auto l1_tensor = make_tensor_at<location::l1>(dst, make_frame_layout<nd_ext_layout_ptn>(m, n));

    run_copy_call_paths<copy_gm_to_l1, copy_gm_to_l1_trait_default>(l1_tensor, gm_tensor);
    run_copy_with_paths<copy_gm_to_l1, copy_gm_to_l1_trait_default>(l1_tensor, gm_tensor);

    EXPECT_EQ(dst[0], 0);
}

using namespace asc::te;
using namespace asc;

using copy_coord_3d = decltype(make_coord(0, 0, 0));
using copy_coord_4d = decltype(make_coord(0, 0, 0, 0));
using copy_shape_4d = decltype(make_shape(1, 1, 1, 1));
static_assert(is_copy_coord_shape_compatible_v<copy_coord_4d, copy_shape_4d>);
static_assert(!is_copy_coord_shape_compatible_v<copy_coord_3d, copy_shape_4d>);

constexpr bool g_debug_print = false; // Set to true to enable debug printing of captured data

// Mock implementations for data copy about gm2l1 functions
extern void set_loop1_stride_outtol1(uint64_t config);
extern void set_loop2_stride_outtol1(uint64_t config);
extern void set_loop_size_outtol1(uint64_t config);
extern void set_pad_val_outtol1(uint64_t config);
extern void copy_gm_to_cbuf_align_v2(half* dst, half* src, uint8_t sid, uint32_t burst_num, uint32_t burst_len,
                                     uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit,
                                     uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride);
extern void copy_gm_to_cbuf_multi_nd2nz(half* dst, half* src, uint8_t sid, uint64_t loop1_src_stride,
                                        uint8_t l2_cache_ctl, uint16_t n_value, uint32_t d_value,
                                        uint64_t loop4_src_stride, bool smallc0_en);
extern void copy_gm_to_cbuf_multi_dn2nz(half* dst, half* src, uint8_t sid, uint64_t loop1_src_stride,
                                        uint8_t l2_cache_ctl, uint16_t n_value, uint32_t d_value,
                                        uint64_t loop4_src_stride, bool smallc0_en);
extern void set_mte2_nz_para(uint64_t para);

#define CAPTURE_GM_TO_L1_DEFINITION(type)                                                                              \
    void capture_copy_gm_to_cbuf_align_v2##type(__cbuf__ type* dst, __gm__ type* src, uint8_t sid, uint32_t block_count,     \
                                           uint32_t block_len, uint8_t left_padding_cnt, uint8_t right_padding_cnt,         \
                                           bool data_select_bit, uint8_t l2_cache_ctl, uint64_t src_stride,                 \
                                           uint32_t dst_stride);                                                        \
    void capture_copy_gm_to_cbuf_multi_nd_to_nz##type(__cbuf__ type* dst, __gm__ type* src, uint8_t sid,                       \
                                              uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,       \
                                              uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);           \
    void capture_copy_gm_to_cbuf_multi_dn_to_nz##type(__cbuf__ type* dst, __gm__ type* src, uint8_t sid,                       \
                                              uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,       \
                                              uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en);

CAPTURE_GM_TO_L1_DEFINITION(uint8_t);
CAPTURE_GM_TO_L1_DEFINITION(half);
CAPTURE_GM_TO_L1_DEFINITION(uint16_t);
CAPTURE_GM_TO_L1_DEFINITION(float);
CAPTURE_GM_TO_L1_DEFINITION(uint32_t);

void capture_set_mte2_nz_para(uint64_t para);

#define MOCKER_GM_TO_L1(type)                                                                                          \
    MOCKER(copy_gm_to_cbuf_align_v2, void (*)(__cbuf__ type*, __gm__ type*, uint8_t, uint32_t, uint32_t, uint8_t,      \
                                              uint8_t, bool, uint8_t, uint64_t, uint32_t))                             \
        .stubs()                                                                                                       \
        .will(invoke(capture_copy_gm_to_cbuf_align_v2##type));                                                              \
    MOCKER(copy_gm_to_cbuf_multi_nd2nz,                                                                                \
           void (*)(__cbuf__ type*, __gm__ type*, uint8_t, uint64_t, uint8_t, uint16_t, uint32_t, uint64_t, bool))     \
        .stubs()                                                                                                       \
        .will(invoke(capture_copy_gm_to_cbuf_multi_nd_to_nz##type));                                                           \
    MOCKER(copy_gm_to_cbuf_multi_dn2nz,                                                                                \
           void (*)(__cbuf__ type*, __gm__ type*, uint8_t, uint64_t, uint8_t, uint16_t, uint32_t, uint64_t, bool))     \
        .stubs()                                                                                                       \
        .will(invoke(capture_copy_gm_to_cbuf_multi_dn_to_nz##type))

void reset_capture();
void print_capture_data();
template <typename T>
void print_tensor(const T& src);

template <typename T, typename U>
void data_copy_gm_to_l1_sim(const T& dst, const U& src);
template <typename T, typename U, typename coord_type>
void data_copy_gm_to_l1_sim(const T& dst, const U& src, const coord_type& coord);

class tensor_api_gm_to_l1 : public testing::Test {
protected:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}

    virtual void SetUp()
    {
        reset_capture();
        MOCKER_GM_TO_L1(uint8_t);
        MOCKER_GM_TO_L1(half);
        MOCKER_GM_TO_L1(uint16_t);
        MOCKER_GM_TO_L1(float);
        MOCKER_GM_TO_L1(uint32_t);
        MOCKER(set_mte2_nz_para, void (*)(uint64_t)).stubs().will(invoke(capture_set_mte2_nz_para));

        tmp_core_type = g_coreType;
        g_coreType = AscendC::AIC_TYPE;
    }

    virtual void TearDown()
    {
        GlobalMockObject::verify();
        g_coreType = tmp_core_type;
    }

private:
    template <typename T>
    void initialize_data()
    {
        using cast_t = Std::conditional_t<sizeof(T) == 1, uint8_t, T>;
        using tt = Std::conditional_t<sizeof(T) == 2, uint16_t, cast_t>;
        std::iota(reinterpret_cast<tt*>(src0_gm), reinterpret_cast<tt*>(src0_gm + gm_size), static_cast<tt>(1));
        std::fill(reinterpret_cast<tt*>(l1a_buf), reinterpret_cast<tt*>(l1a_buf + l1_size), static_cast<tt>(1));
        std::fill(reinterpret_cast<tt*>(l1a_buf_golden), reinterpret_cast<tt*>(l1a_buf_golden + l1_size),
                  static_cast<tt>(1));
    }

private:
    constexpr static uint32_t gm_size = 64 * 1024;
    constexpr static uint32_t l1_size = 64 * 1024;
    __gm__ uint8_t src0_gm[gm_size] = {0};
    __cbuf__ uint8_t l1a_buf[l1_size] = {0};
    __cbuf__ uint8_t l1a_buf_golden[l1_size] = {0};

    int32_t tmp_core_type{0};
};

#define EXPECT_GM2L1_EQ()                                                                                              \
    bool result = std::equal(l1a_buf, l1a_buf + l1_size, l1a_buf_golden);                                                   \
    EXPECT_TRUE(result);                                                                                               \
    if (g_debug_print || !result) {                                                                                      \
        print_capture_data();                                                                                            \
        print_tensor(gm_a);                                                                                              \
        print_tensor(l1a_tensor);                                                                                        \
        print_tensor(l1a_tensor_golden);                                                                                  \
    }

#define TEST_GM2L1_CONCAT_IMPL_(a, b, c, d) a##_##b##_##c##_##index##_##d
#define TEST_GM2L1_CONCAT_(a, b, c, d) TEST_GM2L1_CONCAT_IMPL_(a, b, c, d)
#define TEST_GM2L1_INNER(type, name, gm_a_layout, l1a_layout, counter)                                                    \
    TEST_F(tensor_api_gm_to_l1, TEST_GM2L1_CONCAT_(copy_gm_to_l1_operation, name, type, counter))                                \
    {                                                                                                                  \
        using T = type;                                                                                                \
        auto gm_a = make_tensor(make_mem_ptr<location::gm>(reinterpret_cast<T*>(src0_gm)), gm_a_layout);                      \
        auto l1a_tensor = make_tensor(make_mem_ptr<location::l1>(reinterpret_cast<T*>(l1a_buf)), l1a_layout);                \
        auto l1a_tensor_golden = make_tensor(make_mem_ptr<location::l1>(reinterpret_cast<T*>(l1a_buf_golden)), l1a_layout);    \
        auto atom_copy = make_copy(copy_gm_to_l1{}, copy_gm_to_l1_trait_default{});                                                \
        initialize_data<T>();                                                                                           \
        atom_copy.call(l1a_tensor, gm_a);                                                                                 \
        data_copy_gm_to_l1_sim(l1a_tensor_golden, gm_a);                                                                        \
        EXPECT_GM2L1_EQ();                                                                                             \
    }
#define TEST_GM2L1_COORD_INNER(type, name, gm_a_layout, l1a_layout, make_coord, counter)                                   \
    TEST_F(tensor_api_gm_to_l1, TEST_GM2L1_CONCAT_(copy_gm_to_l1_operation_with_coord, name, type, counter))                       \
    {                                                                                                                  \
        using T = type;                                                                                                \
        auto gm_a = make_tensor(make_mem_ptr<location::gm>(reinterpret_cast<T*>(src0_gm)), gm_a_layout);                      \
        auto l1a_tensor = make_tensor(make_mem_ptr<location::l1>(reinterpret_cast<T*>(l1a_buf)), l1a_layout);                \
        auto l1a_tensor_golden = make_tensor(make_mem_ptr<location::l1>(reinterpret_cast<T*>(l1a_buf_golden)), l1a_layout);    \
        auto atom_copy = make_copy(copy_gm_to_l1{}, copy_gm_to_l1_trait_default{});                                                \
        initialize_data<T>();                                                                                           \
        auto coord = make_coord;                                                                                        \
        data_copy_gm_to_l1_sim(l1a_tensor_golden, gm_a, coord);                                                                 \
    }

#define TEST_GM2L1(type, name, gm_a_layout, l1a_layout) TEST_GM2L1_INNER(type, name, gm_a_layout, l1a_layout, __COUNTER__)

#define scalea_zz_to_zz(type, name, gm_a_layout, l1a_layout, make_coord)                                                  \
    TEST_GM2L1_COORD_INNER(type, name, gm_a_layout, l1a_layout, make_coord, __COUNTER__)

#define MAKE_LAYOUT_FUNC(name)                                                                                         \
    template <typename T>                                                                                              \
    constexpr auto make_##name = [](auto row, auto col) {                                                               \
        constexpr size_t c0_value = is_b4_type<T> ? 64 : 32 / sizeof(T);                                                       \
        return make_frame_layout<name##_layout_ptn, Int<c0_value>>(row, col);                                                   \
    };

MAKE_LAYOUT_FUNC(nd_ext)
MAKE_LAYOUT_FUNC(dn_ext)
MAKE_LAYOUT_FUNC(nd)
MAKE_LAYOUT_FUNC(dn)
MAKE_LAYOUT_FUNC(nz)
MAKE_LAYOUT_FUNC(zn)
#undef MAKE_LAYOUT_FUNC

#define MAKE_LAYOUT_FUNC(name)                                                                                         \
    template <typename T>                                                                                              \
    constexpr auto make_##name =                                                                                        \
        [](auto row, auto col) { return make_frame_layout<name##_layout_ptn, _2>(row, col); };

MAKE_LAYOUT_FUNC(zz)
MAKE_LAYOUT_FUNC(nn)
MAKE_LAYOUT_FUNC(scalea_nd)
MAKE_LAYOUT_FUNC(scalea_dn)
MAKE_LAYOUT_FUNC(scaleb_nd)
MAKE_LAYOUT_FUNC(scaleb_dn)
#undef MAKE_LAYOUT_FUNC

TEST_F(tensor_api_gm_to_l1, copy_gm_to_l1_region_scale_fractal_routes)
{
    using T = fp8_e8m0_t;
    auto gm_scale_a = make_tensor(
        make_mem_ptr<location::gm>(reinterpret_cast<T*>(src0_gm)), make_zz<T>(32, 32));
    auto l1_scale_a = make_tensor(
        make_mem_ptr<location::l1>(reinterpret_cast<T*>(l1a_buf)), make_zz<T>(32, 32));
    auto gm_scale_b = make_tensor(
        make_mem_ptr<location::gm>(reinterpret_cast<T*>(src0_gm)), make_nn<T>(32, 32));
    auto l1_scale_b = make_tensor(
        make_mem_ptr<location::l1>(reinterpret_cast<T*>(l1a_buf)), make_nn<T>(32, 32));
    auto atom_copy = make_copy(copy_gm_to_l1{}, copy_gm_to_l1_trait_default{});

    auto logical_coord = make_coord(0, 0);
    auto logical_shape = make_shape(16, 16);
    static_assert(decltype(gm_scale_a)::layout_type::depth == FOUR_DIM_DATA);
    static_assert(nesting_depth_v<decltype(logical_coord)> == TWO_DIM_DATA);
    static_assert(is_same_structure_v<decltype(logical_coord), decltype(logical_shape)>);
    copy(atom_copy, l1_scale_a, gm_scale_a, zero_coord, logical_coord, logical_shape);
    copy(atom_copy, l1_scale_b, gm_scale_b, zero_coord, logical_coord, logical_shape);

    auto physical_coord = make_coord(make_coord(0, 0), make_coord(0, 0));
    auto physical_shape = gm_scale_a.shape();
    static_assert(nesting_depth_v<decltype(physical_coord)> == FOUR_DIM_DATA);
    static_assert(is_same_structure_v<decltype(physical_coord), decltype(physical_shape)>);
    copy(atom_copy, l1_scale_a, gm_scale_a, zero_coord, physical_coord, physical_shape);
}

// ============================ Batch (multi-matrix) gm->l1 copy ============================
// Batched frame layouts are built with the official three-arg make_frame_layout<Ptn, trait_type>(batch_value, m_value, k_size)
// (see commit d2d3dc8): shape_type = (batch_value, frame_shape), stride = (m_value * k_size, frame_stride). Only BMK-contiguous
// layouts are supported, so m_value/k_size must be alignment-friendly (m_value%16==0, k_size%c0==0) to make the per-batch
// stride m_value*k_size equal to one aligned nz/zn matrix's footprint in l1.
#define MAKE_BATCH_LAYOUT_FUNC(name)                                                                                   \
    template <typename T>                                                                                             \
    constexpr auto make_batch_##name = [](auto batch, auto row, auto col) {                                            \
        constexpr size_t c0_value = is_b4_type<T> ? 64 : 32 / sizeof(T);                                                      \
        return make_frame_layout<name##_layout_ptn, Int<c0_value>>(batch, row, col);                                            \
    };

MAKE_BATCH_LAYOUT_FUNC(nd_ext)
MAKE_BATCH_LAYOUT_FUNC(dn_ext)
MAKE_BATCH_LAYOUT_FUNC(nz)
MAKE_BATCH_LAYOUT_FUNC(zn)
#undef MAKE_BATCH_LAYOUT_FUNC

// Builds batched src(gm)/dst(l1) tensors with the official batch make_frame_layout and runs the real
// copy, then verifies against a golden computed per batch by reusing the validated single-matrix
// data_copy_gm_to_l1_sim on offset views. Per-batch element stride is m_value*k_size for both src and dst (BMK).
#define TEST_GM2L1_BATCH_INNER(type, name, gm_ptn, l1_ptn, batch, m_value, k_size, counter)                                        \
    TEST_F(tensor_api_gm_to_l1, TEST_GM2L1_CONCAT_(copy_gm_to_l1_batch, name, type, counter))                                   \
    {                                                                                                                 \
        using T = type;                                                                                               \
        constexpr int k_batch = (batch);                                                                               \
        constexpr int k_src_batch_start = k_batch > 1 ? 1 : 0;                                                           \
        constexpr int k_copied_batch = k_batch - k_src_batch_start;                                                        \
        const int64_t k_mat_stride = static_cast<int64_t>(m_value) * (k_size);                                                     \
        auto gm_batched = make_batch_##gm_ptn<T>(k_batch, (m_value), (k_size));                                                       \
        auto l1_batched = make_batch_##l1_ptn<T>(k_batch, (m_value), (k_size));                                                       \
        auto gm_a = make_tensor(make_mem_ptr<location::gm>(reinterpret_cast<T*>(src0_gm)), gm_batched);                     \
        auto l1a_tensor = make_tensor(make_mem_ptr<location::l1>(reinterpret_cast<T*>(l1a_buf)), l1_batched);               \
        auto atom_copy = make_copy(copy_gm_to_l1{}, copy_gm_to_l1_trait_default{});                                               \
        initialize_data<T>();                                                                                          \
        if constexpr (k_batch > 1) {                                                                                   \
            auto logical_batch_coord = make_coord(1, make_coord(0, 0));                                                   \
            auto logical_batch_shape = make_shape(k_copied_batch, make_shape((m_value), (k_size)));                                   \
            static_assert(decltype(gm_a)::layout_type::depth == FIVE_DIM_DATA);                                         \
            static_assert(nesting_depth_v<decltype(logical_batch_coord)> == THREE_DIM_DATA);                              \
            static_assert(is_same_structure_v<decltype(logical_batch_coord), decltype(logical_batch_shape)>);                \
            copy(atom_copy, l1a_tensor, gm_a, zero_coord, logical_batch_coord, logical_batch_shape);                          \
        } else {                                                                                                      \
            atom_copy.call(l1a_tensor, gm_a);                                                                            \
        }                                                                                                             \
        for (int b = 0; b < k_copied_batch; ++b) {                                                                      \
            auto gm_b = make_tensor(                                                                                    \
                make_mem_ptr<location::gm>(reinterpret_cast<T*>(src0_gm) + (b + k_src_batch_start) * k_mat_stride),           \
                make_##gm_ptn<T>((m_value), (k_size)));                                                                            \
            auto l1G = make_tensor(                                                                                    \
                make_mem_ptr<location::l1>(reinterpret_cast<T*>(l1a_buf_golden) + b * k_mat_stride),                        \
                make_##l1_ptn<T>((m_value), (k_size)));                                                                            \
            data_copy_gm_to_l1_sim(l1G, gm_b);                                                                               \
        }                                                                                                             \
        bool result = std::equal(l1a_buf, l1a_buf + l1_size, l1a_buf_golden);                                             \
        EXPECT_TRUE(result);                                                                                          \
        if (g_debug_print || !result) {                                                                                 \
            print_capture_data();                                                                                       \
            for (int b = 0; b < k_copied_batch; ++b) {                                                                 \
                std::cout << "==== dst batch " << b << ", src batch " << b + k_src_batch_start << " ====" << std::endl;  \
                auto gm_b = make_tensor(                                                                                \
                    make_mem_ptr<location::gm>(                                                                         \
                        reinterpret_cast<T*>(src0_gm) + (b + k_src_batch_start) * k_mat_stride),                            \
                    make_##gm_ptn<T>((m_value), (k_size)));                                                                        \
                auto l1B = make_tensor(                                                                                \
                    make_mem_ptr<location::l1>(reinterpret_cast<T*>(l1a_buf) + b * k_mat_stride), make_##l1_ptn<T>((m_value), (k_size)));\
                auto l1G = make_tensor(                                                                                \
                    make_mem_ptr<location::l1>(reinterpret_cast<T*>(l1a_buf_golden) + b * k_mat_stride),                    \
                    make_##l1_ptn<T>((m_value), (k_size)));                                                                        \
                print_tensor(gm_b);                                                                                    \
                print_tensor(l1B);                                                                                    \
                print_tensor(l1G);                                                                                    \
            }                                                                                                         \
        }                                                                                                             \
    }
#define TEST_GM2L1_BATCH(type, name, gm_ptn, l1_ptn, batch, m_value, k_size)                                                       \
    TEST_GM2L1_BATCH_INNER(type, name, gm_ptn, l1_ptn, batch, m_value, k_size, __COUNTER__)

// nd -> nz batched. Aligned m_value/k_size so the per-batch stride m_value*k_size equals one nz matrix footprint in l1.
TEST_GM2L1_BATCH(half, nd_to_nz, nd_ext, nz, 1, 32, 64)
TEST_GM2L1_BATCH(half, nd_to_nz, nd_ext, nz, 3, 32, 64)
TEST_GM2L1_BATCH(uint8_t, nd_to_nz, nd_ext, nz, 4, 32, 64)
TEST_GM2L1_BATCH(uint32_t, nd_to_nz, nd_ext, nz, 2, 32, 64)

// dn -> nz batched.
TEST_GM2L1_BATCH(half, dn_to_nz, dn_ext, nz, 3, 32, 64)
TEST_GM2L1_BATCH(uint32_t, dn_to_nz, dn_ext, nz, 2, 32, 64)

// nd -> zn batched.
TEST_GM2L1_BATCH(half, nd_to_zn, nd_ext, zn, 3, 32, 64)
TEST_GM2L1_BATCH(uint32_t, nd_to_zn, nd_ext, zn, 2, 32, 64)

// dn -> zn batched.
TEST_GM2L1_BATCH(half, dn_to_zn, dn_ext, zn, 3, 32, 64)
TEST_GM2L1_BATCH(uint32_t, dn_to_zn, dn_ext, zn, 2, 32, 64)

// nz -> nz batched.
TEST_GM2L1_BATCH(half, nz_to_nz, nz, nz, 3, 32, 64)

// zn -> zn batched.
TEST_GM2L1_BATCH(half, zn_to_zn, zn, zn, 3, 32, 64)

// NC1HWC0 (n_value, c1, height, width, c0_value) flat 5D layout, contiguous. Built with the nc1hwc0_layout_ptn tag so
// copy_gm_to_l1_routing dispatches to copy_gm_to_l1_nc1hwc02nc1hwc0.
template <typename T, size_t c0_value>
auto make_nc1hwc0(int n, int c1, int h, int w)
{
    return make_frame_layout<nc1hwc0_layout_ptn>(n, c1, h, w, static_cast<int>(c0_value));
}

// NC1HWC0 gm->l1 copy, width fully loaded: c1 bursts of height*width*c0_value, golden per-c1 block.
#define TEST_GM2L1_NC1HWC0_INNER(type, n_value, c1, height, width, counter)                                                          \
    TEST_F(tensor_api_gm_to_l1, TEST_GM2L1_CONCAT_(copy_gm_to_l1_nc1_hwc0, nd_to_nd, type, counter))                                \
    {                                                                                                                 \
        using T = type;                                                                                               \
        constexpr size_t c0_value = is_b4_type<T> ? 64 : 32 / sizeof(T);                                                      \
        const int k_block_elems = (height) * (width) * c0_value; /* per-c1 block = height*width*c0_value */                                           \
        auto gm_nc1hwc0 = make_nc1hwc0<T, c0_value>((n_value), (c1), (height), (width));                                                     \
        auto l1_nc1hwc0 = make_nc1hwc0<T, c0_value>((n_value), (c1), (height), (width));                                                     \
        auto gm_a = make_tensor(make_mem_ptr<location::gm>(reinterpret_cast<T*>(src0_gm)), gm_nc1hwc0);                     \
        auto l1a_tensor = make_tensor(make_mem_ptr<location::l1>(reinterpret_cast<T*>(l1a_buf)), l1_nc1hwc0);               \
        auto atom_copy = make_copy(copy_gm_to_l1{}, copy_gm_to_l1_trait_default{});                                               \
        initialize_data<T>();                                                                                          \
        atom_copy.call(l1a_tensor, gm_a);                                                                                \
        copy(atom_copy, l1a_tensor, gm_a, zero_coord, make_coord(0, 0, 0, 0, 0),                                                     \
            make_shape((n_value), (c1), (height), (width), c0_value));                                                                      \
        for (int b = 0; b < (n_value) * (c1); ++b) {                                                                        \
            auto gm_b = make_tensor(                                                                                    \
                make_mem_ptr<location::gm>(reinterpret_cast<T*>(src0_gm) + b * k_block_elems), make_nd<T>(1, k_block_elems)); \
            auto l1G = make_tensor(                                                                                    \
                make_mem_ptr<location::l1>(reinterpret_cast<T*>(l1a_buf_golden) + b * k_block_elems),                       \
                make_nd<T>(1, k_block_elems));                                                                           \
            data_copy_gm_to_l1_sim(l1G, gm_b);                                                                               \
        }                                                                                                             \
        EXPECT_TRUE(std::equal(l1a_buf, l1a_buf + l1_size, l1a_buf_golden));                                               \
    }
#define TEST_GM2L1_NC1HWC0(type, n_value, c1, height, width) TEST_GM2L1_NC1HWC0_INNER(type, n_value, c1, height, width, __COUNTER__)

TEST_GM2L1_NC1HWC0(half, 1, 3, 4, 4)
TEST_GM2L1_NC1HWC0(half, 1, 1, 8, 2)
TEST_GM2L1_NC1HWC0(uint32_t, 1, 2, 4, 4)

// NC1HWC0 gm->l1 copy, width not fully loaded: gm built with src_w, sliced to load_w; per-(c1,height) block.
#define TEST_GM2L1_NC1HWC0_PARTW_INNER(type, n_value, c1, height, src_w, load_w, counter)                                        \
    TEST_F(tensor_api_gm_to_l1, TEST_GM2L1_CONCAT_(copy_gm_to_l1_nc1_hwc0_part_w, nd_to_nd, type, counter))                         \
    {                                                                                                               \
        using T = type;                                                                                             \
        constexpr size_t c0_value = is_b4_type<T> ? 64 : 32 / sizeof(T);                                                    \
        const int k_src_row = (src_w) * c0_value;                                                                            \
        const int k_dst_row = (load_w) * c0_value;                                                                           \
        auto gm_full = make_nc1hwc0<T, c0_value>((n_value), (c1), (height), (src_w));                                                   \
        auto l1_nc1hwc0 = make_nc1hwc0<T, c0_value>((n_value), (c1), (height), (load_w));                                               \
        auto gm_full_t = make_tensor(make_mem_ptr<location::gm>(reinterpret_cast<T*>(src0_gm)), gm_full);                  \
        auto gm_a = gm_full_t.slice(make_coord(0, 0, 0, 0, 0), make_shape((n_value), (c1), (height), (load_w), c0_value));                 \
        auto l1a_tensor = make_tensor(make_mem_ptr<location::l1>(reinterpret_cast<T*>(l1a_buf)), l1_nc1hwc0);             \
        auto atom_copy = make_copy(copy_gm_to_l1{}, copy_gm_to_l1_trait_default{});                                             \
        initialize_data<T>();                                                                                        \
        atom_copy.call(l1a_tensor, gm_a);                                                                              \
        for (int b = 0; b < (n_value) * (c1) * (height); ++b) {                                                                \
            auto gm_b = make_tensor(                                                                                  \
                make_mem_ptr<location::gm>(reinterpret_cast<T*>(src0_gm) + b * k_src_row), make_nd<T>(1, k_dst_row));       \
            auto l1G = make_tensor(                                                                                  \
                make_mem_ptr<location::l1>(reinterpret_cast<T*>(l1a_buf_golden) + b * k_dst_row), make_nd<T>(1, k_dst_row)); \
            data_copy_gm_to_l1_sim(l1G, gm_b);                                                                             \
        }                                                                                                           \
        EXPECT_TRUE(std::equal(l1a_buf, l1a_buf + l1_size, l1a_buf_golden));                                             \
    }
#define TEST_GM2L1_NC1HWC0_PARTW(type, n_value, c1, height, src_w, load_w) \
    TEST_GM2L1_NC1HWC0_PARTW_INNER(type, n_value, c1, height, src_w, load_w, __COUNTER__)

TEST_GM2L1_NC1HWC0_PARTW(half, 1, 3, 4, 8, 4)
TEST_GM2L1_NC1HWC0_PARTW(half, 1, 2, 4, 6, 2)
TEST_GM2L1_NC1HWC0_PARTW(uint32_t, 1, 2, 4, 8, 4)

// nd_to_nd
// constraint: col small to big: dst column stride % 32B = 0, col same: no constraint
// constraint: or support src shape is 1 dim(include 2d continuous, src stride equals to dst stride)
// test various data types, b4 to b64
TEST_GM2L1(fp4x2_e2m1_t, nd_to_nd, make_nd_ext<T>(17, 18), make_nd_ext<T>(19, 64))
TEST_GM2L1(fp4x2_e1m2_t, nd_to_nd, make_nd_ext<T>(17, 18), make_nd_ext<T>(19, 64))
TEST_GM2L1(half, nd_to_nd, make_nd_ext<T>(17, 18), make_nd_ext<T>(19, 32))
TEST_GM2L1(float, nd_to_nd, make_nd_ext<T>(17, 18), make_nd_ext<T>(19, 32))
TEST_GM2L1(int8_t, nd_to_nd, make_nd_ext<T>(17, 18), make_nd_ext<T>(19, 32))
TEST_GM2L1(uint8_t, nd_to_nd, make_nd_ext<T>(17, 18), make_nd_ext<T>(19, 32))
TEST_GM2L1(int16_t, nd_to_nd, make_nd_ext<T>(17, 18), make_nd_ext<T>(19, 32))
TEST_GM2L1(uint16_t, nd_to_nd, make_nd_ext<T>(17, 18), make_nd_ext<T>(19, 32))
TEST_GM2L1(int32_t, nd_to_nd, make_nd_ext<T>(17, 18), make_nd_ext<T>(19, 32))
TEST_GM2L1(uint32_t, nd_to_nd, make_nd_ext<T>(17, 18), make_nd_ext<T>(19, 32))
TEST_GM2L1(int64_t, nd_to_nd, make_nd_ext<T>(17, 18), make_nd_ext<T>(19, 32))
TEST_GM2L1(uint64_t, nd_to_nd, make_nd_ext<T>(17, 18), make_nd_ext<T>(19, 32))
TEST_GM2L1(fp8_e4m3fn_t, nd_to_nd, make_nd_ext<T>(17, 18), make_nd_ext<T>(19, 32))
TEST_GM2L1(fp8_e5m2_t, nd_to_nd, make_nd_ext<T>(17, 18), make_nd_ext<T>(19, 32))
TEST_GM2L1(hifloat8_t, nd_to_nd, make_nd_ext<T>(17, 18), make_nd_ext<T>(19, 32))

// make_nd
TEST_GM2L1(fp4x2_e2m1_t, nd_layout_to_nd_layout, make_nd<T>(17, 18), make_nd<T>(19, 64))
TEST_GM2L1(fp4x2_e1m2_t, nd_layout_to_nd_layout, make_nd<T>(17, 18), make_nd<T>(19, 64))
TEST_GM2L1(half, nd_layout_to_nd_layout, make_nd<T>(17, 18), make_nd<T>(19, 32))
TEST_GM2L1(float, nd_layout_to_nd_layout, make_nd<T>(17, 18), make_nd<T>(19, 32))
TEST_GM2L1(uint8_t, nd_layout_to_nd_layout, make_nd<T>(17, 18), make_nd<T>(19, 32))
TEST_GM2L1(uint16_t, nd_layout_to_nd_layout, make_nd<T>(17, 18), make_nd<T>(19, 32))
TEST_GM2L1(uint32_t, nd_layout_to_nd_layout, make_nd<T>(17, 18), make_nd<T>(19, 32))
TEST_GM2L1(uint64_t, nd_layout_to_nd_layout, make_nd<T>(17, 18), make_nd<T>(19, 32))

// continuous case
// 1 dim case. src/dst col or row shape is 1
TEST_GM2L1(fp4x2_e2m1_t, nd_to_nd_1_dim, make_nd_ext<T>(1, 34), make_nd_ext<T>(1, 34))
TEST_GM2L1(fp4x2_e1m2_t, nd_to_nd_1_dim, make_nd_ext<T>(1, 34), make_nd_ext<T>(1, 34))
TEST_GM2L1(uint8_t, nd_to_nd_1_dim, make_nd_ext<T>(1, 17), make_nd_ext<T>(1, 19))
TEST_GM2L1(uint8_t, nd_to_nd_1_dim, make_nd_ext<T>(1, 17), make_nd_ext<T>(1, 40))
TEST_GM2L1(half, nd_to_nd_1_dim, make_nd_ext<T>(1, 17), make_nd_ext<T>(1, 19))
TEST_GM2L1(half, nd_to_nd_1_dim, make_nd_ext<T>(1, 17), make_nd_ext<T>(1, 40))
TEST_GM2L1(float, nd_to_nd_1_dim, make_nd_ext<T>(1, 17), make_nd_ext<T>(1, 19))
TEST_GM2L1(float, nd_to_nd_1_dim, make_nd_ext<T>(1, 17), make_nd_ext<T>(1, 40))
TEST_GM2L1(uint64_t, nd_to_nd_1_dim, make_nd_ext<T>(1, 17), make_nd_ext<T>(1, 19))
TEST_GM2L1(uint64_t, nd_to_nd_1_dim, make_nd_ext<T>(1, 17), make_nd_ext<T>(1, 40))
TEST_GM2L1(uint8_t, nd_to_nd_1_dim, make_nd_ext<T>(17, 1), make_nd_ext<T>(19, 1))
TEST_GM2L1(uint8_t, nd_to_nd_1_dim, make_nd_ext<T>(17, 1), make_nd_ext<T>(40, 1))
TEST_GM2L1(half, nd_to_nd_1_dim, make_nd_ext<T>(17, 1), make_nd_ext<T>(19, 1))
TEST_GM2L1(half, nd_to_nd_1_dim, make_nd_ext<T>(17, 1), make_nd_ext<T>(40, 1))
TEST_GM2L1(float, nd_to_nd_1_dim, make_nd_ext<T>(17, 1), make_nd_ext<T>(19, 1))
TEST_GM2L1(float, nd_to_nd_1_dim, make_nd_ext<T>(17, 1), make_nd_ext<T>(40, 1))
TEST_GM2L1(uint64_t, nd_to_nd_1_dim, make_nd_ext<T>(17, 1), make_nd_ext<T>(19, 1))
TEST_GM2L1(uint64_t, nd_to_nd_1_dim, make_nd_ext<T>(17, 1), make_nd_ext<T>(40, 1))

// 2d continuous case, src stride equals to dst stride
TEST_GM2L1(fp4x2_e2m1_t, nd_to_nd_1_dim, make_nd_ext<T>(10, 34), make_nd_ext<T>(10, 34))
TEST_GM2L1(fp4x2_e2m1_t, nd_to_nd_1_dim, make_nd_ext<T>(10, 34), make_nd_ext<T>(20, 34))
TEST_GM2L1(uint8_t, nd_to_nd_1_dim, make_nd_ext<T>(10, 17), make_nd_ext<T>(10, 17))
TEST_GM2L1(uint8_t, nd_to_nd_1_dim, make_nd_ext<T>(10, 17), make_nd_ext<T>(20, 17))
TEST_GM2L1(half, nd_to_nd_1_dim, make_nd_ext<T>(10, 17), make_nd_ext<T>(10, 17))
TEST_GM2L1(half, nd_to_nd_1_dim, make_nd_ext<T>(10, 17), make_nd_ext<T>(20, 17))
TEST_GM2L1(float, nd_to_nd_1_dim, make_nd_ext<T>(10, 17), make_nd_ext<T>(10, 17))
TEST_GM2L1(float, nd_to_nd_1_dim, make_nd_ext<T>(10, 17), make_nd_ext<T>(20, 17))
TEST_GM2L1(uint64_t, nd_to_nd_1_dim, make_nd_ext<T>(10, 17), make_nd_ext<T>(10, 17))
TEST_GM2L1(uint64_t, nd_to_nd_1_dim, make_nd_ext<T>(10, 17), make_nd_ext<T>(20, 17))
TEST_GM2L1(uint8_t, nd_to_nd_1_dim, make_nd_ext<T>(10, 30), make_nd_ext<T>(10, 30))
TEST_GM2L1(uint8_t, nd_to_nd_1_dim, make_nd_ext<T>(10, 30), make_nd_ext<T>(35, 30))
TEST_GM2L1(half, nd_to_nd_1_dim, make_nd_ext<T>(10, 30), make_nd_ext<T>(10, 30))
TEST_GM2L1(half, nd_to_nd_1_dim, make_nd_ext<T>(10, 30), make_nd_ext<T>(35, 30))
TEST_GM2L1(float, nd_to_nd_1_dim, make_nd_ext<T>(10, 30), make_nd_ext<T>(10, 30))
TEST_GM2L1(float, nd_to_nd_1_dim, make_nd_ext<T>(10, 30), make_nd_ext<T>(35, 30))
TEST_GM2L1(uint64_t, nd_to_nd_1_dim, make_nd_ext<T>(10, 30), make_nd_ext<T>(10, 30))
TEST_GM2L1(uint64_t, nd_to_nd_1_dim, make_nd_ext<T>(10, 30), make_nd_ext<T>(35, 30))

// src shape include Int<1>()
TEST_GM2L1(fp4x2_e2m1_t, nd_to_nd_1_dim_int, make_nd_ext<T>(Std::Int<1>(), 34), make_nd_ext<T>(1, 38))
TEST_GM2L1(fp4x2_e1m2_t, nd_to_nd_1_dim_int, make_nd_ext<T>(Std::Int<1>(), 34), make_nd_ext<T>(1, 38))
TEST_GM2L1(half, nd_to_nd_1_dim_int, make_nd_ext<T>(Std::Int<1>(), 17), make_nd_ext<T>(1, 19))
TEST_GM2L1(bfloat16_t, nd_to_nd_1_dim_int, make_nd_ext<T>(Std::Int<1>(), 17), make_nd_ext<T>(1, 19))
TEST_GM2L1(float, nd_to_nd_1_dim_int, make_nd_ext<T>(Std::Int<1>(), 17), make_nd_ext<T>(1, 19))
TEST_GM2L1(int8_t, nd_to_nd_1_dim_int, make_nd_ext<T>(Std::Int<1>(), 17), make_nd_ext<T>(1, 19))
TEST_GM2L1(uint8_t, nd_to_nd_1_dim_int, make_nd_ext<T>(Std::Int<1>(), 17), make_nd_ext<T>(1, 19))
TEST_GM2L1(int16_t, nd_to_nd_1_dim_int, make_nd_ext<T>(Std::Int<1>(), 17), make_nd_ext<T>(1, 19))
TEST_GM2L1(uint16_t, nd_to_nd_1_dim_int, make_nd_ext<T>(Std::Int<1>(), 17), make_nd_ext<T>(1, 19))
TEST_GM2L1(int32_t, nd_to_nd_1_dim_int, make_nd_ext<T>(Std::Int<1>(), 17), make_nd_ext<T>(1, 19))
TEST_GM2L1(uint32_t, nd_to_nd_1_dim_int, make_nd_ext<T>(Std::Int<1>(), 17), make_nd_ext<T>(1, 19))
TEST_GM2L1(int64_t, nd_to_nd_1_dim_int, make_nd_ext<T>(Std::Int<1>(), 17), make_nd_ext<T>(1, 19))
TEST_GM2L1(uint64_t, nd_to_nd_1_dim_int, make_nd_ext<T>(Std::Int<1>(), 17), make_nd_ext<T>(1, 19))
TEST_GM2L1(fp8_e4m3fn_t, nd_to_nd_1_dim_int, make_nd_ext<T>(Std::Int<1>(), 17), make_nd_ext<T>(1, 19))
TEST_GM2L1(fp8_e5m2_t, nd_to_nd_1_dim_int, make_nd_ext<T>(Std::Int<1>(), 17), make_nd_ext<T>(1, 19))
TEST_GM2L1(hifloat8_t, nd_to_nd_1_dim_int, make_nd_ext<T>(Std::Int<1>(), 17), make_nd_ext<T>(1, 19))

TEST_GM2L1(fp4x2_e2m1_t, nd_to_nd_1_dim_int, make_nd_ext<T>(34, Std::Int<1>()), make_nd_ext<T>(38, 1))
TEST_GM2L1(fp4x2_e2m1_t, nd_to_nd_1_dim_int, make_nd_ext<T>(34, Std::Int<1>()), make_nd_ext<T>(38, Std::Int<1>()))
TEST_GM2L1(uint8_t, nd_to_nd_1_dim_int, make_nd_ext<T>(17, Std::Int<1>()), make_nd_ext<T>(19, 1))
TEST_GM2L1(uint8_t, nd_to_nd_1_dim_int, make_nd_ext<T>(17, Std::Int<1>()), make_nd_ext<T>(19, Std::Int<1>()))
TEST_GM2L1(uint16_t, nd_to_nd_1_dim_int, make_nd_ext<T>(17, Std::Int<1>()), make_nd_ext<T>(19, 1))
TEST_GM2L1(uint16_t, nd_to_nd_1_dim_int, make_nd_ext<T>(17, Std::Int<1>()), make_nd_ext<T>(19, Std::Int<1>()))
TEST_GM2L1(float, nd_to_nd_1_dim_int, make_nd_ext<T>(17, Std::Int<1>()), make_nd_ext<T>(19, 1))
TEST_GM2L1(float, nd_to_nd_1_dim_int, make_nd_ext<T>(17, Std::Int<1>()), make_nd_ext<T>(19, Std::Int<1>()))
TEST_GM2L1(uint64_t, nd_to_nd_1_dim_int, make_nd_ext<T>(17, Std::Int<1>()), make_nd_ext<T>(19, 1))
TEST_GM2L1(uint64_t, nd_to_nd_1_dim_int, make_nd_ext<T>(17, Std::Int<1>()), make_nd_ext<T>(19, Std::Int<1>()))

// non continuous case, the dst col stride of nd layout needs to be aligned with C0_SIZE(32B)
scalea_zz_to_zz(fp4x2_e2m1_t, nd_to_nd, make_nd_ext<T>(33, 40), make_nd_ext<T>(19, 64), make_coord(10, 10))
scalea_zz_to_zz(uint8_t, nd_to_nd, make_nd_ext<T>(33, 40), make_nd_ext<T>(19, 32), make_coord(10, 10))
scalea_zz_to_zz(uint16_t, nd_to_nd, make_nd_ext<T>(33, 40), make_nd_ext<T>(19, 16), make_coord(10, 10))
scalea_zz_to_zz(uint32_t, nd_to_nd, make_nd_ext<T>(33, 40), make_nd_ext<T>(19, 8), make_coord(10, 10))
scalea_zz_to_zz(fp4x2_e2m1_t, nd_to_nd, make_nd_ext<T>(33, 50), make_nd_ext<T>(19, 32), make_coord(10, 10))
scalea_zz_to_zz(uint8_t, nd_to_nd, make_nd_ext<T>(33, 40), make_nd_ext<T>(19, 32), make_coord(10, 10))
scalea_zz_to_zz(uint16_t, nd_to_nd, make_nd_ext<T>(33, 40), make_nd_ext<T>(19, 32), make_coord(10, 10))
scalea_zz_to_zz(uint32_t, nd_to_nd, make_nd_ext<T>(33, 40), make_nd_ext<T>(19, 32), make_coord(10, 10))
scalea_zz_to_zz(fp4x2_e2m1_t, nd_to_nd, make_nd_ext<T>(33, 26), make_nd_ext<T>(19, 64), make_coord(10, 10))
scalea_zz_to_zz(uint8_t, nd_to_nd, make_nd_ext<T>(33, 25), make_nd_ext<T>(19, 32), make_coord(10, 10))
scalea_zz_to_zz(uint16_t, nd_to_nd, make_nd_ext<T>(33, 25), make_nd_ext<T>(19, 32), make_coord(10, 10))
scalea_zz_to_zz(uint32_t, nd_to_nd, make_nd_ext<T>(33, 25), make_nd_ext<T>(19, 32), make_coord(10, 10))
scalea_zz_to_zz(fp4x2_e2m1_t, nd_to_nd, make_nd_ext<T>(33, 40), make_nd_ext<T>(19, 64), make_coord(16, 16))
scalea_zz_to_zz(uint8_t, nd_to_nd, make_nd_ext<T>(33, 40), make_nd_ext<T>(19, 32), make_coord(16, 16))
scalea_zz_to_zz(uint16_t, nd_to_nd, make_nd_ext<T>(33, 40), make_nd_ext<T>(19, 32), make_coord(16, 16))
scalea_zz_to_zz(uint32_t, nd_to_nd, make_nd_ext<T>(33, 40), make_nd_ext<T>(19, 32), make_coord(16, 16))
scalea_zz_to_zz(fp4x2_e2m1_t, nd_to_nd, make_nd_ext<T>(33, 26), make_nd_ext<T>(19, 64), make_coord(16, 16))
scalea_zz_to_zz(uint8_t, nd_to_nd, make_nd_ext<T>(33, 25), make_nd_ext<T>(19, 32), make_coord(16, 16))
scalea_zz_to_zz(uint16_t, nd_to_nd, make_nd_ext<T>(33, 25), make_nd_ext<T>(19, 32), make_coord(16, 16))
scalea_zz_to_zz(uint32_t, nd_to_nd, make_nd_ext<T>(33, 25), make_nd_ext<T>(19, 32), make_coord(16, 16))

// Additional nd_to_nd test cases with various shapes and coords
scalea_zz_to_zz(half, nd_to_nd, make_nd_ext<T>(17, 32), make_nd_ext<T>(16, 32), make_coord(0, 0))
scalea_zz_to_zz(half, nd_to_nd, make_nd_ext<T>(17, 32), make_nd_ext<T>(16, 32), make_coord(1, 0))
scalea_zz_to_zz(half, nd_to_nd, make_nd_ext<T>(17, 32), make_nd_ext<T>(16, 32), make_coord(0, 16))
scalea_zz_to_zz(float, nd_to_nd, make_nd_ext<T>(16, 16), make_nd_ext<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(float, nd_to_nd, make_nd_ext<T>(16, 16), make_nd_ext<T>(16, 16), make_coord(8, 8))
scalea_zz_to_zz(fp4x2_e2m1_t, nd_to_nd, make_nd_ext<T>(65, 66), make_nd_ext<T>(64, 64), make_coord(1, 2))
scalea_zz_to_zz(uint8_t, nd_to_nd, make_nd_ext<T>(65, 65), make_nd_ext<T>(64, 64), make_coord(1, 1))
scalea_zz_to_zz(uint16_t, nd_to_nd, make_nd_ext<T>(33, 33), make_nd_ext<T>(32, 32), make_coord(1, 1))
scalea_zz_to_zz(uint32_t, nd_to_nd, make_nd_ext<T>(17, 17), make_nd_ext<T>(16, 16), make_coord(1, 1))
scalea_zz_to_zz(half, nd_to_nd, make_nd_ext<T>(100, 100), make_nd_ext<T>(64, 64), make_coord(18, 18))
scalea_zz_to_zz(float, nd_to_nd, make_nd_ext<T>(50, 50), make_nd_ext<T>(32, 32), make_coord(10, 10))
scalea_zz_to_zz(uint64_t, nd_to_nd, make_nd_ext<T>(17, 16), make_nd_ext<T>(16, 16), make_coord(1, 0))
scalea_zz_to_zz(uint64_t, nd_to_nd, make_nd_ext<T>(16, 17), make_nd_ext<T>(16, 16), make_coord(0, 1))
scalea_zz_to_zz(half, nd_to_nd, make_nd_ext<T>(8, 64), make_nd_ext<T>(8, 32), make_coord(0, 16))
scalea_zz_to_zz(float, nd_to_nd, make_nd_ext<T>(64, 8), make_nd_ext<T>(32, 8), make_coord(16, 0))
scalea_zz_to_zz(fp4x2_e2m1_t, nd_to_nd, make_nd_ext<T>(32, 32), make_nd_ext<T>(16, 64), make_coord(8, 8))
scalea_zz_to_zz(uint8_t, nd_to_nd, make_nd_ext<T>(32, 32), make_nd_ext<T>(16, 16), make_coord(8, 8))
scalea_zz_to_zz(uint16_t, nd_to_nd, make_nd_ext<T>(32, 32), make_nd_ext<T>(16, 16), make_coord(8, 8))
scalea_zz_to_zz(uint16_t, nd_to_nd, make_nd_ext<T>(48, 48), make_nd_ext<T>(32, 32), make_coord(8, 8))
scalea_zz_to_zz(uint32_t, nd_to_nd, make_nd_ext<T>(24, 24), make_nd_ext<T>(16, 16), make_coord(4, 4))
scalea_zz_to_zz(fp4x2_e2m1_t, nd_to_nd, make_nd_ext<T>(33, 32), make_nd_ext<T>(16, 32), make_coord(8, 0))
scalea_zz_to_zz(int8_t, nd_to_nd, make_nd_ext<T>(33, 32), make_nd_ext<T>(16, 32), make_coord(8, 0))
scalea_zz_to_zz(int16_t, nd_to_nd, make_nd_ext<T>(32, 33), make_nd_ext<T>(32, 16), make_coord(0, 8))
scalea_zz_to_zz(int32_t, nd_to_nd, make_nd_ext<T>(40, 40), make_nd_ext<T>(32, 32), make_coord(4, 4))
scalea_zz_to_zz(int64_t, nd_to_nd, make_nd_ext<T>(20, 20), make_nd_ext<T>(16, 16), make_coord(2, 2))

scalea_zz_to_zz(half, nd_to_nd, make_nd_ext<T>(17, 32), make_nd_ext<T>(15, 15), make_coord(0, 0))
scalea_zz_to_zz(half, nd_to_nd, make_nd_ext<T>(17, 32), make_nd_ext<T>(15, 15), make_coord(1, 0))
scalea_zz_to_zz(half, nd_to_nd, make_nd_ext<T>(17, 32), make_nd_ext<T>(15, 15), make_coord(0, 16))
scalea_zz_to_zz(float, nd_to_nd, make_nd_ext<T>(16, 16), make_nd_ext<T>(15, 15), make_coord(0, 0))
scalea_zz_to_zz(float, nd_to_nd, make_nd_ext<T>(16, 16), make_nd_ext<T>(15, 7), make_coord(8, 8))
scalea_zz_to_zz(fp4x2_e2m1_t, nd_to_nd, make_nd_ext<T>(65, 66), make_nd_ext<T>(15, 14), make_coord(2, 2))
scalea_zz_to_zz(uint8_t, nd_to_nd, make_nd_ext<T>(65, 65), make_nd_ext<T>(15, 15), make_coord(1, 1))
scalea_zz_to_zz(uint16_t, nd_to_nd, make_nd_ext<T>(33, 33), make_nd_ext<T>(15, 15), make_coord(1, 1))
scalea_zz_to_zz(uint32_t, nd_to_nd, make_nd_ext<T>(17, 17), make_nd_ext<T>(15, 15), make_coord(1, 1))
scalea_zz_to_zz(half, nd_to_nd, make_nd_ext<T>(100, 100), make_nd_ext<T>(15, 15), make_coord(18, 18))
scalea_zz_to_zz(float, nd_to_nd, make_nd_ext<T>(50, 50), make_nd_ext<T>(15, 15), make_coord(10, 10))
scalea_zz_to_zz(uint64_t, nd_to_nd, make_nd_ext<T>(17, 16), make_nd_ext<T>(15, 15), make_coord(1, 0))
scalea_zz_to_zz(uint64_t, nd_to_nd, make_nd_ext<T>(16, 17), make_nd_ext<T>(15, 15), make_coord(0, 1))
scalea_zz_to_zz(half, nd_to_nd, make_nd_ext<T>(8, 64), make_nd_ext<T>(15, 15), make_coord(0, 16))
scalea_zz_to_zz(float, nd_to_nd, make_nd_ext<T>(64, 8), make_nd_ext<T>(15, 7), make_coord(16, 0))
scalea_zz_to_zz(fp4x2_e2m1_t, nd_to_nd, make_nd_ext<T>(32, 32), make_nd_ext<T>(16, 16), make_coord(8, 8))
scalea_zz_to_zz(uint8_t, nd_to_nd, make_nd_ext<T>(32, 32), make_nd_ext<T>(15, 15), make_coord(8, 8))
scalea_zz_to_zz(uint16_t, nd_to_nd, make_nd_ext<T>(32, 32), make_nd_ext<T>(15, 15), make_coord(8, 8))
scalea_zz_to_zz(uint16_t, nd_to_nd, make_nd_ext<T>(48, 48), make_nd_ext<T>(15, 15), make_coord(8, 8))
scalea_zz_to_zz(uint32_t, nd_to_nd, make_nd_ext<T>(24, 24), make_nd_ext<T>(15, 15), make_coord(4, 4))
scalea_zz_to_zz(int8_t, nd_to_nd, make_nd_ext<T>(33, 32), make_nd_ext<T>(15, 15), make_coord(8, 0))
scalea_zz_to_zz(int16_t, nd_to_nd, make_nd_ext<T>(32, 33), make_nd_ext<T>(15, 15), make_coord(0, 8))
scalea_zz_to_zz(int32_t, nd_to_nd, make_nd_ext<T>(40, 40), make_nd_ext<T>(15, 15), make_coord(4, 4))
scalea_zz_to_zz(int64_t, nd_to_nd, make_nd_ext<T>(20, 20), make_nd_ext<T>(15, 15), make_coord(2, 2))

// nd_to_nz
// constraint: fp4 col(include make coord col) % 2 must be 0
// test various data types, b4(fp4x2) to b32
TEST_GM2L1(fp4x2_e2m1_t, nd_to_nz, make_nd_ext<T>(18, 18), make_nz<T>(19, 20))
TEST_GM2L1(fp4x2_e1m2_t, nd_to_nz, make_nd_ext<T>(18, 18), make_nz<T>(19, 20))
TEST_GM2L1(fp8_e4m3fn_t, nd_to_nz, make_nd_ext<T>(18, 9), make_nz<T>(19, 10))
TEST_GM2L1(fp8_e5m2_t, nd_to_nz, make_nd_ext<T>(18, 9), make_nz<T>(19, 10))
TEST_GM2L1(hifloat8_t, nd_to_nz, make_nd_ext<T>(18, 17), make_nz<T>(19, 18))
TEST_GM2L1(half, nd_to_nz, make_nd_ext<T>(18, 17), make_nz<T>(19, 18))
TEST_GM2L1(bfloat16_t, nd_to_nz, make_nd_ext<T>(18, 17), make_nz<T>(19, 18))
TEST_GM2L1(float, nd_to_nz, make_nd_ext<T>(18, 17), make_nz<T>(19, 18))
TEST_GM2L1(int8_t, nd_to_nz, make_nd_ext<T>(18, 9), make_nz<T>(19, 10))
TEST_GM2L1(uint8_t, nd_to_nz, make_nd_ext<T>(18, 9), make_nz<T>(19, 10))
TEST_GM2L1(int16_t, nd_to_nz, make_nd_ext<T>(18, 9), make_nz<T>(19, 10))
TEST_GM2L1(uint16_t, nd_to_nz, make_nd_ext<T>(18, 9), make_nz<T>(19, 10))
TEST_GM2L1(int32_t, nd_to_nz, make_nd_ext<T>(18, 9), make_nz<T>(19, 10))
TEST_GM2L1(uint32_t, nd_to_nz, make_nd_ext<T>(18, 9), make_nz<T>(19, 10))

TEST_GM2L1(fp4x2_e2m1_t, nd_to_nz, make_nd_ext<T>(68, 68), make_nz<T>(69, 70))
TEST_GM2L1(fp4x2_e2m1_t, nd_to_nz, make_nd_ext<T>(18, 18), make_nz<T>(69, 70))
TEST_GM2L1(uint8_t, nd_to_nz, make_nd_ext<T>(18, 18), make_nz<T>(69, 70))
TEST_GM2L1(uint16_t, nd_to_nz, make_nd_ext<T>(18, 18), make_nz<T>(69, 70))
TEST_GM2L1(uint32_t, nd_to_nz, make_nd_ext<T>(18, 18), make_nz<T>(69, 70))
TEST_GM2L1(uint8_t, nd_to_nz, make_nd_ext<T>(28, 28), make_nz<T>(69, 70))
TEST_GM2L1(uint16_t, nd_to_nz, make_nd_ext<T>(28, 28), make_nz<T>(69, 70))
TEST_GM2L1(uint32_t, nd_to_nz, make_nd_ext<T>(28, 28), make_nz<T>(69, 70))
TEST_GM2L1(uint8_t, nd_to_nz, make_nd_ext<T>(38, 38), make_nz<T>(69, 70))
TEST_GM2L1(uint16_t, nd_to_nz, make_nd_ext<T>(38, 38), make_nz<T>(69, 70))
TEST_GM2L1(uint32_t, nd_to_nz, make_nd_ext<T>(38, 38), make_nz<T>(69, 70))
TEST_GM2L1(uint8_t, nd_to_nz, make_nd_ext<T>(48, 48), make_nz<T>(69, 70))
TEST_GM2L1(uint16_t, nd_to_nz, make_nd_ext<T>(48, 48), make_nz<T>(69, 70))
TEST_GM2L1(uint32_t, nd_to_nz, make_nd_ext<T>(48, 48), make_nz<T>(69, 70))

// make_nd
TEST_GM2L1(fp4x2_e2m1_t, nd_layout_to_nz, make_nd<T>(18, 18), make_nz<T>(19, 20))
TEST_GM2L1(fp4x2_e1m2_t, nd_layout_to_nz, make_nd<T>(18, 18), make_nz<T>(19, 20))
TEST_GM2L1(half, nd_layout_to_nz, make_nd<T>(18, 17), make_nz<T>(19, 18))
TEST_GM2L1(float, nd_layout_to_nz, make_nd<T>(18, 17), make_nz<T>(19, 18))
TEST_GM2L1(uint8_t, nd_layout_to_nz, make_nd<T>(18, 9), make_nz<T>(19, 10))
TEST_GM2L1(uint16_t, nd_layout_to_nz, make_nd<T>(18, 9), make_nz<T>(19, 10))
TEST_GM2L1(uint32_t, nd_layout_to_nz, make_nd<T>(18, 9), make_nz<T>(19, 10))

scalea_zz_to_zz(fp4x2_e2m1_t, nd_to_nz, make_nd_ext<T>(33, 26), make_nz<T>(19, 18), make_coord(0, 0))
scalea_zz_to_zz(uint8_t, nd_to_nz, make_nd_ext<T>(33, 25), make_nz<T>(19, 18), make_coord(0, 0))
scalea_zz_to_zz(uint16_t, nd_to_nz, make_nd_ext<T>(33, 25), make_nz<T>(19, 18), make_coord(0, 0))
scalea_zz_to_zz(uint32_t, nd_to_nz, make_nd_ext<T>(33, 25), make_nz<T>(19, 18), make_coord(0, 0))
scalea_zz_to_zz(fp4x2_e2m1_t, nd_to_nz, make_nd_ext<T>(33, 26), make_nz<T>(19, 18), make_coord(10, 10))
scalea_zz_to_zz(uint8_t, nd_to_nz, make_nd_ext<T>(33, 25), make_nz<T>(19, 18), make_coord(10, 10))
scalea_zz_to_zz(uint16_t, nd_to_nz, make_nd_ext<T>(33, 25), make_nz<T>(19, 18), make_coord(10, 10))
scalea_zz_to_zz(uint32_t, nd_to_nz, make_nd_ext<T>(33, 25), make_nz<T>(19, 18), make_coord(10, 10))
scalea_zz_to_zz(fp4x2_e2m1_t, nd_to_nz, make_nd_ext<T>(33, 26), make_nz<T>(19, 18), make_coord(16, 16))
scalea_zz_to_zz(uint8_t, nd_to_nz, make_nd_ext<T>(33, 25), make_nz<T>(19, 18), make_coord(16, 16))
scalea_zz_to_zz(uint16_t, nd_to_nz, make_nd_ext<T>(33, 25), make_nz<T>(19, 18), make_coord(16, 16))
scalea_zz_to_zz(uint32_t, nd_to_nz, make_nd_ext<T>(33, 25), make_nz<T>(19, 18), make_coord(16, 16))
scalea_zz_to_zz(fp4x2_e2m1_t, nd_to_nz, make_nd_ext<T>(64, 64), make_nz<T>(19, 18), make_coord(16, 16))
scalea_zz_to_zz(uint8_t, nd_to_nz, make_nd_ext<T>(64, 64), make_nz<T>(19, 18), make_coord(16, 16))
scalea_zz_to_zz(uint16_t, nd_to_nz, make_nd_ext<T>(64, 64), make_nz<T>(19, 18), make_coord(16, 16))
scalea_zz_to_zz(uint32_t, nd_to_nz, make_nd_ext<T>(64, 64), make_nz<T>(19, 18), make_coord(16, 16))
scalea_zz_to_zz(uint8_t, nd_to_nz, make_nd_ext<T>(18, 18), make_nz<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint16_t, nd_to_nz, make_nd_ext<T>(18, 18), make_nz<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint32_t, nd_to_nz, make_nd_ext<T>(18, 18), make_nz<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint8_t, nd_to_nz, make_nd_ext<T>(28, 28), make_nz<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint16_t, nd_to_nz, make_nd_ext<T>(28, 28), make_nz<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint32_t, nd_to_nz, make_nd_ext<T>(28, 28), make_nz<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint8_t, nd_to_nz, make_nd_ext<T>(38, 38), make_nz<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint16_t, nd_to_nz, make_nd_ext<T>(38, 38), make_nz<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint32_t, nd_to_nz, make_nd_ext<T>(38, 38), make_nz<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint8_t, nd_to_nz, make_nd_ext<T>(48, 48), make_nz<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint16_t, nd_to_nz, make_nd_ext<T>(48, 48), make_nz<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint32_t, nd_to_nz, make_nd_ext<T>(48, 48), make_nz<T>(69, 70), make_coord(10, 10))

// Additional nd_to_nz test cases with various shapes and coords
scalea_zz_to_zz(fp4x2_e2m1_t, nd_to_nz, make_nd_ext<T>(16, 16), make_nz<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(fp4x2_e2m1_t, nd_to_nz, make_nd_ext<T>(16, 16), make_nz<T>(16, 16), make_coord(8, 8))
scalea_zz_to_zz(fp4x2_e1m2_t, nd_to_nz, make_nd_ext<T>(20, 20), make_nz<T>(18, 18), make_coord(2, 2))
scalea_zz_to_zz(fp8_e4m3fn_t, nd_to_nz, make_nd_ext<T>(16, 16), make_nz<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(fp8_e5m2_t, nd_to_nz, make_nd_ext<T>(16, 16), make_nz<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(hifloat8_t, nd_to_nz, make_nd_ext<T>(32, 32), make_nz<T>(32, 32), make_coord(0, 0))
scalea_zz_to_zz(hifloat8_t, nd_to_nz, make_nd_ext<T>(32, 32), make_nz<T>(32, 32), make_coord(16, 16))
scalea_zz_to_zz(half, nd_to_nz, make_nd_ext<T>(32, 32), make_nz<T>(32, 32), make_coord(0, 0))
scalea_zz_to_zz(half, nd_to_nz, make_nd_ext<T>(32, 32), make_nz<T>(32, 32), make_coord(16, 16))
scalea_zz_to_zz(bfloat16_t, nd_to_nz, make_nd_ext<T>(32, 32), make_nz<T>(32, 32), make_coord(0, 0))
scalea_zz_to_zz(float, nd_to_nz, make_nd_ext<T>(32, 32), make_nz<T>(32, 32), make_coord(0, 0))
scalea_zz_to_zz(float, nd_to_nz, make_nd_ext<T>(32, 32), make_nz<T>(32, 32), make_coord(16, 16))
scalea_zz_to_zz(int8_t, nd_to_nz, make_nd_ext<T>(16, 16), make_nz<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(uint8_t, nd_to_nz, make_nd_ext<T>(16, 16), make_nz<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(int16_t, nd_to_nz, make_nd_ext<T>(16, 16), make_nz<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(uint16_t, nd_to_nz, make_nd_ext<T>(16, 16), make_nz<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(int32_t, nd_to_nz, make_nd_ext<T>(16, 16), make_nz<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(uint32_t, nd_to_nz, make_nd_ext<T>(16, 16), make_nz<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(fp4x2_e2m1_t, nd_to_nz, make_nd_ext<T>(68, 68), make_nz<T>(64, 64), make_coord(2, 2))
scalea_zz_to_zz(fp4x2_e2m1_t, nd_to_nz, make_nd_ext<T>(34, 34), make_nz<T>(32, 32), make_coord(1, 2))
scalea_zz_to_zz(half, nd_to_nz, make_nd_ext<T>(64, 64), make_nz<T>(64, 64), make_coord(0, 0))
scalea_zz_to_zz(half, nd_to_nz, make_nd_ext<T>(64, 64), make_nz<T>(64, 64), make_coord(32, 32))
scalea_zz_to_zz(float, nd_to_nz, make_nd_ext<T>(48, 48), make_nz<T>(48, 48), make_coord(16, 16))
scalea_zz_to_zz(uint8_t, nd_to_nz, make_nd_ext<T>(64, 64), make_nz<T>(64, 64), make_coord(0, 0))
scalea_zz_to_zz(uint16_t, nd_to_nz, make_nd_ext<T>(48, 48), make_nz<T>(48, 48), make_coord(0, 0))
scalea_zz_to_zz(uint32_t, nd_to_nz, make_nd_ext<T>(32, 32), make_nz<T>(32, 32), make_coord(0, 0))
scalea_zz_to_zz(fp8_e4m3fn_t, nd_to_nz, make_nd_ext<T>(24, 24), make_nz<T>(24, 24), make_coord(8, 8))
scalea_zz_to_zz(fp8_e5m2_t, nd_to_nz, make_nd_ext<T>(20, 20), make_nz<T>(20, 20), make_coord(4, 4))
scalea_zz_to_zz(hifloat8_t, nd_to_nz, make_nd_ext<T>(40, 40), make_nz<T>(40, 40), make_coord(8, 8))
scalea_zz_to_zz(bfloat16_t, nd_to_nz, make_nd_ext<T>(36, 36), make_nz<T>(36, 36), make_coord(4, 4))
scalea_zz_to_zz(int8_t, nd_to_nz, make_nd_ext<T>(28, 28), make_nz<T>(28, 28), make_coord(8, 8))
scalea_zz_to_zz(int16_t, nd_to_nz, make_nd_ext<T>(32, 32), make_nz<T>(32, 32), make_coord(8, 8))
scalea_zz_to_zz(int32_t, nd_to_nz, make_nd_ext<T>(24, 24), make_nz<T>(24, 24), make_coord(8, 8))

// nd_to_zn
// test various data types, b8 to b32
TEST_GM2L1(fp8_e4m3fn_t, nd_to_zn, make_nd_ext<T>(18, 9), make_zn<T>(19, 10))
TEST_GM2L1(fp8_e5m2_t, nd_to_zn, make_nd_ext<T>(18, 9), make_zn<T>(19, 10))
TEST_GM2L1(hifloat8_t, nd_to_zn, make_nd_ext<T>(18, 17), make_zn<T>(19, 18))
TEST_GM2L1(half, nd_to_zn, make_nd_ext<T>(18, 17), make_zn<T>(19, 18))
TEST_GM2L1(bfloat16_t, nd_to_zn, make_nd_ext<T>(18, 17), make_zn<T>(19, 18))
TEST_GM2L1(float, nd_to_zn, make_nd_ext<T>(18, 17), make_zn<T>(19, 18))
TEST_GM2L1(int8_t, nd_to_zn, make_nd_ext<T>(18, 9), make_zn<T>(19, 10))
TEST_GM2L1(uint8_t, nd_to_zn, make_nd_ext<T>(18, 9), make_zn<T>(19, 10))
TEST_GM2L1(int16_t, nd_to_zn, make_nd_ext<T>(18, 9), make_zn<T>(19, 10))
TEST_GM2L1(uint16_t, nd_to_zn, make_nd_ext<T>(18, 9), make_zn<T>(19, 10))
TEST_GM2L1(int32_t, nd_to_zn, make_nd_ext<T>(18, 9), make_zn<T>(19, 10))
TEST_GM2L1(uint32_t, nd_to_zn, make_nd_ext<T>(18, 9), make_zn<T>(19, 10))

TEST_GM2L1(uint8_t, nd_to_zn, make_nd_ext<T>(18, 9), make_zn<T>(19, 10))
TEST_GM2L1(half, nd_to_zn, make_nd_ext<T>(18, 17), make_zn<T>(19, 18))
TEST_GM2L1(half, nd_to_zn, make_nd_ext<T>(18, 17), make_zn<T>(19, 38))
TEST_GM2L1(half, nd_to_zn, make_nd_ext<T>(18, 17), make_zn<T>(39, 18))
TEST_GM2L1(half, nd_to_zn, make_nd_ext<T>(18, 17), make_zn<T>(39, 48))
TEST_GM2L1(uint32_t, nd_to_zn, make_nd_ext<T>(16, 32), make_zn<T>(16, 32))
TEST_GM2L1(uint32_t, nd_to_zn, make_nd_ext<T>(18, 17), make_zn<T>(19, 18))
TEST_GM2L1(uint8_t, nd_to_zn, make_nd_ext<T>(18, 18), make_zn<T>(69, 70))
TEST_GM2L1(uint16_t, nd_to_zn, make_nd_ext<T>(18, 18), make_zn<T>(69, 70))
TEST_GM2L1(uint32_t, nd_to_zn, make_nd_ext<T>(18, 18), make_zn<T>(69, 70))
TEST_GM2L1(uint8_t, nd_to_zn, make_nd_ext<T>(28, 28), make_zn<T>(69, 70))
TEST_GM2L1(uint16_t, nd_to_zn, make_nd_ext<T>(28, 28), make_zn<T>(69, 70))
TEST_GM2L1(uint32_t, nd_to_zn, make_nd_ext<T>(28, 28), make_zn<T>(69, 70))
TEST_GM2L1(uint8_t, nd_to_zn, make_nd_ext<T>(38, 38), make_zn<T>(69, 70))
TEST_GM2L1(uint16_t, nd_to_zn, make_nd_ext<T>(38, 38), make_zn<T>(69, 70))
TEST_GM2L1(uint32_t, nd_to_zn, make_nd_ext<T>(38, 38), make_zn<T>(69, 70))
TEST_GM2L1(uint8_t, nd_to_zn, make_nd_ext<T>(48, 48), make_zn<T>(69, 70))
TEST_GM2L1(uint16_t, nd_to_zn, make_nd_ext<T>(48, 48), make_zn<T>(69, 70))
TEST_GM2L1(uint32_t, nd_to_zn, make_nd_ext<T>(48, 48), make_zn<T>(69, 70))

// make_nd
TEST_GM2L1(fp8_e4m3fn_t, nd_layout_to_zn, make_nd<T>(18, 9), make_zn<T>(19, 10))
TEST_GM2L1(fp8_e5m2_t, nd_layout_to_zn, make_nd<T>(18, 9), make_zn<T>(19, 10))
TEST_GM2L1(half, nd_layout_to_zn, make_nd<T>(18, 17), make_zn<T>(19, 18))
TEST_GM2L1(float, nd_layout_to_zn, make_nd<T>(18, 17), make_zn<T>(19, 18))
TEST_GM2L1(uint8_t, nd_layout_to_zn, make_nd<T>(18, 9), make_zn<T>(19, 10))
TEST_GM2L1(uint16_t, nd_layout_to_zn, make_nd<T>(18, 9), make_zn<T>(19, 10))
TEST_GM2L1(uint32_t, nd_layout_to_zn, make_nd<T>(18, 9), make_zn<T>(19, 10))

scalea_zz_to_zz(half, nd_to_zn, make_nd_ext<T>(18, 17), make_zn<T>(39, 48), make_coord(0, 0))
scalea_zz_to_zz(uint8_t, nd_to_zn, make_nd_ext<T>(33, 25), make_zn<T>(19, 18), make_coord(0, 0))
scalea_zz_to_zz(uint8_t, nd_to_zn, make_nd_ext<T>(33, 25), make_zn<T>(19, 18), make_coord(10, 10))
scalea_zz_to_zz(uint16_t, nd_to_zn, make_nd_ext<T>(33, 25), make_zn<T>(19, 18), make_coord(0, 0))
scalea_zz_to_zz(uint16_t, nd_to_zn, make_nd_ext<T>(33, 25), make_zn<T>(19, 18), make_coord(10, 10))
scalea_zz_to_zz(float, nd_to_zn, make_nd_ext<T>(33, 25), make_zn<T>(19, 18), make_coord(0, 0))
scalea_zz_to_zz(float, nd_to_zn, make_nd_ext<T>(33, 25), make_zn<T>(19, 18), make_coord(10, 10))
scalea_zz_to_zz(uint8_t, nd_to_zn, make_nd_ext<T>(18, 18), make_zn<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint16_t, nd_to_zn, make_nd_ext<T>(18, 18), make_zn<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint32_t, nd_to_zn, make_nd_ext<T>(18, 18), make_zn<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint8_t, nd_to_zn, make_nd_ext<T>(28, 28), make_zn<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint16_t, nd_to_zn, make_nd_ext<T>(28, 28), make_zn<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint32_t, nd_to_zn, make_nd_ext<T>(28, 28), make_zn<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint8_t, nd_to_zn, make_nd_ext<T>(38, 38), make_zn<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint16_t, nd_to_zn, make_nd_ext<T>(38, 38), make_zn<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint32_t, nd_to_zn, make_nd_ext<T>(38, 38), make_zn<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint8_t, nd_to_zn, make_nd_ext<T>(48, 48), make_zn<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint16_t, nd_to_zn, make_nd_ext<T>(48, 48), make_zn<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint32_t, nd_to_zn, make_nd_ext<T>(48, 48), make_zn<T>(69, 70), make_coord(10, 10))

// Additional nd_to_zn test cases with various shapes and coords
scalea_zz_to_zz(fp8_e4m3fn_t, nd_to_zn, make_nd_ext<T>(16, 16), make_zn<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(fp8_e5m2_t, nd_to_zn, make_nd_ext<T>(16, 16), make_zn<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(hifloat8_t, nd_to_zn, make_nd_ext<T>(32, 32), make_zn<T>(32, 32), make_coord(0, 0))
scalea_zz_to_zz(hifloat8_t, nd_to_zn, make_nd_ext<T>(32, 32), make_zn<T>(32, 32), make_coord(16, 16))
scalea_zz_to_zz(half, nd_to_zn, make_nd_ext<T>(32, 32), make_zn<T>(32, 32), make_coord(0, 0))
scalea_zz_to_zz(half, nd_to_zn, make_nd_ext<T>(32, 32), make_zn<T>(32, 32), make_coord(16, 16))
scalea_zz_to_zz(bfloat16_t, nd_to_zn, make_nd_ext<T>(32, 32), make_zn<T>(32, 32), make_coord(0, 0))
scalea_zz_to_zz(float, nd_to_zn, make_nd_ext<T>(32, 32), make_zn<T>(32, 32), make_coord(0, 0))
scalea_zz_to_zz(float, nd_to_zn, make_nd_ext<T>(32, 32), make_zn<T>(32, 32), make_coord(16, 16))
scalea_zz_to_zz(int8_t, nd_to_zn, make_nd_ext<T>(16, 16), make_zn<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(uint8_t, nd_to_zn, make_nd_ext<T>(16, 16), make_zn<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(int16_t, nd_to_zn, make_nd_ext<T>(16, 16), make_zn<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(uint16_t, nd_to_zn, make_nd_ext<T>(16, 16), make_zn<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(int32_t, nd_to_zn, make_nd_ext<T>(16, 16), make_zn<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(uint32_t, nd_to_zn, make_nd_ext<T>(16, 16), make_zn<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(half, nd_to_zn, make_nd_ext<T>(64, 64), make_zn<T>(64, 64), make_coord(0, 0))
scalea_zz_to_zz(half, nd_to_zn, make_nd_ext<T>(64, 64), make_zn<T>(64, 64), make_coord(32, 32))
scalea_zz_to_zz(float, nd_to_zn, make_nd_ext<T>(48, 48), make_zn<T>(48, 48), make_coord(16, 16))
scalea_zz_to_zz(uint8_t, nd_to_zn, make_nd_ext<T>(64, 64), make_zn<T>(64, 64), make_coord(0, 0))
scalea_zz_to_zz(uint16_t, nd_to_zn, make_nd_ext<T>(48, 48), make_zn<T>(48, 48), make_coord(0, 0))
scalea_zz_to_zz(uint32_t, nd_to_zn, make_nd_ext<T>(32, 32), make_zn<T>(32, 32), make_coord(0, 0))
scalea_zz_to_zz(fp8_e4m3fn_t, nd_to_zn, make_nd_ext<T>(24, 24), make_zn<T>(24, 24), make_coord(8, 8))
scalea_zz_to_zz(fp8_e5m2_t, nd_to_zn, make_nd_ext<T>(20, 20), make_zn<T>(20, 20), make_coord(4, 4))
scalea_zz_to_zz(hifloat8_t, nd_to_zn, make_nd_ext<T>(40, 40), make_zn<T>(40, 40), make_coord(8, 8))
scalea_zz_to_zz(bfloat16_t, nd_to_zn, make_nd_ext<T>(36, 36), make_zn<T>(36, 36), make_coord(4, 4))
scalea_zz_to_zz(int8_t, nd_to_zn, make_nd_ext<T>(28, 28), make_zn<T>(28, 28), make_coord(8, 8))
scalea_zz_to_zz(int16_t, nd_to_zn, make_nd_ext<T>(32, 32), make_zn<T>(32, 32), make_coord(8, 8))
scalea_zz_to_zz(int32_t, nd_to_zn, make_nd_ext<T>(24, 24), make_zn<T>(24, 24), make_coord(8, 8))

// dn_to_nz
// test various data types, b8 to b32
TEST_GM2L1(fp8_e4m3fn_t, dn_to_nz, make_dn_ext<T>(18, 9), make_nz<T>(19, 10))
TEST_GM2L1(fp8_e5m2_t, dn_to_nz, make_dn_ext<T>(18, 9), make_nz<T>(19, 10))
TEST_GM2L1(hifloat8_t, dn_to_nz, make_dn_ext<T>(18, 17), make_nz<T>(19, 18))
TEST_GM2L1(half, dn_to_nz, make_dn_ext<T>(18, 17), make_nz<T>(19, 18))
TEST_GM2L1(bfloat16_t, dn_to_nz, make_dn_ext<T>(18, 17), make_nz<T>(19, 18))
TEST_GM2L1(float, dn_to_nz, make_dn_ext<T>(18, 17), make_nz<T>(19, 18))
TEST_GM2L1(int8_t, dn_to_nz, make_dn_ext<T>(18, 9), make_nz<T>(19, 10))
TEST_GM2L1(uint8_t, dn_to_nz, make_dn_ext<T>(18, 9), make_nz<T>(19, 10))
TEST_GM2L1(int16_t, dn_to_nz, make_dn_ext<T>(18, 9), make_nz<T>(19, 10))
TEST_GM2L1(uint16_t, dn_to_nz, make_dn_ext<T>(18, 9), make_nz<T>(19, 10))
TEST_GM2L1(int32_t, dn_to_nz, make_dn_ext<T>(18, 9), make_nz<T>(19, 10))
TEST_GM2L1(uint32_t, dn_to_nz, make_dn_ext<T>(18, 9), make_nz<T>(19, 10))

TEST_GM2L1(uint8_t, dn_to_nz, make_dn_ext<T>(16, 32), make_nz<T>(16, 32))
TEST_GM2L1(uint8_t, dn_to_nz, make_dn_ext<T>(17, 18), make_nz<T>(18, 19))
TEST_GM2L1(uint16_t, dn_to_nz, make_dn_ext<T>(16, 32), make_nz<T>(16, 32))
TEST_GM2L1(uint16_t, dn_to_nz, make_dn_ext<T>(17, 18), make_nz<T>(18, 19))
TEST_GM2L1(float, dn_to_nz, make_dn_ext<T>(16, 32), make_nz<T>(16, 32))
TEST_GM2L1(float, dn_to_nz, make_dn_ext<T>(17, 18), make_nz<T>(18, 19))
TEST_GM2L1(uint8_t, dn_to_nz, make_dn_ext<T>(18, 18), make_nz<T>(69, 70))
TEST_GM2L1(uint16_t, dn_to_nz, make_dn_ext<T>(18, 18), make_nz<T>(69, 70))
TEST_GM2L1(uint32_t, dn_to_nz, make_dn_ext<T>(18, 18), make_nz<T>(69, 70))
TEST_GM2L1(uint8_t, dn_to_nz, make_dn_ext<T>(28, 28), make_nz<T>(69, 70))
TEST_GM2L1(uint16_t, dn_to_nz, make_dn_ext<T>(28, 28), make_nz<T>(69, 70))
TEST_GM2L1(uint32_t, dn_to_nz, make_dn_ext<T>(28, 28), make_nz<T>(69, 70))
TEST_GM2L1(uint8_t, dn_to_nz, make_dn_ext<T>(38, 38), make_nz<T>(69, 70))
TEST_GM2L1(uint16_t, dn_to_nz, make_dn_ext<T>(38, 38), make_nz<T>(69, 70))
TEST_GM2L1(uint32_t, dn_to_nz, make_dn_ext<T>(38, 38), make_nz<T>(69, 70))
TEST_GM2L1(uint8_t, dn_to_nz, make_dn_ext<T>(48, 48), make_nz<T>(69, 70))
TEST_GM2L1(uint16_t, dn_to_nz, make_dn_ext<T>(48, 48), make_nz<T>(69, 70))
TEST_GM2L1(uint32_t, dn_to_nz, make_dn_ext<T>(48, 48), make_nz<T>(69, 70))

// make_dn
TEST_GM2L1(fp8_e4m3fn_t, dn_layout_to_nz, make_dn<T>(18, 9), make_nz<T>(19, 10))
TEST_GM2L1(fp8_e5m2_t, dn_layout_to_nz, make_dn<T>(18, 9), make_nz<T>(19, 10))
TEST_GM2L1(half, dn_layout_to_nz, make_dn<T>(18, 17), make_nz<T>(19, 18))
TEST_GM2L1(float, dn_layout_to_nz, make_dn<T>(18, 17), make_nz<T>(19, 18))
TEST_GM2L1(uint8_t, dn_layout_to_nz, make_dn<T>(18, 9), make_nz<T>(19, 10))
TEST_GM2L1(uint16_t, dn_layout_to_nz, make_dn<T>(18, 9), make_nz<T>(19, 10))
TEST_GM2L1(uint32_t, dn_layout_to_nz, make_dn<T>(18, 9), make_nz<T>(19, 10))

scalea_zz_to_zz(half, dn_to_nz, make_dn_ext<T>(18, 17), make_nz<T>(39, 48), make_coord(0, 0))
scalea_zz_to_zz(uint8_t, dn_to_nz, make_dn_ext<T>(33, 25), make_nz<T>(19, 18), make_coord(0, 0))
scalea_zz_to_zz(uint8_t, dn_to_nz, make_dn_ext<T>(33, 25), make_nz<T>(19, 18), make_coord(10, 10))
scalea_zz_to_zz(uint16_t, dn_to_nz, make_dn_ext<T>(33, 25), make_nz<T>(19, 18), make_coord(0, 0))
scalea_zz_to_zz(uint16_t, dn_to_nz, make_dn_ext<T>(33, 25), make_nz<T>(19, 18), make_coord(10, 10))
scalea_zz_to_zz(float, dn_to_nz, make_dn_ext<T>(33, 25), make_nz<T>(19, 18), make_coord(0, 0))
scalea_zz_to_zz(float, dn_to_nz, make_dn_ext<T>(33, 25), make_nz<T>(19, 18), make_coord(10, 10))
scalea_zz_to_zz(uint8_t, dn_to_nz, make_dn_ext<T>(18, 18), make_nz<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint16_t, dn_to_nz, make_dn_ext<T>(18, 18), make_nz<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint32_t, dn_to_nz, make_dn_ext<T>(18, 18), make_nz<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint8_t, dn_to_nz, make_dn_ext<T>(28, 28), make_nz<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint16_t, dn_to_nz, make_dn_ext<T>(28, 28), make_nz<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint32_t, dn_to_nz, make_dn_ext<T>(28, 28), make_nz<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint8_t, dn_to_nz, make_dn_ext<T>(38, 38), make_nz<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint16_t, dn_to_nz, make_dn_ext<T>(38, 38), make_nz<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint32_t, dn_to_nz, make_dn_ext<T>(38, 38), make_nz<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint8_t, dn_to_nz, make_dn_ext<T>(48, 48), make_nz<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint16_t, dn_to_nz, make_dn_ext<T>(48, 48), make_nz<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint32_t, dn_to_nz, make_dn_ext<T>(48, 48), make_nz<T>(69, 70), make_coord(10, 10))

// Additional dn_to_nz test cases with various shapes and coords
scalea_zz_to_zz(fp8_e4m3fn_t, dn_to_nz, make_dn_ext<T>(16, 16), make_nz<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(fp8_e5m2_t, dn_to_nz, make_dn_ext<T>(16, 16), make_nz<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(hifloat8_t, dn_to_nz, make_dn_ext<T>(32, 32), make_nz<T>(32, 32), make_coord(0, 0))
scalea_zz_to_zz(hifloat8_t, dn_to_nz, make_dn_ext<T>(32, 32), make_nz<T>(32, 32), make_coord(16, 16))
scalea_zz_to_zz(half, dn_to_nz, make_dn_ext<T>(32, 32), make_nz<T>(32, 32), make_coord(0, 0))
scalea_zz_to_zz(half, dn_to_nz, make_dn_ext<T>(32, 32), make_nz<T>(32, 32), make_coord(16, 16))
scalea_zz_to_zz(bfloat16_t, dn_to_nz, make_dn_ext<T>(32, 32), make_nz<T>(32, 32), make_coord(0, 0))
scalea_zz_to_zz(float, dn_to_nz, make_dn_ext<T>(32, 32), make_nz<T>(32, 32), make_coord(0, 0))
scalea_zz_to_zz(float, dn_to_nz, make_dn_ext<T>(32, 32), make_nz<T>(32, 32), make_coord(16, 16))
scalea_zz_to_zz(int8_t, dn_to_nz, make_dn_ext<T>(16, 16), make_nz<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(uint8_t, dn_to_nz, make_dn_ext<T>(16, 16), make_nz<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(int16_t, dn_to_nz, make_dn_ext<T>(16, 16), make_nz<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(uint16_t, dn_to_nz, make_dn_ext<T>(16, 16), make_nz<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(int32_t, dn_to_nz, make_dn_ext<T>(16, 16), make_nz<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(uint32_t, dn_to_nz, make_dn_ext<T>(16, 16), make_nz<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(half, dn_to_nz, make_dn_ext<T>(64, 64), make_nz<T>(64, 64), make_coord(0, 0))
scalea_zz_to_zz(half, dn_to_nz, make_dn_ext<T>(64, 64), make_nz<T>(64, 64), make_coord(32, 32))
scalea_zz_to_zz(float, dn_to_nz, make_dn_ext<T>(48, 48), make_nz<T>(48, 48), make_coord(16, 16))
scalea_zz_to_zz(uint8_t, dn_to_nz, make_dn_ext<T>(64, 64), make_nz<T>(64, 64), make_coord(0, 0))
scalea_zz_to_zz(uint16_t, dn_to_nz, make_dn_ext<T>(48, 48), make_nz<T>(48, 48), make_coord(0, 0))
scalea_zz_to_zz(uint32_t, dn_to_nz, make_dn_ext<T>(32, 32), make_nz<T>(32, 32), make_coord(0, 0))
scalea_zz_to_zz(fp8_e4m3fn_t, dn_to_nz, make_dn_ext<T>(24, 24), make_nz<T>(24, 24), make_coord(8, 8))
scalea_zz_to_zz(fp8_e5m2_t, dn_to_nz, make_dn_ext<T>(20, 20), make_nz<T>(20, 20), make_coord(4, 4))
scalea_zz_to_zz(hifloat8_t, dn_to_nz, make_dn_ext<T>(40, 40), make_nz<T>(40, 40), make_coord(8, 8))
scalea_zz_to_zz(bfloat16_t, dn_to_nz, make_dn_ext<T>(36, 36), make_nz<T>(36, 36), make_coord(4, 4))
scalea_zz_to_zz(int8_t, dn_to_nz, make_dn_ext<T>(28, 28), make_nz<T>(28, 28), make_coord(8, 8))
scalea_zz_to_zz(int16_t, dn_to_nz, make_dn_ext<T>(32, 32), make_nz<T>(32, 32), make_coord(8, 8))
scalea_zz_to_zz(int32_t, dn_to_nz, make_dn_ext<T>(24, 24), make_nz<T>(24, 24), make_coord(8, 8))

// dn_to_zn
// constraint: fp4 row(include make coord row) % 2 must be 0
// test various data types, b4(fp4x2) to b32
TEST_GM2L1(fp4x2_e1m2_t, dn_to_zn, make_dn_ext<T>(18, 9), make_zn<T>(20, 10))
TEST_GM2L1(fp4x2_e2m1_t, dn_to_zn, make_dn_ext<T>(18, 9), make_zn<T>(20, 10))
TEST_GM2L1(fp8_e4m3fn_t, dn_to_zn, make_dn_ext<T>(18, 9), make_zn<T>(19, 10))
TEST_GM2L1(fp8_e5m2_t, dn_to_zn, make_dn_ext<T>(18, 9), make_zn<T>(19, 10))
TEST_GM2L1(hifloat8_t, dn_to_zn, make_dn_ext<T>(18, 17), make_zn<T>(19, 18))
TEST_GM2L1(half, dn_to_zn, make_dn_ext<T>(18, 17), make_zn<T>(19, 18))
TEST_GM2L1(bfloat16_t, dn_to_zn, make_dn_ext<T>(18, 17), make_zn<T>(19, 18))
TEST_GM2L1(float, dn_to_zn, make_dn_ext<T>(18, 17), make_zn<T>(19, 18))
TEST_GM2L1(int8_t, dn_to_zn, make_dn_ext<T>(18, 9), make_zn<T>(19, 10))
TEST_GM2L1(uint8_t, dn_to_zn, make_dn_ext<T>(18, 9), make_zn<T>(19, 10))
TEST_GM2L1(int16_t, dn_to_zn, make_dn_ext<T>(18, 9), make_zn<T>(19, 10))
TEST_GM2L1(uint16_t, dn_to_zn, make_dn_ext<T>(18, 9), make_zn<T>(19, 10))
TEST_GM2L1(int32_t, dn_to_zn, make_dn_ext<T>(18, 9), make_zn<T>(19, 10))
TEST_GM2L1(uint32_t, dn_to_zn, make_dn_ext<T>(18, 9), make_zn<T>(19, 10))

TEST_GM2L1(float, dn_to_zn, make_dn_ext<T>(16, 32), make_zn<T>(16, 32))
TEST_GM2L1(float, dn_to_zn, make_dn_ext<T>(18, 9), make_zn<T>(19, 10))
TEST_GM2L1(uint16_t, dn_to_zn, make_dn_ext<T>(18, 18), make_zn<T>(19, 20))
TEST_GM2L1(fp4x2_e2m1_t, dn_to_zn, make_dn_ext<T>(18, 18), make_zn<T>(19, 20))
TEST_GM2L1(fp4x2_e2m1_t, dn_to_zn, make_dn_ext<T>(68, 68), make_zn<T>(69, 70))
TEST_GM2L1(uint8_t, dn_to_zn, make_dn_ext<T>(18, 18), make_zn<T>(69, 70))
TEST_GM2L1(uint16_t, dn_to_zn, make_dn_ext<T>(18, 18), make_zn<T>(69, 70))
TEST_GM2L1(uint32_t, dn_to_zn, make_dn_ext<T>(18, 18), make_zn<T>(69, 70))
TEST_GM2L1(uint8_t, dn_to_zn, make_dn_ext<T>(28, 28), make_zn<T>(69, 70))
TEST_GM2L1(uint16_t, dn_to_zn, make_dn_ext<T>(28, 28), make_zn<T>(69, 70))
TEST_GM2L1(uint32_t, dn_to_zn, make_dn_ext<T>(28, 28), make_zn<T>(69, 70))
TEST_GM2L1(uint8_t, dn_to_zn, make_dn_ext<T>(38, 38), make_zn<T>(69, 70))
TEST_GM2L1(uint16_t, dn_to_zn, make_dn_ext<T>(38, 38), make_zn<T>(69, 70))
TEST_GM2L1(uint32_t, dn_to_zn, make_dn_ext<T>(38, 38), make_zn<T>(69, 70))
TEST_GM2L1(uint8_t, dn_to_zn, make_dn_ext<T>(48, 48), make_zn<T>(69, 70))
TEST_GM2L1(uint16_t, dn_to_zn, make_dn_ext<T>(48, 48), make_zn<T>(69, 70))
TEST_GM2L1(uint32_t, dn_to_zn, make_dn_ext<T>(48, 48), make_zn<T>(69, 70))

// make_dn
TEST_GM2L1(fp4x2_e1m2_t, dn_layout_to_zn, make_dn<T>(18, 9), make_zn<T>(20, 10))
TEST_GM2L1(fp4x2_e2m1_t, dn_layout_to_zn, make_dn<T>(18, 9), make_zn<T>(20, 10))
TEST_GM2L1(half, dn_layout_to_zn, make_dn<T>(18, 17), make_zn<T>(19, 18))
TEST_GM2L1(float, dn_layout_to_zn, make_dn<T>(18, 17), make_zn<T>(19, 18))
TEST_GM2L1(uint8_t, dn_layout_to_zn, make_dn<T>(18, 9), make_zn<T>(19, 10))
TEST_GM2L1(uint16_t, dn_layout_to_zn, make_dn<T>(18, 9), make_zn<T>(19, 10))
TEST_GM2L1(uint32_t, dn_layout_to_zn, make_dn<T>(18, 9), make_zn<T>(19, 10))

scalea_zz_to_zz(fp4x2_e2m1_t, dn_to_zn, make_dn_ext<T>(18, 17), make_zn<T>(40, 48), make_coord(0, 0))
scalea_zz_to_zz(fp4x2_e2m1_t, dn_to_zn, make_dn_ext<T>(34, 25), make_zn<T>(20, 18), make_coord(0, 0))
scalea_zz_to_zz(fp4x2_e2m1_t, dn_to_zn, make_dn_ext<T>(34, 25), make_zn<T>(20, 18), make_coord(10, 10))
scalea_zz_to_zz(half, dn_to_zn, make_dn_ext<T>(18, 17), make_zn<T>(39, 48), make_coord(0, 0))
scalea_zz_to_zz(uint8_t, dn_to_zn, make_dn_ext<T>(33, 25), make_zn<T>(19, 18), make_coord(0, 0))
scalea_zz_to_zz(uint8_t, dn_to_zn, make_dn_ext<T>(33, 25), make_zn<T>(19, 18), make_coord(10, 10))
scalea_zz_to_zz(uint16_t, dn_to_zn, make_dn_ext<T>(33, 25), make_zn<T>(19, 18), make_coord(0, 0))
scalea_zz_to_zz(uint16_t, dn_to_zn, make_dn_ext<T>(33, 25), make_zn<T>(19, 18), make_coord(10, 10))
scalea_zz_to_zz(float, dn_to_zn, make_dn_ext<T>(33, 25), make_zn<T>(19, 18), make_coord(0, 0))
scalea_zz_to_zz(float, dn_to_zn, make_dn_ext<T>(33, 25), make_zn<T>(19, 18), make_coord(10, 10))
scalea_zz_to_zz(uint8_t, dn_to_zn, make_dn_ext<T>(18, 18), make_zn<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint16_t, dn_to_zn, make_dn_ext<T>(18, 18), make_zn<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint32_t, dn_to_zn, make_dn_ext<T>(18, 18), make_zn<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint8_t, dn_to_zn, make_dn_ext<T>(28, 28), make_zn<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint16_t, dn_to_zn, make_dn_ext<T>(28, 28), make_zn<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint32_t, dn_to_zn, make_dn_ext<T>(28, 28), make_zn<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint8_t, dn_to_zn, make_dn_ext<T>(38, 38), make_zn<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint16_t, dn_to_zn, make_dn_ext<T>(38, 38), make_zn<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint32_t, dn_to_zn, make_dn_ext<T>(38, 38), make_zn<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint8_t, dn_to_zn, make_dn_ext<T>(48, 48), make_zn<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint16_t, dn_to_zn, make_dn_ext<T>(48, 48), make_zn<T>(69, 70), make_coord(10, 10))
scalea_zz_to_zz(uint32_t, dn_to_zn, make_dn_ext<T>(48, 48), make_zn<T>(69, 70), make_coord(10, 10))

// Additional dn_to_zn test cases with various shapes and coords
scalea_zz_to_zz(fp4x2_e1m2_t, dn_to_zn, make_dn_ext<T>(16, 16), make_zn<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(fp4x2_e2m1_t, dn_to_zn, make_dn_ext<T>(16, 16), make_zn<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(fp8_e4m3fn_t, dn_to_zn, make_dn_ext<T>(16, 16), make_zn<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(fp8_e5m2_t, dn_to_zn, make_dn_ext<T>(16, 16), make_zn<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(hifloat8_t, dn_to_zn, make_dn_ext<T>(32, 32), make_zn<T>(32, 32), make_coord(0, 0))
scalea_zz_to_zz(hifloat8_t, dn_to_zn, make_dn_ext<T>(32, 32), make_zn<T>(32, 32), make_coord(16, 16))
scalea_zz_to_zz(half, dn_to_zn, make_dn_ext<T>(32, 32), make_zn<T>(32, 32), make_coord(0, 0))
scalea_zz_to_zz(half, dn_to_zn, make_dn_ext<T>(32, 32), make_zn<T>(32, 32), make_coord(16, 16))
scalea_zz_to_zz(bfloat16_t, dn_to_zn, make_dn_ext<T>(32, 32), make_zn<T>(32, 32), make_coord(0, 0))
scalea_zz_to_zz(float, dn_to_zn, make_dn_ext<T>(32, 32), make_zn<T>(32, 32), make_coord(0, 0))
scalea_zz_to_zz(float, dn_to_zn, make_dn_ext<T>(32, 32), make_zn<T>(32, 32), make_coord(16, 16))
scalea_zz_to_zz(int8_t, dn_to_zn, make_dn_ext<T>(16, 16), make_zn<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(uint8_t, dn_to_zn, make_dn_ext<T>(16, 16), make_zn<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(int16_t, dn_to_zn, make_dn_ext<T>(16, 16), make_zn<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(uint16_t, dn_to_zn, make_dn_ext<T>(16, 16), make_zn<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(int32_t, dn_to_zn, make_dn_ext<T>(16, 16), make_zn<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(uint32_t, dn_to_zn, make_dn_ext<T>(16, 16), make_zn<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(fp4x2_e2m1_t, dn_to_zn, make_dn_ext<T>(68, 68), make_zn<T>(64, 64), make_coord(2, 2))
scalea_zz_to_zz(fp4x2_e2m1_t, dn_to_zn, make_dn_ext<T>(34, 34), make_zn<T>(32, 32), make_coord(2, 1))
scalea_zz_to_zz(half, dn_to_zn, make_dn_ext<T>(64, 64), make_zn<T>(64, 64), make_coord(0, 0))
scalea_zz_to_zz(half, dn_to_zn, make_dn_ext<T>(64, 64), make_zn<T>(64, 64), make_coord(32, 32))
scalea_zz_to_zz(float, dn_to_zn, make_dn_ext<T>(48, 48), make_zn<T>(48, 48), make_coord(16, 16))
scalea_zz_to_zz(uint8_t, dn_to_zn, make_dn_ext<T>(64, 64), make_zn<T>(64, 64), make_coord(0, 0))
scalea_zz_to_zz(uint16_t, dn_to_zn, make_dn_ext<T>(48, 48), make_zn<T>(48, 48), make_coord(0, 0))
scalea_zz_to_zz(uint32_t, dn_to_zn, make_dn_ext<T>(32, 32), make_zn<T>(32, 32), make_coord(0, 0))
scalea_zz_to_zz(fp8_e4m3fn_t, dn_to_zn, make_dn_ext<T>(24, 24), make_zn<T>(24, 24), make_coord(8, 8))
scalea_zz_to_zz(fp8_e5m2_t, dn_to_zn, make_dn_ext<T>(20, 20), make_zn<T>(20, 20), make_coord(4, 4))
scalea_zz_to_zz(hifloat8_t, dn_to_zn, make_dn_ext<T>(40, 40), make_zn<T>(40, 40), make_coord(8, 8))
scalea_zz_to_zz(bfloat16_t, dn_to_zn, make_dn_ext<T>(36, 36), make_zn<T>(36, 36), make_coord(4, 4))
scalea_zz_to_zz(int8_t, dn_to_zn, make_dn_ext<T>(28, 28), make_zn<T>(28, 28), make_coord(8, 8))
scalea_zz_to_zz(int16_t, dn_to_zn, make_dn_ext<T>(32, 32), make_zn<T>(32, 32), make_coord(8, 8))
scalea_zz_to_zz(int32_t, dn_to_zn, make_dn_ext<T>(24, 24), make_zn<T>(24, 24), make_coord(8, 8))

// nz_to_nz
// constraint: make coord row % 16 = 0, col % 32B = 0
// test various data types, b4 to b64
TEST_GM2L1(fp4x2_e1m2_t, nz_to_nz, make_nz<T>(18, 18), make_nz<T>(19, 20))
TEST_GM2L1(fp4x2_e2m1_t, nz_to_nz, make_nz<T>(18, 18), make_nz<T>(19, 20))
TEST_GM2L1(fp8_e4m3fn_t, nz_to_nz, make_nz<T>(18, 9), make_nz<T>(19, 10))
TEST_GM2L1(fp8_e5m2_t, nz_to_nz, make_nz<T>(18, 9), make_nz<T>(19, 10))
TEST_GM2L1(hifloat8_t, nz_to_nz, make_nz<T>(18, 17), make_nz<T>(19, 18))
TEST_GM2L1(half, nz_to_nz, make_nz<T>(18, 17), make_nz<T>(19, 18))
TEST_GM2L1(bfloat16_t, nz_to_nz, make_nz<T>(18, 17), make_nz<T>(19, 18))
TEST_GM2L1(float, nz_to_nz, make_nz<T>(18, 17), make_nz<T>(19, 18))
TEST_GM2L1(int8_t, nz_to_nz, make_nz<T>(18, 9), make_nz<T>(19, 10))
TEST_GM2L1(uint8_t, nz_to_nz, make_nz<T>(18, 9), make_nz<T>(19, 10))
TEST_GM2L1(int16_t, nz_to_nz, make_nz<T>(18, 9), make_nz<T>(19, 10))
TEST_GM2L1(uint16_t, nz_to_nz, make_nz<T>(18, 9), make_nz<T>(19, 10))
TEST_GM2L1(int32_t, nz_to_nz, make_nz<T>(18, 9), make_nz<T>(19, 10))
TEST_GM2L1(uint32_t, nz_to_nz, make_nz<T>(18, 9), make_nz<T>(19, 10))
TEST_GM2L1(int64_t, nz_to_nz, make_nz<T>(18, 9), make_nz<T>(19, 10))
TEST_GM2L1(uint64_t, nz_to_nz, make_nz<T>(18, 9), make_nz<T>(19, 10))

TEST_GM2L1(fp4x2_e1m2_t, nz_to_nz, make_nz<T>(17, 36), make_nz<T>(38, 40))
TEST_GM2L1(fp4x2_e1m2_t, nz_to_nz, make_nz<T>(13, 36), make_nz<T>(14, 20))
TEST_GM2L1(fp4x2_e1m2_t, nz_to_nz, make_nz<T>(17, 36), make_nz<T>(19, 20))
TEST_GM2L1(uint8_t, nz_to_nz, make_nz<T>(17, 18), make_nz<T>(38, 40))
TEST_GM2L1(uint8_t, nz_to_nz, make_nz<T>(13, 18), make_nz<T>(14, 20))
TEST_GM2L1(uint8_t, nz_to_nz, make_nz<T>(17, 18), make_nz<T>(19, 20))
TEST_GM2L1(uint16_t, nz_to_nz, make_nz<T>(17, 18), make_nz<T>(38, 40))
TEST_GM2L1(uint16_t, nz_to_nz, make_nz<T>(13, 18), make_nz<T>(14, 20))
TEST_GM2L1(uint16_t, nz_to_nz, make_nz<T>(17, 18), make_nz<T>(19, 20))
TEST_GM2L1(uint32_t, nz_to_nz, make_nz<T>(17, 18), make_nz<T>(38, 40))
TEST_GM2L1(uint32_t, nz_to_nz, make_nz<T>(13, 18), make_nz<T>(14, 20))
TEST_GM2L1(uint32_t, nz_to_nz, make_nz<T>(17, 18), make_nz<T>(19, 20))
TEST_GM2L1(uint64_t, nz_to_nz, make_nz<T>(13, 18), make_nz<T>(14, 20))
TEST_GM2L1(uint64_t, nz_to_nz, make_nz<T>(17, 18), make_nz<T>(19, 20))
TEST_GM2L1(fp4x2_e1m2_t, nz_to_nz, make_nz<T>(18, 36), make_nz<T>(69, 70))
TEST_GM2L1(uint8_t, nz_to_nz, make_nz<T>(18, 18), make_nz<T>(69, 70))
TEST_GM2L1(uint16_t, nz_to_nz, make_nz<T>(18, 18), make_nz<T>(69, 70))
TEST_GM2L1(uint32_t, nz_to_nz, make_nz<T>(18, 18), make_nz<T>(69, 70))
TEST_GM2L1(fp4x2_e1m2_t, nz_to_nz, make_nz<T>(28, 56), make_nz<T>(69, 140))
TEST_GM2L1(uint8_t, nz_to_nz, make_nz<T>(28, 28), make_nz<T>(69, 70))
TEST_GM2L1(uint16_t, nz_to_nz, make_nz<T>(28, 28), make_nz<T>(69, 70))
TEST_GM2L1(uint32_t, nz_to_nz, make_nz<T>(28, 28), make_nz<T>(69, 70))
TEST_GM2L1(fp4x2_e1m2_t, nz_to_nz, make_nz<T>(38, 76), make_nz<T>(69, 140))
TEST_GM2L1(uint8_t, nz_to_nz, make_nz<T>(38, 38), make_nz<T>(69, 70))
TEST_GM2L1(uint16_t, nz_to_nz, make_nz<T>(38, 38), make_nz<T>(69, 70))
TEST_GM2L1(uint32_t, nz_to_nz, make_nz<T>(38, 38), make_nz<T>(69, 70))
TEST_GM2L1(fp4x2_e1m2_t, nz_to_nz, make_nz<T>(48, 96), make_nz<T>(69, 140))
TEST_GM2L1(uint8_t, nz_to_nz, make_nz<T>(48, 48), make_nz<T>(69, 70))
TEST_GM2L1(uint16_t, nz_to_nz, make_nz<T>(48, 48), make_nz<T>(69, 70))
TEST_GM2L1(uint32_t, nz_to_nz, make_nz<T>(48, 48), make_nz<T>(69, 70))

scalea_zz_to_zz(half, nz_to_nz, make_nz<T>(18, 17), make_nz<T>(39, 48), make_coord(0, 0))
scalea_zz_to_zz(fp4x2_e1m2_t, nz_to_nz, make_nz<T>(33, 50), make_nz<T>(19, 36), make_coord(0, 0))
scalea_zz_to_zz(fp4x2_e1m2_t, nz_to_nz, make_nz<T>(33, 50), make_nz<T>(19, 36), make_coord(16, 0))
scalea_zz_to_zz(uint8_t, nz_to_nz, make_nz<T>(33, 25), make_nz<T>(19, 18), make_coord(0, 0))
scalea_zz_to_zz(uint8_t, nz_to_nz, make_nz<T>(33, 25), make_nz<T>(19, 18), make_coord(16, 0))
scalea_zz_to_zz(uint16_t, nz_to_nz, make_nz<T>(33, 25), make_nz<T>(19, 18), make_coord(0, 0))
scalea_zz_to_zz(uint16_t, nz_to_nz, make_nz<T>(33, 25), make_nz<T>(19, 18), make_coord(16, 16))
scalea_zz_to_zz(float, nz_to_nz, make_nz<T>(33, 25), make_nz<T>(19, 18), make_coord(0, 0))
scalea_zz_to_zz(float, nz_to_nz, make_nz<T>(33, 25), make_nz<T>(19, 18), make_coord(16, 8))
scalea_zz_to_zz(uint64_t, nz_to_nz, make_nz<T>(33, 25), make_nz<T>(19, 18), make_coord(0, 0))
scalea_zz_to_zz(uint64_t, nz_to_nz, make_nz<T>(33, 25), make_nz<T>(19, 18), make_coord(16, 4))
scalea_zz_to_zz(fp4x2_e1m2_t, nz_to_nz, make_nz<T>(18, 18), make_nz<T>(69, 70), make_coord(16, 0))
scalea_zz_to_zz(uint8_t, nz_to_nz, make_nz<T>(18, 18), make_nz<T>(69, 70), make_coord(16, 0))
scalea_zz_to_zz(uint16_t, nz_to_nz, make_nz<T>(18, 18), make_nz<T>(69, 70), make_coord(16, 16))
scalea_zz_to_zz(uint32_t, nz_to_nz, make_nz<T>(18, 18), make_nz<T>(69, 70), make_coord(16, 16))
scalea_zz_to_zz(fp4x2_e1m2_t, nz_to_nz, make_nz<T>(28, 28), make_nz<T>(69, 70), make_coord(16, 0))
scalea_zz_to_zz(uint8_t, nz_to_nz, make_nz<T>(28, 28), make_nz<T>(69, 70), make_coord(16, 0))
scalea_zz_to_zz(uint16_t, nz_to_nz, make_nz<T>(28, 28), make_nz<T>(69, 70), make_coord(16, 16))
scalea_zz_to_zz(uint32_t, nz_to_nz, make_nz<T>(28, 28), make_nz<T>(69, 70), make_coord(16, 8))
scalea_zz_to_zz(fp4x2_e1m2_t, nz_to_nz, make_nz<T>(38, 38), make_nz<T>(69, 70), make_coord(16, 32))
scalea_zz_to_zz(uint8_t, nz_to_nz, make_nz<T>(38, 38), make_nz<T>(69, 70), make_coord(16, 32))
scalea_zz_to_zz(uint16_t, nz_to_nz, make_nz<T>(38, 38), make_nz<T>(69, 70), make_coord(16, 16))
scalea_zz_to_zz(uint32_t, nz_to_nz, make_nz<T>(38, 38), make_nz<T>(69, 70), make_coord(16, 16))
scalea_zz_to_zz(fp4x2_e1m2_t, nz_to_nz, make_nz<T>(48, 48), make_nz<T>(69, 70), make_coord(16, 16))
scalea_zz_to_zz(uint8_t, nz_to_nz, make_nz<T>(48, 48), make_nz<T>(69, 70), make_coord(16, 16))
scalea_zz_to_zz(uint16_t, nz_to_nz, make_nz<T>(48, 48), make_nz<T>(69, 70), make_coord(16, 16))
scalea_zz_to_zz(uint32_t, nz_to_nz, make_nz<T>(48, 48), make_nz<T>(69, 70), make_coord(16, 16))

// Additional nz_to_nz test cases with various shapes and coords
scalea_zz_to_zz(fp4x2_e1m2_t, nz_to_nz, make_nz<T>(16, 16), make_nz<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(fp4x2_e1m2_t, nz_to_nz, make_nz<T>(16, 16), make_nz<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(fp8_e4m3fn_t, nz_to_nz, make_nz<T>(16, 16), make_nz<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(fp8_e5m2_t, nz_to_nz, make_nz<T>(16, 16), make_nz<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(hifloat8_t, nz_to_nz, make_nz<T>(32, 32), make_nz<T>(32, 32), make_coord(0, 0))
scalea_zz_to_zz(hifloat8_t, nz_to_nz, make_nz<T>(32, 32), make_nz<T>(32, 32), make_coord(16, 16))
scalea_zz_to_zz(half, nz_to_nz, make_nz<T>(32, 32), make_nz<T>(32, 32), make_coord(0, 0))
scalea_zz_to_zz(half, nz_to_nz, make_nz<T>(32, 32), make_nz<T>(32, 32), make_coord(16, 16))
scalea_zz_to_zz(bfloat16_t, nz_to_nz, make_nz<T>(32, 32), make_nz<T>(32, 32), make_coord(0, 0))
scalea_zz_to_zz(float, nz_to_nz, make_nz<T>(32, 32), make_nz<T>(32, 32), make_coord(0, 0))
scalea_zz_to_zz(float, nz_to_nz, make_nz<T>(32, 32), make_nz<T>(32, 32), make_coord(16, 16))
scalea_zz_to_zz(fp4x2_e1m2_t, nz_to_nz, make_nz<T>(16, 16), make_nz<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(int8_t, nz_to_nz, make_nz<T>(16, 16), make_nz<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(uint8_t, nz_to_nz, make_nz<T>(16, 16), make_nz<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(int16_t, nz_to_nz, make_nz<T>(16, 16), make_nz<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(uint16_t, nz_to_nz, make_nz<T>(16, 16), make_nz<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(int32_t, nz_to_nz, make_nz<T>(16, 16), make_nz<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(uint32_t, nz_to_nz, make_nz<T>(16, 16), make_nz<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(int64_t, nz_to_nz, make_nz<T>(16, 16), make_nz<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(uint64_t, nz_to_nz, make_nz<T>(16, 16), make_nz<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(half, nz_to_nz, make_nz<T>(64, 64), make_nz<T>(64, 64), make_coord(0, 0))
scalea_zz_to_zz(half, nz_to_nz, make_nz<T>(64, 64), make_nz<T>(64, 64), make_coord(32, 32))
scalea_zz_to_zz(float, nz_to_nz, make_nz<T>(48, 48), make_nz<T>(48, 48), make_coord(16, 16))
scalea_zz_to_zz(fp4x2_e1m2_t, nz_to_nz, make_nz<T>(64, 64), make_nz<T>(64, 64), make_coord(0, 0))
scalea_zz_to_zz(uint8_t, nz_to_nz, make_nz<T>(64, 64), make_nz<T>(64, 64), make_coord(0, 0))
scalea_zz_to_zz(uint16_t, nz_to_nz, make_nz<T>(48, 48), make_nz<T>(48, 48), make_coord(0, 0))
scalea_zz_to_zz(uint32_t, nz_to_nz, make_nz<T>(32, 32), make_nz<T>(32, 32), make_coord(0, 0))
scalea_zz_to_zz(uint64_t, nz_to_nz, make_nz<T>(32, 32), make_nz<T>(32, 32), make_coord(0, 0))
scalea_zz_to_zz(fp8_e4m3fn_t, nz_to_nz, make_nz<T>(24, 24), make_nz<T>(24, 24), make_coord(8, 8))
scalea_zz_to_zz(fp8_e5m2_t, nz_to_nz, make_nz<T>(20, 20), make_nz<T>(20, 20), make_coord(4, 4))
scalea_zz_to_zz(hifloat8_t, nz_to_nz, make_nz<T>(40, 40), make_nz<T>(40, 40), make_coord(8, 8))
scalea_zz_to_zz(bfloat16_t, nz_to_nz, make_nz<T>(36, 36), make_nz<T>(36, 36), make_coord(4, 4))
scalea_zz_to_zz(fp4x2_e1m2_t, nz_to_nz, make_nz<T>(28, 28), make_nz<T>(28, 28), make_coord(8, 8))
scalea_zz_to_zz(int8_t, nz_to_nz, make_nz<T>(28, 28), make_nz<T>(28, 28), make_coord(8, 8))
scalea_zz_to_zz(int16_t, nz_to_nz, make_nz<T>(32, 32), make_nz<T>(32, 32), make_coord(8, 8))
scalea_zz_to_zz(int32_t, nz_to_nz, make_nz<T>(24, 24), make_nz<T>(24, 24), make_coord(8, 8))
scalea_zz_to_zz(int64_t, nz_to_nz, make_nz<T>(20, 20), make_nz<T>(20, 20), make_coord(4, 4))
scalea_zz_to_zz(uint64_t, nz_to_nz, make_nz<T>(20, 20), make_nz<T>(20, 20), make_coord(4, 4))

// zn_to_zn
// constraint: make coord col % 16 = 0, row % 32B = 0
// test various data types, b4 to b64
TEST_GM2L1(fp4x2_e1m2_t, zn_to_zn, make_zn<T>(18, 8), make_zn<T>(19, 10))
TEST_GM2L1(fp4x2_e2m1_t, zn_to_zn, make_zn<T>(18, 8), make_zn<T>(19, 10))
TEST_GM2L1(fp8_e4m3fn_t, zn_to_zn, make_zn<T>(18, 9), make_zn<T>(19, 10))
TEST_GM2L1(fp8_e5m2_t, zn_to_zn, make_zn<T>(18, 9), make_zn<T>(19, 10))
TEST_GM2L1(hifloat8_t, zn_to_zn, make_zn<T>(18, 17), make_zn<T>(19, 18))
TEST_GM2L1(half, zn_to_zn, make_zn<T>(18, 17), make_zn<T>(19, 18))
TEST_GM2L1(bfloat16_t, zn_to_zn, make_zn<T>(18, 17), make_zn<T>(19, 18))
TEST_GM2L1(float, zn_to_zn, make_zn<T>(18, 17), make_zn<T>(19, 18))
TEST_GM2L1(int8_t, zn_to_zn, make_zn<T>(18, 9), make_zn<T>(19, 10))
TEST_GM2L1(uint8_t, zn_to_zn, make_zn<T>(18, 9), make_zn<T>(19, 10))
TEST_GM2L1(int16_t, zn_to_zn, make_zn<T>(18, 9), make_zn<T>(19, 10))
TEST_GM2L1(uint16_t, zn_to_zn, make_zn<T>(18, 9), make_zn<T>(19, 10))
TEST_GM2L1(int32_t, zn_to_zn, make_zn<T>(18, 9), make_zn<T>(19, 10))
TEST_GM2L1(uint32_t, zn_to_zn, make_zn<T>(18, 9), make_zn<T>(19, 10))
TEST_GM2L1(int64_t, zn_to_zn, make_zn<T>(18, 9), make_zn<T>(19, 10))
TEST_GM2L1(uint64_t, zn_to_zn, make_zn<T>(18, 9), make_zn<T>(19, 10))

TEST_GM2L1(fp4x2_e1m2_t, zn_to_zn, make_zn<T>(17, 18), make_zn<T>(38, 40))
TEST_GM2L1(fp4x2_e1m2_t, zn_to_zn, make_zn<T>(13, 18), make_zn<T>(14, 20))
TEST_GM2L1(fp4x2_e1m2_t, zn_to_zn, make_zn<T>(17, 18), make_zn<T>(19, 20))
TEST_GM2L1(uint8_t, zn_to_zn, make_zn<T>(17, 18), make_zn<T>(38, 40))
TEST_GM2L1(uint8_t, zn_to_zn, make_zn<T>(13, 18), make_zn<T>(14, 20))
TEST_GM2L1(uint8_t, zn_to_zn, make_zn<T>(17, 18), make_zn<T>(19, 20))
TEST_GM2L1(uint16_t, zn_to_zn, make_zn<T>(17, 18), make_zn<T>(38, 40))
TEST_GM2L1(uint16_t, zn_to_zn, make_zn<T>(13, 18), make_zn<T>(14, 20))
TEST_GM2L1(uint16_t, zn_to_zn, make_zn<T>(17, 18), make_zn<T>(19, 20))
TEST_GM2L1(uint32_t, zn_to_zn, make_zn<T>(17, 18), make_zn<T>(38, 40))
TEST_GM2L1(uint32_t, zn_to_zn, make_zn<T>(13, 18), make_zn<T>(14, 20))
TEST_GM2L1(uint32_t, zn_to_zn, make_zn<T>(17, 18), make_zn<T>(19, 20))
TEST_GM2L1(uint64_t, zn_to_zn, make_zn<T>(13, 18), make_zn<T>(14, 20))
TEST_GM2L1(uint64_t, zn_to_zn, make_zn<T>(17, 18), make_zn<T>(19, 20))
TEST_GM2L1(fp4x2_e1m2_t, zn_to_zn, make_zn<T>(18, 18), make_zn<T>(69, 70))
TEST_GM2L1(uint8_t, zn_to_zn, make_zn<T>(18, 18), make_zn<T>(69, 70))
TEST_GM2L1(uint16_t, zn_to_zn, make_zn<T>(18, 18), make_zn<T>(69, 70))
TEST_GM2L1(uint32_t, zn_to_zn, make_zn<T>(18, 18), make_zn<T>(69, 70))
TEST_GM2L1(fp4x2_e1m2_t, zn_to_zn, make_zn<T>(28, 28), make_zn<T>(69, 70))
TEST_GM2L1(uint8_t, zn_to_zn, make_zn<T>(28, 28), make_zn<T>(69, 70))
TEST_GM2L1(uint16_t, zn_to_zn, make_zn<T>(28, 28), make_zn<T>(69, 70))
TEST_GM2L1(uint32_t, zn_to_zn, make_zn<T>(28, 28), make_zn<T>(69, 70))
TEST_GM2L1(fp4x2_e1m2_t, zn_to_zn, make_zn<T>(38, 38), make_zn<T>(69, 70))
TEST_GM2L1(uint8_t, zn_to_zn, make_zn<T>(38, 38), make_zn<T>(69, 70))
TEST_GM2L1(uint16_t, zn_to_zn, make_zn<T>(38, 38), make_zn<T>(69, 70))
TEST_GM2L1(uint32_t, zn_to_zn, make_zn<T>(38, 38), make_zn<T>(69, 70))
TEST_GM2L1(fp4x2_e1m2_t, zn_to_zn, make_zn<T>(48, 48), make_zn<T>(69, 70))
TEST_GM2L1(uint8_t, zn_to_zn, make_zn<T>(48, 48), make_zn<T>(69, 70))
TEST_GM2L1(uint16_t, zn_to_zn, make_zn<T>(48, 48), make_zn<T>(69, 70))
TEST_GM2L1(uint32_t, zn_to_zn, make_zn<T>(48, 48), make_zn<T>(69, 70))

scalea_zz_to_zz(half, zn_to_zn, make_zn<T>(18, 17), make_zn<T>(39, 48), make_coord(0, 0))
scalea_zz_to_zz(fp4x2_e1m2_t, zn_to_zn, make_zn<T>(33, 26), make_zn<T>(19, 18), make_coord(0, 0))
scalea_zz_to_zz(fp4x2_e1m2_t, zn_to_zn, make_zn<T>(33, 26), make_zn<T>(19, 18), make_coord(0, 16))
scalea_zz_to_zz(uint8_t, zn_to_zn, make_zn<T>(33, 25), make_zn<T>(19, 18), make_coord(0, 0))
scalea_zz_to_zz(uint8_t, zn_to_zn, make_zn<T>(33, 25), make_zn<T>(19, 18), make_coord(0, 16))
scalea_zz_to_zz(uint16_t, zn_to_zn, make_zn<T>(33, 25), make_zn<T>(19, 18), make_coord(0, 0))
scalea_zz_to_zz(uint16_t, zn_to_zn, make_zn<T>(33, 25), make_zn<T>(19, 18), make_coord(16, 16))
scalea_zz_to_zz(float, zn_to_zn, make_zn<T>(33, 25), make_zn<T>(19, 18), make_coord(0, 0))
scalea_zz_to_zz(float, zn_to_zn, make_zn<T>(33, 25), make_zn<T>(19, 18), make_coord(8, 16))
scalea_zz_to_zz(uint64_t, zn_to_zn, make_zn<T>(33, 25), make_zn<T>(19, 18), make_coord(0, 0))
scalea_zz_to_zz(uint64_t, zn_to_zn, make_zn<T>(33, 25), make_zn<T>(19, 18), make_coord(4, 16))
scalea_zz_to_zz(fp4x2_e1m2_t, zn_to_zn, make_zn<T>(18, 18), make_zn<T>(69, 70), make_coord(0, 16))
scalea_zz_to_zz(uint8_t, zn_to_zn, make_zn<T>(18, 18), make_zn<T>(69, 70), make_coord(0, 16))
scalea_zz_to_zz(uint16_t, zn_to_zn, make_zn<T>(18, 18), make_zn<T>(69, 70), make_coord(16, 16))
scalea_zz_to_zz(uint32_t, zn_to_zn, make_zn<T>(18, 18), make_zn<T>(69, 70), make_coord(8, 16))
scalea_zz_to_zz(fp4x2_e1m2_t, zn_to_zn, make_zn<T>(28, 28), make_zn<T>(69, 70), make_coord(0, 16))
scalea_zz_to_zz(uint8_t, zn_to_zn, make_zn<T>(28, 28), make_zn<T>(69, 70), make_coord(0, 16))
scalea_zz_to_zz(uint16_t, zn_to_zn, make_zn<T>(28, 28), make_zn<T>(69, 70), make_coord(16, 16))
scalea_zz_to_zz(uint32_t, zn_to_zn, make_zn<T>(28, 28), make_zn<T>(69, 70), make_coord(8, 16))
scalea_zz_to_zz(fp4x2_e1m2_t, zn_to_zn, make_zn<T>(38, 38), make_zn<T>(69, 70), make_coord(0, 16))
scalea_zz_to_zz(uint8_t, zn_to_zn, make_zn<T>(38, 38), make_zn<T>(69, 70), make_coord(0, 16))
scalea_zz_to_zz(uint16_t, zn_to_zn, make_zn<T>(38, 38), make_zn<T>(69, 70), make_coord(16, 16))
scalea_zz_to_zz(uint32_t, zn_to_zn, make_zn<T>(38, 38), make_zn<T>(69, 70), make_coord(16, 16))
scalea_zz_to_zz(fp4x2_e1m2_t, zn_to_zn, make_zn<T>(48, 48), make_zn<T>(69, 70), make_coord(16, 16))
scalea_zz_to_zz(uint8_t, zn_to_zn, make_zn<T>(48, 48), make_zn<T>(69, 70), make_coord(16, 16))
scalea_zz_to_zz(uint16_t, zn_to_zn, make_zn<T>(48, 48), make_zn<T>(69, 70), make_coord(16, 16))
scalea_zz_to_zz(uint32_t, zn_to_zn, make_zn<T>(48, 48), make_zn<T>(69, 70), make_coord(16, 16))

// Additional zn_to_zn test cases with various shapes and coords
scalea_zz_to_zz(fp4x2_e1m2_t, zn_to_zn, make_zn<T>(16, 16), make_zn<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(fp4x2_e2m1_t, zn_to_zn, make_zn<T>(16, 16), make_zn<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(fp8_e4m3fn_t, zn_to_zn, make_zn<T>(16, 16), make_zn<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(fp8_e5m2_t, zn_to_zn, make_zn<T>(16, 16), make_zn<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(hifloat8_t, zn_to_zn, make_zn<T>(32, 32), make_zn<T>(32, 32), make_coord(0, 0))
scalea_zz_to_zz(hifloat8_t, zn_to_zn, make_zn<T>(32, 32), make_zn<T>(32, 32), make_coord(16, 16))
scalea_zz_to_zz(half, zn_to_zn, make_zn<T>(32, 32), make_zn<T>(32, 32), make_coord(0, 0))
scalea_zz_to_zz(half, zn_to_zn, make_zn<T>(32, 32), make_zn<T>(32, 32), make_coord(16, 16))
scalea_zz_to_zz(bfloat16_t, zn_to_zn, make_zn<T>(32, 32), make_zn<T>(32, 32), make_coord(0, 0))
scalea_zz_to_zz(float, zn_to_zn, make_zn<T>(32, 32), make_zn<T>(32, 32), make_coord(0, 0))
scalea_zz_to_zz(float, zn_to_zn, make_zn<T>(32, 32), make_zn<T>(32, 32), make_coord(16, 16))
scalea_zz_to_zz(fp4x2_e1m2_t, zn_to_zn, make_zn<T>(16, 16), make_zn<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(int8_t, zn_to_zn, make_zn<T>(16, 16), make_zn<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(uint8_t, zn_to_zn, make_zn<T>(16, 16), make_zn<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(int16_t, zn_to_zn, make_zn<T>(16, 16), make_zn<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(uint16_t, zn_to_zn, make_zn<T>(16, 16), make_zn<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(int32_t, zn_to_zn, make_zn<T>(16, 16), make_zn<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(uint32_t, zn_to_zn, make_zn<T>(16, 16), make_zn<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(int64_t, zn_to_zn, make_zn<T>(16, 16), make_zn<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(uint64_t, zn_to_zn, make_zn<T>(16, 16), make_zn<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(half, zn_to_zn, make_zn<T>(64, 64), make_zn<T>(64, 64), make_coord(0, 0))
scalea_zz_to_zz(half, zn_to_zn, make_zn<T>(64, 64), make_zn<T>(64, 64), make_coord(32, 32))
scalea_zz_to_zz(float, zn_to_zn, make_zn<T>(48, 48), make_zn<T>(48, 48), make_coord(16, 16))
scalea_zz_to_zz(fp4x2_e1m2_t, zn_to_zn, make_zn<T>(64, 64), make_zn<T>(64, 64), make_coord(0, 0))
scalea_zz_to_zz(uint8_t, zn_to_zn, make_zn<T>(64, 64), make_zn<T>(64, 64), make_coord(0, 0))
scalea_zz_to_zz(uint16_t, zn_to_zn, make_zn<T>(48, 48), make_zn<T>(48, 48), make_coord(0, 0))
scalea_zz_to_zz(uint32_t, zn_to_zn, make_zn<T>(32, 32), make_zn<T>(32, 32), make_coord(0, 0))
scalea_zz_to_zz(uint64_t, zn_to_zn, make_zn<T>(32, 32), make_zn<T>(32, 32), make_coord(0, 0))
scalea_zz_to_zz(fp8_e4m3fn_t, zn_to_zn, make_zn<T>(24, 24), make_zn<T>(24, 24), make_coord(8, 8))
scalea_zz_to_zz(fp8_e5m2_t, zn_to_zn, make_zn<T>(20, 20), make_zn<T>(20, 20), make_coord(4, 4))
scalea_zz_to_zz(hifloat8_t, zn_to_zn, make_zn<T>(40, 40), make_zn<T>(40, 40), make_coord(8, 8))
scalea_zz_to_zz(bfloat16_t, zn_to_zn, make_zn<T>(36, 36), make_zn<T>(36, 36), make_coord(4, 4))
scalea_zz_to_zz(fp4x2_e1m2_t, zn_to_zn, make_zn<T>(28, 28), make_zn<T>(28, 28), make_coord(8, 8))
scalea_zz_to_zz(int8_t, zn_to_zn, make_zn<T>(28, 28), make_zn<T>(28, 28), make_coord(8, 8))
scalea_zz_to_zz(int16_t, zn_to_zn, make_zn<T>(32, 32), make_zn<T>(32, 32), make_coord(8, 8))
scalea_zz_to_zz(int32_t, zn_to_zn, make_zn<T>(24, 24), make_zn<T>(24, 24), make_coord(8, 8))
scalea_zz_to_zz(int64_t, zn_to_zn, make_zn<T>(20, 20), make_zn<T>(20, 20), make_coord(4, 4))
scalea_zz_to_zz(uint64_t, zn_to_zn, make_zn<T>(20, 20), make_zn<T>(20, 20), make_coord(4, 4))

// ScaleA
// constraint: scalea col direction, col(include coord col) % 2 must be 0
TEST_GM2L1(fp8_e8m0_t, scalea_nd_to_zz, make_scalea_nd<T>(18, 34), make_zz<T>(19, 36))
TEST_GM2L1(fp8_e8m0_t, scalea_nd_to_zz, make_scalea_nd<T>(18, 34), make_zz<T>(40, 70))
TEST_GM2L1(fp8_e8m0_t, scalea_nd_to_zz, make_scalea_nd<T>(36, 34), make_zz<T>(40, 36))
TEST_GM2L1(fp8_e8m0_t, scalea_nd_to_zz, make_scalea_nd<T>(32, 32), make_zz<T>(32, 32))
TEST_GM2L1(fp8_e8m0_t, scalea_nd_to_zz, make_scalea_nd<T>(2, 8), make_zz<T>(4, 12))

scalea_zz_to_zz(fp8_e8m0_t, scalea_nd_to_zz, make_scalea_nd<T>(36, 34), make_zz<T>(26, 26), make_coord(0, 0))
scalea_zz_to_zz(fp8_e8m0_t, scalea_nd_to_zz, make_scalea_nd<T>(36, 34), make_zz<T>(26, 26), make_coord(10, 10))
scalea_zz_to_zz(fp8_e8m0_t, scalea_nd_to_zz, make_scalea_nd<T>(36, 34), make_zz<T>(40, 36), make_coord(10, 10))
scalea_zz_to_zz(fp8_e8m0_t, scalea_nd_to_zz, make_scalea_nd<T>(36, 34), make_zz<T>(70, 70), make_coord(10, 10))
scalea_zz_to_zz(fp8_e8m0_t, scalea_nd_to_zz, make_scalea_nd<T>(18, 34), make_zz<T>(70, 70), make_coord(10, 10))
scalea_zz_to_zz(fp8_e8m0_t, scalea_nd_to_zz, make_scalea_nd<T>(18, 18), make_zz<T>(70, 70), make_coord(10, 10))
scalea_zz_to_zz(fp8_e8m0_t, scalea_nd_to_zz, make_scalea_nd<T>(32, 32), make_zz<T>(32, 32), make_coord(10, 10))

// Additional scalea_nd_to_zz test cases with various shapes and coords
scalea_zz_to_zz(fp8_e8m0_t, scalea_nd_to_zz, make_scalea_nd<T>(16, 16), make_zz<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(fp8_e8m0_t, scalea_nd_to_zz, make_scalea_nd<T>(16, 16), make_zz<T>(16, 16), make_coord(8, 8))
scalea_zz_to_zz(fp8_e8m0_t, scalea_nd_to_zz, make_scalea_nd<T>(32, 32), make_zz<T>(32, 32), make_coord(0, 0))
scalea_zz_to_zz(fp8_e8m0_t, scalea_nd_to_zz, make_scalea_nd<T>(32, 32), make_zz<T>(32, 32), make_coord(16, 16))
scalea_zz_to_zz(fp8_e8m0_t, scalea_nd_to_zz, make_scalea_nd<T>(64, 64), make_zz<T>(64, 64), make_coord(0, 0))
scalea_zz_to_zz(fp8_e8m0_t, scalea_nd_to_zz, make_scalea_nd<T>(64, 64), make_zz<T>(64, 64), make_coord(32, 32))
scalea_zz_to_zz(fp8_e8m0_t, scalea_nd_to_zz, make_scalea_nd<T>(48, 48), make_zz<T>(48, 48), make_coord(16, 16))
scalea_zz_to_zz(fp8_e8m0_t, scalea_nd_to_zz, make_scalea_nd<T>(24, 24), make_zz<T>(24, 24), make_coord(8, 8))
scalea_zz_to_zz(fp8_e8m0_t, scalea_nd_to_zz, make_scalea_nd<T>(40, 40), make_zz<T>(40, 40), make_coord(8, 8))
scalea_zz_to_zz(fp8_e8m0_t, scalea_nd_to_zz, make_scalea_nd<T>(36, 36), make_zz<T>(36, 36), make_coord(4, 4))
scalea_zz_to_zz(fp8_e8m0_t, scalea_nd_to_zz, make_scalea_nd<T>(20, 20), make_zz<T>(20, 20), make_coord(4, 4))
scalea_zz_to_zz(fp8_e8m0_t, scalea_nd_to_zz, make_scalea_nd<T>(68, 68), make_zz<T>(64, 64), make_coord(2, 2))
scalea_zz_to_zz(fp8_e8m0_t, scalea_nd_to_zz, make_scalea_nd<T>(34, 34), make_zz<T>(32, 32), make_coord(1, 1))

TEST_GM2L1(fp8_e8m0_t, scalea_dn_to_zz, make_scalea_dn<T>(18, 34), make_zz<T>(19, 36))
TEST_GM2L1(fp8_e8m0_t, scalea_dn_to_zz, make_scalea_dn<T>(18, 34), make_zz<T>(40, 70))
TEST_GM2L1(fp8_e8m0_t, scalea_dn_to_zz, make_scalea_dn<T>(36, 34), make_zz<T>(40, 36))
TEST_GM2L1(fp8_e8m0_t, scalea_dn_to_zz, make_scalea_dn<T>(32, 32), make_zz<T>(32, 32))
TEST_GM2L1(fp8_e8m0_t, scalea_dn_to_zz, make_scalea_dn<T>(2, 8), make_zz<T>(4, 12))

scalea_zz_to_zz(fp8_e8m0_t, scalea_dn_to_zz, make_scalea_dn<T>(36, 34), make_zz<T>(26, 26), make_coord(0, 0))
scalea_zz_to_zz(fp8_e8m0_t, scalea_dn_to_zz, make_scalea_dn<T>(36, 34), make_zz<T>(26, 26), make_coord(10, 10))
scalea_zz_to_zz(fp8_e8m0_t, scalea_dn_to_zz, make_scalea_dn<T>(36, 34), make_zz<T>(40, 36), make_coord(10, 10))
scalea_zz_to_zz(fp8_e8m0_t, scalea_dn_to_zz, make_scalea_dn<T>(36, 34), make_zz<T>(70, 70), make_coord(10, 10))
scalea_zz_to_zz(fp8_e8m0_t, scalea_dn_to_zz, make_scalea_dn<T>(18, 34), make_zz<T>(70, 70), make_coord(10, 10))
scalea_zz_to_zz(fp8_e8m0_t, scalea_dn_to_zz, make_scalea_dn<T>(18, 18), make_zz<T>(70, 70), make_coord(10, 10))
scalea_zz_to_zz(fp8_e8m0_t, scalea_dn_to_zz, make_scalea_dn<T>(32, 32), make_zz<T>(32, 32), make_coord(10, 10))

// Additional scalea_dn_to_zz test cases with various shapes and coords
scalea_zz_to_zz(fp8_e8m0_t, scalea_dn_to_zz, make_scalea_dn<T>(16, 16), make_zz<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(fp8_e8m0_t, scalea_dn_to_zz, make_scalea_dn<T>(16, 16), make_zz<T>(16, 16), make_coord(8, 8))
scalea_zz_to_zz(fp8_e8m0_t, scalea_dn_to_zz, make_scalea_dn<T>(32, 32), make_zz<T>(32, 32), make_coord(0, 0))
scalea_zz_to_zz(fp8_e8m0_t, scalea_dn_to_zz, make_scalea_dn<T>(32, 32), make_zz<T>(32, 32), make_coord(16, 16))
scalea_zz_to_zz(fp8_e8m0_t, scalea_dn_to_zz, make_scalea_dn<T>(64, 64), make_zz<T>(64, 64), make_coord(0, 0))
scalea_zz_to_zz(fp8_e8m0_t, scalea_dn_to_zz, make_scalea_dn<T>(64, 64), make_zz<T>(64, 64), make_coord(32, 32))
scalea_zz_to_zz(fp8_e8m0_t, scalea_dn_to_zz, make_scalea_dn<T>(48, 48), make_zz<T>(48, 48), make_coord(16, 16))
scalea_zz_to_zz(fp8_e8m0_t, scalea_dn_to_zz, make_scalea_dn<T>(24, 24), make_zz<T>(24, 24), make_coord(8, 8))
scalea_zz_to_zz(fp8_e8m0_t, scalea_dn_to_zz, make_scalea_dn<T>(40, 40), make_zz<T>(40, 40), make_coord(8, 8))
scalea_zz_to_zz(fp8_e8m0_t, scalea_dn_to_zz, make_scalea_dn<T>(36, 36), make_zz<T>(36, 36), make_coord(4, 4))
scalea_zz_to_zz(fp8_e8m0_t, scalea_dn_to_zz, make_scalea_dn<T>(20, 20), make_zz<T>(20, 20), make_coord(4, 4))
scalea_zz_to_zz(fp8_e8m0_t, scalea_dn_to_zz, make_scalea_dn<T>(68, 68), make_zz<T>(64, 64), make_coord(2, 2))
scalea_zz_to_zz(fp8_e8m0_t, scalea_dn_to_zz, make_scalea_dn<T>(34, 34), make_zz<T>(32, 32), make_coord(1, 1))

TEST_GM2L1(fp8_e8m0_t, scalea_zz_to_zz, make_zz<T>(18, 34), make_zz<T>(19, 36))
TEST_GM2L1(fp8_e8m0_t, scalea_zz_to_zz, make_zz<T>(18, 34), make_zz<T>(40, 70))
TEST_GM2L1(fp8_e8m0_t, scalea_zz_to_zz, make_zz<T>(36, 34), make_zz<T>(40, 36))
TEST_GM2L1(fp8_e8m0_t, scalea_zz_to_zz, make_zz<T>(32, 32), make_zz<T>(32, 32))
TEST_GM2L1(fp8_e8m0_t, scalea_zz_to_zz, make_zz<T>(2, 8), make_zz<T>(4, 12))

scalea_zz_to_zz(fp8_e8m0_t, scalea_zz_to_zz, make_zz<T>(36, 34), make_zz<T>(26, 26), make_coord(0, 0))
scalea_zz_to_zz(fp8_e8m0_t, scalea_zz_to_zz, make_zz<T>(36, 34), make_zz<T>(26, 26), make_coord(16, 2))
scalea_zz_to_zz(fp8_e8m0_t, scalea_zz_to_zz, make_zz<T>(36, 34), make_zz<T>(40, 36), make_coord(16, 2))
scalea_zz_to_zz(fp8_e8m0_t, scalea_zz_to_zz, make_zz<T>(36, 34), make_zz<T>(70, 70), make_coord(16, 2))
scalea_zz_to_zz(fp8_e8m0_t, scalea_zz_to_zz, make_zz<T>(18, 34), make_zz<T>(70, 70), make_coord(16, 2))
scalea_zz_to_zz(fp8_e8m0_t, scalea_zz_to_zz, make_zz<T>(18, 18), make_zz<T>(70, 70), make_coord(16, 2))
scalea_zz_to_zz(fp8_e8m0_t, scalea_zz_to_zz, make_zz<T>(32, 32), make_zz<T>(32, 32), make_coord(16, 2))
scalea_zz_to_zz(fp8_e8m0_t, scalea_zz_to_zz, make_zz<T>(64, 64), make_zz<T>(64, 64), make_coord(0, 0))
scalea_zz_to_zz(fp8_e8m0_t, scalea_zz_to_zz, make_zz<T>(64, 64), make_zz<T>(64, 64), make_coord(32, 2))
scalea_zz_to_zz(fp8_e8m0_t, scalea_zz_to_zz, make_zz<T>(48, 48), make_zz<T>(48, 48), make_coord(16, 2))
scalea_zz_to_zz(fp8_e8m0_t, scalea_zz_to_zz, make_zz<T>(24, 24), make_zz<T>(24, 24), make_coord(16, 2))
scalea_zz_to_zz(fp8_e8m0_t, scalea_zz_to_zz, make_zz<T>(40, 40), make_zz<T>(40, 40), make_coord(16, 2))
scalea_zz_to_zz(fp8_e8m0_t, scalea_zz_to_zz, make_zz<T>(36, 36), make_zz<T>(36, 36), make_coord(16, 2))
scalea_zz_to_zz(fp8_e8m0_t, scalea_zz_to_zz, make_zz<T>(20, 20), make_zz<T>(20, 20), make_coord(16, 2))
scalea_zz_to_zz(fp8_e8m0_t, scalea_zz_to_zz, make_zz<T>(68, 68), make_zz<T>(64, 64), make_coord(16, 2))
scalea_zz_to_zz(fp8_e8m0_t, scalea_zz_to_zz, make_zz<T>(34, 34), make_zz<T>(32, 32), make_coord(16, 2))

// ScaleB
// constraint: scaleb row direction, row(include coord row) % 2 must be 0
TEST_GM2L1(fp8_e8m0_t, scaleb_nd_to_nn, make_scaleb_nd<T>(18, 34), make_nn<T>(20, 36))
TEST_GM2L1(fp8_e8m0_t, scaleb_nd_to_nn, make_scaleb_nd<T>(18, 34), make_nn<T>(40, 70))
TEST_GM2L1(fp8_e8m0_t, scaleb_nd_to_nn, make_scaleb_nd<T>(36, 33), make_nn<T>(40, 35))
TEST_GM2L1(fp8_e8m0_t, scaleb_nd_to_nn, make_scaleb_nd<T>(32, 32), make_nn<T>(32, 32))
TEST_GM2L1(fp8_e8m0_t, scaleb_nd_to_nn, make_scaleb_nd<T>(2, 8), make_nn<T>(4, 12))

scalea_zz_to_zz(fp8_e8m0_t, scaleb_nd_to_nn, make_scaleb_nd<T>(36, 33), make_nn<T>(26, 25), make_coord(0, 0))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_nd_to_nn, make_scaleb_nd<T>(36, 33), make_nn<T>(26, 25), make_coord(10, 10))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_nd_to_nn, make_scaleb_nd<T>(36, 33), make_nn<T>(40, 35), make_coord(10, 10))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_nd_to_nn, make_scaleb_nd<T>(36, 33), make_nn<T>(70, 70), make_coord(10, 10))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_nd_to_nn, make_scaleb_nd<T>(18, 33), make_nn<T>(70, 70), make_coord(10, 10))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_nd_to_nn, make_scaleb_nd<T>(32, 32), make_nn<T>(32, 32), make_coord(10, 10))

// Additional scaleb_nd_to_nn test cases with various shapes and coords
scalea_zz_to_zz(fp8_e8m0_t, scaleb_nd_to_nn, make_scaleb_nd<T>(16, 16), make_nn<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_nd_to_nn, make_scaleb_nd<T>(16, 16), make_nn<T>(16, 16), make_coord(8, 8))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_nd_to_nn, make_scaleb_nd<T>(32, 32), make_nn<T>(32, 32), make_coord(0, 0))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_nd_to_nn, make_scaleb_nd<T>(32, 32), make_nn<T>(32, 32), make_coord(16, 16))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_nd_to_nn, make_scaleb_nd<T>(64, 64), make_nn<T>(64, 64), make_coord(0, 0))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_nd_to_nn, make_scaleb_nd<T>(64, 64), make_nn<T>(64, 64), make_coord(32, 32))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_nd_to_nn, make_scaleb_nd<T>(48, 48), make_nn<T>(48, 48), make_coord(16, 16))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_nd_to_nn, make_scaleb_nd<T>(24, 24), make_nn<T>(24, 24), make_coord(8, 8))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_nd_to_nn, make_scaleb_nd<T>(40, 40), make_nn<T>(40, 40), make_coord(8, 8))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_nd_to_nn, make_scaleb_nd<T>(36, 36), make_nn<T>(36, 36), make_coord(4, 4))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_nd_to_nn, make_scaleb_nd<T>(20, 20), make_nn<T>(20, 20), make_coord(4, 4))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_nd_to_nn, make_scaleb_nd<T>(68, 68), make_nn<T>(64, 64), make_coord(2, 2))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_nd_to_nn, make_scaleb_nd<T>(34, 34), make_nn<T>(32, 32), make_coord(1, 1))

TEST_GM2L1(fp8_e8m0_t, scaleb_dn_to_nn, make_scaleb_dn<T>(18, 34), make_nn<T>(20, 36))
TEST_GM2L1(fp8_e8m0_t, scaleb_dn_to_nn, make_scaleb_dn<T>(18, 34), make_nn<T>(40, 70))
TEST_GM2L1(fp8_e8m0_t, scaleb_dn_to_nn, make_scaleb_dn<T>(36, 33), make_nn<T>(40, 35))
TEST_GM2L1(fp8_e8m0_t, scaleb_dn_to_nn, make_scaleb_dn<T>(32, 32), make_nn<T>(32, 32))
TEST_GM2L1(fp8_e8m0_t, scaleb_dn_to_nn, make_scaleb_dn<T>(2, 8), make_nn<T>(4, 12))

scalea_zz_to_zz(fp8_e8m0_t, scaleb_dn_to_nn, make_scaleb_dn<T>(36, 33), make_nn<T>(26, 25), make_coord(0, 0))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_dn_to_nn, make_scaleb_dn<T>(36, 33), make_nn<T>(26, 25), make_coord(10, 10))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_dn_to_nn, make_scaleb_dn<T>(36, 33), make_nn<T>(40, 35), make_coord(10, 10))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_dn_to_nn, make_scaleb_dn<T>(36, 33), make_nn<T>(70, 70), make_coord(10, 10))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_dn_to_nn, make_scaleb_dn<T>(18, 33), make_nn<T>(70, 70), make_coord(10, 10))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_dn_to_nn, make_scaleb_dn<T>(32, 32), make_nn<T>(32, 32), make_coord(10, 10))

// Additional scaleb_dn_to_nn test cases with various shapes and coords
scalea_zz_to_zz(fp8_e8m0_t, scaleb_dn_to_nn, make_scaleb_dn<T>(16, 16), make_nn<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_dn_to_nn, make_scaleb_dn<T>(16, 16), make_nn<T>(16, 16), make_coord(8, 8))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_dn_to_nn, make_scaleb_dn<T>(32, 32), make_nn<T>(32, 32), make_coord(0, 0))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_dn_to_nn, make_scaleb_dn<T>(32, 32), make_nn<T>(32, 32), make_coord(16, 16))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_dn_to_nn, make_scaleb_dn<T>(64, 64), make_nn<T>(64, 64), make_coord(0, 0))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_dn_to_nn, make_scaleb_dn<T>(64, 64), make_nn<T>(64, 64), make_coord(32, 32))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_dn_to_nn, make_scaleb_dn<T>(48, 48), make_nn<T>(48, 48), make_coord(16, 16))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_dn_to_nn, make_scaleb_dn<T>(24, 24), make_nn<T>(24, 24), make_coord(8, 8))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_dn_to_nn, make_scaleb_dn<T>(40, 40), make_nn<T>(40, 40), make_coord(8, 8))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_dn_to_nn, make_scaleb_dn<T>(36, 36), make_nn<T>(36, 36), make_coord(4, 4))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_dn_to_nn, make_scaleb_dn<T>(20, 20), make_nn<T>(20, 20), make_coord(4, 4))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_dn_to_nn, make_scaleb_dn<T>(68, 68), make_nn<T>(64, 64), make_coord(2, 2))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_dn_to_nn, make_scaleb_dn<T>(34, 34), make_nn<T>(32, 32), make_coord(1, 1))

TEST_GM2L1(fp8_e8m0_t, scaleb_nn_to_nn, make_nn<T>(18, 34), make_nn<T>(20, 36))
TEST_GM2L1(fp8_e8m0_t, scaleb_nn_to_nn, make_nn<T>(18, 34), make_nn<T>(40, 70))
TEST_GM2L1(fp8_e8m0_t, scaleb_nn_to_nn, make_nn<T>(36, 33), make_nn<T>(40, 35))
TEST_GM2L1(fp8_e8m0_t, scaleb_nn_to_nn, make_nn<T>(32, 32), make_nn<T>(32, 32))
TEST_GM2L1(fp8_e8m0_t, scaleb_nn_to_nn, make_nn<T>(2, 8), make_nn<T>(4, 12))

scalea_zz_to_zz(fp8_e8m0_t, scaleb_nn_to_nn, make_nn<T>(36, 33), make_nn<T>(26, 25), make_coord(0, 0))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_nn_to_nn, make_nn<T>(36, 33), make_nn<T>(26, 25), make_coord(2, 16))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_nn_to_nn, make_nn<T>(36, 33), make_nn<T>(40, 35), make_coord(2, 16))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_nn_to_nn, make_nn<T>(36, 33), make_nn<T>(70, 70), make_coord(2, 16))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_nn_to_nn, make_nn<T>(18, 33), make_nn<T>(70, 70), make_coord(2, 16))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_nn_to_nn, make_nn<T>(32, 32), make_nn<T>(32, 32), make_coord(2, 16))

// Additional scaleb_nn_to_nn test cases with various shapes and coords
scalea_zz_to_zz(fp8_e8m0_t, scaleb_nn_to_nn, make_nn<T>(16, 16), make_nn<T>(16, 16), make_coord(0, 0))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_nn_to_nn, make_nn<T>(16, 16), make_nn<T>(16, 16), make_coord(2, 16))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_nn_to_nn, make_nn<T>(32, 32), make_nn<T>(32, 32), make_coord(0, 0))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_nn_to_nn, make_nn<T>(32, 32), make_nn<T>(32, 32), make_coord(2, 16))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_nn_to_nn, make_nn<T>(64, 64), make_nn<T>(64, 64), make_coord(0, 0))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_nn_to_nn, make_nn<T>(64, 64), make_nn<T>(64, 64), make_coord(2, 16))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_nn_to_nn, make_nn<T>(48, 48), make_nn<T>(48, 48), make_coord(2, 16))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_nn_to_nn, make_nn<T>(24, 24), make_nn<T>(24, 24), make_coord(2, 16))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_nn_to_nn, make_nn<T>(40, 40), make_nn<T>(40, 40), make_coord(2, 16))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_nn_to_nn, make_nn<T>(36, 36), make_nn<T>(36, 36), make_coord(2, 16))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_nn_to_nn, make_nn<T>(20, 20), make_nn<T>(20, 20), make_coord(2, 16))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_nn_to_nn, make_nn<T>(68, 68), make_nn<T>(64, 64), make_coord(2, 16))
scalea_zz_to_zz(fp8_e8m0_t, scaleb_nn_to_nn, make_nn<T>(34, 34), make_nn<T>(32, 32), make_coord(2, 16))

template <typename info_1_type, typename info_2_type, size_t dim, typename layout_type>
auto get_sim_layout_element(const layout_type& layout)
{
    if constexpr (layout_type::depth == 2) {
        if constexpr (dim == 0) {
            return 1U;
        } else {
            return get_element<info_1_type, info_2_type>(layout);
        }
    } else {
        return get_element<info_1_type, info_2_type, dim>(layout);
    }
}

// print_tensor
template <typename T>
void print_tensor(const T& src)
{
    using layout_pattern = get_layout_pattern<typename T::layout_type>;
    using src_type = typename T::element_type;
    auto src_layout = src.layout();
    uint32_t m0_size = get_sim_layout_element<attr_info::shape, attr_info::row, 0>(src_layout);
    uint32_t n0_size = get_sim_layout_element<attr_info::shape, attr_info::column, 0>(src_layout);
    uint32_t m1 = get_sim_layout_element<attr_info::shape, attr_info::row, 1>(src_layout);
    uint32_t n1_size = get_sim_layout_element<attr_info::shape, attr_info::column, 1>(src_layout);
    if constexpr (Std::is_same_v<src_type, fp8_e8m0_t> && Std::is_same_v<layout_pattern, scalea_nd_layout_ptn>) {
        std::cout << "scalea_nd";
    } else if constexpr (Std::is_same_v<src_type, fp8_e8m0_t> && Std::is_same_v<layout_pattern, scalea_dn_layout_ptn>) {
        std::cout << "scalea_dn";
    } else if constexpr (Std::is_same_v<src_type, fp8_e8m0_t> && Std::is_same_v<layout_pattern, scalea_nd_layout_ptn>) {
        std::cout << "scalea_nd";
    } else if constexpr (Std::is_same_v<src_type, fp8_e8m0_t> && Std::is_same_v<layout_pattern, scaleb_dn_layout_ptn>) {
        std::cout << "scaleb_dn";
    } else if constexpr (Std::is_same_v<layout_pattern, nd_ext_layout_ptn> || Std::is_same_v<layout_pattern, nd_layout_ptn>) {
        std::cout << "nd";
    } else if constexpr (Std::is_same_v<layout_pattern, dn_ext_layout_ptn> || Std::is_same_v<layout_pattern, dn_layout_ptn>) {
        std::cout << "dn";
    } else if constexpr (Std::is_same_v<layout_pattern, nz_layout_ptn>) {
        std::cout << "nz";
    } else if constexpr (Std::is_same_v<layout_pattern, zn_layout_ptn>) {
        std::cout << "zn";
    } else if constexpr (Std::is_same_v<layout_pattern, zz_layout_ptn>) {
        std::cout << "zz";
    } else if constexpr (Std::is_same_v<layout_pattern, nn_layout_ptn>) {
        std::cout << "nn";
    } else {
        std::cout << "UnknownLayout";
    }
    if constexpr (T::layout_type::depth == 2) {
        std::cout << " layout Result (2D) (" << m1 << ", " << n1_size << "): " << std::endl;
        for (int i = 0; i < m1; i++) {
            std::cout << i << ":\t";
            for (int j = 0; j < n1_size; j++) {
                auto data_addr = &(src[make_coord(i, j)]);
                if constexpr (sizeof(src_type) == 1) {
                    std::cout << static_cast<uint32_t>(*(reinterpret_cast<uint8_t*>(data_addr))) << "\t";
                } else if constexpr (Std::is_same_v<src_type, half>) {
                    std::cout << *(reinterpret_cast<uint16_t*>(data_addr)) << "\t";
                } else {
                    std::cout << *data_addr << "\t";
                }
            }
            std::cout << std::endl;
        }
    } else if (m0_size == 1 && n0_size == 1) { // for 2D layout, print in 2D format
        std::cout << " layout Result (2D) (" << m1 << ", " << n1_size << "): " << std::endl;
        for (int i = 0; i < m1; i++) {
            std::cout << i << ":\t";
            for (int j = 0; j < n1_size; j++) {
                auto data_addr = &(src[make_coord(i, j)]);
                if constexpr (sizeof(src_type) == 1) {
                    std::cout << static_cast<uint32_t>(*(reinterpret_cast<uint8_t*>(data_addr))) << "\t";
                } else if constexpr (Std::is_same_v<src_type, half>) {
                    std::cout << *(reinterpret_cast<uint16_t*>(data_addr)) << "\t";
                } else {
                    std::cout << *data_addr << "\t";
                }
            }
            std::cout << std::endl;
        }
    } else { // for nz, zn, zz, print in 4D format
        std::cout << " layout Result (4D) (" << m1 << ", " << n1_size << ", " << m0_size << ", " << n0_size << "): " << std::endl;
        for (int i0 = 0; i0 < m1; i0++) {
            for (int i1 = 0; i1 < m0_size; i1++) {
                for (int j0 = 0; j0 < n1_size; j0++) {
                    uint32_t block_id = j0 * m1 + i0;
                    for (int j1 = 0; j1 < n0_size; j1++) {
                        auto data_addr = &(src[make_coord(make_coord(i1, i0), make_coord(j1, j0))]);
                        if constexpr (sizeof(src_type) == 1) {
                            std::cout << static_cast<uint32_t>(*(reinterpret_cast<uint8_t*>(data_addr))) << "\t";
                        } else if constexpr (Std::is_same_v<src_type, half>) {
                            std::cout << *(reinterpret_cast<uint16_t*>(data_addr)) << "\t";
                        } else {
                            std::cout << *data_addr << "\t";
                        }
                    }
                    std::cout << "|";
                }
                std::cout << std::endl;
            }
            std::cout << "-----------------------------------------" << std::endl;
        }
    }
}

inline void __print_type_hierarchy(const std::string& type_str)
{
    int indent_level = 1;
    const int indent_spaces = 4; // 每层缩进的空格数
    for (int s = 0; s < indent_level * indent_spaces; ++s)
        std::cout << " ";
    for (size_t i = 0; i < type_str.size(); ++i) {
        char c = type_str[i];
        if (c == '<') {
            // 遇到 <，换行并增加缩进
            std::cout << c << "\n";
            indent_level++;
            // 打印缩进
            for (int s = 0; s < indent_level * indent_spaces; ++s)
                std::cout << " ";
        } else if (c == ',' && indent_level > 0) {
            // 遇到逗号，换行并保持当前缩进
            std::cout << c << "\n";
            for (int s = 0; s < indent_level * indent_spaces - 1; ++s)
                std::cout << " ";
        } else if (c == '>') {
            // 遇到 >，先换行，减少缩进，再打印 >
            std::cout << "\n";
            indent_level--;
            for (int s = 0; s < indent_level * indent_spaces; ++s)
                std::cout << " ";
            std::cout << c;
        } else {
            // 普通字符直接打印
            std::cout << c;
        }
    }
    std::cout << std::endl;
}

template <typename T, typename... arg_types>
inline void print_type_hierarchy(const arg_types&... args)
{
    if constexpr (!std::is_same_v<T, void>) {
        std::cout << "type Hierarchy for: ";
    }
    ((std::cout << args << " "), ...);
    std::cout << std::endl;
    if constexpr (std::is_same_v<T, void>) {
        return;
    }
    std::string raw_name = typeid(T).name();
    int status = -4;
    char* res = abi::__cxa_demangle(raw_name.c_str(), NULL, NULL, &status);
    std::string ret = (status == 0) ? res : raw_name;
    if (status == 0)
        std::free(res);
    __print_type_hierarchy(ret);
}

// Sim gm2l1 copy by cpu
template <typename T, typename U>
void sim_nd_to_nd(const T& dst, const U& src)
{
    using dst_layout_ptn = get_layout_pattern<typename T::layout_type>;
    using src_layout_pattern = get_layout_pattern<typename U::layout_type>;
    static_assert(Std::is_same_v<dst_layout_ptn, nd_ext_layout_ptn> || Std::is_same_v<dst_layout_ptn, nd_layout_ptn>);
    static_assert(Std::is_same_v<src_layout_pattern, nd_ext_layout_ptn> || Std::is_same_v<src_layout_pattern, nd_layout_ptn>);
    using src_type = typename U::element_type;
    static_assert(std::is_same_v<src_type, typename T::element_type>, "src and dst element types must be the same");
    auto dst_layout = dst.layout();
    auto src_layout = src.layout();
    uint32_t m_value = get_sim_layout_element<attr_info::shape, attr_info::row, 1>(src_layout);
    uint32_t n_value = get_sim_layout_element<attr_info::shape, attr_info::column, 1>(src_layout);

    auto src_row_stride = get_sim_layout_element<attr_info::stride, attr_info::row, 1>(src_layout);
    auto src_col_stride = get_sim_layout_element<attr_info::stride, attr_info::column, 1>(src_layout);
    auto dst_row_stride = get_sim_layout_element<attr_info::stride, attr_info::row, 1>(dst_layout);

    uint32_t c0_elements = C0_SIZE<src_type> / sizeof(src_type);
    uint32_t m1 = get_sim_layout_element<attr_info::shape, attr_info::row, 1>(dst_layout);
    uint32_t n1_size = get_sim_layout_element<attr_info::shape, attr_info::column, 1>(dst_layout);

    uint32_t data_len = m_value * n_value;
    uint32_t align_n = (data_len + c0_elements - 1) / c0_elements * c0_elements;
    if (m_value == 1 || n_value == 1 || (n_value == n1_size && src_row_stride == n_value && dst_row_stride == n1_size)) {
        // if src is already in row major or column major format, treat it as m_value=1 or n_value=1 to simplify the copy
        for (uint32_t i = 0; i < align_n; i++) {
            if (i < data_len) {
                dst.data()[i] = src.data()[i];
            } else {
                // padding with 0 if out of bound
                dst.data()[i] = static_cast<src_type>(0);
            }
        }
        return;
    }
    EXPECT_TRUE((n1_size == n_value) || (dst_row_stride % c0_elements == 0))
        << "When dst column is greater than src column, dst column stride must be aligned to c0_value, but got dst_row_stride: "
        << dst_row_stride << ", c0_elements: " << c0_elements << ", n_value: " << n_value << ", n1_size: " << n1_size;
    uint32_t src_col_n_align_c0 = ((n_value + c0_elements - 1) / c0_elements) * c0_elements;
    for (uint32_t m1 = 0; m1 < m1; m1++) {
        for (uint32_t n1 = 0; n1 < n1_size; n1++) {
            uint32_t dst_index = m1 * n1_size + n1;
            if (m1 < m_value && n1 < n_value) {
                dst.data()[dst_index] = src.data()[m1 * src_row_stride + n1 * src_col_stride];
            } else if (m1 < m_value && n1 >= n_value && n1 < src_col_n_align_c0) {
                // padding with 0 if out of bound
                dst.data()[dst_index] = static_cast<src_type>(0);
            }
        }
    }
    // when col same, use compact mode, 32B align pad 0
    if (n_value == n1_size) {
        for (uint32_t i = data_len; i < align_n; i++) {
            dst.data()[i] = static_cast<src_type>(0);
        }
    }
}

template <typename T, typename U>
void sim_nd_to_nz(const T& dst, const U& src)
{
    using dst_layout_ptn = get_layout_pattern<typename T::layout_type>;
    using src_layout_pattern = get_layout_pattern<typename U::layout_type>;
    static_assert(Std::is_same_v<dst_layout_ptn, nz_layout_ptn>);
    static_assert(Std::is_same_v<src_layout_pattern, nd_ext_layout_ptn> || Std::is_same_v<src_layout_pattern, nd_layout_ptn>);
    using src_type = typename U::element_type;
    static_assert(std::is_same_v<src_type, typename T::element_type>, "src and dst element types must be the same");
    auto dst_layout = dst.layout();
    auto src_layout = src.layout();
    auto m_value = get_sim_layout_element<attr_info::shape, attr_info::row, 1>(src_layout);
    auto n_value = get_sim_layout_element<attr_info::shape, attr_info::column, 1>(src_layout);
    auto src_sm1 = get_sim_layout_element<attr_info::stride, attr_info::row, 1>(src_layout);
    auto src_sn1 = get_sim_layout_element<attr_info::stride, attr_info::column, 1>(src_layout);

    if (is_b4_type<src_type>) {
        EXPECT_TRUE(n_value % 2 == 0) << "For b4 type, col shape must be even for nd format, but got n_value: " << n_value;
        EXPECT_TRUE(src_sm1 % 2 == 0) << "For b4 type, col stride must be even for nd format, but got src_sm1: "
                                     << src_sm1;
    }
    uint32_t c0_elements = C0_ELEMENT<src_type>;
    uint32_t m0_size = get_element<attr_info::shape, attr_info::row, 0>(dst_layout);
    uint32_t n0_size = get_element<attr_info::shape, attr_info::column, 0>(dst_layout);
    uint32_t m1 = get_element<attr_info::shape, attr_info::row, 1>(dst_layout);
    uint32_t n1_size = get_element<attr_info::shape, attr_info::column, 1>(dst_layout);
    uint32_t src_col_n_align_c0 = ((n_value + c0_elements - 1) / c0_elements) * c0_elements;
    for (uint32_t n1 = 0; n1 < n1_size; n1++) {
        for (uint32_t m1 = 0; m1 < m1; m1++) {
            for (uint32_t m0 = 0; m0 < m0_size; m0++) {
                for (uint32_t n0 = 0; n0 < n0_size; n0++) {
                    uint32_t src_row = m1 * m0_size + m0;
                    uint32_t src_col = n1 * n0_size + n0;
                    uint32_t src_index = src_row * src_sm1 + src_col * src_sn1;
                    uint32_t dst_index = ((n1 * m1 + m1) * m0_size + m0) * n0_size + n0;
                    if (src_row < m_value && src_col < n_value) {
                        dst.data()[dst_index] = src.data()[src_index];
                    } else if (src_row < m_value && src_col >= n_value && src_col < src_col_n_align_c0) {
                        // right padding and bottom not padding, right pad to the next c0_value boundary
                        dst.data()[dst_index] = static_cast<src_type>(0);
                    }
                }
            }
        }
    }
}

template <typename T, typename U>
void sim_nd_to_zn(const T& dst, const U& src)
{
    using dst_layout_ptn = get_layout_pattern<typename T::layout_type>;
    using src_layout_pattern = get_layout_pattern<typename U::layout_type>;
    static_assert(Std::is_same_v<dst_layout_ptn, zn_layout_ptn>);
    static_assert(Std::is_same_v<src_layout_pattern, nd_ext_layout_ptn> || Std::is_same_v<src_layout_pattern, nd_layout_ptn>);
    using src_type = typename U::element_type;
    static_assert(std::is_same_v<src_type, typename T::element_type>, "src and dst element types must be the same");
    auto dst_layout = dst.layout();
    auto src_layout = src.layout();
    auto m_value = get_sim_layout_element<attr_info::shape, attr_info::row, 1>(src_layout);
    auto n_value = get_sim_layout_element<attr_info::shape, attr_info::column, 1>(src_layout);

    auto src_col_stride = get_sim_layout_element<attr_info::stride, attr_info::column, 1>(src_layout);
    auto src_row_stride = get_sim_layout_element<attr_info::stride, attr_info::row, 1>(src_layout);

    uint32_t c0_elements = C0_SIZE<src_type> / sizeof(src_type);
    uint32_t m0_size = get_element<attr_info::shape, attr_info::row, 0>(dst_layout);
    uint32_t n0_size = get_element<attr_info::shape, attr_info::column, 0>(dst_layout);
    uint32_t m1 = get_element<attr_info::shape, attr_info::row, 1>(dst_layout);
    uint32_t n1_size = get_element<attr_info::shape, attr_info::column, 1>(dst_layout);

    uint32_t src_row_n_align_c0 = ((m_value + c0_elements - 1) / c0_elements) * c0_elements;
    for (uint32_t m1 = 0; m1 < m1; m1++) {
        for (uint32_t n1 = 0; n1 < n1_size; n1++) {
            for (uint32_t n0 = 0; n0 < n0_size; n0++) {
                for (uint32_t m0 = 0; m0 < m0_size; m0++) {
                    uint32_t src_row = m1 * m0_size + m0;
                    uint32_t src_col = n1 * n0_size + n0;
                    uint32_t dst_index = ((m1 * n1_size + n1) * n0_size + n0) * m0_size + m0;

                    if (src_col < n_value && src_row < m_value) {
                        dst.data()[dst_index] = src.data()[src_col + src_row * src_row_stride];
                    } else if (src_col < n_value && src_row >= m_value && src_row < src_row_n_align_c0) {
                        // bottom padding and right not padding, bottom pad to the next c0_value boundary
                        dst.data()[dst_index] = static_cast<src_type>(0);
                    }
                }
            }
        }
    }
}

template <typename T, typename U>
void sim_dn_to_nz(const T& dst, const U& src)
{
    using dst_layout_ptn = get_layout_pattern<typename T::layout_type>;
    using src_layout_pattern = get_layout_pattern<typename U::layout_type>;
    static_assert(Std::is_same_v<dst_layout_ptn, nz_layout_ptn>);
    static_assert(Std::is_same_v<src_layout_pattern, dn_ext_layout_ptn> || Std::is_same_v<src_layout_pattern, dn_layout_ptn>);
    using src_type = typename U::element_type;
    static_assert(!is_b4_type<src_type>, "DN2NZ does not support b4 type");
    static_assert(std::is_same_v<src_type, typename T::element_type>, "src and dst element types must be the same");
    auto dst_layout = dst.layout();
    auto src_layout = src.layout();
    auto m_value = get_sim_layout_element<attr_info::shape, attr_info::row, 1>(src_layout);
    auto n_value = get_sim_layout_element<attr_info::shape, attr_info::column, 1>(src_layout);

    auto src_col_stride = get_sim_layout_element<attr_info::stride, attr_info::column, 1>(src_layout);

    uint32_t c0_elements = C0_SIZE<src_type> / sizeof(src_type);
    uint32_t m0_size = get_element<attr_info::shape, attr_info::row, 0>(dst_layout);
    uint32_t n0_size = get_element<attr_info::shape, attr_info::column, 0>(dst_layout);
    uint32_t m1 = get_element<attr_info::shape, attr_info::row, 1>(dst_layout);
    uint32_t n1_size = get_element<attr_info::shape, attr_info::column, 1>(dst_layout);
    for (uint32_t n1 = 0; n1 < n1_size; n1++) {
        for (uint32_t m1 = 0; m1 < m1; m1++) {
            for (uint32_t m0 = 0; m0 < m0_size; m0++) {
                for (uint32_t n0 = 0; n0 < n0_size; n0++) {
                    uint32_t src_row = m1 * m0_size + m0;
                    uint32_t src_col = n1 * n0_size + n0;
                    uint32_t dst_index = ((n1 * m1 + m1) * m0_size + m0) * n0_size + n0;
                    uint32_t src_col_n_align_c0 = ((n_value + c0_elements - 1) / c0_elements) * c0_elements;
                    if (src_row < m_value && src_col < n_value) {
                        dst.data()[dst_index] = src.data()[src_row + src_col * src_col_stride];
                    } else if (src_row < m_value && src_col >= n_value && src_col < src_col_n_align_c0) {
                        // right padding and bottom not padding, right pad to the next c0_value boundary
                        dst.data()[dst_index] = static_cast<src_type>(0);
                    }
                }
            }
        }
    }
}

template <typename T, typename U>
void sim_dn_to_zn(const T& dst, const U& src)
{
    using dst_layout_ptn = get_layout_pattern<typename T::layout_type>;
    using src_layout_pattern = get_layout_pattern<typename U::layout_type>;
    static_assert(Std::is_same_v<dst_layout_ptn, zn_layout_ptn>);
    static_assert(Std::is_same_v<src_layout_pattern, dn_ext_layout_ptn> || Std::is_same_v<src_layout_pattern, dn_layout_ptn>);
    using src_type = typename U::element_type;
    static_assert(std::is_same_v<src_type, typename T::element_type>, "src and dst element types must be the same");
    auto dst_layout = dst.layout();
    auto src_layout = src.layout();
    auto m_value = get_sim_layout_element<attr_info::shape, attr_info::row, 1>(src_layout);
    auto n_value = get_sim_layout_element<attr_info::shape, attr_info::column, 1>(src_layout);

    auto src_col_stride = get_sim_layout_element<attr_info::stride, attr_info::column, 1>(src_layout);
    if (is_b4_type<src_type>) {
        EXPECT_TRUE(m_value % 2 == 0) << "For b4 type, col shape must be even for nd format, but got m_value: " << m_value;
        EXPECT_TRUE(src_col_stride % 2 == 0)
            << "For b4 type, col stride must be even for nd format, but got src_col_stride: " << src_col_stride;
    }

    uint32_t c0_elements = C0_SIZE<src_type> / sizeof(src_type);
    uint32_t m0_size = get_element<attr_info::shape, attr_info::row, 0>(dst_layout);
    uint32_t n0_size = get_element<attr_info::shape, attr_info::column, 0>(dst_layout);
    uint32_t m1 = get_element<attr_info::shape, attr_info::row, 1>(dst_layout);
    uint32_t n1_size = get_element<attr_info::shape, attr_info::column, 1>(dst_layout);

    for (uint32_t m1 = 0; m1 < m1; m1++) {
        for (uint32_t n1 = 0; n1 < n1_size; n1++) {
            for (uint32_t n0 = 0; n0 < n0_size; n0++) {
                for (uint32_t m0 = 0; m0 < m0_size; m0++) {
                    uint32_t src_row = m1 * m0_size + m0;
                    uint32_t src_col = n1 * n0_size + n0;
                    uint32_t dst_index = ((m1 * n1_size + n1) * n0_size + n0) * m0_size + m0;
                    uint32_t src_row_n_align_c0 = ((m_value + c0_elements - 1) / c0_elements) * c0_elements;
                    if (src_col < n_value && src_row < m_value) {
                        dst.data()[dst_index] = src.data()[src_col * src_col_stride + src_row];
                    } else if (src_col < n_value && src_row >= m_value && src_row < src_row_n_align_c0) {
                        // bottom padding and right not padding, bottom pad to the next c0_value boundary
                        dst.data()[dst_index] = static_cast<src_type>(0);
                    }
                }
            }
        }
    }
}

template <typename T, typename U>
void sim_nz_to_nz(const T& dst, const U& src)
{
    using dst_layout_ptn = get_layout_pattern<typename T::layout_type>;
    using src_layout_pattern = get_layout_pattern<typename U::layout_type>;
    static_assert(Std::is_same_v<dst_layout_ptn, nz_layout_ptn>);
    static_assert(Std::is_same_v<src_layout_pattern, nz_layout_ptn>);
    using src_type = typename U::element_type;
    static_assert(std::is_same_v<src_type, typename T::element_type>, "src and dst element types must be the same");
    auto dst_layout = dst.layout();
    auto src_layout = src.layout();
    auto src_m1 = get_element<attr_info::shape, attr_info::row, 1>(src_layout);
    auto src_n1 = get_element<attr_info::shape, attr_info::column, 1>(src_layout);
    auto src_sm1 = get_element<attr_info::stride, attr_info::row, 1>(src_layout);
    auto src_sn1 = get_element<attr_info::stride, attr_info::column, 1>(src_layout);
    auto src_sm0 = get_element<attr_info::stride, attr_info::row, 0>(src_layout);
    auto src_sn0 = get_element<attr_info::stride, attr_info::column, 0>(src_layout);

    uint32_t m0_size = get_element<attr_info::shape, attr_info::row, 0>(dst_layout);
    uint32_t n0_size = get_element<attr_info::shape, attr_info::column, 0>(dst_layout);
    uint32_t m1 = get_element<attr_info::shape, attr_info::row, 1>(dst_layout);
    uint32_t n1_size = get_element<attr_info::shape, attr_info::column, 1>(dst_layout);

    auto dst_sm1 = get_element<attr_info::stride, attr_info::row, 1>(dst_layout);
    auto dst_sn1 = get_element<attr_info::stride, attr_info::column, 1>(dst_layout);
    auto dst_sm0 = get_element<attr_info::stride, attr_info::row, 0>(dst_layout);
    auto dst_sn0 = get_element<attr_info::stride, attr_info::column, 0>(dst_layout);

    for (uint32_t n1 = 0; n1 < n1_size; n1++) {
        for (uint32_t m1 = 0; m1 < m1; m1++) {
            for (uint32_t m0 = 0; m0 < m0_size; m0++) {
                for (uint32_t n0 = 0; n0 < n0_size; n0++) {
                    uint32_t src_index = n1 * src_sn1 + m1 * src_sm1 + n0 * src_sn0 + m0 * src_sm0;
                    uint32_t dst_index = n1 * dst_sn1 + m1 * dst_sm1 + n0 * dst_sn0 + m0 * dst_sm0;
                    if (m1 < src_m1 && n1 < src_n1) {
                        dst.data()[dst_index] = src.data()[src_index];
                    }
                    // no pad
                }
            }
        }
    }
}

template <typename T, typename U>
void sim_zn_to_zn(const T& dst, const U& src)
{
    using dst_layout_ptn = get_layout_pattern<typename T::layout_type>;
    using src_layout_pattern = get_layout_pattern<typename U::layout_type>;
    static_assert(Std::is_same_v<dst_layout_ptn, zn_layout_ptn>);
    static_assert(Std::is_same_v<src_layout_pattern, zn_layout_ptn>);
    using src_type = typename U::element_type;
    static_assert(std::is_same_v<src_type, typename T::element_type>, "src and dst element types must be the same");
    auto dst_layout = dst.layout();
    auto src_layout = src.layout();
    auto src_m1 = get_element<attr_info::shape, attr_info::row, 1>(src_layout);
    auto src_n1 = get_element<attr_info::shape, attr_info::column, 1>(src_layout);
    auto src_sm1 = get_element<attr_info::stride, attr_info::row, 1>(src_layout);
    auto src_sn1 = get_element<attr_info::stride, attr_info::column, 1>(src_layout);
    auto src_sm0 = get_element<attr_info::stride, attr_info::row, 0>(src_layout);
    auto src_sn0 = get_element<attr_info::stride, attr_info::column, 0>(src_layout);

    uint32_t m0_size = get_element<attr_info::shape, attr_info::row, 0>(dst_layout);
    uint32_t n0_size = get_element<attr_info::shape, attr_info::column, 0>(dst_layout);
    uint32_t m1 = get_element<attr_info::shape, attr_info::row, 1>(dst_layout);
    uint32_t n1_size = get_element<attr_info::shape, attr_info::column, 1>(dst_layout);

    auto dst_sm1 = get_element<attr_info::stride, attr_info::row, 1>(dst_layout);
    auto dst_sn1 = get_element<attr_info::stride, attr_info::column, 1>(dst_layout);
    auto dst_sm0 = get_element<attr_info::stride, attr_info::row, 0>(dst_layout);
    auto dst_sn0 = get_element<attr_info::stride, attr_info::column, 0>(dst_layout);

    for (uint32_t m1 = 0; m1 < m1; m1++) {
        for (uint32_t n1 = 0; n1 < n1_size; n1++) {
            for (uint32_t n0 = 0; n0 < n0_size; n0++) {
                for (uint32_t m0 = 0; m0 < m0_size; m0++) {
                    uint32_t src_index = m1 * src_sm1 + n1 * src_sn1 + m0 * src_sm0 + n0 * src_sn0;
                    uint32_t dst_index = m1 * dst_sm1 + n1 * dst_sn1 + m0 * dst_sm0 + n0 * dst_sn0;
                    if (m1 < src_m1 && n1 < src_n1) {
                        dst.data()[dst_index] = src.data()[src_index];
                    }
                    // no pad
                }
            }
        }
    }
}

template <typename T, typename U>
void sim_scalea_nd_to_zz(const T& dst, const U& src)
{
    using dst_layout_ptn = get_layout_pattern<typename T::layout_type>;
    using src_layout_pattern = get_layout_pattern<typename U::layout_type>;
    static_assert(Std::is_same_v<dst_layout_ptn, zz_layout_ptn>);
    static_assert(Std::is_same_v<src_layout_pattern, scalea_nd_layout_ptn>);
    using src_type = typename U::element_type;
    static_assert(std::is_same_v<src_type, typename T::element_type>, "src and dst element types must be the same");
    auto dst_layout = dst.layout();
    auto src_layout = src.layout();
    auto m_value = get_element<attr_info::shape, attr_info::row, 1>(src_layout);
    auto n_value = get_element<attr_info::shape, attr_info::column, 1>(src_layout);

    auto src_row_stride = get_element<attr_info::stride, attr_info::row, 1>(src_layout);

    uint32_t m0_size = get_element<attr_info::shape, attr_info::row, 0>(dst_layout);
    uint32_t n0_size = get_element<attr_info::shape, attr_info::column, 0>(dst_layout);
    uint32_t m1 = get_element<attr_info::shape, attr_info::row, 1>(dst_layout);
    uint32_t n1_size = get_element<attr_info::shape, attr_info::column, 1>(dst_layout);

    uint32_t c0_elements = C0_ELEMENT<half>; // sim by b16
    uint32_t src_row_n_align_c0 = ((m_value + c0_elements - 1) / c0_elements) * c0_elements;
    for (uint32_t m1 = 0; m1 < m1; m1++) {
        for (uint32_t n1 = 0; n1 < n1_size; n1++) {
            for (uint32_t m0 = 0; m0 < m0_size; m0++) {
                for (uint32_t n0 = 0; n0 < n0_size; n0++) {
                    uint32_t src_row = m1 * m0_size + m0;
                    uint32_t src_col = n1 * n0_size + n0;
                    uint32_t dst_index = ((m1 * n1_size + n1) * m0_size + m0) * n0_size + n0;
                    if (src_row < m_value && src_col < n_value) {
                        dst.data()[dst_index] = src.data()[src_row * src_row_stride + src_col];
                    } else if (src_col < n_value && src_row >= m_value && src_row < src_row_n_align_c0) {
                        // bottom padding and right not padding, bottom pad to the next c0_value boundary
                        // use dn2nz way to pad, which means padding in the raw row direction, actual col direction
                        dst.data()[dst_index] = static_cast<src_type>(0);
                    }
                }
            }
        }
    }
}

template <typename T, typename U>
void sim_scalea_dn_to_zz(const T& dst, const U& src)
{
    using dst_layout_ptn = get_layout_pattern<typename T::layout_type>;
    using src_layout_pattern = get_layout_pattern<typename U::layout_type>;
    static_assert(Std::is_same_v<dst_layout_ptn, zz_layout_ptn>);
    static_assert(Std::is_same_v<src_layout_pattern, scalea_dn_layout_ptn>);
    using src_type = typename U::element_type;
    static_assert(std::is_same_v<src_type, typename T::element_type>, "src and dst element types must be the same");
    auto dst_layout = dst.layout();
    auto src_layout = src.layout();
    auto m_value = get_element<attr_info::shape, attr_info::row, 1>(src_layout);
    auto s_n = get_element<attr_info::shape, attr_info::column, 0>(src_layout);
    auto b_n = get_element<attr_info::shape, attr_info::column, 1>(src_layout);
    auto n_value = s_n * b_n;

    auto src_b_col_stride = get_element<attr_info::stride, attr_info::column, 1>(src_layout);

    uint32_t m0_size = get_element<attr_info::shape, attr_info::row, 0>(dst_layout);
    uint32_t n0_size = get_element<attr_info::shape, attr_info::column, 0>(dst_layout);
    uint32_t m1 = get_element<attr_info::shape, attr_info::row, 1>(dst_layout);
    uint32_t n1_size = get_element<attr_info::shape, attr_info::column, 1>(dst_layout);

    uint32_t c0_elements = C0_ELEMENT<half>; // sim by b16
    uint32_t src_row_n_align_c0 = ((m_value + c0_elements - 1) / c0_elements) * c0_elements;
    for (uint32_t m1 = 0; m1 < m1; m1++) {
        for (uint32_t n1 = 0; n1 < n1_size; n1++) {
            for (uint32_t m0 = 0; m0 < m0_size; m0++) {
                for (uint32_t n0 = 0; n0 < n0_size; n0++) {
                    uint32_t src_row = m1 * m0_size + m0;
                    uint32_t src_col = n1 * n0_size + n0;
                    uint32_t dst_index = ((m1 * n1_size + n1) * m0_size + m0) * n0_size + n0;
                    if (src_row < m_value && src_col < n_value) {
                        dst.data()[dst_index] = src.data()[n1 * src_b_col_stride + src_row * 2 + n0];
                    } else if (src_col < n_value && src_row >= m_value && src_row < src_row_n_align_c0) {
                        // bottom padding and right not padding, bottom pad to the next c0_value boundary
                        // use dn2nz way to pad, which means padding in the raw row direction, actual col direction
                        dst.data()[dst_index] = static_cast<src_type>(0);
                    }
                }
            }
        }
    }
}

template <typename T, typename U>
void sim_scalea_zz_to_zz(const T& dst, const U& src)
{
    using dst_layout_ptn = get_layout_pattern<typename T::layout_type>;
    using src_layout_pattern = get_layout_pattern<typename U::layout_type>;
    static_assert(Std::is_same_v<dst_layout_ptn, zz_layout_ptn>);
    static_assert(Std::is_same_v<src_layout_pattern, zz_layout_ptn>);
    using src_type = typename U::element_type;
    static_assert(std::is_same_v<src_type, typename T::element_type>, "src and dst element types must be the same");
    auto dst_layout = dst.layout();
    auto src_layout = src.layout();
    auto src_m1 = get_element<attr_info::shape, attr_info::row, 1>(src_layout);
    auto src_n1 = get_element<attr_info::shape, attr_info::column, 1>(src_layout);
    auto src_sm1 = get_element<attr_info::stride, attr_info::row, 1>(src_layout);
    auto src_sn1 = get_element<attr_info::stride, attr_info::column, 1>(src_layout);
    auto src_sm0 = get_element<attr_info::stride, attr_info::row, 0>(src_layout);
    auto src_sn0 = get_element<attr_info::stride, attr_info::column, 0>(src_layout);

    uint32_t m0_size = get_element<attr_info::shape, attr_info::row, 0>(dst_layout);
    uint32_t n0_size = get_element<attr_info::shape, attr_info::column, 0>(dst_layout);
    uint32_t m1 = get_element<attr_info::shape, attr_info::row, 1>(dst_layout);
    uint32_t n1_size = get_element<attr_info::shape, attr_info::column, 1>(dst_layout);

    for (uint32_t m1 = 0; m1 < m1; m1++) {
        for (uint32_t n1 = 0; n1 < n1_size; n1++) {
            for (uint32_t m0 = 0; m0 < m0_size; m0++) {
                for (uint32_t n0 = 0; n0 < n0_size; n0++) {
                    uint32_t src_index = m1 * src_sm1 + n1 * src_sn1 + m0 * src_sm0 + n0 * src_sn0;
                    uint32_t dst_index = ((m1 * n1_size + n1) * m0_size + m0) * n0_size + n0;
                    if (m1 < src_m1 && n1 < src_n1) {
                        dst.data()[dst_index] = src.data()[src_index];
                    }
                    // no pad
                }
            }
        }
    }
}

template <typename T, typename U>
void sim_scaleb_nd_to_nn(const T& dst, const U& src)
{
    using dst_layout_ptn = get_layout_pattern<typename T::layout_type>;
    using src_layout_pattern = get_layout_pattern<typename U::layout_type>;
    static_assert(Std::is_same_v<dst_layout_ptn, nn_layout_ptn>);
    static_assert(Std::is_same_v<src_layout_pattern, scaleb_nd_layout_ptn>);
    using src_type = typename U::element_type;
    static_assert(std::is_same_v<src_type, typename T::element_type>, "src and dst element types must be the same");
    auto dst_layout = dst.layout();
    auto src_layout = src.layout();
    auto s_m = get_element<attr_info::shape, attr_info::row, 0>(src_layout);
    auto b_m = get_element<attr_info::shape, attr_info::row, 1>(src_layout);
    auto s_n = get_element<attr_info::shape, attr_info::column, 0>(src_layout);
    auto b_n = get_element<attr_info::shape, attr_info::column, 1>(src_layout);
    auto src_sm1 = get_element<attr_info::stride, attr_info::row, 1>(src_layout);
    auto src_sn1 = get_element<attr_info::stride, attr_info::column, 1>(src_layout);

    uint32_t m0_size = get_element<attr_info::shape, attr_info::row, 0>(dst_layout);
    uint32_t n0_size = get_element<attr_info::shape, attr_info::column, 0>(dst_layout);
    uint32_t m1 = get_element<attr_info::shape, attr_info::row, 1>(dst_layout);
    uint32_t n1_size = get_element<attr_info::shape, attr_info::column, 1>(dst_layout);

    uint32_t c0_elements = C0_ELEMENT<half>; // sim by b16
    uint32_t src_col_n_align_c0 = ((s_n * b_n + c0_elements - 1) / c0_elements) * c0_elements;
    for (uint32_t n1 = 0; n1 < n1_size; n1++) {
        for (uint32_t m1 = 0; m1 < m1; m1++) {
            for (uint32_t n0 = 0; n0 < n0_size; n0++) {
                for (uint32_t m0 = 0; m0 < m0_size; m0++) {
                    uint32_t src_row = m1 * m0_size + m0;
                    uint32_t src_col = n1 * n0_size + n0;
                    // m1 n1_size n0_size m0_size
                    uint32_t src_index = m1 * src_sm1 + src_col * src_sn1 + m0;
                    // n1_size m1 n0_size m0_size
                    uint32_t dst_index = ((n1 * m1 + m1) * n0_size + n0) * m0_size + m0;
                    if (src_row < s_m * b_m && src_col < s_n * b_n) {
                        dst.data()[dst_index] = src.data()[src_index];
                    } else if (src_row < s_m * b_m && src_col >= s_n * b_n && src_col < src_col_n_align_c0) {
                        // right padding and bottom not padding, right pad to the next batch_value*n_value boundary
                        dst.data()[dst_index] = static_cast<src_type>(0);
                    }
                }
            }
        }
    }
}

template <typename T, typename U>
void sim_scaleb_dn_to_nn(const T& dst, const U& src)
{
    using dst_layout_ptn = get_layout_pattern<typename T::layout_type>;
    using src_layout_pattern = get_layout_pattern<typename U::layout_type>;
    static_assert(Std::is_same_v<dst_layout_ptn, nn_layout_ptn>);
    static_assert(Std::is_same_v<src_layout_pattern, scaleb_dn_layout_ptn>);
    using src_type = typename U::element_type;
    static_assert(std::is_same_v<src_type, typename T::element_type>, "src and dst element types must be the same");
    auto dst_layout = dst.layout();
    auto src_layout = src.layout();
    uint32_t s_m = get_element<attr_info::shape, attr_info::row, 0>(src_layout);
    uint32_t b_m = get_element<attr_info::shape, attr_info::row, 1>(src_layout);
    uint32_t s_n = get_element<attr_info::shape, attr_info::column, 0>(src_layout);
    uint32_t b_n = get_element<attr_info::shape, attr_info::column, 1>(src_layout);
    uint32_t src_sm1 = get_element<attr_info::stride, attr_info::row, 1>(src_layout);
    uint32_t src_sn1 = get_element<attr_info::stride, attr_info::column, 1>(src_layout);

    uint32_t m0_size = get_element<attr_info::shape, attr_info::row, 0>(dst_layout);
    uint32_t n0_size = get_element<attr_info::shape, attr_info::column, 0>(dst_layout);
    uint32_t m1 = get_element<attr_info::shape, attr_info::row, 1>(dst_layout);
    uint32_t n1_size = get_element<attr_info::shape, attr_info::column, 1>(dst_layout);

    uint32_t c0_elements = C0_ELEMENT<half>; // sim by b16
    uint32_t src_col_n_align_c0 = ((s_n * b_n + c0_elements - 1) / c0_elements) * c0_elements;
    for (uint32_t n1 = 0; n1 < n1_size; n1++) {
        for (uint32_t m1 = 0; m1 < m1; m1++) {
            for (uint32_t m0 = 0; m0 < m0_size; m0++) {
                for (uint32_t n0 = 0; n0 < n0_size; n0++) {
                    uint32_t src_row = m1 * m0_size + m0;
                    uint32_t src_col = n1 * n0_size + n0;
                    // m1 n1_size m0_size n0_size
                    uint32_t src_index = src_col * src_sn1 + src_row * src_sm1;
                    // n1_size m1 n0_size m0_size
                    uint32_t dst_index = ((n1 * m1 + m1) * n0_size + n0) * m0_size + m0;
                    if (src_row < s_m * b_m && src_col < s_n * b_n) {
                        dst.data()[dst_index] = src.data()[src_index];
                    } else if (src_row < s_m * b_m && src_col >= s_n * b_n && src_col < src_col_n_align_c0) {
                        // right padding and bottom not padding, right pad to the next batch_value*n_value boundary
                        dst.data()[dst_index] = static_cast<src_type>(0);
                    }
                }
            }
        }
    }
}

template <typename T, typename U>
void sim_scaleb_nn_to_nn(const T& dst, const U& src)
{
    using dst_layout_ptn = get_layout_pattern<typename T::layout_type>;
    using src_layout_pattern = get_layout_pattern<typename U::layout_type>;
    static_assert(Std::is_same_v<dst_layout_ptn, nn_layout_ptn>);
    static_assert(Std::is_same_v<src_layout_pattern, nn_layout_ptn>);
    using src_type = typename U::element_type;
    static_assert(std::is_same_v<src_type, typename T::element_type>, "src and dst element types must be the same");
    auto dst_layout = dst.layout();
    auto src_layout = src.layout();
    uint32_t s_m = get_element<attr_info::shape, attr_info::row, 0>(src_layout);
    uint32_t b_m = get_element<attr_info::shape, attr_info::row, 1>(src_layout);
    uint32_t s_n = get_element<attr_info::shape, attr_info::column, 0>(src_layout);
    uint32_t b_n = get_element<attr_info::shape, attr_info::column, 1>(src_layout);
    uint32_t src_sm = get_element<attr_info::stride, attr_info::row, 0>(src_layout);
    uint32_t src_sn = get_element<attr_info::stride, attr_info::column, 0>(src_layout);
    uint32_t src_bm = get_element<attr_info::stride, attr_info::row, 1>(src_layout);
    uint32_t src_bn = get_element<attr_info::stride, attr_info::column, 1>(src_layout);

    uint32_t m0_size = get_element<attr_info::shape, attr_info::row, 0>(dst_layout);
    uint32_t n0_size = get_element<attr_info::shape, attr_info::column, 0>(dst_layout);
    uint32_t m1 = get_element<attr_info::shape, attr_info::row, 1>(dst_layout);
    uint32_t n1_size = get_element<attr_info::shape, attr_info::column, 1>(dst_layout);

    uint32_t c0_elements = C0_ELEMENT<half>; // sim by b16
    uint32_t src_col_n_align_c0 = ((s_n * b_n + c0_elements - 1) / c0_elements) * c0_elements;

    for (uint32_t n1 = 0; n1 < n1_size; n1++) {
        for (uint32_t m1 = 0; m1 < m1; m1++) {
            for (uint32_t m0 = 0; m0 < m0_size; m0++) {
                for (uint32_t n0 = 0; n0 < n0_size; n0++) {
                    uint32_t src_row = m1 * m0_size + m0;
                    uint32_t src_col = n1 * n0_size + n0;
                    uint32_t src_index = n1 * src_bn + m1 * src_bm + n0 * src_sn + m0 * src_sm;
                    uint32_t dst_index = ((n1 * m1 + m1) * n0_size + n0) * m0_size + m0;
                    if (src_row < s_m * b_m && src_col < s_n * b_n) {
                        dst.data()[dst_index] = src.data()[src_index];
                    } else if (src_row < s_m * b_m && src_col >= s_n * b_n && src_col < src_col_n_align_c0) {
                        // right padding and bottom not padding, right pad to the next batch_value*n_value boundary
                        dst.data()[dst_index] = static_cast<src_type>(0);
                    }
                }
            }
        }
    }
}

template <typename T, typename U>
void data_copy_gm_to_l1_sim(const T& dst, const U& src)
{
    using src_type = typename U::element_type;
    static_assert(std::is_same_v<src_type, typename T::element_type>, "src and dst element types must be the same");
    using dst_layout_ptn = get_layout_pattern<typename T::layout_type>;
    using src_layout_pattern = get_layout_pattern<typename U::layout_type>;

    if constexpr ((Std::is_same_v<dst_layout_ptn, nd_ext_layout_ptn> || Std::is_same_v<dst_layout_ptn, nd_layout_ptn>) &&
                  (Std::is_same_v<src_layout_pattern, nd_ext_layout_ptn> || Std::is_same_v<src_layout_pattern, nd_layout_ptn>)) {
        sim_nd_to_nd(dst, src);
    } else if constexpr (Std::is_same_v<dst_layout_ptn, nz_layout_ptn> &&
                         (Std::is_same_v<src_layout_pattern, nd_ext_layout_ptn> || Std::is_same_v<src_layout_pattern, nd_layout_ptn>)) {
        sim_nd_to_nz(dst, src);
    } else if constexpr (Std::is_same_v<dst_layout_ptn, zn_layout_ptn> &&
                         (Std::is_same_v<src_layout_pattern, nd_ext_layout_ptn> || Std::is_same_v<src_layout_pattern, nd_layout_ptn>)) {
        sim_nd_to_zn(dst, src);
    } else if constexpr (Std::is_same_v<dst_layout_ptn, nz_layout_ptn> &&
                         (Std::is_same_v<src_layout_pattern, dn_ext_layout_ptn> || Std::is_same_v<src_layout_pattern, dn_layout_ptn>)) {
        sim_dn_to_nz(dst, src);
    } else if constexpr (Std::is_same_v<dst_layout_ptn, zn_layout_ptn> &&
                         (Std::is_same_v<src_layout_pattern, dn_ext_layout_ptn> || Std::is_same_v<src_layout_pattern, dn_layout_ptn>)) {
        sim_dn_to_zn(dst, src);
    } else if constexpr (Std::is_same_v<dst_layout_ptn, nz_layout_ptn> && Std::is_same_v<src_layout_pattern, nz_layout_ptn>) {
        sim_nz_to_nz(dst, src);
    } else if constexpr (Std::is_same_v<dst_layout_ptn, zn_layout_ptn> && Std::is_same_v<src_layout_pattern, zn_layout_ptn>) {
        sim_zn_to_zn(dst, src);
    } else if constexpr (Std::is_same_v<dst_layout_ptn, zz_layout_ptn> && Std::is_same_v<src_layout_pattern, scalea_nd_layout_ptn>) {
        sim_scalea_nd_to_zz(dst, src);
    } else if constexpr (Std::is_same_v<dst_layout_ptn, zz_layout_ptn> && Std::is_same_v<src_layout_pattern, scalea_dn_layout_ptn>) {
        sim_scalea_dn_to_zz(dst, src);
    } else if constexpr (Std::is_same_v<dst_layout_ptn, zz_layout_ptn> && Std::is_same_v<src_layout_pattern, zz_layout_ptn>) {
        sim_scalea_zz_to_zz(dst, src);
    } else if constexpr (Std::is_same_v<dst_layout_ptn, nn_layout_ptn> && Std::is_same_v<src_layout_pattern, scaleb_nd_layout_ptn>) {
        sim_scaleb_nd_to_nn(dst, src);
    } else if constexpr (Std::is_same_v<dst_layout_ptn, nn_layout_ptn> && Std::is_same_v<src_layout_pattern, scaleb_dn_layout_ptn>) {
        sim_scaleb_dn_to_nn(dst, src);
    } else if constexpr (Std::is_same_v<dst_layout_ptn, nn_layout_ptn> && Std::is_same_v<src_layout_pattern, nn_layout_ptn>) {
        sim_scaleb_nn_to_nn(dst, src);
    } else {
        // assert error
        static_assert(Std::is_same_v<T, U>, "The data format is not supported.");
    }
}

template <typename T, typename U, typename coord_type>
void data_copy_gm_to_l1_sim(const T& dst, const U& src, const coord_type& coord)
{
    // auto slice_tensor = src(coord, dst);
    // data_copy_gm_to_l1_sim(dst, slice_tensor);
}

// Simulate hardware instruction.
struct copy_gm_to_l1_align_v2_capture {
    void* dst = nullptr;
    void* src = nullptr;
    uint32_t block_count = 0;
    uint32_t block_len = 0;
    uint8_t left_padding_cnt = 0;
    uint8_t right_padding_cnt = 0;
    bool data_select_bit = false;
    uint8_t l2_cache_ctl = 0;
    uint64_t src_stride = 0;
    uint32_t dst_stride = 0;
};

struct copy_gm_to_l1_nd_to_nz_capture {
    void* dst = nullptr;
    void* src = nullptr;
    uint64_t loop1_src_stride = 0;
    uint16_t n_value = 0;
    uint32_t d_value = 0;
    uint64_t loop4_src_stride = 0;
    bool enable_small_c0 = false;
};

struct copy_gm_to_l1_dn_to_nz_capture {
    void* dst = nullptr;
    void* src = nullptr;
    uint64_t loop1_src_stride = 0;
    uint16_t n_value = 0;
    uint32_t d_value = 0;
    uint64_t loop4_src_stride = 0;
    bool enable_small_c0 = false;
};

struct copy_gm_to_l1_nz_para_capture {
    union {
        struct {
            uint16_t nd_num;          // MTE2_NZ_PARA[15:0]
            uint16_t loop2_dst_stride; // MTE2_NZ_PARA[31:16]
            uint16_t loop3_dst_stride; // MTE2_NZ_PARA[47:32]
            uint16_t loop4_dst_stride; // MTE2_NZ_PARA[63:48]
        };
        uint64_t mte2_nz_para;
    };
};

// Global capture object
std::vector<copy_gm_to_l1_align_v2_capture> g_gm_to_l1_align_v2_captures;
std::vector<copy_gm_to_l1_nd_to_nz_capture> g_gm_to_l1_nd_to_nz_captures;
std::vector<copy_gm_to_l1_dn_to_nz_capture> g_gm_to_l1_dn_to_nz_captures;
std::vector<copy_gm_to_l1_nz_para_capture> g_gm_to_l1_nz_para_captures;

// Reset capture data
void reset_capture()
{
    g_gm_to_l1_align_v2_captures.clear();
    g_gm_to_l1_nd_to_nz_captures.clear();
    g_gm_to_l1_dn_to_nz_captures.clear();
    g_gm_to_l1_nz_para_captures.clear();
}

void print_capture_data()
{
    for (const auto& capture : g_gm_to_l1_align_v2_captures) {
        std::cout << "data_copy Capture - dst: " << capture.dst << ", src: " << capture.src
                  << ", block_count: " << capture.block_count << ", block_len: " << capture.block_len
                  << ", left_padding_cnt: " << static_cast<int>(capture.left_padding_cnt)
                  << ", right_padding_cnt: " << static_cast<int>(capture.right_padding_cnt)
                  << ", l2_cache_ctl: " << static_cast<int>(capture.l2_cache_ctl) << ", src_stride: " << capture.src_stride
                  << ", dst_stride: " << capture.dst_stride << std::endl;
    }

    for (const auto& capture : g_gm_to_l1_nd_to_nz_captures) {
        std::cout << "CopyGmToCbufMultiND2nz Capture - dst: " << capture.dst << ", src: " << capture.src
                  << ", loop1_src_stride: " << capture.loop1_src_stride << ", n_value: " << capture.n_value
                  << ", d_value: " << capture.d_value << ", loop4_src_stride: " << capture.loop4_src_stride
                  << ", enable_small_c0: " << std::boolalpha << capture.enable_small_c0 << std::endl;
    }

    for (const auto& capture : g_gm_to_l1_dn_to_nz_captures) {
        std::cout << "CopyGmToCbufMultiDN2nz Capture - dst: " << capture.dst << ", src: " << capture.src
                  << ", loop1_src_stride: " << capture.loop1_src_stride << ", n_value: " << capture.n_value
                  << ", d_value: " << capture.d_value << ", loop4_src_stride: " << capture.loop4_src_stride
                  << ", enable_small_c0: " << std::boolalpha << capture.enable_small_c0 << std::endl;
    }

    for (const auto& capture : g_gm_to_l1_nz_para_captures) {
        std::cout << "set_mte2_nz_para Capture - mte2_nz_para: " << capture.mte2_nz_para << ", nd_num: " << capture.nd_num
                  << ", loop2_dst_stride: " << capture.loop2_dst_stride << ", loop3_dst_stride: " << capture.loop3_dst_stride
                  << ", loop4_dst_stride: " << capture.loop4_dst_stride << std::endl;
    }
}

extern void copy_gm_to_cbuf_multi_nd2nz(half* dst, half* src, uint8_t sid, uint64_t loop1_src_stride,
                                        uint8_t l2_cache_ctl, uint16_t n_value, uint32_t d_value,
                                        uint64_t loop4_src_stride, bool smallc0_en);
template <typename T>
void simulate_nd_to_nz_data_copy(T* dst, T* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value,
                           uint64_t loop4_src_stride, bool enable_small_c0)
{
    if (g_gm_to_l1_nz_para_captures.empty()) {
        return;
    }
    uint16_t nd_num = g_gm_to_l1_nz_para_captures.back().nd_num;
    uint16_t loop2_dst_stride = g_gm_to_l1_nz_para_captures.back().loop2_dst_stride;
    uint16_t loop3_dst_stride = g_gm_to_l1_nz_para_captures.back().loop3_dst_stride;
    uint16_t loop4_dst_stride = g_gm_to_l1_nz_para_captures.back().loop4_dst_stride;
    constexpr uint32_t type_size = sizeof(T);
    uint32_t c0_elements = C0_SIZE<T> / type_size; // Number of elements in one c0_value block
    if (enable_small_c0) {
        for (int h = 0; h < nd_num; h++) {
            const uint8_t* src_nd_addr = reinterpret_cast<const uint8_t*>(src) + h * loop4_src_stride;
            uint8_t* dst_nd_addr = reinterpret_cast<uint8_t*>(dst) + h * loop4_dst_stride * C0_SIZE<T>;

            uint16_t n_ceil = (n_value + 3) / 4;
            for (int j = 0; j < n_ceil; j++) {
                const uint8_t* src_n_addr = (j < n_value) ? (src_nd_addr + j * loop1_src_stride) : nullptr;
                uint8_t* dst_n_addr = dst_nd_addr + j * 4 * type_size;
                for (int k = 0; k < 4; k++) {
                    uint8_t* dst_ele_addr = dst_n_addr + k * type_size;
                    if ((k < d_value) && (src_n_addr != nullptr)) {
                        const uint8_t* src_ele_addr = src_n_addr + k * type_size;
                        std::copy(src_ele_addr, src_ele_addr + type_size, dst_ele_addr);
                    } else {
                        std::fill(dst_ele_addr, dst_ele_addr + type_size, 0); // Padding with zeros
                    }
                }
            }
        }
    } else {
        uint32_t block_num = (d_value + c0_elements - 1) / c0_elements;
        for (int h = 0; h < nd_num; h++) {
            const uint8_t* src_nd_addr = reinterpret_cast<const uint8_t*>(src) + h * loop4_src_stride;
            uint8_t* dst_nd_addr = reinterpret_cast<uint8_t*>(dst) + h * loop4_dst_stride * C0_SIZE<T>;
            for (int i = 0; i < block_num; i++) {
                const uint8_t* src_block_addr = src_nd_addr + i * C0_SIZE<T>;
                uint8_t* dst_block_addr = dst_nd_addr + i * loop3_dst_stride * C0_SIZE<T>;

                for (int j = 0; j < n_value; j++) {
                    const uint8_t* src_n_addr = src_block_addr + j * loop1_src_stride;
                    uint8_t* dst_n_addr = dst_block_addr + j * loop2_dst_stride * C0_SIZE<T>;
                    for (int k = 0; k < c0_elements; k++) {
                        uint32_t src_ele_index = i * c0_elements + k;
                        uint8_t* dst_ele_addr = dst_n_addr + k * type_size;
                        if (src_ele_index < d_value) {
                            const uint8_t* src_ele_addr = src_n_addr + k * type_size;
                            std::copy(src_ele_addr, src_ele_addr + type_size, dst_ele_addr);
                        } else {
                            std::fill(dst_ele_addr, dst_ele_addr + type_size, 0); // Padding with zeros
                        }
                    }
                }
            }
        }
    }
}

extern void copy_gm_to_cbuf_multi_dn2nz(half* dst, half* src, uint8_t sid, uint64_t loop1_src_stride,
                                        uint8_t l2_cache_ctl, uint16_t n_value, uint32_t d_value,
                                        uint64_t loop4_src_stride, bool smallc0_en);
template <typename T>
void simulate_dn_to_nz_data_copy(T* dst, T* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value,
                           uint64_t loop4_src_stride, bool enable_small_c0)
{
    if (g_gm_to_l1_nz_para_captures.empty()) {
        return;
    }
    uint16_t dn_num = g_gm_to_l1_nz_para_captures.back().nd_num;
    uint16_t loop2_dst_stride = g_gm_to_l1_nz_para_captures.back().loop2_dst_stride;
    uint16_t loop3_dst_stride = g_gm_to_l1_nz_para_captures.back().loop3_dst_stride;
    uint16_t loop4_dst_stride = g_gm_to_l1_nz_para_captures.back().loop4_dst_stride;
    constexpr uint32_t type_size = sizeof(T);
    uint32_t c0_elements = C0_SIZE<T> / type_size; // Number of elements in one c0_value block
    if (enable_small_c0) {
        for (int h = 0; h < dn_num; h++) {
            const uint8_t* src_dn_addr = reinterpret_cast<const uint8_t*>(src) + h * loop4_src_stride;
            uint8_t* dst_dn_addr = reinterpret_cast<uint8_t*>(dst) + h * loop4_dst_stride * C0_SIZE<T>;

            uint16_t n_ceil = (n_value + 3) / 4;
            for (int j = 0; j < n_ceil; j++) {
                const uint8_t* src_n_addr = (j < n_value) ? (src_dn_addr + j * type_size) : nullptr;
                uint8_t* dst_n_addr = dst_dn_addr + j * 4 * type_size;
                for (int k = 0; k < 4; k++) {
                    uint8_t* dst_ele_addr = dst_n_addr + k * type_size;
                    if ((k < d_value) && (src_n_addr != nullptr)) {
                        const uint8_t* src_ele_addr = src_n_addr + k * loop1_src_stride;
                        std::copy(src_ele_addr, src_ele_addr + type_size, dst_ele_addr);
                    } else {
                        std::fill(dst_ele_addr, dst_ele_addr + type_size, 0); // Padding with zeros
                    }
                }
            }
        }
    } else {
        uint32_t block_num = (d_value + c0_elements - 1) / c0_elements;
        for (int h = 0; h < dn_num; h++) {
            const uint8_t* src_dn_addr = reinterpret_cast<const uint8_t*>(src) + h * loop4_src_stride;
            uint8_t* dst_dn_addr = reinterpret_cast<uint8_t*>(dst) + h * loop4_dst_stride * C0_SIZE<T>;
            for (int i = 0; i < block_num; i++) {
                const uint8_t* src_block_addr = src_dn_addr + i * loop1_src_stride * c0_elements;
                uint8_t* dst_block_addr = dst_dn_addr + i * loop3_dst_stride * C0_SIZE<T>;

                for (int j = 0; j < n_value; j++) {
                    const uint8_t* src_n_addr = src_block_addr + j * type_size;
                    uint8_t* dst_n_addr = dst_block_addr + j * loop2_dst_stride * C0_SIZE<T>;
                    for (int k = 0; k < c0_elements; k++) {
                        uint32_t src_ele_index = i * c0_elements + k;
                        uint8_t* dst_ele_addr = dst_n_addr + k * type_size;
                        if (src_ele_index < d_value) {
                            const uint8_t* src_ele_addr = src_n_addr + k * loop1_src_stride;
                            std::copy(src_ele_addr, src_ele_addr + type_size, dst_ele_addr);
                        } else {
                            std::fill(dst_ele_addr, dst_ele_addr + type_size, 0); // Padding with zeros
                        }
                    }
                }
            }
        }
    }
}

extern void copy_gm_to_cbuf_align_v2(half* dst, half* src, uint8_t sid, uint32_t burst_num, uint32_t burst_len,
                                     uint8_t left_padding_count, uint8_t right_padding_count, bool data_select_bit,
                                     uint8_t l2_cache_ctl, uint64_t burst_src_stride, uint32_t burst_dst_stride);
template <typename T>
void simulate_align_v2_data_copy(T* dst, T* src, uint32_t block_count, uint32_t block_len, uint8_t left_padding_cnt,
                             uint8_t right_padding_cnt, bool data_select_bit, uint64_t src_stride, uint32_t dst_stride)
{
    bool is_lprp_mode = (left_padding_cnt > 0) || (right_padding_cnt > 0);
    bool is_compact_mode = (dst_stride == block_len);
    uint32_t total_burst_size = block_len + left_padding_cnt * sizeof(T) + right_padding_cnt * sizeof(T);
    uint32_t pad_size = (total_burst_size % C0_SIZE<T> == 0) ? 0 : (C0_SIZE<T> - (total_burst_size % C0_SIZE<T>));
    uint32_t pad_elem = pad_size / sizeof(T);
    // compact mode, left and right pad cnt is zero, dst_stride equals block_len, can directly copy without padding
    if (is_lprp_mode) {
        // In LPRP mode, dst_stride should be aligned to c0_value size
        EXPECT_TRUE(dst_stride % C0_SIZE<T> == 0);
        for (uint32_t block_id = 0; block_id < block_count; block_id++) {
            uint8_t* src_burst = reinterpret_cast<uint8_t*>(src) + block_id * src_stride;
            uint8_t* dst_burst = reinterpret_cast<uint8_t*>(dst) + block_id * dst_stride;

            if (left_padding_cnt > 0) {
                std::fill(dst_burst, dst_burst + left_padding_cnt * sizeof(T), 0); // Padding with zeros
            }
            std::copy(src_burst, src_burst + block_len, dst_burst + left_padding_cnt * sizeof(T));

            uint32_t right_pad_offset = left_padding_cnt * sizeof(T) + block_len;
            if (right_padding_cnt > 0) {
                std::fill(dst_burst + right_pad_offset, dst_burst + right_pad_offset + right_padding_cnt * sizeof(T),
                          0); // Padding with zeros
            }

            uint32_t pad_offset = left_padding_cnt * sizeof(T) + block_len + right_padding_cnt * sizeof(T);
            if (pad_elem > 0) {
                std::fill(dst_burst + pad_offset, dst_burst + pad_offset + pad_elem * sizeof(T), 0); // Padding with zeros
            }
        }
        return;
    }
    if (is_compact_mode) {
        uint8_t* src_base = reinterpret_cast<uint8_t*>(src);
        uint8_t* dst_base = reinterpret_cast<uint8_t*>(dst);
        for (uint32_t block_id = 0; block_id < block_count; block_id++) {
            const uint8_t* src_burst = src_base + block_id * src_stride;
            uint8_t* dst_burst = dst_base + block_id * dst_stride;
            std::copy(src_burst, src_burst + block_len, dst_burst);
        }
        // check tail padding
        uint32_t total_data_len = block_count * block_len;
        uint64_t alignd_size = ((total_data_len + C0_SIZE<T> - 1) / C0_SIZE<T>)*C0_SIZE<T>;
        if (alignd_size > total_data_len) {
            uint8_t* pad_start = dst_base + total_data_len;
            std::fill(pad_start, pad_start + (alignd_size - total_data_len), 0); // Padding with zeros
        }
    } else {
        // normal mode
        for (uint32_t block_id = 0; block_id < block_count; block_id++) {
            uint8_t* src_burst = reinterpret_cast<uint8_t*>(src) + block_id * src_stride;
            uint8_t* dst_burst = reinterpret_cast<uint8_t*>(dst) + block_id * dst_stride;
            std::copy(src_burst, src_burst + block_len, dst_burst);
            if (pad_elem > 0) {
                uint8_t* pad_start = dst_burst + block_len;
                std::fill(pad_start, pad_start + pad_elem * sizeof(T), 0); // Padding with zeros
            }
        }
    }
}

extern void set_mte2_nz_para(uint64_t para);
void capture_set_mte2_nz_para(uint64_t para)
{
    copy_gm_to_l1_nz_para_capture capture;
    capture.mte2_nz_para = para;
    g_gm_to_l1_nz_para_captures.push_back(capture);
}

#define CAPTURE_GM_TO_L1_IMPL(type)                                                                                    \
    void capture_copy_gm_to_cbuf_align_v2##type(__cbuf__ type* dst, __gm__ type* src, uint8_t sid, uint32_t block_count,     \
                                           uint32_t block_len, uint8_t left_padding_cnt, uint8_t right_padding_cnt,         \
                                           bool data_select_bit, uint8_t l2_cache_ctl, uint64_t src_stride,                 \
                                           uint32_t dst_stride)                                                         \
    {                                                                                                                  \
        copy_gm_to_l1_align_v2_capture capture;                                                                               \
        capture.dst = reinterpret_cast<void*>(dst);                                                                    \
        capture.src = reinterpret_cast<void*>(src);                                                                    \
        capture.block_count = block_count;                                                                               \
        capture.block_len = block_len;                                                                                   \
        capture.left_padding_cnt = left_padding_cnt;                                                                       \
        capture.right_padding_cnt = right_padding_cnt;                                                                     \
        capture.data_select_bit = data_select_bit;                                                                         \
        capture.l2_cache_ctl = l2_cache_ctl;                                                                               \
        capture.src_stride = src_stride;                                                                                 \
        capture.dst_stride = dst_stride;                                                                                 \
        g_gm_to_l1_align_v2_captures.push_back(capture);                                                                      \
        simulate_align_v2_data_copy(dst, src, block_count, block_len, left_padding_cnt, right_padding_cnt, data_select_bit,        \
                                src_stride, dst_stride);                                                                 \
    }                                                                                                                  \
    void capture_copy_gm_to_cbuf_multi_nd_to_nz##type(__cbuf__ type* dst, __gm__ type* src, uint8_t sid,                       \
                                              uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,       \
                                              uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)            \
    {                                                                                                                  \
        copy_gm_to_l1_nd_to_nz_capture capture;                                                                                 \
        capture.dst = reinterpret_cast<void*>(dst);                                                                    \
        capture.src = reinterpret_cast<void*>(src);                                                                    \
        capture.loop1_src_stride = loop1_src_stride;                                                                     \
        capture.n_value = n_value;                                                                                      \
        capture.d_value = d_value;                                                                                      \
        capture.loop4_src_stride = loop4_src_stride;                                                                     \
        capture.enable_small_c0 = smallc0_en;                                                                            \
        g_gm_to_l1_nd_to_nz_captures.push_back(capture);                                                                        \
        simulate_nd_to_nz_data_copy(dst, src, loop1_src_stride, n_value, d_value, loop4_src_stride, smallc0_en);             \
    }                                                                                                                  \
    void capture_copy_gm_to_cbuf_multi_dn_to_nz##type(__cbuf__ type* dst, __gm__ type* src, uint8_t sid,                       \
                                              uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,       \
                                              uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)            \
    {                                                                                                                  \
        copy_gm_to_l1_dn_to_nz_capture capture;                                                                                 \
        capture.dst = reinterpret_cast<void*>(dst);                                                                    \
        capture.src = reinterpret_cast<void*>(src);                                                                    \
        capture.loop1_src_stride = loop1_src_stride;                                                                     \
        capture.n_value = n_value;                                                                                      \
        capture.d_value = d_value;                                                                                      \
        capture.loop4_src_stride = loop4_src_stride;                                                                     \
        capture.enable_small_c0 = smallc0_en;                                                                            \
        g_gm_to_l1_dn_to_nz_captures.push_back(capture);                                                                        \
        simulate_dn_to_nz_data_copy(dst, src, loop1_src_stride, n_value, d_value, loop4_src_stride, smallc0_en);             \
    }

CAPTURE_GM_TO_L1_IMPL(uint8_t);
CAPTURE_GM_TO_L1_IMPL(half);
CAPTURE_GM_TO_L1_IMPL(uint16_t);
CAPTURE_GM_TO_L1_IMPL(float);
CAPTURE_GM_TO_L1_IMPL(uint32_t);

#define RUN_GM2L1_SCALE_BATCH_COPY(type, batch, gm_base_layout_expr, l1_base_layout_expr)                                   \
    using T = type;                                                                                                   \
    constexpr uint32_t batch_value = batch;                                                                                     \
    auto gm_base_layout = (gm_base_layout_expr);                                                                           \
    auto l1_base_layout = (l1_base_layout_expr);                                                                           \
    using gm_trait = get_layout_trait<decltype(gm_base_layout)>;                                                          \
    using l1_trait = get_layout_trait<decltype(l1_base_layout)>;                                                          \
    using gm_pattern = get_layout_pattern<decltype(gm_base_layout)>;                                                       \
    using l1_pattern = get_layout_pattern<decltype(l1_base_layout)>;                                                       \
    auto gm_batch_layout = make_batch_pattern_layout<gm_pattern, gm_trait>(batch_value, gm_base_layout);                                 \
    auto l1_batch_layout = make_batch_pattern_layout<l1_pattern, l1_trait>(batch_value, l1_base_layout);                                 \
    auto gm_a = make_tensor(make_mem_ptr<location::gm>(reinterpret_cast<T*>(src0_gm)), gm_batch_layout);                     \
    auto l1a_tensor = make_tensor(make_mem_ptr<location::l1>(reinterpret_cast<T*>(l1a_buf)), l1_batch_layout);               \
    initialize_data<T>();                                                                                              \
    make_copy(copy_gm_to_l1{}, copy_gm_to_l1_trait_default{}).call(l1a_tensor, gm_a);                                              \
    auto src_batch_stride = get<0>(gm_batch_layout.stride());                                                             \
    auto dst_batch_stride = get<0>(l1_batch_layout.stride());                                                             \
    for (uint32_t batch_idx = 0; batch_idx < batch_value; ++batch_idx) {                                                           \
        auto gm_single = make_tensor(                                                                                   \
            make_mem_ptr<location::gm>(reinterpret_cast<T*>(src0_gm) + batch_idx * src_batch_stride), gm_base_layout);        \
        auto l1golden_single = make_tensor(                                                                             \
            make_mem_ptr<location::l1>(reinterpret_cast<T*>(l1a_buf_golden) + batch_idx * dst_batch_stride), l1_base_layout);  \
        data_copy_gm_to_l1_sim(l1golden_single, gm_single);                                                                   \
    }                                                                                                                 \
    bool result = std::equal(l1a_buf, l1a_buf + l1_size, l1a_buf_golden);                                                  \
    EXPECT_TRUE(result);                                                                                              \
    if (g_debug_print || !result) {                                                                                     \
        print_capture_data();                                                                                           \
    }

TEST_F(tensor_api_gm_to_l1, copy_gm_to_l1_batch_scalea_nd_to_zz_routes_to_single_dn_to_nz)
{
    using T = fp8_e8m0_t;

    constexpr uint32_t batch_value = 3;
    constexpr uint32_t m_value = 64;
    constexpr uint32_t n_value = 4;
    auto gm_base_layout = make_scalea_nd<T>(m_value, n_value);
    auto l1_base_layout = make_zz<T>(m_value, n_value);
    using gm_trait = get_layout_trait<decltype(gm_base_layout)>;
    using l1_trait = get_layout_trait<decltype(l1_base_layout)>;
    auto gm_batch_layout = make_batch_pattern_layout<scalea_nd_layout_ptn, gm_trait>(batch_value, gm_base_layout);
    auto l1_batch_layout = make_batch_pattern_layout<zz_layout_ptn, l1_trait>(batch_value, l1_base_layout);
    auto gm_a = make_tensor(make_mem_ptr<location::gm>(reinterpret_cast<T*>(src0_gm)), gm_batch_layout);
    auto l1a_tensor = make_tensor(make_mem_ptr<location::l1>(reinterpret_cast<T*>(l1a_buf)), l1_batch_layout);

    initialize_data<T>();
    make_copy(copy_gm_to_l1{}, copy_gm_to_l1_trait_default{}).call(l1a_tensor, gm_a);

    auto src_batch_stride = get<0>(gm_batch_layout.stride());
    auto dst_batch_stride = get<0>(l1_batch_layout.stride());
    for (uint32_t batch_idx = 0; batch_idx < batch_value; ++batch_idx) {
        auto gm_single = make_tensor(
            make_mem_ptr<location::gm>(reinterpret_cast<T*>(src0_gm) + batch_idx * src_batch_stride), gm_base_layout);
        auto l1golden_single = make_tensor(
            make_mem_ptr<location::l1>(reinterpret_cast<T*>(l1a_buf_golden) + batch_idx * dst_batch_stride), l1_base_layout);
        data_copy_gm_to_l1_sim(l1golden_single, gm_single);
    }
    bool result = std::equal(l1a_buf, l1a_buf + l1_size, l1a_buf_golden);
    EXPECT_TRUE(result);
    if (g_debug_print || !result) {
        print_capture_data();
    }

    ASSERT_EQ(g_gm_to_l1_dn_to_nz_captures.size(), 1);
    ASSERT_EQ(g_gm_to_l1_nz_para_captures.size(), 1);
    const auto& dn2nz = g_gm_to_l1_dn_to_nz_captures.back();
    const auto& nz_para = g_gm_to_l1_nz_para_captures.back();
    auto expected_loop3_dst_stride = get_element<attr_info::stride, attr_info::row, 1>(l1_base_layout) * sizeof(T) /
        C0_SIZE<>;
    auto expected_loop4_dst_stride = dst_batch_stride * sizeof(T) / C0_SIZE<>;
    auto expected_loop1_src_stride = get_element<attr_info::stride, attr_info::row, 1>(gm_base_layout) * sizeof(T);
    auto expected_loop4_src_stride = src_batch_stride * sizeof(T);
    EXPECT_EQ(nz_para.nd_num, batch_value);
    EXPECT_EQ(nz_para.loop2_dst_stride, 1);
    EXPECT_EQ(nz_para.loop3_dst_stride, expected_loop3_dst_stride);
    EXPECT_EQ(nz_para.loop4_dst_stride, expected_loop4_dst_stride);
    EXPECT_EQ(dn2nz.loop1_src_stride, expected_loop1_src_stride);
    EXPECT_EQ(dn2nz.n_value, n_value / 2);
    EXPECT_EQ(dn2nz.d_value, m_value);
    EXPECT_EQ(dn2nz.loop4_src_stride, expected_loop4_src_stride);
    EXPECT_FALSE(dn2nz.enable_small_c0);
}

TEST_F(tensor_api_gm_to_l1, copy_gm_to_l1_batch_scalea_dn_to_zz_routes_to_single_nd_to_nz)
{
    constexpr uint32_t m_value = 64;
    constexpr uint32_t n_value = 32;
    RUN_GM2L1_SCALE_BATCH_COPY(fp8_e8m0_t, 3, make_scalea_dn<T>(m_value, n_value), make_zz<T>(m_value, n_value));

    ASSERT_EQ(g_gm_to_l1_nd_to_nz_captures.size(), 1);
    ASSERT_EQ(g_gm_to_l1_nz_para_captures.size(), 1);
    const auto& nd2nz = g_gm_to_l1_nd_to_nz_captures.back();
    const auto& nz_para = g_gm_to_l1_nz_para_captures.back();
    auto expected_loop3_dst_stride = get_element<attr_info::stride, attr_info::row, 1>(l1_base_layout) * sizeof(T) /
        C0_SIZE<>;
    auto expected_loop4_dst_stride = dst_batch_stride * sizeof(T) / C0_SIZE<>;
    auto expected_loop1_src_stride = get_element<attr_info::stride, attr_info::column, 1>(gm_base_layout) * sizeof(T);
    auto expected_loop4_src_stride = src_batch_stride * sizeof(T);
    EXPECT_EQ(nz_para.nd_num, batch_value);
    EXPECT_EQ(nz_para.loop2_dst_stride, 1);
    EXPECT_EQ(nz_para.loop3_dst_stride, expected_loop3_dst_stride);
    EXPECT_EQ(nz_para.loop4_dst_stride, expected_loop4_dst_stride);
    EXPECT_EQ(nd2nz.loop1_src_stride, expected_loop1_src_stride);
    EXPECT_EQ(nd2nz.n_value, (get_element<attr_info::shape, attr_info::column, 1>(gm_base_layout)));
    EXPECT_EQ(nd2nz.d_value, (get_element<attr_info::shape, attr_info::row, 1>(gm_base_layout)));
    EXPECT_EQ(nd2nz.loop4_src_stride, expected_loop4_src_stride);
    EXPECT_FALSE(nd2nz.enable_small_c0);
}

TEST_F(tensor_api_gm_to_l1, copy_gm_to_l1_batch_scaleb_nd_to_nn_routes_to_single_nd_to_nz)
{
    constexpr uint32_t m_value = 64;
    constexpr uint32_t n_value = 32;
    RUN_GM2L1_SCALE_BATCH_COPY(fp8_e8m0_t, 3, make_scaleb_nd<T>(m_value, n_value), make_nn<T>(m_value, n_value));

    ASSERT_EQ(g_gm_to_l1_nd_to_nz_captures.size(), 1);
    ASSERT_EQ(g_gm_to_l1_nz_para_captures.size(), 1);
    const auto& nd2nz = g_gm_to_l1_nd_to_nz_captures.back();
    const auto& nz_para = g_gm_to_l1_nz_para_captures.back();
    auto expected_loop3_dst_stride = get_element<attr_info::stride, attr_info::column, 1>(l1_base_layout) * sizeof(T) /
        C0_SIZE<>;
    auto expected_loop4_dst_stride = dst_batch_stride * sizeof(T) / C0_SIZE<>;
    auto expected_loop1_src_stride = get_element<attr_info::stride, attr_info::row, 1>(gm_base_layout) * sizeof(T);
    auto expected_loop4_src_stride = src_batch_stride * sizeof(T);
    EXPECT_EQ(nz_para.nd_num, batch_value);
    EXPECT_EQ(nz_para.loop2_dst_stride, 1);
    EXPECT_EQ(nz_para.loop3_dst_stride, expected_loop3_dst_stride);
    EXPECT_EQ(nz_para.loop4_dst_stride, expected_loop4_dst_stride);
    EXPECT_EQ(nd2nz.loop1_src_stride, expected_loop1_src_stride);
    EXPECT_EQ(nd2nz.n_value, (get_element<attr_info::shape, attr_info::row, 1>(gm_base_layout)));
    EXPECT_EQ(nd2nz.d_value, (get_element<attr_info::shape, attr_info::column, 1>(gm_base_layout)));
    EXPECT_EQ(nd2nz.loop4_src_stride, expected_loop4_src_stride);
    EXPECT_FALSE(nd2nz.enable_small_c0);
}

TEST_F(tensor_api_gm_to_l1, copy_gm_to_l1_batch_scaleb_dn_to_nn_routes_to_single_dn_to_nz)
{
    constexpr uint32_t m_value = 64;
    constexpr uint32_t n_value = 32;
    RUN_GM2L1_SCALE_BATCH_COPY(fp8_e8m0_t, 3, make_scaleb_dn<T>(m_value, n_value), make_nn<T>(m_value, n_value));

    ASSERT_EQ(g_gm_to_l1_dn_to_nz_captures.size(), 1);
    ASSERT_EQ(g_gm_to_l1_nz_para_captures.size(), 1);
    const auto& dn2nz = g_gm_to_l1_dn_to_nz_captures.back();
    const auto& nz_para = g_gm_to_l1_nz_para_captures.back();
    auto expected_loop3_dst_stride = get_element<attr_info::stride, attr_info::column, 1>(l1_base_layout) * sizeof(T) /
        C0_SIZE<>;
    auto expected_loop4_dst_stride = dst_batch_stride * sizeof(T) / C0_SIZE<>;
    auto expected_loop1_src_stride = get_element<attr_info::stride, attr_info::column, 1>(gm_base_layout) * sizeof(T);
    auto expected_loop4_src_stride = src_batch_stride * sizeof(T);
    EXPECT_EQ(nz_para.nd_num, batch_value);
    EXPECT_EQ(nz_para.loop2_dst_stride, 1);
    EXPECT_EQ(nz_para.loop3_dst_stride, expected_loop3_dst_stride);
    EXPECT_EQ(nz_para.loop4_dst_stride, expected_loop4_dst_stride);
    EXPECT_EQ(dn2nz.loop1_src_stride, expected_loop1_src_stride);
    EXPECT_EQ(dn2nz.n_value, (get_element<attr_info::shape, attr_info::row, 1>(gm_base_layout) >> 1));
    EXPECT_EQ(dn2nz.d_value, (get_element<attr_info::shape, attr_info::column, 1>(gm_base_layout)));
    EXPECT_EQ(dn2nz.loop4_src_stride, expected_loop4_src_stride);
    EXPECT_FALSE(dn2nz.enable_small_c0);
}

TEST_F(tensor_api_gm_to_l1, copy_gm_to_l1_batch_scalea_zz_to_zz_continuous_routes_to_per_batch_align_v2)
{
    constexpr uint32_t m_value = 64;
    constexpr uint32_t n_value = 32;
    RUN_GM2L1_SCALE_BATCH_COPY(fp8_e8m0_t, 3, make_zz<T>(m_value, n_value), make_zz<T>(m_value, n_value));

    ASSERT_EQ(g_gm_to_l1_align_v2_captures.size(), batch_value);
    const auto& align_v2 = g_gm_to_l1_align_v2_captures.back();
    auto base_block_count = get_element<attr_info::shape, attr_info::row, 1>(gm_base_layout);
    auto base_block_len = get_element<attr_info::shape, attr_info::column, 1>(gm_base_layout) * sizeof(T) *
        get_element<attr_info::shape, attr_info::row, 0>(gm_base_layout) *
        get_element<attr_info::stride, attr_info::row, 0>(gm_base_layout);
    auto expected_src_stride = get_element<attr_info::stride, attr_info::row, 1>(gm_base_layout) * sizeof(T);
    auto expected_dst_stride = get_element<attr_info::stride, attr_info::row, 1>(l1_base_layout) * sizeof(T);
    EXPECT_EQ(align_v2.block_count, base_block_count);
    EXPECT_EQ(align_v2.block_len, base_block_len);
    EXPECT_EQ(align_v2.src_stride, expected_src_stride);
    EXPECT_EQ(align_v2.dst_stride, expected_dst_stride);
}

TEST_F(tensor_api_gm_to_l1, copy_gm_to_l1_batch_scalea_zz_to_zz_non_continuous_dst_falls_back_to_per_batch_align_v2)
{
    constexpr uint32_t m_value = 64;
    constexpr uint32_t n_value = 32;
    RUN_GM2L1_SCALE_BATCH_COPY(fp8_e8m0_t, 3, make_zz<T>(m_value, n_value), make_zz<T>(m_value + 16, n_value));

    ASSERT_EQ(g_gm_to_l1_align_v2_captures.size(), batch_value);
    const auto& align_v2 = g_gm_to_l1_align_v2_captures.back();
    auto base_block_count = get_element<attr_info::shape, attr_info::row, 1>(gm_base_layout);
    auto base_block_len = get_element<attr_info::shape, attr_info::column, 1>(gm_base_layout) * sizeof(T) *
        get_element<attr_info::shape, attr_info::row, 0>(gm_base_layout) *
        get_element<attr_info::stride, attr_info::row, 0>(gm_base_layout);
    EXPECT_EQ(align_v2.block_count, base_block_count);
    EXPECT_EQ(align_v2.block_len, base_block_len);
}

TEST_F(tensor_api_gm_to_l1, copy_gm_to_l1_batch_scaleb_nn_to_nn_continuous_routes_to_per_batch_align_v2)
{
    constexpr uint32_t m_value = 64;
    constexpr uint32_t n_value = 32;
    RUN_GM2L1_SCALE_BATCH_COPY(fp8_e8m0_t, 3, make_nn<T>(m_value, n_value), make_nn<T>(m_value, n_value));

    ASSERT_EQ(g_gm_to_l1_align_v2_captures.size(), batch_value);
    const auto& align_v2 = g_gm_to_l1_align_v2_captures.back();
    auto base_block_count = get_element<attr_info::shape, attr_info::column, 1>(gm_base_layout);
    auto base_block_len = get_element<attr_info::shape, attr_info::row, 1>(gm_base_layout) * sizeof(T) *
        get_element<attr_info::shape, attr_info::column, 0>(gm_base_layout) *
        get_element<attr_info::stride, attr_info::column, 0>(gm_base_layout);
    auto expected_src_stride = get_element<attr_info::stride, attr_info::column, 1>(gm_base_layout) * sizeof(T);
    auto expected_dst_stride = get_element<attr_info::stride, attr_info::column, 1>(l1_base_layout) * sizeof(T);
    EXPECT_EQ(align_v2.block_count, base_block_count);
    EXPECT_EQ(align_v2.block_len, base_block_len);
    EXPECT_EQ(align_v2.src_stride, expected_src_stride);
    EXPECT_EQ(align_v2.dst_stride, expected_dst_stride);
}

TEST_F(tensor_api_gm_to_l1, copy_gm_to_l1_batch_scaleb_nn_to_nn_non_continuous_dst_falls_back_to_per_batch_align_v2)
{
    constexpr uint32_t m_value = 64;
    constexpr uint32_t n_value = 32;
    RUN_GM2L1_SCALE_BATCH_COPY(fp8_e8m0_t, 3, make_nn<T>(m_value, n_value), make_nn<T>(m_value, n_value + 16));

    ASSERT_EQ(g_gm_to_l1_align_v2_captures.size(), batch_value);
    const auto& align_v2 = g_gm_to_l1_align_v2_captures.back();
    auto base_block_count = get_element<attr_info::shape, attr_info::column, 1>(gm_base_layout);
    auto base_block_len = get_element<attr_info::shape, attr_info::row, 1>(gm_base_layout) * sizeof(T) *
        get_element<attr_info::shape, attr_info::column, 0>(gm_base_layout) *
        get_element<attr_info::stride, attr_info::column, 0>(gm_base_layout);
    EXPECT_EQ(align_v2.block_count, base_block_count);
    EXPECT_EQ(align_v2.block_len, base_block_len);
}

// =========================================================================
// Batch nd_to_nd (gm_to_l1)
//
// layout (batch_value, (m_value, n_value)) with strides (s_b, (s_m, s_n)). The implementation only
// keeps the batched branch (no compact-fold):
//   block_count = batch_value
//   block_len   = m_value*n_value*sizeof(T)
//   src_stride  = s_b*sizeof(T)
//   dst_stride  = s_b*sizeof(T)
// make_batch_pattern_layout always emits s_b == m_value*n_value, so src_stride == dst_stride
// == m_value*n_value*sizeof(T).
// (make_batch_nd_ext is provided by the file-scope MAKE_BATCH_LAYOUT_FUNC macro above.)
// =========================================================================

#define EXPECT_GM2L1_BATCH_LAST_CALL(expect_block_count, expect_block_len, expect_src_stride, expect_dst_stride) \
    do {                                                                                                  \
        ASSERT_FALSE(g_gm_to_l1_align_v2_captures.empty());                                                      \
        const auto& last = g_gm_to_l1_align_v2_captures.back();                                                  \
        EXPECT_EQ(last.block_count, static_cast<uint32_t>(expect_block_count));                              \
        EXPECT_EQ(last.block_len, static_cast<uint32_t>(expect_block_len));                                  \
        EXPECT_EQ(last.src_stride, static_cast<uint64_t>(expect_src_stride));                                \
        EXPECT_EQ(last.dst_stride, static_cast<uint32_t>(expect_dst_stride));                                \
    } while (0)

TEST_F(tensor_api_gm_to_l1, copy_gm_to_l1_batch_nd_to_nd_compact_folds_to_single_block)
{
    using T = float;
    constexpr uint32_t batch_value = 4;
    constexpr uint32_t m_value = 8;
    constexpr uint32_t n_value = 16;
    auto gm_a = make_tensor(make_mem_ptr<location::gm>(reinterpret_cast<T*>(src0_gm)), make_batch_nd_ext<T>(batch_value, m_value, n_value));
    auto l1a = make_tensor(make_mem_ptr<location::l1>(reinterpret_cast<T*>(l1a_buf)), make_batch_nd_ext<T>(batch_value, m_value, n_value));

    make_copy(copy_gm_to_l1{}, copy_gm_to_l1_trait_default{}).call(l1a, gm_a);

    constexpr uint32_t expect_block_len = m_value * n_value * sizeof(T);
    constexpr uint64_t expect_stride = static_cast<uint64_t>(m_value) * n_value * sizeof(T);
    EXPECT_GM2L1_BATCH_LAST_CALL(batch_value, expect_block_len, expect_stride, expect_stride);
}

TEST_F(tensor_api_gm_to_l1, copy_gm_to_l1_batch_nd_to_nd_compact_half_type)
{
    using T = half;
    constexpr uint32_t batch_value = 2;
    constexpr uint32_t m_value = 16;
    constexpr uint32_t n_value = 32;
    auto gm_a = make_tensor(make_mem_ptr<location::gm>(reinterpret_cast<T*>(src0_gm)), make_batch_nd_ext<T>(batch_value, m_value, n_value));
    auto l1a = make_tensor(make_mem_ptr<location::l1>(reinterpret_cast<T*>(l1a_buf)), make_batch_nd_ext<T>(batch_value, m_value, n_value));

    make_copy(copy_gm_to_l1{}, copy_gm_to_l1_trait_default{}).call(l1a, gm_a);

    constexpr uint32_t expect_block_len = m_value * n_value * sizeof(T);
    constexpr uint64_t expect_stride = static_cast<uint64_t>(m_value) * n_value * sizeof(T);
    EXPECT_GM2L1_BATCH_LAST_CALL(batch_value, expect_block_len, expect_stride, expect_stride);
}

// gm(NHWC) -> l1(NC1HWC0) via the nd2nz DMA path. Verifies the captured DMA parameters:
// nd_num=height, n_value=width, d_value=channel; src strides read straight from the NHWC layout
// (loop1_src_stride=channel*sizeof, loop4_src_stride=width*channel*sizeof); dst strides loop2_dst_stride=1,
// loop3_dst_stride=height*width, loop4_dst_stride=width.
TEST_F(tensor_api_gm_to_l1, copy_gm_to_l1_nhwc_to_nc1hwc0)
{
    using T = half;
    constexpr uint32_t n_value = 1;
    constexpr uint32_t height = 4;
    constexpr uint32_t width = 4;
    constexpr uint32_t c0_value = 16;
    constexpr uint32_t channel = 32;
    constexpr uint32_t c1 = channel / c0_value; // 2

    auto gm_nhwc = make_frame_layout<nhwc_layout_ptn>(
        static_cast<int32_t>(n_value), static_cast<int32_t>(height), static_cast<int32_t>(width), static_cast<int32_t>(channel));
    auto l1_nc1hwc0 = make_frame_layout<nc1hwc0_layout_ptn>(
        static_cast<int32_t>(n_value), static_cast<int32_t>(c1), static_cast<int32_t>(height), static_cast<int32_t>(width),
        static_cast<int32_t>(c0_value));
    auto gm_a = make_tensor(make_mem_ptr<location::gm>(reinterpret_cast<T*>(src0_gm)), gm_nhwc);
    auto l1a = make_tensor(make_mem_ptr<location::l1>(reinterpret_cast<T*>(l1a_buf)), l1_nc1hwc0);

    make_copy(copy_gm_to_l1{}, copy_gm_to_l1_trait_default{}).call(l1a, gm_a);

    ASSERT_EQ(g_gm_to_l1_nd_to_nz_captures.size(), 1);
    ASSERT_EQ(g_gm_to_l1_nz_para_captures.size(), 1);
    const auto& nd2nz = g_gm_to_l1_nd_to_nz_captures.back();
    const auto& nz_para = g_gm_to_l1_nz_para_captures.back();

    EXPECT_EQ(nz_para.nd_num, height);                         // nd_num = src_h
    EXPECT_EQ(nd2nz.n_value, width);                         // n_value = src_w
    EXPECT_EQ(nd2nz.d_value, channel);                         // d_value = src_c
    EXPECT_EQ(nd2nz.loop1_src_stride, channel * sizeof(T));     // src_c * sizeof(type)
    EXPECT_EQ(nd2nz.loop4_src_stride, width * channel * sizeof(T)); // src_w * src_c * sizeof(type)
    EXPECT_EQ(nz_para.loop2_dst_stride, 1);
    EXPECT_EQ(nz_para.loop3_dst_stride, height * width); // dst_h * dst_w
    EXPECT_EQ(nz_para.loop4_dst_stride, width);     // dst_w
    EXPECT_FALSE(nd2nz.enable_small_c0);

    copy(make_copy(copy_gm_to_l1{}, copy_gm_to_l1_trait_default{}), l1a, gm_a,
        zero_coord, make_coord(0, 0, 0, 0), make_shape(n_value, height, width, channel));
}

// gm(NCHW) -> l1(NC1HWC0) via the dn2nz DMA path (NCHW is the HW<->channel transpose of NHWC). Verifies
// the captured DMA parameters: dn_num=height, n_value=width, d_value=channel; src strides read straight from the NCHW
// layout (loop1_src_stride=height*width*sizeof=stride[1]*sizeof, loop4_src_stride=width*sizeof=stride[2]*sizeof);
// dst strides loop2_dst_stride=1, loop3_dst_stride=height*width, loop4_dst_stride=width.
TEST_F(tensor_api_gm_to_l1, copy_gm_to_l1_nchw_to_nc1hwc0)
{
    using T = half;
    constexpr uint32_t n_value = 1;
    constexpr uint32_t height = 4;
    constexpr uint32_t width = 4;
    constexpr uint32_t c0_value = 16;
    constexpr uint32_t channel = 32;
    constexpr uint32_t c1 = channel / c0_value; // 2

    auto gm_nchw = make_frame_layout<nchw_layout_ptn>(
        static_cast<int32_t>(n_value), static_cast<int32_t>(channel), static_cast<int32_t>(height), static_cast<int32_t>(width));
    auto l1_nc1hwc0 = make_frame_layout<nc1hwc0_layout_ptn>(
        static_cast<int32_t>(n_value), static_cast<int32_t>(c1), static_cast<int32_t>(height), static_cast<int32_t>(width),
        static_cast<int32_t>(c0_value));
    auto gm_a = make_tensor(make_mem_ptr<location::gm>(reinterpret_cast<T*>(src0_gm)), gm_nchw);
    auto l1a = make_tensor(make_mem_ptr<location::l1>(reinterpret_cast<T*>(l1a_buf)), l1_nc1hwc0);

    make_copy(copy_gm_to_l1{}, copy_gm_to_l1_trait_default{}).call(l1a, gm_a);

    ASSERT_EQ(g_gm_to_l1_dn_to_nz_captures.size(), 1);
    ASSERT_EQ(g_gm_to_l1_nz_para_captures.size(), 1);
    const auto& dn2nz = g_gm_to_l1_dn_to_nz_captures.back();
    const auto& nz_para = g_gm_to_l1_nz_para_captures.back();

    EXPECT_EQ(nz_para.nd_num, height);                         // dn_num = src_h
    EXPECT_EQ(dn2nz.n_value, width);                         // n_value = src_w
    EXPECT_EQ(dn2nz.d_value, channel);                         // d_value = src_c
    EXPECT_EQ(dn2nz.loop1_src_stride, height * width * sizeof(T)); // src_h*src_w*sizeof (NCHW stride[1]=height*width)
    EXPECT_EQ(dn2nz.loop4_src_stride, width * sizeof(T));     // src_w*sizeof (NCHW stride[2]=width)
    EXPECT_EQ(nz_para.loop2_dst_stride, 1);
    EXPECT_EQ(nz_para.loop3_dst_stride, height * width); // dst_h * dst_w
    EXPECT_EQ(nz_para.loop4_dst_stride, width);     // dst_w
    EXPECT_FALSE(dn2nz.enable_small_c0);

    copy(make_copy(copy_gm_to_l1{}, copy_gm_to_l1_trait_default{}), l1a, gm_a,
        zero_coord, make_coord(0, 0, 0, 0), make_shape(n_value, channel, height, width));
}

// gm(NCDHW) -> l1(NDC1HWC0): conv3d adds a depth axis D; each depth slice is one NCHW->NC1HWC0
// (dn2nz). Verifies there is one DMA per depth slice, that the per-slice DMA params match the
// conv2d NCHW->NC1HWC0 mapping (dn_num=height, n_value=width, d_value=channel; loop1_src_stride=stride[1]=D*height*width here),
// and that each slice's src/dst pointers advance by the depth stride (src height*width, dst c1*height*width*c0_value).
TEST_F(tensor_api_gm_to_l1, copy_gm_to_l1_ncdhw_to_ndc1hwc0)
{
    using T = half;
    constexpr uint32_t n_value = 1;
    constexpr uint32_t D = 3;
    constexpr uint32_t height = 4;
    constexpr uint32_t width = 4;
    constexpr uint32_t c0_value = 16;
    constexpr uint32_t channel = 32;
    constexpr uint32_t c1 = channel / c0_value; // 2

    auto gm_ncdhw = make_frame_layout<ncdhw_layout_ptn>(
        static_cast<int32_t>(n_value), static_cast<int32_t>(channel), static_cast<int32_t>(D), static_cast<int32_t>(height),
        static_cast<int32_t>(width));
    auto l1_ndc1hwc0 = make_frame_layout<ndc1hwc0_layout_ptn>(
        static_cast<int32_t>(n_value), static_cast<int32_t>(D), static_cast<int32_t>(c1), static_cast<int32_t>(height),
        static_cast<int32_t>(width), static_cast<int32_t>(c0_value));
    auto gm_a = make_tensor(make_mem_ptr<location::gm>(reinterpret_cast<T*>(src0_gm)), gm_ncdhw);
    auto l1a = make_tensor(make_mem_ptr<location::l1>(reinterpret_cast<T*>(l1a_buf)), l1_ndc1hwc0);

    make_copy(copy_gm_to_l1{}, copy_gm_to_l1_trait_default{}).call(l1a, gm_a);

    // One dn2nz DMA (and nz para) per depth slice.
    ASSERT_EQ(g_gm_to_l1_dn_to_nz_captures.size(), D);
    ASSERT_EQ(g_gm_to_l1_nz_para_captures.size(), D);

    for (uint32_t d = 0; d < D; ++d) {
        const auto& dn2nz = g_gm_to_l1_dn_to_nz_captures[d];
        const auto& nz_para = g_gm_to_l1_nz_para_captures[d];

        EXPECT_EQ(nz_para.nd_num, height);                             // dn_num = src_h
        EXPECT_EQ(dn2nz.n_value, width);                             // n_value = src_w
        EXPECT_EQ(dn2nz.d_value, channel);                             // d_value = src_c
        EXPECT_EQ(dn2nz.loop1_src_stride, D * height * width * sizeof(T)); // channel step: NCDHW stride[1]=D*height*width
        EXPECT_EQ(dn2nz.loop4_src_stride, width * sizeof(T));         // height step: NCDHW stride[3]=width
        EXPECT_EQ(nz_para.loop2_dst_stride, 1);
        EXPECT_EQ(nz_para.loop3_dst_stride, height * width); // dst_h * dst_w
        EXPECT_EQ(nz_para.loop4_dst_stride, width);     // dst_w
        EXPECT_FALSE(dn2nz.enable_small_c0);

        // Per-slice pointer offsets: src advances by height*width (depth stride), dst by c1*height*width*c0_value.
        const T* expect_src = reinterpret_cast<const T*>(src0_gm) + static_cast<size_t>(d) * (height * width);
        const T* expect_dst = reinterpret_cast<const T*>(l1a_buf) + static_cast<size_t>(d) * (c1 * height * width * c0_value);
        EXPECT_EQ(dn2nz.src, static_cast<void*>(const_cast<T*>(expect_src)));
        EXPECT_EQ(dn2nz.dst, static_cast<void*>(const_cast<T*>(expect_dst)));
    }
}
