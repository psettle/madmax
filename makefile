#config
DEBUG=1

#setup
SOURCES=
LIBS=
INCLUDE=

#header includes
INCLUDE += src
INCLUDE += src/util

#source includes
SOURCES += src/main.cpp

#final results
EXECUTABLE=out/madmax.exe
BUNDLE=out/madmax.cpp


# C/C++ toolchain setup
ifeq ($(DEBUG), 1)
	FLAG_BUILD_MODE=-O0 -ggdb3
else
	FLAG_BUILD_MODE=-O3
endif

LDFLAGS=-Wall $(FLAG_BUILD_MODE)
CC=g++
CFLAGS=-c -MMD -Wall $(FLAG_BUILD_MODE)

# Cg-Cpp-Bundler setup
PYTHON=python
BUNDLER=modules/cg-cpp-bundler/src/bundler.py

# Intermediate files/arguments
OBJECTS=$(SOURCES:%.cpp=out/%.o)								# C/C++ object files
INCLUDE_FORMATTED=$(addprefix -I, $(INCLUDE)) 	# C/C++ include args
BUNDLE_SOURCE=$(addprefix -i , $(SOURCES))			# Bundler include args
BUNDLE_INCLUDE=$(addprefix -p , $(INCLUDE))			# Bundler source args
DEPENDENCIES=$(OBJECTS:.o=.d)										# Header dependency listings

#targets
.PHONY: all
all: $(EXECUTABLE) $(BUNDLE)


$(EXECUTABLE): $(OBJECTS)
	@$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $@
	@echo $@

$(OBJECTS): out/%.o : %.cpp
	@mkdir -p out/$(dir $<)
	@$(CC) $(CFLAGS) $(INCLUDE_FORMATTED) $< -o $@
	@echo $<

$(BUNDLE): $(EXECUTABLE)
	@$(PYTHON) $(BUNDLER) -o $(BUNDLE) $(BUNDLE_SOURCE) $(BUNDLE_INCLUDE)
	@echo $@

.PHONY: clean
clean:
	@rm -rf out/*


-include $(DEPENDENCIES)
