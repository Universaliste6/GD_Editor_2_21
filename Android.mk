LOCAL_PATH := $(call my-dir)


include $(CLEAR_VARS)
LOCAL_CPP_FEATURES := rtti
LOCAL_MODULE := dobby
LOCAL_SRC_FILES := libraries/libdobby.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include/libzip
include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_CPP_FEATURES := rtti
LOCAL_MODULE := hooking
LOCAL_SHARED_LIBRARIES := dobby
LOCAL_SRC_FILES := libraries/libhooking.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include/libzip
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
COCOS_PATH    := 	$(LOCAL_PATH)/include/cocos2dx/
LOCAL_CPP_FEATURES := rtti
LOCAL_MODULE := cocos2dcpp
LOCAL_SRC_FILES := libraries/libcocos2dcpp.so
LOCAL_EXPORT_C_INCLUDES := 	$(LOCAL_PATH)/include/cocos2dx/include \
							$(LOCAL_PATH)/include/GeometryDash \
							$(COCOS_PATH)/include \
							$(COCOS_PATH)/Includes \
							$(COCOS_PATH)/kazmath/include \
							$(COCOS_PATH)/platform/android \
							$(COCOS_PATH)/support \
							$(COCOS_PATH)/ \
							$(COCOS_PATH)/base_nodes \
							$(COCOS_PATH)/extensions/DS_Dictionary \
							$(COCOS_PATH)/platform \
							$(COCOS_PATH)/extensions \
							$(COCOS_PATH)/extensions\network \

include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_CFLAGS := -w
LOCAL_CPP_FEATURES := rtti
LOCAL_MODULE := game
LOCAL_DISABLE_FORMAT_STRING_CHECKS := true
LOCAL_SHARED_LIBRARIES := cocos2dcpp hooking
TARGET_ARCH_ABI := all
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/includes \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/hooks \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/KittyMemory \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/layers \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/networking \

LOCAL_SRC_FILES :=	src/main.cpp \
					src/hooks/Servers.cpp \
					src/hooks/Emojis.cpp \
					src/hooks/Hacks.cpp \
					src/hooks/AbbreviatedLabels.cpp \
					src/hooks/Options.cpp \
					src/hooks/MenuLayerExt.cpp \
					src/hooks/MultiplayerLayerExt.cpp \
					src/hooks/SwingIconFix.cpp \
					src/hooks/MoreSearchLayerExt.cpp \
					src/hooks/AdvancedLevelInfo.cpp \
					src/hooks/SpeedrunTimer.cpp \
					src/GDPSHelper.cpp \
					src/GDPSManager.cpp \
					src/FunctionHelper.cpp \
					src/hooks/EditorPauseLayerExt.cpp \
					src/hooks/EditLevelLayerExt.cpp \
					src/hooks/LevelEditorLayerExt.cpp \
					src/hooks/ShaderDebug.cpp \
					src/hooks/DPADControl.cpp \
					src/hooks/CollisionFix.cpp \
					src/hooks/ShaderFix.cpp \
					src/hooks/Keybinds.cpp \
					src/KittyMemory/KittyMemory.cpp \
					src/KittyMemory/KittyUtils.cpp \
					src/KittyMemory/MemoryBackup.cpp \
					src/KittyMemory/MemoryPatch.cpp \
					src/fmt/ostream.cc \
					src/fmt/posix.cc \
					src/fmt/printf.cc \
					src/fmt/format.cc \
					src/networking/NetworkingManager.cpp \
					src/networking/Utils.cpp \
					src/networking/Socket.cpp \
					src/networking/Connection.cpp \


LOCAL_LDLIBS := -llog \
				-frtti \
				-lEGL \
				-lGLESv2 \
				-lGLESv1_CM \
				-ldl \
				-landroid \
				

include $(BUILD_SHARED_LIBRARY)