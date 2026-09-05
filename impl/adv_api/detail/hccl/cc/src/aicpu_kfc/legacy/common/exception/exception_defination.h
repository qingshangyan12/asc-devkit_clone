/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef HCCLV2_EXCEPTION_DEFINITION_H
#define HCCLV2_EXCEPTION_DEFINITION_H

#include <map>
#include <string>
#include "../types/hccl_result.h"
#include "../utils/enum_factory.h"

namespace Hccl {
MAKE_ENUM(
    ExceptionType, NULL_PTR_EXCEPTION, INVALID_PARAMS_EXCEPTION, TIMEOUT_EXCEPTION, DRV_API_EXCEPTION,
    NETWORK_API_EXCEPTION, RUNTIME_API_EXCEPTION, RESOURCES_NOT_EXIST_EXCEPTION, NOT_SUPPORT_EXCEPTION,
    INTERNAL_EXCEPTION, RMA_CONN_EXCEPTION, SOCKET_EXCEPTION, RES_UNPACKAGE_EXCEPTION, HOST_IP_NOT_FOUND_EXCEPTION,
    TRACE_API_EXCEPTION, CCU_API_EXCEPTION, SUSPENDING_EXCEPTION)

class ExceptionInfo {
public:
    __attribute__((visibility("hidden")))
    static std::string GetErrorMsg(const ExceptionType& type)
    {
        // Keep exception construction independent of HCOMM's private message symbols.
        static const std::map<ExceptionType, std::string> errorMessages{
            {ExceptionType::NULL_PTR_EXCEPTION, "Null pointer exception: "},
            {ExceptionType::INVALID_PARAMS_EXCEPTION, "Invalid params exception: "},
            {ExceptionType::TIMEOUT_EXCEPTION, "Time out exception: "},
            {ExceptionType::DRV_API_EXCEPTION, "Drv api exception: "},
            {ExceptionType::NETWORK_API_EXCEPTION, "Network api exception: "},
            {ExceptionType::RUNTIME_API_EXCEPTION, "Runtime api exception: "},
            {ExceptionType::RESOURCES_NOT_EXIST_EXCEPTION, "Resources not exist exception: "},
            {ExceptionType::NOT_SUPPORT_EXCEPTION, "Not support exception: "},
            {ExceptionType::INTERNAL_EXCEPTION, "Internal exception: "},
            {ExceptionType::RMA_CONN_EXCEPTION, "RMA connection exception: "},
            {ExceptionType::SOCKET_EXCEPTION, "Socket exception: "},
            {ExceptionType::RES_UNPACKAGE_EXCEPTION, "Resource unpackage exception:"},
            {ExceptionType::HOST_IP_NOT_FOUND_EXCEPTION, "environment variable exception: "},
            {ExceptionType::TRACE_API_EXCEPTION, "Trace api exception: "},
            {ExceptionType::CCU_API_EXCEPTION, "Ccu api exception: "},
            {ExceptionType::SUSPENDING_EXCEPTION, "Suspending exception: "}};
        return errorMessages.at(type);
    }

    static HcclResult GetErrorCode(const ExceptionType& type);

private:
    static std::map<ExceptionType, std::string> errorMsgMap;
    static std::map<ExceptionType, HcclResult> errorCodeMap;
};

} // namespace Hccl

#endif
