#include <vector>
#include <algorithm>

#include "medianFilter_common.h"
#include "medianFilter_gold.h"

extern "C" void medianFilter_gold(
    const uchar *_src, uchar *_dst,
    int w, int h, int r)
{
    const RGB* src = (const RGB*)_src;
    RGB* dst = (RGB*)_dst;

    for (int py = r; py < h-r; ++py) 
    {
        for (int px = r; px < w-r; ++px)
        {
            std::vector<RGB> pixels;                  // create and fill filter window
            for (int dy = -r; dy <= r; ++dy)
            {
                for (int dx = -r; dx <= r; ++dx)
                {
                    const int qx = px+dx;
                    const int qy = py+dy;
                    
                    pixels.push_back(src[qy * w + qx]);
                }
            }
            
            sort(pixels.begin(), pixels.end());         // sort all pixels in the window

            dst[py * w + px] = pixels[pixels.size()/2]; // copy median to output
        }
    }
}
