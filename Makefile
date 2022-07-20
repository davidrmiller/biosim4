DEBUG = false

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

ifeq ($(DEBUG), true)
  CXXFLAGS += -g
  OUT_DIR = bin/Debug
else
  CXXCLAGS += -O3
  LDFLAGS += -O3 -s
  OUT_DIR = bin/Release
endif

SOURCE :=  $(wildcard src/*.cpp src/*.h)
CXXSOURCE :=  $(filter %.cpp, $(SOURCE))
HEADERS :=  $(filter %.h, $(SOURCE))

OBJS = $(patsubst %.cpp,%.o ,$(CXXSOURCE))

all: debug release

debug:
	$(MAKE) DEBUG=true binary
release:
	$(MAKE) DEBUG=false binary

$(OBJS): $(HEADERS)

clean:
	$(RM) $(OBJS)

distclean:
	$(RM) $(OUT_DIR)/biosim4
	$(RM) logs/* images/*

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $(filter %.cpp, $^) -o $@

binary: $(OUT_DIR)/biosim4
$(OUT_DIR)/biosim4: $(OBJS)
	$(LD) -o $@ $^ $(LDFLAGS)

.PHONY: all binary clean distclean
