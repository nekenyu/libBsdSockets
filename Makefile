all:
#
# Top Level Makefile
#

# Subdirectories to be made and dependencies
#

SUBDIRS+= src
SUBDIRS+= examples
SUBDIRS+= test
test: src
examples: src

# Targets supported
#
all clean cleaner cleanest: $(SUBDIRS)
all: TARGET=all
clean: TARGET=clean
cleaner: TARGET=cleaner
cleanest: TARGET=cleanest

# Recursive make in the subdirectories
#
.PHONY: subdirs $(SUBDIRS)
subdirs: $(SUBDIRS)

$(SUBDIRS):
	@echo Entering directory \`$@\'
	make -C $@ $(TARGET)
	@echo Leaving directory \`$@\'


# Other tasks
#

.PHONY: doxygen
doxygen: all
	doxygen Doxyfile

cleanest:
	-rm -rf doc_built

runTest: all
	test/cppunitTest

build: all doxygen runTest
