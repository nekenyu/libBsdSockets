all:

#
# test Makefile
#

TOPDIR= ../
-include $(TOPDIR)/Makefile.config.incl

TARGET= cppunitTest
all: $(TARGET)

SRC:= $(wildcard *.cpp)
OBJS= $(patsubst %.cpp,%.o,$(SRC))
DEPS= $(patsubst %.cpp,%.d,$(SRC))

%.o: %.cpp
	@$(MAKEDEPEND)
	$(CXX) $(CXXFLAGS) -c $<

-include $(DEPS)

clean:
	rm -f $(OBJS)

cleaner: clean
	rm -f $(DEPS)

cleanest: cleaner
	rm -f $(TARGET)

$(TARGET): $(OBJS) $(LIB_BSD_SOCKET)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIB_BSD_SOCKET_INCL) $(LIB_CPPUNIT_INCL)
