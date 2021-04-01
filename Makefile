PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

.PHONY: all test

all: test

test:
	echo BUILD_MODE: $(BUILD_MODE)
	$(MAKE) -C $(PROJECT_ROOT)$@

clean:
	echo About to clean
	$(MAKE) clean -C $(PROJECT_ROOT)test