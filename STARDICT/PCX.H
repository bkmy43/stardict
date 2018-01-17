void setEGA(void);
void openScreen(unsigned char color);
void closeScreen(void);
void writePixel(unsigned int x, unsigned int y);
void EGAWriteLinePlanes(unsigned char *planes, int npl, int planeLen,
			int x, int y);
void setPalette(unsigned char i,
		unsigned char red,
		unsigned char green,
		unsigned char blue);
void PCXGetSize(char *fname, int *x1, int *x2, int *y1, int *y2);
void PCXSetPalette(char *fname);
void PCXWrite(char *fname);

/* PCX_SAVE */
void EGAReadLine (unsigned char *planes, int planeLen, int x, int y);
int PCXSave(char *fname);

