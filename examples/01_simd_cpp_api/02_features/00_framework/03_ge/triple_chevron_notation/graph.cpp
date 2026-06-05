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
#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <iterator>
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
constexpr int64_t BLOCK_NUM = 8;
constexpr int64_t BLOCK_LENGTH = 2048;
constexpr size_t ELEMENT_COUNT = static_cast<size_t>(BLOCK_NUM * BLOCK_LENGTH);
constexpr float X_SCALE = 0.1F;
constexpr float Y_SCALE = 0.2F;

std::unique_ptr<ge::Graph> BuildGraph()
{
    ge::TensorDesc tensorDesc(ge::Shape({BLOCK_NUM, BLOCK_LENGTH}), ge::FORMAT_ND, ge::DT_FLOAT);
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

ge::Tensor BuildInputTensor(float scale)
{
    ge::TensorDesc tensorDesc(ge::Shape({BLOCK_NUM, BLOCK_LENGTH}), ge::FORMAT_ND, ge::DT_FLOAT);
    ge::Tensor tensor(tensorDesc);
    std::vector<float> data(ELEMENT_COUNT);
    for (size_t i = 0U; i < data.size(); ++i) {
        data[i] = static_cast<float>(i) * scale;
    }
    if (tensor.SetData(reinterpret_cast<const uint8_t *>(data.data()), data.size() * sizeof(float)) !=
        ge::GRAPH_SUCCESS) {
        std::cerr << "SetData failed." << std::endl;
    }
    return tensor;
}

std::vector<float> BuildGolden()
{
    std::vector<float> golden(ELEMENT_COUNT);
    for (size_t i = 0U; i < golden.size(); ++i) {
        golden[i] = static_cast<float>(i) * X_SCALE + static_cast<float>(i) * Y_SCALE;
    }
    return golden;
}

bool CheckOutputTensor(const ge::Tensor &outputTensor)
{
    const auto tensorDesc = outputTensor.GetTensorDesc();
    const auto dims = tensorDesc.GetShape().GetDims();
    if ((dims.size() != 2U) || (dims[0] != BLOCK_NUM) || (dims[1] != BLOCK_LENGTH)) {
        std::cerr << "Unexpected output shape." << std::endl;
        return false;
    }
    if (outputTensor.GetSize() < ELEMENT_COUNT * sizeof(float)) {
        std::cerr << "Unexpected output data size: " << outputTensor.GetSize() << std::endl;
        return false;
    }

    const auto *outputData = reinterpret_cast<const float *>(outputTensor.GetData());
    std::vector<float> output(outputData, outputData + ELEMENT_COUNT);
    std::vector<float> golden = BuildGolden();
    auto printTensor = [](const std::vector<float> &tensor, const char *name) {
        constexpr size_t maxPrintSize = 20U;
        std::cout << name << ": ";
        std::copy(tensor.begin(), tensor.begin() + std::min(tensor.size(), maxPrintSize),
            std::ostream_iterator<float>(std::cout, " "));
        if (tensor.size() > maxPrintSize) {
            std::cout << "...";
        }
        std::cout << std::endl;
    };
    printTensor(output, "Output");
    printTensor(golden, "Golden");

    for (size_t i = 0U; i < ELEMENT_COUNT; ++i) {
        if (output[i] != golden[i]) {
            std::cerr << "Unexpected output at index " << i << ", expected " << golden[i] << ", got "
                      << output[i] << std::endl;
            return false;
        }
    }
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
            std::vector<ge::Tensor> inputs{BuildInputTensor(X_SCALE), BuildInputTensor(Y_SCALE)};
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
