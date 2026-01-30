# Coshape

## 产品支持情况

| 产品                                        | 是否支持 |
| ------------------------------------------- | :------: |
| Ascend 950PR/Ascend 950DT                   |    √     |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 |    x     |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 |    √     |
| Atlas 200I/500 A2 推理产品                  |    x     |
| Atlas 推理系列产品AI Core                   |    x     |
| Atlas 推理系列产品Vector Core               |    x     |
| Atlas 训练系列产品                          |    x     |
| Atlas 200/300/500 推理产品                  |    x     |

## 功能说明

获取Layout的的codomain陪域的大小。

## 函数原型

```
template <size_t... Is, typename ShapeType, typename StrideType>
__aicore__ inline constexpr auto Coshape(const Layout<Shape, Size>& layout)
```

## 参数说明

| 参数名 | 输入/输出 | 描述                                                         |
| :----- | :-------- | :----------------------------------------------------------- |
| Is...  | 输入      | 可变参数模板。表示要提取的维度索引列表。                     |
| Shape  | 输入      | Std::tuple结构类型，用于定义数据的逻辑形状。例如二维矩阵的行数和列数或多维张量的各维度大小。 |
| Stride | 输入      | Std::tuple结构类型，用于定义各维度在内存中的步长，即同维度相邻元素在内存中的间隔，间隔的单位为元素，与Shape的维度信息一一对应。 |
| Layout | 输入      | 布局对象，封装了张量的Shape和Stride信息                      |

## 返回值说明

返回Layout对象陪域的大小。

## 约束说明

无

## 调用示例

```
// 初始化Layout数据结构，获取对应数值
AscendC::Shape<int,int,int> shape = AscendC::MakeShape(2, 3);
AscendC::Stride<int,int,int> stride = AscendC::MakeStride(1, 6);
auto layoutMake = AscendC::MakeLayout(shape, stride);

auto cosize  = Cosize(layoutMake); //cosize = 14
auto coshape = Coshape(layoutMake); // coshape = 14
```