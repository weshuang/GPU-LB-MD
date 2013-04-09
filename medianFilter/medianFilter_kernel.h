#ifndef _MEDIAN_FILTER_KERNEL_H_
#define _MEDIAN_FILTER_KERNEL_H_

#include "medianFilter_common.h"

extern "C" void medianFilter(
		const uchar *src, uchar *dst,
		int w, int h,
		int r);

#endif
