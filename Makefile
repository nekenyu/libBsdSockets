all:
#
# Top Level Makefile
#

# Configuration
#
# TODO: Clean this

# Subdirectories to be made
#

SUBDIRS+= src
SUBDIRS+= examples
SUBDIRS+= test

all clean cleaner cleanest: $(SUBDIRS)
all: TARGET=all
clean: TARGET=clean
cleaner: TARGET=cleaner
cleanest: TARGET=cleanest

# Recursive Make 
#

.PHONY: subdirs $(SUBDIRS)
subdirs: $(SUBDIRS)

$(SUBDIRS):
	make -C $@ $(TARGET)

test: src

.PHONY: doxygen
doxygen: all
	doxygen Doxyfile

cleanest:
	-rm -rf doc/doxygen

runTest: all
	test/cppunitTest

build: all doxygen runTest
