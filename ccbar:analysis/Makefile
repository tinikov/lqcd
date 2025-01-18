CXXFLAGS = -std=c++17 -O3

SOURCE = ./src
BIN = ./bin

PROG_NAME = \
mean \
jre \
cart2sphr \
trev2 \
effmass \
a1plus \
prev \
fks-ti \
v-ti \
fks-td \
v-td \

PRE = \
dataio.o \
misc.o

TARGETS = $(addprefix $(BIN)/,$(PROG_NAME))
OBJS = $(addprefix $(SOURCE)/,$(PRE))

all: bin $(TARGETS)

bin:
	mkdir -p $(BIN)

$(TARGETS): $(BIN)/%: $(SOURCE)/%.o $(OBJS)
	$(CXX) $(CXXFLAGS) $< $(OBJS) -o $@

clean:
	$(RM) $(SOURCE)/*.o

clean.all:
	$(RM) $(TARGETS)
	$(RM) $(SOURCE)/*.o
