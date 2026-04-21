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

下文列出了使用CMake编译时常用的链接库、以及默认链接库。

**表 1**  常用的链接库（在使用高阶API时，必须链接以下库，因为这些库是高阶API功能所依赖的。在其他场景下，可以根据具体需求选择是否链接这些库。）

<a name="table1632724817328"></a>
<table><thead align="left"><tr id="row1132712485321"><th class="cellrowborder" valign="top" width="42.19%" id="mcps1.2.3.1.1"><p id="p377063619281"><a name="p377063619281"></a><a name="p377063619281"></a>使用场景</p>
</th>
<th class="cellrowborder" valign="top" width="57.809999999999995%" id="mcps1.2.3.1.2"><p id="p5423124282814"><a name="p5423124282814"></a><a name="p5423124282814"></a>名称</p>
</th>
</tr>
</thead>
<tbody><tr id="row131961149143112"><td class="cellrowborder" rowspan="3" valign="top" width="42.19%" headers="mcps1.2.3.1.1 "><p id="p477013682820"><a name="p477013682820"></a><a name="p477013682820"></a>使用高阶API相关的Tiling接口时需要同时链接。</p>
</td>
<td class="cellrowborder" valign="top" width="57.809999999999995%" headers="mcps1.2.3.1.2 "><p id="p94248427280"><a name="p94248427280"></a><a name="p94248427280"></a>libtiling_api.a</p>
</td>
</tr>
<tr id="row957113206355"><td class="cellrowborder" valign="top" headers="mcps1.2.3.1.1 "><p id="p1942416423283"><a name="p1942416423283"></a><a name="p1942416423283"></a>libregister.so</p>
</td>
</tr>
<tr id="row1652295021916"><td class="cellrowborder" valign="top" headers="mcps1.2.3.1.1 "><p id="p134241942162817"><a name="p134241942162817"></a><a name="p134241942162817"></a>libgraph_base.so</p>
</td>
</tr>
<tr id="row025115214213"><td class="cellrowborder" valign="top" width="42.19%" headers="mcps1.2.3.1.1 "><p id="p1777015364285"><a name="p1777015364285"></a><a name="p1777015364285"></a>使用PlatformAscendC相关硬件平台信息接口时需要链接。</p>
</td>
<td class="cellrowborder" valign="top" width="57.809999999999995%" headers="mcps1.2.3.1.2 "><p id="p1642415421284"><a name="p1642415421284"></a><a name="p1642415421284"></a>libplatform.so</p>
</td>
</tr>
</tbody>
</table>



