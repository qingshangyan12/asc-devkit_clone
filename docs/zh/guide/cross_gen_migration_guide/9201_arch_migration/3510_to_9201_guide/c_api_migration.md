# C API迁移指导

本节针对3510到9201的SIMD C API兼容性变更提供索引和适配方法。C API属于体系架构相关接口，不能仅根据函数名称相同判断其原型、支持类型及行为兼容。应使用`c_api/asc_simd.h`等公开头文件，并核对目标SDK版本要求。

**表1** 数据搬运、系统变量和同步接口

| C API | 受影响场景 | 兼容方案 |
|-------|------------|----------|
| asc_copy_l0c2ub | 依赖dualdst向两个AIV分发输出 | 改为单目的搬运，重新分配输出和Vector后处理，见[L0C到UB搬运](./basic_api_migration.md#l0c-to-ub-migration)。 |
| asc_copy_l12ub | 依赖两个AIV分别搬运L1中的数据 | 按一个AIV重算数据量、步长、UB预算及同步，见[UB与L1搬运](./basic_api_migration.md#ub-l1-migration)。 |
| asc_copy_ub2l1 | 依赖两个AIV分别向L1写入数据 | 按一个AIV重算L1区间、缓冲区复用及消费者依赖。 |
| asc_get_sub_block_num | 将返回值或两个AIV数量写死为2 | 按实际执行配置切分任务；9201的1:1场景只有一个AIV。 |
| asc_get_sub_block_id | 根据索引1安排第二份工作 | 合并原两个AIV的职责；检查索引换算、工作区和尾块。 |
| asc_sync_subblock_arrive、asc_sync_subblock_wait | mode=1中依赖原两个AIV的同步关系 | 重建实际参与者的通知/等待关系，核对目标模式及ID范围。 |
| asc_sync_block_arrive、asc_sync_block_wait | mode=2中依赖原两个AIV的同步关系 | 按1:1重新检查通知次数、等待次数及空任务路径。 |
| asc_sync_intra_arrive、asc_sync_intra_wait | mode=4中的AIC与AIV0/AIV1映射 | 移除对第二个AIV的等待和ID映射，保留AIC与唯一AIV的数据依赖。 |

同步的完整适配方法见[核间同步迁移](./basic_api_migration.md#cross-core-sync-migration)。不能仅修改`__mix__`声明，或者将同步ID中的1改成0，而保留原来的参与者数量和执行次数。

## 搬运与同步参数检查

**说明**：历史重载和新重载的参数顺序、参数数量以及控制字段可能不同。尤其是asc_copy_l0c2ub，不能用固定位置的参数值跨版本判断是否开启dualdst。

**兼容方案**：

1. 按实际调用实参和公开头文件确定选中的重载，定位对应的目的选择或dualdst控制字段。
2. 按9201支持的参数配置单目的搬运，重算目的布局、数据量、量化和格式转换参数。
3. 同步检查相邻的通知/等待和UB到L1搬运，避免只修改单条搬运而破坏完整流水。

**表2** Regbase归约、重排及指数接口

| C API | 兼容性影响 | 兼容方案 |
|-------|------------|----------|
| asc_reduce_sum、asc_reduce_max、asc_reduce_min | 原SUM/MAX/MIN接口软仿可能额外使用一个寄存器 | 核对目标SDK公开接口及结果布局，检查寄存器和溢出空间。 |
| asc_reduce_sum_datablock、asc_reduce_max_datablock、asc_reduce_min_datablock | DataBlock归约原接口软仿可能额外使用一个寄存器 | 保留分块粒度、mask和结果排列，检查寄存器峰值。 |
| asc_pair_reduce_sum | SUM软仿可能额外使用一个寄存器 | 检查配对归约语义及临时寄存器活跃区间。 |
| asc_intlv_b8、asc_intlv_b16、asc_intlv_b32 | 掩码交织原接口软仿可能额外使用一个寄存器 | 核对掩码粒度、两份输出及后续使用位置。 |
| asc_deintlv_b8、asc_deintlv_b16、asc_deintlv_b32 | 掩码去交织原接口软仿可能额外使用一个寄存器 | 核对掩码粒度、两份输出及后续使用位置。 |
| asc_intlv、asc_deintlv | 向量数据交织/去交织原接口软仿可能额外使用一个寄存器 | 核对数据类型、输出布局以及寄存器资源。 |
| asc_exp | 部分fp16输入与3510结果相差1 ULP | 精度敏感场景采用fp32计算并回归验证。 |

归约和重排的适配方法见[归约接口](./reg_api_migration.md#reduce-migration)、[交织与去交织](./reg_api_migration.md#interleave-migration)及[寄存器压力](./reg_api_migration.md#register-pressure-migration)。指数精度见[Exp精度](./reg_api_migration.md#exp-precision-migration)。

原接口的软仿兼容方案需要配套SDK提供相应公开接口。若目标版本不再声明原名称或重载，应核对该SDK的迁移映射并采用公开替代接口；不能将3510头文件或内部实现复制到9201工程，也不能根据硬件v2指令名推导公开API名称。

## 移位接口<a id="c-api-shift-migration"></a>

**表3** C API移位适配

| C API | 需要核对的内容 | 兼容方案 |
|-------|----------------|----------|
| asc_shiftleft、asc_shiftright | 向量移位量是否存在负值；类型是否由有符号改为无符号；数据是否为b64 | 确认每个有效通道移位量非负，按公开原型构造移位向量；b64另查资源开销。 |
| asc_shiftleft_scalar、asc_shiftright_scalar | 标量是否为常量；值域是否非负；b8是否需要范围处理；实参实际匹配的标量类型 | 合法常量可折叠范围判断；运行时值按范围约束处理，禁止未经检查直接窄化。 |

### 按公开原型适配向量移位量

**说明**：部分配套SDK的9201 C API公开重载将移位向量改为vector_uint8_t、vector_uint16_t、vector_uint32_t。即使被移位数据为vector_int8_t，移位量也可能要求vector_uint8_t。3510代码中使用vector_int8_t保存正移位量，也需要适配类型。

**迁移示例**：以下为VF内的b8左移片段，`src`和`dst`为vector_int8_t，`mask`已按b8配置。代码按当前公开原型区分移位量类型，目标SDK还需提供相应重载。

```cpp
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 9201)
vector_uint8_t shift;
asc_duplicate_scalar(shift, static_cast<uint8_t>(2));
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
vector_int8_t shift;
asc_duplicate_scalar(shift, static_cast<int8_t>(2));
#endif
asc_shiftleft(dst, src, shift, mask);
```

对运行时生成的有符号移位向量，应先证明或检查其各有效通道非负，再采用公开接口支持的类型转换或数据构造方式。只改变类型不能解决负移位语义问题。

### 常量标量移位

**迁移示例**：以下代码中的`src`、`dst`均为vector_uint8_t，`mask`已初始化。

```cpp
asc_shiftleft_scalar(dst, src, 2, mask);
asc_shiftright_scalar(dst, src, 4, mask);
```

上例确实使用b8数据，但移位量为编译期常量2和4。如果SDK通过比较255实现范围兼容，内联和常量传播后比较可被折叠。不能据此认定必然增加运行时Scalar判断；也不能保证所有优化配置都生成相同代码，应检查目标编译器优化后的指令。

### 运行时标量移位

**说明**：材料中的b8兼容方案是对非负s16移位量做范围判断，大于255时饱和到255，否则转为u8。该逻辑是否已由接口实现，应按配套SDK确认。

当前部分9201公开原型中，b8/b16的asc_shiftleft_scalar、asc_shiftright_scalar接收uint16_t，b32接收uint32_t。此时将调用实参改为uint8_t仍会转换为形参类型，并不等于选中一个专用u8接口。不要将Regbase ShiftLefts/ShiftRights新增ScalarT类型的说明直接套用到全部C API重载。

如果SDK尚未提供所需的范围兼容，且业务允许的非负移位量可能超过255，可先在算子中按[范围转换示例](./reg_api_migration.md#b8-scalar-shift-migration)处理，再按该公开原型传参。应验证移位量0、7、8、255、256及s16最大值，特别检查大移位量不会窄化为0而改变结果。测试算术右移时还应包含负的被移位数据。
