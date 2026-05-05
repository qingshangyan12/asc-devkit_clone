# 基于RegBase编程log1p样例

## 概述
本样例基于RegBase编程范式实现log(1+x)高精度计算，支持Ascend 950PR/Ascend 950DT。

## 支持的产品
- Ascend 950PR/Ascend 950DT

## 样例描述
- 样例功能：
  对输入数据逐元素计算 log(1+x) = log1p(x)。对于 |x| < 0.125 采用8阶Taylor多项式逼近以消除(1+x)的浮点精度损失；对于 |x| >= 0.125 直接计算 log(1+x)。
- 样例规格：
  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="3" align="center">AIV样例</td></tr>
  <tr><td rowspan="2" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td></tr>
  <tr><td align="center">x</td><td align="center">[N]</td><td align="center">float / half</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">y</td><td align="center">[N]</td><td align="center">float / half</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">log1p_kernel_float / log1p_kernel_half</td></tr>
  </table>
- 测试数据覆盖：
  - |x| < 1e-7 的极小值（验证多项式精度）
  - |x| 在 [1e-3, 0.12] 区间（多项式路径）
  - |x| 在 0.125 边界附近（验证路径切换精度）
  - |x| 在 [0.13, 10.0] 区间（直接log路径）
  - 负值接近-1的情况

## 编译运行

- 配置环境变量
  ```bash
  source /usr/local/Ascend/cann/set_env.sh
  ```

- 样例执行
  ```bash
  mkdir -p build && cd build;
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..; make -j;
  python3 ../scripts/gen_data.py
  ./demo
  ```

  CPU调试 / NPU仿真模式：
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..; make -j;
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..; make -j;
  ```
