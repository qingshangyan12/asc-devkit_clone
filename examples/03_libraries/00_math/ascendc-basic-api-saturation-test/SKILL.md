---
name: ascendc-basic-api-saturation-test
description: Ascend C 基础API饱和/非饱和模式验证技能。需要用户提供想验证的基础API名称（必选）、asc-devkit仓中该API的参考样例链接或者样例文件夹名称（可选）。
---

# Ascend C 基础API饱和/非饱和模式验证

## 使用时机

**适用**：某个API饱和/非饱和模式验证，必须包含：具体API名称（如Add/Mul等等）、饱和/非饱和模式验证关键提示词。

**不适用**：未提供具体的正确的API名称；未提供以上关键提示词。

---

## 工作流程

### 阶段一、样例准备

- Step1. 拷贝基线样例
    - 基于用户提供的指定API参考样例路径（如果用户未提供，去样例仓 `asc-devkit/examples` 检索，找到最匹配的），将对应样例目录，拷贝到当前目录下的`tmp-workspace`中。
    - 如果用户既未提供API参考样例路径，在样例仓也未检索到对应API样例，进行报错提示用户，终止当前流程。

- Step2. 在新路径下执行样例
    - 基于样例的README中编译执行章节，在新路径样例目录下新增`test.sh`。
    - 进入到`test.sh`所在的目录，执行`bash test.sh`，确认基线样例功能正常。如果功能不正常，进行报错提示用户，终止当前流程。

### 阶段二、整改样例

#### Step1. 分析API资料
查看 `asc-devkit/docs/api/context/` API文档，重点分析：
- 产品支持：必须是Atlas A2 训练/推理系列产品支持的API
- 数据类型：输入/输出的合法数据类型组合（如half/float/int16_t/int32_t等）

#### Step2. 按文件重构

##### 1. `{api_name}.asc` 文件
- **多核→单核**：`numBlocks>1`改为`1`，适配地址偏移和tiling
- **模板化数据类型**：使用`template<typename T>`支持多种类型
- **饱和/非饱和模式**：在API调用前插入`SetSaturationMode<T>()`函数
  ```cpp
  // 浮点型(float/half/bfloat16)：CTRL_48_BIT
  // 整型(int8/16/32)：CTRL_53_BIT
  if constexpr (std::is_same_v<DataType, float> || std::is_same_v<DataType, half> || std::is_same_v<DataType, bfloat16_t>) {
    if (isSat) {
        // 浮点型，饱和模式
        set_ctrl(sbitset0(get_ctrl(), AscendC::CTRL_48_BIT));
    } else {
        // 浮点型，INF/NAN模式
        set_ctrl(sbitset1(get_ctrl(), AscendC::CTRL_48_BIT));
    }
  } else {
    if (isSat) {
        // 整型，饱和模式
        set_ctrl(sbitset1(get_ctrl(), AscendC::CTRL_53_BIT));
    } else {
        // 整型，INF/NAN模式
        set_ctrl(sbitset0(get_ctrl(), AscendC::CTRL_53_BIT));
    }
  }
  ```
可结合参考`references/basic-api-example-template/example_refactore_rules.md`。

##### 2. `gen_data.py` 文件
- 特殊值构造：使用`get_dtype_limits()`获取min/max/0/NaN/INF等
- 数据生成：
  - 单输入：`generate_one_input(data_type)`
  - 双输入：`generate_two_inputs(data_type)` 生成所有组合
  - 多输入：依次类推，新增函数实现功能
- Golden计算：分别计算sat和non-sat模式结果
可结合参考`references/basic-api-example-template/example_refactore_rules.md`。

##### 3. `verify_result.py` 文件
- 输出格式：参考 `references/output-format.md`
  - 单输入场景
  - 双输入场景（如Add/Mul）
  - 双输出场景（如ReduceMax，使用output1=值，output2=索引）
- **重要**：多输入场景需遍历所有组合（比如双输入场景，浮点型7²=49行，整型4²=16行）
- 比对逻辑：推荐使用Python验证，而非.asc中的CompareResult函数
可结合参考`references/basic-api-example-template/example_refactore_rules.md`和`references/output-format.md`。

##### 4. `test.sh` 文件
拷贝 `set_atomic_add_refactor/test.sh`，适配DTYPES和INDICES：
```bash
DTYPES=("int16_t" "int32_t" "half" "float")  # 根据API支持的数据类型
INDICES=(0 1 2 3)
```
可结合参考`references/basic-api-example-template/example_refactore_rules.md`。

### 阶段三、验证整改后的样例
- 执行`test.sh`脚本，确认重构整改后的样例功能正常（无编译错误），打印结果符合预期（包含不同数据类型输入、饱和/非饱和模式输入场景下的结果打印）

## 常见问题速查

### 1. 若执行样例找不到已配置的CANN环境，可自行配置
```bash 
source /home/developer/Ascend/cann-9.0.0/set_env.sh
```
注：以上仅限gitcode算力平台生效，其他工作环境请自行配置
