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
  -I./src/include \
  $(shell pkg-config --cflags opencv4)

LDFLAGS += \
  -lopencv_core \
  -lopencv_video \
  -lopencv_videoio \
  -lgomp \
  -lpthread \
  -lsfml-graphics \
  -lsfml-window \
  -lsfml-system \
  -ltgui \
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

SOURCE :=  $(wildcard src/*.cpp src/*.h src/userio/*.cpp src/userio/*.h \
  src/userio/sfmlComponents/*.cpp src/userio/sfmlComponents/*.h \
  src/utils/*.cpp src/utils/*.h \
  src/ai/*.cpp src/ai/*.h \
  src/userio/sfmlComponents/flowComponents/*.cpp src/userio/sfmlComponents/flowComponents/*.h \
  src/userio/sfmlComponents/settingsComponents/*.cpp src/userio/sfmlComponents/settingsComponents/*.h \
  )
CXXSOURCE :=  $(filter %.cpp, $(SOURCE))
HEADERS :=  $(filter %.h, $(SOURCE))
OBJS := $(subst src/,$(OBJ_DIR)/, $(CXXSOURCE:.cpp=.o))
INCLUDES = -I./src/include
LIBS = -L/path/to/cereal/lib -lcereal


all: debug release


before_debug:
	test -d bin/Debug || mkdir -p bin/Debug
	test -d obj/Debug/src || mkdir -p obj/Debug/src
	test -d obj/Debug/src/userio || mkdir -p obj/Debug/src/userio
	test -d obj/Debug/src/userio/sfmlComponents || mkdir -p obj/Debug/src/userio/sfmlComponents
	test -d obj/Debug/src/userio/sfmlComponents/flowComponents || mkdir -p obj/Debug/src/userio/sfmlComponents/flowComponents
	test -d obj/Debug/src/userio/sfmlComponents/settingsComponents || mkdir -p obj/Debug/src/userio/sfmlComponents/settingsComponents
	test -d obj/Debug/src/utils || mkdir -p obj/Debug/src/utils
	test -d obj/Debug/src/ai || mkdir -p obj/Debug/src/ai

before_release:
	test -d bin/Release || mkdir -p bin/Release
	test -d obj/Release/src || mkdir -p obj/Release/src
	test -d obj/Release/src/userio || mkdir -p obj/Release/src/userio
	test -d obj/Release/src/userio/sfmlComponents || mkdir -p obj/Release/src/userio/sfmlComponents
	test -d obj/Release/src/userio/sfmlComponents/flowComponents || mkdir -p obj/Release/src/userio/sfmlComponents/flowComponents
	test -d obj/Release/src/userio/sfmlComponents/settingsComponents || mkdir -p obj/Release/src/userio/sfmlComponents/settingsComponents
	test -d obj/Release/src/utils || mkdir -p obj/Release/src/utils
	test -d obj/Release/src/ai || mkdir -p obj/Release/src/ai

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

