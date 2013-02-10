# Variable default definitions. Override them by exporting them in your shell.
V8_HOME				?=

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    	:= v8-base-prebuilt
LOCAL_SRC_FILES		:= $(TARGET_ARCH_ABI)/libv8_base.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    	:= v8-snapshot-prebuilt
LOCAL_SRC_FILES		:= $(TARGET_ARCH_ABI)/libv8_snapshot.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    	:= jav8
LOCAL_SRC_FILES 	:= jav8.cpp Wrapper.cpp Utils.cpp

LOCAL_CPP_FEATURES 	:= rtti exceptions
LOCAL_CFLAGS += \
	-DENABLE_DEBUGGER_SUPPORT \
	-DV8_NATIVE_REGEXP \
	-DARM \
	-DV8_TARGET_ARCH_ARM \
	-DENABLE_LOGGING_AND_PROFILING

LOCAL_C_INCLUDES 		:= $(V8_HOME) $(V8_HOME)/include $(V8_HOME)/src
LOCAL_STATIC_LIBRARIES 	:= v8-base-prebuilt v8-snapshot-prebuilt
LOCAL_LDLIBS 			:= -llog

include $(BUILD_SHARED_LIBRARY)
