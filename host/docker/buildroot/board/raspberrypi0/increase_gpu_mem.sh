# Increase the memory allocated to the VideoCore chip
sed -i s/gpu_mem_512=100/gpu_mem_512=256/ ${BINARIES_DIR}/rpi-firmware/config.txt
