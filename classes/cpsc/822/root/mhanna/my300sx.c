/*
template for 300SX driver
*/

#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/kernel_stat.h>
#include <asm/processor.h>
#include <linux/fs.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/major.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/malloc.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <asm/mman.h>
#include <asm/uaccess.h>

#include "my300sx.h"

void load_dac();
void set_vmode();
void load_graphics_defaults();
void clear_framebuffer();

int control_mapped;
int dma_mapped;

int glint_open(struct inode *inode, struct file *fp)
{

Use ioremap() to get kernel virtual addresses for control base, 
framebuffer base, and local buffer base from corresponding 
physical addresses.  Do a soft reset.

control_mapped = 0;
dma_mapped = 0;

return 0;
}


void glint_release(struct inode *inode, struct file *fp)
{
if(control_mapped) do_munmap( the mem-mapped control region );
if(dma_mapped){
	do_munmap(the mem-mapped dma buffer 0);
	do_munmap(the mem-mapped dma buffer 1);
	kfree(dma buffer 0 from kernel mem);
	kfree(dma buffer 1 from kernel mem);
	}
}

int glint_ioctl(struct inode *inode, struct file *fp, 
		unsigned int cmd, unsigned long arg) {

switch(cmd){
	
	case BIND_CONTROL:

		if(control_mapped) return 1;
		-switch fsuid to root
		-open /dev/mem
		-use control base as offset, GLINT_CONTROL_SIZE as size in
			call to do_mmap(); this returns a virtual base address
			suitable for user writes

		-close /dev/mem
		-switch fsuid back
		-use copy_to_user() to write the virtual base back to user
			through arg

		control_mapped = 1;
		return 0;

	case BIND_FRAMEB:
	case BIND_LOCALB:
		/* probably don't want to implement these two */
		break;

	case BIND_DMA:
		if(dma_mapped) return 1;

		- grab two large regions for dma buffers, memmap them, and 
		then hand the user both the virtual address and the physical 
                address on each; the user has to write to the virtual address
                to fill the buffer and has to load the physical address into 
                the DMAAddress register;  note that kmalloc internally tacks 
		on header space (24 bytes), so you'll need to round up to the
		nearest page boundary beyond what kmalloc returns

		/* reserve pages */
		firstpage = MAP_NR(kernel address of first page of buffer 0);
		lastpage = MAP_NR(kernel address of last page of buffer 0);
		for(page=firstpage;page<=lastpage;page++){
			mem_map[page].flags |= (unsigned int)(1<<31);
			}

		- do same for buffer 1

		-use virt_to_phys to generate physical base addresses of bufs

		- change fsuid to root
		- open /dev/mem
		- memmap dma buffers to get virtual addresses
		- use copy_to_user to write buffer addresses to user

		dma_mapped = 1;
		return 0;
		break;

	case VMODE:

		/* get desired video mode from user space and set it */
		vmode = (int)(arg);
		if(vmode==V_OFF){
			printk("got vmode OFF\n");
			/* 
			pass-through is 1 bit in polarity register;
			turn it off here
			*/
			polarity = EVAL_K_CONTROL(VTGPolarity);
			polarity &= 0xffdf;
			LOAD_K_CONTROL(VTGPolarity,polarity);
			return 0;
			}
		load_dac();
		set_vmode(vmode);
		load_graphics_defaults();
		slow_clear_framebuffer();
		polarity = EVAL_K_CONTROL(VTGPolarity);
		polarity |= 0x0020;
		LOAD_K_CONTROL(VTGPolarity,polarity);
		return 0;

	default:
	}
return 1;
}

struct file_operations glint_fops = {
	NULL,	/* lseek */
	NULL,	/* read */
	NULL,	/* write */
	NULL,	/* readdir */
	NULL,	/* poll */
	glint_ioctl,	/* ioctl */
	NULL,	/* mmap */
	glint_open,	/* open */
	NULL,	/* flush */
	glint_release,	/* release */
	NULL,	/* fsync */
	NULL,	/* fasync */
	NULL,	/* check_media_change */
	NULL,	/* revalidate */
	NULL	/* lock */
	};

int init_module(void)
{
struct pci_dev *pci_dev;
int reg_error;

if(reg_error=register_chrdev(GLINT_MAJOR,"glint",&glint_fops)){
	printk("GLINT: cannot register; received %d\n",reg_error);
	return 1;
	}

pci_dev=NULL;
pci_dev=pci_find_device(PCI_VENDOR_ID_3DLABS,PCI_DEVICE_ID_300SX,pci_dev);

glint_board.pci_bus = pci_dev->bus->number;
glint_board.pci_devfn = pci_dev->devfn;
glint_board.pci_vendor = pci_dev->vendor;
glint_board.pci_device = pci_dev->device;
glint_board.control_base= pci_dev->base_address[0];
glint_board.local_buffer_base= pci_dev->base_address[1];
glint_board.frame_buffer_base= pci_dev->base_address[2];

-use pcibios_write_config_word() to enable memory access and declare
 the board to be a dma bus master

return 0;
}


void cleanup_module(void)
{
unregister_chrdev(GLINT_MAJOR,"glint");
}

void load_dac()
{
unsigned int value;

/* LOAD_DAC() macro depends upon correct base address */
glint_board.k_dac_base = glint_board.k_control_base+(DAC_BYTE_OFFSET>>2);

printk("starting dac load\n");

LOAD_DAC(PelMask,0x00ffffff);

LOAD_DAC(IndexLow,IBM525_MISC_CTL_ONE); 
LOAD_DAC(IndexData,IBM525_MISR_CNTL_OFF|IBM525_VMSK_CNTL_OFF|IBM525_PADR_RFMT_READ_ADDR|IBM525_SENS_DSAB_DISABLE|IBM525_VRAM_SIZE_64);

LOAD_DAC(IndexLow,IBM525_MISC_CTL_TWO);
LOAD_DAC(IndexData,IBM525_PCLK_SEL_PLL|IBM525_INTL_MODE_DISABLE|IBM525_BLANK_CNTL_NORMAL|IBM525_COL_RES_8_BIT|IBM525_PORT_SEL_VRAM);

LOAD_DAC(IndexLow,IBM525_MISC_CTL_THREE);
LOAD_DAC(IndexData,IBM525_SWAP_RB_ENABLE|IBM525_SWAP_WORD_31_00_FIRST|IBM525_SWAP_NIB_07_04_FIRST); 

LOAD_DAC(IndexLow,IBM525_MISC_CLK_CTL);
LOAD_DAC(IndexData,IBM525_DDOTCLK_DISABLE|IBM525_SCLK_ENABLE|IBM525_PLL_ENABLE); 

LOAD_DAC(IndexLow,IBM525_SYNC_CTL);
LOAD_DAC(IndexData,IBM525_DLY_CNTL_ADD|IBM525_VSYN_INVT_DISABLE|IBM525_HSYN_INVT_DISABLE|IBM525_VSYN_CNTL_NORMAL|IBM525_HSYN_CNTL_NORMAL); 

LOAD_DAC(IndexLow,IBM525_HSYNC_CTL);
LOAD_DAC(IndexData,IBM525_HSYN_POS(0));

LOAD_DAC(IndexLow,IBM525_PWR_MGMT);
LOAD_DAC(IndexData,IBM525_SCLK_PWR_NORMAL|IBM525_DDOT_PWR_DISABLE|IBM525_SYNC_PWR_NORMAL|IBM525_ICLK_PWR_NORMAL|IBM525_DAC_PWR_NORMAL); 

LOAD_DAC(IndexLow,IBM525_DAC_OP);
LOAD_DAC(IndexData,IBM525_SOG_DISABLE|IBM525_BRB_NORMAL|IBM525_DSR_FAST|IBM525_DPE_ENABLE);

LOAD_DAC(IndexLow,IBM525_PALETTE_CTL);
LOAD_DAC(IndexData,IBM525_6BIT_LINEAR_ENABLE|IBM525_PALETTE_PARTITION(0));

LOAD_DAC(IndexLow,IBM525_PEL_FMT);
LOAD_DAC(IndexData,IBM525_PIXEL_FORMAT_32_BPP);

LOAD_DAC(IndexLow,IBM525_PEL_CTL_32BIT);
LOAD_DAC(IndexData,IBM525_B32_DCOL_DIRECT);

LOAD_DAC(IndexLow,IBM525_PLL_REF_DIV_COUNT);
LOAD_DAC(IndexData,IBM525_PLL_REFCLK_50_MHz);

LOAD_DAC(IndexLow,IBM525_PLL_CTL_ONE);
LOAD_DAC(IndexData,IBM525_REF_SRC_REFCLK|IBM525_PLL_INT_FS_DIRECT);

LOAD_DAC(IndexLow,IBM525_PLL_CTL_TWO);
LOAD_DAC(IndexData,IBM525_PLL_INT_FS(0));

LOAD_DAC(IndexLow,IBM525_CURSOR_CTL);
LOAD_DAC(IndexData,IBM525_CURSOR_MODE_OFF);
printk("ending dac load\n");
}

void set_vmode(int mode)
{
int width, height;

width = 0;
height = 0;
switch(mode){
	case V_OFF:
		return;
		
	case V_800x600_60Hz:
		printk("got vmode 800x600_60Hz\n");
		width = 800;
		height = 600;
		/* pel clock is in dac, but depends on res */
		LOAD_DAC(IndexLow,IBM525_F0);
		LOAD_DAC(IndexData,IBM525_DF(1)|IBM525_VCO_DIV_COUNT(15));
		
		-set 800x600 at 60Hz values for all these registers:
	        VTGHlimit
		VTGHSyncStart
		VTGHSyncEnd
		VTGHBlankEnd
		
		VTGVlimit
		VTGVSyncStart
		VTGVSyncEnd
		VTGVBlankEnd

		VTGHGateStart
		VTGHGateEnd
		VTGVGateStart
		VTGVGateEnd

		VTGSerialClk
		VTGFrameRowAddr

		/* load polarity with pass-through bit off */
		LOAD_K_CONTROL(VTGPolarity,POLARITY_800x600_60Hz_OFF);

		-initialize these registers:

	 	FBReadMode
		FBWriteMode
	 	LBReadMode
		LBWriteMode
		LBReadFormat
		LBWriteFormat
		break;
	default:
		break;
	}

FIFOSYNC;

-init these registers ... same for all video modes
ScreenSize
ScissorMaxXY
ScissorMinXY
Window
ScissorMode
FBSourceOffset
LBSourceOffset
PixelSize
FBPixelOffset
FBWindowBase
LBWindowBase
FBHardwareWriteMask
FBSoftwareWriteMask
FilterMode
StatisticMode
WindowOrigin
}

void load_graphics_defaults()
{
FIFOSYNC;
-load some more default values

ConstantColor
FogColor
FBBlockColor
AlphaTestMode
AntialiasMode
ColorDDAMode
DitherMode
DepthMode
FogMode
LogicalOpMode
RasterizerMode
StencilMode
AreaStippleMode
LineStippleMode
TextureColorMode
AlphaBlendMode
}

/* really slow clear */
void clear_framebuffer()
{
int i;
for(i=0;i<FB_SIZE_WORDS;i++){
	directly write word to fb
	}
}

