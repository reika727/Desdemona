CXX      := g++
CXXFLAGS := -Wall -Wextra -std=c++17
LDFLAGS  := -lglut -lGLU -lGL
SRC      := main.cpp othello.cpp
TARGET   := desdemona.out

$(TARGET): $(SRC)
	$(CXX) $^ $(CXXFLAGS) $(LDFLAGS) -o $@

.PHONY: test
test: $(TARGET)
	./$<

.PHONY: clean
clean:
	$(RM) $(TARGET)
