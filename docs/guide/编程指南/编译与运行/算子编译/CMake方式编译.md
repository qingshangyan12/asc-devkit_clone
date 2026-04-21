# 通过CMake编译<a name="ZH-CN_TOPIC_0000002428982142"></a>

项目中可以使用CMake来更简便地使用毕昇编译器编译Ascend C算子，生成可执行文件、动态库、静态库或二进制文件。

以下是CMake脚本的示例及其核心步骤说明：

```
# 1、find_package(ASC)是CMake中用于查找和配置Ascend C编译工具链的命令
find_package(ASC)  

# 2、指定项目支持的语言包括ASC和CXX，ASC表示支持使用毕昇编译器对Ascend C编程语言进行编译
project(kernel_samples LANGUAGES ASC CXX)

# 3、使用CMake接口编译可执行文件、动态库、静态库、二进制文件
add_executable(demo
    add_custom.asc
)
#.....
target_compile_options(demo PRIVATE
    # --npu-arch用于指定NPU的架构版本，dav-后为架构版本号，各产品型号对应的架构版本号请通过[对应关系表](../../语言扩展层/SIMD-BuiltIn关键字.md#table65291052154114)进行查询。
    # <COMPILE_LANGUAGE:ASC>:表明该编译选项仅对语言ASC生效
    $<$<COMPILE_LANGUAGE:ASC>: --npu-arch=dav-2201>    
)
```

以下是动态库、静态库编译示例，同时展示如何将源文件切换为用语言ASC编译：

-   编译.cpp文件生成动态库

    ```
    # 将.cpp文件置为ASC属性，启用Ascend C语言进行编译
    set_source_files_properties(
        add_custom_base.cpp 
        sub_custom_base.cpp
        PROPERTIES LANGUAGE ASC
    )
    
    add_library(kernel_lib SHARED
        add_custom_base.cpp 
        sub_custom_base.cpp
    )
    
    target_compile_options(kernel_lib PRIVATE
        $<$<COMPILE_LANGUAGE:ASC>: --npu-arch=dav-2201>
    )
    
    add_executable(demo
        main.cpp
    )
    target_link_libraries(demo PRIVATE
        kernel_lib
    )
    ```

-   编译.asc文件生成静态库

    ```
    # .asc文件会默认启用Ascend C语言进行编译，不需要通过set_source_files_properties进行设置
    add_library(kernel_lib STATIC
        add_custom_base.asc 
        sub_custom_base.asc
    )
    
    target_compile_options(kernel_lib PRIVATE
        $<$<COMPILE_LANGUAGE:ASC>: --npu-arch=dav-2201>
    )
    
    add_executable(demo
        main.cpp
    )
    target_link_libraries(demo PRIVATE
        kernel_lib
    )
    ```




