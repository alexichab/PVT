CC = mpicc
CFLAGS = -Wall -Wextra -Werror
CXX = mpicxx
CXXFLAGS = -pedantic -std=c++17
CXXSRC = $(wildcard src/*.cpp)
CXXEXEC = $(patsubst src/%.cpp,bin/%,$(CXXSRC))
DIRGUARD = @mkdir -p $(@D)

.PHONY: all
all: $(CXXEXEC)

bin/%: src/%.cpp
	$(DIRGUARD)
	$(CXX) -o $@ $<

.PHONY: clean
.SILENT: clean
clean:
	rm -f $(CXXEXEC)

.PHONY: format
.SILENT: format
format:
	clang-format -i $(CXXSRC)
