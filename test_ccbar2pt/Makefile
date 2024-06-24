.SUFFIXES: .cpp .h .o

LQCD_ROOT_PATH = ../
BRIDGE_INSTALL_PATH = $(LQCD_ROOT_PATH)/bridge-2.0.2/build

include $(BRIDGE_INSTALL_PATH)/make.inc

headers =
sources = sample_Spectrum.cpp
objects = $(sources:%.cpp=%.o)

program = sample_Spectrum

all: $(program)

$(program): $(objects)
	$(LD) $(LDFLAGS) -o $@ $(objects) $(LDLIBS)

%.o: %.cpp $(headers)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

msg:
	@cat $(BRIDGE_INSTALL_PATH)/message.txt

clean:
	rm -f $(program) $(objects)

