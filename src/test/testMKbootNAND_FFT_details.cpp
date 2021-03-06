#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <sys/time.h>
#include "tfhe.h"
#include "polynomials.h"
#include "lwesamples.h"
#include "lwekey.h"
#include "lweparams.h"
#include "tlwe.h"
#include "tgsw.h"



#include "mkTFHEparams.h"
#include "mkTFHEkeys.h"
#include "mkTFHEkeygen.h"
#include "mkTFHEsamples.h"
#include "mkTFHEfunctions.h"





 

using namespace std;



// **********************************************************************************
// ********************************* MAIN *******************************************
// **********************************************************************************


void dieDramatically(string message) {
    cerr << message << endl;
    abort();
}


//EXPORT void tLweExtractKey(LweKey* result, const TLweKey* key); //TODO: change the name and put in a .h
//EXPORT void tfhe_createLweBootstrappingKeyFFT(LweBootstrappingKeyFFT* bk, const LweKey* key_in, const TGswKey* rgsw_key);
//EXPORT void tfhe_bootstrapFFT(LweSample* result, const LweBootstrappingKeyFFT* bk, Torus32 mu1, Torus32 mu0, const LweSample* x);


void CheckGswPhase(MKTGswExpSample * RGSWSample, int message, MKRLweKey* MKrlwekey, MKTFHEParams* MKparams){
    int parties, dg, Bgbit,N; 
    N = MKparams->N;
    Bgbit = MKparams->Bgbit; 
    dg = MKparams->dg;
    parties = MKparams->parties; 
    TorusPolynomial *sampleExp_decrypt = new_TorusPolynomial_array((parties+1)*dg, N);
    MKtGswEXPSymDecrypt(sampleExp_decrypt, RGSWSample, MKrlwekey);
    for (int i = 0; i < parties; ++i)
    {
        for (int j = 0; j < dg; ++j)
        {
            int32_t MsizeBg = 1 << (Bgbit*(j+1)); // Bg^{j+1}

            double maxerr = 0; 
            for (int h = 0; h < N; ++h)
            {
                //if (modSwitchFromTorus32(sampleExp_decrypt[i*dg+j].coefsT[h], MsizeBg) != sampleUE_clear * MKrlwekey->key[i].key->coefs[h])
                //{
                    //cout << "Error " << i << "," << j << "," << h << " - ";
                    // cout << modSwitchFromTorus32(sampleExp_decrypt[i*dg+j].coefsT[h], MsizeBg) << ";"; 
                    double a = (double) sampleExp_decrypt[i*dg+j].coefsT[h] / pow(2,32);
                    double b = (double) message * MKrlwekey->key[i].key->coefs[h] / (double) MsizeBg;
                    //cout << abs(a-b) << endl;
                    maxerr = max(maxerr, abs(a-b)); 
                //}
            }
            std::cout << "Error " << i << "," << j << " : " << maxerr << endl;
        }
    }
    std::cout << endl;
    // verify that: 
    // phi_parties = phi_b ~ mu
    for (int j = 0; j < dg; ++j)
    {
        int32_t MsizeBg = 1 << (Bgbit*(j+1)); // Bg^{j+1}
        double maxerr = 0; 
        double a = (double) sampleExp_decrypt[parties*dg+j].coefsT[0] / pow(2,32);
        double b = (double) message / (double) MsizeBg;
        maxerr = max(maxerr, abs(a-b)); 
        for (int h = 1; h < N; ++h)
        {
            double a = (double) sampleExp_decrypt[parties*dg+j].coefsT[h] / pow(2,32);
            double b = 0; // msg = 0 for higher coefficients
            maxerr = max(maxerr, abs(a-b));                   
        }
        std::cout << "Error " << parties << "," << j << ":" << maxerr << endl;
    }
    std::cout << endl;
}

        



int32_t main(int32_t argc, char **argv) {

    // Test trials
    const int32_t nb_trials = 100;

    // generate params 
    static const int32_t k = 1;
    //static const int32_t bk_l = 2;
    //static const int32_t bk_Bgbit = 10;
    //static const int32_t ks_basebit = 2;
    //static const int32_t ks_length = 8;
    static const double ks_stdev = 2.44e-5; //standard deviation
    static const double bk_stdev = 3.29e-10; //standard deviation
    static const double max_stdev = 0.012467; //max standard deviation for a 1/4 msg space
    // new params
    static const int32_t n = 500;            // LWE modulus
    static const int32_t n_extract = 1024;    // LWE extract modulus (used in bootstrapping)
    static const int32_t hLWE = 0;         // HW secret key LWE --> not used
    static const double stdevLWE = 0.012467;      // LWE ciphertexts standard deviation
    static const int32_t Bksbit = 2;       // Base bit key switching
    static const int32_t dks = 8;          // dimension key switching
    static const double stdevKS = 2.44e-5;       // KS key standard deviation
    static const int32_t N = 1024;            // RLWE,RGSW modulus
    static const int32_t hRLWE = 0;        // HW secret key RLWE,RGSW --> not used
    static const double stdevRLWEkey = 3.29e-10; // 0; // 0.012467;  // RLWE key standard deviation
    static const double stdevRLWE = 3.29e-10; // 0; // 0.012467;     // RLWE ciphertexts standard deviation
    static const double stdevRGSW = 3.29e-10;     // RGSW ciphertexts standard deviation 
    static const int32_t Bgbit = 7;        // Base bit gadget
    static const int32_t dg = 4;           // dimension gadget
    static const double stdevBK = 3.29e-10;       // BK standard deviation
    static const int32_t parties = 2;      // number of parties

    // 2 parties, B=2^7, d=4
    // 4 parties, B=2^6, d=5
    // 8 parties, B=2^4, d=8
    // noise 3.29e-10, security level 152 by following TFHE analysis

    // params
    LweParams *extractedLWEparams = new_LweParams(n_extract, ks_stdev, max_stdev);
    LweParams *LWEparams = new_LweParams(n, ks_stdev, max_stdev);
    TLweParams *RLWEparams = new_TLweParams(N, k, bk_stdev, max_stdev);
    MKTFHEParams *MKparams = new_MKTFHEParams(n, n_extract, hLWE, stdevLWE, Bksbit, dks, stdevKS, N, 
                            hRLWE, stdevRLWEkey, stdevRLWE, stdevRGSW, Bgbit, dg, stdevBK, parties);


    cout << "Params: DONE!" << endl;






    // Key generation 
    // LWE key
    cout << "Starting KEY GENERATION" << endl;
    clock_t begin_KG = clock();
        
    MKLweKey* MKlwekey = new_MKLweKey(LWEparams, MKparams);
    MKLweKeyGen(MKlwekey);
    cout << "KeyGen MKlwekey: DONE!" << endl;
    // RLWE key 
    MKRLweKey* MKrlwekey = new_MKRLweKey(RLWEparams, MKparams);
    MKRLweKeyGen(MKrlwekey);
    cout << "KeyGen MKrlwekey: DONE!" << endl;
    // LWE key extracted 
    MKLweKey* MKextractedlwekey = new_MKLweKey(extractedLWEparams, MKparams);
    MKtLweExtractKey(MKextractedlwekey, MKrlwekey);
    cout << "KeyGen MKextractedlwekey: DONE!" << endl;
    // bootstrapping + key switching keys
    MKLweBootstrappingKey* MKlweBK = new_MKLweBootstrappingKey(LWEparams, RLWEparams, MKparams);
    MKlweCreateBootstrappingKey(MKlweBK, MKlwekey, MKrlwekey, MKextractedlwekey, 
                                extractedLWEparams, LWEparams, RLWEparams, MKparams);
    cout << "KeyGen MKlweBK: DONE!" << endl;
    // bootstrapping FFT + key switching keys
    MKLweBootstrappingKeyFFT* MKlweBK_FFT = new_MKLweBootstrappingKeyFFT(MKlweBK, LWEparams, RLWEparams, MKparams);
    cout << "KeyGen MKlweBK_FFT: DONE!" << endl;



    // testing expanded BK
    // sample RGSW Expand
    //TorusPolynomial *sampleExp_decrypt = new_TorusPolynomial_array((parties+1)*dg, N);
        
    // Check that the encryption of coefficients of secret key are expanded 
    // Correctly. 
    for (int part = 0; part < parties; ++part)
    {
        for (int key_index = 0; key_index < n; ++key_index)
        {
            cout << "PARTY: " << part << " Key index: " << key_index << endl;
            int message = MKlwekey->key[part].key[key_index]; 
            CheckGswPhase(&MKlweBK->bk[part*n+key_index], message, MKrlwekey, MKparams);
        }
    }
    









    
    clock_t end_KG = clock();
    double time_KG = ((double) end_KG - begin_KG)/CLOCKS_PER_SEC;
    cout << "Finished KEY GENERATION" << endl;
    



    int32_t error_count = 0;

    double argv_time_NAND = 0.0;




    for (int trial = 0; trial < nb_trials; ++trial)
    {
        cout << "****************" << endl;
        cout << "Trial: " << trial << endl;
        cout << "****************" << endl;


        int32_t mess1 = rand() % 2;
        int32_t mess2 = rand() % 2;
        int32_t out = 1 - (mess1 * mess2);
        // generate 2 samples in input
        MKLweSample *test_in1 = new_MKLweSample(LWEparams, MKparams);
        MKLweSample *test_in2 = new_MKLweSample(LWEparams, MKparams);
        MKbootsSymEncrypt(test_in1, mess1, MKlwekey);
        MKbootsSymEncrypt(test_in2, mess2, MKlwekey);
        // generate output sample
        MKLweSample *test_out = new_MKLweSample(LWEparams, MKparams);

        cout << "Encryption: DONE!" << endl;




        // verify encrypt 
        cout << "Message 1: clear = " << mess1 << ", decrypted = " << MKbootsSymDecrypt(test_in1, MKlwekey) << endl;
        cout << "Message 2: clear = " << mess2 << ", decrypted = " << MKbootsSymDecrypt(test_in2, MKlwekey) << endl;






        // evaluate MK bootstrapped NAND 
        cout << "Starting MK bootstrapped NAND: trial " << trial << endl;
        clock_t begin = clock();
        /*
        MKbootsNAND_FFT(test_out, test_in1, test_in2, MKlweBK_FFT, LWEparams, extractedLWEparams, RLWEparams, MKparams);
        */

        // testing in detail
        clock_t begin_boot = clock();
        static const Torus32 MU = modSwitchToTorus32(1, 8);
        MKLweSample *temp_result = new_MKLweSample(LWEparams, MKparams);

        //compute: (0,1/8) - ca - cb
        static const Torus32 NandConst = modSwitchToTorus32(1, 8);
        MKlweNoiselessTrivial(temp_result, NandConst, MKparams);
        MKlweSubTo(temp_result, test_in1, MKparams);
        MKlweSubTo(temp_result, test_in2, MKparams);


        //if the phase is positive, the result is 1/8
        //if the phase is positive, else the result is -1/8
        MKLweSample *u = new_MKLweSample(extractedLWEparams, MKparams);



        TorusPolynomial *testvect = new_TorusPolynomial(N);
        int32_t *bara = new int32_t[parties*n];

        // b*2N
        int32_t barb = modSwitchFromTorus32(temp_result->b, 2*N);
        // a*2N
        for (int i = 0; i < parties; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                bara[n*i+j] = modSwitchFromTorus32(temp_result->a[n*i+j], 2*N);
            }
        }
        
        //the initial testvec = [mu,mu,mu,...,mu]
        for (int32_t i = 0; i < N; i++) 
        {
            testvect->coefsT[i] = MU;
        }




        
        TorusPolynomial *testvectbis = new_TorusPolynomial(N);
        MKTLweSample *acc = new_MKTLweSample(RLWEparams, MKparams);

        if (barb !=0)
        {
            torusPolynomialMulByXai(testvectbis, 2*N - barb, testvect);
        }
        else
        {
            torusPolynomialCopy(testvectbis, testvect);
        }

        MKtLweNoiselessTrivial(acc, testvectbis, MKparams);


        cout << endl;
        cout << "before blind rotate: [";

        TorusPolynomial *phase0 = new_TorusPolynomial(N);
        MKtLwePhase(phase0, acc, MKrlwekey);

        for (int i = 0; i < N-1; ++i)
        {
            cout << (phase0->coefsT[i] > 0 ? 1 : 0) << ", " ;
        }
        cout << (phase0->coefsT[N-1] > 0 ? 1 : 0) << "]" << endl;



        MKtfhe_blindRotateFFT(acc, MKlweBK_FFT->bkFFT, bara, RLWEparams, MKparams, MKrlwekey);
        cout << endl;
        cout << "after blind rotate: [";

        TorusPolynomial *phase = new_TorusPolynomial(N);
        MKtLwePhase(phase, acc, MKrlwekey);

        for (int i = 0; i < N-1; ++i)
        {
            cout << (phase->coefsT[i] > 0 ? 1 : 0) << ", " ;
        }
        cout << (phase->coefsT[N-1] > 0 ? 1 : 0) << "]" << endl;



        cout << endl;





        MKtLweExtractMKLweSample(u, acc, MKparams);

        delete_MKTLweSample(acc);
        delete_TorusPolynomial(testvectbis);













        delete[] bara;
        delete_TorusPolynomial(testvect);
        clock_t end_boot = clock();
        double time_boot = ((double) end_boot - begin_boot)/CLOCKS_PER_SEC;
        cout << "Finished BOOT" << endl;
        cout << "Time per BOOT (seconds)... " << time_boot << endl;


        cout << endl;
        cout << "after bootstrap_woKSFFT: " << MKbootsSymDecrypt(u, MKextractedlwekey) << " vs " << out << endl;
        cout << endl;
        // MK Key Switching
        clock_t begin_KS = clock();
        MKlweKeySwitch(test_out, MKlweBK_FFT->ks, u, LWEparams, MKparams);


        clock_t end_KS = clock();
        double time_KS = ((double) end_KS - begin_KS)/CLOCKS_PER_SEC;
        cout << "Finished KS" << endl;
        cout << "Time per KS (seconds)... " << time_KS << endl;

        cout << endl;
        cout << "after KS: " << MKbootsSymDecrypt(u, MKextractedlwekey) << " vs " << MKbootsSymDecrypt(test_out, MKlwekey) << endl;
        cout << endl;

        delete_MKLweSample(u);
        delete_MKLweSample(temp_result);
        

        // testing in detail




        clock_t end = clock();
        double time = ((double) end - begin)/CLOCKS_PER_SEC;
        cout << "Finished MK bootstrapped" << endl;
        cout << "Time per MKbootNAND gate (seconds)... " << time << endl;

        argv_time_NAND += time;






        // verify NAND
        int32_t outNAND = MKbootsSymDecrypt(test_out, MKlwekey);
        cout << "NAND: clear = " << out << ", decrypted = " << outNAND << endl;
        if (outNAND != out) {
            error_count +=1;
            cout << "ERROR!!! " << trial << "," << trial << " - ";
            cout << t32tod(MKlwePhase(test_in1, MKlwekey)) << " - ";
            cout << t32tod(MKlwePhase(test_in2, MKlwekey)) << " - ";
            cout << t32tod(MKlwePhase(test_out, MKlwekey)) << endl;
        }






        // delete samples
        delete_MKLweSample(test_out);
        delete_MKLweSample(test_in2);
        delete_MKLweSample(test_in1);
    }

    cout << endl;
    cout << "Time per KEY GENERATION (seconds)... " << time_KG << endl;
    cout << "ERRORS: " << error_count << " over " << nb_trials << " tests!" << endl;
    cout << "Average time per bootNAND: " << argv_time_NAND/nb_trials << " seconds" << endl;

   

    // delete keys
    delete_MKLweBootstrappingKeyFFT(MKlweBK_FFT);
    delete_MKLweBootstrappingKey(MKlweBK);
    delete_MKLweKey(MKextractedlwekey);
    delete_MKRLweKey(MKrlwekey);
    delete_MKLweKey(MKlwekey);
    // delete params
    delete_MKTFHEParams(MKparams);
    delete_TLweParams(RLWEparams);
    delete_LweParams(LWEparams);
    delete_LweParams(extractedLWEparams);

    return 0;
}
