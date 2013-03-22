all:

#
# src Makefile
#

TOPDIR= ../
-include $(TOPDIR)/Makefile.config.incl

SRC:= $(wildcard *.cpp)
OBJS= $(patsubst %.cpp,%.o,$(SRC))
DEPS= $(patsubst %.cpp,%.d,$(SRC))

all: $(OBJS)

%.o: %.cpp
	@$(MAKEDEPEND)
	$(CPP) $(CPPFLAGS) -c $<

-include $(DEPS)

clean:
	rm -f $(OBJS)

cleaner: clean
	rm -f $(DEPS)

cleanest: cleaner
	rm -f $(TARGET)
