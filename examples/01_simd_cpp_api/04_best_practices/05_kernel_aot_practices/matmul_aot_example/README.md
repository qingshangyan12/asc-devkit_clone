# Matmul AOT 样例

## 概述

本样例在 Matmul 高阶 API 实现矩阵乘的基础上，引入 **AOT（Ahead-of-Time）编译优化**：通过在编译阶段将 Tiling 参数常量化，让编译器对 kernel 进行更激进的优化（循环展开、死代码消除、常量传播等），从而在运行时根据实际 Tiling 自动匹配预编译的特化版本，获得性能提升。

矩阵乘本身使用 Matmul 高阶 API 完成 `C = A × B` 的计算，A/B 为 half 类型，C 为 float 类型，规格固定为 `M=512, N=512, K=128`。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 支持的CANN软件版本

- \>= CANN 9.0.0

## 目录结构介绍

```
├── matmul_aot_example
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── aot_helper.h            // AOT 框架核心实现（Holder、Registry、Dispatcher）
│   └── matmul_aot_example.asc  // Ascend C 样例实现（含 kernel 和 host 调用）
```

## AOT 原理

### 核心思想

AOT 编译的核心思想是：**将运行时才确定的 Tiling 参数提前到编译阶段常量化**，让编译器能够进行更激进的优化，如：

- **循环展开（Loop Unrolling）**：基于常量边界展开 Matmul 内部循环
- **死代码消除（Dead Code Elimination）**：根据常量配置裁剪不需要的分支
- **常量传播（Constant Propagation）**：在编译期计算常量表达式，减少运行时计算
- **更好的指令调度**：利用常量信息优化 Cube 计算单元的指令排布

### 整体流程

```
┌─────────────────────────────────────────────────────────────┐
│                    编译阶段 (Compile Time)                     │
├─────────────────────────────────────────────────────────────┤
│  预定义 Tiling 常量值（字节数组形式）                            │
│    └── AOT_MatmulTiling_512x512x128_Value: M=512,N=512,K=128  │
│                                                              │
│  注册到 AOT 注册表                                             │
│    └── using MatmulTilingAOTRegistry = aot::AOTRegistry<...>   │
│                                                              │
│  编译时生成特化版本 + 通用版本                                  │
│    ├── matmul_custom<AOTHolder<...>> → 按常量优化              │
│    └── matmul_custom<RuntimeHolder<...>> → 通用运行时版本       │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                    运行阶段 (Runtime)                          │
├─────────────────────────────────────────────────────────────┤
│  1. Host 侧调用 GenerateTiling() 生成运行时 Tiling 数据         │
│  2. AOTDispatcher 将运行时 Tiling 与注册表中所有 AOT Holder     │
│     逐条 memcmp 对比                                           │
│  3. 匹配成功 → 调用对应的 AOT 特化 kernel 版本                  │
│     匹配失败 → 回退到 RuntimeHolder 通用版本                    │
└─────────────────────────────────────────────────────────────┘
```

## 代码详解

### 1. AOT Helper 框架（aot_helper.h）

本样例的 AOT 框架实现（`aot_helper.h`）包含以下核心组件，详细说明参见同目录下 [aot_helper.h](aot_helper.h) 源码。

| 组件 | 说明 |
|------|------|
| `aot::AOTHolder<T, ValuePtr>` | AOT 常量持有者：通过 `constexpr uint8_t[]` 字节数组定义编译期 Tiling 常量，并在模板参数中携带 |
| `aot::RuntimeHolder<T>` | 运行时持有者：标记通用（非 AOT）版本的占位类型 |
| `aot::AOTRegistry<Holders...>` | AOT 注册表：以类型参数包形式注册所有 AOT Holder，编译开销 O(1) |
| `aot::AOTDispatcher<T, Registry>` | 运行时分发器：基于 C++17 折叠表达式遍历注册表，`memcmp` 匹配后调用对应特化版本 |
| `aot::GetHolderDataRef<T, HT>(rt_data)` | 数据获取辅助函数：AOT 版本返回编译期常量引用，通用版本返回运行时数据引用 |

### 2. AOT Tiling 常量定义

```cpp
// Tiling 数据以原始字节数组形式定义为编译期常量
static constexpr uint8_t AOT_MatmulTiling_512x512x128_Value[] = {
    0x02, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00,
    0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, ...
};
```

该字节数组对应规格 `M=512, N=512, K=128` 下由 `MultiCoreMatmulTiling::GetTiling()` 生成的 `TCubeTiling` 完整内存布局，包含 usedCoreNum、singleCoreM、singleCoreN、singleCoreKa、singleCoreKb 等全部 tiling 字段。

> **如何获取常量值**：本样例在 Host 侧 `GenerateTiling()` 中会打印 tiling 原始字节数据（以 `0x%02x` 格式），将运行时输出直接拷贝为 `constexpr uint8_t[]` 即可作为 AOT 常量使用。

### 3. AOT 注册

```cpp
using MatmulTilingAOTRegistry = aot::AOTRegistry<
    // 此处可以有多个实例，以逗号分隔的list
    aot::AOTHolder<AscendC::tiling::TCubeTiling, AOT_MatmulTiling_512x512x128_Value>
>;
```

当前注册了 `512×512×128` 这一个配置。如需添加更多预编译配置，只需定义新的字节数组常量，并追加到注册表中：

```cpp
static constexpr uint8_t AOT_MatmulTiling_1024x1024x256_Value[] = { ... };
using AOT_MatmulTiling_1024x1024x256 = aot::AOTHolder<AscendC::tiling::TCubeTiling, AOT_MatmulTiling_1024x1024x256_Value>;

using MatmulTilingAOTRegistry = aot::AOTRegistry<
    aot::AOTHolder<AscendC::tiling::TCubeTiling, AOT_MatmulTiling_512x512x128_Value>,
    AOT_MatmulTiling_1024x1024x256
>;
```

无需修改 kernel 调用逻辑，`AOTDispatcher` 自动处理新增版本。

### 4. Kernel 模板设计

```cpp
template <typename TH=aot::RuntimeHolder<AscendC::tiling::TCubeTiling>>
__global__ __cube__ void matmul_custom(
    __gm__ uint8_t* a, __gm__ uint8_t* b, __gm__ uint8_t* c, __gm__ uint8_t* workspace,
    AscendC::tiling::TCubeTiling tilingInput)
{
    // AOT 版本编译期校验：确保 Holder 中的 value 在编译期完成常量化
    if constexpr (!std::is_same_v<TH, aot::RuntimeHolder<AscendC::tiling::TCubeTiling>>) {
        static_assert(TH::value.M != 0);
    }
    // 获取 Tiling 数据：AOT 版本返回编译期常量，通用版本返回运行时传入值
    const auto &tiling = aot::GetHolderDataRef<AscendC::tiling::TCubeTiling, TH>(tilingInput);
    // ... 后续 Matmul 计算逻辑使用 tiling 中的常量/变量 ...
}
```

关键点：
- 模板参数 `TH` 默认是 `RuntimeHolder`，AOT 分发时替换为具体的 `AOTHolder`
- `GetHolderDataRef` 对 AOT 版本直接返回编译期 `constexpr` 常量引用，编译器可据此进行常量传播、循环展开等优化
- `static_assert` 提供编译期守卫，确保 AOT Holder 中的值已正确初始化

### 5. 运行时分发

```cpp
auto tilPtr = reinterpret_cast<const uint8_t*>(&tiling);
aot::AOTDispatcher<AscendC::tiling::TCubeTiling, MatmulTilingAOTRegistry>::template dispatch(
    tilPtr,                             // 运行时 Tiling 字节指针
    [&](auto tiling_holder, AscendC::tiling::TCubeTiling value) {
        using Holder = decltype(tiling_holder);
        matmul_custom<Holder><<<numBlocks, nullptr, stream>>>(aDevice, bDevice, cDevice, workspaceDevice, tiling);
        if constexpr (std::is_same_v<Holder, aot::RuntimeHolder<AscendC::tiling::TCubeTiling>>) {
            printf("##### 使用运行时 Tiling\n");
        } else {
            static_assert(Holder::value.M != 0);
            printf("##### AOT matched! M=%u, N=%u, Ka=%u, Kb=%u\n",
                   Holder::value.M, Holder::value.N, Holder::value.Ka, Holder::value.Kb);
        }
    }
);
```

分发流程：
1. `AOTDispatcher::dispatch()` 接收运行时 Tiling 字节指针
2. 通过 C++17 折叠表达式，对注册表中每个 `AOTHolder` 执行 `memcmp` 逐字节比对
3. `||` 短路语义：首次命中即停止，未命中则调用 `RuntimeHolder` 通用版本
4. 每个 `try_one` 带 `__attribute__((always_inline))` 强制内联，避免 N 个 Holder 留下 N 份独立函数体

## 样例描述

### 矩阵乘功能（简述）

本样例使用 Ascend C Matmul 高阶 API 实现矩阵乘法 `C = A × B`。其中 A 矩阵形状为 `[M, K]`，B 矩阵形状为 `[K, N]`，输出 C 矩阵形状为 `[M, N]`。Matmul 高阶 API 封装了数据搬运、Cube 计算调度、基础流水同步等细节，开发者主要完成矩阵规格配置、tiling 生成、输入输出 Tensor 设置和结果写回。

- 样例规格：
  本样例参数 `M = 512, N = 512, K = 128`。输入 A、B 矩阵均为 `half` 类型、`ND` 格式，输出 C 矩阵为 `float` 类型、`ND` 格式。输入输出规格如下表所示：
  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">Matmul</td></tr>
  <tr><td rowspan="3" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">A</td><td align="center">[M, K]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td align="center">B</td><td align="center">[K, N]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">C</td><td align="center">[M, N]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">matmul_custom</td></tr>
  </table>

  样例为纯 Cube 矩阵计算场景，固定按 2 个 Cube 核生成 tiling。在本样例规格下，tiling 结果会把 `M = 512` 按 2 个核均分，每个核处理 `singleCoreM = 256`，`singleCoreN = 512`，`singleCoreKa = singleCoreKb = 128`。

### AOT 优化效果

当运行时的 Tiling 参数与 AOT 注册表中 `512×512×128` 配置匹配时，编译器在编译阶段已完成：

- **循环展开优化**：Matmul 内部分块循环根据常量 singleCoreM/N/Ka/Kb 完全展开
- **常量传播**：地址偏移 `GetBlockIdx() * tiling.singleCoreM * tiling.Ka` 等表达式可在编译期计算
- **死代码消除**：与常量配置无关的条件分支被裁剪
- **指令调度优化**：编译器可基于已知的分块大小做更优的指令排布

运行时输出示例：

```
##### AOT matched! M=512, N=512, Ka=128, Kb=128
```

若运行时 Tiling 参数发生变化（例如修改了 K 值），则自动回退到通用版本：

```
##### 使用运行时 Tiling
```

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量
  请根据当前环境上 CANN 开发套件包的安装方式，配置环境变量。
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为 CANN 包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。

- 样例执行

  在本样例目录下执行如下命令。
  ```bash
  mkdir -p build && cd build;                                               # 创建并进入 build 目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j;                      # 编译工程（默认 npu 模式）
  ./matmul_aot_example                                                      # 执行样例
  ```

  使用 NPU 仿真模式时，添加 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。

  示例如下：
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # NPU 仿真模式
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明
  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`sim` | 运行模式：NPU 运行、NPU 仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201`（默认）、`dav-3510` | NPU 架构：dav-2201 对应 Atlas A2 训练系列产品/Atlas A2 推理系列产品和 Atlas A3 训练系列产品/Atlas A3 推理系列产品，dav-3510 对应 Ascend 950PR/Ascend 950DT |

- 执行结果
  执行成功后会打印 AOT 匹配状态及输出矩阵前 16 个元素的值，若 Tiling 匹配 AOT 注册表则输出：
  ```bash
  ##### AOT matched! M=512, N=512, Ka=128, Kb=128
  =============== output c matrix[:16] ===============
  128.00 128.00 128.00 128.00 128.00 128.00 128.00 128.00 ...
  ```

## 扩展更多 AOT 配置

当需要使用其他矩阵规格的 AOT 优化时，按以下步骤操作：

1. **获取目标规格的 Tiling 字节数据**：修改 `GenerateTiling()` 中的 M/N/K 参数，运行一次样例，在输出中找到 `matmul raw tiling data` 段的字节数组
2. **定义 AOT 常量**：将字节数组定义为 `static constexpr uint8_t[]`
3. **创建 AOT Holder**：`using AOT_MatmulTiling_<规格> = aot::AOTHolder<AscendC::tiling::TCubeTiling, <常量名>>`
4. **更新注册表**：将新 Holder 追加到 `MatmulTilingAOTRegistry` 的类型参数包中

## 注意事项

1. **Tiling 字节兼容性**：`TCubeTiling` 的字节布局在不同 CANN 版本或不同架构下可能不同，需确保 AOT 常量与当前编译环境一致
2. **编译时间**：每个 AOT 特化版本都会增加编译时间和二进制体积（kernel 端按 N 线性增长，比 host dispatcher 更敏感），建议只针对高频使用的配置预编译
3. **运行时开销**：匹配过程使用 `memcmp` 逐条比对，注册表规模在约 1000 个 Holder 内仍是微秒级开销
4. **静态编译期校验**：AOT 版本通过 `static_assert(Holder::value.M != 0)` 确保常量已正确初始化
5. **纯 Cube 场景**：本样例使用 `ASCENDC_CUBE_ONLY` 宏，kernel 运行在 `__cube__` 计算单元上，仅适用于纯矩阵乘场景
