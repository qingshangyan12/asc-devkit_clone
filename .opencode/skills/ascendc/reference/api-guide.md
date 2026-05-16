# C++ 代码与 AscendC 语言对应关系

本文档提供标准 C++ 代码与 AscendC API 的对应关系，帮助开发者将标准 C++ 算法逻辑转换为 AscendC API实现。

---

## 1. 内存与变量

### 标准 C++

```cpp
template <typename T>
void APIComputeCPP(T* dst, T* src)
{
    // 进行计算，如调用Abs函数
    *dst = std::abs(*src); 
};
```

### AscendC Reg API

```cpp
// API层级，C++的入参变量对应UB的一个LocalTensor加载到寄存器的RegTensor，该LocalTensor为API入参LocalTensor<T> &src

// 通过GetPhyAddr()获取LocalTensor的物理地址
__ubuf__ T* dstUb = (__ubuf__ T*)dst.GetPhyAddr();
__ubuf__ T* srcUb = (__ubuf__ T*)src.GetPhyAddr();

// 将物理地址传递到vf函数内，读取数据到RegTensor
APIComputeVF<T>(dstUb, srcUb, calSize, repeatTimes, stride);

// 在VF函数内声明对应类型的RegTensor，进行后续计算
template <typename T>
__simd_vf__ inline void APIComputeVF(__ubuf__ T* dstUb, __ubuf__ T* srcUb, uint32_t calSize, uint16_t repeatTimes, uint16_t stride)
{
    Reg::RegTensor<T> srcReg;
    Reg::RegTensor<T> dstReg;
    Reg::MaskReg mask;
    uint32_t sreg = calSize; // 创建sreg，控制计算元素的数量
    for (uint16_t i = 0; i < repeatTimes; ++i) {
        mask = Reg::UpdateMask<T>(sreg);
        // 从UB加载数据到RegTensor
        Reg::LoadAlign<T>(srcReg, srcUb + i * stride);
        // 进行后续计算，如调用Abs
        Reg::Abs(dstReg, srcReg, mask);
        // 存储计算结果到UB
        Reg::StoreAlign<T>(dstUb + i * stride, dstReg, mask);
    }
}
```

## 2. 向量算术运算

### 变量二元运算对照表

| C++ 逐元素操作 | AscendC Reg API |
|--------------|----------------|
| `dst = a + b` | `Reg::Add(dst, a, b, fullMask)` |
| `dst = a - b` | `Reg::Sub(dst, a, b, fullMask)` |
| `dst = a * b` | `Reg::Mul(dst, a, b, fullMask)` |
| `dst = a / b` | `Reg::Div(dst, a, b, fullMask)` |    
| `dst = max(a, b)` | `Reg::Max(dst, a, b, fullMask)` |
| `dst = min(a, b)` | `Reg::Min(dst, a, b, fullMask)` |

### 常量二元运算对照表

| C++ | AscendC Reg API |
|-----|----------------|
| `dst = a + s` | `Reg::Adds(dst, src, s, fullMask)` |
| `dst = a * s` | `Reg::Muls(dst, src, s, fullMask)` |
| `dst = max(a, s)` | `Reg::Maxs(dst, src, s, fullMask)` |
| `dst = min(a, s)` | `Reg::Mins(dst, src, s, fullMask)` |

---

## 3. 一元运算对照表

| C++ | AscendC Reg API | 说明 |
|-----|----------------|------|
| `dst = abs(a)` | `Reg::Abs(dst, a, fullMask)` | 取绝对值 |
| `dst = -a` | `Reg::Neg(dst, a, fullMask)` | 取负 |
| `dst = exp(a)` | `Reg::Exp(dst, a, fullMask)` | 指数运算 |
| `dst = log(a)` | `Reg::Log(dst, a, fullMask)` | 自然对数 |

---

## 4. 融合运算对照表

| C++ | AscendC Reg API | 说明 |
|-----|----------------|------|
| `dst = s*a + dst` | `Reg::Axpy(dst, a, s, fullMask)` | 标量乘加 |
| `dst = fmaf(dst, a, b)` | `Reg::MulDstAdd(dst, a, b, fullMask)` | 融合乘加 |

---

## 5. 比较运算对照表

| C++ | AscendC Reg API |
|-----|----------------|
| `dst = (a == b)` | `Reg::Compare<T, EQ>(dstMask, a, b, fullMask)` |
| `dst = (a != b)` | `Reg::Compare<T, NE>(dstMask, a, b, fullMask)` |
| `dst = (a < b)` | `Reg::Compare<T, LT>(dstMask, a, b, fullMask)` |
| `dst = (a > b)` | `Reg::Compare<T, GT>(dstMask, a, b, fullMask)` |
| `dst = (a <= b)` | `Reg::Compare<T, LE>(dstMask, a, b, fullMask)` |
| `dst = (a >= b)` | `Reg::Compare<T, GE>(dstMask, a, b, fullMask)` |
| `dst = (a == 3)` | `Reg::Compares<T, EQ>(dstMask, a, 3, fullMask)` |
| `dst = (a != 3)` | `Reg::Compares<T, NE>(dstMask, a, 3, fullMask)` |
| `dst = (a < 3)` | `Reg::Compares<T, LT>(dstMask, a, 3, fullMask)` |
| `dst = (a > 3)` | `Reg::Compares<T, GT>(dstMask, a, 3, fullMask)` |
| `dst = (a <= 3)` | `Reg::Compares<T, LE>(dstMask, a, 3, fullMask)` |
| `dst = (a >= 3)` | `Reg::Compares<T, GE>(dstMask, a, 3, fullMask)` |

---

## 6. 逻辑运算对照表

| C++ | AscendC Reg API |
|-----|----------------|
| `dst = (a && b)` | `Reg::And(dstMask, aMask, bMask, fullMask)` |
| `dst = (a \|\| b)` | `Reg::Or(dstMask, aMask, bMask, fullMask)` |
| `dst = !a` | `Reg::Not(dstMask, aMask, fullMask)` |

---

## 7. 位运算对照表

| C++ | AscendC Reg API |
|-----|----------------|
| `dst = a & b` | `Reg::And(dst, a, b, fullMask)` |
| `dst = a \| b` | `Reg::Or(dst, a, b, fullMask)` |
| `dst = a ^ b` | `Reg::Xor(dst, a, b, fullMask)` |
| `dst = ~a` | `Reg::Not(dst, a, fullMask)` |

---

## 8. 分支语句和MergingMode

C++代码中的分支语句通过AscendC的MaskReg的逻辑运算在后续运算中进行控制。由于大部分Reg API不支持Merging Mode(未被Mask筛选的元素在目的操作数中保留原值，当前大部分Reg API都不支持Merging Mode，即在目的操作数中被置0)，需要使用临时Reg存储中间结果，通过Copy操作将结果写入dstReg。

```cpp
template <typename T>
void APIComputeCPP(T* dst, T* src)
{
    if (*src > 0){
        *dst = std::abs(*src); 
    } else {
        *dst = -std::abs(*src); 
    }
};
```

其对应的AscendC代码为
```cpp
template <typename T>
__simd_vf__ inline void APIComputeVF(__ubuf__ T* dstUb, __ubuf__ T* srcUb, uint32_t calSize, uint16_t repeatTimes, uint16_t stride)
{
    Reg::RegTensor<T> srcReg;
    Reg::RegTensor<T> dstReg;
    Reg::RegTensor<T> tempReg;
    Reg::MaskReg mask;
    Reg::MaskReg gtMask;
    uint32_t sreg = calSize; // 创建sreg，控制计算元素的数量
    for (uint16_t i = 0; i < repeatTimes; ++i) {
        mask = Reg::UpdateMask<T>(sreg);
        // 从UB加载数据到RegTensor
        Reg::LoadAlign<T>(srcReg, srcUb + i * stride);
        // 进行分支判断
        Reg::Compares<T, CMPMODE::GT>(gtMask, srcReg, 0, mask);
        // 进行后续计算，如调用Abs
        Reg::Abs(dstReg, srcReg, mask);
        // 对gtMask取反，得到else分支
        Reg::Not(gtMask, gtMask, mask);
        // 进行后续计算，将结果写入tempReg，调用Reg::Copy避免Merging Mode将之前的运算结果覆盖
        Reg::Abs(tempReg, srcReg, gtMask);
        Reg::Neg(tempReg, tempReg, gtMask);
        Reg::Copy(dstReg, tempReg, gtMask);
        // 存储计算结果到UB
        Reg::StoreAlign<T>(dstUb + i * stride, dstReg, mask);
    }
}
```

Reg::UpdateMask<T>(sreg)函数用于根据sreg更新MaskReg，sreg为计算元素的数量，每次循环其完成一次自减操作。
例如，sreg = 65时，数据类型为float，每次循环，一个RegTensor内对256 / sizeof(float) = 64个float类型的元素，在循环之后，
sreg -= 64，即sreg更新为2. 在下一次循环中，sreg = 2，即只对RegTensor中前2个float类型的元素进行计算和搬运。

---

### 常用头文件

```cpp
#include "kernel_operator.h"              // 基础API（C++ Tensor）
```
