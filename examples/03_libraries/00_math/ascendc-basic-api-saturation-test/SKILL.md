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

### 阶段二、整改样例

#### Step1. 分析API资料
查看 `asc-devkit/docs/api/context/` API文档，重点分析：
- 产品支持：必须是Atlas A2 训练/推理系列产品支持的API；
- 数据类型：输入/输出的合法数据类型组合（如half/float/int16_t/int32_t等）；
- **【重要】判断输入数量**：参考本SKILL的 **【重要】如何判断API有几个输入** 内容，分析API的输入数量。

#### Step2. 按文件重构（必须严格参照模板）

> **【重要】以下内容是重构要点总结，具体明细可参考`references/basic-api-example-template/example-refactor-rules.md`和`references/output-format.md`。**  
> **【重要】所有Python脚本必须直接拷贝模板代码并修改，必须严格参照`references/basic-api-example-template/example-refactor-rules.md`执行。**

##### 1. `{api_name}.asc` 文件
- **多核→单核**：`numBlocks>1`改为`1`，适配地址偏移和tiling
- **模板化数据类型**：使用`template<typename T>`支持多种类型
- **饱和/非饱和模式**：在API调用前插入`SetSaturationMode<T>()`函数（参考`references/basic-api-example-template/example-refactor-rules.md`）
- **命令行参数**：
  - 单输入：`./demo <dtype> <special_index> <saturation>`
  - 双输入-类型A：`./demo <dtype> <input1_index> <input2_index> <saturation>`
  - 双输入-类型B：`./demo <dtype> <input1_index> <input2_index> <saturation>`


##### 2. `gen_data.py` 文件（必须拷贝模板）
- **必须严格参照模板** `set_atomic_add_refactor/scripts/gen_data.py`
- **根据输入数量决定数据生成方式**：
  - 单输入：`generate_one_input(data_type)`，每个special_index对应一个特殊值
  - 双输入-类型A（2 tensor）：`generate_two_inputs(data_type)` 生成所有组合
  - 双输入-类型B（1 tensor + 1 scalar）：`generate_two_inputs(data_type)` 生成所有组合

##### 3. `verify_result.py` 文件（必须拷贝模板）
- **必须严格参照模板** `set_atomic_add_refactor/scripts/verify_result.py`
- **输出格式**：必须严格符合 `references/output-format.md`
  - **单输入**：表格列 `| input1 |`
  - **双输入-类型A**：表格列 `| input1 | input2 |`
  - **双输入-类型B**：表格列 `| input1 | scalar |`
- **重要**：多输入场景需遍历所有组合（比如双输入场景，浮点型7²=49行，整型4²=16行）
- **结果验证方式**：详细说明见 `example-refactor-rules.md` 第 1.6 节

##### 4. `test.sh` 文件
- **必须严格参照模板** `set_atomic_add_refactor/test.sh`
拷贝 `set_atomic_add_refactor/test.sh`，适配DTYPES和INDICES：
```bash
DTYPES=("int16_t" "int32_t" "half" "float")  # 根据API支持的数据类型
INDICES=(0 1 2 3)
```
- 根据输入数量修改循环嵌套

### 阶段三、验证整改后的样例
- 执行`test.sh`脚本，确认重构整改后的样例功能正常（无编译错误），打印结果符合预期（包含不同数据类型输入、饱和/非饱和模式输入场景下的结果打印）

## 常见问题速查

### 1. 若执行样例找不到已配置的CANN环境，可自行配置
```bash 
source /home/developer/Ascend/cann-9.0.0/set_env.sh
```
注：以上仅限gitcode算力平台生效，其他工作环境请自行配置

### 2. **【重要】如何判断API有几个输入**

**第一步：查看API函数原型**

查看 `asc-devkit/docs/api/context/{API_NAME}.md` 中的"函数原型"章节：
- 如果只有一个 `src` 参数 → 单输入
- 如果有 `src0`, `src1` 两个参数 → 双输入-类型A（2 tensor）
- 如果有 `src` + `scalarValue` 参数 → 双输入-类型B（1 tensor + 1 scalar）

**第二步：结合"参数说明"表格验证**

在"参数说明"表格中，检查"参数名称"列：
- 统计所有"源操作数"类型的参数
- 确认哪些是 `tensor`，哪些是 `scalar`

**第三步：结合"功能说明"确认**

查看功能说明中的数学公式，例如：
- Adds: `z = x + scalarValue` → 有 x 和 scalarValue 两个输入

**【常见API判断速查】**

| API输入数量类型 | 示例API | 特征 |
|---------|--------|------|
| 单输入（1 tensor） | Exp,Ln,Abs,Reciprocal,Sqrt,Rsqrt,Relu | 函数原型只有1个src参数 |
| 双输入-类型A（2 tensor） | Add,Sub,Mul,Div,Max,Min | 函数原型有src0, src1两个tensor参数 |
| 双输入-类型B（1 tensor + 1 scalar） | Adds,Muls,Maxs,Mins,LeakyRelu | 函数原型有src和scalarValue参数 |
