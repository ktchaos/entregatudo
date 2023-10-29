CXX = g++
SRCDIR = .

# Lista de arquivos fonte
SOURCES = $(wildcard $(SRCDIR)/*.cpp)

# Nome do executável
TARGET = main

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) -o $@ $^

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all run clean
