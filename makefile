#config
DEBUG=0

#setup
SOURCES=
LIBS=
INCLUDE=

#header includes
INCLUDE += src
INCLUDE += src/util
INCLUDE += src/math
INCLUDE += src/monte
INCLUDE += src/engine
INCLUDE += src/engine/unit

#source includes
SOURCES += src/main.cpp

SOURCES += src/engine/engine_State.cpp
SOURCES += src/engine/engine_Player.cpp
SOURCES += src/engine/engine_Game.cpp
SOURCES += src/engine/unit/engine_UnitType.cpp
SOURCES += src/engine/unit/engine_Vehicle.cpp
SOURCES += src/monte/monte_Evaluate.cpp
SOURCES += src/monte/monte_Optimize.cpp
SOURCES += src/monte/monte_Generator.cpp

#final results
EXECUTABLE=out/madmax.exe
BUNDLE=out/madmax.cpp
MINI_BUNDLE=out/minimadmax.txt
EXE_BUNDLE=out/minimadmax.exe


# C/C++ toolchain setup
ifeq ($(DEBUG), 1)
	FLAG_BUILD_MODE=-O0 -ggdb3 -DOMIT_CG_OPTIMIZE_FLAGS
else
	FLAG_BUILD_MODE=-O3
endif

LDFLAGS=-Wall $(FLAG_BUILD_MODE)
CC=g++
CFLAGS=-c -MMD -Wall $(FLAG_BUILD_MODE)

# Cg-Cpp-Bundler setup
PYTHON=python
BUNDLER=modules/cg-cpp-bundler/src/bundler.py
MINIFIER=modules/cminify/minifier.py

# Intermediate files/arguments
OBJECTS=$(SOURCES:%.cpp=out/%.o)								# C/C++ object files
INCLUDE_FORMATTED=$(addprefix -I, $(INCLUDE)) 	# C/C++ include args
BUNDLE_SOURCE=$(addprefix -i , $(SOURCES))			# Bundler include args
BUNDLE_INCLUDE=$(addprefix -p , $(INCLUDE))			# Bundler source args
DEPENDENCIES=$(OBJECTS:.o=.d)										# Header dependency listings

#targets
.PHONY: all
all: $(EXECUTABLE) $(MINI_BUNDLE) $(EXE_BUNDLE)


$(EXECUTABLE): $(OBJECTS)
	@$(CC) $(LDFLAGS) $(OBJECTS) $(LIBS) -o $@
	@echo $@

$(OBJECTS): out/%.o : %.cpp
	@mkdir -p out/$(dir $<)
	@$(CC) $(CFLAGS) $(INCLUDE_FORMATTED) $< -o $@
	@echo $<

$(BUNDLE): $(EXECUTABLE)
	@$(PYTHON) $(BUNDLER) -o $(BUNDLE) $(BUNDLE_SOURCE) $(BUNDLE_INCLUDE) -ns
	@echo $@

$(MINI_BUNDLE): $(BUNDLE)
	@$(PYTHON) $(MINIFIER) $(BUNDLE) > $(MINI_BUNDLE)
	@echo $@

$(EXE_BUNDLE) : $(MINI_BUNDLE)
	@$(CC) $(LDFLAGS) -x c++ $(MINI_BUNDLE) -o $@
	@echo $@

.PHONY: clean
clean:
	@rm -rf out/*


-include $(DEPENDENCIES)
