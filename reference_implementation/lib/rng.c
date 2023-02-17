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


#include "rng.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h> // void srand(unsigned int seed); int rand(void); RAND_MAX
#include <string.h> // void *memset(void *s, int c, size_t n);
#include <time.h> // struct timespec; clock_gettime(...); CLOCK_REALTIME
#include "parameters.h"



/******************************************************************************/
/*----------------------------------------------------------------------------*/
/*              start PSEUDO-RAND GENERATOR ROUTINES for rnd.h                */
/*----------------------------------------------------------------------------*/

/* Initializes a CSPRNG from input seed */
SHAKE_STATE_STRUCT platform_csprng_state;

void initialize_prng(SHAKE_STATE_STRUCT *shake_state,
                              const unsigned char * seed) {
   xof_shake_init(shake_state, 128); 
   xof_shake_update(shake_state,seed,SEED_LENGTH_BYTES);
   xof_shake_final(shake_state);
} 

