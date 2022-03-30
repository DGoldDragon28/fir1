CXXFLAGS += -g -Wall -O2 -I..
OBJS = correlate.o main.o
EXE = wav-correlate


$(EXE) : $(OBJS)
	g++ $(CXXFLAGS) $(LDFLAGS) -o $@ $^

all : clean $(EXE)

clean: 
	-rm $(EXE) $(OBJS)

.PHONY: clean all

