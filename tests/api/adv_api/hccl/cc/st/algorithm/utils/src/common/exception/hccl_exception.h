/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef HCCL_EXCEPTION_H
#define HCCL_EXCEPTION_H

#include <exception>
#include <string>
#include <vector>
#include "exception_defination.h"

namespace HcclSim {

// public 继承修正：原私有继承（漏写 public）使 catch(const std::exception&) 无法捕获
// HcclSim 异常族（私有基类不可隐式上转型），fixture 的编排期/执行期异常保护均退化为
// catch(...) 丢失 what() 信息——ST 框架所有 THROW 异常必须可按 std::exception 多态捕获
class HcclException : public std::exception {
public:
    explicit HcclException(const ExceptionType& exceptionType, const std::string& userDefinedMsg);

    const char* what() const noexcept override;

    HcclResult GetErrorCode() const;

    std::vector<std::string> GetBackTraceStrings() const;

private:
    void StoreBackTrace();

    std::vector<std::string> backtraceStrings;
    ExceptionType exceptionType;
    std::string userDefinedMsg{""};
    std::string errorMsg{""};
};

} // namespace HcclSim
#endif
