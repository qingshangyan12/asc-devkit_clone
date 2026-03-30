# 输出打印格式模板

## 单个输入场景
比如Exp/Ln等单目API，只需要一个input。

- 输入数据类型为浮点型，比如half类型，则verify_result.py的打印结果如下（要求每一行内容能跟表头对齐）：  

    [API: Exp]--[InputDataType: half]--[OutputDataType: half]
    | inputType    | outputType    | input1     | golden(non-sat)  | golden(sat) | output(non-sat)  | output(sat) |
    | ---          | ---           | ---        | ---              | ---         | ---              | ---         |
    | half         | half          | min        | -                | -           | -                | ---         |
    | half         | half          | max        | -                | -           | -                | ---         |
    | half         | half          | +0         | -                | -           | -                | ---         |
    | half         | half          | -0         | -                | -           | -                | ---         |
    | half         | half          | nan        | -                | -           | -                | ---         |
    | half         | half          | inf        | -                | -           | -                | ---         |
    | half         | half          | -inf       | -                | -           | -                | ---         |
    Sat verify result: Matches: xx / 7.
    Non-sat verify results: Matches: xx / 7.

- 输入数据类型为整型，比如int32_t类型，则verify_result.py的打印结果如下（要求每一行内容能跟表头对齐）：

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

## 两个输入场景
比如Add/Mul/Adds等双目API，需要两个input。

- 输入数据类型为浮点型，比如half类型，则verify_result.py的打印结果如下（要求每一行内容能跟表头对齐）：  

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
    ...
    Sat verify result: Matches: xx / 49.
    Non-sat verify results: Matches: xx / 49.

- 输入数据类型为整型，比如int32_t类型，则verify_result.py的打印结果如下（要求每一行内容能跟表头对齐）：

    [API: Add]--[InputDataType: int32_t]--[OutputDataType: int32_t]
    | inputType    | outputType    | input1     | input2      | golden(non-sat)  | golden(sat) | output(non-sat)  | output(sat) |
    | ---          | ---           | ---        | ---         | ---              | ---         | ---              | ---         |
    | int32_t      | int32_t       | min        | min         | -                | -           | -                | -           |
    | int32_t      | int32_t       | min        | max         | -                | -           | -                | -           |
    | int32_t      | int32_t       | min        | +0          | -                | -           | -                | -           |
    | int32_t      | int32_t       | min        | -0          | -                | -           | -                | -           |
    | int32_t      | int32_t       | max        | min         | -                | -           | -                | -           |
    | int32_t      | int32_t       | max        | max         | -                | -           | -                | -           |
    ...
    Sat verify result: Matches: xx / 16.
    Non-sat verify results: Matches: xx / 16.

- 注意事项
    - 每一行内容能跟表头对齐
    - 不同数据类型之间的表格，要至少隔开一行，方便呈现
    - 第一行[API: xxx]内容，采用黄色等醒目颜色标识
    - golden(non-sat)和output(non-sat)输出，采用橙色标识
    - golden(sat)和output(sat)输出，采用蓝色标识
    - verify result的结果不是100%时，采用红色标识，结果是100%时候，采用绿色标识
    - 多输入场景，需遍历所有输入组合

## 双输出场景
比如ReduceMax/ReduceMin/ReduceSum等归约API，除了输出最大值/最小值/和值外，还会输出对应索引。

- 输入数据类型为浮点型，比如half类型，则verify_result.py的打印结果如下（要求每一行内容能跟表头对齐）：

    [API: ReduceMax]--[InputDataType: half]--[OutputDataType: half]
    | inputType    | outputType    | input1     | golden(non-sat)  | output1 | output2 | golden(sat) | output1 | output2 | output(non-sat) | output1 | output2 | output(sat) | output1 | output2 |
    | ---          | ---           | ---        | ---              | ---     | ---     | ---         | ---     | ---     | ---              | ---     | ---     | ---          | ---     | ---     |
    | half         | half          | min        | -                | -       | -       | -           | -       | -       | -                | -       | -       | -            | -       | -       |
    | half         | half          | max        | -                | -       | -       | -           | -       | -       | -                | -       | -       | -            | -       | -       |
    | half         | half          | +0         | -                | -       | -       | -           | -       | -       | -                | -       | -       | -            | -       | -       |
    | half         | half          | -0         | -                | -       | -       | -           | -       | -       | -                | -       | -       | -            | -       | -       |
    | half         | half          | nan        | -                | -       | -       | -           | -       | -       | -                | -       | -       | -            | -       | -       |
    | half         | half          | inf        | -                | -       | -       | -           | -       | -       | -                | -       | -       | -            | -       | -       |
    | half         | half          | -inf       | -                | -       | -       | -           | -       | -       | -                | -       | -       | -            | -       | -       |
    Sat verify result: Matches: xx / 7.
    Non-sat verify results: Matches: xx / 7.

- 注意事项
    - 每一行内容能跟表头对齐
    - 输出值用output1表示，索引用output2表示
    - 不同数据类型之间的表格，要至少隔开一行，方便呈现
    - 第一行[API: xxx]内容，采用黄色等醒目颜色标识
    - golden(non-sat)和output(non-sat)输出，采用橙色标识
    - golden(sat)和output(sat)输出，采用蓝色标识
    - verify result的结果不是100%时，采用红色标识，结果是100%时候，采用绿色标识
    - 索引验证：如果golden索引与output索引一致，则视为匹配（忽略值是否正确）

## 更多输入场景
参考单个输入、双个输入、双输出场景，以此类推。