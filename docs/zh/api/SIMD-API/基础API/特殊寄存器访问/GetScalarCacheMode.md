# GetScalarCacheMode ![已检查](https://img.shields.io/badge/已检查-blue)

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

获取标量缓存的模式，用于查询L2 Cache当前的读写缓存配置。通过模板参数rwMode指定读或写模式，读取CTRL寄存器对应比特位的值并返回CacheMode枚举。当rwMode为READ时，读取CTRL[19:16]；当rwMode为WRITE时，读取CTRL[23:20]。

## 函数原型

```cpp
template <CacheRwMode rwMode>
__aicore__ inline CacheMode GetScalarCacheMode()
```

## 参数说明

**表1** 模板参数说明

| 参数名 | 描述 |
|--------|------|
| rwMode | 读写模式，类型为CacheRwMode枚举类。取值为READ（读模式，读取CTRL[19:16]）或WRITE（写模式，读取CTRL[23:20]），不支持RW，否则编译报错。 |

## 数据类型

本接口为寄存器访问接口，不涉及张量数据类型操作。返回值类型为CacheMode。

## 返回值说明

| 返回值类型 | 含义 |
|-----------|------|
| CacheMode | 返回当前标量缓存的模式。具体取值请参考[表2](#table2)。 |

**表2** CacheMode枚举值说明<a name="table2"></a>

| 取值 | CTRL寄存器比特位值 | 含义 |
|------|---------------------|------|
| CACHE_MODE_NORMAL | 4'b0000 | 普通缓存模式。 |
| CACHE_MODE_LAST | 4'b0001 | Last缓存模式。 |
| CACHE_MODE_PERSISTENT | 4'b0010 | 持久化缓存模式。 |
| CACHE_MODE_DISABLE | 4'b0100 | 禁用缓存模式。 |

## 约束说明

- rwMode仅支持READ或WRITE，不支持RW，否则编译报错。
- READ模式读取CTRL[19:16]，WRITE模式读取CTRL[23:20]，两者相互独立。

## 调用示例

如下示例中分别获取读缓存模式和写缓存模式的当前配置。

```cpp
AscendC::CacheMode readMode = AscendC::GetScalarCacheMode<AscendC::CacheRwMode::READ>();
AscendC::CacheMode writeMode = AscendC::GetScalarCacheMode<AscendC::CacheRwMode::WRITE>();
```
