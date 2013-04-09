#ifndef _MEDIAN_FILTER_COMMON_H_
#define _MEDIAN_FILTER_COMMON_H_

typedef unsigned int uint;
typedef unsigned char uchar;


/// Compute the luminance of a pixel with the given color
// use fixed-point luminance to prevent unexpected round-off errors
// return 0.2126f * r + 0.7152f * g + 0.0722f * b;
#define computeLuminance(r, g, b) (uint(3525200) * (r) + uint(11858999) * (g) + uint(1197175) * (b))

/// Defines an RGB pixel
struct RGB
{
    uchar R, G, B;

    // uchar __padding;

    bool operator <(const RGB& rhs) const
    {
        return computeLuminance(R,G,B) < computeLuminance(rhs.R, rhs.G, rhs.B);
    }
};

#endif
