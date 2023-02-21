#ifndef _PASP_CEXACT
#define _PASP_CEXACT

#include "cdata.h"
#include "cprogram.h"
#include "cinf.h"

typedef struct {
  /* Number of learnable probabilistic facts. */
  size_t n;
  /* Number of learnable annotated disjunctions. */
  size_t m;
  /* Number of models for each learnable probabilistic fact. */
  uint16_t (*F)[2];
  /* Indices of learnable PFs within the global PF array. */
  uint16_t *I_F;
  /* Number of models for each value of each learnable annotated disjunction. */
  uint16_t **A;
  /* Indices of learnable ADs within the global AD array. */
  uint16_t *I_A;
} count_storage_t;

void free_count_storage_contents(count_storage_t *C, bool free_shared);
void free_count_storage(count_storage_t *C);

/* Compute (exactly) query probabilities by exhaustively enumerating all models. */
bool exact_enum(program_t *P, double **R, bool lstable_sat, psemantics_t psem, bool quiet);
/* Count number of models for each learnable probabilistic fact or annotated disjunction. */
bool count_models(program_t *P, bool lstable_sat, count_storage_t *C);

typedef struct {
  /* Probabilities for each learnable PF. */
  double (*F)[2];
  /* Probabilities for each learnable AD. */
  double **A;
  /* Probabilities for each learnable (grounded) NR. */
  double (*NR)[2];
  /* Probabilities for each learnable (grounded) NA. */
  double **NA;
  /* Number of models consistent with observation. */
  uint16_t N;
  /* Probability of observation. */
  double o;
} prob_obs_storage_t;

typedef struct {
  /* Number of learnable probabilistic facts. */
  size_t n;
  /* Number of learnable annotated disjunctions. */
  size_t m;
  /* Number of learnable neural rules. */
  size_t nr;
  /* Number of learnable neural annotated disjunctions. */
  size_t na;
  /* Number of observations. */
  size_t o;
  /* Probabilities for each observation. */
  prob_obs_storage_t *P;
  /* Indices of learnable PFs within the global PF array. */
  uint16_t *I_F;
  /* Indices of learnable ADs within the global AD array. */
  uint16_t *I_A;
  /* Indices of learnable NRs within the global NR array. */
  uint16_t *I_NR;
  /* Indices of learnable NAs within the global NA array. */
  uint16_t *I_NA;
  /* Index values for locating NRs within the total choice bitvector. */
  uint16_t *O_NR;
  /* Index values for locating NAs within the total choice bitvector. */
  uint16_t *O_NA;
} prob_storage_t;

bool init_prob_storage(prob_storage_t *Q, program_t *P, uint16_t *I_F, size_t n_lpf, uint16_t *I_A,
    size_t n_lad, uint16_t *I_NR, size_t n_lnr, uint16_t *I_NA, size_t n_lna, uint16_t *O_NR,
    uint16_t *O_NA, observations_t *O);
/* Note: If Q[0] is zero-initialized, then Q[0].I_A and Q[0].I_F are allocated and dynamically set
 * according to P. However, if they are not NULL, then init_prob_storage_seq reuses found values. */
size_t init_prob_storage_seq(prob_storage_t Q[NUM_PROCS], program_t *P, observations_t *O);
void free_prob_storage_contents(prob_storage_t *Q, bool free_shared);
void free_prob_storage(prob_storage_t *Q);

/* Compute the probability of an observation O (a '\0' terminating const char*), returning the
 * probability ℙ(θ, O) and ℙ(O), where θ covers learnable PFs and ADs. The probabilities are not
 * normalized - e.g. if using the maxent semantic, then these have to be divided by the number of
 * models (i.e. the output of count_models). */
bool prob_obs(program_t *P, observations_t *obs, bool lstables_sat, prob_storage_t *ret, bool derive);
/* Same as prob_obs, but reuse the prob_storage_t's in Q. It's memory safe to assign ret to &Q[0]
 * or NULL; the latter used if the user prefers to access data directly from Q. */
bool prob_obs_reuse(program_t *P, observations_t *obs, bool lstable_sat, prob_storage_t *ret,
    prob_storage_t Q[NUM_PROCS], bool derive);

#endif
