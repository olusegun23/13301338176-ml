
lxc-ls --fancy
 1811  df -k
 1812  cd sudo cd /var/lib/lxc


sudo lxc-stop -n  p3-armjf
 1853  EXTRA_OPTS="-L /usr/arm-linux-gnueabi"  



qemu-img create -f raw armdisk.img 4G

http://ports.ubuntu.com/ubuntu-ports/dists/trusty-updates/main/installer-armhf/current/images/generic/netboot/

./qemu-system-arm -m 1024M -sd armdisk.img -M vexpress-a9 -cpu cortex-a9 -kernel vmlinuz-generic-ubuntu -initrd initrd-generic-ubuntu.gz 




./qemu-system-arm  -cpu cortex-a15 -machine type=virt  -m 2048 -kernel zImage -append 'console=ttyAMA0 root=/dev/vda rw'  -serial mon:stdio -display none -drive index=0,id=rootfs,file=arm-trusty.img -device virtio-blk-device,drive=rootfs -netdev user,id=mynet -device virtio-net-device,netdev=mynet



./qemu-system-arm -m 1024M -machine type=virt  -kernel vmlinuz-generic -append 'console=ttyAMA0 root=/dev/vda rw'  -serial mon:stdio -display none -drive index=0,id=rootfs,file=arm-trusty.img -device virtio-blk-device,drive=rootfs -netdev user,id=mynet -device virtio-net-device,netdev=mynet

fdisk -l  armdisk.img

mount -o loop,offset=1048576    armdisk.img   /mnt/arm
 1991  ls -ltr /mnt/arm
 1992  cp  /mnt/arm/initrd.img-3.2.0-4-vexpress   initrd.img-3.2.0-4-vexpress-qmeu
 1993  umount /mnt/arm




qemu-system-arm -m 1024M -sd armdisk.img -M vexpress-a9 -cpu cortex-a9  -kernel vmlinuz-3.2.0-4-vexpress-qmeu -initrd initrd.img-3.2.0-4-vexpress-qmeu  -append "root=/dev/mmcblk0p2"  -no-reboot
