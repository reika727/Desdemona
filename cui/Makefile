CXX      := g++
CXXFLAGS := -Wall -Wextra -MMD -MP -std=c++17
SRC      := othello_cui.cpp ../othello.cpp ../othello_algorithm.cpp
TARGET   := othello_cui.out

$(TARGET): $(SRC)
	$(CXX) $^ $(CXXFLAGS) -o $@

-include *.d

.PHONY: test
test: $(TARGET)
	./$<

.PHONY: clean
clean:
	$(RM) $(TARGET) *.d
