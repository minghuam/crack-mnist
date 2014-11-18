#include <iostream>
#include <cstdio>
#include <cstdlib>

class Util{
public:
	// https://en.wikipedia.org/wiki/Netpbm_format
	static void save_pgm(const char* file, const unsigned char* data, \
		int width, int height, bool inverted = false){
		FILE *fp = fopen(file, "wb");
		fprintf(fp, "P5\n# created by minghuam\n%d %d\n%d\n", width, height, 255);
		if(inverted){
			unsigned char *buf = (unsigned char*)malloc(width*height);
			for(int i=0; i<width*height; i++){
				buf[i] = 255 - data[i];
			}
			fwrite(buf, 1, width*height, fp);
			free(buf);
		}else{
			fwrite(data, 1, width*height, fp);
		}
		fclose(fp);
	}
	
};