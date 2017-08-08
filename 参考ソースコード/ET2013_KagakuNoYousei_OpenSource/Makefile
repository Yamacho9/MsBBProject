# nxtOSEKルートディレクトリ
NXTOSEK_ROOT = /cygdrive/c/cygwin/nxtOSEK

# toppers_osekディレクトリ  ※DOS形式
TOPPERS_OSEK_ROOT_SG = C:/cygwin/nxtOSEK/toppers_osek

# ターゲット実行形式ファイル名
TARGET = et2013

# インクルードパス
USER_INC_PATH= $(NXTOSEK_ROOT)/ecrobot/nxtway_gs_balancer

#ROMのみ作成
BUILD_MODE = ROM_ONLY

# ライブラリ
USER_LIB = nxtway_gs_balancer

# Cソースファイル
#   倒立制御する際には必要。
TARGET_SOURCES = \
	RunMethod/balancer_param.c \

# C++ソースファイル
#   改行する場合は、末尾に \ を付けること。
TARGET_CPP_SOURCES = \
	Competition.cpp \
	Condition/Condition.cpp \
	Condition/AllDistanceCondition.cpp \
	Course/RunBlock.cpp \
	Course/RunCourse.cpp \
	Course/Track.cpp \
	RunMethod/LineTraceMethod.cpp \
	RunMethod/RunMethod.cpp \
	RunMethod/Posture.cpp \
	RunMethod/Inclination.cpp \
	RunMethod/Vertical.cpp \
	RunMethod/PID.cpp \
	UI/UI.cpp \
	SubFunction/Calibration.cpp \
	SubFunction/Stop.cpp \
	SubFunction/Sound.cpp \
	Communication/GetLogData.cpp \
	Communication/RemoteStart.cpp \


# TOPPERS/ATK(OSEK)設定ファイル
TOPPERS_OSEK_OIL_SOURCE = Competition.oil

# 下記のマクロは変更しないでください
O_PATH ?= build

include $(NXTOSEK_ROOT)/ecrobot/ecrobot++.mak

#Copyright (c) 2014 Kagaku No Yousei. All Rights Reserved.

