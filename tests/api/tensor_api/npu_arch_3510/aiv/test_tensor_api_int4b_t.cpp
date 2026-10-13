#include <gtest/gtest.h>
#include "tensor_api/stub/cce_stub.h"
#include "tensor_api/tensor.h"
#include "tensor_api/experimental/arch/vector/reg_data_load.h"
#include "tensor_api/experimental/arch/vector/reg_data_store.h"

namespace {

template <typename Tensor, asc::te::experimental::load_sideband_mode Mode>
__simd_callee__ inline void compile_int4_load(Tensor& tensor)
{
    using namespace asc::te::experimental;
    auto coordinate = asc::te::make_coord(64);
    auto value = load<Mode>(tensor, coordinate);
    auto value_without_coord = load<Mode>(tensor);
    if constexpr (Mode == load_sideband_mode::deintlv) {
        static_assert(AscendC::Std::is_same_v<decltype(value), reg_pair<int4x2_t>>);
        static_assert(AscendC::Std::is_same_v<decltype(value_without_coord), reg_pair<int4x2_t>>);
        store<store_sideband_mode::intlv>(tensor, coordinate, value.first, value.second);
        store<store_sideband_mode::intlv>(tensor, value_without_coord.first, value_without_coord.second);
    } else {
        static_assert(AscendC::Std::is_same_v<decltype(value), reg_tensor<int4x2_t>>);
        static_assert(AscendC::Std::is_same_v<decltype(value_without_coord), reg_tensor<int4x2_t>>);
        store(tensor, coordinate, value);
        store(tensor, value_without_coord);
        store<store_sideband_mode::store_1st>(tensor, coordinate, value);
        store<store_sideband_mode::store_1st>(tensor, value_without_coord);
    }
}

template <typename Tensor, asc::te::experimental::broadcast_mode Mode>
__simd_callee__ inline void compile_int4_broadcast(Tensor& tensor)
{
    using namespace asc::te::experimental;
    auto value = load_broadcast<Mode>(tensor, asc::te::make_coord(64));
    auto value_without_coord = load_broadcast<Mode>(tensor);
    static_assert(AscendC::Std::is_same_v<decltype(value), reg_tensor<int4x2_t>>);
    static_assert(AscendC::Std::is_same_v<decltype(value_without_coord), reg_tensor<int4x2_t>>);
}

template <typename Tensor>
__simd_vf__ inline void compile_int4_interfaces(Tensor tensor)
{
    using namespace asc::te::experimental;
    compile_int4_load<Tensor, load_sideband_mode::direct>(tensor);
    compile_int4_load<Tensor, load_sideband_mode::downsample>(tensor);
    compile_int4_load<Tensor, load_sideband_mode::unpack>(tensor);
    compile_int4_load<Tensor, load_sideband_mode::unpack4>(tensor);
    compile_int4_load<Tensor, load_sideband_mode::upsample>(tensor);
    compile_int4_load<Tensor, load_sideband_mode::deintlv>(tensor);
    compile_int4_broadcast<Tensor, broadcast_mode::elem>(tensor);
    compile_int4_broadcast<Tensor, broadcast_mode::datablock>(tensor);
}

template <typename DataType>
__aicore__ inline void instantiate_int4_interfaces(__ubuf__ DataType* source)
{
    auto layout = asc::te::make_layout(asc::te::make_shape(2048));
    auto tensor = asc::te::make_tensor(asc::te::make_mem_ptr<asc::te::location::ub>(source), layout);
    static_assert(AscendC::Std::is_same_v<typename decltype(tensor)::data_type, DataType>);
    compile_int4_interfaces(tensor);
}

}

TEST(TensorApiRegInt4, CompileLogicalInt4Interfaces)
{
    auto interfaces = &instantiate_int4_interfaces<int4b_t>;
    (void)interfaces;
}

TEST(TensorApiRegInt4, LogicalCoordinateToPackedAddress)
{
    __ubuf__ int4b_t storage[128];
    auto layout = asc::te::make_layout(asc::te::make_shape(4, 64), asc::te::make_stride(64, 1));
    auto tensor = asc::te::make_tensor(asc::te::make_mem_ptr<asc::te::location::ub>(storage), layout);

    auto engine00 = tensor.engine() + tensor.layout()(asc::te::make_coord(0, 0));
    auto engine02 = tensor.engine() + tensor.layout()(asc::te::make_coord(0, 2));
    auto engine10 = tensor.engine() + tensor.layout()(asc::te::make_coord(1, 0));
    auto engine12 = tensor.engine() + tensor.layout()(asc::te::make_coord(1, 2));

    EXPECT_EQ(engine00.begin().get(), storage);
    EXPECT_EQ(engine02.begin().get(), storage + 1);
    EXPECT_EQ(engine10.begin().get(), storage + 32);
    EXPECT_EQ(engine12.begin().get(), storage + 33);
}
