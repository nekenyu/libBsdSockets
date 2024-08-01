#
# Top Level Makefile
#

# Default target
all:

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
	+make -C $@ $(TARGET)
	@echo Leaving directory \`$@\'


# Other tasks
#

.PHONY: docs
docs: all
	doxygen Doxyfile

.PHONY: cleanest
cleanest:
	-rm -rf doc_built

.PHONY: runTest
runTest: all
	build/test/cppunit-test.exe

.PHONY: world
world: all docs runTest
