# store（双寄存器 intlv）

> **命名空间** `asc::te` · **头文件** `local_tensor.h`

## 1. 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
- Atlas 200I/500 A2 推理产品：不支持
- Atlas 推理系列产品AI Core：不支持
- Atlas 推理系列产品Vector Core：不支持
- Atlas 训练系列产品：不支持

## 2. 功能说明

头文件路径：`"c_api/reg_compute/reg_store.h"`。

从矢量数据寄存器交错搬出到UB的接口，忽略mask，将src0、src1中的元素交错存储于dst中，存储长度为VL*2。支持数据类型为b8、b16、b32。

## 3. 函数原型

```cpp
namespace asc::te {

// 使用 store_sideband_mode（完整枚举见 §4.3）
enum class store_sideband_mode {
    direct, store_1st, intlv, pack, pack_quarter
};

template <store_sideband_mode sideband_mode = store_sideband_mode::direct,
          typename Coord, typename T>
decltype(auto) local_tensor::store(
    Coord crd,
    reg_tensor<T> src0,
    reg_tensor<T> src1,
    reg_tensor<bool> mask = all_mask<value_type>());
// 仅 sideband_mode == intlv 时走交织写回。

}
```

## 4. 参数说明

**表1 参数说明**

| 参数名 | 描述 |
|--------|------|
| `crd` | 写回坐标。 |
| `first` / `second` | `reg_pair` 或双寄存器 intlv 写回。 |
| `mask` | 写掩码。 |

## 5. 返回值说明

无返回值（写回 UB）；有效 lane 由 `mask` 控制。

## 6. 约束说明

- 本接口仅在 AIV 上生效，非 AIV 调用直接返回。
- 写回有效 lane 由 `mask` 控制；尾块须传入 `update_mask` 结果。

## 7. 调用示例

VF 内调用示例（摘自接口列表；完整 Kernel 直调可参考同目录计算类接口手册如 `operator_and.md`）。

以 Ascend 950PR/Ascend 950DT 产品（对应 NPU 架构为 dav-3510）为例：

```bash
bisheng example.asc -o main --npu-arch=dav-3510; ./main
```

```cpp
__simd_vf__ void vf_store_intlv(local_tensor<half> src0_ub, local_tensor<half> src1_ub, local_tensor<half> dst_ub, uint32_t total) {
    constexpr uint32_t VL = asc_get_vf_len<half>;
    uint32_t repeatTimes = div_ceil(total, VL);
    for (uint32_t i = 0; i < repeatTimes; ++i) {
        auto m = update_mask<half>(total);                          // 生成本拍谓词，并扣减 rem
        auto x0 = src0_ub.load(make_coord(i * VL)).with_mask(m);       // load 不带 mask，需链式绑定
        auto x1 = src1_ub.load(make_coord(i * VL)).with_mask(m);
        auto [a, b] = interleave(x0, x1);
        dst_ub.store<store_sideband_mode::intlv>(make_coord(i * VL), a, b, m); // 写回有效 vec_len
    }
}
```
