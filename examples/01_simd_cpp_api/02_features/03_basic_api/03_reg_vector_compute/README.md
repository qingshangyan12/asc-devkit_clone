# 向量计算类api样例介绍

## 概述

本路径下包含了与向量计算相关的多个API的样例。每个样例均基于Ascend C的<<<>>>直调方法，支持main函数和kernel函数在同一个cpp文件中实现。

## 算子开发样例

| 目录名称　　　　　 | 功能描述　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　|
| --------------------| ---------------------------------------------------------------------------------------------|
| [abs](./abs)　　　 | 本样例基于RegBase编程范式实现Abs运算，Relu/Exp/Sqrt/Ln/Log/Log2/Log10/Neg接口皆可参考该样例 |
| [mul](./mul)　　　 | 本样例基于RegBase编程范式实现Mul运算，Add/Sub/Div/Max/Min/Prelu接口皆可参考该样例　　　　　 |
| [muls](./muls)　　 | 本样例基于RegBase编程范式实现Muls运算，Adds/Maxs/Mins/LeakyRelu接口皆可参考该样例　　　　　 |
| [reduce](./reduce) | 本样例基于RegBase编程范式实现Reduce运算（SUM模式），Reduce接口支持SUM/MAX/MIN归约模式　　　 |
