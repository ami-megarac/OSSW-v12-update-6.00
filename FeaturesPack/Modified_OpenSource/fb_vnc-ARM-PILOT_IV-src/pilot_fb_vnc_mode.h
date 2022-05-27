#ifndef __PILOT_FB_VNC_MODE_H__
#define __PILOT_FB_VNC_MODE_H__


struct pilot_fb_mode_info_t {
	unsigned long horiz_total;
	unsigned long horiz_display_end;
	unsigned long horiz_front_porch;
	unsigned long horiz_sync;

	unsigned long verti_total;
	unsigned long verti_display_end;
	unsigned long verti_front_porch;
	unsigned long verti_sync;

	int refresh_rate;
	int clk_tab_index;

	unsigned int flags;
};


struct pilotfb_dfbinfo {
	unsigned long ulFBSize;
	unsigned long ulFBPhys;

	unsigned long ulCMDQSize;
	unsigned long ulCMDQOffset;

	unsigned long ul2DMode;
};

struct pilotfb_dispsrc {
	u32 val;
};

typedef struct tagModeDescription {
 unsigned long dwWidth; // # pixels in each horizontal line
 unsigned long dwHeight; // # vertical lines
 unsigned long dwPitch; // # pixels between (X=0,Y=0) and (X=0,Y=1)
 unsigned long dwBitsPerPixel; // # bits in each pixel. Divide by 8 to get UINT8s per pixel
 unsigned long bMgaValid; // TRUE if this structure is valid and in MGA [non-VGA] mode
 unsigned long adwReserved[ 3 ]; // padding
 } ModeDescription;

#endif /* !__PILOT_FB_VNC_MODE_H__ */


