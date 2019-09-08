LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := camera_sdm660_shim.cpp
LOCAL_SHARED_LIBRARIES := libui libutils
LOCAL_MODULE := libcamera_sdm660_shim
LOCAL_MODULE_TAGS := optional
LOCAL_PROPRIETARY_MODULE := true
include $(BUILD_SHARED_LIBRARY)
