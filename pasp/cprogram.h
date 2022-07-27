#ifndef Py_CPROGRAMMODULE_H
#define Py_CPROGRAMMODULE_H

#ifdef __cplusplus
extern "C" {
#endif 

#include <clingo.h>

typedef struct prob_fact {
  double p;
  const char *f;
  PyObject *f_obj;
  clingo_symbol_t cl_f;
} prob_fact_t;

typedef struct credal_fact {
  double l;
  double u;
  const char *f;
  PyObject *f_obj;
  clingo_symbol_t cl_f;
} credal_fact_t;

typedef struct query {
  clingo_symbol_t *Q;
  bool *Q_s;
  size_t Q_n;
  clingo_symbol_t *E;
  bool *E_s;
  size_t E_n;
} query_t;

typedef struct program {
  const char *P;
  PyObject *P_obj;
  prob_fact_t *PF;
  size_t PF_n;
  query_t *Q;
  size_t Q_n;
  credal_fact_t *CF;
  size_t CF_n;
} program_t;

#define PyCprogram_print_prob_fact_NUM 0
#define PyCprogram_print_prob_fact_RETURN void
#define PyCprogram_print_prob_fact_PROTO (prob_fact_t *pf)

#define PyCprogram_free_prob_fact_contents_NUM 1
#define PyCprogram_free_prob_fact_contents_RETURN void
#define PyCprogram_free_prob_fact_contents_PROTO (prob_fact_t *pf)

#define PyCprogram_free_prob_fact_NUM 2
#define PyCprogram_free_prob_fact_RETURN void
#define PyCprogram_free_prob_fact_PROTO (prob_fact_t *pf)

#define PyCprogram_print_credal_fact_NUM 3
#define PyCprogram_print_credal_fact_RETURN void
#define PyCprogram_print_credal_fact_PROTO (credal_fact_t *cf)

#define PyCprogram_free_credal_fact_contents_NUM 4
#define PyCprogram_free_credal_fact_contents_RETURN void
#define PyCprogram_free_credal_fact_contents_PROTO (credal_fact_t *cf)

#define PyCprogram_free_credal_fact_NUM 5
#define PyCprogram_free_credal_fact_RETURN void
#define PyCprogram_free_credal_fact_PROTO (credal_fact_t *cf)

#define PyCprogram_print_query_NUM 6
#define PyCprogram_print_query_RETURN bool
#define PyCprogram_print_query_PROTO (query_t *Q)

#define PyCprogram_free_query_contents_NUM 7
#define PyCprogram_free_query_contents_RETURN void
#define PyCprogram_free_query_contents_PROTO (query_t *Q)

#define PyCprogram_free_query_NUM 8
#define PyCprogram_free_query_RETURN void
#define PyCprogram_free_query_PROTO (query_t *Q)

#define PyCprogram_print_program_NUM 9
#define PyCprogram_print_program_RETURN void
#define PyCprogram_print_program_PROTO (program_t *P)

#define PyCprogram_free_program_contents_NUM 10
#define PyCprogram_free_program_contents_RETURN void
#define PyCprogram_free_program_contents_PROTO (program_t *P)

#define PyCprogram_free_program_NUM 11
#define PyCprogram_free_program_RETURN void
#define PyCprogram_free_program_PROTO (program_t *P)

#define PyCprogram_from_python_prob_fact_NUM 12
#define PyCprogram_from_python_prob_fact_RETURN bool
#define PyCprogram_from_python_prob_fact_PROTO (PyObject *py_pf, prob_fact_t *pf)

#define PyCprogram_from_python_credal_fact_NUM 13
#define PyCprogram_from_python_credal_fact_RETURN bool
#define PyCprogram_from_python_credal_fact_PROTO (PyObject *py_cf, credal_fact_t *cf)

#define PyCprogram_from_python_query_NUM 14
#define PyCprogram_from_python_query_RETURN bool
#define PyCprogram_from_python_query_PROTO (PyObject *py_q, query_t *q)

#define PyCprogram_from_python_program_NUM 15
#define PyCprogram_from_python_program_RETURN bool
#define PyCprogram_from_python_program_PROTO (PyObject *py_P, program_t *P)

#define PyCprogram_API_pointers 16

#ifdef CPROGRAM_MODULE

static PyCprogram_print_prob_fact_RETURN print_prob_fact PyCprogram_print_prob_fact_PROTO;
static PyCprogram_free_prob_fact_contents_RETURN free_prob_fact_contents PyCprogram_free_prob_fact_contents_PROTO;
static PyCprogram_free_prob_fact_RETURN free_prob_fact PyCprogram_free_prob_fact_PROTO;
static PyCprogram_print_credal_fact_RETURN print_credal_fact PyCprogram_print_credal_fact_PROTO;
static PyCprogram_free_credal_fact_contents_RETURN free_credal_fact_contents PyCprogram_free_credal_fact_contents_PROTO;
static PyCprogram_free_credal_fact_RETURN free_credal_fact PyCprogram_free_credal_fact_PROTO;
static PyCprogram_print_query_RETURN print_query PyCprogram_print_query_PROTO;
static PyCprogram_free_query_contents_RETURN free_query_contents PyCprogram_free_query_contents_PROTO;
static PyCprogram_free_query_RETURN free_query PyCprogram_free_query_PROTO;
static PyCprogram_print_program_RETURN print_program PyCprogram_print_program_PROTO;
static PyCprogram_free_program_contents_RETURN free_program_contents PyCprogram_free_program_contents_PROTO;
static PyCprogram_free_program_RETURN free_program PyCprogram_free_program_PROTO;
static PyCprogram_from_python_prob_fact_RETURN from_python_prob_fact PyCprogram_from_python_prob_fact_PROTO;
static PyCprogram_from_python_credal_fact_RETURN from_python_credal_fact PyCprogram_from_python_credal_fact_PROTO;
static PyCprogram_from_python_query_RETURN from_python_query PyCprogram_from_python_query_PROTO;
static PyCprogram_from_python_program_RETURN from_python_program PyCprogram_from_python_program_PROTO;

#else

static void** PyCprogram_API;

#define print_prob_fact \
  (*(PyCprogram_print_prob_fact_RETURN (*)PyCprogram_print_prob_fact_PROTO) PyCprogram_API[PyCprogram_print_prob_fact_NUM])
#define free_prob_fact_contents \
  (*(PyCprogram_free_prob_fact_contents_RETURN (*)PyCprogram_free_prob_fact_contents_PROTO) PyCprogram_API[PyCprogram_free_prob_fact_contents_NUM])
#define free_prob_fact \
  (*(PyCprogram_free_prob_fact_RETURN (*)PyCprogram_free_prob_fact_PROTO) PyCprogram_API[PyCprogram_free_prob_fact_NUM])
#define print_credal_fact \
  (*(PyCprogram_print_credal_fact_RETURN (*)PyCprogram_print_credal_fact_PROTO) PyCprogram_API[PyCprogram_print_credal_fact_NUM])
#define free_credal_fact_contents \
  (*(PyCprogram_free_credal_fact_contents_RETURN (*)PyCprogram_free_credal_fact_contents_PROTO) PyCprogram_API[PyCprogram_free_credal_fact_contents_NUM])
#define free_credal_fact \
  (*(PyCprogram_free_credal_fact_RETURN (*)PyCprogram_free_credal_fact_PROTO) PyCprogram_API[PyCprogram_free_credal_fact_NUM])
#define print_query \
  (*(PyCprogram_print_query_RETURN (*)PyCprogram_print_query_PROTO) PyCprogram_API[PyCprogram_print_query_NUM])
#define free_query_contents \
  (*(PyCprogram_free_query_contents_RETURN (*)PyCprogram_free_query_contents_PROTO) PyCprogram_API[PyCprogram_free_query_contents_NUM])
#define free_query \
  (*(PyCprogram_free_query_RETURN (*)PyCprogram_free_query_PROTO) PyCprogram_API[PyCprogram_free_query_NUM])
#define print_program \
  (*(PyCprogram_print_program_RETURN (*)PyCprogram_print_program_PROTO) PyCprogram_API[PyCprogram_print_program_NUM])
#define free_program_contents \
  (*(PyCprogram_free_program_contents_RETURN (*)PyCprogram_free_program_contents_PROTO) PyCprogram_API[PyCprogram_free_program_contents_NUM])
#define free_program \
  (*(PyCprogram_free_program_RETURN (*)PyCprogram_free_program_PROTO) PyCprogram_API[PyCprogram_free_program_NUM])
#define from_python_prob_fact \
  (*(PyCprogram_from_python_prob_fact_RETURN (*)PyCprogram_from_python_prob_fact_PROTO) PyCprogram_API[PyCprogram_from_python_prob_fact_NUM])
#define from_python_credal_fact \
  (*(PyCprogram_from_python_credal_fact_RETURN (*)PyCprogram_from_python_credal_fact_PROTO) PyCprogram_API[PyCprogram_from_python_credal_fact_NUM])
#define from_python_query \
  (*(PyCprogram_from_python_query_RETURN (*)PyCprogram_from_python_query_PROTO) PyCprogram_API[PyCprogram_from_python_query_NUM])
#define from_python_program \
  (*(PyCprogram_from_python_program_RETURN (*)PyCprogram_from_python_program_PROTO) PyCprogram_API[PyCprogram_from_python_program_NUM])

static int import_cprogram(void) {
  PyCprogram_API = (void**) PyCapsule_Import("cprogram._C_API", 0);
  return (PyCprogram_API != NULL) ? 0 : -1;
}

#endif

#ifdef __cplusplus
}
#endif

#endif