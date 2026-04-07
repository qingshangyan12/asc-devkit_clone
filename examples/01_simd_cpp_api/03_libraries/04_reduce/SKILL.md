---
name: multi-scenario-sample-pattern
description: 多场景样例合并技能。用于将多个相似样例合并为一个多场景样例，通过场景编号控制不同实现。
---

# 多场景样例合并技能

## 技能描述

本技能用于将多个相似样例合并为一个多场景样例，通过场景编号（scenarioNum）控制不同实现。帮助开发者遵循多场景样例的最佳实践，包括宏配置、条件编译、脚本文件处理等。

## 触发时机

当用户需要将多个相似样例合并时使用本技能：
- 识别可合并的相似样例
- 检查合并样例的规范性
- 验证场景实现的完整性
- 提供多场景样例的参考实现

## 合并原则

### 1. 样例识别

**可合并样例的特征**：
- 功能相似，仅在参数或配置上有差异
- 数据流路径相同
- 使用相同的API接口
- 输入输出shape和类型相同或兼容

**不可合并样例的特征**：
- 功能完全不同
- 数据流路径差异较大
- 使用不同的API接口
- 输入输出shape和类型不兼容

### 2. 场景编号管理

**要求**：
- 场景编号从1开始，连续且无间断
- 每个被合并的样例对应一个场景编号
- 场景编号在编译时通过宏传递

**示例**：
- scalar_quantized_activation_during → scenarioNum=1
- tensor_quantized_activation_during → scenarioNum=2

### 3. 文件处理策略

#### .asc文件处理

**要求**：
- 使用`constexpr uint32_t scenarioNum = SCENARIO_NUM;`接收场景编号
- 使用`if constexpr (scenarioNum == X)`进行条件编译
- 保留原始样例的注释
- 为每个场景分支添加清晰的注释说明

**示例**：
```cpp
constexpr uint32_t scenarioNum = SCENARIO_NUM;

__cube__ __global__ void KernelFixpipeCustom(GM_ADDR fmData, GM_ADDR weData,
                                              GM_ADDR deqTensor, GM_ADDR elewiseTensor,
                                              GM_ADDR tiling, GM_ADDR outputData)
{
    if constexpr (scenarioNum == 1 || scenarioNum == 2) {
        TestDataCopyL0c2Gm<int8_t, int8_t, int32_t, half> op;
        AscendC::TPipe pipe;
        op.Init(fmData, weData, deqTensor, elewiseTensor, tiling, outputData, &pipe);
        op.Process();
    } else {
        AscendC::printf("not support!");
    }
}
```

**场景分支示例**：
```cpp
if constexpr (scenarioNum == 1) {
    // 场景1：Scalar量化模式
    float tmp = 0.5f; // deq factor
    uint64_t deqScalar = static_cast<uint64_t>(*reinterpret_cast<int32_t*>(&tmp));
    AscendC::SetFixpipePreQuantFlag(deqScalar);
    AscendC::PipeBarrier<PIPE_FIX>();
} else if constexpr (scenarioNum == 2) {
    // 场景2：Tensor量化模式
    AscendC::LocalTensor<uint64_t> cbufWorkspace = inQueueCubeWorkspace.AllocTensor<uint64_t>();
    // GM -> L1
    AscendC::DataCopy(cbufWorkspace, mDeqTensorGm,
            {1, static_cast<uint16_t>(deqDataSize / AscendC::ONE_BLK_SIZE), 0, 0});
    set_flag(PIPE_MTE2, PIPE_FIX, EVENT_ID3);
    wait_flag(PIPE_MTE2, PIPE_FIX, EVENT_ID3);
    AscendC::LocalTensor<uint64_t> deqTensorTempBuf = inQueueDeqFB.AllocTensor<uint64_t>(); // deq tensor
    uint16_t fbufBurstLen = deqDataSize / 128;
    AscendC::DataCopyParams dataCopyParams(1, fbufBurstLen, 0, 0);
    // L1 -> Fixpipe
    AscendC::DataCopy(deqTensorTempBuf, cbufWorkspace, dataCopyParams);
    AscendC::SetFixPipeConfig(deqTensorTempBuf);
    AscendC::PipeBarrier<PIPE_FIX>();
    inQueueDeqFB.FreeTensor(deqTensorTempBuf);
    inQueueCubeWorkspace.FreeTensor(cbufWorkspace);
}
```

#### CMakeLists.txt处理

**要求**：
- 使用`option(SCENARIO_NUM "X" "场景描述")`定义场景
- 通过`-DSCENARIO_NUM=${SCENARIO_NUM}`传递场景编号
- 保留原始样例的编译配置

**示例**：
```cmake
# 场景配置
option(SCENARIO_NUM "1" "Scalar量化模式")
option(SCENARIO_NUM "2" "Tensor量化模式")

# 编译选项
add_compile_options(-DSCENARIO_NUM=${SCENARIO_NUM})
```

#### scripts/gen_data.py处理

**要求**：
- 保留所有被合并样例的共同函数
- 将gen_golden_data()改为gen_golden_data(scenarioNum)，接收场景编号参数
- 根据scenarioNum选择不同的参数配置
- 使用argparse接收scenarioNum参数
- 保留原始样例的注释

**共同函数示例**：
```python
def ceil_div(a_value, b_value):
    """
    ceil division
    """
    return (a_value + b_value - 1) // b_value

def clear_last_10bits_of_float32(fp32_data):
    int32_data = fp32_data.view("uint32")
    for index, data in enumerate(int32_data):
        int32_data[index] = = np.bitwise_and(data, 0xFFFFE000)
    dealed_data = int32_data.view("float32")
    return dealed_data

def copy_conv2d(fm_shape, k_shape, fm_tensor_dtype, weight_tensor_dtype, output_l0c_dtype,
                deq_tensor_dtype, output_gm_dtype, stride_list, pad_list, pad_value, dilation_list,
                if_relu=False, deq_mode=None, deq_value=1.0, dst_src_stride=0, if_nz2nd=False,
                if_split=False, clip_relu=0, ele_wise=0):
    # 共同实现
    pass

def copy_conv2d_gen_data(params):
    # 共同实现
    pass
```

**场景参数配置示例**：
```python
def gen_golden_data(scenarioNum):
    """
    生成测试输入数据和真值数据
    Args:
        scenarioNum: 场景编号(1=Scalar量化,2=Tensor量化)
    """
    if scenarioNum == 1:
        my_params = {"fm_shape": [1, 4, 4, 32], "weight_shape": [1, 2, 2, 128, 32],
                    "fm_type": np.int8, "weight_type": np.int8,
                    "dst_l0c_type": "float32", "deq_dtype": np.float32, "dst_gm_type": np.float16,
                    "stride_list": [1, 1], "pad_list": [0, 0, 0, 0],
                    "dilation_list": [1, 1], "pad_value": 0,
                    "quantize_params": {"mode": "int322fp16", "mode_param": 0.5}, "kernel_name": "cce_copy_conv2d",
                    "deq_type": "scalar", "relu": True, "nz2nd": True, "channel_split": False,
                    "init_l1out": True, "clip_relu": 0, "ele_wise": 0, "elewise_op": 0}
    else:
        my_params = {"fm_shape": [1, 4, 4, 32], "weight_shape": [1, 2, 2, 128, 32],
                    "fm_type": np.int8, "weight_type": np.int8,
                    "dst_l0c_type": "float32", "deq_dtype": np.uint64, "dst_gm_type": np.float16,
                    "stride_list": [1, 1], "pad_list": [0, 0, 0, 0],
                    "dilation_list": [1, 1], "pad_value": 0,
                    "quantize_params": {"mode": "int322fp16", "mode_param": 1}, "kernel_name": "cce_copy_conv2d",
                    "deq_type": "tensor", "relu": True, "nz2nd": True, "channel_split": False,
                    "init_l1out": True, "clip_relu": 0, "ele_wise": 0, "elewise_op": 0}

    fm_data, we_data, deq_tensor_value, elewise_tensor, golden_data = copy_conv2d_gen_data(my_params)

    tiling = np.zeros([16]).astype(np.uint32)
    tiling[0] = 1
    tiling[1] = 4
    tiling[2] = 4
    tiling[3] = 2
    tiling[4] = 2
    tiling[5] = 128
    tiling[6] = 32
    tiling[7] = 1
    tiling[8] = 1
    if scenarioNum == 1:
        tiling[9] = 11  # DEQF16
    else:
        tiling[9] = 10  # VDEQF16
    tiling[10] = True
    tiling[11] = True
    tiling[12] = False
    tiling[13] = 0
    tiling[14] = 0

    os.makedirs("input", exist_ok=True)
    os.makedirs("output", exist_ok=True)

    fm_data.tofile("./input/fm_data.bin")
    we_data.tofile("./input/we_data.bin")
    deq_tensor_value.tofile("./input/deq_data.bin")
    elewise_tensor.tofile("./input/elewise_data.bin")
    tiling.tofile("./input/tiling_data.bin")
    golden_data.tofile("./output/golden_data.bin")

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-scenarioNum', type=int, default=1, choices=range(1, 3))
    args = parser.parse_args()
    gen_golden_data(args.scenarioNum)
```

#### scripts/verify_result.py处理

**要求**：
- 保留所有被合并样例的共同函数
- 根据scenarioNum选择不同的验证逻辑（如有差异）
- 使用argparse接收scenarioNum参数

#### README.md处理

**要求**：
- 添加场景对照表，说明scenarioNum与场景的对应关系
- 概述部分说明支持多种场景
- 编译运行部分增加场景配置说明
- 样例规格部分说明不同场景的参数差异

**场景对照表示例**：
```markdown
## 场景说明

| scenarioNum | 量化模式 | 描述 |
|-----------|----------|------|
| 1 | Scalar量化 | 使用SetFixpipePreQuant接口设置Scalar量化参数，量化模式为DEQF16，量化参数为0.5 |
| 2 | Tensor量化 | 使用SetFixPipeConfig接口设置Tensor量化参数，量化模式为VDEQF16，量化参数为1 |
```

**编译运行示例**：
```markdown
## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。
- 配置环境变量
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
  - 默认路径，root用户安装CANN软件包
    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - 默认路径，非root用户安装CANN软件包
    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - 指定路径install_path，安装CANN软件包
    ```bash
    source ${install_path}/cann/set_env.sh
    ```

- 样例执行
  ```bash
  SCENARIO=1
  mkdir -p build && cd build;   # 创建并进入build目录
  cmake .. -DSCENARIO_NUM=$SCENARIO;make -j;    # 编译工程
  python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO   # 生成测试输入数据
  ./demo                        # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden_data.bin   # 验证输出结果是否正确，确认算法逻辑正确
  ```
  执行结果如下，说明精度对比成功：
  ```bash
  test pass!
  ```
```

## 合并步骤

### 步骤 1：识别可合并样例

检查样例目录，识别可合并的相似样例：
- 比较README.md中的功能描述
- 比较.asc文件的数据流路径
- 比较使用的API接口
- 比较输入输出规格

### 步骤 2：确定场景编号

为每个被合并的样例分配场景编号：
- 从1开始，连续编号
- 记录场景编号与样例的对应关系

### 步骤 3：合并.asc文件

1. 创建新的.asc文件，添加版权声明和文件描述
2. 添加`constexpr uint32_t scenarioNum = SCENARIO_NUM;`
3. 合并所有样例的共同代码（如结构体、类定义）
4. 使用`if constexpr (scenarioNum == X)`包装场景差异代码
5. 为每个场景分支添加清晰的注释说明
6. 在核函数中使用`if constexpr`检查场景编号

### 步骤 4：合并CMakeLists.txt

1. 使用`option(SCENARIO_NUM "X" "场景描述")`定义场景
2. 通过`-DSCENARIO_NUM=${SCENARIO_NUM}`传递场景编号
3. 保留原始样例的编译配置

### 步骤 5：合并scripts/gen_data.py

1. 保留所有被合并样例的共同函数
2. 将gen_golden_data()改为gen_golden_data(scenarioNum)
3. 根据scenarioNum选择不同的参数配置
4. 使用argparse接收scenarioNum参数
5. 保留原始样例的注释

### 步骤 6：合并scripts/verify_result.py

1. 保留所有被合并样例的共同函数
2. 根据scenarioNum选择不同的验证逻辑（如有差异）
3. 使用argparse接收scenarioNum参数

### 步骤 7：合并README.md

1. 添加场景对照表
2. 概述部分说明支持多种场景
3. 编译运行部分增加场景配置说明
4. 样例规格部分说明不同场景的参数差异

## 检查规范

### 1. .asc文件检查

**要求**：
- ✅ 使用`constexpr uint32_t scenarioNum = SCENARIO_NUM;`
- ✅ 使用`if constexpr (scenarioNum == X)`进行条件编译
- ✅ 每个场景分支都有清晰的注释说明
- ✅ 场景编号连续且无间断
- ✅ 所有场景都有对应的代码分支

### 2. CMakeLists.txt检查

**要求**：
- ✅ 使用`option(SCENARIO_NUM "X" "场景描述")`定义场景
- ✅ 通过`-DSCENARIO_NUM=${SCENARIO_NUM}`传递场景编号
- ✅ 场景编号连续且无间断

### 3. scripts/gen_data.py检查

**要求**：
- ✅ gen_golden_data()接收scenarioNum参数
- ✅ 使用argparse接收scenarioNum参数
- ✅ 根据scenarioNum选择不同的参数配置
- ✅ 保留所有被合并样例的共同函数

### 4. README.md检查

**要求**：
- ✅ 添加场景对照表
- ✅ 概述部分说明支持多种场景
- ✅ 编译运行部分增加场景配置说明
- ✅ 样例规格部分说明不同场景的参数差异

## 合并示例

### 示例：data_copy_quantized_activation_during

**目录**：`examples/01_simd_cpp_api/02_features/03_basic_api/00_data_movement/data_copy_quantized_activation_during`

**被合并样例**：
- scalar_quantized_activation_during → scenarioNum=1
- tensor_quantized_activation_during → scenarioNum=2

**合并结果**：

#### 1. .asc文件

```cpp
constexpr uint32_t scenarioNum = SCENARIO_NUM;

__cube__ __global__ void KernelFixpipeCustom(GM_ADDR fmData, GM_ADDR weData,
                                              GM_ADDR deqTensor, GM_ADDR elewiseTensor,
                                              GM_ADDR tiling, GM_ADDR outputData)
{
    if constexpr (scenarioNum == 1 || scenarioNum == 2) {
        TestDataCopyL0c2Gm<int8_t, int8_t, int32_t, half> op;
        AscendC::TPipe pipe;
        op.Init(fmData, weData, deqTensor, elewiseTensor, tiling, outputData, &pipe);
        op.Process();
    } else {
        AscendC::printf("not support!");
    }
}
```

#### 2. CMakeLists.txt

```cmake
# 场景配置
option(SCENARIO_NUM "1" "Scalar量化模式")
option(SCENARIO_NUM "2" "Tensor量化模式")

# 编译选项
add_compile_options(-DSCENARIO_NUM=${SCENARIO_NUM})
```

#### 3. scripts/gen_data.py

```python
def gen_golden_data(scenarioNum):
    """
    生成测试输入数据和真值数据
    Args:
        scenarioNum: 场景编号(1=Scalar量化,2=Tensor量化)
    """
    if scenarioNum == 1:
        my_params = {"deq_dtype": np.float32, "mode_param": 0.5}
    else:
        my_params = {"deq_dtype": np.uint64, "mode_param": 1}

    # ... 生成数据

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('-scenarioNum', type=int, default=1, choices=range(1, 3))
    args = parser.parse_args()
    gen_golden_data(args.scenarioNum)
```

#### 4. README.md

```markdown
## 场景说明

| scenarioNum | 量化模式 | 描述 |
|-----------|----------|------|
| 1 | Scalar量化 | 使用SetFixpipePreQuant接口设置Scalar量化参数，量化模式为DEQF16，量化参数为0.5 |
| 2 | Tensor量化 | 使用SetFixPipeConfig接口设置Tensor量化参数，量化模式为VDEQF16，量化参数为1 |
```

## 最佳实践建议

### 1. 样例识别

1. **功能相似性**
   - 确保被合并样例的功能相似
   - 仅在参数或配置上有差异
   - 避免合并功能完全不同的样例

2. **数据流一致性**
   - 确保数据流路径相同
   - 使用相同的API接口
   - 输入输出shape和类型相同或兼容

### 2. 场景编号管理

1. **编号规则**
   - 场景编号从1开始，连续且无间断
   - 使用`SCENARIO_NUM`宏传递场景编号
   - 避免使用硬编码的场景编号

2. **场景映射**
   - 记录场景编号与样例的对应关系
   - 在README.md中提供场景对照表
   - 确保每个场景都有清晰的描述

### 3. 文件处理

1. **.asc文件**
   - 使用`if constexpr`进行条件编译
   - 为每个场景分支添加清晰的注释说明
   - 保留原始样例的注释

2. **CMakeLists.txt**
   - 使用`option`定义场景
   - 通过`-DSCENARIO_NUM`传递场景编号
   - 保留原始样例的编译配置

3. **scripts/gen_data.py**
   - 保留所有被合并样例的共同函数
   - 根据scenarioNum选择不同的参数配置
   - 使用argparse接收scenarioNum参数

4. **README.md**
   - 添加场景对照表
   - 概述部分说明支持多种场景
   - 编译运行部分增加场景配置说明

### 4. 代码组织

1. **条件编译**
   - 使用`if constexpr`进行条件编译
   - 避免运行时开销
   - 确保每个场景都有对应的代码分支

2. **注释说明**
   - 为每个场景分支添加清晰的注释说明
   - 保留原始样例的注释
   - 在README.md中提供详细的场景描述

## 常见问题类型

### 1. 样例识别问题

- **问题**：功能完全不同的样例被合并
  - **示例**：将加法样例和乘法样例合并
  - **影响**：导致代码混乱，难以维护

- **问题**：数据流路径不同的样例被合并
  - **示例**：将GM→UB和GM→L1的样例合并
  - **影响**：导致代码复杂，难以理解

### 2. 场景编号问题

- **问题**：场景编号不连续
  - **示例**：有场景1、3，缺少场景2
  - **影响**：导致宏配置混乱，难以维护

- **问题**：场景编号从0开始
  - **示例**：SCENARIO_NUM=0
  - **影响**：不符合常规习惯，容易出错

### 3. 文件处理问题

- **问题**：未保留原始样例的注释
  - **示例**：合并后代码缺少注释
  - **影响**：代码可读性差

- **问题**：未保留共同函数
  - **示例**：重复定义相同的函数
  - **影响**：代码冗余，难以维护

### 4. 文档描述问题

- **问题**：缺少场景对照表
  - **示例**：README中没有场景说明
  - **影响**：用户不清楚各场景的用途

- **问题**：编译运行说明不完整
  - **示例**：只列出了命令，没有说明场景配置
  - **影响**：用户不知道如何切换场景

## 注意事项

1. **合并原则**
   - 确保被合并样例的功能相似
   - 仅在参数或配置上有差异
   - 避免合并功能完全不同的样例

2. **场景编号**
   - 场景编号从1开始，连续且无间断
   - 使用`SCENARIO_NUM`宏传递场景编号
   - 避免使用硬编码的场景编号

3. **文件处理**
   - 保留原始样例的注释
   - 保留所有被合并样例的共同函数
   - 根据scenarioNum选择不同的参数配置

4. **文档描述**
   - 添加场景对照表
   - 概述部分说明支持多种场景
   - 编译运行部分增加场景配置说明
