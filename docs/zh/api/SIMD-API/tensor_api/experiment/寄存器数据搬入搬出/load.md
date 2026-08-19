# load

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

头文件路径：`"c_api/reg_compute/reg_load.h"`。

对齐数据搬运接口，从UB连续对齐搬入目的操作数，实现NORM搬入模式：正常模式，搬运VL数据。

支持三种偏移方式：
- 偏移固定传入0，由用户自行更新源操作数的地址。
- 通过int32_t传入偏移，用户可以选择更新偏移或者更新源操作数的地址。
- 通过addr_reg地址寄存器传入偏移，用户可以选择更新偏移或者更新源操作数的地址。

## 3. 函数原型

```cpp
enum class load_sideband_mode {
    direct = 0,  // 纯对齐搬运          asc_loadalign
    deintlv,     // 解交织              asc_loadalign_deintlv
    downsample,  // 下采样              asc_loadalign_downsample
    upsample,    // 上采样              asc_loadalign_upsample
    unpack2,     // 2× 位宽解包         asc_loadalign_unpack
    unpack4      // 4× 位宽解包         asc_loadalign_unpack4
};

template <load_sideband_mode sideband_mode = load_sideband_mode::direct,
          typename Coord>
decltype(auto) local_tensor::load(Coord crd);
// 返回 reg_tensor<value_type>，不绑 mask，需 .with_mask(m)。
```

## 4. 参数说明

**表1 参数说明**

| 参数名 | 输入/输出 | 描述 |
|--------|-----------|------|
| `crd` | 输入 | 坐标，通常 `make_coord(i * VL)`。 |
| `sideband_mode` | 模板 | `direct` / `deintlv` / `downsample` / `upsample` / `unpack2` / `unpack4`。 |
| 返回值 | 输出 | `reg_tensor<T>`，**须** `.with_mask(m)`。 |

## 5. 返回值说明

返回 `reg_tensor<T>` 一拍数据；**不含**有效 mask，须 `.with_mask(m)`。

## 6. 约束说明

- 本接口仅在 AIV 上生效，非 AIV 调用直接返回。
- 返回值不含有效 mask，须 `.with_mask(m)`。
- 有效 lane 由绑定的 mask 决定；尾块须 `update_mask<T>(remain)`。

## 7. 调用示例

VF 内调用示例（摘自接口列表；完整 Kernel 直调可参考同目录计算类接口手册如 `operator_and.md`）。

以 Ascend 950PR/Ascend 950DT 产品（对应 NPU 架构为 dav-3510）为例：

```bash
bisheng example.asc -o main --npu-arch=dav-3510; ./main
```

```cpp
__simd_vf__ void vf_load(local_tensor<half> src_ub, local_tensor<half> dst_ub, uint32_t total) {
    constexpr uint32_t VL = asc_get_vf_len<half>;
    uint32_t repeatTimes = div_ceil(total, VL);
    for (uint32_t i = 0; i < repeatTimes; ++i) {
        auto m = update_mask<half>(total);                          // 生成本拍谓词，并扣减 rem
        auto x = src_ub.load<load_sideband_mode::direct>(make_coord(i * VL)).with_mask(m); // load 不带 mask，需链式绑定
        auto d = src_ub.load<load_sideband_mode::deintlv>(make_coord(i * VL)).with_mask(m); // 解交织
        auto w = src_ub.load<load_sideband_mode::unpack2>(make_coord(i * VL)).with_mask(m); // 2× 解包
        auto y = relu(x);                                          // mask 跟随 x；标量按 vec_len 广播
        dst_ub.store(make_coord(i * VL), y, m);                      // 写回有效 vec_len
    }
}
```
