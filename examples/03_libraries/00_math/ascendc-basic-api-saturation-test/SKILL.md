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
    - **【重要】优先级**：用户提供的参考样例 > `asc-devkit/examples`检索到的调用该API的样例
    - **必须优先使用用户提供的参考样例**（即使不是相同API，但样例写法差不多），不要自行检索其他样例
    - 将对应样例目录，拷贝到当前目录下的`tmp-workspace`中，并进行更名为{api_name}_saturation_verify
    - 如果用户既未提供API参考样例路径，在样例仓也未检索到对应API样例，进行报错提示用户，终止当前流程

- Step2. 在新路径下执行样例
    - 基于样例的README中编译执行章节，在新路径样例目录下新增`test.sh`
    - 进入到`test.sh`所在的目录，执行`bash test.sh`，确认基线样例功能正常。如果功能不正常，进行报错提示用户，终止当前流程

### 阶段二、整改重构样例

> **详细重构规范请参考** `references/basic-api-example_template/example-refactor-rules.md`

#### 按文件重构
按模板重构以下文件，详细说明见 `example-refactor-rules.md`：

| 文件 | 说明 |
|------|------|
| `{api_name}.asc` | 核心算子代码 |
| `gen_data.py` | 数据生成脚本 |
| `verify_result.py` | 结果验证脚本 |
| `test.sh` | 测试脚本 |

### 阶段三、验证整改后的样例
- 执行`test.sh`脚本，确认重构整改后的样例功能正常（无编译错误），打印结果符合预期（包含不同数据类型输入、饱和/非饱和模式输入场景下的结果打印）

## 常见问题速查

### 1. 若执行样例找不到已配置的CANN环境，可自行配置
```bash 
source /home/developer/Ascend/cann-9.0.0/set_env.sh
```
注：以上仅限gitcode算力平台生效，其他工作环境请自行配置
