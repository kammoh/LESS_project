#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "fq_arith.h"
#include "monomial_mat.h"
#include "timing_and_stat.h"
#include "codes.h"
#include "LESS.h"
#include "rng.h"

#define GRN "\e[0;32m"
#define WHT "\e[0;37m"

void inverse_mod_tester(){
    uint32_t value[Q-1];
    uint32_t inverse[Q-1];
    for(uint32_t i=1; i <= Q-1; i++){
        value[i-1] = i;
        inverse[i-1] = fq_inv(i);
    }
    int all_ok = 1;
    for(uint32_t i=1; i <= Q-1; i++){
        if((value[i-1]*inverse[i-1]) % Q !=1){
           printf("%u*%u=%u\n",
                  value[i-1],
                  inverse[i-1],
                  (value[i-1]*inverse[i-1])%Q);
           all_ok = 0;
        }
    }
    if (all_ok){
        puts("All inverses on F_q OK\n");
    }
}

void monomial_tester(){
    monomial_t mat1, mat2, id;
    monomial_mat_rnd(&mat1);
    monomial_mat_pretty_print(&mat1);
    monomial_mat_inv(&mat2,&mat1);
    monomial_mat_pretty_print(&mat2);
    monomial_mat_mul(&id,&mat2,&mat1);
    monomial_mat_pretty_print(&id);
}

void gausselim_tester(){
    generator_mat_t G,GMul;
    generator_rnd(&G);
    int res;
    generator_pretty_print_name("G", &G);
    uint8_t is_pivot_column[N];
    memset(is_pivot_column,0,sizeof(is_pivot_column));
    res = generator_gausselim(&G, is_pivot_column);
    generator_pretty_print_name("G", &G);
    fprintf(stderr," gausselim is ok : %s\n", res == 1 ? "yes": "no" );
    monomial_t mat1;
    monomial_mat_rnd(&mat1);
    generator_monomial_mul(&GMul,&G,&mat1);
    generator_pretty_print_name("GMul", &GMul);
    memset(is_pivot_column,0,sizeof(is_pivot_column));
    res = generator_gausselim(&GMul, is_pivot_column);
    generator_pretty_print_name("GMul", &GMul);

}

void gen_by_monom_tester(){
    uint8_t is_pivot_column[N];
    memset(is_pivot_column,0,sizeof(is_pivot_column));
     generator_mat_t G = {0}, G2, Gcheck;
     do {
         generator_rnd(&G);
       } while( generator_gausselim(&G, is_pivot_column) ==0 );
    monomial_t mat1, mat2;
    monomial_mat_rnd(&mat1);
    monomial_mat_inv(&mat2,&mat1);
    generator_monomial_mul(&G2,&G,&mat1);
    generator_monomial_mul(&Gcheck,&G2,&mat2);
    if( memcmp( &Gcheck,&G,sizeof(generator_mat_t)) !=0 ){
       generator_pretty_print_name("", &G);
       generator_pretty_print_name("", &G2);
       generator_pretty_print_name("", &Gcheck);
    } else {
        printf("Generator-monomial multiplication: ok\n");
    }
}

void info(){
    fprintf(stderr,"Code parameters: n= %d, k= %d, q=%d\n", N,K,Q);
    fprintf(stderr,"l = %d, num. keypairs = %ld\n",
            ELL,
            NUM_KEYPAIRS);
}


void LESS_sign_verify_test(){
    pubkey_t pk;
    prikey_t sk;
    sig_t signature;
    char message[8] = "Signme!";
    LESS_keygen(&sk,&pk);
    LESS_sign(&sk,&pk,message,sizeof(message),&signature);
    int is_signature_ok;
    is_signature_ok = LESS_verify(&pk,message,8,&signature);
    fprintf(stderr,"Keygen-Sign-Verify: %s", is_signature_ok == 1 ? "functional\n": "not functional\n" );
}

int main(int argc, char* argv[]){
    unsigned char PRNG_SEED[SEED_LENGTH_BYTES] = "1234567890";
    initialize_prng(&platform_csprng_state, PRNG_SEED);
    fprintf(stderr,"LESS reference implementation functional testbench\n");
    info();
    inverse_mod_tester();
    gen_by_monom_tester();
    LESS_sign_verify_test();
    monomial_tester();
    gausselim_tester();

    return 0;
}
