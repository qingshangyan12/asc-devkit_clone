#!/bin/bash
SOC_VERSION=${1:-"910B"}
EXAMPLES_DIR="$(pwd)/examples"
function run_case(){
    if [[ -f $1/README.md ]];then
        if grep -q "${SOC_VERSION}" $1/README.md;then
            sed -n '/默认路径，root用户安装CANN软件包/{:a; N; /```\n/!ba;s/.*\n```\n//; p; q}' $1/README.md | tail -n +3 |  head -n -2 > $1/auto_run.sh
            sed -n '/配置安装路径后，执行以下命令统一配置环境变量。/{:a; N; /```\n/!ba;s/.*\n```\n//; p; q}' $1/README.md | tail -n +3 |  head -n -2 >> $1/auto_run.sh
            sed -n '/- 样例执行/{:a; N; /```\n/!ba; p; q}' $1/README.md | tail -n +3 | head -n -2 >> $1/auto_run.sh
            expected_output=$(sed -n '/执行结果如下/{:a; N; /```\n/!ba;s/.*\n```\n//; p; q}' $1/README.md | tail -n +3 | head -n -2 | sed -n 's/^[[:space:]]*//p')
            echo "================START RUN $1 SOC_VERSION:${SOC_VERSION}=========="
            if [[ $1 =~ 'cpudebug' ]];then
                cd $1;bash auto_run.sh Ascend910B | tee $1.log;
            elif [[ $1 =~ '02_dumptensor' ]];then
                sed -n '/执行cube.asc样例的命令如下所示：/{:a; N; /```\n/!ba;s/.*\n```\n//; p; q}' $1/README.md | tail -n +3 | head -n -2 >> $1/auto_run.sh
                sed -n '/执行vector.asc样例的命令如下所示：/{:a; N; /```\n/!ba;s/.*\n```\n//; p; q}' $1/README.md | tail -n +3 | head -n -2 >> $1/auto_run.sh
                cd $1;bash auto_run.sh | tee $1.log;
            else
                cd $1;bash auto_run.sh | tee $1.log;
            fi
            echo "'${expected_output}'" | grep -Fz - $1.log
            if [ $? -eq 0 ];then
                    result="pass"
            else
                    result="fail"
            fi
            echo "$1:$result" >> ${EXAMPLES_DIR}/result.txt
            cd -
        else
            echo "===================$1 done't support ${SOC_VERSION}===================="
        fi
    fi
}

find $EXAMPLES_DIR -mindepth 2 -maxdepth 2 -type d -name "*_*" | while read dir;do
    cd $dir
    if find . -maxdepth 1  -type d -name "*_*" | grep -q './';then
        find . -maxdepth 1  -type d -name "*_*" | while read subdir;do
            run_case $subdir
        done
    else
        run_case $dir
    fi
    cd - > /dev/null
done