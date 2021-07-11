/*
 * Copyright (c) 2016 NSR (National Security Research Institute)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to deal 
 * in the Software without restriction, including without limitation the rights 
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
 * copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
 * THE SOFTWARE.
 */

#pragma once
#ifndef _UTILS_CPU_INFO_H
#define _UTILS_CPU_INFO_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	unsigned char mmx;
	unsigned char sse;
	unsigned char sse2;
	unsigned char sse3;
	
	unsigned char pclmul;
	unsigned char ssse3;
	unsigned char sse41;
	unsigned char sse42;
	unsigned char aes;
	
	unsigned char avx;
	unsigned char fma3;
	
	unsigned char rdrand;
	
	unsigned char avx2;
	
	unsigned char bmi1;
	unsigned char bmi2;
	unsigned char adx;
	unsigned char sha;
	unsigned char prefetchwt1;
	
	unsigned char avx512f;
	unsigned char avx512cd;
	unsigned char avx512pf;
	unsigned char avx512er;
	unsigned char avx512vl;
	unsigned char avx512bw;
	unsigned char avx512dq;
	unsigned char avx512ifma;
	unsigned char avx512vbmi;
	
	unsigned char x64;
	unsigned char abm;
	unsigned char sse4a;
	unsigned char fma4;
	unsigned char xop;
} info_ia32;

void get_ia32_cpuinfo(info_ia32* pInfo, unsigned char check_os_support);

#ifdef __cplusplus
}
#endif
#endif
