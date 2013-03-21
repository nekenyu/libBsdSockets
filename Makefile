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

runTest: all
	test/cppunitTest

