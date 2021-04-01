PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

GTEST_ROOT=/opt/googletest
GTEST_INCLUDE_DIR = $(GTEST_ROOT)/googletest/include
HEADER_DIR = ../include
OBJS = mmoGTest.o
INCLUDE = -I$(GTEST_INCLUDE_DIR) -I$(HEADER_DIR)
LIBS = -lgtest -lgtest_main -lpthread
LIBDIRS = -L$(GTEST_ROOT)/build/lib

CPPFLAGS += -Wall -Werror -std=c++17 

ifeq ($(BUILD_MODE),debug)
 CFLAGS += -g
else ifeq ($(BUILD_MODE),run)
 CFLAGS += -O2
else ifeq ($(BUILD_MODE),profile)
 CFLAGS += -O2
else
 CFLAGS += -g
endif

all: mmoGTest
	echo 'Compilation complete. Success?'

mmoGTest:	$(OBJS)
	$(CXX) $(LIBDIRS) -o $@ $^ $(LIBS)
	mkdir -p ../build/default
	mv $@ ../build/default

%.o:	%.cpp $(HEADER_DIR)/memoryManager.h
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(INCLUDE) -o $@ $<

%.o:	$(PROJECT_ROOT)test/%.c
	echo "DOS"
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $(INCLUDE) -o $@ $<

clean:
	rm -fr mmoGTest $(OBJS)
	rm -fr ../build/default/mmoGTest
	
.PHONY: all clean

