# aclrtcKernelSpecialization<a name="aclrtc-kernel-specialization"></a>

## 产品支持情况<a name="product-support"></a>

| 产品 | 是否支持 |
| :--- | :---: |
| Ascend 950PR/Ascend 950DT | √ |
| Atlas A3 推理系列产品 | √ |
| Atlas A2 推理系列产品 | √ |

## 功能说明<a name="function-description"></a>

头文件路径为：`"acl/acl_rt_compile.h"`。需要链接的库文件为`libacl_rtc.so`。

根据已注册的JIT编译资源及指定的常量参数，对指定的ACLNN核函数（Kernel）进行常量化编译，并将生成的Kernel ELF文件保存至指定路径。

本接口在Host侧同步完成编译和文件输出，生成的ELF文件供后续加载使用。

## 函数原型<a name="function-prototype"></a>

```cpp
aclError aclrtcKernelSpecialization(const aclrtcKernelSpecRequest *request, const char *outPath)
```

## 参数说明<a name="parameters"></a>

**表1**  接口参数说明

| 参数名 | 参数类型 | 输入/输出 | 描述 |
| --- | --- | --- | --- |
| request | const aclrtcKernelSpecRequest * | 输入 | Kernel特化编译请求，不能为空。使用前须将结构体清零，再设置需要的成员。 |
| outPath | const char * | 输入 | 特化Kernel ELF的输出文件路径，不能为空指针或空字符串。相对路径按调用本接口时的当前工作目录解析，父目录须已存在且可写。 |

### aclrtcKernelSpecRequest<a name="aclrtc-kernel-spec-request"></a>

`aclrtcKernelSpecRequest`定义位于`"acl/acl_rt_compile.h"`。

**表2**  aclrtcKernelSpecRequest成员说明

| 成员名 | 成员类型 | 描述 |
| --- | --- | --- |
| resourceId | const char * | JIT编译资源标识，取资源Manifest中的`resource_id`，须精确匹配。不能为空指针或空字符串。 |
| kernelEntry | const char * | 待常量化Kernel的完整入口名称，取资源Manifest中的`kernel_name`，须精确匹配。不能为空指针或空字符串。 |
| argsCount | uint64_t | 参数数组覆盖的Kernel参数个数。数组下标与Kernel参数索引一致，从0开始；每个待常量化参数的索引均须小于argsCount。 |
| argsAddr | const void *const * | 参数数据地址数组，按Kernel参数索引排列。不能为空，待常量化参数对应的元素须指向实际数据所在的Host缓冲区，不能传入Device地址。 |
| argsBytes | const uint64_t * | 参数数据长度数组，单位为字节，按Kernel参数索引排列。Manifest中`arg_type`为`struct`（默认值）时，数组不能为空，对应元素须大于0；为`pointer`时，使用Manifest中的`byte_size`，不读取对应元素。全部待常量化参数均为`pointer`类型时，本数组可为空。 |
| options | const char *const * | 基础编译对象的编译选项数组，不用于链接。optionCount为0时可为空；非0时数组及各字符串均不能为空指针，各字符串不能为空字符串。 |
| optionCount | uint64_t | `options`数组的元素个数。与`skOptionCount`之和不能超过4096。 |
| skOptions | const char *const * | SuperKernel编译选项数组，不用于链接。包含一次`--enable-super-kernel`时启用，其他选项用于SuperKernel对象编译；未包含时忽略其他选项，重复包含时报错。skOptionCount为0时可为空；非0时数组及各字符串均不能为空指针，各字符串不能为空字符串。 |
| skOptionCount | uint64_t | `skOptions`数组的元素个数。与`optionCount`之和不能超过4096。 |
| reserved | uint8_t[256] | 保留字段，当前版本必须全部置为0。 |

待常量化参数及其索引由Manifest的`constant_infos`和`parameter_index`指定。支持的普通编译选项请参考[《毕昇编译器用户指南》](https://www.hiascend.com/document/redirect/CannCommunityBiSheng)。

## 返回值说明<a name="return-values"></a>

返回值类型为aclError，ACL_SUCCESS表示常量化编译、链接及ELF文件输出成功，其他值表示失败。

| 返回值 | 描述 |
| --- | --- |
| ACL_SUCCESS | 特化编译、链接并输出Kernel ELF成功。 |
| ACL_ERROR_RTC_INVALID_INPUT | 请求参数或输出路径不满足要求。 |
| ACL_ERROR_RTC_INVALID_OPTION | 编译选项包含不支持的内容。 |
| ACL_ERROR_RTC_COMPILATION | Kernel编译失败。 |
| ACL_ERROR_RTC_LINKING | Kernel链接失败。 |
| ACL_ERROR_RTC_OUT_OF_MEMORY | 内存不足。 |
| ACL_ERROR_RTC_FAILURE | 编译资源处理、输出文件生成或其他RTC处理失败。 |

错误码的详细说明请参考[RTC错误码](RTC_error_code.md)。

## 约束说明<a name="constraints"></a>

- 调用前须确保`resourceId`对应的JIT编译资源已注册且可被运行时发现，`kernelEntry`在该资源中存在，参数数据类型及布局与该资源的常量参数声明一致。
- `request`及其引用的字符串、数组和参数数据须在本次同步调用期间保持有效且不可修改。非空参数数组须覆盖Manifest使用的参数索引。调用返回后可释放请求数组和常量参数缓冲区，接口不会替调用方释放这些内存。
- 待常量化参数的Host缓冲区可读长度须不小于接口读取的数据长度。
- `resourceId`、`kernelEntry`、`outPath`及各编译选项均须为空字符结尾的字符串，每个字符串内容长度不超过1 MiB，全部字符串内容的累计长度不超过4 MiB，长度均不计结尾空字符。
- 每个编译选项字符串作为一个完整参数传递，不按空格拆分。
- 调用期间，调用方不应读取或替换outPath指定的输出文件。
- 本接口只生成特化Kernel ELF，不负责将其加载到设备、替换模型任务或执行Kernel。

## 调用示例<a name="example"></a>

以下示例将一个uint32_t参数固定为8，生成常量化Kernel ELF。调用前须准备好可被运行时发现的JIT编译资源，示例适用于以下参数约定：

- Manifest仅声明一个待常量化参数，其`parameter_index`为0，`arg_type`为`struct`，参数数据类型为uint32_t。
- 从该资源Manifest获取`resource_id`和`kernel_name`，分别作为程序的两个命令行参数传入。
- 当前目录可写，输出文件为`./specialized_kernel.elf`。

其他资源须按其Manifest调整参数数组、数据类型和长度，不能仅替换资源标识及Kernel入口名称。

```cpp
#include "acl/acl.h"
#include "acl/acl_rt_compile.h"
#include <cstdint>
#include <cstdio>

int main(int argc, char *argv[])
{
    if (argc != 3) {
        std::fprintf(stderr, "Usage: kernel_specialization <resource_id> <kernel_name>\n");
        return 1;
    }
    uint32_t constValue = 8;
    const void *argsAddr[] = {&constValue};
    const uint64_t argsBytes[] = {sizeof(constValue)};

    aclrtcKernelSpecRequest request{};
    request.resourceId = argv[1];
    request.kernelEntry = argv[2];
    request.argsCount = 1;
    request.argsAddr = argsAddr;
    request.argsBytes = argsBytes;

    aclError ret = aclrtcKernelSpecialization(&request, "./specialized_kernel.elf");
    if (ret != ACL_SUCCESS) {
        std::fprintf(stderr, "aclrtcKernelSpecialization failed: %d\n", ret);
        return 1;
    }
    return 0;
}
```
