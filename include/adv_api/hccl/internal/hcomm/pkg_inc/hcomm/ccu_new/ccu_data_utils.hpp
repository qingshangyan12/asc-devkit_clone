/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef CCU_DATA_UTILS_HPP
#define CCU_DATA_UTILS_HPP

#include <exception>
#include <string>

#include "ccu_types.h"

namespace ccu {

struct NoAllocTag {};

class CcuException : public std::exception {
public:
    CcuException(CcuResult code, const char *what)
        : code_(code), what_(BuildMessage(code, what)) {}

    const char *what() const noexcept override { return what_.c_str(); }
    CcuResult   code() const noexcept { return code_; }

private:
    static std::string BuildMessage(CcuResult code, const char *what)
    {
        std::string msg = "[ccu] ";
        msg += (what != nullptr) ? what : "(unknown)";
        msg += " (CcuResult=";
        msg += std::to_string(static_cast<int>(code));
        msg += ")";
        return msg;
    }

    CcuResult   code_;
    std::string what_;
};

} // namespace ccu

template <typename lhsT, typename rhsT> class CcuOperator {
public:
    CcuOperator(lhsT lhs, rhsT rhs) : lhs(lhs), rhs(rhs) {}

    lhsT lhs;
    rhsT rhs;
};

enum class CcuArithmeticOperatorType { ADDITION, INVALID };

template <typename lhsT, typename rhsT> class CcuArithmeticOperator : public CcuOperator<lhsT, rhsT> {
public:
    CcuArithmeticOperator(lhsT lhs, rhsT rhs, CcuArithmeticOperatorType type)
        : CcuOperator<lhsT, rhsT>(lhs, rhs), type(type)
    {
        Check();
    }

    void Check() const
    {
        throw ::ccu::CcuException(CcuResult::CCU_E_PARA, "CcuArithmeticOperator: invalid operand types");
    }

    CcuArithmeticOperatorType type{CcuArithmeticOperatorType::INVALID};
};

#endif