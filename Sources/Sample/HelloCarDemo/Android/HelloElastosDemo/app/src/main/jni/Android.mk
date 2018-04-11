
LOCAL_PATH := $(call my-dir)

#Elastos.Runtime
include $(CLEAR_VARS)
LOCAL_MODULE    := Elastos.Runtime
LOCAL_SRC_FILES := $(LOCAL_PATH)/elastos/libs/libElastos.Runtime.so
include $(PREBUILT_SHARED_LIBRARY)

#Elastos.CoreLibrary
include $(CLEAR_VARS)
LOCAL_MODULE    := Elastos.CoreLibrary
LOCAL_SRC_FILES := $(LOCAL_PATH)/elastos/libs/libElastos.CoreLibrary.so
include $(PREBUILT_SHARED_LIBRARY)

#Elastos.HelloCarDemo
include $(CLEAR_VARS)
LOCAL_MODULE    := Elastos.HelloCarDemo
LOCAL_SRC_FILES := $(LOCAL_PATH)/Elastos.HelloCarDemo/libElastos.HelloCarDemo.so
include $(PREBUILT_SHARED_LIBRARY)

#Elastos.CoreLibrary depend libcrypto.so
include $(CLEAR_VARS)
LOCAL_MODULE    := crypto
LOCAL_SRC_FILES := $(LOCAL_PATH)/elastos/libs/libcrypto.so
include $(PREBUILT_SHARED_LIBRARY)

#Elastos.CoreLibrary depend libicuuc.cr.so
include $(CLEAR_VARS)
LOCAL_MODULE    := icuuc.cr
LOCAL_SRC_FILES := $(LOCAL_PATH)/elastos/libs/libicuuc.cr.so
include $(PREBUILT_SHARED_LIBRARY)

#Elastos.CoreLibrary depend libc++_shared.so
include $(CLEAR_VARS)
LOCAL_MODULE    := c++_shared
LOCAL_SRC_FILES := $(LOCAL_PATH)/elastos/libs/libc++_shared.so
include $(PREBUILT_SHARED_LIBRARY)

#Elastos.CoreLibrary depend libstlport.so
include $(CLEAR_VARS)
LOCAL_MODULE    := stlport
LOCAL_SRC_FILES := $(LOCAL_PATH)/elastos/libs/libstlport.so
include $(PREBUILT_SHARED_LIBRARY)

#Elastos.Runtime depend libelacarrier.so
include $(CLEAR_VARS)
LOCAL_MODULE    := elacarrier
LOCAL_SRC_FILES := $(LOCAL_PATH)/elastos/libs/libelacarrier.so
include $(PREBUILT_SHARED_LIBRARY)

#Elastos.Runtime depend libelacommon.so
include $(CLEAR_VARS)
LOCAL_MODULE    := elacommon
LOCAL_SRC_FILES := $(LOCAL_PATH)/elastos/libs/libelacommon.so
include $(PREBUILT_SHARED_LIBRARY)

#Elastos.Runtime depend libelasession.so
include $(CLEAR_VARS)
LOCAL_MODULE    := elasession
LOCAL_SRC_FILES := $(LOCAL_PATH)/elastos/libs/libelasession.so
include $(PREBUILT_SHARED_LIBRARY)

### shared library
include $(CLEAR_VARS)
LOCAL_SRC_FILES := elastos_testthirdlib.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/elastos/include \
      $(LOCAL_PATH)/elastos/include/elastos \
      $(LOCAL_PATH)/Elastos.HelloCarDemo

LOCAL_CFLAGS += -std=c++11 -D_GNUC -D_linux -D_UNDEFDLLEXP -fvisibility=hidden

LOCAL_MODULE := testThirdLib

#The elastos eco and lib.
LOCAL_LDFLAGS := $(LOCAL_PATH)/elastos/libs/libElastos.Runtime.so \
		$(LOCAL_PATH)/elastos/libs/libElastos.CoreLibrary.so \
		$(LOCAL_PATH)/Elastos.HelloCarDemo/libElastos.HelloCarDemo.so \
		$(LOCAL_PATH)/elastos/libs/Elastos.Runtime.lib

#LOCAL_LDFLAGS := $(LOCAL_PATH)/elastoslib/Elastos.Runtime.eco \
#		$(LOCAL_PATH)/elastoslib/Elastos.CoreLibrary.eco \
#		$(LOCAL_PATH)/elastoslib/Elastos.HelloCarDemo.eco \
#		$(LOCAL_PATH)/elastoslib/Elastos.Runtime.lib

include $(BUILD_SHARED_LIBRARY)
