all: prepare
	npm run dist

prepare:
	npm install

install: all
	$(INSTALL) -D -m 0755 $(@D)/overscan $(TARGET_DIR)/var/www/

clean:
	rm -fr dist node_modules npm-debug.log
