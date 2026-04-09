#!/bin/bash

# env
export ASCEND_HOME_DIR=/usr/local/Ascend/ascend-toolkit/latest
source $ASCEND_HOME_DIR/set_env.sh

# config
echo $PWD
code_path=$PWD/../examples
log_path=$PWD/tmplog_devkit


example_list=(
01_simd_cpp_api/00_introduction/01_vector/basic_api_memory_allocator_add
01_simd_cpp_api/00_introduction/01_vector/basic_api_tque_add
01_simd_cpp_api/00_introduction/02_matrix/matmul
01_simd_cpp_api/00_introduction/03_fusion_operation/matmul_leakyrelu
01_simd_cpp_api/01_utilities/03_cpudebug
01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/batch_mmad
01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/fixpipe_co12c1_tensor_quantization_s322f16
01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/fixpipe_co12gm_tensor_quantization_s322f16
01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/load_data
01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/load_data_with_transpose_b16
01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_load3dv2
01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_s8_f16_f32_with_A_B_transpose_option
01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/brcb
01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/compare
01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/gather
01_simd_cpp_api/02_features/03_basic_api/02_memory_vector_compute/mrg_sort
01_simd_cpp_api/02_features/03_basic_api/04_resource_management/get_tpipe_ptr
01_simd_cpp_api/02_features/03_basic_api/06_sync_control/group_barrier
01_simd_cpp_api/02_features/03_basic_api/08_atomic_operations/set_atomic_add
01_simd_cpp_api/03_libraries/00_matrix/matmul_fused_mannul
01_simd_cpp_api/03_libraries/00_matrix/matmul_a2b2_share
01_simd_cpp_api/03_libraries/00_matrix/matmul_tscm
01_simd_cpp_api/03_libraries/01_activation/gelu
01_simd_cpp_api/03_libraries/02_normalization/layernorm
01_simd_cpp_api/03_libraries/03_quantization/antiquant
01_simd_cpp_api/03_libraries/04_reduce/reducemax
01_simd_cpp_api/03_libraries/05_sort/sort
01_simd_cpp_api/03_libraries/06_index/arange
01_simd_cpp_api/03_libraries/07_filter/dropout
01_simd_cpp_api/03_libraries/08_tensor_transformation/broadcast
01_simd_cpp_api/03_libraries/12_math/acosh
02_simd_c_api/00_introduction/01_add/c_api_async_add
02_simd_c_api/00_introduction/01_add/c_api_delicacy_async_add
02_simd_c_api/00_introduction/01_add/c_api_sync_add
)

example1(){
	cd $1/$2/
	rm -rf build
	mkdir -p build && cd build  # 创建并进入build目录
	cmake ..;make -j            # 编译工程
	python3 ../scripts/gen_data.py  # 生成测试输入数据
	./demo                       # 执行编译生成的可执行程序，执行样例
	python3 ../scripts/verify_result.py output/output.bin output/golden.bin # 验证输出结果是否正确，确认算法逻辑正确
}

example2(){
	cd $1/$2/
	rm -rf build
	mkdir -p build && cd build;   # 创建并进入build目录
	cmake ..;make -j;             # 编译工程
	python3 ../scripts/gen_data.py   # 生成测试输入数据
	./demo                        # 执行编译生成的可执行程序，执行样例
	python3 ../scripts/verify_result.py output/output1.bin output/golden1.bin output/output2.bin output/golden2.bin
}

example3(){
	cd $1/$2/
	rm -rf build
	mkdir -p build && cd build;   # 创建并进入build目录
	cmake ..;make -j;             # 编译工程
	python3 ../scripts/gen_data.py   # 生成测试输入数据
	./demo                        # 执行编译生成的可执行程序，执行样例
	python3 ../scripts/verify_result.py output/output_ABshare.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确

}

example4(){
	cd $1/$2/
	rm -rf build
	mkdir -p build && cd build;   # 创建并进入build目录
	cmake ..;make -j;             # 编译工程
	python3 ../scripts/gen_data.py   # 生成测试输入数据
	./demo                        # 执行编译生成的可执行程序，执行样例
	python3 ../scripts/verify_result.py output/output_noABshare.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确

}

example5(){
	cd $1/$2/
	rm -rf build
	mkdir -p build && cd build;        # 创建并进入build目录
	cmake .. -DDT_MODE=0;make -j;      # 编译工程, 0可选01234， 例如cmake .. -DDT_MODE=1;make -j;
	python3 ../scripts/gen_data.py  0  # 生成测试输入数据, 0可选01234， 例如python3 ../scripts/gen_data.py  1
	./demo                             # 执行编译生成的可执行程序，执行样例
	python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
}

example6(){
	cd $1/$2/
	rm -rf build
	mkdir -p build && cd build;   # 创建并进入build目录
	cmake ..;make -j;             # 编译工程
	./demo                        # 执行样例
}

example7(){
	cd $1/$2/
	rm -rf build
	mkdir -p build && cd build;   # 创建并进入build目录
	cmake ..;make -j;             # 编译工程
	./add                         # 执行样例
}

example8(){
	cd $1/$2/
	rm -rf build
	mkdir -p build && cd build;
	cmake .. -Dsoc_version=Ascend910B1; make -j
	python3 ../scripts/gen_data.py
	./add
	python3 ../scripts/verify_result.py output.bin golden.bin
}

example9(){
	cd $1/$2/
	rm -rf build
	mkdir -p build && cd build;           # 创建并进入build目录
	cmake ..;make -j;                     # 编译工程
	python3 ../scripts/gen_data.py        # 生成测试输入数据
	msprof op ./demo                      # 基于可执行文件demo通过msprof op或msprof op simulator执行算子调优
	python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
}

example10(){
	cd $1/$2/
	rm -rf build
	mkdir -p build && cd build;   # 创建并进入build目录
	cmake ..;make -j;             # 编译工程
	python3 ../scripts/gen_data.py   # 生成测试输入数据
	./add                        # 执行编译生成的可执行程序，执行样例
	python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
}

example11(){
	cd $1/$2/
	rm -rf build
	B=4 M=30 K=40 N=70
	mkdir -p build && cd build;      # 创建并进入build目录
	cmake .. -DB_SIZE=$B -DM_SIZE=$M -DK_SIZE=$K -DN_SIZE=$N;make -j;    # 编译工程
	python3 ../scripts/gen_data.py -b=$B -m=$M -k=$K -n=$N   # 生成测试输入数据
	./demo                           # 执行编译生成的可执行程序，执行样例
	python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确

}

example12(){
	cd $1/$2/
	rm -rf build
	SCENARIO=4 M=30 K=40 N=70
	mkdir -p build && cd build;      # 创建并进入build目录
	cmake .. -DSCENARIO_NUM=$SCENARIO -DM_SIZE=$M -DK_SIZE=$K -DN_SIZE=$N;make -j;    # 编译工程
	python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO -m=$M -k=$K -n=$N   # 生成测试输入数据
	./demo                           # 执行编译生成的可执行程序，执行样例
	python3 ../scripts/verify_result.py -scenarioNum=$SCENARIO output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
}

example13(){
	cd $1/$2/
	rm -rf build
	SCENARIO=12 M=30 K=70 N=50
	mkdir -p build && cd build;      # 创建并进入build目录
	cmake .. -DSCENARIO_NUM=$SCENARIO -DM_SIZE=$M -DK_SIZE=$K -DN_SIZE=$N;make -j;    # 编译工程
	python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO -m=$M -k=$K -n=$N   # 生成测试输入数据
	./demo                           # 执行编译生成的可执行程序，执行样例
	python3 ../scripts/verify_result.py -scenarioNum=$SCENARIO output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
}

example14(){
	cd $1/$2/
	rm -rf build
	mkdir -p build && cd build;
	cmake -DNPU_ARCH=dav-2201 ..;make -j; # 默认npu模式
	python3 ../scripts/gen_data.py
	./demo
}

example15(){
	cd $1/$2/
	rm -rf build
	mkdir -p build && cd build;   # 创建并进入build目录
	cmake ..;make -j;             # 编译工程
	python3 ../scripts/gen_data.py   # 生成测试输入数据
	./demo                        # 执行编译生成的可执行程序，执行样例
}

example16(){
	cd $1/$2/
	rm -rf build
	mkdir -p build && cd build;   # 创建并进入build目录
	cmake ..;make -j;             # 编译工程
	./c_api_add_example           # 执行样例
}

date_time3=`date "+%Y-%m-%d %H:%M:%S"`
echo "=== $date_time3 ==="
mkdir -p ${log_path}
rm -rf ${log_path}/*
rm -rf result_devkit.txt
rm -rf devikit_cases.txt

for example_name in ${example_list[*]}
do
	case_name=$(echo ${example_name} | awk -F '/' '{print $(NF)}')
	if [ "${case_name}" == "matmul_a2b2_share" ]; then
	date_time1=`date "+%Y-%m-%d %H:%M:%S"`
	echo ">>>>>>>>>>>>>>>>>>>>> $date_time1 run ${case_name} start! <<<<<<<<<<<<<<<<<<<<<"
	example2 ${code_path} ${example_name} 2>&1 | tee ${log_path}/${case_name}.log
	date_time2=`date "+%Y-%m-%d %H:%M:%S"`
    durantion=`echo $(($(date +%s -d "${date_time2}") - $(date +%s -d "${date_time1}"))) | awk '{t=split("60 s 60 m 24 h 999 d",a);for(n=1;n<t;n+=2){if($1==0)break;s=$1%a[n]a[n+1]s;$1=int($1/a[n])}print s}'`
    echo "test case ${case_name} durantion:$durantion"
	echo ">>>>>>>>>>>>>>>>>>>>> $date_time2 run ${case_name} finished! <<<<<<<<<<<<<<<<<<<<<"
	elif [ "${case_name}" == "matmul_ibshareAB" ]; then
	date_time1=`date "+%Y-%m-%d %H:%M:%S"`
	echo ">>>>>>>>>>>>>>>>>>>>> $date_time1 run ${case_name} start! <<<<<<<<<<<<<<<<<<<<<"
	example3 ${code_path} ${example_name} 2>&1 | tee ${log_path}/${case_name}.log
	date_time2=`date "+%Y-%m-%d %H:%M:%S"`
    durantion=`echo $(($(date +%s -d "${date_time2}") - $(date +%s -d "${date_time1}"))) | awk '{t=split("60 s 60 m 24 h 999 d",a);for(n=1;n<t;n+=2){if($1==0)break;s=$1%a[n]a[n+1]s;$1=int($1/a[n])}print s}'`
    echo "test case ${case_name} durantion:$durantion"
	echo ">>>>>>>>>>>>>>>>>>>>> $date_time2 run ${case_name} finished! <<<<<<<<<<<<<<<<<<<<<"
	elif [ "${case_name}" == "matmul_no_ibshareAB" ]; then
	date_time1=`date "+%Y-%m-%d %H:%M:%S"`
	echo ">>>>>>>>>>>>>>>>>>>>> $date_time1 run ${case_name} start! <<<<<<<<<<<<<<<<<<<<<"
	example4 ${code_path} ${example_name} 2>&1 | tee ${log_path}/${case_name}.log
	date_time2=`date "+%Y-%m-%d %H:%M:%S"`
    durantion=`echo $(($(date +%s -d "${date_time2}") - $(date +%s -d "${date_time1}"))) | awk '{t=split("60 s 60 m 24 h 999 d",a);for(n=1;n<t;n+=2){if($1==0)break;s=$1%a[n]a[n+1]s;$1=int($1/a[n])}print s}'`
    echo "test case ${case_name} durantion:$durantion"
	echo ">>>>>>>>>>>>>>>>>>>>> $date_time2 run ${case_name} finished! <<<<<<<<<<<<<<<<<<<<<"
	elif [ "${case_name}" == "matmul_b8" ]||[ "${case_name}" == "matmul_ibshareB" ]; then
	date_time1=`date "+%Y-%m-%d %H:%M:%S"`
	echo ">>>>>>>>>>>>>>>>>>>>> $date_time1 run ${case_name} start! <<<<<<<<<<<<<<<<<<<<<"
	example5 ${code_path} ${example_name} 2>&1 | tee ${log_path}/${case_name}.log
	date_time2=`date "+%Y-%m-%d %H:%M:%S"`
    durantion=`echo $(($(date +%s -d "${date_time2}") - $(date +%s -d "${date_time1}"))) | awk '{t=split("60 s 60 m 24 h 999 d",a);for(n=1;n<t;n+=2){if($1==0)break;s=$1%a[n]a[n+1]s;$1=int($1/a[n])}print s}'`
    echo "test case ${case_name} durantion:$durantion"
	echo ">>>>>>>>>>>>>>>>>>>>> $date_time2 run ${case_name} finished! <<<<<<<<<<<<<<<<<<<<<"
	elif [ "${case_name}" == "hello_world_npu" ]||[ "${case_name}" == "add_dynamic" ]||[ "${case_name}" == "get_tpipe_ptr" ]||[ "${case_name}" == "group_barrier" ]||[ "${case_name}" == "02_dump" ]||[ "${case_name}" == "acosh" ]; then
	date_time1=`date "+%Y-%m-%d %H:%M:%S"`
	echo ">>>>>>>>>>>>>>>>>>>>> $date_time1 run ${case_name} start! <<<<<<<<<<<<<<<<<<<<<"
	example6 ${code_path} ${example_name} 2>&1 | tee ${log_path}/${case_name}.log
	date_time2=`date "+%Y-%m-%d %H:%M:%S"`
    durantion=`echo $(($(date +%s -d "${date_time2}") - $(date +%s -d "${date_time1}"))) | awk '{t=split("60 s 60 m 24 h 999 d",a);for(n=1;n<t;n+=2){if($1==0)break;s=$1%a[n]a[n+1]s;$1=int($1/a[n])}print s}'`
    echo "test case ${case_name} durantion:$durantion"
	echo ">>>>>>>>>>>>>>>>>>>>> $date_time2 run ${case_name} finished! <<<<<<<<<<<<<<<<<<<<<"
	elif [ "${case_name}" == "basic_api_tque_add" ]; then
	date_time1=`date "+%Y-%m-%d %H:%M:%S"`
	echo ">>>>>>>>>>>>>>>>>>>>> $date_time1 run ${case_name} start! <<<<<<<<<<<<<<<<<<<<<"
	example7 ${code_path} ${example_name} 2>&1 | tee ${log_path}/${case_name}.log
	date_time2=`date "+%Y-%m-%d %H:%M:%S"`
    durantion=`echo $(($(date +%s -d "${date_time2}") - $(date +%s -d "${date_time1}"))) | awk '{t=split("60 s 60 m 24 h 999 d",a);for(n=1;n<t;n+=2){if($1==0)break;s=$1%a[n]a[n+1]s;$1=int($1/a[n])}print s}'`
    echo "test case ${case_name} durantion:$durantion"
	echo ">>>>>>>>>>>>>>>>>>>>> $date_time2 run ${case_name} finished! <<<<<<<<<<<<<<<<<<<<<"
	elif [ "${case_name}" == "03_cpudebug" ]; then
	date_time1=`date "+%Y-%m-%d %H:%M:%S"`
	echo ">>>>>>>>>>>>>>>>>>>>> $date_time1 run ${case_name} start! <<<<<<<<<<<<<<<<<<<<<"
	example8 ${code_path} ${example_name} 2>&1 | tee ${log_path}/${case_name}.log
	date_time2=`date "+%Y-%m-%d %H:%M:%S"`
    durantion=`echo $(($(date +%s -d "${date_time2}") - $(date +%s -d "${date_time1}"))) | awk '{t=split("60 s 60 m 24 h 999 d",a);for(n=1;n<t;n+=2){if($1==0)break;s=$1%a[n]a[n+1]s;$1=int($1/a[n])}print s}'`
    echo "test case ${case_name} durantion:$durantion"
	echo ">>>>>>>>>>>>>>>>>>>>> $date_time2 run ${case_name} finished! <<<<<<<<<<<<<<<<<<<<<"
	elif [ "${case_name}" == "msProf" ]; then
	date_time1=`date "+%Y-%m-%d %H:%M:%S"`
	echo ">>>>>>>>>>>>>>>>>>>>> $date_time1 run ${case_name} start! <<<<<<<<<<<<<<<<<<<<<"
	example9 ${code_path} ${example_name} 2>&1 | tee ${log_path}/${case_name}.log
	date_time2=`date "+%Y-%m-%d %H:%M:%S"`
    durantion=`echo $(($(date +%s -d "${date_time2}") - $(date +%s -d "${date_time1}"))) | awk '{t=split("60 s 60 m 24 h 999 d",a);for(n=1;n<t;n+=2){if($1==0)break;s=$1%a[n]a[n+1]s;$1=int($1/a[n])}print s}'`
    echo "test case ${case_name} durantion:$durantion"
	echo ">>>>>>>>>>>>>>>>>>>>> $date_time2 run ${case_name} finished! <<<<<<<<<<<<<<<<<<<<<"
	elif [ "${case_name}" == "basic_api_memory_allocator_add" ]; then
	date_time1=`date "+%Y-%m-%d %H:%M:%S"`
	echo ">>>>>>>>>>>>>>>>>>>>> $date_time1 run ${case_name} start! <<<<<<<<<<<<<<<<<<<<<"
	example10 ${code_path} ${example_name} 2>&1 | tee ${log_path}/${case_name}.log
	date_time2=`date "+%Y-%m-%d %H:%M:%S"`
    durantion=`echo $(($(date +%s -d "${date_time2}") - $(date +%s -d "${date_time1}"))) | awk '{t=split("60 s 60 m 24 h 999 d",a);for(n=1;n<t;n+=2){if($1==0)break;s=$1%a[n]a[n+1]s;$1=int($1/a[n])}print s}'`
    echo "test case ${case_name} durantion:$durantion"
	echo ">>>>>>>>>>>>>>>>>>>>> $date_time2 run ${case_name} finished! <<<<<<<<<<<<<<<<<<<<<"
	elif [ "${case_name}" == "batch_mmad" ]; then
	date_time1=`date "+%Y-%m-%d %H:%M:%S"`
	echo ">>>>>>>>>>>>>>>>>>>>> $date_time1 run ${case_name} start! <<<<<<<<<<<<<<<<<<<<<"
	example11 ${code_path} ${example_name} 2>&1 | tee ${log_path}/${case_name}.log
	date_time2=`date "+%Y-%m-%d %H:%M:%S"`
    durantion=`echo $(($(date +%s -d "${date_time2}") - $(date +%s -d "${date_time1}"))) | awk '{t=split("60 s 60 m 24 h 999 d",a);for(n=1;n<t;n+=2){if($1==0)break;s=$1%a[n]a[n+1]s;$1=int($1/a[n])}print s}'`
    echo "test case ${case_name} durantion:$durantion"
	echo ">>>>>>>>>>>>>>>>>>>>> $date_time2 run ${case_name} finished! <<<<<<<<<<<<<<<<<<<<<"
	elif [ "${case_name}" == "mmad_load3dv2" ]; then
	date_time1=`date "+%Y-%m-%d %H:%M:%S"`
	echo ">>>>>>>>>>>>>>>>>>>>> $date_time1 run ${case_name} start! <<<<<<<<<<<<<<<<<<<<<"
	example12 ${code_path} ${example_name} 2>&1 | tee ${log_path}/${case_name}.log
	date_time2=`date "+%Y-%m-%d %H:%M:%S"`
    durantion=`echo $(($(date +%s -d "${date_time2}") - $(date +%s -d "${date_time1}"))) | awk '{t=split("60 s 60 m 24 h 999 d",a);for(n=1;n<t;n+=2){if($1==0)break;s=$1%a[n]a[n+1]s;$1=int($1/a[n])}print s}'`
    echo "test case ${case_name} durantion:$durantion"
	echo ">>>>>>>>>>>>>>>>>>>>> $date_time2 run ${case_name} finished! <<<<<<<<<<<<<<<<<<<<<"
	elif [ "${case_name}" == "mmad_s8_f16_f32_with_A_B_transpose_option" ]; then
	date_time1=`date "+%Y-%m-%d %H:%M:%S"`
	echo ">>>>>>>>>>>>>>>>>>>>> $date_time1 run ${case_name} start! <<<<<<<<<<<<<<<<<<<<<"
	example13 ${code_path} ${example_name} 2>&1 | tee ${log_path}/${case_name}.log
	date_time2=`date "+%Y-%m-%d %H:%M:%S"`
    durantion=`echo $(($(date +%s -d "${date_time2}") - $(date +%s -d "${date_time1}"))) | awk '{t=split("60 s 60 m 24 h 999 d",a);for(n=1;n<t;n+=2){if($1==0)break;s=$1%a[n]a[n+1]s;$1=int($1/a[n])}print s}'`
    echo "test case ${case_name} durantion:$durantion"
	echo ">>>>>>>>>>>>>>>>>>>>> $date_time2 run ${case_name} finished! <<<<<<<<<<<<<<<<<<<<<"
	elif [ "${case_name}" == "gelu" ]; then
	date_time1=`date "+%Y-%m-%d %H:%M:%S"`
	echo ">>>>>>>>>>>>>>>>>>>>> $date_time1 run ${case_name} start! <<<<<<<<<<<<<<<<<<<<<"
	example14 ${code_path} ${example_name} 2>&1 | tee ${log_path}/${case_name}.log
	date_time2=`date "+%Y-%m-%d %H:%M:%S"`
    durantion=`echo $(($(date +%s -d "${date_time2}") - $(date +%s -d "${date_time1}"))) | awk '{t=split("60 s 60 m 24 h 999 d",a);for(n=1;n<t;n+=2){if($1==0)break;s=$1%a[n]a[n+1]s;$1=int($1/a[n])}print s}'`
    echo "test case ${case_name} durantion:$durantion"
	echo ">>>>>>>>>>>>>>>>>>>>> $date_time2 run ${case_name} finished! <<<<<<<<<<<<<<<<<<<<<"
	elif [ "${case_name}" == "layernorm" ]||[ "${case_name}" == "antiquant" ]||[ "${case_name}" == "reducemax" ]||[ "${case_name}" == "sort" ]||[ "${case_name}" == "arange" ]||[ "${case_name}" == "dropout" ]||[ "${case_name}" == "broadcast" ]||[ "${case_name}" == "philoxrandom" ]; then
	date_time1=`date "+%Y-%m-%d %H:%M:%S"`
	echo ">>>>>>>>>>>>>>>>>>>>> $date_time1 run ${case_name} start! <<<<<<<<<<<<<<<<<<<<<"
	example15 ${code_path} ${example_name} 2>&1 | tee ${log_path}/${case_name}.log
	date_time2=`date "+%Y-%m-%d %H:%M:%S"`
    durantion=`echo $(($(date +%s -d "${date_time2}") - $(date +%s -d "${date_time1}"))) | awk '{t=split("60 s 60 m 24 h 999 d",a);for(n=1;n<t;n+=2){if($1==0)break;s=$1%a[n]a[n+1]s;$1=int($1/a[n])}print s}'`
    echo "test case ${case_name} durantion:$durantion"
	echo ">>>>>>>>>>>>>>>>>>>>> $date_time2 run ${case_name} finished! <<<<<<<<<<<<<<<<<<<<<"
	elif [ "${case_name}" == "c_api_async_add" ]||[ "${case_name}" == "c_api_delicacy_async_add" ]||[ "${case_name}" == "c_api_sync_add" ]; then
	date_time1=`date "+%Y-%m-%d %H:%M:%S"`
	echo ">>>>>>>>>>>>>>>>>>>>> $date_time1 run ${case_name} start! <<<<<<<<<<<<<<<<<<<<<"
	example16 ${code_path} ${example_name} 2>&1 | tee ${log_path}/${case_name}.log
	date_time2=`date "+%Y-%m-%d %H:%M:%S"`
    durantion=`echo $(($(date +%s -d "${date_time2}") - $(date +%s -d "${date_time1}"))) | awk '{t=split("60 s 60 m 24 h 999 d",a);for(n=1;n<t;n+=2){if($1==0)break;s=$1%a[n]a[n+1]s;$1=int($1/a[n])}print s}'`
    echo "test case ${case_name} durantion:$durantion"
	echo ">>>>>>>>>>>>>>>>>>>>> $date_time2 run ${case_name} finished! <<<<<<<<<<<<<<<<<<<<<"
	else
	date_time1=`date "+%Y-%m-%d %H:%M:%S"`
	echo ">>>>>>>>>>>>>>>>>>>>> $date_time1 run ${case_name} start! <<<<<<<<<<<<<<<<<<<<<"
	example1 ${code_path} ${example_name} 2>&1 | tee ${log_path}/${case_name}.log
	date_time2=`date "+%Y-%m-%d %H:%M:%S"`
    durantion=`echo $(($(date +%s -d "${date_time2}") - $(date +%s -d "${date_time1}"))) | awk '{t=split("60 s 60 m 24 h 999 d",a);for(n=1;n<t;n+=2){if($1==0)break;s=$1%a[n]a[n+1]s;$1=int($1/a[n])}print s}'`
    echo "test case ${case_name} durantion:$durantion"
	echo ">>>>>>>>>>>>>>>>>>>>> $date_time2 run ${case_name} finished! <<<<<<<<<<<<<<<<<<<<<"
	fi
done

cd $log_path
ls * > ../devkit_cases.txt
sed -i 's/.log//g' ../devkit_cases.txt
cd ../
while read line
do
prf=$(grep -E "test pass|passed|\[Block \(5\/6\)\]: OUTPUT = 24" $log_path/$line.log)
if [ -n "$prf" ]; then
    echo "$line pass" >> result_devkit.txt
else
    echo "$line fail" >> result_devkit.txt
fi
done < devkit_cases.txt

prf2=$(grep -E "fail" devkit_cases.txt)
if [ -n "$prf2" ]; then
    echo "execute samples failed"
else
    echo "execute samples success"
fi

date_time4=`date "+%Y-%m-%d %H:%M:%S"`
echo "=== $date_time4 ==="
durantion1=`echo $(($(date +%s -d "${date_time4}") - $(date +%s -d "${date_time3}"))) | awk '{t=split("60 s 60 m 24 h 999 d",a);for(n=1;n<t;n+=2){if($1==0)break;s=$1%a[n]a[n+1]s;$1=int($1/a[n])}print s}'`
echo "test cases all durantion:$durantion1"
