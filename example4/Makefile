# Makefile to build class 'xfade~' for Pure Data.
# Needs Makefile.pdlibbuilder as helper makefile for platform-dependent build
# settings and rules.

# library name
lib.name = examples

# input source file (class name == source file basename)
class.sources = xfade~.c

# all extra files to be included in binary distribution of the library
datafiles =

# installation directory
PDLIBDIR = /home/schmidh/Documents/Pd/externals

# include Makefile.pdlibbuilder from submodule directory 'pd-lib-builder'
PDLIBBUILDER_DIR=../pd-lib-builder/
include $(PDLIBBUILDER_DIR)/Makefile.pdlibbuilder
