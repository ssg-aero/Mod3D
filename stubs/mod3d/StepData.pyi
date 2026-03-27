"""
STEP data module
"""
from __future__ import annotations
__all__: list[str] = ['StepModel']
class StepModel:
    """
    Provides access to the entities and header of a loaded STEP file.
    
    Obtained from :meth:`mod3d.StepControl.Reader.step_model` or
    :meth:`mod3d.StepControl.Writer.model`.
    """
    def __repr__(self) -> str:
        ...
    def nb_entities(self) -> int:
        """
        Returns the total number of entities in the model.
        
        Returns
        -------
        int
        """
