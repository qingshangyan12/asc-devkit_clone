# asc_set_store_atomic_config_v1

## 产品支持情况

<!-- npu="A3" id1 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：支持
<!-- end id1 -->
<!-- npu="910b" id2 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：支持
<!-- end id2 -->

## 功能说明

- 接口声明文件：`"c_api/atomic/atomic.h"`
- 核心功能：设置原子操作启用位与原子操作类型的值。
- 实现原理/公式：通过设置`type`参数指定原子操作的数据类型，通过`op`参数指定原子操作类型（如求和），从而配置后续搬运操作的原子行为。

## 函数原型

<!-- npu="A3" id3 -->
针对Atlas A3 训练系列产品/Atlas A3 推理系列产品：

```cpp
__aicore__ inline void asc_set_store_atomic_config_v1(uint16_t type, uint16_t op)
```

<!-- end id3 -->
<!-- npu="910b" id4 -->
针对Atlas A2 训练系列产品/Atlas A2 推理系列产品：

```cpp
__aicore__ inline void asc_set_store_atomic_config_v1(uint16_t type, uint16_t op)
```

<!-- end id4 -->

## 参数说明

**表 1**  参数说明

<!-- npu="A3" id5 -->

| 参数名 | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| type | 输入 | 原子操作启用位。<br>0：无原子操作。<br>1：开启原子操作，进行原子操作的数据类型为float。<br>2：开启原子操作，进行原子操作的数据类型为half。<br>3：开启原子操作，进行原子操作的数据类型为int16_t。<br>4：开启原子操作，进行原子操作的数据类型为int32_t。<br>5：开启原子操作，进行原子操作的数据类型为int8_t。<br>6：开启原子操作，进行原子操作的数据类型为bfloat16_t。<br>其余值无具体含义。 |
| op | 输入 | 原子操作类型。<br>0：求和操作。<br>其余值无具体含义。 |

<!-- end id5 -->
<!-- npu="910b" id6 -->

| 参数名 | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| type | 输入 | 原子操作启用位。<br>0：无原子操作。<br>1：开启原子操作，进行原子操作的数据类型为float。<br>2：开启原子操作，进行原子操作的数据类型为half。<br>3：开启原子操作，进行原子操作的数据类型为int16_t。<br>4：开启原子操作，进行原子操作的数据类型为int32_t。<br>5：开启原子操作，进行原子操作的数据类型为int8_t。<br>6：开启原子操作，进行原子操作的数据类型为bfloat16_t。<br>其余值无具体含义。 |
| op | 输入 | 原子操作类型。<br>0：求和操作。<br>其余值无具体含义。 |

<!-- end id6 -->

## 数据类型支持

<!-- npu="A3" id7 -->
针对Atlas A3 训练系列产品/Atlas A3 推理系列产品：

支持对int8_t、int16_t、int32_t、bfloat16_t、half、float类型的数据进行原子操作。

<!-- end id7 -->
<!-- npu="910b" id8 -->
针对Atlas A2 训练系列产品/Atlas A2 推理系列产品：

支持对int8_t、int16_t、int32_t、bfloat16_t、half、float类型的数据进行原子操作。

<!-- end id8 -->

## 返回值说明

无

## 流水类型

PIPE_S

## 资源占用情况

- 临时空间占用：不占用临时空间。
- 同步资源占用：不占用同步资源。
- 资源占用与参数/数据量关系：无。

## 约束说明

无

## 调用示例

<!-- npu="A3" id9 -->
针对Atlas A3 训练系列产品/Atlas A3 推理系列产品：

```cpp
// 设置原子操作为求和操作，数据类型为float
uint16_t type = 1;
uint16_t op = 0;
asc_set_store_atomic_config_v1(type, op);
```

- 完整样例链接：`<TODO>`

<!-- end id9 -->
<!-- npu="910b" id10 -->
针对Atlas A2 训练系列产品/Atlas A2 推理系列产品：

```cpp
// 设置原子操作为求和操作，数据类型为float
uint16_t type = 1;
uint16_t op = 0;
asc_set_store_atomic_config_v1(type, op);
```

- 完整样例链接：`<TODO>`

<!-- end id10 -->
