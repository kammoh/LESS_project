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

#pragma once

#if defined(SHA_3_LIBKECCAK)
#include <libXKCP.a.headers/KeccakHash.h>

// %%%%%%%%%%%%%%%%%%%%%%%%%% SHAKE Wrappers %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#define SHAKE_STATE_STRUCT Keccak_HashInstance
static inline
void xof_shake_init(SHAKE_STATE_STRUCT *state, int val) {
   if (val == 128)
    /* will result in a zero-length output for Keccak_HashFinal */
       Keccak_HashInitialize_SHAKE128(state);
   else
    /* will result in a zero-length output for Keccak_HashFinal */
      Keccak_HashInitialize_SHAKE256(state);
}

static inline
void xof_shake_update(SHAKE_STATE_STRUCT *state,
                      const unsigned char *input,
                      unsigned int inputByteLen) {
   Keccak_HashUpdate(state,
                     (const BitSequence *) input,
                     (BitLength) inputByteLen*8 );
}

static inline
void xof_shake_final(SHAKE_STATE_STRUCT *state) {
   Keccak_HashFinal(state, NULL);
}

static inline
void xof_shake_extract(SHAKE_STATE_STRUCT *state,
                       unsigned char *output,
                       unsigned int outputByteLen) {
   Keccak_HashSqueeze(state,
                      (BitSequence *) output,
                      (BitLength) outputByteLen*8 );
}

// %%%%%%%%%%%%%%%%%%%%%%%%%% SHA-3 Wrappers %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#define SHA3_STATE_STRUCT Keccak_HashInstance
static inline
void sha3_256(unsigned char *output,
              const unsigned char *input,
              unsigned int inputByteLen) {
   SHA3_STATE_STRUCT state;
   Keccak_HashInitialize(&state, 1088,  512, 256, 0x06);
   Keccak_HashUpdate(&state, input, inputByteLen*8);
   Keccak_HashFinal(&state, output);
}

/**
  *  Function to compute SHA3-384 on the input message.
  *  The output length is fixed to 48 bytes.
  */
static inline
void sha3_384(unsigned char *output,
              const unsigned char *input,
              unsigned int inputByteLen) {
   SHA3_STATE_STRUCT state;
   Keccak_HashInitialize(&state, 832,  768, 384, 0x06);
   Keccak_HashUpdate(&state, input, inputByteLen*8);
   Keccak_HashFinal(&state, output);
}

/**
  *  Function to compute SHA3-512 on the input message.
  *  The output length is fixed to 64 bytes.
  */
static inline
void sha3_512(unsigned char *output,
              const unsigned char *input,
              unsigned int inputByteLen) {
   SHA3_STATE_STRUCT state;
   Keccak_HashInitialize(&state, 576,  1024, 512, 0x06);
   Keccak_HashUpdate(&state, input, inputByteLen*8);
   Keccak_HashFinal(&state, output);
}

#else
#include "fips202.h"
#include <assert.h>
#define SHA3_STATE_STRUCT shake256ctx
#define SHAKE_STATE_STRUCT shake128incctx

static inline
void xof_shake_init(SHAKE_STATE_STRUCT *state, int val) {
   if (val == 128){
    /* will result in a zero-length output for Keccak_HashFinal */
       shake128_inc_init(state);
   } else {
   /* when other categories are available, add use of SHAKE 256*/
       assert(0);
   }
}

static inline
void xof_shake_update(SHAKE_STATE_STRUCT *state,
                      const unsigned char *input,
                      unsigned int inputByteLen) {
   shake128_inc_absorb(state, 
                       (const uint8_t *)input, 
                       inputByteLen);
}

static inline
void xof_shake_final(SHAKE_STATE_STRUCT *state) {
    shake128_inc_finalize(state);
}

static inline
void xof_shake_extract(SHAKE_STATE_STRUCT *state,
                       unsigned char *output,
                       unsigned int outputByteLen) {
    shake128_inc_squeeze(output, outputByteLen, state);
}


/* includes */
/* One-stop SHA3-256 shop */
// void sha3_256(uint8_t *output, const uint8_t *input, size_t inlen);
/* One-stop SHA3-384 shop */
// void sha3_384(uint8_t *output, const uint8_t *input, size_t inlen);
/* One-stop SHA3-512 shop */
// void sha3_512(uint8_t *output, const uint8_t *input, size_t inlen);
/* and all the init-absorb-finalize variants for all of them*/

/* Initialize incremental hashing API */
// void shake128_inc_init(shake128incctx *state);
/* Absorb more information into the XOF.
 * Can be called multiple times. */
// void shake128_inc_absorb(shake128incctx *state, const uint8_t *input, size_t inlen);
/* Finalize the XOF for squeezing */
// void shake128_inc_finalize(shake128incctx *state);
/* Squeeze output out of the sponge.
 * Supports being called multiple times */
// void shake128_inc_squeeze(uint8_t *output, size_t outlen, shake128incctx *state);

#endif
