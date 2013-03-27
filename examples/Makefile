all:

#
# src Makefile
#

TOPDIR= ../
-include $(TOPDIR)/Makefile.config.incl

SRC:= $(wildcard *.cpp)
DEPS= $(patsubst %.cpp,%.d,$(SRC))
EXES= $(patsubst %.cpp,%.exe,$(SRC))

all: $(EXES)

%.exe: %.cpp
	@$(MAKEDEPEND)
	$(CPP) $(CPPFLAGS) -o $@ $^ $(LIB_BSD_SOCKET_INCL)

-include $(DEPS)

clean:

cleaner: clean
	rm -f $(DEPS)

cleanest: cleaner
	rm -f $(EXES)
