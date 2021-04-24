TARGET := run.exe

MAIN := main.cpp

HEADER := $(include/**.h)

REBUILDABLES = $(TARGET)

$(TARGET): $(MAIN) $(HEADER)
	g++ -o $@ $< -I ./include/ -std=c++11 -fopenmp

clean:
	rm $(REBUILDABLES)
