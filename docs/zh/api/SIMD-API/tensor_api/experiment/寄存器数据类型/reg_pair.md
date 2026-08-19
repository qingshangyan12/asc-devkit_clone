# reg_pair<T>

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

`interleave` / `deinterleave`（及 intlv store）的双寄存器打包：`first` / `second` 各带自己的 mask。不是通用数组，长度为编译期固定的 2。

部分硬件指令一次产出或消费 **两路** 向量（交织/解交织）。`reg_pair<T>` 把这两路合成一个返回值，避免输出形参。

- 访问：`pair.first`、`pair.second`，类型均为 `reg_tensor<T>`。

- 写回：双寄存器 `store<intlv>(crd, pair.first, pair.second, m)`；不要写成 `auto y = a, b`。

- 与未合入的 `reg_array<N,T>` 不同：本结构只服务交织类 API，不能当下标批处理容器。

## 3. 函数原型

```cpp
namespace asc::te {

template <typename T>
struct reg_pair {
    reg_tensor<T> first;
    reg_tensor<T> second;
};

}
```

## 4. 参数说明

**表1 说明**

| 成员 | 描述 |
|------|------|
| `first` / `second` | 双路 `reg_tensor<T>`，用于 intlv/deintlv 等。 |

## 5. 返回值说明

类型封装，见 §2 功能说明。

## 6. 约束说明

- 本接口仅在 AIV 上生效，非 AIV 调用直接返回。
- 用于 intlv/deintlv 等双路数据；`first` / `second` 各自携带 mask。

## 7. 调用示例

VF 内调用示例（摘自接口列表；完整 Kernel 直调可参考同目录计算类接口手册如 `operator_and.md`）。

以 Ascend 950PR/Ascend 950DT 产品（对应 NPU 架构为 dav-3510）为例：

```bash
bisheng example.asc -o main --npu-arch=dav-3510; ./main
```

```cpp
__simd_vf__ void vf_reg_pair(local_tensor<half> src0_ub, local_tensor<half> src1_ub, local_tensor<half> dst_ub, uint32_t total) {
    constexpr uint32_t VL = asc_get_vf_len<half>;
    uint32_t repeatTimes = div_ceil(total, VL);
    for (uint32_t i = 0; i < repeatTimes; ++i) {
        auto m = update_mask<half>(total);                          // 生成本拍谓词，并扣减 rem
        auto x0 = src0_ub.load(make_coord(i * VL)).with_mask(m);       // load 不带 mask，需链式绑定
        auto x1 = src1_ub.load(make_coord(i * VL)).with_mask(m);
        auto pair = interleave(x0, x1);                           // pair.first / pair.second
        dst_ub.store<store_sideband_mode::intlv>(make_coord(i * VL), pair.first, pair.second, m); // 写回有效 vec_len
    }
}
```
