# 以LeakyReLU-Affine融合与Clamp算子为载体的SIMD与SIMT混合编程Lambda捕获样例

## 概述

本样例是[SIMT编程版Lambda捕获样例](../../../03_simt_api/02_features/02_language_features/00_lambda_operator_capture/README.md)在**SIMD与SIMT混合编程**下的对照实现：同一组算子，即LeakyReLU-Affine融合算子（`y = scale * leaky_relu(x, alpha) + bias`，同遍历统计负数个数）与Clamp算子（`y = clamp(x, lo, hi)`），在SIMD与SIMT混合编程下按各执行上下文的Lambda标记规则实现，展示Lambda捕获在SIMD与SIMT混合编程算子开发中的实际编程作用：

1. **SIMT VF内Lambda标记`__simt_callee__`**：值捕获LeakyReLU斜率与仿射系数完成"激活→仿射"融合；引用捕获线程私有负数计数器完成同遍历统计，每线程计数经`asc_atomic_add`原子累加为本核计数。
2. **SIMD VF内Lambda标记`__simd_callee__`**：值捕获Clamp上下界参与边界计算，标量结果在矢量循环前算好，经`asc_duplicate_scalar`生成上下界矢量寄存器，再以`asc_min`与`asc_max`矢量intrinsics完成连续向量裁剪，这是“Lambda标量逻辑与矢量intrinsics结合”的标准数据流。
3. **核函数体内Lambda标记`__aicore__`**：核函数体即`__global__ __vector__`的SIMD侧scalar代码，Lambda按核号计算本核数据分片，供SIMT VF与SIMD VF复用。
4. **策略与框架分离**：SIMT VF内的通用`elementwise_simt`框架是`__simt_callee__`模板函数，只负责线程保护与逐元素分发，融合闭包经模板形参注入。

> [!IMPORTANT]
> SIMD与SIMT混合编程场景下，各执行上下文中定义的Lambda**必须显式标记函数属性**，否则编译报错：`__simt_vf__`函数体内标记`__simt_callee__`、`__simd_vf__`函数体内标记`__simd_callee__`、SIMD与SIMT混合编程核函数体内标记`__aicore__`。标记位于Lambda参数列表之后；与`mutable`共存时，标记位于`mutable`之前。完整标记选择规则见[Lambda表达式](../../../../docs/zh/guide/technical_appendix/cpp_standard_support/cpp_language_feature/lambda_expressions.md)文档。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|---|---|
| Ascend 950PR&950DT系列产品 | \>= CANN 9.3.0 |

## 目录结构介绍

```text
├── lambda_operator_hybrid
│   ├── CMakeLists.txt             // cmake编译文件
│   ├── lambda_operator_hybrid.asc // 以LeakyReLU-Affine与Clamp为载体的SIMD与SIMT混合编程Lambda捕获样例实现
│   └── README.md                  // 样例说明文档
```

## 样例实现

### 执行流程

Host生成取值覆盖`[-5, 5]`的输入`x[i] = (i*7) % 11 - 5`，覆盖正数、负数与上下界越界点，启动8个Vector Core、每核1024元素，共8192元素，单次核启动完成两个算子与负数统计：

| 阶段 | 编程方式 | Lambda捕获作用 | 输出期望 |
|---|---|---|---|
| SIMT VF `simt_leaky_affine_vf` | 每核1024线程，每线程1元素 | `leaky`与`affine`值捕获超参；`tally`引用捕获线程私有计数；`fused`引用捕获三个内层闭包单遍融合；`asc_atomic_add`合并本核计数 | `y1[i] = scale*leaky_relu(x[i],alpha)+bias`，`y1[5]=0.4` |
| SIMD VF `simd_clamp_vf` | 矢量intrinsics连续计算 | 上下界经`asc_duplicate_scalar`进矢量寄存器，`asc_min`与`asc_max`完成裁剪 | `y2[i] = clamp(x[i], -2, 3)`，`y2[5]=-2.0` |
| 核函数体scalar代码 | `__aicore__` lambda | `per_block`按核号计算本核分片，并将本核GM输入分片搬入UB供两个VF共用 | 每核计数`negCount[b]`，合计`negTotal=3723` |

数据流：GM输入分片经`asc_copy_gm2ub_align`搬入UB → SIMT VF以该UB输入完成融合与统计 → SIMD VF以同一UB输入完成Clamp计算 → `asc_copy_ub2gm_align`将两份UB结果搬回GM。两个算子均作用于原始输入`x[i]`，`neg_count`按核偏移写入，各核独立计数。

### 关键代码说明

**SIMT VF内的闭包构造**：

```cpp
auto leaky  = [alpha](float x) __simt_callee__ { return x > 0.0f ? x : alpha * x; };
auto affine = [scale, bias](float x) __simt_callee__ { return x * scale + bias; };
uint32_t neg_local = 0;
auto tally  = [&neg_local](float x) __simt_callee__ { if (x < 0.0f) { ++neg_local; } };
auto fused  = [&tally, &leaky, &affine](float x) __simt_callee__ {
    tally(x);
    return affine(leaky(x));
};
elementwise_simt(fused, output, v);          // 闭包经模板形参注入框架
asc_atomic_add(neg_count, neg_local);        // 线程计数原子合并为本核计数
```

**SIMD VF内的标量与矢量数据流**：callee-Lambda的标量结果参与矢量计算时，在矢量循环前计算并作为操作数进入，比如本样例的上下界经`asc_duplicate_scalar`广播为矢量寄存器后，`asc_min`与`asc_max`在寄存器级完成裁剪。

**流水同步**：SIMT VF与SIMD VF写UB后`asc_copy_ub2gm_align`搬出前，需`asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0)`与`asc_sync_wait(...)`同步Vector计算流水和MTE3搬出流水。


## 编译运行

在本样例根目录下执行如下步骤，编译并执行算子。
- 配置环境变量  
  请根据当前环境上CANN开发套件包的[安装方式](../../../../docs/zh/quick_start.md#prepare&install)，配置环境变量。
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

  ```text
  [lambda_operator_hybrid] leaky-affine(SIMT VF) + clamp(SIMD VF) via capturing lambdas: PASS (y[5]=0.400000, yClamp[5]=-2.000000, negTotal=3723)
  ```

