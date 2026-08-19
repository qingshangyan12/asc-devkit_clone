# reg_tensor<T>

> **命名空间** `asc::te` · **头文件** `reg_vector/reg_tensor.h`

## 1. 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
- Atlas 200I/500 A2 推理产品：不支持
- Atlas 推理系列产品 AI Core：不支持
- Atlas 推理系列产品 Vector Core：不支持
- Atlas 训练系列产品：不支持

## 2. 功能说明

一拍硬件向量寄存器的值类型：持有 `value`（数据）与内嵌 `mask`（谓词）。算术/激活读左操作数 mask；非活跃 lane **ZEROING**。`with_mask` 只替换谓词并返回 `*this`，便于 `load(crd).with_mask(m)` 链式绑定。

`reg_tensor<T>` 是 Vector 编程的基本计算句柄，对应硬件 **一个 VL 拍** 的向量寄存器，而不是任意 shape 的 tile。已特化 `float` / `half` / `bool`（`bool` 即谓词 `vector_bool`）。

- **数据与谓词绑在一起**：后续 `+`、`relu`、`select` 等不再单独传 MaskReg，结果 mask 默认等于左源（或指定源）的 mask。

- **load 不带 mask**：从 UB 读出的对象必须立刻 `.with_mask(m)`，否则后续计算按未定义/全开谓词处理，尾块会写脏。

- **赋值 `operator=`**：允许把另一寄存器或标量语义写入；标量路径内部等价于广播后再写 `value`。

- **与 cuTile 差异**：`ct::tile` 不可变且无 mask 成员；本类型可变谓词，边界在计算侧用 `update_mask` 表达。

## 3. 函数原型

```cpp
namespace asc::te {

template <typename T>
struct reg_tensor {
    using elem_type = T;
    using reg_type  = typename type_get<T>::T;
    reg_type     value;
    vector_bool  mask;

    reg_tensor() = default;
    explicit reg_tensor(vector_bool mask_value);

    template <typename U>
    decltype(auto) operator=(U src);

    reg_tensor<elem_type>& with_mask(reg_tensor<bool> reg_mask);
};

}
```

## 4. 参数说明

**表1 说明**

| 成员 | 描述 |
|------|------|
| `value` | 矢量数据寄存器（VL 宽）。 |
| `mask` | 内嵌 `vector_bool` 谓词。 |

## 5. 返回值说明

类型封装，见 §2 功能说明。

## 6. 约束说明

- 本接口仅在 AIV 上生效，非 AIV 调用直接返回。
- `value` 与 `mask` 成对使用；经 `load` 读入后须 `.with_mask(m)`。

## 7. 调用示例

VF 内调用示例（摘自接口列表；完整 Kernel 直调可参考同目录计算类接口手册如 `operator_and.md`）。

以 Ascend 950PR/Ascend 950DT 产品（对应 NPU 架构为 dav-3510）为例：

```bash
bisheng example.asc -o main --npu-arch=dav-3510; ./main
```

```cpp
__simd_vf__ void vf_reg_tensor(local_tensor<half> src_ub, local_tensor<half> dst_ub, uint32_t total) {
    constexpr uint32_t VL = asc_get_vf_len<half>;
    uint32_t repeatTimes = div_ceil(total, VL);
    for (uint32_t i = 0; i < repeatTimes; ++i) {
        auto m = update_mask<half>(total);                          // 生成本拍谓词，并扣减 rem
        auto x = src_ub.load(make_coord(i * VL)).with_mask(m);       // load 不带 mask，需链式绑定
        auto y = relu(x + 0.5f);                                  // mask 跟随 x；标量按 vec_len 广播
        dst_ub.store(make_coord(i * VL), y, m);                      // 写回有效 vec_len
    }
}
```
