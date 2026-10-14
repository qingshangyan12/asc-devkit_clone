#!/usr/bin/env python3
# -*- coding: UTF-8 -*-
# ----------------------------------------------------------------------------
# Copyright Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
# ----------------------------------------------------------------------------
import json
import logging
import os
import subprocess
import sys
import shutil
import time
import fcntl
import re

# 运行时禁止生成__pycache__/文件夹
sys.dont_write_bytecode = True


def setting_logformat(log_level='INFO'):
    """
    :param log_level: 设置安装日志级别
    :return: None
    """
    if log_level.upper() not in ['DEBUG', 'INFO', 'WARNING', 'ERROR']:
        logging.error(f'{log_level} is not the correct configuration option')

    log_format = "%(asctime)s-%(levelname)s-%(message)s"
    date_format = "%m/%d/%Y %H:%M"
    logging.basicConfig(level=log_level, format=log_format, datefmt=date_format)


class UninstallPackage:
    def __init__(self, args):
        self.uninstall_mode = args[0] if len(args) == 1 else ''
        self.script_dir = os.path.dirname(os.path.realpath(__file__))
        self.package_dir = os.path.dirname(self.script_dir)
        self.package_prefix_dir = os.path.dirname(self.package_dir)
        self.package_name = os.path.basename(self.package_dir)
        self.install_path = os.path.dirname(os.path.dirname(self.package_prefix_dir))
        self.soc_version = ''


    def _get_soc_version(self):
        for root, _, files in os.walk(f'{self.package_dir}/config'):
            for file in files:
                if file == 'binary_info_config.json':
                    self.soc_version = os.path.basename(root)
                    logging.info(f'soc_version:---{self.soc_version}---')
                    break


    def __update_config_ini(self):
        ini_file = os.path.join(self.package_prefix_dir, 'config.ini')
        with open(ini_file, mode='r') as f:
            data = f.read()
        update_data = data.replace(f'={self.package_name},', '=').replace(f',{self.package_name}', '').replace(
            f'{self.package_name}', '').strip()
        logging.info('read and update config.ini settings')
        with open(ini_file, mode='w') as f:
            f.write(update_data)

        if update_data.endswith('='):
            return []
        package_list = update_data.split('=')[-1].split(',')

        return package_list


    def _update_uninstall_file(self, package_list):
        src_file = f'{self.package_prefix_dir}/uninstall.sh'
        with open(src_file, mode='r') as file:
            data = file.read()
        logging.debug(data)
        pattern = r'(?<=/)[^/]+(?=/scripts)'
        data = re.sub(pattern, package_list[-1], data)

        with open(src_file, mode='w') as file:
            file.write(data)
        os.chmod(src_file, 0o755)

    def update_shape_info(self, package_list):
        # 清空所有已存在的且soc_version相同的shape_info文件夹
        for root, dirs, _ in os.walk(f'{self.package_prefix_dir}/config/', topdown=False):
            for shape_info_dir in dirs:
                if shape_info_dir == 'shape_info' and self.soc_version in root:
                    logging.debug(f'shape_info_dir is: {root}/{shape_info_dir}')
                    shutil.rmtree(os.path.join(root, shape_info_dir))
        # shape_file更新
        shape_info_dir_list = []
        for i in package_list:
            if os.path.isdir(f'{self.package_prefix_dir}/{i}/config/{self.soc_version}/shape_info'):
                shape_info_dir_list.append(f'{self.package_prefix_dir}/{i}/config/{self.soc_version}/shape_info')
        if len(shape_info_dir_list) == 0:
            return
        if not os.path.isdir(f'{self.package_prefix_dir}/config/{self.soc_version}/shape_info'):
            os.makedirs(f'{self.package_prefix_dir}/config/{self.soc_version}/shape_info')
        for shape_info_dir in shape_info_dir_list:
            for root, dirs, files in os.walk(shape_info_dir):
                for shape_file in files:
                    shutil.copy2(os.path.join(root, shape_file),
                                 f'{self.package_prefix_dir}/config/{self.soc_version}/shape_info')


    def locked_update_json(self):
        self._get_soc_version()
        if self.uninstall_mode == '--nolock':
            self.update_json()
        else:
            lock_file = f'{self.package_prefix_dir}/config/static_kernel.lock'
            try:
                # 尝试以读写模式打开锁文件，如果不存在则创建
                fd = os.open(lock_file, os.O_RDWR | os.O_CREAT | os.O_EXCL)
                os.close(fd)
            except OSError:
                # 如果锁文件已经存在，说明其他进程正在运行，那么其他进程要么在使用锁，锁文件已经存在，要么在创建文件锁，1秒钟肯定够用
                logging.debug(f'Another instance is already running.')
                time.sleep(1)

            with open(lock_file, 'r+') as f:
                fcntl.flock(f, fcntl.LOCK_EX)
                try:
                    logging.debug(f'[PID {os.getpid()}] has locked the file lock')
                    self.update_json()
                finally:
                    fcntl.flock(f, fcntl.LOCK_UN)
                    logging.debug(f'[PID {os.getpid()}] has unlocked the file lock')


    @staticmethod
    def _remove_dir_with_lock_preserved(dir_path, preserve_file):
        """
        删除目录下所有内容，但保留指定文件及其所在的目录结构。
        :param dir_path: 待清理的目录路径
        :param preserve_file: 需要保留的文件完整路径
        :return: None
        """
        if not os.path.isdir(dir_path):
            return
        preserve_path = os.path.normpath(preserve_file) if os.path.exists(preserve_file) else None
        for root, dirs, files in os.walk(dir_path, topdown=False):
            for file_name in files:
                file_path = os.path.join(root, file_name)
                if preserve_path is not None and os.path.normpath(file_path) == preserve_path:
                    logging.debug(f'保留锁文件: {file_path}')
                    continue
                os.remove(file_path)
            for dir_name in dirs:
                dir_to_remove = os.path.join(root, dir_name)
                try:
                    os.rmdir(dir_to_remove)
                except OSError:
                    logging.debug(f'目录非空，保留: {dir_to_remove}')


    def update_json(self):
        package_list = self.__update_config_ini()
        if self.uninstall_mode == '--all':
            package_list = []
        logging.debug(f'package_list: ---{package_list}---')
        logging.info('ready to update binary_info_config.json')
        if len(package_list) == 0:
            logging.debug('删除整个目录')
            aicore_path = f'{self.install_path}/static_kernel/ai_core'
            # lock file will always exist and will not been removed, to avoid parallellized install task
            lock_file = f'{aicore_path}/config/static_kernel.lock'
            self._remove_dir_with_lock_preserved(aicore_path, lock_file)
            return True
        self._update_uninstall_file(package_list)
        json_list = [f'{self.package_prefix_dir}/{i}/config/{self.soc_version}/binary_info_config.json'
                     for i in package_list if
                     os.path.isfile(f'{self.package_prefix_dir}/{i}/config/{self.soc_version}/binary_info_config.json')]
        logging.debug(f'json_list: ---{json_list}---')
        if len(json_list) >= 1:
            with open(json_list[0], mode='r') as f:
                src_info = json.load(f)
            del json_list[0]
            src_info = self.__parse_json(src_info, json_list)
            logging.debug(f'comment of binary_info_config.json is: {src_info}')
            with open(f'{self.package_prefix_dir}/config/{self.soc_version}/binary_info_config.json', mode='w') as f:
                json.dump(src_info, f, indent=4, sort_keys=True)

            shutil.rmtree(self.package_dir)
            self.update_shape_info(package_list)

        else:
            logging.debug('删除当前子包整个目录')
            shutil.rmtree(f'{self.package_prefix_dir}/config/{self.soc_version}')
            shutil.rmtree(self.package_dir)
            return True

        return True

    @staticmethod
    def __update_src_info(key: str, dst_info: dict, src_info: dict) -> dict:
        if 'valueDependIndex' in dst_info[key]:
            src_info[key]['valueDependIndex'] = dst_info[key]['valueDependIndex']
        elif 'valueDependIndex' in src_info[key]:
            del src_info[key]['valueDependIndex']

        if 'simplifiedKeyMode' in dst_info[key]:
            src_info[key]['simplifiedKeyMode'] = dst_info[key]['simplifiedKeyMode']
        elif 'simplifiedKeyMode' in src_info[key]:
            del src_info[key]['simplifiedKeyMode']
        for i in dst_info[key]['staticList']:
            simplified_key = i['simplifiedKeyWithPlatform']
            for index, j in enumerate(src_info[key]['staticList']):
                if simplified_key == j['simplifiedKeyWithPlatform']:
                    src_info[key]['staticList'][index] = i
                    break
            else:
                src_info[key]['staticList'].append(i)

    def __parse_json(self, src_info, json_list):
        if len(json_list) >= 1:
            with open(json_list[0], mode='r') as f:
                dst_info = json.load(f)
            keys_list = dst_info.keys()
            for key in keys_list:
                if key not in src_info:
                    src_info.setdefault(key, dst_info[key])
                    continue
                self.__update_src_info(key, dst_info, src_info)
            logging.debug(f'src_info: {src_info}')

            del json_list[0]
            if len(json_list) < 1:
                return src_info
        else:
            return src_info
        return self.__parse_json(src_info, json_list)


def main():
    setting_logformat()
    args = sys.argv[1:]
    if len(args) != 0 and len(args) != 1:
        logging.error(f'number of input parameters is not right')
        return False
    worker = UninstallPackage(args)
    worker.locked_update_json()
    return True


if __name__ == '__main__':
    if not main():
        sys.exit(1)
