"""特化流程共用的带阶段标识错误。"""


class SpecializationError(RuntimeError):
    def __init__(self, stage, message, *, published=False):
        self.published = published
        self.stage = stage
        self.message = message
        super().__init__(f'{stage}: {message}')


class ArtifactCollisionError(SpecializationError):
    """最终产物已存在，禁止回退 OPC 覆盖同名文件。"""
