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

#include <stdlib.h>
#include <stdint.h>
#include "rng.h"
#include "sha3.h"
#include "utils.h"
#include <string.h>

void hash( uint8_t digest[DENSE_HASH_LENGTH],
             const char * const m,
             const uint64_t mlen,
             generator_mat_t G_tilde[T]){
    unsigned char *message_buffer = (unsigned char *) malloc( mlen + T*sizeof(generator_mat_t));
    memcpy(message_buffer, G_tilde, T*sizeof(generator_mat_t) );
    memcpy(message_buffer + T*sizeof(generator_mat_t), m, mlen);
    sha3_256(digest,message_buffer, mlen+T*sizeof(generator_mat_t));
    free(message_buffer);
}

/* parses a digest expanding it according to the LESS variant requirements
 * it either expands bitwise the digest or generates a fixed-weight string
 * ELL bit sized elements. Each one of the W non-null ELL bits element may have
 * any value. Currently ELL+log2(N+1) < 16, so extracting 16 b at a time is ok.*/

/* bitmask to rejection-sample numbers modulo PARSED_DIGEST_LEN, to place
 * the fixed number of ones */
#include <stdio.h>
#define MAX_KEYPAIR_INDEX (NUM_KEYPAIRS-1)
#define KEYPAIR_INDEX_MASK ( ((uint16_t)1 << BITS_TO_REPRESENT(MAX_KEYPAIR_INDEX)) -1 )

/* bitmask for rejection sampling of the position */
#define  POSITION_MASK (( (POSITION_T)1 << BITS_TO_REPRESENT(PARSED_DIGEST_LEN))-1)

void parse_digest( uint8_t parsed_digest[PARSED_DIGEST_LEN],
                   const uint8_t digest[DENSE_HASH_LENGTH]){
#if defined(LESSF)
    SHAKE_STATE_STRUCT shake_state;
    xof_shake_init(&shake_state, 128);
    xof_shake_update(&shake_state,
                     (const unsigned char*) digest,
                      DENSE_HASH_LENGTH);
    uint16_t rnd_buf;
    xof_shake_final(&shake_state);

    int placed_elements = 0;
    while (placed_elements < W){
        uint8_t value;
        POSITION_T pos;
        do {
            xof_shake_extract(&shake_state,
                              (unsigned char *) &rnd_buf,
                              sizeof(uint16_t));
            value = rnd_buf & ( ((uint16_t)1 << BITS_TO_REPRESENT(NUM_KEYPAIRS-1)) -1 );
            pos   = rnd_buf >> BITS_TO_REPRESENT(MAX_KEYPAIR_INDEX) ;
            pos   = pos & POSITION_MASK;
        } while ( (value >= NUM_KEYPAIRS) || /* for non-power-of-two keypair numbers */
                  (pos  >= PARSED_DIGEST_LEN) || /* rejection sampling */
                  (parsed_digest[pos] != 0) ); /* skip elements already placed */
        parsed_digest[pos] = value;
        placed_elements +=(value !=0);
    }
#else
#error LESS with variable length digest is not supported
#endif
}
