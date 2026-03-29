---
name: ascendc-basic-api-saturation-test
description: Ascend C 基础API饱和/非饱和模式验证技能。
---

# Ascend C 基础API饱和/非饱和模式验证

## 使用时机

**适用**：某个API饱和/非饱和模式验证，必须包含：具体API名称（如Add/Mul等等）、饱和/非饱和模式验证关键提示词。

**不适用**：未提供具体的正确的API名称；未饱和以上关键提示词。

---

## 工作流程

### 阶段一、样例准备

- Step1. 拷贝基线样例
    - 基于用户提供的指定API参考样例路径（如果用户未提供，去样例仓 `asc-devkit/examples` 检索，找到最匹配的），将对应样例目录，拷贝到当前目录下的`tmp-workspace`中。
    - 如果用户既未提供API参考样例路径，在样例仓也未检索到对应API样例，进行报错提示用户，终止当前流程。

- Step2. 在新路径下执行样例
    - 按样例的README中的编译执行章节，新增`test.sh`并执行，确认基线样例功能正常。如果功能不正常，进行报错提示用户，终止当前流程。

### 阶段二、整改样例

- Step1. 分析API资料

    查看 `asc-devkit/docs/api/context/` API 文档，找到对应的API接口文档，重点分析以下内容：
    - 产品支持情况，要求是Atlas A2 训练系列产品/Atlas A2 推理系列产品支持的API，否则报错提示用户，终止当前流程。  
    - 参数说明表格，查看输入/输出数据类型支持情况（比如half/float/bfloat16_t/int8_t/int16_t/int32_t等），为下一步的样例改造做准备。
    
    注意：1、只关注Atlas A2 训练系列产品/Atlas A2 推理系列产品的数据类型支持情况；2、要关注输入数据类型和输出数据类型的合法组合。

- Step2. 参考样例模板进行整改
    
    - 样例模板在`references/basic-api-example-template`
        - 结合`set_atomic_add_baseline`代码和`set_atomic_add_refactor`代码，以及`example_refactor_rules.md`，明确需要重构整改的要点。
    - 关键整改内容如下
        - 样例目录结构参照模板样式。
        - 支持不同的数据类型输入场景。
        - 支持饱和/非饱和模式，涵盖数据构造和算子.asc代码适配。

### 阶段三、验证整改后的样例
- 执行`test.sh`脚本，确认重构整改后的样例功能正常（无编译错误），打印结果符合预期（包含不同数据类型输入、饱和/非饱和模式输入场景下的结果打印）
