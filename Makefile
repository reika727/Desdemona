CXX      := g++
CXXFLAGS := -Wall -Wextra -MMD -MP -std=c++17
LDFLAGS  := -lglut -lGLU -lGL
SRC      := main.cpp othello.cpp
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
