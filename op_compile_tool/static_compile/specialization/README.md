# 静态特化后端

此目录实现单 shape 的 aclrtc JIT 尝试与 OPC 回退，沿用原 shape 扩展、并发、索引、复制和打包流程。无单独的算子或芯片白名单。目标由当前 CANN 平台库解析，已验证芯片范围不等于硬编码支持范围。

## 正式输入

- 由 OPC 描述的 origin_path 回到采集节点，严格关联同一 PID、同名节点的 opcompile/debug JSON。
- 动态资源来自实际采集的 bin_info，配套 JSON 为同路径同名 .json。运行字段直接读取节点顶层，不读取独立 run_info.json、tiling_data.bin 或 shape_context.json。
- 不接受 specialization.dynamic_elf/dynamic_json/context/kernel_source 测试覆盖。需要打桩时由测试脚本准备一致的采集输入，生产代码不替换实际执行资源。
- 所有算子的输入输出、optional、动态 Tensor 组和属性结构统一来自资源 supportInfo descriptors，不含内置算子 schema。资源 null 只能匹配采集 null，不凭算子名猜参数。
- 资源类型由显式身份或同次采集的动态选择证明；字段名、位置、类型、shape、属性等必须一致。缺失或无法验证时失败回退。

## request 与元数据

resourceId 从真实 ELF 元数据读取。kernel_name/tiling_key 必须映射到资源入口。tiling_data 为采集 hex 原始内容，解码后在本进程创建 host buffer；argsCount=tiling_args_index+1，其他位置NULL/0，argsBytes为实际字节数。同步调用结束前保持缓冲区有效。

动态参数布局复用当前目标平台的编译器生成逻辑并与DFX交叉校验；参数组需folded_with_desc。选中key的DFX注册大小可小于JSON最大容量，但必须匹配实际tiling字节。当前普通注册规则仍为tiling大小+8，不支持的注册形式回退。

aicpu_block_dim/tiling_cond不参与request或静态JSON生成，可选保留、不作JIT准入校验。OPC输入转换会剔除全部运行字段。

静态JSON使用原OPC staticKey规则；parameters按逻辑inputs+outputs+workspace数量核对，不等同于request长度或包含FFTS/tiling的launch参数数。特殊初始化尚未支持，不能仅写JSON声明不存在的能力。

SK选项复用原parser及生成器，能力不足则回退。当前SK实现面向四变体包装，SK_BIND目标关联校验仍需完善，不能视为任意SK资源正式验收通过。

## 发布与回退

每任务独立worker、JIT/OPC staging隔离；文件/目录fsync后同文件系统renameat2(RENAME_NOREPLACE)，禁止覆盖。发布后错误终止打包，不再发布第二套结果。后端结果区分真正JIT与OPC回退，无缓存。

当前900秒超时发送进程组TERM/KILL；整组退出确认仍是交付缺口，不应把只wait组长表述为已完成该要求。

## 尚未完成的范围

- optional动静态入口不匹配：明确拒绝，双桩穿刺未合入。
- MIX 按所选 kernelList 项的 taskRation 解析物理入口：0:1 为 AIV，1:0 为 AIC，1:1/1:2 为 AIC+AIV。校验所有实体的 DFX（存在时）及输出符号；SK 为各架构分别生成映射并要求各自的主函数及三个 split 函数。request 仍使用逻辑 kernelEntry。
- 多实体 MIX 已完成解析/JSON 回归及真实动态资源布局检查；双核 JIT 编译、ACLNN 执行和 SK 端到端验收尚未完成，不能将代码支持等同于执行验收通过。
- 其他descriptor模式、OOM/debug、int64、特殊初始化及非普通tiling注册尚未通用支持。
- MC2/输出shape等额外参数需要完整识别与校验，不能仅凭默认空上下文认定普通ABI。
- 私有编译器接口兼容、SK_BIND目标关联、进程组退出确认和更完整诊断仍需交付前完善。

这些限制应产生明确失败/回退，不能用算子特判补数据。历史VALIDATION.md与DOCS1_REVIEW.md是历史记录，不是当前正式能力清单。

## 测试

在CANN环境下将op_compile_tool加入PYTHONPATH，TMPDIR与日志缓存置于工作目录，运行：

```bash
python -B -m unittest discover -s static_compile/specialization/tests -v
```

测试fixture中的具体算子名只作为测试样本，不参与生产分支。生产验收需动态/OPC/JIT优先三组数值、实际后端及产物校验，回退结果必须单独标记。
