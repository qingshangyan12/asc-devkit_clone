# // 除了asc_simd.h(include)  
# // impl define逻辑  
#if !defined(__ASCEND_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS__)  




#warning "replaceFileNameString is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use asc_simd.h and use public functions or variables defined in interface headers files."  




#define __ASCEND_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS__  
#define __UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC__  
#endif  




# // impl undefine逻辑  
#if defined(__UNDEF_ASCEND_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC__)  
#undef __ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS__  
#undef __UNDEF_ASCENDC_C_API_INCLUDE_COMPILDER_INTERNAL_HEADERS_ASCENDC__  
#endif  




# // impl define逻辑  
#if !defined(__ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS__)  




#warning "crt/mma.hpp is an internal header file and must not be used directly. Please use ASCENDC_runtime_api.h or ASCENDC_runtime.h instead."  




#define __ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS__  
#define __UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC__  
#endif  




# // impl undefine逻辑  
#if defined(__UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC__)  
#undef __ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS__  
#undef __UNDEF_ASCENDC_C_API_INCLUDE_COMPILDER_INTERNAL_HEADERS_ASCENDC__  
#endif  




# // include  
#if !defined(__ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS__)  
#define __ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS__  
#define __UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H__  
#endif  




#if defined(__UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H__)  
#undef __ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS__  
#undef __UNDEF_ASCENDC_C_API_INCLUDE_COMPILDER_INTERNAL_HEADERS_ASCENDC_C_API_H__  
#endif  








import os  
import shutil  
import sys  
from typing import List  




TARGET_EXTENSION: List[str] = [".h", ".hpp", ".cpp", ".c", ".cc"]  
FILE_ENCODING: str = "utf-8"  
BAK_SUFFIX: str = ".bak"  








def get_all_files(dir_path: str) -> List[str]:  
    file_list = []  
    for root, _, files in os.walk(dir_path):  
        print(root)  
        print(files)  
        for file in files:  
            print(f"file:{file}\n")  
            if not TARGET_EXTENSION or any(file.endswith(ext) for ext in TARGET_EXTENSION):  
                file_path = os.path.join(root, file)  
                print(file_path)  
                file_list.append(file_path)  
    return file_list  




def find_first_ifndef(lines: List[str]) -> int:  
    for idx, line in enumerate(lines):  
        if "#ifndef" in line.strip():  
            return idx  
    return -1  




def process_file(file_path: str, prefix_template: str, suffix_template: str):  
    file_name = os.path.basename(file_path)  
    actual_prefix = prefix_template.replace("replaceFileNameString", file_name)  
    actual_suffix = suffix_template.replace("replaceFileNameString", file_name)  




    try:  
        with open(file_path, "r", encoding=FILE_ENCODING) as f:  
            lines = f.readlines()  




            ifndef_idx = find_first_ifndef(lines)  
            if ifndef_idx != -1:  
                lines.insert(ifndef_idx, "\n")  
                lines.insert(ifndef_idx + 1, f"{actual_prefix}\n")  
                print(f"[前缀] 在{file_path}的第{ifndef_idx}行#ifndef前插入内容")  




            else:  
                print(f"[提示] {file_path}中未找到#ifndef，跳过前缀插入")  




        if lines:  
            if not lines[-1].endswith("\n"):  
                lines[-1] += "\n"  
        lines.append("\n")  
        lines.append(f"{actual_suffix}\n")  




        with open(file_path, "w", encoding=FILE_ENCODING) as f:  
            f.writelines(lines)  
        print(f"[成功] 处理完成：{file_path}\n")  




    except Exception as e:  
        shutil.move(bak_file_path, file_path)  
        print(f"[失败] 处理{file_path}时出错：{str(e)}，已恢复文件\n")  




def main():  
    target_dir = "m:/Users/g00954368/Desktop/code/code_marco/asc-devkit-dev/include/"  




    prefix_template = """#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)  
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS  
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H  
#endif    
"""  

    suffxi_template = """#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_C_API_H)  
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS  
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILDER_INTERNAL_HEADERS_ASCENDC_C_API_H  
#endif  
"""  
    if not os.path.isdir(target_dir):  
        print(f"错误：目标目录{target_dir}不存在！")  
        sys.exit(1)  




    file_list = get_all_files(target_dir)  
    if not file_list:  
        print("提示：未找到符合后缀条件的文件！")  
        sys.exit(0)  




    print(f"共找到{len(file_list)} 个待处理文件，开始处理...\n")  
    for file_path in file_list:  
        process_file(file_path, prefix_template, suffxi_template)  




    print("所有文件处理完成！")  
    print(f"注：源文件已备份为 {BAK_SUFFIX} 后缀，确认无误后可手动删除备份文件")  


if __name__ == "__main__":  
    script_full_path = os.path.abspath(__file__)  
    print("脚本绝对路径（含文件名）:", script_full_path)  
    main()