"""Stage-labelled errors shared by the specialization pipeline."""
class SpecializationError(RuntimeError):
    def __init__(self, stage, message, *, published=False):
        self.published = published
        self.stage = stage
        self.message = message
        super().__init__(f'{stage}: {message}')
