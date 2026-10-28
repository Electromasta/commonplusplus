APP = sandbox

BLD = _bld
TRIPLET = x64-linux
FEATURES = reflect;experimental

VCPKG_ROOT ?= $(patsubst %/,%,$(dir $(realpath $(shell command -v vcpkg))))
VCPKG_INC = $(BLD)/vcpkg_installed/$(TRIPLET)/include

COMPILER = g++
FLAGS = -std=c++23 -Wall -Wextra -Wfatal-errors -Wno-misleading-indentation -DGLM_FORCE_SWIZZLE -DGLM_ENABLE_EXPERIMENTAL

HEADERS = $(shell find src/common -name '*.h' | sort)
EXPERIMENTAL_HEADERS = $(shell find experimental -name '*.h' | sort)
EXPERIMENTAL_SOURCES = $(shell find experimental -name '*.cpp' | sort)

ifeq ($(OS), Windows_NT)
	CLEAN = del /Q
	EXT = .exe
else
	CLEAN = rm -rf
	EXT =
endif

.PHONY: build install run check experimental clean deep-clean

# CMake does the real build (vcpkg needs it); these targets are the front end.
# Release here means -O2 with asserts left on, same as the old Makefile.
build: $(BLD)/build.ninja
	@cmake --build $(BLD)
	@echo "Build complete: $(BLD)/$(APP)"

install: $(BLD)/build.ninja
	@echo "Install Complete"

$(BLD)/build.ninja: CMakeLists.txt vcpkg.json
	@echo "Configuring, first time might take a while"
	@cmake -S . -B $(BLD) -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS_RELEASE=-O2 \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
		-DCMAKE_TOOLCHAIN_FILE=$(VCPKG_ROOT)/scripts/buildsystems/vcpkg.cmake \
		-DVCPKG_MANIFEST_FEATURES="$(FEATURES)"
	@ln -sf $(BLD)/compile_commands.json compile_commands.json

run: build
	@./$(BLD)/$(APP)$(EXT)

# Every stable header must compile on its own
check: $(BLD)/build.ninja
	@for h in $(HEADERS); do \
		echo "#include \"$${h#src/}\"" | $(COMPILER) $(FLAGS) -fsyntax-only -I src -I $(VCPKG_INC) -x c++ - || exit 1; \
	done
	@echo "Check complete: every header compiles on its own"

# Parked code is never installed, but it still has to compile
experimental: $(BLD)/build.ninja
	@for h in $(EXPERIMENTAL_HEADERS); do \
		echo "#include \"$${h#experimental/}\"" | $(COMPILER) $(FLAGS) -fsyntax-only -I src -I experimental -I $(VCPKG_INC) -x c++ - || exit 1; \
	done
	@for f in $(EXPERIMENTAL_SOURCES); do \
		$(COMPILER) $(FLAGS) -fsyntax-only -I src -I experimental -I $(VCPKG_INC) $$f || exit 1; \
	done
	@echo "Experimental complete: everything compiles"

clean:
	@cmake --build $(BLD) --target clean
	@echo "Cleaned"

deep-clean:
	@$(CLEAN) $(BLD) compile_commands.json
	@echo "Cleaned"
