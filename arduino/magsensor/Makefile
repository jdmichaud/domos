ifeq ($(PLATFORMIO_UPLOAD_PORT),)
$(warning PLATFORMIO_UPLOAD_PORT is unset. Set it to the Pololu device, the lowest one)
endif

all:
	platformio run

load:
	platformio run -t upload

pololu:
	platformio run -e pololu -t upload
