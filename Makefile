# ###############################################################################
# ################### MOOSE Application Standard Makefile #######################
# ###############################################################################
# #
# # Optional Environment variables
# # MOOSE_DIR        - Root directory of the MOOSE project 
# # HERD_TRUNK_DIR   - Location of the HERD repository (or parent directory)
# # FRAMEWORK_DIR    - Location of the MOOSE framework
# #
# ###############################################################################
# MOOSE_DIR          ?= $(shell dirname `pwd`)/moose
# HERD_TRUNK_DIR     ?= $(shell dirname `pwd`)
# FRAMEWORK_DIR      ?= $(MOOSE_DIR)/framework
# ###############################################################################
# CURRENT_DIR        := $(shell pwd)
#
# # framework
# include $(FRAMEWORK_DIR)/build.mk
# include $(FRAMEWORK_DIR)/moose.mk
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

################################## MODULES ####################################
SOLID_MECHANICS   := yes
LINEAR_ELASTICITY := yes
CONTACT           := yes
HEAT_CONDUCTION   := yes
WATER_STEAM_EOS   := yes
MISC              := yes
# PHASE_FIELD       := yes
# TENSOR_MECHANICS  := yes
# CHEMICAL_REACTIONS := yes
include           $(MOOSE_DIR)/modules/modules.mk
###############################################################################
# dep apps
APPLICATION_DIR    := $(HERD_TRUNK_DIR)/bison
APPLICATION_NAME   := bison
include            $(FRAMEWORK_DIR)/app.mk

# APPLICATION_DIR    := $(HERD_TRUNK_DIR)/marmot
# APPLICATION_NAME   := marmot
# include            $(FRAMEWORK_DIR)/app.mk

APPLICATION_DIR    := $(CURRENT_DIR)
APPLICATION_NAME   := buck
APP_REV_HEADER     := $(CURRENT_DIR)/include/base/BuckRevision.h
BUILD_EXEC         := yes
DEP_APPS           := $(shell $(FRAMEWORK_DIR)/scripts/find_dep_apps.py $(APPLICATION_NAME))
include            $(FRAMEWORK_DIR)/app.mk

# APPLICATION_DIR    := $(CURRENT_DIR)
# APPLICATION_NAME   := buck
# #APP_REV_HEADER     := $(CURDIR)/include/base/BisonRevision.h
# BUILD_EXEC         := yes
# DEP_APPS           := $(shell $(FRAMEWORK_DIR)/scripts/find_dep_apps.py $(APPLICATION_NAME))
# include            $(FRAMEWORK_DIR)/app.mk
###############################################################################
# Additional special case targets should be added here
