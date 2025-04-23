.PHONY: init
init:
	@mkdir -p build

php:
	@find . -type f -path "./src/web/pages/*.php" | sort
	@find . -type f -path "./*.php" | sort | \
	  xargs -I {} sh -c '\
	    file="{}"; \
	    if [ "$${file##*.}" = "php" ]; then \
	      dir=$$(dirname "$$file"); \
	      html_file="$$dir/$$(basename "$${file%.php}.html")"; \
	      php "$$file" > "$$html_file"; \
	    fi'
	@find . -type f -path "./src/web/pages/*.php" -exec sh -c '\
	  for file; do \
	    if [ "$${file##*.}" = "php" ]; then \
	      dir=$$(dirname "$$file"); \
	      html_file="$$dir/$$(basename "$${file%.php}.html")"; \
	      php "$$file" > "$$html_file"; \
	    fi; \
	  done' sh {} +
	@find . -type f -name "*.html" | grep -v "node_modules" | sort

ts:
	@tsc --init
	@tsc --project tsconfig.json --outDir src/web/assets/js --sourceMap

sass:
	@sass src/web/assets/sass:src/web/assets/css --style compressed --update

.PHONY: clean
clean:
	@rm -rf .DS_Store
	@rm -rf src/.DS_Store
	@rm -rf src/web/assets/css
	@rm -rf src/web/assets/js
	@rm -rf tsconfig.json
	@rm -rf package-lock.json
	@rm -rf src/web/pages/*.html
	@rm -rf src/web/*.html
	@rm -rf build

.PHONY: build
build: clean init php ts sass
	@cd build && cmake .. && make all

.PHONY: run
run:
	@cd build && ./website

.PHONY: help
help:
	@echo "Makefile commands:"
	@echo "  init        - Create build directory"
	@echo "  php         - Generate HTML files from PHP files"
	@echo "  ts          - Initialize TypeScript configuration and compile"
	@echo "  sass        - Compile SASS files to CSS"
	@echo "  clean       - Remove generated files and directories"
	@echo "  build       - Build the project"
	@echo "  run         - Run the project"
	@echo "  help        - Show this help message"
