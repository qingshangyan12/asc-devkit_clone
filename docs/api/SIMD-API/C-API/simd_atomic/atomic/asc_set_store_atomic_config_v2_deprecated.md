# asc_set_store_atomic_config_v2（废弃）

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->

## 功能说明

**该接口已废弃。原子加操作请使用[asc_atomic_add](../scalar_compute/asc_atomic_add.md)实现。**

- 接口声明文件：`"c_api/atomic/atomic.h"`
- 核心功能：设置原子操作启用位与原子操作类型的值。
- 实现原理/公式：通过设置`type`参数指定原子操作的数据类型，通过`op`参数指定原子操作类型（如求和），从而配置后续搬运操作的原子行为。

## 函数原型

<!-- npu="950" id2 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
__aicore__ inline void asc_set_store_atomic_config_v2(uint16_t type, uint16_t op)
```

<!-- end id2 -->

## 参数说明

**表 1**  参数说明

<!-- npu="950" id3 -->

| 参数名 | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| type | 输入 | 原子操作启用位。<br>1：开启原子操作，进行原子操作的数据类型为int32_t。<br>4：开启原子操作，进行原子操作的数据类型为half。<br>5：开启原子操作，进行原子操作的数据类型为float。<br>7：开启原子操作，进行原子操作的数据类型为bfloat16_t。<br>9：开启原子操作，进行原子操作的数据类型为int16_t。<br>10：开启原子操作，进行原子操作的数据类型为int8_t。<br>其余值无具体含义。 |
| op | 输入 | 原子操作类型。<br>2：求和操作。<br>其余值无具体含义。 |

<!-- end id3 -->

## 数据类型支持

<!-- npu="950" id4 -->
针对Ascend 950PR/Ascend 950DT：

支持对int8_t、int16_t、int32_t、bfloat16_t、half、float类型的数据进行原子操作。

<!-- end id4 -->

## 返回值说明

无

## 流水类型

PIPE_S

## 资源占用情况

- 临时空间占用：不占用临时空间。
- 同步资源占用：不占用同步资源。
- 资源占用与参数/数据量关系：无。

## 约束说明

- 该接口已废弃，建议使用[asc_atomic_add](../scalar_compute/asc_atomic_add.md)替代。

## 调用示例

<!-- npu="950" id5 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
// 设置原子操作为求和操作，数据类型为int32_t
uint16_t type = 1;
uint16_t op = 2;
asc_set_store_atomic_config_v2(type, op);
```

- 完整样例链接：`<TODO>`

<!-- end id5 -->
