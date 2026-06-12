#include "hccl/hccl_common.h"
#include <iostream>

int main() {
    std::cout << "HCCL_CMD_ALLTOALLV = " << static_cast<int>(AscendC::HCCL_CMD_ALLTOALLV) << std::endl;
    std::cout << "HCCL_CMD_ALLTOALL = " << static_cast<int>(AscendC::HCCL_CMD_ALLTOALL) << std::endl;
    std::cout << "HCCL_CMD_REDUCE_SCATTER = " << static_cast<int>(AscendC::HCCL_CMD_REDUCE_SCATTER) << std::endl;
    std::cout << "HCCL_CMD_ALLGATHER = " << static_cast<int>(AscendC::HCCL_CMD_ALLGATHER) << std::endl;
    return 0;
}
