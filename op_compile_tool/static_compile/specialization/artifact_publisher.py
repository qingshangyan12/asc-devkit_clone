"""将已校验的产物逐个发布到共享输出目录，不覆盖已有文件。

暂存目录必须由调用方私有。文件分别原子 rename，整个文件对不具备事务原子性；
任一文件发布后发生错误均报告 published=True，由父进程中止打包。
"""

import ctypes
import errno
import os
from pathlib import Path
import stat
from .errors import ArtifactCollisionError, SpecializationError


def _validate_no_symlinks(path):
    for component in (path, *path.parents):
        if component.is_symlink():
            raise SpecializationError('publish', f'symlink path is not allowed: {component}')


def _sync_files(directory_fd, names):
    for name in names:
        fd = os.open(name, os.O_RDONLY | os.O_NOFOLLOW, dir_fd=directory_fd)
        try:
            os.fsync(fd)
        finally:
            os.close(fd)
    os.fsync(directory_fd)


def publish_artifacts(stage_dir, destination_dir):
    """fsync 后将暂存文件移入已有目标目录，返回该目录 Path。"""
    stage = Path(os.path.abspath(stage_dir))
    destination = Path(os.path.abspath(destination_dir))
    published = False
    fds = []
    try:
        _validate_no_symlinks(stage)
        _validate_no_symlinks(destination)
        if stage == destination or stage in destination.parents or destination in stage.parents:
            raise SpecializationError('publish', 'stage and destination must be disjoint directories')
        if not stage.is_dir() or stage == stage.parent:
            raise SpecializationError('publish', 'stage must be a directory')
        stage_parent = os.open(stage.parent, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW)
        fds.append(stage_parent)
        dest_fd = os.open(destination, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW)
        fds.append(dest_fd)
        stage_fd = os.open(stage.name, os.O_RDONLY | os.O_DIRECTORY | os.O_NOFOLLOW, dir_fd=stage_parent)
        fds.append(stage_fd)
        device = os.fstat(stage_fd).st_dev
        if any(os.fstat(fd).st_dev != device for fd in (stage_parent, dest_fd)):
            raise SpecializationError('publish', 'stage and destination must share a filesystem')
        names = sorted(os.listdir(stage_fd))
        for name in names:
            info = os.stat(name, dir_fd=stage_fd, follow_symlinks=False)
            if not stat.S_ISREG(info.st_mode):
                raise SpecializationError('publish', f'stage contains nonregular artifact: {name}')
            if os.path.lexists(destination / name):
                raise ArtifactCollisionError('publish', f'artifact already exists: {destination / name}')
        libc = ctypes.CDLL(None, use_errno=True)
        try:
            rename = libc.renameat2
        except AttributeError as exc:
            raise SpecializationError('publish', 'renameat2 RENAME_NOREPLACE is unsupported') from exc
        rename.argtypes = [ctypes.c_int, ctypes.c_char_p, ctypes.c_int, ctypes.c_char_p, ctypes.c_uint]
        rename.restype = ctypes.c_int
        _sync_files(stage_fd, names)
        for name in names:
            rc = rename(stage_fd, os.fsencode(name), dest_fd, os.fsencode(name), 1)
            if rc != 0:
                error = ctypes.get_errno()
                if error == errno.EEXIST:
                    raise ArtifactCollisionError('publish', f'artifact already exists: {destination / name}',
                                                 published=published)
                raise OSError(error, os.strerror(error))
            published = True
        os.fsync(stage_fd)
        os.fsync(dest_fd)
        stage.rmdir()
        os.fsync(stage_parent)
        return destination
    except OSError as exc:
        suffix = '; artifacts are already visible' if published else ''
        raise SpecializationError('publish', str(exc) + suffix, published=published) from exc
    finally:
        for fd in reversed(fds):
            os.close(fd)
