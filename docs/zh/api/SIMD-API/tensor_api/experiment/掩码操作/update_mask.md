# update_mask

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

根据value大小生成对应的掩码寄存器中的值。掩码寄存器的元素有效范围从0到VL_T（位宽为Vector Length的对应数据类型的元素个数）。执行完该函数后，value会减去VL_T。算法逻辑表示如下：
  ```cpp
  value = (value < VL_T) ? 0 : (value - VL_T);
  ```

## 3. 函数原型

```cpp
namespace asc::te {

template <typename T>
decltype(auto) update_mask(uint32_t& remain);
// remain 按 min(remain, VL) 就地扣减；返回本拍 reg_tensor<bool>。

}
```

## 4. 参数说明

| 参数名       | 输入/输出 | 描述                |
| --------- | ----- | ----------------- |
| value       | 输入/输出 | 矢量计算需要操作的元素的具体数量。 |

## 5. 返回值说明

返回本拍有效 lane 的谓词 `reg_tensor<T>`，并更新 `remain`。

## 6. 约束说明

- 本接口仅在 AIV 上生效，非 AIV 调用直接返回。
- 模板参数 `T` 须与后续 `load` / `compute` 元素类型一致。
- `remain` 按本拍有效 lane 递减。

## 7. 调用示例

VF 内调用示例（摘自接口列表；完整 Kernel 直调可参考同目录计算类接口手册如 `operator_and.md`）。

以 Ascend 950PR/Ascend 950DT 产品（对应 NPU 架构为 dav-3510）为例：

```bash
bisheng example.asc -o main --npu-arch=dav-3510; ./main
```

```cpp
__simd_vf__ void vf_update_mask(local_tensor<half> src_ub, local_tensor<half> dst_ub, uint32_t total) {
    constexpr uint32_t VL = asc_get_vf_len<half>;
    uint32_t repeatTimes = div_ceil(total, VL);
    for (uint32_t i = 0; i < repeatTimes; ++i) {
        auto m = update_mask<half>(total);                              // 生成本拍谓词，并扣减 rem
        auto x = src_ub.load(make_coord(i * VL)).with_mask(m);             // load 不带 mask，需链式绑定
        auto y = relu(x);                                               // mask 跟随 x；标量按 vec_len 广播
        dst_ub.store(make_coord(i * VL), y, m);                            // 写回有效 vec_len
    }
}
```
