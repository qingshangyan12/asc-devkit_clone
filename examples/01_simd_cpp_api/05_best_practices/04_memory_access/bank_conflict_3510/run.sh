export SCENARIO_NUM=1          # 1-14可选
mkdir -p build && cd build
cmake .. -DSCENARIO_NUM=${SCENARIO_NUM} -DCMAKE_ASC_RUN_MODE=sim
make -j
# python3 ../scripts/gen_data.py --case ${SCENARIO_NUM}
# ./demo                     # 精度验证 + 打点性能（输出cycles计数到stdout）
# python3 ../scripts/verify_result.py output/output_z.bin output/golden.bin
msprof op simulator --soc-version=Ascend950PR_9599 ./demo 