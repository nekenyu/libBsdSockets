all:

#
# src Makefile
#

TOPDIR= ../
-include $(TOPDIR)/Makefile.config.incl

SRC:= $(wildcard *.cpp)
DEPS= $(patsubst %.cpp,%.d,$(SRC))
OBJS= $(patsubst %.cpp,%.o,$(SRC))
EXES= $(patsubst %.cpp,%.exe,$(SRC))

all: $(EXES)

%.o: %.cpp
	@$(MAKEDEPEND)
	$(CPP) $(CPPFLAGS) -c $<

%.exe: %.o $(LIB_BSD_SOCKET)
	@$(MAKEDEPEND)
	$(CPP) $(CPPFLAGS) -o $@ $^ $(LIB_BSD_SOCKET_INCL)

-include $(DEPS)

clean:
	rm -f $(OBJS)

cleaner: clean
	rm -f $(DEPS)

cleanest: cleaner
	rm -f $(EXES)
