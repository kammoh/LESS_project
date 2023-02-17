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
#include <inttypes.h>

typedef struct {
    FQ_ELEM values[K][N];
} generator_mat_t;

typedef struct {
    FQ_ELEM values[K][N-K];   /* values of the non-pivot columns */
    POSITION_T column_pos[N-K]; /* positions of the non-pivot columns */   
} rref_generator_mat_t;  

/* multiplies a monomial matrix by a generator matrix */
void generator_monomial_mul(generator_mat_t *res,
                            const generator_mat_t *G,
                            const monomial_t * const monom);
void generator_monomial_mul_in_place(generator_mat_t *G,
                                     const monomial_t * const monom);

/* Computes the row-reduced echelon form of the generator matrix
 * returns 1 on success, 0 on failure, computation is done in-place 
 * Provides the positions of the pivot columns, one-hot encoded in 
 * is_pivot_column*/
int generator_gausselim(generator_mat_t *G,
                        uint8_t is_pivot_column[N]);

/* extracts the last N-K columns from a generator matrix, filling 
 * in the compact RREF representation*/
void generator_rref_compact(rref_generator_mat_t* compact,
                          const generator_mat_t* const full,
                          const uint8_t is_pivot_column[N] );

/* Expands a compact representation of a generator matrix into full form*/
void generator_rref_expand(generator_mat_t * full,
                         const rref_generator_mat_t * const compact);

/* samples a random monomial matrix */
void generator_rnd(generator_mat_t *res);

void generator_pretty_print_name(char* name, const generator_mat_t * const G);
void generator_rref_pretty_print_name(char* name, const rref_generator_mat_t * const G);
