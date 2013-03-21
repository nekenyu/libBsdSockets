all:

#
# src Makefile
#

TOPDIR= ../
-include $(TOPDIR)/Makefile.config.incl

TARGET= ../lib/libBsdSocket.a
all: $(TARGET)

SRC:= $(wildcard *.cpp)
OBJS= $(patsubst %.cpp,%.o,$(SRC))
DEPS= $(patsubst %.cpp,%.d,$(SRC))

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

$(TARGET): $(TARGET)($(OBJS))
