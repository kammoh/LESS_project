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

#include "LESS.h"
#include "utils.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

void LESS_keygen(prikey_t * SK,
                 pubkey_t * PK){
    generator_mat_t tmp_full_G;
    uint8_t is_pivot_column[N];
    /* Find a full rank G: this can be performed only once if
     * multikey attacks are not an issue. */
    do {
        memset(is_pivot_column,0,sizeof(is_pivot_column));
        generator_rnd(&tmp_full_G);
    } while ( generator_gausselim(&tmp_full_G,is_pivot_column) == 0);
    
    /* the first monomial matrix is an identity */
    monomial_mat_id(&SK->private_Q_inv[0]);
    generator_rref_compact(&PK->SF_G_C[0],&tmp_full_G,is_pivot_column);
    
    generator_rref_expand(&tmp_full_G,&PK->SF_G_C[0]);
    for(int i = 1; i < NUM_KEYPAIRS; i++){
        monomial_t private_Q;
        generator_mat_t result_G;
        
        monomial_mat_rnd(&private_Q);
        generator_monomial_mul(&result_G,
                               &tmp_full_G,
                               &private_Q);
        memset(is_pivot_column,0,sizeof(is_pivot_column));
        int rref_ok = generator_gausselim(&result_G,is_pivot_column);
        assert(rref_ok != 0);
        generator_rref_compact(&PK->SF_G_C[i],
                             &result_G,
                             is_pivot_column);       
        monomial_mat_inv(&SK->private_Q_inv[i], &private_Q);
    }
}


void LESS_sign(const prikey_t * SK,
              const pubkey_t * PK,
              const char * const m,
              const uint64_t mlen,
              sig_t * sig) {
    monomial_t Q_tilde[T];
    generator_mat_t G_tilde[T];
    generator_mat_t tmp_full_G;
    uint8_t is_pivot_column[N] = {0};

    generator_rref_expand(&tmp_full_G,&PK->SF_G_C[0]);  
    for(int i = 0; i < T; i++){
           monomial_mat_rnd(&Q_tilde[i]);
           generator_monomial_mul(&G_tilde[i],
                                  &tmp_full_G,
                                  &Q_tilde[i]);
         int rref_ok =  generator_gausselim(&G_tilde[i],is_pivot_column);
         assert(rref_ok != 0);
    }

    hash(sig->digest, m, mlen, G_tilde);

    uint8_t parsed_digest[PARSED_DIGEST_LEN] = {0};
    parse_digest(parsed_digest,sig->digest);

    for(int i = 0; i < T; i++){
       monomial_mat_mul(&sig->mu[i],
                       &SK->private_Q_inv[parsed_digest[i]],
                       &Q_tilde[i]);
    }
}

int timing_safe_memcmp(const unsigned char* a, 
                       const unsigned char* b, 
                       unsigned int bytelen){
    int are_different = 0;
    for (int i =0; i< bytelen; i++){
        are_different |= (a[i] != b[i]);
    }
    return are_different;
}

int LESS_verify(const pubkey_t * const PK,
                const char * const m,
                const uint64_t mlen,
                const sig_t * const sig){
    uint8_t parsed_digest[PARSED_DIGEST_LEN] = {0};
    generator_mat_t G_hat[T] = {0};
    generator_mat_t tmp_full_G;
    uint8_t is_pivot_column[N];
    
    parse_digest(parsed_digest,sig->digest);
    
    for(int i = 0; i < T; i++){
        generator_rref_expand(&tmp_full_G,&PK->SF_G_C[parsed_digest[i]]);  
        
        generator_monomial_mul(&G_hat[i],
                               &tmp_full_G,
                               &sig->mu[i]);
        int is_gausselim_ok;
        is_gausselim_ok = generator_gausselim(&G_hat[i],is_pivot_column);
        assert(is_gausselim_ok);
    }

    uint8_t recomputed_digest[DENSE_HASH_LENGTH] = {0};
    hash(recomputed_digest,m,mlen,G_hat);
    int does_hash_match = 0;
    does_hash_match = (timing_safe_memcmp(recomputed_digest,sig->digest,DENSE_HASH_LENGTH) == 0);
    return does_hash_match;
}
