#include "medianFilter_kernel.h"
#include "medianFilter_gold.h"

#include <cuda.h>
#include <cuda_runtime.h>

#include <helper_cuda.h>
#include <helper_timer.h>
#include <helper_image.h>


void loadImage(
    uchar **img,
    uint *width,
    uint *height,
    const char* imagePath,
    const char* )
{
    if (imagePath == 0) {
        fprintf(stderr, "Error finding image file '%s'\n", imagePath);
        exit(-1);
    }

    sdkLoadPGM(imagePath, img, width, height);

    if (!*img) {
        fprintf(stderr, "Error opening file '%s'\n", imagePath);
        exit(-1);
    }

    printf("Loaded '%s', %d x %d pixels\n", imagePath, *width, *height);
}

void saveImage(
    uchar* img,
    const uint w,
    const uint h,
    const char* file_path)
{
    sdkSavePGM(file_path, img, w, h);
}

bool testNoBounds(
    const uchar* _result1,
    const uchar* _result2,
    const uint w,
    const uint h,
    const uint r)
{
    const RGB* result1 = (const RGB*)_result1;
    const RGB* result2 = (const RGB*)_result2;
    for (uint x = r; x < w-r; ++x) {
        for (uint y = r; y < h-r; ++y) {
            const uint idx = y*w+x;
            const RGB& r1 = result1[idx];
            const RGB& r2 = result2[idx];
            if (computeLuminance(r1.R, r1.B, r1.G) != computeLuminance(r2.R, r2.B, r2.G))
                return false;
        }
    }
    return true;
}

int main(int, char* argv[])
{
    // In assignment 2.1, we will fix r as 1,
    // and therefore it's filter size is 3x3.
    const uint r = 1;

    uint width, height;
    uchar* h_img = nullptr;

    // This is the filename of the image file in PGM format
    // and the file should be put under the subdirectory, "data".
    const char* image_filename = "data/testimg.ppm";

    loadImage(&h_img, &width, &height, image_filename, argv[0]);
    //    saveImage(h_img, width, height, "result.pgm");

    if (width <= 2*r || height <= 2*r) 
    {
        fprintf(stderr, "Filter radius is too large.\n");
        exit(-1);
    }
    
    uchar* yourResult = new uchar[3 * width * height];

    // Run your median filter
    {
        cudaDeviceReset();

        StopWatchInterface* timer = nullptr;
        sdkCreateTimer(&timer);
        timer->start();

        medianFilter(h_img, yourResult, width, height, r);

		//on yourResult
		FILE *fin;
		fin=fopen("youresult.dat","w");
		RGB* rgbyourResult = (RGB*) yourResult;
		int i,j;	
		for(i=0;i<height;i++)//height
		{
			for(j=0;j<width;j++)
				fprintf(fin, "%ld ", computeLuminance(rgbyourResult[i*width+j].R, rgbyourResult[i*width+j].G, rgbyourResult[i*width+j].B));
			fprintf(fin, "\n");
		}
		fclose(fin);

        timer->stop();
        //FILE *fp = fopen("speed", "w");
        //fprintf(fp, "%f", timer->getTime());
        //fclose(fp);
        printf("Your version took: %f ms.\n", timer->getTime());
        sdkDeleteTimer(&timer);
    }
    
    uchar* cpuResult = new uchar[3 * width * height];

    // Run GOLD version on CPU
    {
        StopWatchInterface* timer = nullptr;
        sdkCreateTimer(&timer);
        timer->start();

        medianFilter_gold(h_img, cpuResult, width, height, r);

		//on cpuresult
		FILE *fin;
		fin=fopen("cpuresult.dat","w");
		RGB* rgbcpuResult= (RGB*) cpuResult;
		int i,j;
		for(i=0;i<height;i++)
		{
			for(j=0;j<width;j++)
				fprintf(fin, "%ld ", computeLuminance(rgbcpuResult[i*width+j].R, rgbcpuResult[i*width+j].G, rgbcpuResult[i*width+j].B));
			fprintf(fin,"\n");
		}
		fclose(fin);

        timer->stop();
        printf("GOLD version took: %f ms.\n", timer->getTime());
        sdkDeleteTimer(&timer);
    }

    //saveImage(yourResult, width, height, "result.pgm");
    if (testNoBounds(yourResult, cpuResult, width, height, r))
    {
        printf("Test PASSED.\n");
    }
    else
    {
        printf("Test FAILED.\n");
    }
    delete [] yourResult;
    delete [] cpuResult;
    
    //printf("Press ENTER to exit.");
    //getchar();
    return 0;
}
