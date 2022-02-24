ifneq ($(BUILD),debug)
    BUILD = release
endif

CXX = c++
LD = c++

CXXFLAGS += \
  -Wall \
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
  OBJ_DIR = obj/Debug/
  CXXFLAGS += -g
else
  OUT_DIR = bin/Release/
  OBJ_DIR = obj/Release/
  CXXCLAGS += -O3
  LDFLAGS += -O3 -s
endif

SOURCE :=  $(wildcard src/*.cpp src/*.h)
CXXSOURCE :=  $(filter %.cpp, $(SOURCE))
HEADERS :=  $(filter %.h, $(SOURCE))
OBJS := $(subst src/,$(OBJ_DIR)/, $(CXXSOURCE:.cpp=.o))


all: debug release

.PHONY : release debug    
debug:
	@$(MAKE) --no-print-directory bin/Debug/biosim4 BUILD=$@

release:
	@$(MAKE) --no-print-directory bin/Release/biosim4 BUILD=$@


$(OUT_DIR)biosim4: $(OBJS)
	$(LD) -o $@ $^ $(LDFLAGS)

$(OBJS): $(HEADERS)

$(OBJ_DIR)%.o : src%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean: clean_debug clean_release

clean_debug:
	$(RM) obj/Debug/*
	$(RM) bin/Debug/biosim4

clean_release:
	$(RM) obj/Release/*
	$(RM) bin/Release/biosim4

distclean: clean
	$(RM) logs/* images/*

.PHONY: all clean distclean

