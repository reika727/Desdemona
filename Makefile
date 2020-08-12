CXX      := g++
CXXFLAGS := -Wall -Wextra -MMD -MP -std=c++17
LDFLAGS  := -pthread -lglut -lGLU -lGL
SRC      := main.cpp othello.cpp othello_algorithm.cpp
TARGET   := desdemona.out

$(TARGET): $(SRC)
	$(CXX) $^ $(CXXFLAGS) $(LDFLAGS) -o $@

-include *.d

.PHONY: test
test: $(TARGET)
	./$<

.PHONY: clean
clean:
	$(RM) $(TARGET) *.d
