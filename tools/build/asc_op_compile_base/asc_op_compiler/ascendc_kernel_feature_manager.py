#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2025 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and contiditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------
"""
ascendc kernel feature manager
"""

import threading
from abc import ABC, abstractmethod
from typing import Dict, Type, Any, Callable, Optional


class BaseFeature(ABC):
    """
    base feature class
    """
    def __init__(self, value: int):
        if not isinstance(value, int):
            raise TypeError(f"value's type must be INT, current tpye is {type(value).__name__}")
        self.value = value
    
    @property
    @abstractmethod
    def feature_name(self) -> str:
        pass

    @abstractmethod
    def get_feature_value(self) -> Any:
        pass
    
    def check_value_valid(self, data: Any) -> Any:
        pass

    def get_intersection_value(self, others_value: int) -> int:
        return min(self.value, others_value)


class AscendCKernelFeatureManager:
    """
    super kernel feature manager
    """
    _instance = None
    _lock = threading.Lock()

    def __new__(cls, *args, **kwargs):
        with cls._lock:
            if not cls._instance:
                cls._instance = super(AscendCKernelFeatureManager, cls).__new__(cls, *args, **kwargs)
        return cls._instance

    def __init__(self, use_ordered: bool = False):
        # store registered feature class: {feature_name, feature_class_instance}
        self._feature_instances: Dict[str, BaseFeature] = {}

    def register_feature(self, feature_cls: Type[BaseFeature]) -> None:
        """
        register feature class
        :param feature_cls: feature class base on BaseFeature
        :raises ValueError: if duplicated or unimplemented
        """
        if not issubclass(feature_cls, BaseFeature):
            raise ValueError(f"class {feature_cls.__name__} must be inherited from BaseFeature")

        instance = feature_cls()
        if instance.feature_name in self._feature_instances:
            raise ValueError(f"feature {instance.feature_name} already existed")
        self._feature_instances[instance.feature_name] = instance

    def unregister_feature(self, feature_name: str) -> None:
        if feature_name not in self._feature_instances:
            raise KeyError(f"feature {feature_name} not exists")
        
        instance = self._feature_instances.pop(feature_name)

    def get_feature(self, feature_name: str) -> BaseFeature:
        """
        get feature instance according to feature_name
        :param feature_name: feature_name
        :return feature instance
        """
        if feature_name not in self._feature_instances:
            raise KeyError(f"feature {feature_name} not exists")
        return self._feature_instances[feature_name]

    def get_feature_value(self, feature_name: str) -> Any:
        """
        get feature value according to feature name
        """
        feature = self.get_feature(feature_name)
        return feature.get_feature_value()

    def get_all_feature_value(self):
        """
        get all feature value
        """
        all_features = {}
        for feature_name in self._feature_instances.keys():
            all_features[feature_name] = self.get_feature_value(feature_name)
        return all_features

    def list_features(self):
        return list(self._feature_instances.keys())

    def check_feature_value_valid(self, feature_data):
        """
        check whether the input data is valid
        """
        for feature_name, value in feature_data.items():
            if feature_name not in self._feature_instances:
                continue
            feature = self._feature_instances[feature_name]
            feature.check_value_valid(value)

    def get_intersection_feature_value(self, feature_data):
        res = {}
        for feature_name, value in feature_data.items():
            if feature_name not in self._feature_instances:
                continue
            feature = self.get_feature(feature_name)
            if feature_name not in res:
                res[feature_name] = feature.get_intersection_value(value)
        return res


def register_feature(manager: AscendCKernelFeatureManager):
    """
    decorator: used to quickly register a feature class to a specified manager
    :param manager: feature manager instance
    example
    @register_feature(global_ascendc_kernel_feature_manager)
    class FeatureExample(BaseFeature):
        def __init__(self):
            super().__init__(1) # feature value

        @property
        def feature_name(self) -> str:
            return "test_feature_class"

        def get_feature_value(self) -> int:
            return self.value

        def check_value_valid(self, data: int):
            if data != 1:
                raise ValueError(f"feature value {data} is not supported")
    """
    def decorator(feature_cls: Type[BaseFeature]) -> Type[BaseFeature]:
        manager.register_feature(feature_cls)
        return feature_cls

    return decorator


global_ascendc_kernel_feature_manager = AscendCKernelFeatureManager()


def get_features():
    return global_ascendc_kernel_feature_manager.get_all_feature_value()


