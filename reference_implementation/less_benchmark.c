/**
 *
 * Reference ISO-C11 Implementation of LESS.
 *
 * @version 1.0 (February 2022)
 *
 * @author Alessandro Barenghi <alessandro.barenghi@polimi.it>
 *
 * This code is hereby placed in the public domain.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ''AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "parameters.h"
#include "timing_and_stat.h"
#include "LESS.h"
#include "rng.h"

void microbench(){
    welford_t timer;
    welford_init(&timer);
    generator_mat_t G;
    generator_rnd(&G);
    uint8_t is_pivot_column[N];
    memset(is_pivot_column,0,sizeof(is_pivot_column));

    uint64_t cycles;
    for(int i = 0; i <100; i++) {
        cycles = x86_64_rtdsc();
        generator_gausselim(&G, is_pivot_column);
        welford_update(&timer,x86_64_rtdsc()-cycles);
    }
    welford_print(timer);
}

void info(){
    fprintf(stderr,"Reference implementation of LESS\n");
    fprintf(stderr,"Code parameters: n= %d, k= %d, q=%d\n", N,K,Q);
    fprintf(stderr,"l = %d, num. keypairs = %ld\n",
            ELL,
            NUM_KEYPAIRS);
}


#define NUM_TESTS 30
void LESS_sign_verify_speed(){
    welford_t timer;
    uint64_t cycles;
    pubkey_t pk;
    prikey_t sk;
    sig_t signature;
    char message[8] = "Signme!";
    info();

    welford_init(&timer);
    for(int i = 0; i <NUM_TESTS; i++) {
        cycles = x86_64_rtdsc();
        LESS_keygen(&sk,&pk);
        welford_update(&timer,(x86_64_rtdsc()-cycles)/1000.0);
    }
    printf("keygen (avg,stddev):");
    welford_print(timer);
    printf("\n");

    welford_init(&timer);
    for(int i = 0; i <NUM_TESTS; i++) {
        cycles = x86_64_rtdsc();
        LESS_sign(&sk,&pk,message,8,&signature);
        welford_update(&timer,(x86_64_rtdsc()-cycles)/1000.0);
    }
    printf("sign (avg,stddev):");
    welford_print(timer);
    printf("\n");

    int is_signature_ok;
    welford_init(&timer);
    for(int i = 0; i <NUM_TESTS; i++) {
        cycles = x86_64_rtdsc();
        is_signature_ok = LESS_verify(&pk,message,8,&signature);
        welford_update(&timer,(x86_64_rtdsc()-cycles)/1000.0);
    }
    printf("verify (avg,stddev):");
    welford_print(timer);
    printf("\n");
    fprintf(stderr,"Keygen-Sign-Verify: %s", is_signature_ok == 1 ? "functional\n": "not functional\n" );
}
int main(int argc, char* argv[]){
    // initialize_pseudo_random_generator_seed(1, "9");
    fprintf(stderr,"LESS reference implementation\n");
    LESS_sign_verify_speed();
//     microbench();
    return 0;
}
