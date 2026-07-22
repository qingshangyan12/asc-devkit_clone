          set -x
          cd ${{ steps.process_checkout.outputs.path }}
          echo $(grep -E "^VERSION_ID=" /etc/os-release | cut -d'"' -f2)
          sudo update-alternatives --set gcc /usr/bin/gcc-14
          gcc --version
          source /home/jenkins/Ascend/cann/bin/setenv.bash
          set +e
          if [ "$TARGET_BRANCH" = "master" ] || [ "$TARGET_BRANCH" == "9.1.0-beta.2" ]; then
              bash build.sh --${{ inputs.ut_type }} --cov --cann_3rd_lib_path="/home/jenkins/opensource" -f "pr_filelist.txt"
          elif [ "$TARGET_BRANCH" = "exp/kadc" ]; then
              echo "not need run ut"
              exit 0
          elif [ "$TARGET_BRANCH" = "feature/tensor_api_from_9.0.0" ]; then
            if [ "${{ inputs.ut_type }}" = "UT_Test_basic_test_three" ] || [ "${{ inputs.ut_type }}" == "UT_Test_basic_test_two" ] || [ "${{ inputs.ut_type }}" == "UT_Test_basic_test_one" ]; then
              bash build.sh --${{ inputs.ut_type }} --cann_3rd_lib_path="/home/jenkins/opensource"
            else
              echo "not need run ut"
              exit 0
            fi
          else
            if [ "${{ inputs.ut_type }}" = "UT_Test_basic_test_three" ] || [ "${{ inputs.ut_type }}" == "UT_Test_basic_test_two" ] || [ "${{ inputs.ut_type }}" == "UT_Test_basic_test_one" ] || [ "${{ inputs.ut_type }}" == "UT_Test_adv_test*" ]; then
              bash build.sh --${{ inputs.ut_type }} --cov --cann_3rd_lib_path="/home/jenkins/opensource"
            else
              echo "not need run ut"
              exit 0
            fi
          fi

          if [ $ret -ne 200 ] && [ $ret -ne 0 ]; then
              echo "run ut fail"
              exit 1
          fi
          # if [ $ret -eq 0 ]; then
          #     if [ "$coverage_save" = "true" ];then
          #       echo "ut_process=coverage" >> $ATOMGIT_OUTPUT
          #     else
          #       echo "ut_process=ut_cov" >> $ATOMGIT_OUTPUT
          #     fi
          # fi
          echo "ut_process=coverage" >> "${ATOMGIT_OUTPUT}"
          exit 0
