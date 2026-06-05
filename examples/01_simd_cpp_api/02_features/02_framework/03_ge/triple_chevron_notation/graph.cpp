/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <vector>

#include "acl/acl.h"
#include "add_custom_proto.h"
#include "ge/ge_api.h"
#include "graph.h"
#include "ops_proto_legacy.h"
#include "tensor.h"
#include "types.h"

using ge::Operator;

namespace {
constexpr uint32_t GRAPH_ID = 0U;
constexpr size_t ELEMENT_COUNT = 16U;
constexpr uint16_t FP16_ONE = 0x3C00U;
constexpr uint16_t FP16_TWO = 0x4000U;
constexpr uint16_t FP16_THREE = 0x4200U;

std::unique_ptr<ge::Graph> BuildGraph()
{
    ge::TensorDesc tensorDesc(ge::Shape({static_cast<int64_t>(ELEMENT_COUNT)}), ge::FORMAT_ND, ge::DT_FLOAT16);
    auto x = ge::op::Data("x");
    x.update_input_desc_x(tensorDesc);
    x.update_output_desc_y(tensorDesc);

    auto y = ge::op::Data("y");
    y.update_input_desc_x(tensorDesc);
    y.update_output_desc_y(tensorDesc);

    auto add = ge::op::AddCustom("add_custom").set_input_x(x).set_input_y(y);
    add.update_output_desc_z(tensorDesc);

    std::vector<Operator> inputs{x, y};
    std::vector<Operator> outputs{add};
    auto graph = std::make_unique<ge::Graph>("TripleChevronAddGraph");
    graph->SetInputs(inputs).SetOutputs(outputs);
    return graph;
}

ge::Tensor BuildInputTensor(uint16_t value)
{
    ge::TensorDesc tensorDesc(ge::Shape({static_cast<int64_t>(ELEMENT_COUNT)}), ge::FORMAT_ND, ge::DT_FLOAT16);
    ge::Tensor tensor(tensorDesc);
    std::vector<uint16_t> data(ELEMENT_COUNT, value);
    if (tensor.SetData(reinterpret_cast<const uint8_t *>(data.data()), data.size() * sizeof(uint16_t)) !=
        ge::GRAPH_SUCCESS) {
        std::cerr << "SetData failed." << std::endl;
    }
    return tensor;
}

bool CheckOutputTensor(const ge::Tensor &outputTensor)
{
    const auto tensorDesc = outputTensor.GetTensorDesc();
    const auto dims = tensorDesc.GetShape().GetDims();
    if ((dims.size() != 1U) || (dims[0] != static_cast<int64_t>(ELEMENT_COUNT))) {
        std::cerr << "Unexpected output shape." << std::endl;
        return false;
    }
    if (outputTensor.GetSize() < ELEMENT_COUNT * sizeof(uint16_t)) {
        std::cerr << "Unexpected output data size: " << outputTensor.GetSize() << std::endl;
        return false;
    }

    const auto *outputData = reinterpret_cast<const uint16_t *>(outputTensor.GetData());
    std::cout << "output values:";
    for (size_t i = 0U; i < ELEMENT_COUNT; ++i) {
        std::cout << " 0x" << std::hex << outputData[i] << std::dec;
        if (outputData[i] != FP16_THREE) {
            std::cerr << "\nUnexpected output at index " << i << ", expected fp16 3.0 (0x" << std::hex
                      << FP16_THREE << "), got 0x" << outputData[i] << std::dec << std::endl;
            return false;
        }
    }
    std::cout << std::endl;
    return true;
}
}  // namespace

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    std::map<ge::AscendString, ge::AscendString> options = {
        {"ge.exec.deviceId", "1"},
        {"ge.graphRunMode", "0"},
    };

    const auto initRet = ge::GEInitialize(options);
    if (initRet != ge::SUCCESS) {
        std::cerr << "GEInitialize failed, ret: " << initRet << std::endl;
        return 1;
    }

    int retCode = 0;
    {
        ge::Session session(options);
        auto graph = BuildGraph();
        if (graph == nullptr) {
            std::cerr << "BuildGraph failed." << std::endl;
            (void)ge::GEFinalize();
            return 1;
        }

        const auto addGraphRet = session.AddGraph(GRAPH_ID, *graph);
        if (addGraphRet != ge::SUCCESS) {
            std::cerr << "AddGraph failed, ret: " << addGraphRet << std::endl;
            retCode = 1;
        } else {
            std::vector<ge::Tensor> inputs{BuildInputTensor(FP16_ONE), BuildInputTensor(FP16_TWO)};
            std::vector<ge::Tensor> outputs;
            const auto runRet = session.RunGraph(GRAPH_ID, inputs, outputs);
            if (runRet != ge::SUCCESS) {
                std::cerr << "RunGraph failed, ret: " << runRet << std::endl;
                retCode = 1;
            } else if (outputs.empty()) {
                std::cerr << "RunGraph succeeded but outputs is empty." << std::endl;
                retCode = 1;
            } else if (!CheckOutputTensor(outputs[0])) {
                retCode = 1;
            } else {
                std::cout << "AddCustom GE triple-chevron sample success." << std::endl;
            }
            (void)session.RemoveGraph(GRAPH_ID);
        }
    }

    const auto finalizeRet = ge::GEFinalize();
    if (finalizeRet != ge::SUCCESS) {
        std::cerr << "GEFinalize failed, ret: " << finalizeRet << std::endl;
        return 1;
    }
    return retCode;
}
