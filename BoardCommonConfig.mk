#
# Copyright (C) 2013 The CyanogenMod Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# This variable is set first, so it can be overridden
# by BoardConfigVendor.mk

LOCAL_PATH := device/samsung/t0lte

TARGET_SPECIFIC_HEADER_PATH += $(LOCAL_PATH)/include

# Bluetooth
BOARD_BLUETOOTH_BDROID_BUILDCFG_INCLUDE_DIR := $(LOCAL_PATH)/bluetooth
BOARD_CUSTOM_BT_CONFIG := $(LOCAL_PATH)/bluetooth/vnd_t0lte.txt

# Camera
BOARD_GLOBAL_CFLAGS += -DCAMERA_WITH_CITYID_PARAM

TARGET_REQUIRES_SYNCHRONOUS_SETSURFACE := true

# Text Relocations
TARGET_NEEDS_PLATFORM_TEXT_RELOCATIONS := true

# Kernel
TARGET_KERNEL_SOURCE := kernel/samsung/smdk4412
ifeq ($(TARGET_VOICE_TECH), cdma)
TARGET_KERNEL_CONFIG := lineageos_t0ltecdma_defconfig
else
TARGET_KERNEL_CONFIG := lineageos_t0lte_defconfig
endif

# Init
ifneq ($(WITH_TWRP), true)
TARGET_INIT_VENDOR_LIB := libinit_t0lte
endif

# Cache
BOARD_CACHEIMAGE_PARTITION_SIZE :=104857600
BOARD_CACHEIMAGE_FILE_SYSTEM_TYPE := ext4

# Recovery
TARGET_RECOVERY_FSTAB := device/samsung/t0lte/rootdir/fstab.smdk4x12
TARGET_RECOVERY_DENSITY := mdpi
RECOVERY_FSTAB_VERSION := 2

TARGET_USERIMAGES_USE_F2FS := true

# Keylayouts and input devices
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/usr/keylayout/sec_touchkey.kl:vendor/usr/keylayout/sec_touchkey.kl \
    $(LOCAL_PATH)/usr/keylayout/sec_e-pen.kl:vendor/usr/keylayout/sec_e-pen.kl \
    $(LOCAL_PATH)/usr/idc/sec_e-pen.idc:vendor/usr/idc/sec_e-pen.idc

# RIL
BOARD_PROVIDES_LIBRIL := true
BOARD_MODEM_TYPE := mdm9x35

# assert
TARGET_OTA_ASSERT_DEVICE := t0lte,t0ltexx,GT-N7105,t0ltedv,GT-N7105T,t0lteatt,SGH-I317,t0ltetmo,SGH-T889,t0ltecan,t0ltevl,SGH-I317M

# Selinux
BOARD_VENDOR_SEPOLICY_DIRS += device/samsung/t0lte/selinux/vendor
SYSTEM_EXT_PUBLIC_SEPOLICY_DIRS += device/samsung/t0lte/selinux/public
SYSTEM_EXT_PRIVATE_SEPOLICY_DIRS += device/samsung/t0lte/selinux/private
