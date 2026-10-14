"""通过一次不可覆盖的原子 rename 发布已校验的产物目录。

暂存目录必须由调用方私有，并在发布期间保持不变。rename 后 fsync 失败时，
说明结果已经可见但持久性未确认；此时报告该状态，不进行具有误导性的回滚。
"""

import ctypes
import os
from pathlib import Path
import stat
from .errors import SpecializationError


def _validate_no_symlinks(path):
    for component in (path, *path.parents):
        if component.is_symlink():
            raise SpecializationError('publish', f'symlink path is not allowed: {component}')


def _sync_tree(directory_fd, device):
    for name in os.listdir(directory_fd):
        info = os.stat(name, dir_fd=directory_fd, follow_symlinks=False)
        if info.st_dev != device:
            raise SpecializationError('publish', 'stage contains another filesystem')
        if stat.S_ISDIR(info.st_mode):
            fd = os.open(name, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW, dir_fd=directory_fd)
            try:
                _sync_tree(fd, device)
            finally:
                os.close(fd)
        elif stat.S_ISREG(info.st_mode):
            fd = os.open(name, os.O_RDONLY | os.O_NOFOLLOW, dir_fd=directory_fd)
            try:
                if not stat.S_ISREG(os.fstat(fd).st_mode):
                    raise SpecializationError('publish', 'stage entry changed type')
                os.fsync(fd)
            finally:
                os.close(fd)
        else:
            raise SpecializationError('publish', f'stage contains nonregular entry: {name}')
    os.fsync(directory_fd)


def publish_artifacts(stage_dir, destination_dir):
    """返回持久化发布后的目标 Path，绝不覆盖已有结果。"""
    stage = Path(os.path.abspath(stage_dir))
    destination = Path(os.path.abspath(destination_dir))
    renamed = False
    fds = []
    try:
        _validate_no_symlinks(stage)
        _validate_no_symlinks(destination)
        if stage == destination or stage in destination.parents or destination in stage.parents:
            raise SpecializationError('publish', 'stage and destination must be disjoint directories')
        if not stage.is_dir() or stage == stage.parent:
            raise SpecializationError('publish', 'stage must be a directory')
        if os.path.lexists(destination):
            raise SpecializationError('publish', 'destination already exists')
        stage_parent = os.open(stage.parent, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW)
        fds.append(stage_parent)
        dest_parent = os.open(destination.parent, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW)
        fds.append(dest_parent)
        stage_fd = os.open(stage.name, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW, dir_fd=stage_parent)
        fds.append(stage_fd)
        device = os.fstat(stage_fd).st_dev
        if any(os.fstat(fd).st_dev != device for fd in (stage_parent, dest_parent)):
            raise SpecializationError('publish', 'stage and destination must share a filesystem')
        libc = ctypes.CDLL(None, use_errno=True)
        try:
            rename = libc.renameat2
        except AttributeError as exc:
            raise SpecializationError('publish', 'renameat2 RENAME_NOREPLACE is unsupported') from exc
        rename.argtypes = [ctypes.c_int, ctypes.c_char_p, ctypes.c_int, ctypes.c_char_p, ctypes.c_uint]
        rename.restype = ctypes.c_int
        _sync_tree(stage_fd, device)
        rc = rename(stage_parent, os.fsencode(stage.name), dest_parent, os.fsencode(destination.name), 1)
        if rc != 0:
            error = ctypes.get_errno()
            raise OSError(error, os.strerror(error))
        renamed = True
        os.fsync(stage_parent)
        os.fsync(dest_parent)
        return destination
    except OSError as exc:
        suffix = '; directory published but durability is unconfirmed' if renamed else ''
        raise SpecializationError('publish', str(exc) + suffix, published=renamed) from exc
    finally:
        for fd in reversed(fds):
            os.close(fd)
