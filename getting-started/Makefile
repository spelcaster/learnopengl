# Copyright 2016 <https://github.com/spelcaster>
# Created by: Hugo Augusto Freitas do Carmo
#
# Last modified at     | Modified by     | Description

# compiler flags
CFLAGS = -Wall -Wextra -pedantic

ifdef RELEASE
CFLAGS += -O2
else
CFLAGS += -g
endif

CFLAGS += -std=c++14

# define any directories containing header files other than /usr/include
INCLUDES  =

# define library paths in addition to /usr/lib
LDFLAGS =

# define any libraries to link
LDLIBS = -lGL -lGLEW -lglfw -lX11 -lXrandr -lXi -lpthread -lSOIL

TEST_LDLIBS += -lgmock -lgtest

# define the program source files
SOURCES = $(wildcard src/*.cpp)

MAIN_FILES = src/main.cpp
TEST_SOURCES  = $(filter-out $(MAIN_FILES),$(SOURCES))
TEST_SOURCES += $(wildcard tests/*.cpp)

# define the program object files
OBJECTS = $(addprefix obj/, $(notdir $(SOURCES:.cpp=.o)))

TEST_OBJECTS = $(addprefix obj/, $(notdir $(TEST_SOURCES:.cpp=.o)))

# define the executable
EXECUTABLE = build/game
TEST_EXECUTABLE = build/game_test

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $(OBJECTS) $(LDLIBS)

test: $(TEST_SOURCES) $(TEST_EXECUTABLE)

$(TEST_EXECUTABLE): $(TEST_OBJECTS)
	$(CXX) $(CFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $(TEST_OBJECTS) $(TEST_LDLIBS)

# this is a suffix replacement rule for buildings .o's from .cpp's
# it uses automatic variables:
# $< - the name of the prerequisite of the rule (a .cpp file)
# $@ - the name of the target of the rule (a .o file)
# (see the GNU make manual section about automatic variables)
obj/%.o:src/%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

obj/%.o:tests/%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

.PHONY: clean clean-test

clean:
	$(RM) -rv -- $(RESOURCES) $(OBJECTS) $(EXECUTABLE)

clean-test:
	$(RM) -rv -- $(RESOURCES) $(TEST_OBJECTS) $(TEST_EXECUTABLE)

