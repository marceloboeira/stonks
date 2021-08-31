BREW ?= `which brew`
ARDUINO_CLI ?= `which arduino-cli`
TARGET_BOARD ?= arduino:avr:mega:cpu=atmega1280
TARGET_PORT ?= `arduino-cli board list | grep dev | grep -v Bluetooth | awk '{print $$1, $$8}'`

.PHONY: help
help: ## List the available commands
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

.PHONY: setup
setup: ## Install Arduino CLI
	@$(BREW) install arduino-cli

.PHONY: compile
compile: ## Compile to the TARGET_BOARD
	@$(ARDUINO_CLI) compile -b $(TARGET_BOARD) --verify

.PHONY: upload
upload: ## Upload code to the TARGET_BOARD and TARGET_PORT
	@AVAILABLE_PORT=$(TARGET_PORT);\
	TARGET_PORT=$${AVAILABLE_PORT:-FALSE};\
	if [ $$TARGET_PORT = FALSE ]; then\
		echo "Error: No target board found";\
		else\
		$(ARDUINO_CLI) upload -b $(TARGET_BOARD) -p $$TARGET_PORT ;\
		echo "Finished uploading" ;\
	fi
