# load_data_2dmx_l12l0 样例

## 概述

本样例介绍了在 MX FP4/FP8 矩阵乘法场景下，使用 `Load2Dv2` 接口搬运 A/B 矩阵数据，使用 `Load2DMX` 接口搬运 scaleA/scaleB 矩阵数据的方法，共 6 种场景：
- 使用 `Load2Dv2` API搬运 A/B 矩阵数据
- 使用 `Load2DMX` API搬运 scale 矩阵数据
- L0上A矩阵的数据排布为 **Nz 格式**，B数据排布为 **Zn 格式**
- L0上scaleA矩阵的数据排布为 **Zz 格式**，scaleB数据排布为 **Nn 格式**
- 支持 FP4 (fp4x2_e1m2_t / fp4x2_e2m1_t) 和 FP8 (fp8_e4m3fn_t / fp8_e5m2_t) 数据类型组合
- Scale 矩阵使用 fp8_e8m0_t 数据类型

## 支持的产品
- Atlas 350 加速卡（Ascend 950PR/Ascend 950DT）

## 目录结构

```
├── load_data_2dmx_l12l0
│   ├── scripts
│   │   ├── gen_data.py             // 输入数据和真值数据生成脚本
│   │   └── verify_result.py        // 验证输出数据和真值数据是否一致
│   ├── CMakeLists.txt              // 编译工程文件
│   ├── data_utils.h                // 数据读入写出函数
│   ├── load_data_2dmx_l12l0.asc    // Ascend C样例实现 & 调用样例
│   └── README.md                   // 样例说明文档
```

## 样例描述

### 样例功能

样例整体流程如下：

```
GM(ND) -> L1(Nz) -> L0A(Nz)/L0B(Zn) -> L0C(Nz) -> GM(ND)
        │         │                  │          │       
     DataCopy  Load2Dv2            Mmad        Fixpipe 
               Load2DMX
```

**步骤详解**：

1. **GM → L1**：
   - 使用 `DataCopy` 实现 ND 到 Nz 格式转换（A/B 矩阵）
   - 使用 `DataCopy` 实现 ND 到 Nz 格式转换（scale 矩阵，按 b16 视图搬运）
2. **L1 → L0A/L0B**：
   - 使用 `Load2Dv2` 接口搬运 A/B 矩阵数据
   - 使用 `Load2DMX` 接口搬运 scale 矩阵数据
3. **矩阵乘**：使用 `Mmad` 接口执行 MX 矩阵乘法
4. **L0C → GM**：使用 `Fixpipe` 接口搬出结果

### 场景说明

所有场景基于相同的矩阵乘规格：[m, n, k] = [40, 50, 70]，核函数名称为 "KernelLoadDataL12L0Load2DMX"。

<a name="表1"></a>
<table border="2">
<caption style="font-weight: normal;">
    <span style="font-weight: bold; font-size: 1.2em;">📌 表1：scenarioNum 不同取值的含义</span>
  <tr>
    <td><span style="font-weight: bold;">scenarioNum</span></td>
    <td><span style="font-weight: bold;">数据类型</span></td>
    <td><span style="font-weight: bold;">A矩阵类型</span></td>
    <td><span style="font-weight: bold;">B矩阵类型</span></td>
    <td><span style="font-weight: bold;">isScaleAtranspose</span></td>
    <td><span style="font-weight: bold;">isScaleBtranspose</span></td>
    <td><span style="font-weight: bold;">搬运方式</span></td>
  </tr>
  <tr>
    <td><span style="font-weight: bold;">1</span></td>
    <td>FP4</td>
    <td>fp4x2_e1m2_t</td>
    <td>fp4x2_e2m1_t</td>
    <td>false</td>
    <td>true</td>
    <td>单次调用 Load2Dv2 + Load2DMX</td>
  </tr>
  <tr>
    <td><span style="font-weight: bold;">2</span></td>
    <td>FP4</td>
    <td>fp4x2_e2m1_t</td>
    <td>fp4x2_e1m2_t</td>
    <td>false</td>
    <td>false</td>
    <td>单次调用 Load2Dv2 + Load2DMX</td>
  </tr>
  <tr>
    <td><span style="font-weight: bold;">3</span></td>
    <td>FP8</td>
    <td>fp8_e4m3fn_t</td>
    <td>fp8_e5m2_t</td>
    <td>false</td>
    <td>true</td>
    <td>单次调用 Load2Dv2 + Load2DMX</td>
  </tr>
  <tr>
    <td><span style="font-weight: bold;">4</span></td>
    <td>FP8</td>
    <td>fp8_e5m2_t</td>
    <td>fp8_e4m3fn_t</td>
    <td>false</td>
    <td>false</td>
    <td>单次调用 Load2Dv2 + Load2DMX</td>
  </tr>
  <tr>
    <td><span style="font-weight: bold;">5</span></td>
    <td>FP4</td>
    <td>fp4x2_e1m2_t</td>
    <td>fp4x2_e2m1_t</td>
    <td>true</td>
    <td>false</td>
    <td>for循环 + Load2Dv2 + Load2DMX</td>
  </tr>
  <tr>
    <td><span style="font-weight: bold;">6</span></td>
    <td>FP8</td>
    <td>fp8_e4m3fn_t</td>
    <td>fp8_e5m2_t</td>
    <td>true</td>
    <td>false</td>
    <td>for循环 + Load2Dv2 + Load2DMX</td>
  </tr>
</table>

**注**：
- **场景 1-4**：使用单次 Load2Dv2 和 Load2DMX 调用完成 L1 → L0 的搬运
- **场景 5-6**：isScaleAtranspose=true，需要转置，使用 for 循环方式搬运，避免多搬脏数据
- **矩阵形状**：
  - A矩阵：[40, 70]，不转置输入
  - B矩阵：[50, 70]，转置输入（即 GM 上存储的是 [70, 50] 的转置矩阵）
  - ScaleA矩阵：[40, ceil(70/32)] = [40, 4]，按场景决定是否转置
  - ScaleB矩阵：[50, ceil(70/32)] = [50, 4]，按场景决定是否转置
  - 输出C矩阵：[40, 50]

### MX 数据类型说明

MX 格式（Microscaling）是一种块数据格式，每 32 个元素共享一个缩放因子。

<a name="表2"></a>
<table border="2">
<caption style="font-weight: normal;">
    <span style="font-weight: bold; font-size: 1.2em;">📌 表2：MxMatmul 支持的数据类型组合</span>
  <tr>
    <td><span style="font-weight: bold;">A矩阵</span></td>
    <td><span style="font-weight: bold;">B矩阵</span></td>
    <td><span style="font-weight: bold;">Scale矩阵</span></td>
    <td><span style="font-weight: bold;">块大小(k)</span></td>
  </tr>
  <tr>
    <td>fp4x2_e1m2_t</td>
    <td>fp4x2_e1m2_t / fp4x2_e2m1_t</td>
    <td>fp8_e8m0_t</td>
    <td>32</td>
  </tr>
  <tr>
    <td>fp4x2_e2m1_t</td>
    <td>fp4x2_e2m1_t / fp4x2_e1m2_t</td>
    <td>fp8_e8m0_t</td>
    <td>32</td>
  </tr>
  <tr>
    <td>fp8_e4m3fn_t</td>
    <td>fp8_e4m3fn_t / fp8_e5m2_t</td>
    <td>fp8_e8m0_t</td>
    <td>32</td>
  </tr>
  <tr>
    <td>fp8_e5m2_t</td>
    <td>fp8_e4m3fn_t / fp8_e5m2_t</td>
    <td>fp8_e8m0_t</td>
    <td>32</td>
  </tr>
</table>

### 样例实现

#### Load2DMX 接口参数说明

`Load2DMX` 接口用于将 scale 矩阵数据从 L1 搬运到 L0A/L0B，参数结构体 `LoadData2DMxParams` 包含：

- **xStartPosition**: 源矩阵 X 轴方向（M 维度）起始位置，单位为 1 个分形（16元素）
- **yStartPosition**: 源矩阵 Y 轨方向（K 维度）起始位置，单位为 32B
- **xStep**: 源矩阵 X 轴方向搬运长度，单位为 1 个分形
- **yStep**: 源矩阵 Y 轴方向搬运长度，单位为 32B
- **srcStride**: 源矩阵 X 方向相邻分形起始地址间隔，单位为 32B
- **dstStride**: 目标矩阵 X 方向相邻分形起始地址间隔，单位为 32B

#### 场景 1-4：单次 Load2DMX 搬运

```cpp
AscendC::LoadData2DParamsV2 loadDataParams;
loadDataParams.mStartPosition = 0;
loadDataParams.kStartPosition = 0;
loadDataParams.mStep = CeilDivision(mAlignL1, fractalShape[0]);
loadDataParams.kStep = CeilDivision(kaAlignL1, fractalShape[1]);
loadDataParams.srcStride = CeilDivision(mAlignL1, fractalShape[0]);
loadDataParams.dstStride = CeilDivision(mAlignL0, fractalShape[0]);
loadDataParams.ifTranspose = false;

AscendC::LoadData2DMxParams loadMxDataParams;
loadMxDataParams.xStartPosition = 0;
loadMxDataParams.yStartPosition = 0;
loadMxDataParams.xStep = CeilDivision(mAlignL0, CUBE_BLOCK);
loadMxDataParams.yStep = CeilDivision(packedK, SCALE_CEIL_NUMBER);
loadMxDataParams.srcStride = scaleK;
loadMxDataParams.dstStride = CeilDivision(packedK, SCALE_CEIL_NUMBER);

AscendC::LoadData(a2Local, a1Local, scaleA1Local, loadDataParams, loadMxDataParams);
```

#### 场景 5-6：for 循环 + Load2DMX 搬运

当 isScaleAtranspose=true 时，需要使用 for 循环方式避免搬运多余的脏数据：

```cpp
uint16_t mStepAlign = isFP4 ? FP4_M_STEP_ALIGN : FP8_M_STEP_ALIGN;
uint16_t L0ALoopNum = CeilDivision(kaAlignL0, fractalShape[0] * fractalNum);

loadDataParams.mStep = mStepAlign;
loadDataParams.kStep = CeilDivision(mAlignL0, fractalShape[1]);
loadDataParams.ifTranspose = true;

uint32_t dstOffset = 0;
for (uint16_t loopIdx = 0; loopIdx < L0ALoopNum; ++loopIdx) {
    loadDataParams.mStartPosition = mStepAlign * loopIdx;

    AscendC::LoadData2DMxParams loadMxDataParams;
    loadMxDataParams.xStartPosition = mStepAlign * loopIdx;
    loadMxDataParams.yStartPosition = 0;
    loadMxDataParams.xStep = mStepAlign;
    loadMxDataParams.yStep = CeilDivision(packedK, SCALE_CEIL_NUMBER);
    loadMxDataParams.srcStride = scaleK;
    loadMxDataParams.dstStride = CeilDivision(packedK, SCALE_CEIL_NUMBER);

    AscendC::LoadData(a2Local[dstOffset], a1Local, scaleA1Local, loadDataParams, loadMxDataParams);
    dstOffset += CeilAlign(mAlignL0, fractalShape[0]) * fractalShape[1];
}
```

## 编译运行

### 配置环境变量

```bash
# 默认路径，root用户安装CANN软件包
source /usr/local/Ascend/cann/set_env.sh

# 默认路径，非root用户安装CANN软件包
source $HOME/Ascend/cann/set_env.sh

# 指定路径install_path，安装CANN软件包
source ${install_path}/cann/set_env.sh

# 使用 ks_cann 环境（支持 dav-3510 架构）
source /mnt/workspace/gitCode/cann/ks_cann/cann/set_env.sh
```

### 样例执行

```bash
SCENARIO=1
mkdir -p build && cd build;
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO ..;make -j;
python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO
./demo
python3 ../scripts/verify_result.py -scenarioNum=$SCENARIO output/output.bin output/golden.bin
```

使用 CPU调试 或 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。

```bash
cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO ..;make -j;   # CPU调试模式
cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO ..;make -j;   # NPU仿真模式
```

> **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

### 编译选项说明

| 选项 | 可选值 | 说明 |
|------|--------|------|
| `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU 运行、CPU调试、NPU仿真 |
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU 架构：Atlas 350 加速卡 |
| `SCENARIO_NUM` | `1`-`6` | 场景编号：不同数据类型和 scale 转置组合 |

### 执行结果

执行结果如下，说明精度对比成功。
```bash
test pass!
```
