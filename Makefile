# Self documenting makefile format based on the article by François Zaninotto
# https://marmelab.com/blog/2016/02/29/auto-documented-makefile.html

.DEFAULT_GOAL := help

.PHONY: compile test help clean

BUILDDIR = build

help: ## Print makefile and target info
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | \
	awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

compile: ## Compile all sources
	meson compile -C $(BUILDDIR)

test: ## Run tests
	meson test --wrap='valgrind --leak-check=full --error-exitcode=1' \
	--print-errorlogs -C $(BUILDDIR)

clean: ## Clean build directory
	meson compile --clean -C $(BUILDDIR)

