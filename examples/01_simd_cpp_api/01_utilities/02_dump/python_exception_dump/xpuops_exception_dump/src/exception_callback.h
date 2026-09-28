/**
 * xpuops_exception_dump 内部头文件 - 通用 Exception Callback（单层设计）
 */
#ifndef XPUOPS_EXCEPTION_CALLBACK_H_
#define XPUOPS_EXCEPTION_CALLBACK_H_

#include <stddef.h>
#include <stdint.h>
#include <string>

namespace xpuops {

// 异常 dump 上下文（公共信息 + 落盘要素）
struct DumpContext {
    char timestamp[32];  // 20260922_112907_566
    uint32_t deviceId;
    uint32_t taskId;
    uint32_t streamId;
    uint32_t threadId;
    uint32_t errorCode;
    char kernelName[512];  // demangle 后的 kernel 名（模板参数已去掉）
    std::string dumpDir;   // 输出目录
    std::string filePrefix; // 产物文件名前缀
};

// 注册 Runtime exception callback（幂等：重复调用仅注册一次）
int RegisterExceptionCallback();

// callback 是否已注册
bool IsCallbackRegistered();

// 设置 kernel name 前缀启用列表（kernels==NULL 或 count==0 时对所有 kernel 生效）
void SetEnabledKernels(const char *kernels[], size_t count);

} // namespace xpuops

#endif /* XPUOPS_EXCEPTION_CALLBACK_H_ */
