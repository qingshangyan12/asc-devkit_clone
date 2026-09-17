/**
 * exception_dump_public.h
 *
 * Exception Dump 外挂回调库的样例自有 C ABI 声明（3 个接口）。
 *
 * 头文件来源说明：
 *   - 本头文件随样例维护，声明的是样例自定义的 C ABI 接口；
 *   - 回调库实现（exception_dump_public.cpp）依赖的 acl/acl.h、acl/acl_rt.h
 *     来自 CANN 包（$ASCEND_HOME_PATH/include），仅在编译回调库自身时使用；
 *   - Python model 经 ctypes 加载 so，不需要任何头文件；本头供 C/C++ 宿主程序
 *     接入时使用。
 */

#ifndef EXCEPTION_DUMP_PUBLIC_H
#define EXCEPTION_DUMP_PUBLIC_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * 登记一个输入 tensor 的 dump 描述。
 *   name      输入名（落盘文件名 <name>.bin，同时写入 inputs_info.txt）
 *   ptrIndex  该输入 device 指针在 kernel args 区中的槽位（0-based，8 字节/槽，
 *             严格按目标 kernel launch 形参顺序校准）
 *   size      该输入的字节数
 *   shapeStr  shape 描述字符串（如 "(18, 64, 1, 128)"）
 *   dtypeStr  dtype 描述字符串（如 "torch.bfloat16"）
 * 返回 0 成功；超出容量（64 个）返回负值。
 */
int AddInputInfo(const char *name, size_t ptrIndex, size_t size,
                 const char *shapeStr, const char *dtypeStr);

/*
 * 清空全部输入登记。开始新一次 model 调用前调用，防止残留上一轮的输入描述。
 */
void ClearInputInfo(void);

/*
 * 引导注册：经 CANN 公开接口 aclrtExceptionInfoCallbackRegister 注册异常回调，
 * 并按 kernelName 过滤异常任务（mangled 名子串匹配， <<<>>> 模板 kernel 直接
 * 传函数名本体即可）。算子侧零改动。
 * 首次匹配到目标 kernel 异常时，回调内自动挂载 per-binary 回调；当前异常与
 * 后续异常均会保存现场。
 * 返回 0 成功；注册失败返回 CANN 错误码。
 */
int RegisterPublicExceptionDump(const char *kernelName);

#ifdef __cplusplus
}
#endif

#endif /* EXCEPTION_DUMP_PUBLIC_H */
