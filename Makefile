# ##############################################################################
#  Makefile for the dmlist module
#
#     This Makefile is used to build the dmlist module library
#
#   DONT EDIT THIS FILE - it is automatically generated. 
#   Edit the scripts/Makefile-lib.in file instead.
#       
# ##############################################################################
ifeq ($(DMOD_DIR),)
    DMOD_DIR = _codeql_build_dir/_deps/dmod-src
endif

#
#  Name of the module
#
DMOD_LIB_NAME=libdmlist.a
DMOD_SOURCES=src/dmlist.c
DMOD_INC_DIRS = include\
		$(DMOD_DIR)/inc\
		_codeql_build_dir/_deps/dmod-build
DMOD_LIBS = dmod_inc
DMOD_GEN_HEADERS_IN = 
DMOD_DEFINITIONS = DMLIST_VERSION_STRING="== dmlist ver. 1.0 ==\n"

# -----------------------------------------------------------------------------
# 	Initialization of paths
# -----------------------------------------------------------------------------
include $(DMOD_DIR)/paths.mk

# -----------------------------------------------------------------------------
# 	Including the template for the library
# -----------------------------------------------------------------------------
DMOD_LIB_OBJS_DIR = $(DMOD_OBJS_DIR)/dmlist
include $(DMOD_SLIB_FILE_PATH)
