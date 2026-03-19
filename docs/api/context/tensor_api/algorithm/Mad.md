# Mad

## 产品支持情况

| 产品     | 是否支持 |
| ----------- |:----:|
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √    |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √    |

## 功能说明

执行矩阵乘加操作，完成 dst = src0 * src1 + dst 的计算。

## 函数原型

```cpp
template <typename Tp, const Tp& traits, typename T, typename... Params>
__aicore__ inline void Mad(const MmadAtom<T>& atomMad, const Params& ...params)

template <typename T, typename... Params>
__aicore__ inline void Mad(const MmadAtom<T>& atomMad, const Params& ...params)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
|--------|-----------|------|
| atomMad | 输入 | 矩阵乘加原子操作对象 |
| params | 输入 | 矩阵乘加操作的参数，可变参数 |

## 返回值说明

无

## 约束说明

- atomMad必须是有效的MmadAtom对象
- params的数量和类型必须与矩阵乘加操作的要求匹配
- 源操作数和目的操作数必须位于支持的存储空间

## 调用示例

```cpp
// 创建MmadAtom
auto mmadAtom = AscendC::MakeMad(arg1, arg2, arg3);

// 执行矩阵乘加操作
AscendC::Mad(mmadAtom, dst, src0, src1);
```