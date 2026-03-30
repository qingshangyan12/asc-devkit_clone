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
        - 多核样例改造成单核样例（比如若numsBlock>1则改成1，并对应适配修改地址偏移、tiling相关代码）。
        - 支持不同的数据类型输入场景。
        - 支持饱和/非饱和模式，涵盖数据构造和算子.asc代码适配。
    
    - `verify_result.py`的输出格式参考`references/output-format.md`
        - 单输入场景：参考"单个输入场景"章节
        - 双输入场景（如Add/Mul）：参考"两个输入场景"章节  
        - 双输出场景（如ReduceMax/ReduceMin/ReduceSum）：参考"双输出场景"章节，使用`output1`表示值，`output2`表示索引

    - `gen_data.py`中，如果是单输入场景调用`gen_data_utils`的`generate_one_input`函数来构造输入，如果是双输入场景调用`gen_data_utils`的`generate_two_inputs`函数来构造输入

    - `test.sh`拷贝`set_atomic_add_refactor`的test.sh内容，基于当前API支持的数据类型，适配修改`DTYPES`和`INDICES`。

    - 双输出API的处理（如ReduceMax/ReduceMin/ReduceSum等）
        - 双输出API输出格式参考`output-format.md`中的"双输出场景"章节，使用`output1`表示值输出，`output2`表示索引输出
        - 索引验证规则：golden索引与output索引一致即视为匹配（可忽略值是否正确）

    - 部分基线样例中`.asc`文件已有`CompareResult`等函数用于比对结果，可参考该函数的逻辑来生成`verify_result.py`。推荐使用`verify_result.py`来完成样例的`output`与`golden`的输出结果-真值比对，而不是`.asc`文件的函数。

    - 注意事项
        - `{api_name}.asc`文件的饱和/非饱和模式的代码适配方式，无需参考API文档，请严格遵守以下写法，不要随意自行修改。(参考`set_atomic_add_refactor`中`set_atomic_add.asc`的`SetSaturationMode`函数实现即可，区分浮点型和整型)
        ```cpp
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

### 阶段三、验证整改后的样例
- 执行`test.sh`脚本，确认重构整改后的样例功能正常（无编译错误），打印结果符合预期（包含不同数据类型输入、饱和/非饱和模式输入场景下的结果打印）

## 常见问题速查

### 1. 若执行样例找不到已配置的CANN环境，可自行配置
```bash 
source /home/developer/Ascend/cann-9.0.0/set_env.sh
```
注：以上仅限gitcode算力平台生效，其他工作环境请自行配置

### 2. 验证脚本要点
- 验证脚本必须使用Python，不能用C++内联比对
- 只需要比对前20个元素即可快速验证
