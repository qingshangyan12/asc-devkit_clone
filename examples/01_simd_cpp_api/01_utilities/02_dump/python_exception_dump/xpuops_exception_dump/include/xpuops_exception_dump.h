/**
 * xpuops_exception_dump - NPU Runtime 异常信息自动 Dump 外挂库
 * 公开 C 接口
 */
#ifndef XPUOPS_EXCEPTION_DUMP_H_
#define XPUOPS_EXCEPTION_DUMP_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化 Exception Dump 系统
 * @param kernels   kernel name 前缀列表，为 NULL 时对所有 kernel 生效
 * @param count     kernel 数量，为 0 时表示对所有 kernel 生效
 *
 * 调用后：
 *   1. 注册 Runtime exception callback（幂等，仅注册一次；接口按 CANN 版本运行期自适应）
 *   2. 设置 kernel name 前缀启用列表
 *      - count == 0: 对所有 kernel 生效
 *      - count > 0:  仅对 kernels 中指定前缀的 kernel dump
 * 接入无需修改任何 C++ 代码，注册动作在 Python 侧（enable_exception_dump）或本接口完成。
 */
int xpuopsExceptionDumpInit(const char *kernels[], size_t count);

#ifdef __cplusplus
}
#endif

#endif /* XPUOPS_EXCEPTION_DUMP_H_ */
