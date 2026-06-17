/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file op_cfg_generator.cpp
 * \brief
 */

#include "op_cpu_cfg_generator.h"

#include <fstream>
#include <cctype>
#include <limits.h>

#include "ascendc_tool_log.h"
#include "mmpa/mmpa_api.h"
#include "op_generator_factory.h"
#include "register/op_def_factory.h"
#include "register/op_def.h"

namespace ops {
enum class CPUType { AICPU = 0, HOSTCPU = 1 };

std::string CPUCfgGenerator::GetDataTypeName(const ge::DataType& type) const
{
    switch (type) {
        case ge::DT_FLOAT:
            return "DT_FLOAT";
        case ge::DT_FLOAT16:
            return "DT_FLOAT16";
        case ge::DT_INT8:
            return "DT_INT8";
        case ge::DT_INT16:
            return "DT_INT16";
        case ge::DT_INT32:
            return "DT_INT32";
        case ge::DT_INT64:
            return "DT_INT64";
        case ge::DT_UINT1:
            return "DT_UINT1";
        case ge::DT_UINT8:
            return "DT_UINT8";
        case ge::DT_UINT16:
            return "DT_UINT16";
        case ge::DT_UINT32:
            return "DT_UINT32";
        case ge::DT_UINT64:
            return "DT_UINT64";
        case ge::DT_BOOL:
            return "DT_BOOL";
        case ge::DT_DOUBLE:
            return "DT_DOUBLE";
        case ge::DT_DUAL:
            return "DT_DUAL";
        case ge::DT_DUAL_SUB_INT8:
            return "DT_DUAL_SUB_INT8";
        case ge::DT_DUAL_SUB_UINT8:
            return "DT_DUAL_SUB_UINT8";
        case ge::DT_STRING:
            return "DT_STRING";
        case ge::DT_COMPLEX64:
            return "DT_COMPLEX64";
        case ge::DT_COMPLEX128:
            return "DT_COMPLEX128";
        case ge::DT_QINT8:
            return "DT_QINT8";
        case ge::DT_QINT16:
            return "DT_QINT16";
        case ge::DT_QINT32:
            return "DT_QINT32";
        case ge::DT_QUINT8:
            return "DT_QUINT8";
        case ge::DT_QUINT16:
            return "DT_QUINT16";
        case ge::DT_RESOURCE:
            return "DT_RESOURCE";
        case ge::DT_STRING_REF:
            return "DT_STRING_REF";
        case ge::DT_INT4:
            return "DT_INT4";
        case ge::DT_BF16:
            return "DT_BF16";
        case ge::DT_COMPLEX32:
            return "DT_COMPLEX32";
        case ge::DT_HIFLOAT8:
            return "DT_HIFLOAT8";
        case ge::DT_FLOAT8_E4M3FN:
            return "DT_FLOAT8_E4M3FN";
        case ge::DT_FLOAT8_E5M2:
            return "DT_FLOAT8_E5M2";
        case ge::DT_FLOAT8_E8M0:
            return "DT_FLOAT8_E8M0";
        case ge::DT_FLOAT6_E3M2:
            return "DT_FLOAT6_E3M2";
        case ge::DT_FLOAT6_E2M3:
            return "DT_FLOAT6_E2M3";
        case ge::DT_FLOAT4_E2M1:
            return "DT_FLOAT4_E2M1";
        case ge::DT_FLOAT4_E1M2:
            return "DT_FLOAT4_E1M2";
        default:
            return "unknow";
    }
}

void CPUCfgGenerator::GetParamFormats(std::vector<ge::Format>& formats, std::string& fmtstr) const
{
    fmtstr = "";
    for (auto fmt : formats) {
        fmtstr += std::string(ge::GetFormatName(fmt)) + ",";
    }
    fmtstr.resize(fmtstr.size() - 1);
}

void CPUCfgGenerator::GetParamDataTypes(std::vector<ge::DataType>& types, std::string& tpstr) const
{
    tpstr = "";
    for (auto type : types) {
        tpstr += GetDataTypeName(type) + ",";
    }
    tpstr.resize(tpstr.size() - 1);
}

void CPUCfgGenerator::GenParamInfo(std::ofstream& outfile, std::vector<OpParamDef>& param, bool isOutput) const
{
    std::string type = (isOutput ? "output" : "input");
    std::string tpstr;
    std::string fmtstr;
    size_t ind = 0;
    for (auto def : param) {
        bool isDynamic = (def.GetParamType() == Option::DYNAMIC);
        outfile << (isDynamic ? "dynamic_" : "") << type << ind << ".name=" << def.GetParamName().GetString()
                << std::endl;
        if (def.GetDataTypes().size() > 0) {
            GetParamDataTypes(def.GetDataTypes(), tpstr);
            outfile << (isDynamic ? "dynamic_" : "") << type << ind << ".type=" << tpstr << std::endl;
        }

        if (def.GetFormats().size() > 0) {
            GetParamFormats(def.GetFormats(), fmtstr);
            outfile << type << ind << ".format=" << fmtstr << std::endl;
        }
        ind++;
    }
}

void CPUCfgGenerator::GenConfigInfo(
    std::ofstream& outfile, const std::map<ge::AscendString, ge::AscendString>& config) const
{
    for (auto& iter : config) {
        outfile << iter.first.GetString() << "=" << iter.second.GetString() << std::endl;
    }
}

opbuild::Status CPUCfgGenerator::GenerateCode(void)
{
    std::string genPath;
    ASCENDLOGI("CPU Cfg GenerateCode called!");
    Generator::GetGenPath(genPath);
    char resolvedGenPath[PATH_MAX] = {0};
    if (realpath(genPath.c_str(), resolvedGenPath) == nullptr) {
        ASCENDLOGE("Generate Path %s is invalid!", genPath.c_str());
        return opbuild::OPBUILD_FAILED;
    }
    std::vector<std::string> ops = this->GetAllOp();
    std::map<std::string, std::string> cfgFileStreams;
    std::string cpuCfgFileName = "";
    std::string cpuArg;
    Generator::GetCPUMode(cpuArg);
    CPUType cpuType = CPUType::AICPU;
    if (!cpuArg.empty()) {
        if (cpuArg == "--aicpu") {
            cpuType = CPUType::AICPU;
            cpuCfgFileName = "aicpu_kernel.ini";
        } else if (cpuArg == "--hostcpu") {
            cpuType = CPUType::HOSTCPU;
            cpuCfgFileName = "hostcpu_kernel.ini";
        } else {
            return opbuild::OPBUILD_FAILED;
        }
    } else {
        return opbuild::OPBUILD_FAILED;
    }
    cpuCfgFileName = genPath + "/" + cpuCfgFileName;

    std::ofstream opInfoFile = std::ofstream(cpuCfgFileName);
    std::map<ge::AscendString, ge::AscendString>* config;
    for (auto op : ops) {
        OpDef opsDef = OpDefFactory::OpDefCreate(op.c_str());
        // parse name
        opInfoFile << "[" << opsDef.GetOpType().GetString() << "]" << std::endl;

        // parse inputs
        std::vector<OpParamDef>& inputs = opsDef.GetInputs();
        GenParamInfo(opInfoFile, inputs, false);

        // parse outputs
        std::vector<OpParamDef>& outputs = opsDef.GetOutputs();
        GenParamInfo(opInfoFile, outputs, true);

        // parse config
        if (cpuType == CPUType::AICPU) {
            config = &(opsDef.AICPU().GetCfgInfo());
        } else {
            config = &(opsDef.HostCPU().GetCfgInfo());
        }
        GenConfigInfo(opInfoFile, *config);
    }
    opInfoFile.close();
    ASCENDLOGI("CPU Cfg GenerateCode end!");
    return opbuild::OPBUILD_SUCCESS;
}

CPUCfgGenerator::CPUCfgGenerator(std::vector<std::string>& ops) : Generator(ops)
{
    ASCENDLOGI("Stub CPUCfgGenerator construct!");
}

static opbuild::Status CPUCfgGeneratorBuilder(std::vector<std::string>& ops)
{
    CPUCfgGenerator g(ops);
    return g.GenerateCode();
}

static void AddCPUCfgGenerator(void) __attribute__((constructor));
void AddCPUCfgGenerator(void) { GeneratorFactory::AddBuilder("cpu_cfg", CPUCfgGeneratorBuilder); }

} // namespace ops
