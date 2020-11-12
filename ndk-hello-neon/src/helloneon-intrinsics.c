/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include "helloneon-intrinsics.h"
#include <arm_neon.h>

/* this source file should only be compiled by Android.mk /CMake when targeting
 * the armeabi-v7a ABI, and should be built in NEON mode
 */
void fir_filter_neon_intrinsics(short *output, const short* input, const short* kernel, int width, int kernelSize)
{
#if 1
   int offset = -kernelSize/2;

   for (int i=0; i<width; i++)
   {
        int sum = 0;
        int32x4_t sum_vec = vdupq_n_s32(0);
        for(int j=0; j<kernelSize/4; j++)
        {
            int16x4_t  kernel_vec = vld1_s16(kernel + j*4);
            int16x4_t  input_vec = vld1_s16(input + (i+offset+j*4));
            sum_vec = vmlal_s16(sum_vec, kernel_vec, input_vec);
        }

        sum += vgetq_lane_s32(sum_vec, 0);
        sum += vgetq_lane_s32(sum_vec, 1);
        sum += vgetq_lane_s32(sum_vec, 2);
        sum += vgetq_lane_s32(sum_vec, 3);

        if(kernelSize & 3)
        {
            for(int j=kernelSize-(kernelSize&3); j<kernelSize; j++)
                sum += kernel[j] * input[i+offset+j];
        }

        output[i] = (short)((sum + 0x8000) >> 16);
    }
#else /* for comparison purposes only */
    int offset = -kernelSize/2;
    for (int i=0; i<width; i++) {
        int sum = 0;
        for (int j=0; j<kernelSize; j++) {
            sum += kernel[j]*input[i+offset+j];
        }
        output[i] = (short)((sum + 0x8000) >> 16);
    }
#endif
}
