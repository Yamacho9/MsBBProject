APPL_COBJS += balancer.o balancer_param.o LineTrace.o LineTrace_param.o CalcDistanceAndDirection.o

APPL_CXXOBJS += Calibration.o judgeSection.o

SRCLANG := c++

ifdef CONFIG_EV3RT_APPLICATION

# Include libraries
include $(EV3RT_SDK_LIB_DIR)/libcpp-ev3/Makefile

endif
