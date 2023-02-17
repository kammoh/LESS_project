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

#include "parameters.h"
#include "monomial_mat.h"
#include "codes.h"

/* in memory key format, the first matrix for the prikey is an identity */
typedef struct {
    rref_generator_mat_t SF_G_C[NUM_KEYPAIRS];
} pubkey_t;

typedef struct {
    monomial_t private_Q_inv[NUM_KEYPAIRS];
} prikey_t;


#ifdef sig_t
#undef sig_t
#endif

typedef struct {
    monomial_t mu[T];
    uint8_t digest[DENSE_HASH_LENGTH]; /* stored as one l-bit value per byte after parsing */
} sig_t_t;
#define sig_t sig_t_t


/* keygen cannot fail */
void LESS_keygen(prikey_t * SK,
                 pubkey_t * PK);

/* sign cannot fail */
void LESS_sign(const prikey_t * SK,
              const pubkey_t * PK,
              const char * const m,
              const uint64_t mlen,
              sig_t * sig);

/* verify returns 1 if signature is ok, 0 otherwise */
int LESS_verify(const pubkey_t * const PK,
                const char * const m,
                const uint64_t mlen,
                const sig_t * const sig);
