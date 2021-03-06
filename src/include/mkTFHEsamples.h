#ifndef MKTFHESAMPLES_H
#define MKTFHESAMPLES_H

#include "tfhe_core.h"



// MK LWE sample (a_1, ..., a_k, b)
struct MKLweSample {
	Torus32* a; //-- the parties*n coefs of the mask
    Torus32 b;  //
   	double current_variance; //-- average noise of the sample
   	const int32_t parties;
   	const int32_t n;

#ifdef __cplusplus
   MKLweSample(const LweParams* LWEparams, const MKTFHEParams* MKparams);
   ~MKLweSample();
   MKLweSample(const MKLweSample&)=delete;
   MKLweSample& operator=(const MKLweSample&)=delete;
#endif
};


// alloc 
EXPORT MKLweSample* alloc_MKLweSample();
EXPORT MKLweSample* alloc_MKLweSample_array(int32_t nbelts);
//free memory space 
EXPORT void free_MKLweSample(MKLweSample* ptr);
EXPORT void free_MKLweSample_array(int32_t nbelts, MKLweSample* ptr);
// init
EXPORT void init_MKLweSample(MKLweSample* obj, const LweParams* LWEparams, const MKTFHEParams* MKparams);
EXPORT void init_MKLweSample_array(int32_t nbelts, MKLweSample* obj, const LweParams* LWEparams, 
        const MKTFHEParams* MKparams);
// destroys the structure
EXPORT void destroy_MKLweSample(MKLweSample* obj);
EXPORT void destroy_MKLweSample_array(int32_t nbelts, MKLweSample* obj);
// new = alloc + init
EXPORT MKLweSample* new_MKLweSample(const LweParams* LWEparams, const MKTFHEParams* MKparams);
EXPORT MKLweSample* new_MKLweSample_array(int32_t nbelts, const LweParams* LWEparams, const MKTFHEParams* MKparams);
// delete = destroy + free
EXPORT void delete_MKLweSample(MKLweSample* obj);
EXPORT void delete_MKLweSample_array(int32_t nbelts, MKLweSample* obj);












// MK RLWE sample (a_1, ..., a_k, b)
struct MKTLweSample {
    TorusPolynomial *a; ///< array of length parties+1: mask + right term
    TorusPolynomial *b; ///< alias of a[parties] to get the right term
    double current_variance; ///< avg variance of the sample
    const int32_t parties;
    const int32_t N;


#ifdef __cplusplus
    MKTLweSample(const TLweParams* RLWEparams, const MKTFHEParams* MKparams);
    ~MKTLweSample();
    MKTLweSample(const MKTLweSample &) = delete;
    void operator=(const MKTLweSample &) = delete;
#endif
};


// alloc 
EXPORT MKTLweSample* alloc_MKTLweSample();
EXPORT MKTLweSample* alloc_MKTLweSample_array(int32_t nbelts);
//free memory space 
EXPORT void free_MKTLweSample(MKTLweSample* ptr);
EXPORT void free_MKTLweSample_array(int32_t nbelts, MKTLweSample* ptr);
// init
EXPORT void init_MKTLweSample(MKTLweSample* obj, const TLweParams* RLWEparams, const MKTFHEParams* MKparams);
EXPORT void init_MKTLweSample_array(int32_t nbelts, MKTLweSample* obj, const TLweParams* RLWEparams, 
        const MKTFHEParams* MKparams);
// destroys the structure
EXPORT void destroy_MKTLweSample(MKTLweSample* obj);
EXPORT void destroy_MKTLweSample_array(int32_t nbelts, MKTLweSample* obj);
// new = alloc + init
EXPORT MKTLweSample* new_MKTLweSample(const TLweParams* RLWEparams, const MKTFHEParams* MKparams);
EXPORT MKTLweSample* new_MKTLweSample_array(int32_t nbelts, const TLweParams* RLWEparams, 
        const MKTFHEParams* MKparams);
// delete = destroy + free
EXPORT void delete_MKTLweSample(MKTLweSample* obj);
EXPORT void delete_MKTLweSample_array(int32_t nbelts, MKTLweSample* obj);

















// MK RLWE sample FFT (a_1, ..., a_k, b)
struct MKTLweSampleFFT {
    LagrangeHalfCPolynomial *a; ///< array of length parties+1: mask + right term
    LagrangeHalfCPolynomial *b; ///< alias of a[parties] to get the right term
    double current_variance; ///< avg variance of the sample
    const int32_t parties; 

#ifdef __cplusplus
    MKTLweSampleFFT(const TLweParams *params, const MKTFHEParams* MKparams, LagrangeHalfCPolynomial *arr, 
		double current_variance);
    ~MKTLweSampleFFT();
    MKTLweSampleFFT(const MKTLweSampleFFT &) = delete;
    void operator=(const MKTLweSampleFFT &) = delete;
#endif
};


// alloc 
EXPORT MKTLweSampleFFT* alloc_MKTLweSampleFFT();
EXPORT MKTLweSampleFFT* alloc_MKTLweSampleFFT_array(int32_t nbelts);
//free memory space 
EXPORT void free_MKTLweSampleFFT(MKTLweSampleFFT* ptr);
EXPORT void free_MKTLweSampleFFT_array(int32_t nbelts, MKTLweSampleFFT* ptr);
// init
EXPORT void init_MKTLweSampleFFT(MKTLweSampleFFT* obj, const TLweParams* RLWEparams, const MKTFHEParams* MKparams, 
        LagrangeHalfCPolynomial *arr, double current_variance);
EXPORT void init_MKTLweSampleFFT_array(int32_t nbelts, MKTLweSampleFFT* obj, const TLweParams* RLWEparams, 
        const MKTFHEParams* MKparams, LagrangeHalfCPolynomial *arr, double current_variance);
// destroys the structure
EXPORT void destroy_MKTLweSampleFFT(MKTLweSampleFFT* obj);
EXPORT void destroy_MKTLweSampleFFT_array(int32_t nbelts, MKTLweSampleFFT* obj);
// new = alloc + init
EXPORT MKTLweSampleFFT* new_MKTLweSampleFFT(const TLweParams* RLWEparams, const MKTFHEParams* MKparams, 
        LagrangeHalfCPolynomial *arr, double current_variance);
EXPORT MKTLweSampleFFT* new_MKTLweSampleFFT_array(int32_t nbelts, const TLweParams* RLWEparams, 
        const MKTFHEParams* MKparams, LagrangeHalfCPolynomial *arr, double current_variance);
// delete = destroy + free
EXPORT void delete_MKTLweSampleFFT(MKTLweSampleFFT* obj);
EXPORT void delete_MKTLweSampleFFT_array(int32_t nbelts, MKTLweSampleFFT* obj);




























// MK RGSW UniEnc sample (C,D,F)=(c0,c1,d0,d1,f0,f1)
struct MKTGswUESample {
    TorusPolynomial *c; ///< array of length 6*dg
    TorusPolynomial *d; ///< alias of c[2*dg]
    TorusPolynomial *f; ///< alias of c[4*dg]
    int32_t party; // party 
    double current_variance; ///< avg variance of the sample
    const int32_t dg;
    const int32_t N;

#ifdef __cplusplus
    MKTGswUESample(const TLweParams* RLWEparams, const MKTFHEParams* MKparams);
    ~MKTGswUESample();
    MKTGswUESample(const MKTGswUESample &) = delete;
    void operator=(const MKTGswUESample &) = delete;
#endif
};




// alloc
EXPORT MKTGswUESample* alloc_MKTGswUESample();
EXPORT MKTGswUESample* alloc_MKTGswUESample_array(int32_t nbelts);
//free memory space 
EXPORT void free_MKTGswUESample(MKTGswUESample* ptr);
EXPORT void free_MKTGswUESample_array(int32_t nbelts, MKTGswUESample* ptr);
// initialize the structure
EXPORT void init_MKTGswUESample(MKTGswUESample* obj, const TLweParams* RLWEparams, const MKTFHEParams* MKparams);
EXPORT void init_MKTGswUESample_array(int32_t nbelts, MKTGswUESample* obj, const TLweParams* RLWEparams, 
        const MKTFHEParams* MKparams);
//destroys the structure
EXPORT void destroy_MKTGswUESample(MKTGswUESample* obj);
EXPORT void destroy_MKTGswUESample_array(int32_t nbelts, MKTGswUESample* obj);
// new = alloc + init
EXPORT MKTGswUESample* new_MKTGswUESample(const TLweParams* RLWEparams, const MKTFHEParams* MKparams);
EXPORT MKTGswUESample* new_MKTGswUESample_array(int32_t nbelts, const TLweParams* RLWEparams, 
        const MKTFHEParams* MKparams);
// delete = destroy + free
EXPORT void delete_MKTGswUESample(MKTGswUESample* obj);
EXPORT void delete_MKTGswUESample_array(int32_t nbelts, MKTGswUESample* obj);





























// MK RGSW UniEnc sample FFT (C,D,F)=(c0,c1,d0,d1,f0,f1)
struct MKTGswUESampleFFT {
    LagrangeHalfCPolynomial *c; ///< array of length 6*dg
    LagrangeHalfCPolynomial *d; ///< alias of c[2*dg]
    LagrangeHalfCPolynomial *f; ///< alias of c[4*dg]
    int32_t party; // party 
    double current_variance; ///< avg variance of the sample
    const int32_t dg; 

#ifdef __cplusplus
    MKTGswUESampleFFT(const TLweParams* RLWEparams, const MKTFHEParams* MKparams, 
		LagrangeHalfCPolynomial *arr, double current_variance);
    ~MKTGswUESampleFFT();
    MKTGswUESampleFFT(const MKTGswUESampleFFT &) = delete;
    void operator=(const MKTGswUESampleFFT &) = delete;
#endif
};


// alloc
EXPORT MKTGswUESampleFFT* alloc_MKTGswUESampleFFT();
EXPORT MKTGswUESampleFFT* alloc_MKTGswUESampleFFT_array(int32_t nbelts);
//free memory space 
EXPORT void free_MKTGswUESampleFFT(MKTGswUESampleFFT* ptr);
EXPORT void free_MKTGswUESampleFFT_array(int32_t nbelts, MKTGswUESampleFFT* ptr);
// initialize the structure
EXPORT void init_MKTGswUESampleFFT(MKTGswUESampleFFT* obj, const TLweParams* RLWEparams, const MKTFHEParams* MKparams, 
        LagrangeHalfCPolynomial *arr, double current_variance);
EXPORT void init_MKTGswUESampleFFT_array(int32_t nbelts, MKTGswUESampleFFT* obj, const TLweParams* RLWEparams, 
        const MKTFHEParams* MKparams, LagrangeHalfCPolynomial *arr, double current_variance);
//destroys the structure
EXPORT void destroy_MKTGswUESampleFFT(MKTGswUESampleFFT* obj);
EXPORT void destroy_MKTGswUESampleFFT_array(int32_t nbelts, MKTGswUESampleFFT* obj);
// new = alloc + init
EXPORT MKTGswUESampleFFT* new_MKTGswUESampleFFT(const TLweParams* RLWEparams, const MKTFHEParams* MKparams, 
        LagrangeHalfCPolynomial *arr, double current_variance);
EXPORT MKTGswUESampleFFT* new_MKTGswUESampleFFT_array(int32_t nbelts, const TLweParams* RLWEparams, 
        const MKTFHEParams* MKparams, LagrangeHalfCPolynomial *arr, double current_variance);
// delete = destroy + free
EXPORT void delete_MKTGswUESampleFFT(MKTGswUESampleFFT* obj);
EXPORT void delete_MKTGswUESampleFFT_array(int32_t nbelts, MKTGswUESampleFFT* obj);























// MK RGSW Expanded sample: party i C=(y1, ..., d1, ..., yk, c1, d0+x1, ..., d0, ..., d0+xk, c0)
struct MKTGswExpSample {
    TorusPolynomial *c; ///< array of length 2*(parties+1)*dg
    TorusPolynomial *y;
    TorusPolynomial *c1;
    TorusPolynomial *x;
    TorusPolynomial *c0;
    int32_t party; // party (from 0 to parties-1)
    double current_variance; ///< avg variance of the sample
    const int32_t parties; 
    const int32_t dg;
    const int32_t N;

#ifdef __cplusplus
    MKTGswExpSample(const TLweParams* RLWEparams, const MKTFHEParams* MKparams);
    ~MKTGswExpSample();
    MKTGswExpSample(const MKTGswExpSample &) = delete;
    void operator=(const MKTGswExpSample &) = delete;
#endif
};


// alloc
EXPORT MKTGswExpSample* alloc_MKTGswExpSample() ;
EXPORT MKTGswExpSample* alloc_MKTGswExpSample_array(int32_t nbelts);
//free memory space 
EXPORT void free_MKTGswExpSample(MKTGswExpSample* ptr);
EXPORT void free_MKTGswExpSample_array(int32_t nbelts, MKTGswExpSample* ptr);
// initialize the structure
EXPORT void init_MKTGswExpSample(MKTGswExpSample* obj, const TLweParams* RLWEparams, const MKTFHEParams* MKparams);
EXPORT void init_MKTGswExpSample_array(int32_t nbelts, MKTGswExpSample* obj, const TLweParams* RLWEparams, 
        const MKTFHEParams* MKparams);
//destroys the structure
EXPORT void destroy_MKTGswExpSample(MKTGswExpSample* obj);
EXPORT void destroy_MKTGswExpSample_array(int32_t nbelts, MKTGswExpSample* obj);
// new = alloc + init
EXPORT MKTGswExpSample* new_MKTGswExpSample(const TLweParams* RLWEparams, const MKTFHEParams* MKparams);
EXPORT MKTGswExpSample* new_MKTGswExpSample_array(int32_t nbelts, const TLweParams* RLWEparams, 
        const MKTFHEParams* MKparams);
// delete = destroy + free
EXPORT void delete_MKTGswExpSample(MKTGswExpSample* obj);
EXPORT void delete_MKTGswExpSample_array(int32_t nbelts, MKTGswExpSample* obj);






















// MK RGSW Expanded sample FFT: party i C=(y1, ..., d1, ..., yk, c1, d0+x1, ..., d0, ..., d0+xk, c0)
struct MKTGswExpSampleFFT {
    LagrangeHalfCPolynomial *c; ///< array of length 2*(parties+1)*dg
    LagrangeHalfCPolynomial *y;
    LagrangeHalfCPolynomial *c1;
    LagrangeHalfCPolynomial *x;
    LagrangeHalfCPolynomial *c0;
    int32_t party; // party (from 0 to parties-1)
    double current_variance; ///< avg variance of the sample
    const int32_t parties;
    const int32_t dg;
    
#ifdef __cplusplus
    MKTGswExpSampleFFT(const TLweParams* RLWEparams, const MKTFHEParams* MKparams, 
		LagrangeHalfCPolynomial *arr, double current_variance);
    ~MKTGswExpSampleFFT();
    MKTGswExpSampleFFT(const MKTGswExpSampleFFT &) = delete;
    void operator=(const MKTGswExpSampleFFT &) = delete;
#endif
};




// alloc
EXPORT MKTGswExpSampleFFT* alloc_MKTGswExpSampleFFT();
EXPORT MKTGswExpSampleFFT* alloc_MKTGswExpSampleFFT_array(int32_t nbelts);
//free memory space 
EXPORT void free_MKTGswExpSampleFFT(MKTGswExpSampleFFT* ptr);
EXPORT void free_MKTGswExpSampleFFT_array(int32_t nbelts, MKTGswExpSampleFFT* ptr);
// initialize the structure
EXPORT void init_MKTGswExpSampleFFT(MKTGswExpSampleFFT* obj, const TLweParams* RLWEparams, const MKTFHEParams* MKparams, 
        double current_variance);
EXPORT void init_MKTGswExpSampleFFT_array(int32_t nbelts, MKTGswExpSampleFFT* obj, const TLweParams* RLWEparams, 
        const MKTFHEParams* MKparams, double current_variance);
//destroys the structure
EXPORT void destroy_MKTGswExpSampleFFT(MKTGswExpSampleFFT* obj);
EXPORT void destroy_MKTGswExpSampleFFT_array(int32_t nbelts, MKTGswExpSampleFFT* obj);
// new = alloc + init
EXPORT MKTGswExpSampleFFT* new_MKTGswExpSampleFFT(const TLweParams* RLWEparams, const MKTFHEParams* MKparams, 
        double current_variance);
EXPORT MKTGswExpSampleFFT* new_MKTGswExpSampleFFT_array(int32_t nbelts, const TLweParams* RLWEparams, 
        const MKTFHEParams* MKparams, double current_variance);
// delete = destroy + free
EXPORT void delete_MKTGswExpSampleFFT(MKTGswExpSampleFFT* obj);
EXPORT void delete_MKTGswExpSampleFFT_array(int32_t nbelts, MKTGswExpSampleFFT* obj);













#endif //MKTFHESAMPLES_H

