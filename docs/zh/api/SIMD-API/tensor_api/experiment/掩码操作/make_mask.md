# make_mask

> **命名空间** `asc::te` · **头文件** `reg_vector/reg_compute.h`

## 1. 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
- Atlas 200I/500 A2 推理产品：不支持
- Atlas 推理系列产品AI Core：不支持
- Atlas 推理系列产品Vector Core：不支持
- Atlas 训练系列产品：不支持

## 2. 功能说明

头文件路径：`"c_api/reg_compute/reg_vector.h"`。

根据入参生成相应的掩码寄存器。生成的掩码用于约束RegTensor的计算范围，将掩码作为参数传入其他函数，则只有掩码为True所对应的元素才会参与计算。

## 3. 函数原型

```cpp
enum class mask_pattern {
    all,                                      // 全活跃，同 all_mask<T>()
    vl1, vl2, vl3, vl4, vl8, vl16, vl32, vl64, vl128,  // 前 n 个 lane
    every3, every4,                           // 周期稀疏
    half, quarter,                            // 半宽 / 四分之一宽
    none                                      // 全不活跃，同 none_mask<T>()
};

template <mask_pattern pattern = mask_pattern::all, typename T>
decltype(auto) make_mask();
// 按 T 的 sizeof 选 b16/b32；当前实现尚未按 pattern 分发，一律 PAT_ALL。
```

## 4. 参数说明

| 参数名       | 输入/输出 | 描述                |
| --------- | ----- | ----------------- |
| pat_mode  | 输入 | 掩码寄存器的创建模式，pat_mode表示选取元素的范围，定义如下：<pre><code>PAT_ALL, // 所有元素都参与计算<br>PAT_VL1, // 最低位的1个元素参与计算<br>PAT_VL2, // 最低位的2个元素参与计算<br>PAT_VL3, // 最低位的3个元素参与计算<br>PAT_VL4, // 最低位的4个元素参与计算<br>PAT_VL8, // 最低位的8个元素参与计算<br>PAT_VL16, // 最低位的16个元素参与计算<br>PAT_VL32, // 最低位的32个元素参与计算<br>PAT_VL64, // 最低位的64个元素参与计算<br>PAT_VL128, // 最低位的128个元素参与计算<br>PAT_M3, // 下标为3的倍数的元素参与计算<br>PAT_M4, // 下标为4的倍数的元素参与计算<br>PAT_H, // 低一半的元素参与计算<br>PAT_Q, // 低四分之一的元素参与计算<br>PAT_ALLF = 15 // 任何元素都不参与计算</pre> |

## 5. 返回值说明

返回 `reg_tensor<T>` 谓词寄存器。

## 6. 约束说明

- 本接口仅在 AIV 上生效，非 AIV 调用直接返回。
- 模板参数 `T` 须与后续 `load` / `compute` 元素类型一致。

## 7. 调用示例

VF 内调用示例（摘自接口列表；完整 Kernel 直调可参考同目录计算类接口手册如 `operator_and.md`）。

以 Ascend 950PR/Ascend 950DT 产品（对应 NPU 架构为 dav-3510）为例：

```bash
bisheng example.asc -o main --npu-arch=dav-3510; ./main
```

```cpp
__simd_vf__ void vf_make_mask(local_tensor<half> src_ub, local_tensor<half> dst_ub, uint32_t total) {
    constexpr uint32_t VL = asc_get_vf_len<half>;
    uint32_t repeatTimes = div_ceil(total, VL);
    for (uint32_t i = 0; i < repeatTimes; ++i) {
        auto m = update_mask<half>(total);                          // 生成本拍谓词，并扣减 rem
        auto m_all = make_mask<mask_pattern::all, half>();
        auto x = src_ub.load(make_coord(i * VL)).with_mask(m_all);   // load 不带 mask，需链式绑定
        auto y = x + fill(1.0f).with_mask(m_all);                   // mask 跟随 x；标量按 vec_len 广播
        dst_ub.store(make_coord(i * VL), y, m);                      // 写回有效 vec_len
    }
}
```
