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

#include "../include/lsh.h"
#include "lsh_local.h"


#ifndef LSH_NO_SIMD
#include "cpu_info.h"

#ifdef LSH_COMPILE_AVX2
#include "avx2/lsh256_avx2.h"
#include "avx2/lsh512_avx2.h"
#endif

#ifdef LSH_COMPILE_XOP
#include "xop/lsh256_xop.h"
#include "xop/lsh512_xop.h"
#endif

#ifdef LSH_COMPILE_SSSE3
#include "ssse3/lsh256_ssse3.h"
#include "ssse3/lsh512_ssse3.h"
#endif

#ifdef LSH_COMPILE_SSE2
#include "sse2/lsh256_sse2.h"
#include "sse2/lsh512_sse2.h"
#endif

#include "no_arch/lsh256.h"
#include "no_arch/lsh512.h"

#ifdef LSH_ARCH_IA32
static info_ia32 g_info_ia32 = { 0, };
#endif

static const char * g_cszSIMD = "ndef";

static lsh_err lsh256_init_ndef(struct LSH256_Context * ctx, const lsh_type algtype);
static lsh_err lsh256_update_ndef(struct LSH256_Context * ctx, const lsh_u8 * data, size_t databitlen);
static lsh_err lsh256_final_ndef(struct LSH256_Context * ctx, lsh_u8 * hashval);
static lsh_err lsh256_digest_ndef(const lsh_type algtype, const lsh_u8 * data, size_t databitlen, lsh_u8 * hashval);

static lsh_err lsh512_init_ndef(struct LSH512_Context * ctx, const lsh_type algtype);
static lsh_err lsh512_update_ndef(struct LSH512_Context * ctx, const lsh_u8 * data, size_t databitlen);
static lsh_err lsh512_final_ndef(struct LSH512_Context * ctx, lsh_u8 * hashval);
static lsh_err lsh512_digest_ndef(const lsh_type algtype, const lsh_u8 * data, size_t databitlen, lsh_u8 * hashval);


static PtrLSHInit256 g_pLSH256_init = lsh256_init_ndef;
static PtrLSHUpdate256 g_pLSH256_update = lsh256_update_ndef;
static PtrLSHFinal256 g_pLSH256_final = lsh256_final_ndef;
static PtrLSHDigest256 g_pLSH256_digest = lsh256_digest_ndef;

static PtrLSHInit512 g_pLSH512_init = lsh512_init_ndef;
static PtrLSHUpdate512 g_pLSH512_update = lsh512_update_ndef;
static PtrLSHFinal512 g_pLSH512_final = lsh512_final_ndef;
static PtrLSHDigest512 g_pLSH512_digest = lsh512_digest_ndef;

static lsh_err lsh256_init_ndef(struct LSH256_Context * ctx, const lsh_type algtype){ lsh_init_simd(); return g_pLSH256_init(ctx, algtype); }
static lsh_err lsh256_update_ndef(struct LSH256_Context * ctx, const lsh_u8 * data, size_t databitlen){ lsh_init_simd(); return g_pLSH256_update(ctx, data, databitlen); }
static lsh_err lsh256_final_ndef(struct LSH256_Context * ctx, lsh_u8 * hashval){ lsh_init_simd(); return g_pLSH256_final(ctx, hashval); }
static lsh_err lsh256_digest_ndef(const lsh_type algtype, const lsh_u8 * data, size_t databitlen, lsh_u8 * hashval){ lsh_init_simd(); return g_pLSH256_digest(algtype, data, databitlen, hashval); }

static lsh_err lsh512_init_ndef(struct LSH512_Context * ctx, const lsh_type algtype){ lsh_init_simd(); return g_pLSH512_init(ctx, algtype); }
static lsh_err lsh512_update_ndef(struct LSH512_Context * ctx, const lsh_u8 * data, size_t databitlen){ lsh_init_simd(); return g_pLSH512_update(ctx, data, databitlen); }
static lsh_err lsh512_final_ndef(struct LSH512_Context * ctx, lsh_u8 * hashval){ lsh_init_simd(); return g_pLSH512_final(ctx, hashval); }
static lsh_err lsh512_digest_ndef(const lsh_type algtype, const lsh_u8 * data, size_t databitlen, lsh_u8 * hashval){ lsh_init_simd(); return g_pLSH512_digest(algtype, data, databitlen, hashval); }


const char * lsh_get_simd_type(){
	if (g_pLSH256_init == lsh256_init_ndef){
		lsh_init_simd();
	}

	return g_cszSIMD;
}

void lsh_init_simd(){
#ifdef LSH_ARCH_IA32
	get_ia32_cpuinfo(&g_info_ia32, 1);

	if (!g_info_ia32.sse2){
		g_info_ia32.xop = 0;
		g_info_ia32.avx2 = 0;
	}

	if (0){}
#ifdef LSH_COMPILE_AVX2
	else if (g_info_ia32.avx2){
		g_cszSIMD = "avx2";

		g_pLSH256_init = lsh256_avx2_init;
		g_pLSH256_update = lsh256_avx2_update;
		g_pLSH256_final = lsh256_avx2_final;
		g_pLSH256_digest = lsh256_avx2_digest;

		g_pLSH512_init = lsh512_avx2_init;
		g_pLSH512_update = lsh512_avx2_update;
		g_pLSH512_final = lsh512_avx2_final;
		g_pLSH512_digest = lsh512_avx2_digest;
	}
#endif //AVX2
#ifdef LSH_COMPILE_XOP
	else if (g_info_ia32.xop){
		g_cszSIMD = "xop";

		g_pLSH256_init = lsh256_xop_init;
		g_pLSH256_update = lsh256_xop_update;
		g_pLSH256_final = lsh256_xop_final;
		g_pLSH256_digest = lsh256_xop_digest;

		g_pLSH512_init = lsh512_xop_init;
		g_pLSH512_update = lsh512_xop_update;
		g_pLSH512_final = lsh512_xop_final;
		g_pLSH512_digest = lsh512_xop_digest;
	}
#endif
#ifdef LSH_COMPILE_SSSE3
	else if (g_info_ia32.ssse3){
		g_cszSIMD = "ssse3";

		g_pLSH256_init = lsh256_ssse3_init;
		g_pLSH256_update = lsh256_ssse3_update;
		g_pLSH256_final = lsh256_ssse3_final;
		g_pLSH256_digest = lsh256_ssse3_digest;

		g_pLSH512_init = lsh512_ssse3_init;
		g_pLSH512_update = lsh512_ssse3_update;
		g_pLSH512_final = lsh512_ssse3_final;
		g_pLSH512_digest = lsh512_ssse3_digest;
	}
#endif
#ifdef LSH_COMPILE_SSE2
	else if (g_info_ia32.sse2){
		g_cszSIMD = "sse2";

		g_pLSH256_init = lsh256_sse2_init;
		g_pLSH256_update = lsh256_sse2_update;
		g_pLSH256_final = lsh256_sse2_final;
		g_pLSH256_digest = lsh256_sse2_digest;

		g_pLSH512_init = lsh512_sse2_init;
		g_pLSH512_update = lsh512_sse2_update;
		g_pLSH512_final = lsh512_sse2_final;
		g_pLSH512_digest = lsh512_sse2_digest;
	}
#endif // SSE2
#endif
	else{
		g_cszSIMD = "ref";

		g_pLSH256_init = lsh256_init;
		g_pLSH256_update = lsh256_update;
		g_pLSH256_final = lsh256_final;
		g_pLSH256_digest = lsh256_digest;

		g_pLSH512_init = lsh512_init;
		g_pLSH512_update = lsh512_update;
		g_pLSH512_final = lsh512_final;
		g_pLSH512_digest = lsh512_digest;
	}
}


lsh_err lsh_init(union LSH_Context * state, const lsh_type algtype){
	if (state == NULL){
		return LSH_ERR_NULL_PTR;
	}

	if (LSH_IS_LSH256(algtype)){
		return g_pLSH256_init(&state->ctx256, algtype);
	}
	else if (LSH_IS_LSH512(algtype)){
		return g_pLSH512_init(&state->ctx512, algtype);
	}
	else{
		return LSH_ERR_INVALID_ALGTYPE;
	}
}
lsh_err lsh_update(union LSH_Context * state, const lsh_u8 * data, size_t databitlen){
	if (state == NULL){
		return LSH_ERR_NULL_PTR;
	}

	if (LSH_IS_LSH256(state->algtype)){
		return g_pLSH256_update(&state->ctx256, data, databitlen);
	}
	else{
		return g_pLSH512_update(&state->ctx512, data, databitlen);
	}
}
lsh_err lsh_final(union LSH_Context * state, lsh_u8 * hashval){
	if (state == NULL){
		return LSH_ERR_NULL_PTR;
	}

	if (LSH_IS_LSH256(state->algtype)){
		return g_pLSH256_final(&state->ctx256, hashval);
	}
	else{
		return g_pLSH512_final(&state->ctx512, hashval);
	}

}

lsh_err lsh_digest(const lsh_type algtype, const lsh_u8 * data, size_t databitlen, lsh_u8 * hashval){
	if (LSH_IS_LSH256(algtype)){
		return g_pLSH256_digest(algtype, data, databitlen, hashval);
	}
	else if(LSH_IS_LSH512(algtype)){
		return g_pLSH512_digest(algtype, data, databitlen, hashval);
	}
	else{
		return LSH_ERR_INVALID_ALGTYPE;
	}
}

#else

lsh_err lsh_init(union LSH_Context * state, const lsh_type algtype){
	if (state == NULL){
		return LSH_ERR_NULL_PTR;
	}

	if (LSH_IS_LSH256(algtype)){
		return lsh256_init(&state->ctx256, algtype);
	}
	else if (LSH_IS_LSH512(algtype)){
		return lsh512_init(&state->ctx512, algtype);
	}
	else{
		return LSH_ERR_INVALID_ALGTYPE;
	}
}
lsh_err lsh_update(union LSH_Context * state, const lsh_u8 * data, size_t databitlen){
	if (state == NULL){
		return LSH_ERR_NULL_PTR;
	}
	
	if (LSH_IS_LSH256(state->algtype)){
		return lsh256_update(&state->ctx256, data, databitlen);
	}
	else{
		return lsh512_update(&state->ctx512, data, databitlen);
	}
}
lsh_err lsh_final(union LSH_Context * state, lsh_u8 * hashval){
	if (state == NULL){
		return LSH_ERR_NULL_PTR;
	}

	if (LSH_IS_LSH256(state->algtype)){
		return lsh256_final(&state->ctx256, hashval);
	}
	else{
		return lsh512_final(&state->ctx512, hashval);
	}

}

lsh_err lsh_digest(const lsh_type algtype, const lsh_u8 * data, size_t databitlen, lsh_u8 * hashval){
	if (LSH_IS_LSH256(algtype)){
		return lsh256_digest(algtype, data, databitlen, hashval);
	}
	else if(LSH_IS_LSH512(algtype)){
		return lsh512_digest(algtype, data, databitlen, hashval);
	}
	else{
		return LSH_ERR_INVALID_ALGTYPE;
	}
}
#endif
