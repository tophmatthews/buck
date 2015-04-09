###############################################################################
################### MOOSE Application Standard Makefile #######################
###############################################################################
#
# Optional Environment variables
# MOOSE_DIR        - Root directory of the MOOSE project 
#
###############################################################################
# Use the MOOSE submodule if it exists and MOOSE_DIR is not set
MOOSE_SUBMODULE    := $(CURDIR)/moose
ifneq ($(wildcard $(MOOSE_SUBMODULE)/framework/Makefile),)
  MOOSE_DIR        ?= $(MOOSE_SUBMODULE)
else
  MOOSE_DIR        ?= $(shell dirname `pwd`)/moose
endif

# framework
FRAMEWORK_DIR      := $(MOOSE_DIR)/framework
include $(FRAMEWORK_DIR)/build.mk
include $(FRAMEWORK_DIR)/moose.mk

############################## ELK MODULES ####################################
# THERMOCHIMICA     := no
# include thermochimica.mk
# ###############################################################################

# ################################## MODULES ####################################
# SOLID_MECHANICS   := yes
# LINEAR_ELASTICITY := no
# CONTACT           := no
# HEAT_CONDUCTION   := no
# WATER_STEAM_EOS   := no
# MISC              := no
# include           $(MOOSE_DIR)/modules/modules.mk
###############################################################################

APPLICATION_DIR    := $(CURDIR)
APPLICATION_NAME   := buck
APP_REV_HEADER     := $(CURDIR)/include/base/BuckRevision.h
BUILD_EXEC         := yes
DEP_APPS           := $(shell $(FRAMEWORK_DIR)/scripts/find_dep_apps.py $(APPLICATION_NAME))
include            $(FRAMEWORK_DIR)/app.mk

###############################################################################
# Additional special case targets should be added here
