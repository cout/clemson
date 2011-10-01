/**************************************************************/
/*              Version:  1				      */
/*		Team   :  1				      */
/**************************************************************/


/*
300SX device driver
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
void slow_clear_framebuffer();
unsigned long ido_mmap(struct file * file, unsigned long addr, 
			unsigned long len, unsigned long prot, 
			unsigned long flags, unsigned long off);

int control_mapped;
int dma_mapped;


int glint_open(struct inode *inode, struct file *fp)
{
  /*  ioremap the base addresses of control registers, local buffer
      and Frame buffers
  */
  glint_board.k_control_base = ( unsigned long *) ioremap ( 
		glint_board.control_base , GLINT_CONTROL_SIZE) ;
  glint_board.k_local_buffer_base = ( unsigned long *) ioremap ( 
		glint_board.local_buffer_base , LOCAL_BUFFER_SIZE) ;
  glint_board.k_frame_buffer_base = ( unsigned long *) ioremap ( 
		glint_board.frame_buffer_base , FRAME_BUFFER_SIZE) ;

  printk ( "V Control base = 0x%x\n" , glint_board.k_control_base ) ;
  printk ( "V Local base = 0x%x\n" , glint_board.k_local_buffer_base ) ;
  printk ( "V Frame base = 0x%x\n" , glint_board.k_frame_buffer_base ) ;
  printk ( "FIFO SpaceReg = 0x%x\n" , *(glint_board.k_control_base + (0x18 >> 2)) ) ;

  control_mapped = 0;
  dma_mapped = 0;

  /* soft reset  */
  LOAD_K_CONTROL ( ResetStatus , 0x80000000 ) ; 
  while( EVAL_K_CONTROL(ResetStatus) != 0);
  return 0;
}


int glint_release(struct inode *inode, struct file *fp)
{
  int i ;
  if(control_mapped) 
    do_munmap(  glint_board.u_control_base, GLINT_CONTROL_SIZE ) ;

  if(dma_mapped)
  {
    for ( i = 0 ; i < NUM_OF_BUFS ; i ++ )
    {
	do_munmap( (unsigned long ) glint_board.dmagob[i].virt_base , MAX_DMA );

	/* freeing the kernel memory */
	kfree ( glint_board.dmagob[i].kbase ) ;
    }
  }
}


int glint_ioctl(struct inode *inode, struct file *fp, 
		unsigned int cmd, unsigned long arg) 
{
  int vmode , i ;
  int firstpage, lastpage, page ;
  unsigned long polarity , retVal, ret, restore;
  unsigned long kbase0, kbase1, pbase0, pbase1 ;
  int oldfsuid ;
  struct file *fmem , *temp;

  switch ( cmd ) 
  {
    case BIND_CONTROL:
		printk ( "In BIND_CTRL\n" ) ;
		if(control_mapped) return 1;
		restore = current->fsuid;
		current->fsuid = 0;
		cap_raise(current->cap_effective, CAP_SYS_RAWIO);
		temp = filp_open("/dev/mem", O_RDWR, 0600);
	
		/* map the physical address of control base register
		   to user memory space
  		*/
		ret = do_mmap(temp, 0, GLINT_CONTROL_SIZE, PROT_READ | PROT_WRITE , MAP_SHARED, glint_board.control_base);
		filp_close(temp, NULL);
		cap_lower(current->cap_effective, CAP_SYS_RAWIO);
		current->fsuid = restore;
		
		copy_to_user((char *)arg, &ret, 4);
		glint_board.u_control_base = ret;
		control_mapped = 1;
		return 0;

	case BIND_DMA:

		if(dma_mapped) return 1;

		for ( i = 0 ; i < NUM_OF_BUFS ; i++ )
		{
		  glint_board.dmagob[i].kbase = 
			kmalloc ( THE_BIG_BUCKET, GFP_KERNEL);

		  if ( !glint_board.dmagob[i].kbase ) 
		    printk ( "++Error in kmalloc[%d]\n" , i  ) ;

		  /* reserve pages for buffer 0 */
		  firstpage = MAP_NR ( glint_board.dmagob[i].kbase );
		  lastpage = MAP_NR ( glint_board.dmagob[i].kbase + 
						MAX_DMA / 4 - 1 );

		  for(page=firstpage;page<=lastpage;page++)
		    mem_map[page].flags |= (unsigned long)(1<<31);

		  /* Get the physical base address for the dma buffers */
	   	  glint_board.dmagob[i].phys_base = 
		    (unsigned long)virt_to_phys(glint_board.dmagob[i].kbase);	
	
		  /* change fsuid to root */
		  oldfsuid = current->fsuid ;
		  current->fsuid = 0 ;
		  cap_raise ( current->cap_effective  , CAP_SYS_RAWIO ) ;	
	
		  /* open /dev/mem */
		  fmem = filp_open ( "/dev/mem" , O_RDWR , 0600 ) ;  	
		  if ( !fmem ) 
		    printk ( "++Error while opening dev/mem\n" ) ;
	
		  /* memmap dma buffers to get virtual addresses */

		  glint_board.dmagob[i].virt_base = (unsigned long *)do_mmap (
				fmem , 0  , 
				MAX_DMA , 
				PROT_WRITE|PROT_READ|PROT_EXEC , MAP_SHARED, 
				glint_board.dmagob[i].phys_base ) ; 	

		  if ( (long) glint_board.dmagob[i].virt_base < 0 )
		    printk ( "ERR OR RRRR\n" ) ;

		  filp_close ( fmem , NULL ) ;
		  cap_lower ( current->cap_effective  , CAP_SYS_RAWIO ) ;	
		  current->fsuid = oldfsuid ;
	
		}

		/* use copy_to_user to write buffer addresses to user */
		for ( i = 0 ; i < NUM_OF_BUFS ; i++ )
		{
			copy_to_user (& (((struct dmagob *) (arg)+i)->phys_base) , &glint_board.dmagob[i].phys_base , 4 ) ;
			copy_to_user (& ((unsigned long *) ((struct dmagob *) (arg)+i)->virt_base) , &glint_board.dmagob[i].virt_base , 4 ) ;

		}

		dma_mapped = 1;
		return 0;
		break;

	case VMODE :

		/* get desired video mode from user space and set it */
		vmode = (int)(arg);
		if(vmode==V_OFF)
		{
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

	case VERSION:
		*(unsigned int *) arg = 1;
		return 0;

	}
}

struct file_operations glint_fops = {
	NULL,	/* lseek */
	NULL,	/* read */
	NULL,	/* write */
	NULL,	/* readdir */
	NULL,	/* poll */
	glint_ioctl , /* 	ioctl */
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
	unsigned short val ;

	if ( reg_error=register_chrdev ( GLINT_MAJOR ,
					 "glint" , &glint_fops )
	   )
	{
		printk ( "GLINT: cannot register; received %d\n" , reg_error ) ;
		return 1 ;
	}

	pci_dev=NULL;
	pci_dev=pci_find_device ( PCI_VENDOR_ID_3DLABS ,
					PCI_DEVICE_ID_300SX , 
					pci_dev);
	if ( !pci_dev )
		printk ( "Find device failed\n" ) ;
	glint_board.pci_bus = pci_dev->bus->number;
	glint_board.pci_devfn = pci_dev->devfn;
	glint_board.pci_vendor = pci_dev->vendor;
	glint_board.pci_device = pci_dev->device;
	glint_board.control_base= pci_dev->base_address[0];
	glint_board.local_buffer_base= pci_dev->base_address[1];
	glint_board.frame_buffer_base= pci_dev->base_address[2];

	printk ( "Control base = %x\n" , glint_board.control_base ) ;
	printk ( "Local base = %x\n" , glint_board.local_buffer_base ) ;
	printk ( "Frame base = %x\n" , glint_board.frame_buffer_base ) ;

	/* -use pcibios_write_config_word() to enable memory access and declare
 		the board to be a dma bus master
	*/

	pcibios_read_config_word ( glint_board.pci_bus , 
					glint_board.pci_devfn , 
			      		PCI_COMMAND , 
					&val ) ;

	pcibios_write_config_word ( glint_board.pci_bus , 
				glint_board.pci_devfn,
			       	PCI_COMMAND ,
				val | PCI_COMMAND_MEMORY | PCI_COMMAND_MASTER
 				   );
	return 0;
}


void cleanup_module(void)
{
unregister_chrdev(GLINT_MAJOR,"glint");
}

void load_dac()
{
	/* unsigned int value; */ 

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
		/* pel clock is in dac, but depends on res  */
		LOAD_DAC(IndexLow,IBM525_F0); 
		LOAD_DAC(IndexData,IBM525_DF(1)|IBM525_VCO_DIV_COUNT(15)); 
		
		/*	-set 800x600 at 60Hz values for all these registers: */
		LOAD_K_CONTROL(VTGHlimit, 0x210 ); 
  		LOAD_K_CONTROL(VTGHSyncStart , 0x14 );
  		LOAD_K_CONTROL(VTGHSyncEnd , 0x54);
  		LOAD_K_CONTROL(VTGHBlankEnd , 0x80);
		
  		LOAD_K_CONTROL(VTGVlimit, 0x274); 
  		LOAD_K_CONTROL(VTGVSyncStart, 0x2); 
  		LOAD_K_CONTROL(VTGVSyncEnd , 0x6);
  		LOAD_K_CONTROL(VTGVBlankEnd , 0x1c);

  		LOAD_K_CONTROL(VTGHGateStart , 0x7e);
  		LOAD_K_CONTROL(VTGHGateEnd , 0x20e);
  		LOAD_K_CONTROL(VTGVGateStart , 0x1b);
  		LOAD_K_CONTROL(VTGVGateEnd , 0x1c);

  		LOAD_K_CONTROL(VTGSerialClk, SERIAL_CLOCK_SPEC  );

  		LOAD_K_CONTROL(VTGFrameRowAddr,0) ;

/*  		load polarity with pass-through bit off  */
  		LOAD_K_CONTROL(VTGPolarity, /* 0xb0 */ POLARITY_800x600_60Hz_OFF  ); 

/*  		initialize these registers */

  		LOAD_K_CONTROL(FBReadMode ,FBREADMODE_W800_OTL);
  		LOAD_K_CONTROL(FBWriteMode, FBWRITEMODE_ENABLE);
  		LOAD_K_CONTROL(LBReadMode ,LBREADMODE_W800_OTL);
  		LOAD_K_CONTROL(LBWriteMode, LBWRITEMODE_ENABLE);
  		LOAD_K_CONTROL(LBReadFormat, 
			       LBFORMAT_D24_SW0_SP24_FCW8_FCP24_GIDW0_GIDP32); 
  		LOAD_K_CONTROL(LBWriteFormat, 
			       LBFORMAT_D24_SW0_SP24_FCW8_FCP24_GIDW0_GIDP32); 
		break; 
	default: 
		break; 
  	} 

  FIFOSYNC; 

/*  -init these registers ... same for all video modes */
  LOAD_K_CONTROL(ScreenSize, 0x02580320); 
  LOAD_K_CONTROL(ScissorMaxXY , 0x0 ); 
  LOAD_K_CONTROL(ScissorMinXY , 0x00); 
  LOAD_K_CONTROL(Window , 0x0 );  
  LOAD_K_CONTROL(ScissorMode , 0x02 ); 
  LOAD_K_CONTROL(FBSourceOffset , 0x0);  
  LOAD_K_CONTROL(LBSourceOffset , 0x0);

  LOAD_K_CONTROL(PixelSize,  0x0 ); 
  LOAD_K_CONTROL(FBPixelOffset,0x0); 
  LOAD_K_CONTROL(FBWindowBase ,0x0); 
  LOAD_K_CONTROL(LBWindowBase ,0x0 );
  LOAD_K_CONTROL(FBHardwareWriteMask ,  0xffffffff );
  LOAD_K_CONTROL(FBSoftwareWriteMask , 0xffffffff );
  LOAD_K_CONTROL(FilterMode ,  0x0);
  LOAD_K_CONTROL(StatisticMode , 0x0);
  LOAD_K_CONTROL(WindowOrigin , 0x0); 
}

void load_graphics_defaults()
{
  FIFOSYNC; 
/*  -load some more default values */

  LOAD_K_CONTROL(ConstantColor, 0x0); 
  LOAD_K_CONTROL(FogColor, 0xffffff); 
  LOAD_K_CONTROL(FBBlockColor , 0x0);
  LOAD_K_CONTROL(AlphaTestMode, 0x0);
  LOAD_K_CONTROL(AntialiasMode, 0x0 ); 
  LOAD_K_CONTROL(ColorDDAMode , 0x3);
  LOAD_K_CONTROL(DitherMode , DITHER_ENABLE_BGR); 
  LOAD_K_CONTROL(DepthMode ,DEPTH_ENABLE_LESS );
  LOAD_K_CONTROL(FogMode, 0x0 ); 
  LOAD_K_CONTROL(LogicalOpMode, 0x0 );  
  LOAD_K_CONTROL(RasterizerMode, 0x0); 
  LOAD_K_CONTROL(StencilMode, 0x0 ); 
  LOAD_K_CONTROL(AreaStippleMode, 0x0 ); 
  LOAD_K_CONTROL(LineStippleMode , 0x0 ); 
  LOAD_K_CONTROL(TextureColorMode, 0x0 ); 
  LOAD_K_CONTROL(AlphaBlendMode, 0x0); 
}

/* really slow clear */
void slow_clear_framebuffer()
{
  int i; 
  for(i=0;i <  (FB_SIZE_WORDS)  ;i++){ 
  	   *(glint_board.k_frame_buffer_base + i) =   0x77;
	}

}

