.PHONY: init
init:
	@mkdir -p build

.PHONY: clean
clean:
	@rm -rf .DS_Store
	@rm -rf src/.DS_Store
	@rm -rf build

.PHONY: build
build: clean init
	@cd build && cmake .. && make all

.PHONY: run
run:
	@cd build && ./website

.PHONY: help
help:
	@echo "Makefile commands:"
	@echo "  init        - Create build directory"
	@echo "  clean       - Remove build directory"
	@echo "  build       - Build the project"
	@echo "  run         - Run the project"
	@echo "  help        - Show this help message"
