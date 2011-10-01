#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fs.h>
#include <linux/hdreg.h>

struct drive_info {
	struct hd_geometry hdg;
	int block_size, dev_size;
	struct hd_driveid id;
	long nice;
};

int get_drive_info(const char *device, struct drive_info *di) {
	int fd;
	if((fd = open(device, O_RDONLY)) == -1) return -1;

	ioctl(fd, HDIO_GETGEO, &di->hdg);
	ioctl(fd, BLKSSZGET, &di->block_size);
	ioctl(fd, BLKGETSIZE, &di->dev_size);
	ioctl(fd, HDIO_GET_IDENTITY, &di->id);
	ioctl(fd, HDIO_GET_NICE, &di->nice);
}

unsigned int get_start_sect(const char *device) {
	struct hd_geometry hdg;
	int fd;
	if((fd = open(device, O_RDONLY)) == -1) return -1;

	ioctl(fd, HDIO_GETGEO, &hdg);
	return hdg.start;
}

int string_print(const char *s, int n) {
	int j;
	for(j = 0; j < n; j++) {
		if(!s[j]) break;
		putc(s[j], stdout);
	}
	return j;
}

int main() {
	struct drive_info di;
	get_drive_info("/dev/hda", &di);

	printf("Model: ");
	string_print(di.id.model, 40);
	printf("\nFirmware revision: ");
	string_print(di.id.fw_rev, 8);
	printf("\nSerial no.: ");
	string_print(di.id.serial_no, 20);
	printf("\nBlock size: %d\n", di.block_size);
	printf("Device size: %d\n", di.dev_size);
	printf("Physical cylinders: %d\n", di.id.cyls);
	printf("Physical heads: %d\n", di.id.heads);
	printf("Physical sectors: %d\n", di.id.sectors);
	printf("Cylinders: %d\n", di.hdg.cylinders);
	printf("Heads: %d\n", di.hdg.heads);
	printf("Sectors: %d\n", di.hdg.sectors);
	printf("Bytes per track: %d\n", di.id.track_bytes);
	printf("Bytes per sector: %d\n", di.id.sector_bytes);
	if(di.id.capability&(1<<0))
		printf("DMA level: %d\n", di.id.tDMA);
	else
		printf("PIO level: %d\n", di.id.tPIO);
	if(di.id.capability&(1<<1))
		printf("LBA size: %d\n", di.id.lba_capacity);
	printf("Multiple sector count: %d\n", di.id.multsect);
	printf("DWORD io %s implemented\n", di.id.dword_io?"":"not");
	printf("Nice: %ld\n", di.nice);
	printf("Starting sector: %d\n", di.hdg.start);

	printf("Starting sector of partition 1: %d\n",
		get_start_sect("/dev/hda1"));
	printf("Starting sector of partition 2: %d\n",
		get_start_sect("/dev/hda2"));

	return 0;
}
