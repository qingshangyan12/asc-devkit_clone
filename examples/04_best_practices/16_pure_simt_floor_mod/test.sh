CANN_PATH=$(printenv ASCEND_HOME_PATH)  # 获取CANN包安装路径
OUTPUT="floor_mod_simt"               #用户自定义编译生成的二进制文件名称

bisheng \
  -x dpp --cce-aicore-arch=dav-c310-vec \
  -std=c++17 \
   floor_mod.asc \
  -I${CANN_PATH}/include \
  -I${CANN_PATH}/include/ascendc/host_api \
  -I${CANN_PATH}/compiler/ascendc/include/highlevel_api \
  -I${CANN_PATH}/compiler/tikcpp/tikcfw \
  -I${CANN_PATH}/compiler/tikcpp/tikcfw/lib \
  -I${CANN_PATH}/compiler/tikcpp/tikcfw/lib/matmul \
  -I${CANN_PATH}/compiler/tikcpp/tikcfw/impl \
  -I${CANN_PATH}/compiler/tikcpp/tikcfw/interface \
  -L${CANN_PATH}/lib64 \
  -lascendc_runtime \
  -lascendcl \
  -lruntime \
  -lregister \
  -lerror_manager \
  -lprofapi \
  -lunified_dlog \
  -lmmpa \
  -lascend_dump \
  -ltiling_api \
  -lplatform \
  -ldl \
  -lc_sec \
  -lstdc++ \
  -o ${OUTPUT}

  ./${OUTPUT}                        # 执行样例