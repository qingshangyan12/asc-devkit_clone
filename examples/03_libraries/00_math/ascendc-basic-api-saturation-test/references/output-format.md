# 输出打印格式模板

如何判断是单输入、双输入等场景，参考并遵守`SKILL.md`的 **【重要】如何判断API有几个输入** 内容。

## 单输入场景

- 输入数据类型为浮点型，比如half类型：  

    [API: Exp]--[InputDataType: half]--[OutputDataType: half]
    | inputType    | outputType    | input1     | golden(non-sat)  | golden(sat) | output(non-sat)  | output(sat) |
    | ---          | ---           | ---        | ---              | ---         | ---              | ---         |
    | half         | half          | min        | -                | -           | -                | -           |
    | half         | half          | max        | -                | -           | -                | -           |
    | half         | half          | +0         | -                | -           | -                | -           |
    | half         | half          | -0         | -                | -           | -                | -           |
    | half         | half          | nan        | -                | -           | -                | -           |
    | half         | half          | inf        | -                | -           | -                | -           |
    | half         | half          | -inf       | -                | -           | -                | -           |
    Sat verify result: Matches: xx / 7.
    Non-sat verify results: Matches: xx / 7.

- 输入数据类型为整型，比如int32_t类型：

    [API: Exp]--[InputDataType: int32_t]--[OutputDataType: int32_t]
    | inputType    | outputType    | input1     | golden(non-sat)  | golden(sat) | output(non-sat)  | output(sat) |
    | ---          | ---           | ---        | ---              | ---         | ---              | ---         |
    | int32_t      | int32_t       | min        | -                | -           | -                | -           |
    | int32_t      | int32_t       | max        | -                | -           | -                | -           |
    | int32_t      | int32_t       | +0         | -                | -           | -                | -           |
    | int32_t      | int32_t       | -0         | -                | -           | -                | -           |
    Sat verify result: Matches: xx / 4.
    Non-sat verify results: Matches: xx / 4.

- 注意事项
    - 每一行内容能跟表头对齐
    - 不同数据类型之间的表格，要至少隔开一行，方便呈现
    - 第一行[API: xxx]内容，采用黄色等醒目颜色标识
    - golden(non-sat)和output(non-sat)输出，采用橙色标识
    - golden(sat)和output(sat)输出，采用蓝色标识
    - verify result的结果不是100%时，采用红色标识，结果是100%时候，采用绿色标识

## 双输入场景（2个输入）

### 类型A：2个tensor输入（如Add等）

两个tensor输入，无scalar参数。

- 输入数据类型为浮点型，比如half类型：

    [API: Add]--[InputDataType: half]--[OutputDataType: half]
    | inputType    | outputType    | input1     | input2      | golden(non-sat)  | golden(sat) | output(non-sat)  | output(sat) |
    | ---          | ---           | ---        | ---         | ---              | ---         | ---              | ---         |
    | half         | half          | min        | min         | -                | -           | -                | -           |
    | half         | half          | min        | max         | -                | -           | -                | -           |
    | half         | half          | min        | +0          | -                | -           | -                | -           |
    | half         | half          | min        | -0          | -                | -           | -                | -           |
    | half         | half          | min        | nan         | -                | -           | -                | -           |
    | half         | half          | min        | +inf        | -                | -           | -                | -           |
    | half         | half          | min        | -inf        | -                | -           | -                | -           |
    | half         | half          | max        | min         | -                | -           | -                | -           |
    | half         | half          | max        | max         | -                | -           | -                | -           |
    ...(此处省略剩余组合场景)
    Sat verify result: Matches: xx / 49.
    Non-sat verify results: Matches: xx / 49.

### 类型B：1个tensor + 1个scalar输入（如Adds等）

参考`SKILL.md`的 **【重要】如何判断API有几个输入** 内容判断，这类API的两个输入：
- 源操作数（src）：tensor类型
- scalarValue：标量类型

这类API的表格列需要同时显示tensor输入值和scalar值。

- 输入数据类型为浮点型，比如half类型：

    [API: Adds]--[InputDataType: half]--[OutputDataType: half]
    | inputType    | outputType    | input1     | scalar      | golden(non-sat)  | golden(sat) | output(non-sat)  | output(sat) |
    | ---          | ---           | ---        | ---         | ---              | ---         | ---              | ---         |
    | half         | half          | min        | min         | -                | -           | -                | -           |
    | half         | half          | min        | max         | -                | -           | -                | -           |
    | half         | half          | min        | +0          | -                | -           | -                | -           |
    | half         | half          | min        | -0          | -                | -           | -                | -           |
    | half         | half          | min        | nan         | -                | -           | -                | -           |
    | half         | half          | min        | +inf        | -                | -           | -                | -           |
    | half         | half          | min        | -inf        | -                | -           | -                | -           |
    | half         | half          | max        | min         | -                | -           | -                | -           |
    | half         | half          | max        | max         | -                | -           | -                | -           |
    ...(此处省略剩余组合场景)
    Sat verify result: Matches: xx / 16.
    Non-sat verify results: Matches: xx / 16.

- 注意事项
    - 每一行内容能跟表头对齐
    - **必须包含scalar列**，scalar值为API的第二个输入参数
    - scalar跟tensor地位相同，也需要构造特殊值进行组合
    - 不同数据类型之间的表格，要至少隔开一行，方便呈现
    - 第一行[API: xxx]内容，采用黄色等醒目颜色标识
    - golden(non-sat)和output(non-sat)输出，采用橙色标识
    - golden(sat)和output(sat)输出，采用蓝色标识
    - verify result的结果不是100%时，采用红色标识，结果是100%时候，采用绿色标识
    - 多输入场景，需遍历所有输入组合

---

## 更多输入场景

参考上述场景，以此类推：
- 3个tensor输入：表格列 `| input1 | input2 | input3 |`
- 2 tensor + 1 scalar：表格列 `| input1 | input2 | scalar |`

---

## 总结：输入类型与表格列对照表

| API类型 | 输入 | 表格列 | 示例API | 遍历方式 |
|---------|------|--------|---------|---------|
| 双输入-类型A | 2 tensor | input1, input2 | Add等 | 7×7=49 |
| 双输入-类型B | 1 tensor + 1 scalar | input1, **scalar** | Adds等 | 7×7=49 |
| 单输入 | 1 tensor | input1 | Exp等 | 7 |
| 多输入 | >2 tensor | input1, input2, ... | - | - |
