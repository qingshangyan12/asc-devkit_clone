# SetScalarCacheMode ![已检查](https://img.shields.io/badge/已检查-blue)

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->
<!-- npu="x90" id8 -->
- Kirin X90：不支持
<!-- end id8 -->
<!-- npu="9030" id9 -->
- Kirin 9030：不支持
<!-- end id9 -->

## 功能说明

头文件路径为：`"basic_api/kernel_operator_common_intf.h"`。

设置标量缓存的模式，用于控制L2 Cache的读写缓存行为。通过模板参数rwMode指定读或写模式，通过模板参数cacheMode指定缓存模式，将对应模式写入CTRL寄存器的特定比特位。当rwMode为READ时，配置CTRL[19:16]；当rwMode为WRITE时，配置CTRL[23:20]。读模式与写模式相互独立，修改其中一个不会影响另一个。

## 函数原型

```cpp
template <CacheRwMode rwMode, CacheMode cacheMode>
__aicore__ inline void SetScalarCacheMode()
```

## 参数说明

**表1** 模板参数说明

| 参数名 | 描述 |
|--------|------|
| rwMode | 读写模式，类型为CacheRwMode枚举类。取值为READ（读模式，配置CTRL[19:16]）或WRITE（写模式，配置CTRL[23:20]），不支持RW，否则编译报错。 |
| cacheMode | 缓存模式，类型为CacheMode枚举类。具体取值请参考[表2](#table2)。 |

**表2** CacheMode枚举值说明<a name="table2"></a>

| 取值 | CTRL寄存器比特位值 | 含义 |
|------|---------------------|------|
| CACHE_MODE_NORMAL | 4'b0000 | 普通缓存模式。 |
| CACHE_MODE_LAST | 4'b0001 | Last缓存模式。 |
| CACHE_MODE_PERSISTENT | 4'b0010 | 持久化缓存模式。 |
| CACHE_MODE_DISABLE | 4'b0100 | 禁用缓存模式。 |

## 数据类型

本接口为寄存器访问接口，不涉及张量数据类型操作。

## 返回值说明

无返回值。

## 约束说明

- rwMode仅支持READ或WRITE，不支持RW，否则编译报错。
- READ模式配置CTRL[19:16]，WRITE模式配置CTRL[23:20]，两者相互独立。

## 调用示例

如下示例中分别设置读缓存模式为LAST、写缓存模式为PERSISTENT。

```cpp
AscendC::SetScalarCacheMode<AscendC::CacheRwMode::READ, AscendC::CacheMode::CACHE_MODE_LAST>();
AscendC::SetScalarCacheMode<AscendC::CacheRwMode::WRITE, AscendC::CacheMode::CACHE_MODE_PERSISTENT>();
```
