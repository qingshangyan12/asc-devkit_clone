# IBSet与IBWait的MIX配比分析

## 分析结论

在分离模式的`__mix__(1, 1)`核函数中，配对调用`IBSet<false>`和`IBWait<false>`**不会因为MIX配比为1:1而必然出现同步功能错误**。当前实现中，两者使用完全相同的地址公式，并且都将相邻`eventID`之间的事件步长固定为`GetBlockNum() × 2`。因此，只要两次调用传入相同的`gmWorkspace`、`blockIdx`和`eventID`，并按当前实现的固定事件步长申请足够的`gmWorkspace`，两者会访问同一个同步标志，可正常完成AIV核之间的同步。

固定的`GetBlockNum() × 2`并不是`__mix__(1, 1)`场景中的实际AIV核数。该场景的实际AIV核数为`GetBlockNum()`，所以当前寻址会在相邻事件区域之间留下未使用的空间，但不会因此产生事件地址冲突。

存在两类功能风险：第一，如果开发者根据`__mix__(1, 1)`的实际AIV核数，将事件步长按`GetBlockNum()`计算并据此申请工作区，当`eventID > 0`时，接口仍会按`GetBlockNum() × 2`寻址，可能访问已申请空间之外的地址；第二，AIC侧调用这两个接口会直接返回，不能用它们同步AIC核或保护依赖AIC执行结果的数据访问。当前不能修改接口代码，因此必须在[IBSet](docs/zh/api/SIMD-API/basic_api/sync_control/inter_core_sync/IBSet.md)和[IBWait](docs/zh/api/SIMD-API/basic_api/sync_control/inter_core_sync/IBWait.md)资料中明确这些实现限制，并要求按固定事件步长计算空间。

## MIX配比与实际核数

[MIX修饰符说明](docs/zh/guide/programming_guide/language_extension/simd_builtin_keywords.md#L225)明确，`__mix__(cube, vec)`支持`(1, 0)`、`(0, 1)`、`(1, 1)`和`(1, 2)`，因此不能将MIX模式等同于1:2配比。

对于同时启动AIC和AIV的场景，[GetTaskRatio返回值说明](docs/zh/api/SIMD-API/basic_api/tool_interface/system_resources_and_variables/GetTaskRatio.md#L57)给出的AIV侧比例如下：

| MIX配比 | AIV侧`GetTaskRatio()` | 实际AIV核数 |
| --- | ---: | ---: |
| 1:1 | 1 | `GetBlockNum()` |
| 1:2 | 2 | `GetBlockNum() × 2` |

[GetBlockIdx返回值说明](docs/zh/api/SIMD-API/basic_api/tool_interface/system_resources_and_variables/GetBlockIdx.md#L54)进一步确认，1:1场景中AIV侧索引范围为`[0, GetBlockNum())`，1:2场景中为`[0, 2 × GetBlockNum())`。代码中的[GetBlockIdxImpl实现](impl/basic_api/dav_c220/kernel_operator_sys_var_impl.h#L56)使用`get_block_idx() × GetTaskRationImpl() + get_subblockid()`生成AIV逻辑索引，而[GetTaskRationImpl实现](impl/basic_api/dav_c220/kernel_operator_sys_var_impl.h#L43)从`get_subblockdim()`取得实际配比。

因此，`blockIdx`的合法核索引范围应由实际AIV数量决定，不能由`IBSet`和`IBWait`内部固定的`2 × GetBlockNum()`事件步长反推。

## 实现路径分析

### 1. AIC不会执行同步逻辑

在分离模式对应的实现分支中，[IBSet](impl/basic_api/kernel_operator_block_sync_intf_impl.h#L147)和[IBWait](impl/basic_api/kernel_operator_block_sync_intf_impl.h#L189)检测到当前为AIC时直接返回。因此，MIX核函数中的实际同步操作只发生在AIV侧。如果业务期望AIC等待其他核，或期望其他核等待AIC通过IBSet发出的通知，调用这两个接口不能建立该同步关系，会产生功能问题；这一点与MIX配比是1:1还是1:2无关。

### 2. `isAIVOnly=false`固定使用2倍事件步长

`IBSet`先将`blockNum`初始化为`GetBlockNum()`，然后在相关分离架构上，当`isAIVOnly`为false时将其改为`GetBlockNum() × 2`，见[IBSet的blockNum计算](impl/basic_api/kernel_operator_block_sync_intf_impl.h#L146)。[IBWait的blockNum计算](impl/basic_api/kernel_operator_block_sync_intf_impl.h#L188)执行相同处理。

这两个分支没有调用`GetTaskRatio()`，所以固定的2倍步长不随`__mix__(1, 1)`或`__mix__(1, 2)`变化。对比同类同步接口，[SyncAll实现](impl/basic_api/dav_c220/kernel_operator_sync_impl.h#L62)在`isAIVOnly=false`时使用`GetTaskRationImpl() × GetBlockNum()`计算实际参与同步的AIV核数，进一步说明“实际AIV核数”和IBSet/IBWait的“固定事件步长”是两个不同概念。

### 3. IBSet和IBWait访问相同地址

[IBSet寻址代码](impl/basic_api/kernel_operator_block_sync_intf_impl.h#L160)和[IBWait寻址代码](impl/basic_api/kernel_operator_block_sync_intf_impl.h#L202)均为：

```cpp
auto localSyncGM = gmWorkspace[blockNum * 8 * eventID + blockIdx * 8];
```

`gmWorkspace`的元素类型为`int32_t`，8个元素占32字节。因此，在`isAIVOnly=false`时，两者访问的字节偏移均为：

$$
(2 \times \mathrm{GetBlockNum}() \times \mathrm{eventID} + \mathrm{blockIdx}) \times 32\text{字节}
$$

只要配对调用的参数一致，IBSet写入1的地址就是IBWait轮询并清零的地址。MIX配比为1:1不会改变这一对应关系。

### 4. 1:1配比会产生空洞，但不会产生地址重叠

设`GetBlockNum() = N`。在`__mix__(1, 1)`场景中，有效的AIV逻辑索引为`0`至`N - 1`，而事件步长固定为`2N`。事件`e`使用的标志位区间为：

$$
[2Ne,\ 2Ne + N - 1]
$$

下一事件`e + 1`使用的区间起点为`2N(e + 1)`，大于当前区间终点。因此，不同事件不会重叠；每个事件区域之后会留下N个未使用的标志位。

## 工作区不足的风险示例

假设`GetBlockNum() = 4`，核函数使用`__mix__(1, 1)`，共享工作区的最大参数为`eventID_max = 1`、`blockIdx_max = 3`。

按当前IBSet/IBWait实现，事件步长为8，最小空间为：

$$
(8 \times 1 + 3 + 1) \times 32\text{字节} = 384\text{字节}
$$

如果误将实际AIV核数4当作事件步长，只会申请：

$$
(4 \times 1 + 3 + 1) \times 32\text{字节} = 256\text{字节}
$$

但`eventID = 1`、`blockIdx = 3`对应的同步标志起始偏移已达到352字节，接口还会从该位置读写32字节，因此需要覆盖到384字节。只申请256字节会造成越界访问风险。

当所有调用的`eventID`均为0时，地址公式中的事件步长项为0，1:1和1:2的步长差异不会影响该事件所需的最小空间。例如`blockIdx_max = 3`时仍只需`(3 + 1) × 32 = 128`字节。这个特例不能用于推导`eventID > 0`时的工作区大小。

## 使用约束与处理方案

- 分离模式的MIX核函数中，应显式调用`IBSet<false>`和`IBWait<false>`，并保证配对调用的`gmWorkspace`、`blockIdx`和`eventID`一致。
- 只能使用这两个接口同步AIV核；不能依赖MIX核中AIC侧的调用建立同步关系，因为AIC侧会直接返回。
- `blockIdx`应按实际参与同步的AIV核索引确定。`__mix__(1, 1)`下的范围为`[0, GetBlockNum() - 1]`，`__mix__(1, 2)`下为`[0, 2 × GetBlockNum() - 1]`。
- 无论MIX配比为1:1还是1:2，当前实现中`isAIVOnly=false`时的`eventStride`均须取`GetBlockNum() × 2`。
- `gmWorkspace`须按下式申请，并初始化为0：

$$
\text{gmWorkspace空间大小} \geq (\mathrm{eventStride} \times \mathrm{eventID\_max} + \mathrm{blockIdx\_max} + 1) \times 32\text{字节}
$$

- 当前接口实现暂不能修改，因此本次只修订资料，明确固定事件步长及其与实际AIV核数的区别。未来若将实现改为`GetTaskRatio() × GetBlockNum()`，还需评估既有工作区布局的兼容性，并补充1:1、1:2、多`eventID`和边界空间测试，不能仅替换一处乘数。

## 分析边界

当前仓库未发现`IBSet<false>`或`IBWait<false>`在`__mix__(1, 1)`下的功能测试。上述结论来自当前版本接口实现、系统变量实现和公开资料的静态推导，能够确认寻址关系和空间边界。对于在MIX核函数中省略模板实参、按默认`isAIVOnly=true`调用的方式，虽然1:1场景下地址计算恰好使用`GetBlockNum()`，但这与接口定义的MIX寻址模式不符，且仓库没有对应测试，不能据此认定为受支持用法。实际硬件回归仍应覆盖1:1配比下的配对唤醒、多事件复用及工作区边界场景。
