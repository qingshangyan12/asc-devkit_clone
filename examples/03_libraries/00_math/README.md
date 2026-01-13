# Math API样例介绍
## 概述
本样例集介绍了Math API不同特性的典型用法，给出了对应的端到端实现。

## 算子开发样例
|  目录名称                                                   |  功能描述                                              |
| ------------------------------------------------------------ | ---------------------------------------------------- |
| [acos](./acos) | 本样例介绍了高阶api Acos的调用，该api的功能为按元素做反余弦函数计算 |
| [acosh](./acosh) | 本样例介绍了高阶api Acosh的调用，该api的功能为按元素做双曲反余弦函数计算 |
| [addcdiv](./addcdiv) | 本样例通过Ascend C编程语言实现了Addcdiv算子，使用<<<>>>内核调用符来完成算子核函数在NPU侧运行验证的基础流程，给出了对应的端到端实现 |
| [asin](./asin) | 本样例介绍了高阶api Asin的调用，该api的功能为按元素做反正弦函数计算 |
| [asinh](./asinh) | 本样例介绍了高阶api Asinh的调用，该api的功能为按元素做反双曲正弦函数计算 |
| [atanh](./atanh) | 本样例介绍了高阶api Atanh的调用，该api的功能为按元素做反双曲正切余弦函数计算 |
| [axpy_half_float](./axpy_half_float) | 本样例基于Axpy实现源操作数src中每个元素与标量求积后和目的操作数dst中的对应元素相加的功能。Axpy接口的源操作数和目的操作数的数据类型只能取三种组合：(half, half)、(float, float)、(half, float)。本样例中输入tensor和标量的数据类型为half，输出tensor数据类型为float。本样例通过Ascend C编程语言实现了Axpy算子，使用<<<>>>内核调用符来完成算子核函数在NPU侧运行验证的基础流程，给出了对应的端到端实现 |
| [axpy_half_half](./axpy_half_half) | 本样例基于Axpy实现源操作数src中每个元素与标量求积后和目的操作数dst中的对应元素相加的功能。Axpy接口的源操作数和目的操作数的数据类型只能取三种组合：(half, half)、(float, float)、(half, float)。本样例中输入tensor、标量、输出tensor数据类型均为half。本样例通过Ascend C编程语言实现了Axpy算子，使用<<<>>>内核调用符来完成算子核函数在NPU侧运行验证的基础流程，给出了对应的端到端实现 |
| [ceil](./ceil) | 本样例介绍了高阶api Ceil的调用，该api的功能为获取大于或等于x的最小的整数值，即向正无穷取整操作 |
| [cos](./cos) | 本样例介绍了调用Cos高阶API实现cos算子，并按照核函数直调的方式给出了对应的端到端实现 |
| [cosh](./cosh) | 本样例介绍了高阶api Cosh的调用，该api的功能为按元素做双曲余弦函数计算 |
| [cumsum](./cumsum) | 本样例基于Kernel直调算子工程，介绍了调用CumSum高阶API实现cumsum单算子，用于对输入张量按行或列进行累加和操作，输出结果中每个元素都是输入张量中对应位置及之前所有行或列的元素累加和 |
| [erf](./erf) | 本样例介绍了高阶api Erf的调用，该api的功能为按元素做误差函数计算（也称为高斯误差函数，error function or Gauss error function） |
| [erfc](./erfc) | 本样例介绍了高阶api Erfc的调用，该api的功能为返回输入x的互补误差函数结果，积分区间为x到无穷大 |
| [frac](./frac) | 本样例介绍了高阶api Frac的调用，该api的功能为按元素做取小数计算 |
| [isfinite](./isfinite) | 本样例介绍了高阶api IsFinite的调用，该api的功能为按元素判断输入的浮点数是否非NAN、非INF，输出结果为浮点数或者布尔值 |
| [leaky_relu](./leaky_relu) | 本样例基于LeakyRelu实现激活函数，可用于对输入tensor按元素执行Leaky Relu（Leaky Rectified Liner Unit）操作。本样例通过Ascend C编程语言实现了LeakyRelu算子，使用<<<>>>内核调用符来完成算子核函数在NPU侧运行验证的基础流程，给出了对应的端到端实现。 |
| [lgamma](./lgamma) | 本样例介绍了高阶api Lgamma的调用，该api的功能为按元素计算x的gamma函数的绝对值并求自然对数 |
| [log](./log) | 本样例介绍了高阶api Atanh的调用，该api的功能为按元素做反双曲正切余弦函数计算 |
| [sign](./sign) | 本样例介绍了高阶api Sign的调用，该api的功能为按元素执行Sign操作，Sign是指返回输入数据的符号，如果为0则返回0，如果为正数则返回1，如果为负数则返回-1 |
| [sinh](./sinh) | 本样例介绍了高阶api Sinh的调用，该api的功能为按元素做双曲正弦函数计算 |
| [tan](./tan) | 本样例介绍了高阶api Tan的调用，该api的功能为按元素做正切函数计算 |
| [trunc](./trunc) | 本样例介绍了高阶api Trunc的调用，该api的功能为按元素做浮点数截断，即向零取整操作 |
| [xor](./xor) | 本样例介绍了高阶api Xor的调用，该api的功能为按元素执行Xor运算 |