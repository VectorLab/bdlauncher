
CFLAGS+= -fPIC -s -O3 -std=gnu17
CXXFLAGS+= -fPIC -s -O3 -std=gnu++17
# -Wall -Werror
# -Wno-invalid-offsetof

# Macros

define compile
	@echo " CXX $(1)"
	@$(CXX) $(CXXFLAGS) -c -o $(1) $(2)
endef
define compilec
	@echo " CC  $(1)"
	@$(CC) $(CFLAGS) -c -o $(1) $(2)
endef
define link
	@echo " LD  $(1)"
	@$(CXX) $(LDFLAGS) $(2) -o $(1) $(3)
endef

# Phony Targets
.PHONY: all
all: preload genconfig

.PHONY: preload
preload: build/zydis/usr build/preload.so

.PHONY: clean
clean:
	@rm -rf build/*
	@touch build/.keep

.PHONY: genconfig
genconfig:
	@./script/genconfig.sh $(CC) $(CXX)

build/zydis/usr: lib/zydis/CMakeLists.txt
	@./script/cmake_zydis.sh $(CC) $(CXX)

# old .PHONY: clean
#	@rm -rf build/*.o
#	@rm -rf $(DLL_PRELOAD)

# raw target

_DLL_PRELOAD_SRC=$(wildcard preload/*.cpp)
_DLL_PRELOAD_OBJ=$(patsubst preload/%.cpp,build/%.o,$(_DLL_PRELOAD_SRC))
_DLL_PRELOAD_LIB=build/zydis/usr/lib/libZydis.a

build/preload.so: $(_DLL_PRELOAD_OBJ) $(_DLL_PRELOAD_LIB)
	$(call link,$@,-shared -fPIC -ldl,$^)

# Object Target

build/%.o: preload/%.cpp build/zydis/usr
	$(call compile,$@,$< -I include -I build/zydis/usr/include )

