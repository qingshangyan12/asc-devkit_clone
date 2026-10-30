# 以LeakyReLU-Affine融合与Clamp算子为载体的Lambda捕获样例

## 概述

本样例回答一个问题：**Lambda捕获在实际算子开发里到底解决了什么问题**；对应的SIMD与SIMT混合编程对照实现含Lambda标记规则，见[lambda_operator_hybrid](../../../../05_simd_simt_hybrid/03_features/lambda_operator_hybrid/README.md)样例。

以LeakyReLU-Affine融合算子（`y = scale * leaky_relu(x, alpha) + bias`）与Clamp算子（`y = clamp(x, lo, hi)`）两个简单算子为载体，展示SIMT编程核函数内Lambda捕获外部参数的四种实际编程作用：

1. **值捕获携带算子超参**：`alpha/scale/bias/lo/hi`等超参经核函数参数传入后，由核内Lambda**值捕获**，即拷贝进闭包。通用框架函数`elementwise_transform`的签名不含任何算子参数，只负责线程映射，新增算子不需要为每个超参增加形参，也不会出现长参数链。
2. **引用捕获累计统计状态**：负数计数器`negCount`被统计Lambda**引用捕获**后原地更新，一次遍历"边变换边统计"，统计结果不经过参数传递或返回值，直接落线程私有变量。
3. **闭包组合表达融合算子**：外层`fused`Lambda引用捕获`leaky/affine/tally`三个闭包，将"统计→激活→仿射"组合成单遍融合算子，无需为融合算子另写乘加展开。
4. **策略与框架分离**：`elementwise_transform`只写一份线程映射代码，注入LeakyReLU-Affine的Lambda得到融合算子，注入Clamp的Lambda得到裁剪算子，实现了策略与执行框架的正交分离，策略由Lambda承载，执行框架由模板函数承载。

> [!NOTE]
> Ascend C的Lambda是Device侧局部闭包：核函数参数仅支持基础类型、POD与指针，**Host侧无法把Lambda传入核函数**，不存在CUDA `--extended-lambda`等价机制。因此标准模式是：**超参作为普通标量传入核函数，核函数内定义Lambda捕获消费**，本样例即按此模式组织。SIMT编程场景核函数内Lambda无需标记callee属性，也不支持标记；SIMD与SIMT混合编程场景需按执行上下文标记`__simt_callee__`、`__simd_callee__`、`__aicore__`，详见[Lambda表达式](../../../../../docs/zh/guide/technical_appendix/cpp_standard_support/cpp_language_feature/lambda_expressions.md)文档及SIMD与SIMT混合编程对照样例。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|---|---|
| Ascend 950PR&950DT系列产品 | \>= CANN 9.3.0 |

## 目录结构介绍

```text
├── 00_lambda_operator_capture
│   ├── CMakeLists.txt               // cmake编译文件
│   ├── lambda_operator_capture.asc  // 以LeakyReLU-Affine与Clamp为载体的Lambda捕获样例实现
│   └── README.md                    // 样例说明文档
```

## 样例实现

### 执行流程

Host生成取值覆盖`[-5, 5]`的输入`x[i] = (i*7) % 11 - 5`，同时覆盖正数、负数与上下界越界点，单次核启动计算两个算子并统计每线程处理的负数个数：

| 计算 | 实现方式 | 输出期望 |
|---|---|---|
| `y1[i] = scale * leaky_relu(x[i], alpha) + bias` | `fused`闭包引用捕获`tally/leaky/affine`三个闭包，注入`elementwise_transform` | `y1[5] = 0.4`（`x[5]=-3`：`2×0.1×(-3)+1`） |
| `y2[i] = clamp(x[i], lo, hi)` | `clamp`闭包值捕获`lo/hi`，注入同一`elementwise_transform` | `y2[5] = -2.0`（`x[5]=-3`被`lo=-2`截断） |
| 每线程负数计数 `negStats[t]` | `tally`引用捕获`negCount`，随融合算子同遍历累计 | 全体线程合计`negTotal = 1862`，即输入4096个元素中取值为负的个数，按线程映射逐线程复算 |

Host按相同公式与相同线程映射（`grid-stride loop`）复算期望值，逐元素以`1e-5`相对容差比对，并与逐线程计数精确比对。

### 关键代码说明

**通用逐元素框架**：这是Device侧模板函数，签名不感知任何算子参数，例如：

```cpp
template <typename Op>
__aicore__ inline void elementwise_transform(Op op, const float* input, float* output, uint32_t total_length)
{
    uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    const uint32_t stride = gridDim.x * blockDim.x;
    for (; idx < total_length; idx += stride) {
        output[idx] = op(input[idx]);
    }
}
```

**核函数内构造闭包**：包括超参值捕获、统计引用捕获与闭包组合，例如：

```cpp
auto leaky  = [alpha](float v) { return v > 0.0f ? v : alpha * v; };   // 值捕获超参
auto affine = [scale, bias](float v) { return v * scale + bias; };      // 值捕获超参
uint32_t negCount = 0;
auto tally  = [&negCount](float v) { if (v < 0.0f) { ++negCount; } };   // 引用捕获统计状态
auto fused  = [&tally, &leaky, &affine](float v) {                      // 引用捕获内层闭包
    tally(v);
    return affine(leaky(v));
};
elementwise_transform(fused, input, yLeakyAffine, totalLength);
```

随后注入另一个闭包即得到第二个算子：`elementwise_transform(clamp, input, yClamp, totalLength)`。

## 编译运行
在本样例根目录下执行如下步骤，编译并执行算子。
- 配置环境变量  
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/zh/quick_start.md#prepare&install)，配置环境变量。
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：**`${install_path}`为CANN包安装目录，未指定安装目录时默认安装至`/usr/local/Ascend`下。

- 样例执行

  在本样例目录下执行如下命令。
  ```bash
  mkdir -p build && cd build
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
  make -j
  ./demo
  ```

  使用NPU仿真模式时添加`-DCMAKE_ASC_RUN_MODE=sim`。
  示例如下：
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..; make -j;   # NPU仿真模式
  ```

  > **注意：** 切换编译模式前需清理cmake缓存，可在build目录下执行`rm CMakeCache.txt`后重新cmake。

  编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`sim` | 运行模式：NPU运行、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU 架构：本样例仅支持 dav-3510（Ascend 950PR&950DT系列产品） |

  执行结果如下，说明两个算子输出与负数统计全部校验通过。

  ```text
  [lambda_operator_capture] leaky-affine + clamp via capturing lambdas: PASS (y[5]=0.400000, yClamp[5]=-2.000000, negTotal=1862)
  ```