.SUFFIXES: .cpp .h .o

LQCD_ROOT_PATH = ../
BRIDGE_INSTALL_PATH = $(LQCD_ROOT_PATH)/bridge-2.0.2/build

SOURCE = ./src
BIN = ./bin

include $(BRIDGE_INSTALL_PATH)/make.inc

PROG_NAME = solve_correlator

PRE = 

TARGETS = $(addprefix $(BIN)/,$(PROG_NAME))
OBJS = $(addprefix $(SOURCE)/,$(PRE)) 

all: msg bin $(TARGETS)

bin:
	mkdir -p $(BIN)

$(TARGETS): $(BIN)/%: $(SOURCE)/%.o $(OBJS)
	$(CXX) $(CXXFLAGS) $< $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@

msg:
	@cat $(BRIDGE_INSTALL_PATH)/message.txt

clean:
	$(RM) $(SOURCE)/*.o

clean.all:
	$(RM) $(TARGETS)
	$(RM) $(SOURCE)/*.o

