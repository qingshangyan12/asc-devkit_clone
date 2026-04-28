# 通过CMake编译<a name="ZH-CN_TOPIC_0000002428982142"></a>

项目中可以使用CMake来更简便地使用毕昇编译器编译Ascend C算子，生成可执行文件、动态库、静态库或二进制文件。

以下是CMake脚本的示例及其核心步骤说明：

```cmake
# 1、find_package(ASC)是CMake中用于查找和配置Ascend C编译工具链的命令
find_package(ASC)  

# 2、指定项目支持的语言包括ASC，ASC表示支持使用毕昇编译器对Ascend C编程语言进行编译
project(kernel_samples LANGUAGES ASC)

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

-   编译.asc文件生成动态库

    ```cmake
    # 3、使用CMake接口编译动态库
    add_library(kernel_shared SHARED
        cube.asc 
        vector.asc
        mix.asc
    )
    ```

-   编译.asc文件生成静态库

    ```cmake
    # 3、使用CMake接口编译静态库
    add_library(kernel_lib STATIC
          cube.asc 
          vector.asc
          mix.asc
    )
    ```
    表1 常用的CMAKE配置变量说明
    
|  变量名称| 配置说明 |  
|--|--|
|CMAKE_BUILD_TYPE  | 编译模式选项，可配置为：“Release”，Release版本，不包含调试信息，编译最终发布的版本。“Debug”，Debug版本，包含调试信息，便于开发者开发和调试。 |  
|CMAKE_INSTALL_PREFIX | 用于指定CMake执行install时，安装的路径前缀，执行install后编译产物（ascendc_library中指定的target以及对应的头文件）会安装在该路径下。默认路径为当前目录的out目录下。。 |  
