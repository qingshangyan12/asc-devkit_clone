/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#include "kernel_operator_list_tensor_intf.h"

//template<class T> class TensorDesc {
//    __aicore__ inline void SetShapeAddr(uint64_t* shapePtr);
//    __aicore__ inline uint64_t GetDim();
//    __aicore__ inline uint64_t GetIndex();
//    __aicore__ inline uint64_t GetShape(uint32_t offset);
//    __aicore__ inline __gm__ T* GetDataPtr();
//    __aicore__ inline GlobalTensor<T> GetDataObj();
//};
void HostTestTensorDesc() {
    AscendC::TensorDesc<float> desc;
    uint64_t shapePtr[4] = {1, 16, 32, 64};
    desc.SetShapeAddr(shapePtr);
    desc.GetDim();
    desc.GetIndex();
    desc.GetShape(0);
    desc.GetDataPtr();
    desc.GetDataObj();
}

//template<class T> __aicore__ inline void GetDesc(TensorDesc<T>& desc, uint32_t index);
void HostTestListTensorDescGetDesc() {
    AscendC::ListTensorDesc listTensorDesc;
    AscendC::TensorDesc<float> desc;
    uint32_t index = 0;
    listTensorDesc.GetDesc<float>(desc, index);
}