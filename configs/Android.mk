LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := tiny_hw
LOCAL_VENDOR_MODULE := true
LOCAL_MODULE_OWNER := samsung
LOCAL_SRC_FILES := tiny_hw.xml
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_SUFFIX := .xml
LOCAL_MODULE_RELATIVE_PATH := sound
include $(BUILD_PREBUILT)

