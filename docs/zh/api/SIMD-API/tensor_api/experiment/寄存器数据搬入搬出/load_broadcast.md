# load_broadcast

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

对齐数据搬运接口，从UB连续对齐搬入目的操作数，实现BRC搬入模式：搬运一个b8/b16/b32类型的数据，并Broadcast到所有元素位置。

支持三种偏移方式：
- 偏移固定传入0，由用户自行更新源操作数的地址。
- 通过int32_t传入偏移，用户可以选择更新偏移或者更新源操作数的地址。
- 通过addr_reg地址寄存器传入偏移，用户可以选择更新偏移或者更新源操作数的地址。

## 3. 函数原型

```cpp
enum class broadcast_mode {
    elem,             // 单元素 → 整拍
    datablock,        // 按 32B datablock 广播
    elem2datablock    // 元素先扩到 datablock
};

template <broadcast_mode brc_mode = broadcast_mode::elem, typename Coord>
decltype(auto) local_tensor::load_broadcast(Coord crd);
```

## 4. 参数说明

**表1 参数说明**

| 参数名 | 描述 |
|--------|------|
| `crd` | 广播源坐标。 |
| `brc_mode` | `elem` / `datablock` / `elem2datablock`。 |

## 5. 返回值说明

返回广播后的 `reg_tensor<T>`；须 `.with_mask(m)`。

## 6. 约束说明

- 本接口仅在 AIV 上生效，非 AIV 调用直接返回。
- 返回值不含有效 mask，须 `.with_mask(m)`。

## 7. 调用示例

VF 内调用示例（摘自接口列表；完整 Kernel 直调可参考同目录计算类接口手册如 `operator_and.md`）。

以 Ascend 950PR/Ascend 950DT 产品（对应 NPU 架构为 dav-3510）为例：

```bash
bisheng example.asc -o main --npu-arch=dav-3510; ./main
```

```cpp
__simd_vf__ void vf_load_broadcast(local_tensor<half> src_ub, local_tensor<half> dst_ub, uint32_t total) {
    auto bias = src_ub.load_broadcast<broadcast_mode::elem>(make_coord(0));
    constexpr uint32_t VL = asc_get_vf_len<half>;
    uint32_t repeatTimes = div_ceil(total, VL);
    for (uint32_t i = 0; i < repeatTimes; ++i) {
        auto m = update_mask<half>(total);                              // 生成本拍谓词，并扣减 rem
        auto x = src_ub.load(make_coord(i * VL)).with_mask(m);             // load 不带 mask，需链式绑定
        auto y = x + bias;                                              // mask 跟随 x；标量按 vec_len 广播
        dst_ub.store(make_coord(i * VL), y, m);                            // 写回有效 vec_len
    }
}
```
