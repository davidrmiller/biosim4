ifneq ($(BUILD),debug)
    BUILD = release
endif

CXX = c++
LD = c++

CXXFLAGS += \
  -Wall \
  -pedantic \
  -std=c++17 \
  -fexceptions \
  -fopenmp \
  $(shell pkg-config --cflags opencv4)

LDFLAGS += \
  -lopencv_core \
  -lopencv_video \
  -lopencv_videoio \
  -lgomp \
  -lpthread \
  -fopenmp

ifeq ($(BUILD),debug)
  OUT_DIR = bin/Debug/
  OBJ_DIR = obj/Debug/src
  CXXFLAGS += -g
else
  OUT_DIR = bin/Release/
  OBJ_DIR = obj/Release/src
  CXXFLAGS += -O3
  LDFLAGS += -O3 -s
endif

SOURCE :=  $(wildcard src/*.cpp src/*.h)
CXXSOURCE :=  $(filter %.cpp, $(SOURCE))
HEADERS :=  $(filter %.h, $(SOURCE))
OBJS := $(subst src/,$(OBJ_DIR)/, $(CXXSOURCE:.cpp=.o))


all: debug release


before_debug:
	test -d bin/Debug || mkdir -p bin/Debug
	test -d obj/Debug/src || mkdir -p obj/Debug/src

before_release:
	test -d bin/Release || mkdir -p bin/Release
	test -d obj/Release/src || mkdir -p obj/Release/src

.PHONY : release debug    
debug: before_debug
	@$(MAKE) --no-print-directory bin/Debug/biosim4 BUILD=$@

release: before_release
	@$(MAKE) --no-print-directory bin/Release/biosim4 BUILD=$@


$(OUT_DIR)biosim4: $(OBJS)
	$(LD) -o $@ $^ $(LDFLAGS)

$(OBJS): $(HEADERS)

$(OBJ_DIR)%.o : src%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean: clean_debug clean_release

clean_debug:
	$(RM) -f obj/Debug/src/*
	$(RM) -f bin/Debug/biosim4

clean_release:
	$(RM) -f obj/Release/src/*
	$(RM) -f bin/Release/biosim4

distclean: clean
	$(RM) -f logs/* images/*

.PHONY: all clean distclean

