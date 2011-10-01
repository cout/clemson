/* 
major and minor device numbers are consistent with Ken's, but 
that's about the extent of the overlap ... don't count on any similarities 
*/ 
#define GLINT_MAJOR 50 
#define GLINT_MINOR_300SX 0 
#define GLINT_MINOR_500TXD 3 
#define PCI_VENDOR_ID_3DLABS     0x3d3d 
#define PCI_DEVICE_ID_300SX      0x0001 
#define PCI_DEVICE_ID_500TXD     0x000?  
#define GLINT_CONTROL_SIZE (128*1024) 
#define ONE_PAGE (4096) 
#define FB_SIZE_WORDS (1024*1024)
#define THE_BIG_BUCKET (128*1024)
#define MAX_DMA (THE_BIG_BUCKET - ONE_PAGE)
#define V_OFF 0
#define V_1024x768_60Hz 1
#define V_640x480_75Hz 2
#define V_800x600_60Hz 3

/* glint control registers (byte offsets from control base) */ 

#define ResetStatus (0x0000) 
#define InFIFOSpace (0x0018)
#define FBMemoryCtl (0x1800)
#define RasterizerMode (0x80a0)
#define PixelSize (0x80c0)
#define ScissorMode (0x8180)
#define ScissorMinXY (0x8188)
#define ScissorMaxXY (0x8190)
#define ScreenSize (0x8198)
#define AreaStippleMode (0x81a0)
#define LineStippleMode (0x81a8)
#define WindowOrigin (0x81c8)
#define TextureColorMode (0x8680)
#define FogMode (0x8690)
#define FogColor (0x8698)
#define ColorDDAMode (0x87e0)
#define ConstantColor (0x87e8)
#define AlphaTestMode (0x8800)
#define AntialiasMode (0x8808)
#define AlphaBlendMode (0x8810)
#define DitherMode (0x8818)
#define FBSoftwareWriteMask (0x8820)
#define LogicalOpMode (0x8828)
#define LBReadMode (0x8880)
#define LBReadFormat (0x8888)
#define LBSourceOffset (0x8890)
#define LBWindowBase (0x88b8)
#define LBWriteMode (0x88c0)
#define LBWriteFormat (0x88c8)
#define Window (0x8980)
#define StencilMode (0x8988)
#define DepthMode (0x89a0)
#define FBReadMode (0x8a80)
#define FBSourceOffset (0x8a88)
#define FBPixelOffset (0x8a90)
#define FBWindowBase (0x8ab0)
#define FBWriteMode (0x8ab8)
#define FBHardwareWriteMask (0x8ac0)
#define FBBlockColor (0x8ac8)
#define FilterMode (0x8c00)
#define StatisticMode (0x8c08)

/* glint commands to load into control registers */
#define SOFTWARE_RESET (0x80000000)
#define FBREADMODE_W1024_OBL (0x10006)
#define FBREADMODE_W640_OBL (0x1001d)
#define FBREADMODE_W800_OBL (0x10065)
#define FBREADMODE_W800_OTL (0x00065)
#define LBREADMODE_W1024_OBL (0x40006)
#define LBREADMODE_W640_OBL (0x4001d)
#define LBREADMODE_W800_OBL (0x40065)
#define LBREADMODE_W800_OTL (0x00065)
#define FBWRITEMODE_ENABLE (0x01)
#define LBWRITEMODE_ENABLE (0x01)
#define LBFORMAT_D24_SW0_SP24_FCW8_FCP24_GIDW0_GIDP32 (0x8521) 
#define FILTERMODE_STAG_SDATA (0x0c00)
#define WINDOW_FAST_CLEAR (0x40001)
#define SCISSOR_SCREEN (0x2)
#define COLORDDA_GOURAUD_ENABLE (0x3)
#define DEPTH_ENABLE_LESS (0x13)
#define DITHER_ENABLE_BGR (0x1)

/* 
return value of a control register while in kernel address space;
right shift is due to arithmetic on ptr-to-unsigned-int type
*/
#define EVAL_K_CONTROL(register) (*(glint_board.k_control_base+(register>>2)))
#define LOAD_K_CONTROL(register,value) ((*(glint_board.k_control_base+(register>>2)))=value)
#define LOAD_DAC(register,value) { volatile int i; for(i=0;i<100;i++); ((*(glint_board.k_dac_base+(register>>2)))=value) ; }
#define LOAD_K_FRAMEB(pixel,value) ((*(glint_board.k_frame_buffer_base+(pixel)))=value)
#define LOAD_K_LOCALB(pixel,value) ((*(glint_board.k_local_buffer_base+(pixel)))=value)

#define FIFOSYNC while(EVAL_K_CONTROL(InFIFOSpace)<16)

struct glint_device {
	unsigned int control_base;
	unsigned int *k_control_base;
	unsigned int *k_dac_base;
	unsigned int local_buffer_base;
	unsigned int *k_local_buffer_base;
	unsigned int frame_buffer_base;
	unsigned int *k_frame_buffer_base;
	struct dmagob {
		unsigned int phys_base;
		unsigned int *virt_base;
		unsigned int *kbase;
		} dmagob[2];
	unsigned char pci_bus;
	unsigned char pci_devfn;
	unsigned char pci_irq;
	unsigned short pci_vendor;
	unsigned short pci_device;
	unsigned char pci_revision_id;
} glint_board;

/* ioctl choices */
#define BIND_CONTROL 0
#define BIND_FRAMEB 1
#define BIND_LOCALB 2
#define BIND_DMA 3
#define VMODE 4

/* dac registers ... ibm525 */
#define DAC_BYTE_OFFSET (0x4000)

#define PaletteAddrWrite (0x0000)
#define PaletteData (0x0008)
#define PelMask (0x0010)
#define PaletteAddrRead (0x0018)
#define IndexLow (0x0020)
#define IndexHigh (0x0028)
#define IndexData (0x0030)
#define IndexCtl (0x0038)

/* values to load into IndexLow (and maybe IndexHigh) register */

#define IBM525_REV (0x0000)
#define IBM525_ID (0x0001)
#define IBM525_MISC_CLK_CTL (0x0002)
#define IBM525_SYNC_CTL (0x0003)
#define IBM525_HSYNC_CTL (0x0004)
#define IBM525_PWR_MGMT (0x0005)
#define IBM525_DAC_OP (0x0006)
#define IBM525_PALETTE_CTL (0x0007)
#define IBM525_PEL_FMT (0x000a)
#define IBM525_PEL_CTL_8BIT (0x000b)
#define IBM525_PEL_CTL_16BIT (0x000c)
#define IBM525_PEL_CTL_24BIT (0x000d)
#define IBM525_PEL_CTL_32BIT (0x000e)
#define IBM525_PLL_CTL_ONE (0x0010)
#define IBM525_PLL_CTL_TWO (0x0011)
#define IBM525_PLL_REF_DIV_COUNT (0x0014)
#define IBM525_F0 (0x0020)
#define IBM525_F1 (0x0021)
#define IBM525_F2 (0x0022)
#define IBM525_F3 (0x0023)
#define IBM525_F4 (0x0024)
#define IBM525_F5 (0x0025)
#define IBM525_F6 (0x0026)
#define IBM525_F7 (0x0027)
#define IBM525_F8 (0x0028)
#define IBM525_F9 (0x0029)
#define IBM525_F10 (0x002a)
#define IBM525_F11 (0x002b)
#define IBM525_F12 (0x002c)
#define IBM525_F13 (0x002d)
#define IBM525_F14 (0x002e)
#define IBM525_F15 (0x002f)
#define IBM525_CURSOR_CTL (0x0030)
#define IBM525_CURSOR_XLOW (0x0031)
#define IBM525_CURSOR_XHI (0x0032)
#define IBM525_CURSOR_YLOW (0x0033)
#define IBM525_CURSOR_YHI (0x0034)
#define IBM525_CURSOR_HOTX (0x0035)
#define IBM525_CURSOR_HOTY (0x0036)
#define IBM525_CURSOR_RED_ONE (0x0040)
#define IBM525_CURSOR_GREEN_ONE (0x0041)
#define IBM525_CURSOR_BLUE_ONE (0x0042)
#define IBM525_CURSOR_RED_TWO (0x0043)
#define IBM525_CURSOR_GREEN_TWO (0x0044)
#define IBM525_CURSOR_BLUE_TWO (0x0045)
#define IBM525_CURSOR_RED_THREE (0x0046)
#define IBM525_CURSOR_GREEN_THREE (0x0047)
#define IBM525_CURSOR_BLUE_THREE (0x0048)
#define IBM525_BORDER_RED (0x0060)
#define IBM525_BORDER_GREEN (0x0061)
#define IBM525_BORDER_BLUE (0x0062)
#define IBM525_MISC_CTL_ONE (0x0070)
#define IBM525_MISC_CTL_TWO (0x0071)
#define IBM525_MISC_CTL_THREE (0x0072)
#define IBM525_DAC_SENSE (0x0082)
#define IBM525_MISR_RED (0x0084)
#define IBM525_MISR_GREEN (0x0086)
#define IBM525_MISR_BLUE (0x0088)
#define IBM525_PLL_VCO_DIV_INPUT (0x008e)
#define IBM525_PLL_VCO_REF_INPUT (0x008f)
#define IBM525_VRAM_MASK_LO (0x0090)
#define IBM525_VRAM_MASK_HI (0x0091)
/* caution ... this one takes high index := 0x01, low index := 0x00 */
#define IBM525_CURSOR_ARRAY (0x0100)

/* values to load into IndexData register */
/* MiscControlOne */ 
 
#define IBM525_MISR_CNTL_OFF                    (0 << 7) 
#define IBM525_MISR_CNTL_ON                     (1 << 7) 
#define IBM525_VMSK_CNTL_OFF                    (0 << 6) 
#define IBM525_VMSK_CNTL_ON                     (1 << 6) 
#define IBM525_PADR_RFMT_READ_ADDR              (0 << 5) 
#define IBM525_PADR_RFMT_PAL_STATE              (1 << 5) 
#define IBM525_SENS_DSAB_ENABLE                 (0 << 4) 
#define IBM525_SENS_DSAB_DISABLE                (1 << 4)  
#define IBM525_SENS_SEL_BIT3                    (0 << 3) 
#define IBM525_SENS_SEL_BIT7                    (1 << 3) 
#define IBM525_VRAM_SIZE_32                     (0 << 0) 
#define IBM525_VRAM_SIZE_64                     (1 << 0) 
 
/* MiscControlTwo */ 
  
#define IBM525_PCLK_SEL_LCLK                    (0 << 6) 
#define IBM525_PCLK_SEL_PLL                     (1 << 6) 
#define IBM525_PCLK_SEL_EXT                     (2 << 6) 
#define IBM525_INTL_MODE_DISABLE                (0 << 5) 
#define IBM525_INTL_MODE_ENABLE                 (1 << 5) 
#define IBM525_BLANK_CNTL_NORMAL                (0 << 4) 
#define IBM525_BLANK_CNTL_BLANKED               (1 << 4) 
#define IBM525_COL_RES_6_BIT                    (0 << 2) 
#define IBM525_COL_RES_8_BIT                    (1 << 2) 
#define IBM525_PORT_SEL_VGA                     (0 << 0) 
#define IBM525_PORT_SEL_VRAM                    (1 << 0) 
 
/* MiscControlThree */ 
 
#define IBM525_SWAP_RB_DISABLE                  (0 << 7) 
#define IBM525_SWAP_RB_ENABLE                   (1 << 7) 
#define IBM525_SWAP_WORD_31_00_FIRST            (0 << 4) 
#define IBM525_SWAP_WORD_63_32_FIRST            (1 << 4) 
#define IBM525_SWAP_NIB_07_04_FIRST             (0 << 2) 
#define IBM525_SWAP_NIB_03_00_FIRST             (1 << 2) 
 
/* MiscClockControl */ 
 
#define IBM525_DDOTCLK_ENABLE                   (0 << 7) 
#define IBM525_DDOTCLK_DISABLE                  (1 << 7) 
#define IBM525_SCLK_ENABLE                      (0 << 6) 
#define IBM525_SCLK_DISABLE                     (1 << 6) 
#define IBM525_B24P_DDOT_DIV_PLL                (0 << 5) 
#define IBM525_B24P_DDOT_SCLK                   (1 << 5) 
#define IBM525_DDOT_PLL_DIV_1                   (0 << 1) 
#define IBM525_DDOT_PLL_DIV_2                   (1 << 1) 
#define IBM525_DDOT_PLL_DIV_4                   (2 << 1) 
#define IBM525_DDOT_PLL_DIV_8                   (3 << 1) 
#define IBM525_DDOT_PLL_DIV_16                  (4 << 1) 
#define IBM525_PLL_DISABLE                      (0 << 0) 
#define IBM525_PLL_ENABLE                       (1 << 0) 
 
/* SyncControl */ 
 
#define IBM525_DLY_CNTL_ADD                     (0 << 7) 
#define IBM525_DLY_SYNC_NOADD                   (1 << 7) 
#define IBM525_CSYN_INVT_DISABLE                (0 << 6) 
#define IBM525_CSYN_INVT_ENABLE                 (1 << 6) 
#define IBM525_VSYN_INVT_DISABLE                (0 << 5) 
#define IBM525_VSYN_INVT_ENABLE                 (1 << 5) 
#define IBM525_HSYN_INVT_DISABLE                (0 << 4) 
#define IBM525_HSYN_INVT_ENABLE                 (1 << 4) 
#define IBM525_VSYN_CNTL_NORMAL                 (0 << 2) 
#define IBM525_VSYN_CNTL_HIGH                   (1 << 2) 
#define IBM525_VSYN_CNTL_LOW                    (2 << 2)  
#define IBM525_VSYN_CNTL_DISABLE                (3 << 2) 
#define IBM525_HSYN_CNTL_NORMAL                 (0 << 0) 
#define IBM525_HSYN_CNTL_HIGH                   (1 << 0) 
#define IBM525_HSYN_CNTL_LOW                    (2 << 0) 
#define IBM525_HSYN_CNTL_DISABLE                (3 << 0)  
 
/* HSyncControl */ 
 
#define IBM525_HSYN_POS(n)                      ((n) & 0xF) 
 
/* PowerManagement */
 
#define IBM525_SCLK_PWR_NORMAL                  (0 << 4) 
#define IBM525_SCLK_PWR_DISABLE                 (1 << 4) 
#define IBM525_DDOT_PWR_NORMAL                  (0 << 3) 
#define IBM525_DDOT_PWR_DISABLE                 (1 << 3) 
#define IBM525_SYNC_PWR_NORMAL                  (0 << 2) 
#define IBM525_SYNC_PWR_DISABLE                 (1 << 2) 
#define IBM525_ICLK_PWR_NORMAL                  (0 << 1) 
#define IBM525_ICLK_PWR_DISABLE                 (1 << 1) 
#define IBM525_DAC_PWR_NORMAL                   (0 << 0) 
#define IBM525_DAC_PWR_DISABLE                  (1 << 0) 
 
/* DACOperation */  
 
#define IBM525_SOG_DISABLE                      (0 << 3) 
#define IBM525_SOG_ENABLE                       (1 << 3) 
#define IBM525_BRB_NORMAL                       (0 << 2) 
#define IBM525_BRB_ALWAYS                       (1 << 2) 
#define IBM525_DSR_SLOW                         (0 << 1) 
#define IBM525_DSR_FAST                         (1 << 1) 
#define IBM525_DPE_DISABLE                      (0 << 0) 
#define IBM525_DPE_ENABLE                       (1 << 0) 
 
/* PaletteControl */ 
 
#define IBM525_6BIT_LINEAR_ENABLE               (0 << 7) 
#define IBM525_6BIT_LINEAR_DISABLE              (1 << 7) 
#define IBM525_PALETTE_PARTITION(n)             ((n) & 0xF) 

 
/* PixelFormat */ 
 
#define IBM525_PIXEL_FORMAT_4_BPP               (2 << 0) 
#define IBM525_PIXEL_FORMAT_8_BPP               (3 << 0) 
#define IBM525_PIXEL_FORMAT_16_BPP              (4 << 0) 
#define IBM525_PIXEL_FORMAT_24_BPP              (5 << 0) 
#define IBM525_PIXEL_FORMAT_32_BPP              (6 << 0) 
 
/* 8BitPixelControl */ 
 
#define IBM525_B8_DCOL_INDIRECT                 (0 << 0) 
#define IBM525_B8_DCOL_DIRECT                   (1 << 0) 
 
/* 16BitPixelControl */  
 
#define IBM525_B16_DCOL_INDIRECT                (0 << 6) 
#define IBM525_B16_DCOL_DYNAMIC                 (1 << 6) 
#define IBM525_B16_DCOL_DIRECT                  (3 << 6) 
#define IBM525_B16_POL_FORCES_BYPASS            (0 << 5) 
#define IBM525_B16_POL_FORCES_LOOKUP            (1 << 5) 
#define IBM525_B16_ZIB                          (0 << 2) 
#define IBM525_B16_LINEAR                       (1 << 2) 
#define IBM525_B16_555                          (0 << 1) 
#define IBM525_B16_565                          (1 << 1) 
#define IBM525_B16_SPARSE                       (0 << 0) 
#define IBM525_B16_CONTIGUOUS                   (1 << 0) 
 
/* 24BitPixelControl */ 
 
#define IBM525_B24_DCOL_INDIRECT                (0 << 0) 
#define IBM525_B24_DCOL_DIRECT                  (1 << 0) 
 
/* 32BitPixelControl */ 
 
#define IBM525_B32_POL_FORCES_BYPASS            (0 << 2) 
#define IBM525_B32_POL_FORCES_LOOKUP            (1 << 2) 
#define IBM525_B32_DCOL_INDIRECT                (0 << 0) 
#define IBM525_B32_DCOL_DYNAMIC                 (1 << 0) 
#define IBM525_B32_DCOL_DIRECT                  (3 << 0) 

/* PLLControlOne */
#define IBM525_REF_SRC_REFCLK                   (0 << 4) 
#define IBM525_REF_SRC_EXTCLK                   (1 << 4) 
#define IBM525_PLL_EXT_FS_DIRECT                (0 << 0) 
#define IBM525_PLL_EXT_FS_M_N                   (1 << 0) 
#define IBM525_PLL_INT_FS_DIRECT                (2 << 0) 
#define IBM525_PLL_INT_FS_M_N                   (3 << 0) 
 
/* PLLControlTwo */ 
#define IBM525_PLL_INT_FS(n)                    ((n) & 0xF) 
 
/* PLLRefDivCount */ 
#define IBM525_REF_DIV_COUNT(n)                 ((n) & 0x1F)  
 
#define IBM525_PLL_REFCLK_4_MHz                 (0x02) 
#define IBM525_PLL_REFCLK_6_MHz                 (0x03) 
#define IBM525_PLL_REFCLK_8_MHz                 (0x04) 
#define IBM525_PLL_REFCLK_10_MHz                (0x05) 
#define IBM525_PLL_REFCLK_12_MHz                (0x06) 
#define IBM525_PLL_REFCLK_14_MHz                (0x07) 
#define IBM525_PLL_REFCLK_16_MHz                (0x08) 
#define IBM525_PLL_REFCLK_18_MHz                (0x09) 
#define IBM525_PLL_REFCLK_20_MHz                (0x0A) 
#define IBM525_PLL_REFCLK_22_MHz                (0x0B) 
#define IBM525_PLL_REFCLK_24_MHz                (0x0C) 
#define IBM525_PLL_REFCLK_26_MHz                (0x0D) 
#define IBM525_PLL_REFCLK_28_MHz                (0x0E) 
#define IBM525_PLL_REFCLK_30_MHz                (0x0F)  
#define IBM525_PLL_REFCLK_32_MHz                (0x10) 
#define IBM525_PLL_REFCLK_34_MHz                (0x11) 
#define IBM525_PLL_REFCLK_36_MHz                (0x12) 
#define IBM525_PLL_REFCLK_38_MHz                (0x13) 
#define IBM525_PLL_REFCLK_40_MHz                (0x14) 
#define IBM525_PLL_REFCLK_42_MHz                (0x15) 
#define IBM525_PLL_REFCLK_44_MHz                (0x16) 
#define IBM525_PLL_REFCLK_46_MHz                (0x17) 
#define IBM525_PLL_REFCLK_48_MHz                (0x18) 
#define IBM525_PLL_REFCLK_50_MHz                (0x19) 
#define IBM525_PLL_REFCLK_52_MHz                (0x1A) 
#define IBM525_PLL_REFCLK_54_MHz                (0x1B) 
#define IBM525_PLL_REFCLK_56_MHz                (0x1C) 
#define IBM525_PLL_REFCLK_58_MHz                (0x1D) 
#define IBM525_PLL_REFCLK_60_MHz                (0x1E) 
#define IBM525_PLL_REFCLK_62_MHz                (0x1F) 
 
/* F0-F15[7:0] */ 
 
#define IBM525_DF(n)                            (((n) & 0x3) << 6) 
#define IBM525_VCO_DIV_COUNT(n)                 ((n) & 0x3F) 

/* CursorControl */ 
 
#define IBM525_SMLC_PART_0                      (0 << 6) 
#define IBM525_SMLC_PART_1                      (1 << 6) 
#define IBM525_SMLC_PART_2                      (2 << 6) 
#define IBM525_SMLC_PART_3                      (3 << 6) 
#define IBM525_PIX_ORDER_RIGHT_TO_LEFT          (0 << 5) 
#define IBM525_PIX_ORDER_LEFT_TO_RIGHT          (1 << 5) 
#define IBM525_LOC_READ_LAST_WRITTEN            (0 << 4) 
#define IBM525_LOC_READ_ACTUAL_LOCATION         (1 << 4) 
#define IBM525_UPDT_CNTL_DELAYED                (0 << 3) 
#define IBM525_UPDT_CNTL_IMMEDIATE              (1 << 3) 
#define IBM525_CURSOR_SIZE_32                   (0 << 2) 
#define IBM525_CURSOR_SIZE_64                   (1 << 2) 
#define IBM525_CURSOR_MODE_OFF                  (0 << 0)  
#define IBM525_CURSOR_MODE_3_COLOR              (1 << 0)  
#define IBM525_CURSOR_MODE_2_COLOR_HL           (2 << 0) 
#define IBM525_CURSOR_MODE_2_COLOR              (3 << 0) 

/* some extras ...  */
#define DIVIDER (2)

/* VTG registers */

#define VTGHlimit (0x3000)
#define VTGHSyncStart (0x3008)
#define VTGHSyncEnd (0x3010)
#define VTGHBlankEnd (0x3018)
#define VTGVlimit (0x3020)
#define VTGVSyncStart (0x3028)
#define VTGVSyncEnd (0x3030)
#define VTGVBlankEnd (0x3038)
#define VTGHGateStart (0x3040)
#define VTGHGateEnd (0x3048)
#define VTGVGateStart (0x3050)
#define VTGVGateEnd (0x3058)
#define VTGPolarity (0x3060)
#define VTGFrameRowAddr (0x3068)
#define VTGSerialClk (0x3078)

/* commands to load into VTG registers */

#define SERIAL_CLOCK_SPEC (0x06)

/* ON values shouldn't be needed ... */
#define POLARITY_1024x768_60Hz_ON (0xb2)
#define POLARITY_1024x768_60Hz_OFF (0x92)
#define POLARITY_640x480_75Hz_ON (0xba)
#define POLARITY_640x480_75Hz_OFF (0x9a)
#define POLARITY_800x600_60Hz_ON (0xb0)
#define POLARITY_800x600_60Hz_OFF (0x90)

