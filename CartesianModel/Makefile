CLANG_FORMAT = clang-format
QMAKE = qmake

QMAKE_FLAGS = CONFIG+=qtquickcompiler

BUILD_ROOT = build
BUILD_DIR = $(BUILD_ROOT)/host
XBUILD_DIR = $(BUILD_ROOT)/am335x-custom
XBUILD_MAKEFLAGS = -j4

.PHONY: help
help:
	@echo 'Make targets:'
	@echo '  build      - build program for host'
	@echo '  xbuild     - build program for target device'
	@echo '  tests      - build and run automated tests'
	@echo '  clean      - clean generated files'
	@echo '  format     - fix up styling of source files'
	@echo
	@echo 'Make variables:'
	@echo '  BUILD_DIR=$(BUILD_DIR)'
	@echo '  QMAKE_FLAGS=$(QMAKE_FLAGS)'

.PHONY: all
all: build

.PHONY: build
build:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && $(QMAKE) $(QMAKE_FLAGS) $(CURDIR)
	$(MAKE) -C $(BUILD_DIR)

.PHONY: xbuild
xbuild:
	$(MAKE) -C ../ITZ3000Linux/docker-toolchain run \
		COMMAND='make -C $(CURDIR) build BUILD_DIR=$(XBUILD_DIR) $(XBUILD_MAKEFLAGS) \
			&& arm-linux-gnueabihf-strip $(CURDIR)/$(XBUILD_DIR)/bin/itz3000-gui'

.PHONY: tests
tests: override QMAKE_FLAGS += BUILD_TESTS=1
tests: build
	$(BUILD_DIR)/tests/itz3000-test

.PHONY: clean
clean:
	$(RM) -R $(BUILD_ROOT)

.PHONY: format
format:
	$(CLANG_FORMAT) -i src/*.cpp src/*.h tests/*.cpp tests/*.h
