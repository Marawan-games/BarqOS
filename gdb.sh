# gdb :
qemu-system-x86_64 \
 -s -S \
 -no-reboot \
 -m 1024 \
 -enable-kvm \
 -bios /usr/share/OVMF/OVMF_CODE.fd \
 -drive format=raw,file=image.iso
 
 # slow mode