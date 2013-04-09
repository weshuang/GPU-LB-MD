#ifndef _MEDIAN_FILTER_GOLD_H_
#define _MEDIAN_FILTER_GOLD_H_

extern "C" void medianFilter_gold(
		const uchar *src, uchar *dst,
		int width, int height,
		int filter_radius);

#endif
