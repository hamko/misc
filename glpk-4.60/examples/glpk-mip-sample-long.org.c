#include <stdlib.h>
#include <stdio.h>
//#include <glpk.h>

/* glpk.h (GLPK API) */

/***********************************************************************
*  This code is part of GLPK (GNU Linear Programming Kit).
*
*  Copyright (C) 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008,
*  2009, 2010, 2011, 2013, 2014, 2015, 2016 Andrew Makhorin, Department
*  for Applied Informatics, Moscow Aviation Institute, Moscow, Russia.
*  All rights reserved. E-mail: <mao@gnu.org>.
*
*  GLPK is free software: you can redistribute it and/or modify it
*  under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  GLPK is distributed in the hope that it will be useful, but WITHOUT
*  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
*  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public
*  License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with GLPK. If not, see <http://www.gnu.org/licenses/>.
***********************************************************************/

#ifndef GLPK_H
#define GLPK_H

#include <stdarg.h>
#include <stddef.h>

typedef struct glp_prob glp_prob;
#ifdef __cplusplus
extern "C" {
#endif

/* library version numbers: */
#define GLP_MAJOR_VERSION  4
#define GLP_MINOR_VERSION  60

/* LP/MIP problem object */

/* optimization direction flag: */
#define GLP_MIN            1  /* minimization */
#define GLP_MAX            2  /* maximization */

/* kind of structural variable: */
#define GLP_CV             1  /* continuous variable */
#define GLP_IV             2  /* integer variable */
#define GLP_BV             3  /* binary variable */

/* type of auxiliary/structural variable: */
#define GLP_FR             1  /* free (unbounded) variable */
#define GLP_LO             2  /* variable with lower bound */
#define GLP_UP             3  /* variable with upper bound */
#define GLP_DB             4  /* double-bounded variable */
#define GLP_FX             5  /* fixed variable */

/* status of auxiliary/structural variable: */
#define GLP_BS             1  /* basic variable */
#define GLP_NL             2  /* non-basic variable on lower bound */
#define GLP_NU             3  /* non-basic variable on upper bound */
#define GLP_NF             4  /* non-basic free (unbounded) variable */
#define GLP_NS             5  /* non-basic fixed variable */

/* scaling options: */
#define GLP_SF_GM       0x01  /* perform geometric mean scaling */
#define GLP_SF_EQ       0x10  /* perform equilibration scaling */
#define GLP_SF_2N       0x20  /* round scale factors to power of two */
#define GLP_SF_SKIP     0x40  /* skip if problem is well scaled */
#define GLP_SF_AUTO     0x80  /* choose scaling options automatically */

/* solution indicator: */
#define GLP_SOL            1  /* basic solution */
#define GLP_IPT            2  /* interior-point solution */
#define GLP_MIP            3  /* mixed integer solution */

/* solution status: */
#define GLP_UNDEF          1  /* solution is undefined */
#define GLP_FEAS           2  /* solution is feasible */
#define GLP_INFEAS         3  /* solution is infeasible */
#define GLP_NOFEAS         4  /* no feasible solution exists */
#define GLP_OPT            5  /* solution is optimal */
#define GLP_UNBND          6  /* solution is unbounded */

typedef struct
{     /* basis factorization control parameters */
      int msg_lev;            /* (not used) */
      int type;               /* factorization type: */
#if 1 /* 05/III-2014 */
#define GLP_BF_LUF      0x00  /* plain LU-factorization */
#define GLP_BF_BTF      0x10  /* block triangular LU-factorization */
#endif
#define GLP_BF_FT       0x01  /* Forrest-Tomlin (LUF only) */
#define GLP_BF_BG       0x02  /* Schur compl. + Bartels-Golub */
#define GLP_BF_GR       0x03  /* Schur compl. + Givens rotation */
      int lu_size;            /* (not used) */
      double piv_tol;         /* sgf_piv_tol */
      int piv_lim;            /* sgf_piv_lim */
      int suhl;               /* sgf_suhl */
      double eps_tol;         /* sgf_eps_tol */
      double max_gro;         /* (not used) */
      int nfs_max;            /* fhvint.nfs_max */
      double upd_tol;         /* (not used) */
      int nrs_max;            /* scfint.nn_max */
      int rs_size;            /* (not used) */
      double foo_bar[38];     /* (reserved) */
} glp_bfcp;

typedef struct
{     /* simplex method control parameters */
      int msg_lev;            /* message level: */
#define GLP_MSG_OFF        0  /* no output */
#define GLP_MSG_ERR        1  /* warning and error messages only */
#define GLP_MSG_ON         2  /* normal output */
#define GLP_MSG_ALL        3  /* full output */
#define GLP_MSG_DBG        4  /* debug output */
      int meth;               /* simplex method option: */
#define GLP_PRIMAL         1  /* use primal simplex */
#define GLP_DUALP          2  /* use dual; if it fails, use primal */
#define GLP_DUAL           3  /* use dual simplex */
      int pricing;            /* pricing technique: */
#define GLP_PT_STD      0x11  /* standard (Dantzig's rule) */
#define GLP_PT_PSE      0x22  /* projected steepest edge */
      int r_test;             /* ratio test technique: */
#define GLP_RT_STD      0x11  /* standard (textbook) */
#define GLP_RT_HAR      0x22  /* Harris' two-pass ratio test */
#if 1 /* 16/III-2016 */
#define GLP_RT_FLIP     0x33  /* long-step (flip-flop) ratio test */
#endif
      double tol_bnd;         /* spx.tol_bnd */
      double tol_dj;          /* spx.tol_dj */
      double tol_piv;         /* spx.tol_piv */
      double obj_ll;          /* spx.obj_ll */
      double obj_ul;          /* spx.obj_ul */
      int it_lim;             /* spx.it_lim */
      int tm_lim;             /* spx.tm_lim (milliseconds) */
      int out_frq;            /* spx.out_frq */
      int out_dly;            /* spx.out_dly (milliseconds) */
      int presolve;           /* enable/disable using LP presolver */
      double foo_bar[36];     /* (reserved) */
} glp_smcp;

typedef struct
{     /* interior-point solver control parameters */
      int msg_lev;            /* message level (see glp_smcp) */
      int ord_alg;            /* ordering algorithm: */
#define GLP_ORD_NONE       0  /* natural (original) ordering */
#define GLP_ORD_QMD        1  /* quotient minimum degree (QMD) */
#define GLP_ORD_AMD        2  /* approx. minimum degree (AMD) */
#define GLP_ORD_SYMAMD     3  /* approx. minimum degree (SYMAMD) */
      double foo_bar[48];     /* (reserved) */
} glp_iptcp;

typedef struct glp_tree glp_tree;
/* branch-and-bound tree */

typedef struct
{     /* integer optimizer control parameters */
      int msg_lev;            /* message level (see glp_smcp) */
      int br_tech;            /* branching technique: */
#define GLP_BR_FFV         1  /* first fractional variable */
#define GLP_BR_LFV         2  /* last fractional variable */
#define GLP_BR_MFV         3  /* most fractional variable */
#define GLP_BR_DTH         4  /* heuristic by Driebeck and Tomlin */
#define GLP_BR_PCH         5  /* hybrid pseudocost heuristic */
      int bt_tech;            /* backtracking technique: */
#define GLP_BT_DFS         1  /* depth first search */
#define GLP_BT_BFS         2  /* breadth first search */
#define GLP_BT_BLB         3  /* best local bound */
#define GLP_BT_BPH         4  /* best projection heuristic */
      double tol_int;         /* mip.tol_int */
      double tol_obj;         /* mip.tol_obj */
      int tm_lim;             /* mip.tm_lim (milliseconds) */
      int out_frq;            /* mip.out_frq (milliseconds) */
      int out_dly;            /* mip.out_dly (milliseconds) */
      void (*cb_func)(glp_tree *T, void *info);
                              /* mip.cb_func */
      void *cb_info;          /* mip.cb_info */
      int cb_size;            /* mip.cb_size */
      int pp_tech;            /* preprocessing technique: */
#define GLP_PP_NONE        0  /* disable preprocessing */
#define GLP_PP_ROOT        1  /* preprocessing only on root level */
#define GLP_PP_ALL         2  /* preprocessing on all levels */
      double mip_gap;         /* relative MIP gap tolerance */
      int mir_cuts;           /* MIR cuts       (GLP_ON/GLP_OFF) */
      int gmi_cuts;           /* Gomory's cuts  (GLP_ON/GLP_OFF) */
      int cov_cuts;           /* cover cuts     (GLP_ON/GLP_OFF) */
      int clq_cuts;           /* clique cuts    (GLP_ON/GLP_OFF) */
      int presolve;           /* enable/disable using MIP presolver */
      int binarize;           /* try to binarize integer variables */
      int fp_heur;            /* feasibility pump heuristic */
      int ps_heur;            /* proximity search heuristic */
      int ps_tm_lim;          /* proxy time limit, milliseconds */
      int sr_heur;            /* simple rounding heuristic */
#if 1 /* 24/X-2015; not documented--should not be used */
      int use_sol;            /* use existing solution */
      const char *save_sol;   /* filename to save every new solution */
      int alien;              /* use alien solver */
#endif
#if 1 /* 16/III-2016; not documented--should not be used */
      int flip;               /* use long-step dual simplex */
#endif
      double foo_bar[23];     /* (reserved) */
} glp_iocp;

typedef struct
{     /* additional row attributes */
      int level;
      /* subproblem level at which the row was added */
      int origin;
      /* row origin flag: */
#define GLP_RF_REG         0  /* regular constraint */
#define GLP_RF_LAZY        1  /* "lazy" constraint */
#define GLP_RF_CUT         2  /* cutting plane constraint */
      int klass;
      /* row class descriptor: */
#define GLP_RF_GMI         1  /* Gomory's mixed integer cut */
#define GLP_RF_MIR         2  /* mixed integer rounding cut */
#define GLP_RF_COV         3  /* mixed cover cut */
#define GLP_RF_CLQ         4  /* clique cut */
      double foo_bar[7];
      /* (reserved) */
} glp_attr;

/* enable/disable flag: */
#define GLP_ON             1  /* enable something */
#define GLP_OFF            0  /* disable something */

/* reason codes: */
#define GLP_IROWGEN     0x01  /* request for row generation */
#define GLP_IBINGO      0x02  /* better integer solution found */
#define GLP_IHEUR       0x03  /* request for heuristic solution */
#define GLP_ICUTGEN     0x04  /* request for cut generation */
#define GLP_IBRANCH     0x05  /* request for branching */
#define GLP_ISELECT     0x06  /* request for subproblem selection */
#define GLP_IPREPRO     0x07  /* request for preprocessing */

/* branch selection indicator: */
#define GLP_NO_BRNCH       0  /* select no branch */
#define GLP_DN_BRNCH       1  /* select down-branch */
#define GLP_UP_BRNCH       2  /* select up-branch */

/* return codes: */
#define GLP_EBADB       0x01  /* invalid basis */
#define GLP_ESING       0x02  /* singular matrix */
#define GLP_ECOND       0x03  /* ill-conditioned matrix */
#define GLP_EBOUND      0x04  /* invalid bounds */
#define GLP_EFAIL       0x05  /* solver failed */
#define GLP_EOBJLL      0x06  /* objective lower limit reached */
#define GLP_EOBJUL      0x07  /* objective upper limit reached */
#define GLP_EITLIM      0x08  /* iteration limit exceeded */
#define GLP_ETMLIM      0x09  /* time limit exceeded */
#define GLP_ENOPFS      0x0A  /* no primal feasible solution */
#define GLP_ENODFS      0x0B  /* no dual feasible solution */
#define GLP_EROOT       0x0C  /* root LP optimum not provided */
#define GLP_ESTOP       0x0D  /* search terminated by application */
#define GLP_EMIPGAP     0x0E  /* relative mip gap tolerance reached */
#define GLP_ENOFEAS     0x0F  /* no primal/dual feasible solution */
#define GLP_ENOCVG      0x10  /* no convergence */
#define GLP_EINSTAB     0x11  /* numerical instability */
#define GLP_EDATA       0x12  /* invalid data */
#define GLP_ERANGE      0x13  /* result out of range */

/* condition indicator: */
#define GLP_KKT_PE         1  /* primal equalities */
#define GLP_KKT_PB         2  /* primal bounds */
#define GLP_KKT_DE         3  /* dual equalities */
#define GLP_KKT_DB         4  /* dual bounds */
#define GLP_KKT_CS         5  /* complementary slackness */

/* MPS file format: */
#define GLP_MPS_DECK       1  /* fixed (ancient) */
#define GLP_MPS_FILE       2  /* free (modern) */

typedef struct
{     /* MPS format control parameters */
      int blank;
      /* character code to replace blanks in symbolic names */
      char *obj_name;
      /* objective row name */
      double tol_mps;
      /* zero tolerance for MPS data */
      double foo_bar[17];
      /* (reserved for use in the future) */
} glp_mpscp;

typedef struct
{     /* CPLEX LP format control parameters */
      double foo_bar[20];
      /* (reserved for use in the future) */
} glp_cpxcp;

typedef struct glp_tran glp_tran;
/* MathProg translator workspace */

glp_prob *glp_create_prob(void); // <-------------------------------
/* create problem object */

void glp_set_prob_name(glp_prob *P, const char *name); // <-------------------------------
/* assign (change) problem name */

void glp_set_obj_name(glp_prob *P, const char *name);
/* assign (change) objective function name */

void glp_set_obj_dir(glp_prob *P, int dir);
/* set (change) optimization direction flag */

int glp_add_rows(glp_prob *P, int nrs); // <-------------------------------
/* add new rows to problem object */

int glp_add_cols(glp_prob *P, int ncs); // <-------------------------------
/* add new columns to problem object */

void glp_set_row_name(glp_prob *P, int i, const char *name); // <-------------------------------
/* assign (change) row name */

void glp_set_col_name(glp_prob *P, int j, const char *name); // <-------------------------------
/* assign (change) column name */

void glp_set_row_bnds(glp_prob *P, int i, int type, double lb, // <-------------------------------
      double ub);
/* set (change) row bounds */

void glp_set_col_bnds(glp_prob *P, int j, int type, double lb, // <-------------------------------
      double ub);
/* set (change) column bounds */

void glp_set_obj_coef(glp_prob *P, int j, double coef);
/* set (change) obj. coefficient or constant term */

void glp_set_mat_row(glp_prob *P, int i, int len, const int ind[],
      const double val[]);
/* set (replace) row of the constraint matrix */

void glp_set_mat_col(glp_prob *P, int j, int len, const int ind[],
      const double val[]);
/* set (replace) column of the constraint matrix */

void glp_load_matrix(glp_prob *P, int ne, const int ia[], // <-------------------------------
      const int ja[], const double ar[]);
/* load (replace) the whole constraint matrix */

int glp_check_dup(int m, int n, int ne, const int ia[], const int ja[]);
/* check for duplicate elements in sparse matrix */

void glp_sort_matrix(glp_prob *P);
/* sort elements of the constraint matrix */

void glp_del_rows(glp_prob *P, int nrs, const int num[]);
/* delete specified rows from problem object */

void glp_del_cols(glp_prob *P, int ncs, const int num[]);
/* delete specified columns from problem object */

void glp_copy_prob(glp_prob *dest, glp_prob *prob, int names);
/* copy problem object content */

void glp_erase_prob(glp_prob *P);
/* erase problem object content */

void glp_delete_prob(glp_prob *P);
/* delete problem object */

const char *glp_get_prob_name(glp_prob *P);
/* retrieve problem name */

const char *glp_get_obj_name(glp_prob *P);
/* retrieve objective function name */

int glp_get_obj_dir(glp_prob *P);
/* retrieve optimization direction flag */

int glp_get_num_rows(glp_prob *P);
/* retrieve number of rows */

int glp_get_num_cols(glp_prob *P);
/* retrieve number of columns */

const char *glp_get_row_name(glp_prob *P, int i);
/* retrieve row name */

const char *glp_get_col_name(glp_prob *P, int j);
/* retrieve column name */

int glp_get_row_type(glp_prob *P, int i);
/* retrieve row type */

double glp_get_row_lb(glp_prob *P, int i);
/* retrieve row lower bound */

double glp_get_row_ub(glp_prob *P, int i);
/* retrieve row upper bound */

int glp_get_col_type(glp_prob *P, int j);
/* retrieve column type */

double glp_get_col_lb(glp_prob *P, int j);
/* retrieve column lower bound */

double glp_get_col_ub(glp_prob *P, int j);
/* retrieve column upper bound */

double glp_get_obj_coef(glp_prob *P, int j);
/* retrieve obj. coefficient or constant term */

int glp_get_num_nz(glp_prob *P);
/* retrieve number of constraint coefficients */

int glp_get_mat_row(glp_prob *P, int i, int ind[], double val[]);
/* retrieve row of the constraint matrix */

int glp_get_mat_col(glp_prob *P, int j, int ind[], double val[]);
/* retrieve column of the constraint matrix */

void glp_create_index(glp_prob *P);
/* create the name index */

int glp_find_row(glp_prob *P, const char *name);
/* find row by its name */

int glp_find_col(glp_prob *P, const char *name);
/* find column by its name */

void glp_delete_index(glp_prob *P);
/* delete the name index */

void glp_set_rii(glp_prob *P, int i, double rii);
/* set (change) row scale factor */

void glp_set_sjj(glp_prob *P, int j, double sjj);
/* set (change) column scale factor */

double glp_get_rii(glp_prob *P, int i);
/* retrieve row scale factor */

double glp_get_sjj(glp_prob *P, int j);
/* retrieve column scale factor */

void glp_scale_prob(glp_prob *P, int flags);
/* scale problem data */

void glp_unscale_prob(glp_prob *P);
/* unscale problem data */

void glp_set_row_stat(glp_prob *P, int i, int stat);
/* set (change) row status */

void glp_set_col_stat(glp_prob *P, int j, int stat);
/* set (change) column status */

void glp_std_basis(glp_prob *P);
/* construct standard initial LP basis */

void glp_adv_basis(glp_prob *P, int flags);
/* construct advanced initial LP basis */

void glp_cpx_basis(glp_prob *P);
/* construct Bixby's initial LP basis */

int glp_simplex(glp_prob *P, const glp_smcp *parm);
/* solve LP problem with the simplex method */

int glp_exact(glp_prob *P, const glp_smcp *parm);
/* solve LP problem in exact arithmetic */

void glp_init_smcp(glp_smcp *parm);
/* initialize simplex method control parameters */

int glp_get_status(glp_prob *P);
/* retrieve generic status of basic solution */

int glp_get_prim_stat(glp_prob *P);
/* retrieve status of primal basic solution */

int glp_get_dual_stat(glp_prob *P);
/* retrieve status of dual basic solution */

double glp_get_obj_val(glp_prob *P); // <-------------------------------
/* retrieve objective value (basic solution) */

int glp_get_row_stat(glp_prob *P, int i);
/* retrieve row status */

double glp_get_row_prim(glp_prob *P, int i);
/* retrieve row primal value (basic solution) */

double glp_get_row_dual(glp_prob *P, int i);
/* retrieve row dual value (basic solution) */

int glp_get_col_stat(glp_prob *P, int j);
/* retrieve column status */

double glp_get_col_prim(glp_prob *P, int j);
/* retrieve column primal value (basic solution) */

double glp_get_col_dual(glp_prob *P, int j);
/* retrieve column dual value (basic solution) */

int glp_get_unbnd_ray(glp_prob *P);
/* determine variable causing unboundedness */

#if 1 /* 08/VIII-2013; not documented yet */
int glp_get_it_cnt(glp_prob *P);
/* get simplex solver iteration count */
#endif

#if 1 /* 08/VIII-2013; not documented yet */
void glp_set_it_cnt(glp_prob *P, int it_cnt);
/* set simplex solver iteration count */
#endif

int glp_interior(glp_prob *P, const glp_iptcp *parm);
/* solve LP problem with the interior-point method */

void glp_init_iptcp(glp_iptcp *parm);
/* initialize interior-point solver control parameters */

int glp_ipt_status(glp_prob *P);
/* retrieve status of interior-point solution */

double glp_ipt_obj_val(glp_prob *P);
/* retrieve objective value (interior point) */

double glp_ipt_row_prim(glp_prob *P, int i);
/* retrieve row primal value (interior point) */

double glp_ipt_row_dual(glp_prob *P, int i);
/* retrieve row dual value (interior point) */

double glp_ipt_col_prim(glp_prob *P, int j);
/* retrieve column primal value (interior point) */

double glp_ipt_col_dual(glp_prob *P, int j);
/* retrieve column dual value (interior point) */

void glp_set_col_kind(glp_prob *P, int j, int kind);
/* set (change) column kind */

int glp_get_col_kind(glp_prob *P, int j);
/* retrieve column kind */

int glp_get_num_int(glp_prob *P);
/* retrieve number of integer columns */

int glp_get_num_bin(glp_prob *P);
/* retrieve number of binary columns */

int glp_intopt(glp_prob *P, const glp_iocp *parm); // <-------------------------------
/* solve MIP problem with the branch-and-bound method */

void glp_init_iocp(glp_iocp *parm); // <-------------------------------
/* initialize integer optimizer control parameters */

int glp_mip_status(glp_prob *P);
/* retrieve status of MIP solution */

double glp_mip_obj_val(glp_prob *P);
/* retrieve objective value (MIP solution) */

double glp_mip_row_val(glp_prob *P, int i); // <-------------------------------
/* retrieve row value (MIP solution) */

double glp_mip_col_val(glp_prob *P, int j); // <-------------------------------
/* retrieve column value (MIP solution) */

#if 0
void glp_check_kkt(glp_prob *P, int sol, int cond, double *ae_max,
      int *ae_ind, double *re_max, int *re_ind);
/* check feasibility/optimality conditions */

int glp_print_sol(glp_prob *P, const char *fname);
/* write basic solution in printable format */

int glp_read_sol(glp_prob *P, const char *fname);
/* read basic solution from text file */

int glp_write_sol(glp_prob *P, const char *fname);
/* write basic solution to text file */

int glp_print_ranges(glp_prob *P, int len, const int list[],
      int flags, const char *fname);
/* print sensitivity analysis report */

int glp_print_ipt(glp_prob *P, const char *fname);
/* write interior-point solution in printable format */

int glp_read_ipt(glp_prob *P, const char *fname);
/* read interior-point solution from text file */

int glp_write_ipt(glp_prob *P, const char *fname);
/* write interior-point solution to text file */

int glp_print_mip(glp_prob *P, const char *fname);
/* write MIP solution in printable format */

int glp_read_mip(glp_prob *P, const char *fname);
/* read MIP solution from text file */

int glp_write_mip(glp_prob *P, const char *fname);
/* write MIP solution to text file */

int glp_bf_exists(glp_prob *P);
/* check if LP basis factorization exists */

int glp_factorize(glp_prob *P);
/* compute LP basis factorization */

int glp_bf_updated(glp_prob *P);
/* check if LP basis factorization has been updated */

void glp_get_bfcp(glp_prob *P, glp_bfcp *parm);
/* retrieve LP basis factorization control parameters */

void glp_set_bfcp(glp_prob *P, const glp_bfcp *parm);
/* change LP basis factorization control parameters */

int glp_get_bhead(glp_prob *P, int k);
/* retrieve LP basis header information */

int glp_get_row_bind(glp_prob *P, int i);
/* retrieve row index in the basis header */

int glp_get_col_bind(glp_prob *P, int j);
/* retrieve column index in the basis header */

void glp_ftran(glp_prob *P, double x[]);
/* perform forward transformation (solve system B*x = b) */

void glp_btran(glp_prob *P, double x[]);
/* perform backward transformation (solve system B'*x = b) */

int glp_warm_up(glp_prob *P);
/* "warm up" LP basis */

int glp_eval_tab_row(glp_prob *P, int k, int ind[], double val[]);
/* compute row of the simplex tableau */

int glp_eval_tab_col(glp_prob *P, int k, int ind[], double val[]);
/* compute column of the simplex tableau */

int glp_transform_row(glp_prob *P, int len, int ind[], double val[]);
/* transform explicitly specified row */

int glp_transform_col(glp_prob *P, int len, int ind[], double val[]);
/* transform explicitly specified column */

int glp_prim_rtest(glp_prob *P, int len, const int ind[],
      const double val[], int dir, double eps);
/* perform primal ratio test */

int glp_dual_rtest(glp_prob *P, int len, const int ind[],
      const double val[], int dir, double eps);
/* perform dual ratio test */

void glp_analyze_bound(glp_prob *P, int k, double *value1, int *var1,
      double *value2, int *var2);
/* analyze active bound of non-basic variable */

void glp_analyze_coef(glp_prob *P, int k, double *coef1, int *var1,
      double *value1, double *coef2, int *var2, double *value2);
/* analyze objective coefficient at basic variable */

int glp_ios_reason(glp_tree *T);
/* determine reason for calling the callback routine */

glp_prob *glp_ios_get_prob(glp_tree *T);
/* access the problem object */

void glp_ios_tree_size(glp_tree *T, int *a_cnt, int *n_cnt,
      int *t_cnt);
/* determine size of the branch-and-bound tree */

int glp_ios_curr_node(glp_tree *T);
/* determine current active subproblem */

int glp_ios_next_node(glp_tree *T, int p);
/* determine next active subproblem */

int glp_ios_prev_node(glp_tree *T, int p);
/* determine previous active subproblem */

int glp_ios_up_node(glp_tree *T, int p);
/* determine parent subproblem */

int glp_ios_node_level(glp_tree *T, int p);
/* determine subproblem level */

double glp_ios_node_bound(glp_tree *T, int p);
/* determine subproblem local bound */

int glp_ios_best_node(glp_tree *T);
/* find active subproblem with best local bound */

double glp_ios_mip_gap(glp_tree *T);
/* compute relative MIP gap */

void *glp_ios_node_data(glp_tree *T, int p);
/* access subproblem application-specific data */

void glp_ios_row_attr(glp_tree *T, int i, glp_attr *attr);
/* retrieve additional row attributes */

int glp_ios_pool_size(glp_tree *T);
/* determine current size of the cut pool */

int glp_ios_add_row(glp_tree *T,
      const char *name, int klass, int flags, int len, const int ind[],
      const double val[], int type, double rhs);
/* add row (constraint) to the cut pool */

void glp_ios_del_row(glp_tree *T, int i);
/* remove row (constraint) from the cut pool */

void glp_ios_clear_pool(glp_tree *T);
/* remove all rows (constraints) from the cut pool */

int glp_ios_can_branch(glp_tree *T, int j);
/* check if can branch upon specified variable */

void glp_ios_branch_upon(glp_tree *T, int j, int sel);
/* choose variable to branch upon */

void glp_ios_select_node(glp_tree *T, int p);
/* select subproblem to continue the search */

int glp_ios_heur_sol(glp_tree *T, const double x[]);
/* provide solution found by heuristic */

void glp_ios_terminate(glp_tree *T);
/* terminate the solution process */

#ifdef GLP_UNDOC
int glp_gmi_cut(glp_prob *P, int j, int ind[], double val[], double
      phi[]);
/* generate Gomory's mixed integer cut (core routine) */
#endif

#ifdef GLP_UNDOC
int glp_gmi_gen(glp_prob *P, glp_prob *pool, int max_cuts);
/* generate Gomory's mixed integer cuts */
#endif

#ifdef GLP_UNDOC
typedef struct glp_mir glp_mir;
/* MIR cut generator workspace */
#endif

#ifdef GLP_UNDOC
glp_mir *glp_mir_init(glp_prob *P);
/* create and initialize MIR cut generator */
#endif

#ifdef GLP_UNDOC
int glp_mir_gen(glp_prob *P, glp_mir *mir, glp_prob *pool);
/* generate mixed integer rounding (MIR) cuts */
#endif

#ifdef GLP_UNDOC
void glp_mir_free(glp_mir *mir);
/* delete MIR cut generator workspace */
#endif

#ifdef GLP_UNDOC
typedef struct glp_cfg glp_cfg;
/* conflict graph descriptor */
#endif

#ifdef GLP_UNDOC
glp_cfg *glp_cfg_init(glp_prob *P);
/* create and initialize conflict graph */
#endif

#ifdef GLP_UNDOC
void glp_cfg_free(glp_cfg *G);
/* delete conflict graph descriptor */
#endif

#ifdef GLP_UNDOC
int glp_clq_cut(glp_prob *P, glp_cfg *G, int ind[], double val[]);
/* generate clique cut from conflict graph */
#endif

void glp_init_mpscp(glp_mpscp *parm);
/* initialize MPS format control parameters */

int glp_read_mps(glp_prob *P, int fmt, const glp_mpscp *parm,
      const char *fname);
/* read problem data in MPS format */

int glp_write_mps(glp_prob *P, int fmt, const glp_mpscp *parm,
      const char *fname);
/* write problem data in MPS format */

void glp_init_cpxcp(glp_cpxcp *parm);
/* initialize CPLEX LP format control parameters */

int glp_read_lp(glp_prob *P, const glp_cpxcp *parm, const char *fname);
/* read problem data in CPLEX LP format */

int glp_write_lp(glp_prob *P, const glp_cpxcp *parm, const char *fname);
/* write problem data in CPLEX LP format */

int glp_read_prob(glp_prob *P, int flags, const char *fname);
/* read problem data in GLPK format */

int glp_write_prob(glp_prob *P, int flags, const char *fname);
/* write problem data in GLPK format */

glp_tran *glp_mpl_alloc_wksp(void);
/* allocate the MathProg translator workspace */

void glp_mpl_init_rand(glp_tran *tran, int seed);
/* initialize pseudo-random number generator */

int glp_mpl_read_model(glp_tran *tran, const char *fname, int skip);
/* read and translate model section */

int glp_mpl_read_data(glp_tran *tran, const char *fname);
/* read and translate data section */

int glp_mpl_generate(glp_tran *tran, const char *fname);
/* generate the model */

void glp_mpl_build_prob(glp_tran *tran, glp_prob *prob);
/* build LP/MIP problem instance from the model */

int glp_mpl_postsolve(glp_tran *tran, glp_prob *prob, int sol);
/* postsolve the model */

void glp_mpl_free_wksp(glp_tran *tran);
/* free the MathProg translator workspace */

int glp_read_cnfsat(glp_prob *P, const char *fname);
/* read CNF-SAT problem data in DIMACS format */

int glp_check_cnfsat(glp_prob *P);
/* check for CNF-SAT problem instance */

int glp_write_cnfsat(glp_prob *P, const char *fname);
/* write CNF-SAT problem data in DIMACS format */

int glp_minisat1(glp_prob *P);
/* solve CNF-SAT problem with MiniSat solver */

int glp_intfeas1(glp_prob *P, int use_bound, int obj_bound);
/* solve integer feasibility problem */

int glp_init_env(void);
/* initialize GLPK environment */

const char *glp_version(void);
/* determine library version */

int glp_free_env(void);
/* free GLPK environment */

void glp_puts(const char *s);
/* write string on terminal */

void glp_printf(const char *fmt, ...);
/* write formatted output on terminal */

void glp_vprintf(const char *fmt, va_list arg);
/* write formatted output on terminal */

int glp_term_out(int flag);
/* enable/disable terminal output */

void glp_term_hook(int (*func)(void *info, const char *s), void *info);
/* install hook to intercept terminal output */

int glp_open_tee(const char *name);
/* start copying terminal output to text file */

int glp_close_tee(void);
/* stop copying terminal output to text file */

#ifndef GLP_ERRFUNC_DEFINED
#define GLP_ERRFUNC_DEFINED
typedef void (*glp_errfunc)(const char *fmt, ...);
#endif

#define glp_error glp_error_(__FILE__, __LINE__)
glp_errfunc glp_error_(const char *file, int line);
/* display fatal error message and terminate execution */

#if 1 /* 07/XI-2015 */
int glp_at_error(void);
/* check for error state */
#endif

#define glp_assert(expr) \
      ((void)((expr) || (glp_assert_(#expr, __FILE__, __LINE__), 1)))
void glp_assert_(const char *expr, const char *file, int line);
/* check for logical condition */

void glp_error_hook(void (*func)(void *info), void *info);
/* install hook to intercept abnormal termination */

#define glp_malloc(size) glp_alloc(1, size)
/* allocate memory block (obsolete) */

#define glp_calloc(n, size) glp_alloc(n, size)
/* allocate memory block (obsolete) */

void *glp_alloc(int n, int size);
/* allocate memory block */

void *glp_realloc(void *ptr, int n, int size);
/* reallocate memory block */

void glp_free(void *ptr);
/* free (deallocate) memory block */

void glp_mem_limit(int limit);
/* set memory usage limit */

void glp_mem_usage(int *count, int *cpeak, size_t *total,
      size_t *tpeak);
/* get memory usage information */

double glp_time(void);
/* determine current universal time */

double glp_difftime(double t1, double t0);
/* compute difference between two time values */

typedef struct glp_graph glp_graph;
typedef struct glp_vertex glp_vertex;
typedef struct glp_arc glp_arc;

struct glp_graph
{     /* graph descriptor */
      void *pool; /* DMP *pool; */
      /* memory pool to store graph components */
      char *name;
      /* graph name (1 to 255 chars); NULL means no name is assigned
         to the graph */
      int nv_max;
      /* length of the vertex list (enlarged automatically) */
      int nv;
      /* number of vertices in the graph, 0 <= nv <= nv_max */
      int na;
      /* number of arcs in the graph, na >= 0 */
      glp_vertex **v; /* glp_vertex *v[1+nv_max]; */
      /* v[i], 1 <= i <= nv, is a pointer to i-th vertex */
      void *index; /* AVL *index; */
      /* vertex index to find vertices by their names; NULL means the
         index does not exist */
      int v_size;
      /* size of data associated with each vertex (0 to 256 bytes) */
      int a_size;
      /* size of data associated with each arc (0 to 256 bytes) */
};

struct glp_vertex
{     /* vertex descriptor */
      int i;
      /* vertex ordinal number, 1 <= i <= nv */
      char *name;
      /* vertex name (1 to 255 chars); NULL means no name is assigned
         to the vertex */
      void *entry; /* AVLNODE *entry; */
      /* pointer to corresponding entry in the vertex index; NULL means
         that either the index does not exist or the vertex has no name
         assigned */
      void *data;
      /* pointer to data associated with the vertex */
      void *temp;
      /* working pointer */
      glp_arc *in;
      /* pointer to the (unordered) list of incoming arcs */
      glp_arc *out;
      /* pointer to the (unordered) list of outgoing arcs */
};

struct glp_arc
{     /* arc descriptor */
      glp_vertex *tail;
      /* pointer to the tail endpoint */
      glp_vertex *head;
      /* pointer to the head endpoint */
      void *data;
      /* pointer to data associated with the arc */
      void *temp;
      /* working pointer */
      glp_arc *t_prev;
      /* pointer to previous arc having the same tail endpoint */
      glp_arc *t_next;
      /* pointer to next arc having the same tail endpoint */
      glp_arc *h_prev;
      /* pointer to previous arc having the same head endpoint */
      glp_arc *h_next;
      /* pointer to next arc having the same head endpoint */
};

glp_graph *glp_create_graph(int v_size, int a_size);
/* create graph */

void glp_set_graph_name(glp_graph *G, const char *name);
/* assign (change) graph name */

int glp_add_vertices(glp_graph *G, int nadd);
/* add new vertices to graph */

void glp_set_vertex_name(glp_graph *G, int i, const char *name);
/* assign (change) vertex name */

glp_arc *glp_add_arc(glp_graph *G, int i, int j);
/* add new arc to graph */

void glp_del_vertices(glp_graph *G, int ndel, const int num[]);
/* delete vertices from graph */

void glp_del_arc(glp_graph *G, glp_arc *a);
/* delete arc from graph */

void glp_erase_graph(glp_graph *G, int v_size, int a_size);
/* erase graph content */

void glp_delete_graph(glp_graph *G);
/* delete graph */

void glp_create_v_index(glp_graph *G);
/* create vertex name index */

int glp_find_vertex(glp_graph *G, const char *name);
/* find vertex by its name */

void glp_delete_v_index(glp_graph *G);
/* delete vertex name index */

int glp_read_graph(glp_graph *G, const char *fname);
/* read graph from plain text file */

int glp_write_graph(glp_graph *G, const char *fname);
/* write graph to plain text file */

void glp_mincost_lp(glp_prob *P, glp_graph *G, int names, int v_rhs,
      int a_low, int a_cap, int a_cost);
/* convert minimum cost flow problem to LP */

int glp_mincost_okalg(glp_graph *G, int v_rhs, int a_low, int a_cap,
      int a_cost, double *sol, int a_x, int v_pi);
/* find minimum-cost flow with out-of-kilter algorithm */

int glp_mincost_relax4(glp_graph *G, int v_rhs, int a_low, int a_cap,
      int a_cost, int crash, double *sol, int a_x, int a_rc);
/* find minimum-cost flow with Bertsekas-Tseng relaxation method */

void glp_maxflow_lp(glp_prob *P, glp_graph *G, int names, int s,
      int t, int a_cap);
/* convert maximum flow problem to LP */

int glp_maxflow_ffalg(glp_graph *G, int s, int t, int a_cap,
      double *sol, int a_x, int v_cut);
/* find maximal flow with Ford-Fulkerson algorithm */

int glp_check_asnprob(glp_graph *G, int v_set);
/* check correctness of assignment problem data */

/* assignment problem formulation: */
#define GLP_ASN_MIN        1  /* perfect matching (minimization) */
#define GLP_ASN_MAX        2  /* perfect matching (maximization) */
#define GLP_ASN_MMP        3  /* maximum matching */

int glp_asnprob_lp(glp_prob *P, int form, glp_graph *G, int names,
      int v_set, int a_cost);
/* convert assignment problem to LP */

int glp_asnprob_okalg(int form, glp_graph *G, int v_set, int a_cost,
      double *sol, int a_x);
/* solve assignment problem with out-of-kilter algorithm */

int glp_asnprob_hall(glp_graph *G, int v_set, int a_x);
/* find bipartite matching of maximum cardinality */

double glp_cpp(glp_graph *G, int v_t, int v_es, int v_ls);
/* solve critical path problem */

int glp_read_mincost(glp_graph *G, int v_rhs, int a_low, int a_cap,
      int a_cost, const char *fname);
/* read min-cost flow problem data in DIMACS format */

int glp_write_mincost(glp_graph *G, int v_rhs, int a_low, int a_cap,
      int a_cost, const char *fname);
/* write min-cost flow problem data in DIMACS format */

int glp_read_maxflow(glp_graph *G, int *s, int *t, int a_cap,
      const char *fname);
/* read maximum flow problem data in DIMACS format */

int glp_write_maxflow(glp_graph *G, int s, int t, int a_cap,
      const char *fname);
/* write maximum flow problem data in DIMACS format */

int glp_read_asnprob(glp_graph *G, int v_set, int a_cost, const char
      *fname);
/* read assignment problem data in DIMACS format */

int glp_write_asnprob(glp_graph *G, int v_set, int a_cost, const char
      *fname);
/* write assignment problem data in DIMACS format */

int glp_read_ccdata(glp_graph *G, int v_wgt, const char *fname);
/* read graph in DIMACS clique/coloring format */

int glp_write_ccdata(glp_graph *G, int v_wgt, const char *fname);
/* write graph in DIMACS clique/coloring format */

int glp_netgen(glp_graph *G, int v_rhs, int a_cap, int a_cost,
      const int parm[1+15]);
/* Klingman's network problem generator */

void glp_netgen_prob(int nprob, int parm[1+15]);
/* Klingman's standard network problem instance */

int glp_gridgen(glp_graph *G, int v_rhs, int a_cap, int a_cost,
      const int parm[1+14]);
/* grid-like network problem generator */

int glp_rmfgen(glp_graph *G, int *s, int *t, int a_cap,
      const int parm[1+5]);
/* Goldfarb's maximum flow problem generator */

int glp_weak_comp(glp_graph *G, int v_num);
/* find all weakly connected components of graph */

int glp_strong_comp(glp_graph *G, int v_num);
/* find all strongly connected components of graph */

int glp_top_sort(glp_graph *G, int v_num);
/* topological sorting of acyclic digraph */

int glp_wclique_exact(glp_graph *G, int v_wgt, double *sol, int v_set);
/* find maximum weight clique with exact algorithm */

#endif // <----------------

#ifdef __cplusplus
}
#endif

#endif


























/* glpapi01.c (problem creating and modifying routines) */

/***********************************************************************
*  This code is part of GLPK (GNU Linear Programming Kit).
*
*  Copyright (C) 2000, 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008,
*  2009, 2010, 2011, 2013 Andrew Makhorin, Department for Applied
*  Informatics, Moscow Aviation Institute, Moscow, Russia. All rights
*  reserved. E-mail: <mao@gnu.org>.
*
*  GLPK is free software: you can redistribute it and/or modify it
*  under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  GLPK is distributed in the hope that it will be useful, but WITHOUT
*  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
*  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public
*  License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with GLPK. If not, see <http://www.gnu.org/licenses/>.
***********************************************************************/


/* env.h (GLPK environment) */

/***********************************************************************
*  This code is part of GLPK (GNU Linear Programming Kit).
*
*  Copyright (C) 2000-2015 Andrew Makhorin, Department for Applied
*  Informatics, Moscow Aviation Institute, Moscow, Russia. All rights
*  reserved. E-mail: <mao@gnu.org>.
*
*  GLPK is free software: you can redistribute it and/or modify it
*  under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  GLPK is distributed in the hope that it will be useful, but WITHOUT
*  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
*  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public
*  License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with GLPK. If not, see <http://www.gnu.org/licenses/>.
***********************************************************************/

#ifndef ENV_H
#define ENV_H
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


typedef struct ENV ENV;
typedef struct MBD MBD;

#define SIZE_T_MAX (~(size_t)0)
/* largest value of size_t type */

#define TBUF_SIZE 4096
/* terminal output buffer size, in bytes */

#define EBUF_SIZE 1024
/* error message buffer size, in bytes */

/* enable/disable flag: */
#define GLP_ON  1
#define GLP_OFF 0

struct ENV
{     /* GLPK environment block */
      char version[7+1];
      /* version string returned by the routine glp_version */
      ENV *self;
      /* pointer to this block to check its validity */
      /*--------------------------------------------------------------*/
      /* terminal output */
      char *term_buf; /* char term_buf[TBUF_SIZE]; */
      /* terminal output buffer */
      int term_out;
      /* flag to enable/disable terminal output */
      int (*term_hook)(void *info, const char *s);
      /* user-defined routine to intercept terminal output */
      void *term_info;
      /* transit pointer (cookie) passed to the routine term_hook */
      FILE *tee_file;
      /* output stream used to copy terminal output */
      /*--------------------------------------------------------------*/
      /* error handling */
#if 1 /* 07/XI-2015 */
      int err_st;
      /* error state flag; set on entry to glp_error */
#endif
      const char *err_file;
      /* value of the __FILE__ macro passed to glp_error */
      int err_line;
      /* value of the __LINE__ macro passed to glp_error */
      void (*err_hook)(void *info);
      /* user-defined routine to intercept abnormal termination */
      void *err_info;
      /* transit pointer (cookie) passed to the routine err_hook */
      char *err_buf; /* char err_buf[EBUF_SIZE]; */
      /* buffer to store error messages (used by I/O routines) */
      /*--------------------------------------------------------------*/
      /* dynamic memory allocation */
      size_t mem_limit;
      /* maximal amount of memory, in bytes, available for dynamic
       * allocation */
      MBD *mem_ptr;
      /* pointer to the linked list of allocated memory blocks */
      int mem_count;
      /* total number of currently allocated memory blocks */
      int mem_cpeak;
      /* peak value of mem_count */
      size_t mem_total;
      /* total amount of currently allocated memory, in bytes; it is
       * the sum of the size field over all memory block descriptors */
      size_t mem_tpeak;
      /* peak value of mem_total */
#if 1 /* 23/XI-2015 */
      /*--------------------------------------------------------------*/
      /* bignum module working area */
      void *gmp_pool; /* DMP *gmp_pool; */
      /* working memory pool */
      int gmp_size;
      /* size of working array */
      unsigned short *gmp_work; /* ushort gmp_work[gmp_size]; */
      /* working array */
#endif
      /*--------------------------------------------------------------*/
      /* dynamic linking support (optional) */
      void *h_odbc;
      /* handle to ODBC shared library */
      void *h_mysql;
      /* handle to MySQL shared library */
};

struct MBD
{     /* memory block descriptor */
      size_t size;
      /* size of block, in bytes, including descriptor */
      MBD *self;
      /* pointer to this descriptor to check its validity */
      MBD *prev;
      /* pointer to previous memory block descriptor */
      MBD *next;
      /* pointer to next memory block descriptor */
};

#define get_env_ptr _glp_get_env_ptr
ENV *get_env_ptr(void);
/* retrieve pointer to environment block */

#define tls_set_ptr _glp_tls_set_ptr
void tls_set_ptr(void *ptr);
/* store global pointer in TLS */

#define tls_get_ptr _glp_tls_get_ptr
void *tls_get_ptr(void);
/* retrieve global pointer from TLS */

#define xputs glp_puts
void glp_puts(const char *s);
/* write string on terminal */

#define xprintf glp_printf
void glp_printf(const char *fmt, ...);
/* write formatted output on terminal */

#define xvprintf glp_vprintf
void glp_vprintf(const char *fmt, va_list arg);
/* write formatted output on terminal */

int glp_term_out(int flag);
/* enable/disable terminal output */

void glp_term_hook(int (*func)(void *info, const char *s), void *info);
/* install hook to intercept terminal output */

int glp_open_tee(const char *fname);
/* start copying terminal output to text file */

int glp_close_tee(void);
/* stop copying terminal output to text file */

#ifndef GLP_ERRFUNC_DEFINED
#define GLP_ERRFUNC_DEFINED
typedef void (*glp_errfunc)(const char *fmt, ...);
#endif

#define xerror glp_error_(__FILE__, __LINE__)
glp_errfunc glp_error_(const char *file, int line);
/* display fatal error message and terminate execution */

#define xassert(expr) \
      ((void)((expr) || (glp_assert_(#expr, __FILE__, __LINE__), 1)))
void glp_assert_(const char *expr, const char *file, int line);
/* check for logical condition */

void glp_error_hook(void (*func)(void *info), void *info);
/* install hook to intercept abnormal termination */

#define put_err_msg _glp_put_err_msg
void put_err_msg(const char *msg);
/* provide error message string */

#define get_err_msg _glp_get_err_msg
const char *get_err_msg(void);
/* obtain error message string */

#define xmalloc(size) glp_alloc(1, size)
/* allocate memory block (obsolete) */

#define xcalloc(n, size) glp_alloc(n, size)
/* allocate memory block (obsolete) */

#define xalloc(n, size) glp_alloc(n, size)
#define talloc(n, type) ((type *)glp_alloc(n, sizeof(type)))
void *glp_alloc(int n, int size);
/* allocate memory block */

#define xrealloc(ptr, n, size) glp_realloc(ptr, n, size)
#define trealloc(ptr, n, type) ((type *)glp_realloc(ptr, n, \
      sizeof(type)))
void *glp_realloc(void *ptr, int n, int size);
/* reallocate memory block */

#define xfree(ptr) glp_free(ptr)
#define tfree(ptr) glp_free(ptr)
void glp_free(void *ptr);
/* free memory block */

void glp_mem_limit(int limit);
/* set memory usage limit */

void glp_mem_usage(int *count, int *cpeak, size_t *total,
      size_t *tpeak);
/* get memory usage information */

typedef struct glp_file glp_file;
/* sequential stream descriptor */

#define glp_open _glp_open
glp_file *glp_open(const char *name, const char *mode);
/* open stream */

#define glp_eof _glp_eof
int glp_eof(glp_file *f);
/* test end-of-file indicator */

#define glp_ioerr _glp_ioerr
int glp_ioerr(glp_file *f);
/* test I/O error indicator */

#define glp_read _glp_read
int glp_read(glp_file *f, void *buf, int nnn);
/* read data from stream */

#define glp_getc _glp_getc
int glp_getc(glp_file *f);
/* read character from stream */

#define glp_write _glp_write
int glp_write(glp_file *f, const void *buf, int nnn);
/* write data to stream */

#define glp_format _glp_format
int glp_format(glp_file *f, const char *fmt, ...);
/* write formatted data to stream */

#define glp_close _glp_close
int glp_close(glp_file *f);
/* close stream */

#define xtime glp_time
double glp_time(void);
/* determine current universal time */

#define xdifftime glp_difftime
double glp_difftime(double t1, double t0);
/* compute difference between two time values */

#define xdlopen _glp_dlopen
void *xdlopen(const char *module);
/* open dynamically linked library */

#define xdlsym _glp_dlsym
void *xdlsym(void *h, const char *symbol);
/* obtain address of symbol from dynamically linked library */

#define xdlclose _glp_dlclose
void xdlclose(void *h);
/* close dynamically linked library */

#endif

/* eof */




/* env.c (GLPK environment initialization/termination) */

/***********************************************************************
*  This code is part of GLPK (GNU Linear Programming Kit).
*
*  Copyright (C) 2000-2013 Andrew Makhorin, Department for Applied
*  Informatics, Moscow Aviation Institute, Moscow, Russia. All rights
*  reserved. E-mail: <mao@gnu.org>.
*
*  GLPK is free software: you can redistribute it and/or modify it
*  under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  GLPK is distributed in the hope that it will be useful, but WITHOUT
*  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
*  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public
*  License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with GLPK. If not, see <http://www.gnu.org/licenses/>.
***********************************************************************/


/***********************************************************************
*  NAME
*
*  glp_init_env - initialize GLPK environment
*
*  SYNOPSIS
*
*  int glp_init_env(void);
*
*  DESCRIPTION
*
*  The routine glp_init_env initializes the GLPK environment. Normally
*  the application program does not need to call this routine, because
*  it is called automatically on the first call to any API routine.
*
*  RETURNS
*
*  The routine glp_init_env returns one of the following codes:
*
*  0 - initialization successful;
*  1 - environment has been already initialized;
*  2 - initialization failed (insufficient memory);
*  3 - initialization failed (unsupported programming model). */

int glp_init_env(void)
{     ENV *env;
      int ok;
      /* check if the programming model is supported */
      ok = (CHAR_BIT == 8 && sizeof(char) == 1 &&
         sizeof(short) == 2 && sizeof(int) == 4 &&
         (sizeof(void *) == 4 || sizeof(void *) == 8));
      if (!ok)
         return 3;
      /* check if the environment is already initialized */
      if (tls_get_ptr() != NULL)
         return 1;
      /* allocate and initialize the environment block */
      env = malloc(sizeof(ENV));
      if (env == NULL)
         return 2;
      memset(env, 0, sizeof(ENV));
      sprintf(env->version, "%d.%d",
         GLP_MAJOR_VERSION, GLP_MINOR_VERSION);
      env->self = env;
      env->term_buf = malloc(TBUF_SIZE);
      if (env->term_buf == NULL)
      {  free(env);
         return 2;
      }
      env->term_out = GLP_ON;
      env->term_hook = NULL;
      env->term_info = NULL;
      env->tee_file = NULL;
#if 1 /* 23/XI-2015 */
      env->err_st = 0;
#endif
      env->err_file = NULL;
      env->err_line = 0;
      env->err_hook = NULL;
      env->err_info = NULL;
      env->err_buf = malloc(EBUF_SIZE);
      if (env->err_buf == NULL)
      {  free(env->term_buf);
         free(env);
         return 2;
      }
      env->err_buf[0] = '\0';
      env->mem_limit = SIZE_T_MAX;
      env->mem_ptr = NULL;
      env->mem_count = env->mem_cpeak = 0;
      env->mem_total = env->mem_tpeak = 0;
#if 1 /* 23/XI-2015 */
      env->gmp_pool = NULL;
      env->gmp_size = 0;
      env->gmp_work = NULL;
#endif
      env->h_odbc = env->h_mysql = NULL;
      /* save pointer to the environment block */
      tls_set_ptr(env);
      /* initialization successful */
      return 0;
}

/***********************************************************************
*  NAME
*
*  get_env_ptr - retrieve pointer to environment block
*
*  SYNOPSIS
*
*  #include "env.h"
*  ENV *get_env_ptr(void);
*
*  DESCRIPTION
*
*  The routine get_env_ptr retrieves and returns a pointer to the GLPK
*  environment block.
*
*  If the GLPK environment has not been initialized yet, the routine
*  performs initialization. If initialization fails, the routine prints
*  an error message to stderr and terminates the program.
*
*  RETURNS
*
*  The routine returns a pointer to the environment block. */

ENV *get_env_ptr(void)
{     ENV *env = tls_get_ptr();
      /* check if the environment has been initialized */
      if (env == NULL)
      {  /* not initialized yet; perform initialization */
         if (glp_init_env() != 0)
         {  /* initialization failed; display an error message */
            fprintf(stderr, "GLPK initialization failed\n");
            fflush(stderr);
            /* and abnormally terminate the program */
            abort();
         }
         /* initialization successful; retrieve the pointer */
         env = tls_get_ptr();
      }
      /* check if the environment block is valid */
      if (env->self != env)
      {  fprintf(stderr, "Invalid GLPK environment\n");
         fflush(stderr);
         abort();
      }
      return env;
}

/***********************************************************************
*  NAME
*
*  glp_version - determine library version
*
*  SYNOPSIS
*
*  const char *glp_version(void);
*
*  RETURNS
*
*  The routine glp_version returns a pointer to a null-terminated
*  character string, which specifies the version of the GLPK library in
*  the form "X.Y", where X is the major version number, and Y is the
*  minor version number, for example, "4.16". */

const char *glp_version(void)
{     ENV *env = get_env_ptr();
      return env->version;
}

/***********************************************************************
*  NAME
*
*  glp_free_env - free GLPK environment
*
*  SYNOPSIS
*
*  int glp_free_env(void);
*
*  DESCRIPTION
*
*  The routine glp_free_env frees all resources used by GLPK routines
*  (memory blocks, etc.) which are currently still in use.
*
*  Normally the application program does not need to call this routine,
*  because GLPK routines always free all unused resources. However, if
*  the application program even has deleted all problem objects, there
*  will be several memory blocks still allocated for the library needs.
*  For some reasons the application program may want GLPK to free this
*  memory, in which case it should call glp_free_env.
*
*  Note that a call to glp_free_env invalidates all problem objects as
*  if no GLPK routine were called.
*
*  RETURNS
*
*  0 - termination successful;
*  1 - environment is inactive (was not initialized). */

int glp_free_env(void)
{     ENV *env = tls_get_ptr();
      MBD *desc;
      /* check if the environment is active */
      if (env == NULL)
         return 1;
      /* check if the environment block is valid */
      if (env->self != env)
      {  fprintf(stderr, "Invalid GLPK environment\n");
         fflush(stderr);
         abort();
      }
      /* close handles to shared libraries */
      if (env->h_odbc != NULL)
         xdlclose(env->h_odbc);
      if (env->h_mysql != NULL)
         xdlclose(env->h_mysql);
      /* free memory blocks which are still allocated */
      while (env->mem_ptr != NULL)
      {  desc = env->mem_ptr;
         env->mem_ptr = desc->next;
         free(desc);
      }
      /* close text file used for copying terminal output */
      if (env->tee_file != NULL)
         fclose(env->tee_file);
      /* invalidate the environment block */
      env->self = NULL;
      /* free memory allocated to the environment block */
      free(env->term_buf);
      free(env->err_buf);
      free(env);
      /* reset a pointer to the environment block */
      tls_set_ptr(NULL);
      /* termination successful */
      return 0;
}

/* eof */


typedef struct AVL AVL;
typedef struct AVLNODE AVLNODE;

#define avl_create_tree _glp_avl_create_tree
AVL *avl_create_tree(int (*fcmp)(void *info, const void *key1,
      const void *key2), void *info);
/* create AVL tree */

#define avl_strcmp _glp_avl_strcmp
int avl_strcmp(void *info, const void *key1, const void *key2);
/* compare character string keys */

#define avl_insert_node _glp_avl_insert_node
AVLNODE *avl_insert_node(AVL *tree, const void *key);
/* insert new node into AVL tree */

#define avl_set_node_type _glp_avl_set_node_type
void avl_set_node_type(AVLNODE *node, int type);
/* assign the type field of specified node */

#define avl_set_node_link _glp_avl_set_node_link
void avl_set_node_link(AVLNODE *node, void *link);
/* assign the link field of specified node */

#define avl_find_node _glp_avl_find_node
AVLNODE *avl_find_node(AVL *tree, const void *key);
/* find node in AVL tree */

#define avl_get_node_type _glp_avl_get_node_type
int avl_get_node_type(AVLNODE *node);
/* retrieve the type field of specified node */

#define avl_get_node_link _glp_avl_get_node_link
void *avl_get_node_link(AVLNODE *node);
/* retrieve the link field of specified node */

#define avl_delete_node _glp_avl_delete_node
void avl_delete_node(AVL *tree, AVLNODE *node);
/* delete specified node from AVL tree */

#define avl_delete_tree _glp_avl_delete_tree
void avl_delete_tree(AVL *tree);
/* delete AVL tree */

typedef struct DMP DMP;

#define dmp_debug _glp_dmp_debug
extern int dmp_debug;
/* debug mode flag */

#define dmp_create_pool _glp_dmp_create_pool
DMP *dmp_create_pool(void);
/* create dynamic memory pool */

#define dmp_talloc(pool, type) \
      ((type *)dmp_get_atom(pool, sizeof(type)))

#define dmp_get_atom _glp_dmp_get_atom
void *dmp_get_atom(DMP *pool, int size);
/* get free atom from dynamic memory pool */

#define dmp_tfree(pool, atom) \
      dmp_free_atom(pool, atom, sizeof(*(atom)))

#define dmp_free_atom _glp_dmp_free_atom
void dmp_free_atom(DMP *pool, void *atom, int size);
/* return atom to dynamic memory pool */

#define dmp_in_use _glp_dmp_in_use
size_t dmp_in_use(DMP *pool);
/* determine how many atoms are still in use */

#define dmp_delete_pool _glp_dmp_delete_pool
void dmp_delete_pool(DMP *pool);
/* delete dynamic memory pool */

struct DMP
{     /* dynamic memory pool */
      void *avail[32];
      /* avail[k], 0 <= k <= 31, is a pointer to first available (free)
       * atom of (k+1)*8 bytes long; at the beginning of each free atom
       * there is a pointer to another free atom of the same size */
      void *block;
      /* pointer to most recently allocated memory block; at the
       * beginning of each allocated memory block there is a pointer to
       * previously allocated memory block */
      int used;
      /* number of bytes used in most recently allocated memory block */
      size_t count;
      /* number of atoms which are currently in use */
};

#define DMP_BLK_SIZE 8000
/* size of memory blocks, in bytes, allocated for memory pools */

struct prefix
{     /* atom prefix (for debugging only) */
      DMP *pool;
      /* dynamic memory pool */
      int size;
      /* original atom size, in bytes */
};

#define prefix_size ((sizeof(struct prefix) + 7) & ~7)
/* size of atom prefix rounded up to multiple of 8 bytes */

int dmp_debug;
/* debug mode flag */

/***********************************************************************
*  NAME
*
*  dmp_create_pool - create dynamic memory pool
*
*  SYNOPSIS
*
*  #include "dmp.h"
*  DMP *dmp_create_pool(void);
*
*  DESCRIPTION
*
*  The routine dmp_create_pool creates a dynamic memory pool.
*
*  RETURNS
*
*  The routine returns a pointer to the memory pool created. */

DMP *dmp_create_pool(void)
{     DMP *pool;
      int k;
      xassert(sizeof(void *) <= 8);
      if (dmp_debug)
         xprintf("dmp_create_pool: warning: debug mode is on\n");
      pool = talloc(1, DMP);
      for (k = 0; k <= 31; k++)
         pool->avail[k] = NULL;
      pool->block = NULL;
      pool->used = DMP_BLK_SIZE;
      pool->count = 0;
      return pool;
}

/***********************************************************************
*  NAME
*
*  dmp_get_atom - get free atom from dynamic memory pool
*
*  SYNOPSIS
*
*  #include "dmp.h"
*  void *dmp_get_atom(DMP *pool, int size);
*
*  DESCRIPTION
*
*  The routine dmp_get_atom obtains a free atom (memory space) from the
*  specified memory pool.
*
*  The parameter size is the atom size, in bytes, 1 <= size <= 256.
*
*  Note that the free atom contains arbitrary data, not binary zeros.
*
*  RETURNS
*
*  The routine returns a pointer to the free atom obtained. */

void *dmp_get_atom(DMP *pool, int size)
{     void *atom;
      int k, need;
      xassert(1 <= size && size <= 256);
      /* round up atom size to multiple of 8 bytes */
      need = (size + 7) & ~7;
      /* determine number of corresponding list of free atoms */
      k = (need >> 3) - 1;
      /* obtain free atom */
      if (pool->avail[k] == NULL)
      {  /* corresponding list of free atoms is empty */
         /* if debug mode is on, add atom prefix size */
         if (dmp_debug)
            need += prefix_size;
         if (pool->used + need > DMP_BLK_SIZE)
         {  /* allocate new memory block */
            void *block = talloc(DMP_BLK_SIZE, char);
            *(void **)block = pool->block;
            pool->block = block;
            pool->used = 8; /* sufficient to store pointer */
         }
         /* allocate new atom in current memory block */
         atom = (char *)pool->block + pool->used;
         pool->used += need;
      }
      else
      {  /* obtain atom from corresponding list of free atoms */
         atom  = pool->avail[k];
         pool->avail[k] = *(void **)atom;
      }
      /* if debug mode is on, fill atom prefix */
      if (dmp_debug)
      {  ((struct prefix *)atom)->pool = pool;
         ((struct prefix *)atom)->size = size;
         atom = (char *)atom + prefix_size;
      }
      /* increase number of allocated atoms */
      pool->count++;
      return atom;
}

/***********************************************************************
*  NAME
*
*  dmp_free_atom - return atom to dynamic memory pool
*
*  SYNOPSIS
*
*  #include "dmp.h"
*  void dmp_free_atom(DMP *pool, void *atom, int size);
*
*  DESCRIPTION
*
*  The routine dmp_free_atom returns the specified atom (memory space)
*  to the specified memory pool, making the atom free.
*
*  The parameter size is the atom size, in bytes, 1 <= size <= 256.
*
*  Note that the atom can be returned only to the pool, from which it
*  was obtained, and its size must be exactly the same as on obtaining
*  it from the pool. */

void dmp_free_atom(DMP *pool, void *atom, int size)
{     int k;
      xassert(1 <= size && size <= 256);
      /* determine number of corresponding list of free atoms */
      k = ((size + 7) >> 3) - 1;
      /* if debug mode is on, check atom prefix */
      if (dmp_debug)
      {  atom = (char *)atom - prefix_size;
         xassert(((struct prefix *)atom)->pool == pool);
         xassert(((struct prefix *)atom)->size == size);
      }
      /* return atom to corresponding list of free atoms */
      *(void **)atom = pool->avail[k];
      pool->avail[k] = atom;
      /* decrease number of allocated atoms */
      xassert(pool->count > 0);
      pool->count--;
      return;
}

/***********************************************************************
*  NAME
*
*  dmp_in_use - determine how many atoms are still in use
*
*  SYNOPSIS
*
*  #include "dmp.h"
*  size_t dmp_in_use(DMP *pool);
*
*  RETURNS
*
*  The routine returns the number of atoms of the specified memory pool
*  which are still in use. */

size_t dmp_in_use(DMP *pool)
{     return
         pool->count;
}

/***********************************************************************
*  NAME
*
*  dmp_delete_pool - delete dynamic memory pool
*
*  SYNOPSIS
*
*  #include "dmp.h"
*  void dmp_delete_pool(DMP *pool);
*
*  DESCRIPTION
*
*  The routine dmp_delete_pool deletes the specified dynamic memory
*  pool freeing all the memory allocated to this object. */

void dmp_delete_pool(DMP *pool)
{     while (pool->block != NULL)
      {  void *block = pool->block;
         pool->block = *(void **)block;
         tfree(block);
      }
      tfree(pool);
      return;
}

struct AVL
{     /* AVL tree (Adelson-Velsky & Landis binary search tree) */
      DMP *pool;
      /* memory pool for allocating nodes */
      AVLNODE *root;
      /* pointer to the root node */
      int (*fcmp)(void *info, const void *key1, const void *key2);
      /* application-defined key comparison routine */
      void *info;
      /* transit pointer passed to the routine fcmp */
      int size;
      /* the tree size (the total number of nodes) */
      int height;
      /* the tree height */
};

struct AVLNODE
{     /* node of AVL tree */
      const void *key;
      /* pointer to the node key (data structure for representing keys
         is supplied by the application) */
      int rank;
      /* node rank = relative position of the node in its own subtree =
         the number of nodes in the left subtree plus one */
      int type;
      /* reserved for the application specific information */
      void *link;
      /* reserved for the application specific information */
      AVLNODE *up;
      /* pointer to the parent node */
      short int flag;
      /* node flag:
         0 - this node is the left child of its parent (or this node is
             the root of the tree and has no parent)
         1 - this node is the right child of its parent */
      short int bal;
      /* node balance = the difference between heights of the right and
         left subtrees:
         -1 - the left subtree is higher than the right one;
          0 - the left and right subtrees have the same height;
         +1 - the left subtree is lower than the right one */
      AVLNODE *left;
      /* pointer to the root of the left subtree */
      AVLNODE *right;
      /* pointer to the root of the right subtree */
};

AVL *avl_create_tree(int (*fcmp)(void *info, const void *key1,
      const void *key2), void *info)
{     /* create AVL tree */
      AVL *tree;
      tree = xmalloc(sizeof(AVL));
      tree->pool = dmp_create_pool();
      tree->root = NULL;
      tree->fcmp = fcmp;
      tree->info = info;
      tree->size = 0;
      tree->height = 0;
      return tree;
}

int avl_strcmp(void *info, const void *key1, const void *key2)
{     /* compare character string keys */
      xassert(info == info);
      return strcmp(key1, key2);
}

static AVLNODE *rotate_subtree(AVL *tree, AVLNODE *node);

AVLNODE *avl_insert_node(AVL *tree, const void *key)
{     /* insert new node into AVL tree */
      AVLNODE *p, *q, *r;
      short int flag;
      /* find an appropriate point for insertion */
      p = NULL; q = tree->root;
      while (q != NULL)
      {  p = q;
         if (tree->fcmp(tree->info, key, p->key) <= 0)
         {  flag = 0;
            q = p->left;
            p->rank++;
         }
         else
         {  flag = 1;
            q = p->right;
         }
      }
      /* create new node and insert it into the tree */
      r = dmp_get_atom(tree->pool, sizeof(AVLNODE));
      r->key = key; r->type = 0; r->link = NULL;
      r->rank = 1; r->up = p;
      r->flag = (short int)(p == NULL ? 0 : flag);
      r->bal = 0; r->left = NULL; r->right = NULL;
      tree->size++;
      if (p == NULL)
         tree->root = r;
      else
         if (flag == 0) p->left = r; else p->right = r;
      /* go upstairs to the root and correct all subtrees affected by
         insertion */
      while (p != NULL)
      {  if (flag == 0)
         {  /* the height of the left subtree of [p] is increased */
            if (p->bal > 0)
            {  p->bal = 0;
               break;
            }
            if (p->bal < 0)
            {  rotate_subtree(tree, p);
               break;
            }
            p->bal = -1; flag = p->flag; p = p->up;
         }
         else
         {  /* the height of the right subtree of [p] is increased */
            if (p->bal < 0)
            {  p->bal = 0;
               break;
            }
            if (p->bal > 0)
            {  rotate_subtree(tree, p);
               break;
            }
            p->bal = +1; flag = p->flag; p = p->up;
         }
      }
      /* if the root has been reached, the height of the entire tree is
         increased */
      if (p == NULL) tree->height++;
      return r;
}

void avl_set_node_type(AVLNODE *node, int type)
{     /* assign the type field of specified node */
      node->type = type;
      return;
}

void avl_set_node_link(AVLNODE *node, void *link)
{     /* assign the link field of specified node */
      node->link = link;
      return;
}

AVLNODE *avl_find_node(AVL *tree, const void *key)
{     /* find node in AVL tree */
      AVLNODE *p;
      int c;
      p = tree->root;
      while (p != NULL)
      {  c = tree->fcmp(tree->info, key, p->key);
         if (c == 0) break;
         p = (c < 0 ? p->left : p->right);
      }
      return p;
}

int avl_get_node_type(AVLNODE *node)
{     /* retrieve the type field of specified node */
      return node->type;
}

void *avl_get_node_link(AVLNODE *node)
{     /* retrieve the link field of specified node */
      return node->link;
}

static AVLNODE *find_next_node(AVL *tree, AVLNODE *node)
{     /* find next node in AVL tree */
      AVLNODE *p, *q;
      if (tree->root == NULL) return NULL;
      p = node;
      q = (p == NULL ? tree->root : p->right);
      if (q == NULL)
      {  /* go upstairs from the left subtree */
         for (;;)
         {  q = p->up;
            if (q == NULL) break;
            if (p->flag == 0) break;
            p = q;
         }
      }
      else
      {  /* go downstairs into the right subtree */
         for (;;)
         {  p = q->left;
            if (p == NULL) break;
            q = p;
         }
      }
      return q;
}

void avl_delete_node(AVL *tree, AVLNODE *node)
{     /* delete specified node from AVL tree */
      AVLNODE *f, *p, *q, *r, *s, *x, *y;
      short int flag;
      p = node;
      /* if both subtrees of the specified node are non-empty, the node
         should be interchanged with the next one, at least one subtree
         of which is always empty */
      if (p->left == NULL || p->right == NULL) goto skip;
      f = p->up; q = p->left;
      r = find_next_node(tree, p); s = r->right;
      if (p->right == r)
      {  if (f == NULL)
            tree->root = r;
         else
            if (p->flag == 0) f->left = r; else f->right = r;
         r->rank = p->rank; r->up = f;
         r->flag = p->flag; r->bal = p->bal;
         r->left = q; r->right = p;
         q->up = r;
         p->rank = 1; p->up = r; p->flag = 1;
         p->bal = (short int)(s == NULL ? 0 : +1);
         p->left = NULL; p->right = s;
         if (s != NULL) s->up = p;
      }
      else
      {  x = p->right; y = r->up;
         if (f == NULL)
            tree->root = r;
         else
            if (p->flag == 0) f->left = r; else f->right = r;
         r->rank = p->rank; r->up = f;
         r->flag = p->flag; r->bal = p->bal;
         r->left = q; r->right = x;
         q->up = r; x->up = r; y->left = p;
         p->rank = 1; p->up = y; p->flag = 0;
         p->bal = (short int)(s == NULL ? 0 : +1);
         p->left = NULL; p->right = s;
         if (s != NULL) s->up = p;
      }
skip: /* now the specified node [p] has at least one empty subtree;
         go upstairs to the root and adjust the rank field of all nodes
         affected by deletion */
      q = p; f = q->up;
      while (f != NULL)
      {  if (q->flag == 0) f->rank--;
         q = f; f = q->up;
      }
      /* delete the specified node from the tree */
      f = p->up; flag = p->flag;
      q = p->left != NULL ? p->left : p->right;
      if (f == NULL)
         tree->root = q;
      else
         if (flag == 0) f->left = q; else f->right = q;
      if (q != NULL) q->up = f, q->flag = flag;
      tree->size--;
      /* go upstairs to the root and correct all subtrees affected by
         deletion */
      while (f != NULL)
      {  if (flag == 0)
         {  /* the height of the left subtree of [f] is decreased */
            if (f->bal == 0)
            {  f->bal = +1;
               break;
            }
            if (f->bal < 0)
               f->bal = 0;
            else
            {  f = rotate_subtree(tree, f);
               if (f->bal < 0) break;
            }
            flag = f->flag; f = f->up;
         }
         else
         {  /* the height of the right subtree of [f] is decreased */
            if (f->bal == 0)
            {  f->bal = -1;
               break;
            }
            if (f->bal > 0)
               f->bal = 0;
            else
            {  f = rotate_subtree(tree, f);
               if (f->bal > 0) break;
            }
            flag = f->flag; f = f->up;
         }
      }
      /* if the root has been reached, the height of the entire tree is
         decreased */
      if (f == NULL) tree->height--;
      /* returns the deleted node to the memory pool */
      dmp_free_atom(tree->pool, p, sizeof(AVLNODE));
      return;
}

static AVLNODE *rotate_subtree(AVL *tree, AVLNODE *node)
{     /* restore balance of AVL subtree */
      AVLNODE *f, *p, *q, *r, *x, *y;
      xassert(node != NULL);
      p = node;
      if (p->bal < 0)
      {  /* perform negative (left) rotation */
         f = p->up; q = p->left; r = q->right;
         if (q->bal <= 0)
         {  /* perform single negative rotation */
            if (f == NULL)
               tree->root = q;
            else
               if (p->flag == 0) f->left = q; else f->right = q;
            p->rank -= q->rank;
            q->up = f; q->flag = p->flag; q->bal++; q->right = p;
            p->up = q; p->flag = 1;
            p->bal = (short int)(-q->bal); p->left = r;
            if (r != NULL) r->up = p, r->flag = 0;
            node = q;
         }
         else
         {  /* perform double negative rotation */
            x = r->left; y = r->right;
            if (f == NULL)
               tree->root = r;
            else
               if (p->flag == 0) f->left = r; else f->right = r;
            p->rank -= (q->rank + r->rank);
            r->rank += q->rank;
            p->bal = (short int)(r->bal >= 0 ? 0 : +1);
            q->bal = (short int)(r->bal <= 0 ? 0 : -1);
            r->up = f; r->flag = p->flag; r->bal = 0;
            r->left = q; r->right = p;
            p->up = r; p->flag = 1; p->left = y;
            q->up = r; q->flag = 0; q->right = x;
            if (x != NULL) x->up = q, x->flag = 1;
            if (y != NULL) y->up = p, y->flag = 0;
            node = r;
         }
      }
      else
      {  /* perform positive (right) rotation */
         f = p->up; q = p->right; r = q->left;
         if (q->bal >= 0)
         {  /* perform single positive rotation */
            if (f == NULL)
               tree->root = q;
            else
               if (p->flag == 0) f->left = q; else f->right = q;
            q->rank += p->rank;
            q->up = f; q->flag = p->flag; q->bal--; q->left = p;
            p->up = q; p->flag = 0;
            p->bal = (short int)(-q->bal); p->right = r;
            if (r != NULL) r->up = p, r->flag = 1;
            node = q;
         }
         else
         {  /* perform double positive rotation */
            x = r->left; y = r->right;
            if (f == NULL)
               tree->root = r;
            else
               if (p->flag == 0) f->left = r; else f->right = r;
            q->rank -= r->rank;
            r->rank += p->rank;
            p->bal = (short int)(r->bal <= 0 ? 0 : -1);
            q->bal = (short int)(r->bal >= 0 ? 0 : +1);
            r->up = f; r->flag = p->flag; r->bal = 0;
            r->left = p; r->right = q;
            p->up = r; p->flag = 0; p->right = x;
            q->up = r; q->flag = 1; q->left = y;
            if (x != NULL) x->up = p, x->flag = 1;
            if (y != NULL) y->up = q, y->flag = 0;
            node = r;
         }
      }
      return node;
}

void avl_delete_tree(AVL *tree)
{     /* delete AVL tree */
      dmp_delete_pool(tree->pool);
      xfree(tree);
      return;
}


typedef struct FVS FVS;

struct FVS
{     /* sparse vector in full vector storage format */
      int n;
      /* vector dimension (total number of elements) */
      int nnz;
      /* number of non-zero elements, 0 <= nnz <= n */
      int *ind; /* int ind[1+n]; */
      /* ind[0] is not used;
       * ind[k] = j, 1 <= k <= nnz, means that vec[j] != 0
       * non-zero indices in the array ind are stored in arbitrary
       * order; if vec[j] = 0, its index j should NOT be presented in
       * the array ind */
      double *vec; /* double vec[1+n]; */
      /* vec[0] is not used;
       * vec[j], 1 <= j <= n, is a numeric value of j-th element */
};

#define fvs_alloc_vec _glp_fvs_alloc_vec
void fvs_alloc_vec(FVS *x, int n);
/* allocate sparse vector */

#define fvs_check_vec _glp_fvs_check_vec
void fvs_check_vec(const FVS *x);
/* check sparse vector */

#define fvs_gather_vec _glp_fvs_gather_vec
void fvs_gather_vec(FVS *x, double eps);
/* gather sparse vector */

#define fvs_clear_vec _glp_fvs_clear_vec
void fvs_clear_vec(FVS *x);
/* clear sparse vector */

#define fvs_copy_vec _glp_fvs_copy_vec
void fvs_copy_vec(FVS *x, const FVS *y);
/* copy sparse vector */

#define fvs_adjust_vec _glp_fvs_adjust_vec
void fvs_adjust_vec(FVS *x, double eps);
/* replace tiny vector elements by exact zeros */

#define fvs_free_vec _glp_fvs_free_vec
void fvs_free_vec(FVS *x);
/* deallocate sparse vector */

void fvs_alloc_vec(FVS *x, int n)
{     /* allocate sparse vector */
      int j;
      xassert(n >= 0);
      x->n = n;
      x->nnz = 0;
      x->ind = talloc(1+n, int);
      x->vec = talloc(1+n, double);
      for (j = 1; j <= n; j++)
         x->vec[j] = 0.0;
      return;
}

void fvs_check_vec(const FVS *x)
{     /* check sparse vector */
      /* NOTE: for testing/debugging only */
      int n = x->n;
      int nnz = x->nnz;
      int *ind = x->ind;
      double *vec = x->vec;
      char *map;
      int j, k;
      xassert(n >= 0);
      xassert(0 <= nnz && nnz <= n);
      map = talloc(1+n, char);
      for (j = 1; j <= n; j++)
         map[j] = (vec[j] != 0.0);
      for (k = 1; k <= nnz; k++)
      {  j = ind[k];
         xassert(1 <= j && j <= n);
         xassert(map[j]);
         map[j] = 0;
      }
      for (j = 1; j <= n; j++)
         xassert(!map[j]);
      tfree(map);
      return;
}

void fvs_gather_vec(FVS *x, double eps)
{     /* gather sparse vector */
      int n = x->n;
      int *ind = x->ind;
      double *vec = x->vec;
      int j, nnz = 0;
      for (j = n; j >= 1; j--)
      {  if (-eps < vec[j] && vec[j] < +eps)
            vec[j] = 0.0;
         else
            ind[++nnz] = j;
      }
      x->nnz = nnz;
      return;
}

void fvs_clear_vec(FVS *x)
{     /* clear sparse vector */
      int *ind = x->ind;
      double *vec = x->vec;
      int k;
      for (k = x->nnz; k >= 1; k--)
         vec[ind[k]] = 0.0;
      x->nnz = 0;
      return;
}

void fvs_copy_vec(FVS *x, const FVS *y)
{     /* copy sparse vector */
      int *x_ind = x->ind;
      double *x_vec = x->vec;
      int *y_ind = y->ind;
      double *y_vec = y->vec;
      int j, k;
      xassert(x != y);
      xassert(x->n == y->n);
      fvs_clear_vec(x);
      for (k = x->nnz = y->nnz; k >= 1; k--)
      {  j = x_ind[k] = y_ind[k];
         x_vec[j] = y_vec[j];
      }
      return;
}

void fvs_adjust_vec(FVS *x, double eps)
{     /* replace tiny vector elements by exact zeros */
      int nnz = x->nnz;
      int *ind = x->ind;
      double *vec = x->vec;
      int j, k, cnt = 0;
      for (k = 1; k <= nnz; k++)
      {  j = ind[k];
         if (-eps < vec[j] && vec[j] < +eps)
            vec[j] = 0.0;
         else
            ind[++cnt] = j;
      }
      x->nnz = cnt;
      return;
}

void fvs_free_vec(FVS *x)
{     /* deallocate sparse vector */
      tfree(x->ind);
      tfree(x->vec);
      x->n = x->nnz = -1;
      x->ind = NULL;
      x->vec = NULL;
      return;
}





/***********************************************************************
*  Sparse Vector Area (SVA) is a container for sparse vectors. This
*  program object is used mainly on computing factorization, where the
*  sparse vectors are rows and columns of sparse matrices.
*
*  The SVA storage is a set of locations numbered 1, 2, ..., size,
*  where size is the size of SVA, which is the total number of
*  locations currently allocated. Each location is identified by its
*  pointer p, 1 <= p <= size, and is the pair (ind[p], val[p]), where
*  ind[p] and val[p] are, respectively, the index and value fields used
*  to store the index and numeric value of a particular vector element.
*
*  Each sparse vector is identified by its reference number k,
*  1 <= k <= n, where n is the total number of vectors currently stored
*  in SVA, and defined by the triplet (ptr[k], len[k], cap[k]), where:
*  ptr[k] is a pointer to the first location of the vector; len[k] is
*  the vector length, which is the number of its non-zero elements,
*  len[k] >= 0; and cap[k] is the capacity of the vector, which is the
*  total number of adjacent locations allocated to that vector,
*  cap[k] >= len[k]. Thus, non-zero elements of k-th vector are stored
*  in locations ptr[k], ptr[k]+1, ..., ptr[k]+len[k]-1, and locations
*  ptr[k]+len[k], ptr[k]+len[k]+1, ..., ptr[k]+cap[k]-1 are reserved.
*
*  The SVA storage is divided into three parts as follows:
*
*  Locations 1, 2, ..., m_ptr-1 constitute the left (dynamic) part of
*  SVA. This part is used to store vectors, whose capacity may change.
*  Note that all vectors stored in the left part are also included in
*  a doubly linked list, where they are ordered by increasing their
*  pointers ptr[k] (this list is needed for efficient implementation
*  of the garbage collector used to defragment the left part of SVA);
*
*  Locations m_ptr, m_ptr+1, ..., r_ptr-1 are free and constitute the
*  middle (free) part of SVA.
*
*  Locations r_ptr, r_ptr+1, ..., size constitute the right (static)
*  part of SVA. This part is used to store vectors, whose capacity is
*  not changed. */

typedef struct SVA SVA;

struct SVA
{     /* sparse vector area */
      int n_max;
      /* maximal value of n (enlarged automatically) */
      int n;
      /* number of currently allocated vectors, 0 <= n <= n_max */
      int *ptr; /* int ptr[1+n_max]; */
      /* ptr[0] is not used;
       * ptr[k], 1 <= i <= n, is pointer to first location of k-th
       * vector in the arrays ind and val */
      int *len; /* int len[1+n_max]; */
      /* len[0] is not used;
       * len[k], 1 <= k <= n, is length of k-th vector, len[k] >= 0 */
      int *cap; /* int cap[1+n_max]; */
      /* cap[0] is not used;
       * cap[k], 1 <= k <= n, is capacity of k-th vector (the number
       * of adjacent locations allocated to it), cap[k] >= len[k] */
      /* NOTE: if cap[k] = 0, then ptr[k] = 0 and len[k] = 0 */
      int size;
      /* total number of locations in SVA */
      int m_ptr, r_ptr;
      /* partitioning pointers that define the left, middle, and right
       * parts of SVA (see above); 1 <= m_ptr <= r_ptr <= size+1 */
      int head;
      /* number of first (leftmost) vector in the linked list */
      int tail;
      /* number of last (rightmost) vector in the linked list */
      int *prev; /* int prev[1+n_max]; */
      /* prev[0] is not used;
       * prev[k] is number of vector which precedes k-th vector in the
       * linked list;
       * prev[k] < 0 means that k-th vector is not in the list */
      int *next; /* int next[1+n_max]; */
      /* next[0] is not used;
       * next[k] is number of vector which succedes k-th vector in the
       * linked list;
       * next[k] < 0 means that k-th vector is not in the list */
      /* NOTE: only vectors having non-zero capacity and stored in the
       *       left part of SVA are included in this linked list */
      int *ind; /* int ind[1+size]; */
      /* ind[0] is not used;
       * ind[p], 1 <= p <= size, is index field of location p */
      double *val; /* double val[1+size]; */
      /* val[0] is not used;
       * val[p], 1 <= p <= size, is value field of location p */
#if 1
      int talky;
      /* option to enable talky mode */
#endif
};

#define sva_create_area _glp_sva_create_area
SVA *sva_create_area(int n_max, int size);
/* create sparse vector area (SVA) */

#define sva_alloc_vecs _glp_sva_alloc_vecs
int sva_alloc_vecs(SVA *sva, int nnn);
/* allocate new vectors in SVA */

#define sva_resize_area _glp_sva_resize_area
void sva_resize_area(SVA *sva, int delta);
/* change size of SVA storage */

#define sva_defrag_area _glp_sva_defrag_area
void sva_defrag_area(SVA *sva);
/* defragment left part of SVA */

#define sva_more_space _glp_sva_more_space
void sva_more_space(SVA *sva, int m_size);
/* increase size of middle (free) part of SVA */

#define sva_enlarge_cap _glp_sva_enlarge_cap
void sva_enlarge_cap(SVA *sva, int k, int new_cap, int skip);
/* enlarge capacity of specified vector */

#define sva_reserve_cap _glp_sva_reserve_cap
void sva_reserve_cap(SVA *sva, int k, int new_cap);
/* reserve locations for specified vector */

#define sva_make_static _glp_sva_make_static
void sva_make_static(SVA *sva, int k);
/* relocate specified vector to right part of SVA */

#define sva_check_area _glp_sva_check_area
void sva_check_area(SVA *sva);
/* check sparse vector area (SVA) */

#define sva_delete_area _glp_sva_delete_area
void sva_delete_area(SVA *sva);
/* delete sparse vector area (SVA) */







/***********************************************************************
*  The structure LUF describes sparse LU-factorization.
*
*  The LU-factorization has the following format:
*
*     A = F * V = P * L * U * Q,                                     (1)
*
*     F = P * L * P',                                                (2)
*
*     V = P * U * Q,                                                 (3)
*
*  where A is a given (unsymmetric) square matrix, F and V are matrix
*  factors actually computed, L is a lower triangular matrix with unity
*  diagonal, U is an upper triangular matrix, P and Q are permutation
*  matrices, P' is a matrix transposed to P. All the matrices have the
*  same order n.
*
*  Matrices F and V are stored in both row- and column-wise sparse
*  formats in the associated sparse vector area (SVA). Unity diagonal
*  elements of matrix F are not stored. Pivot elements of matrix V
*  (which correspond to diagonal elements of matrix U) are stored in
*  a separate ordinary array.
*
*  Permutation matrices P and Q are stored in ordinary arrays in both
*  row- and column-like formats.
*
*  Matrices L and U are completely defined by matrices F, V, P, and Q,
*  and therefore not stored explicitly. */

typedef struct LUF LUF;

struct LUF
{     /* sparse LU-factorization */
      int n;
      /* order of matrices A, F, V, P, Q */
      SVA *sva;
      /* associated sparse vector area (SVA) used to store rows and
       * columns of matrices F and V; note that different objects may
       * share the same SVA */
      /*--------------------------------------------------------------*/
      /* matrix F in row-wise format */
      /* during the factorization process this object is not used */
      int fr_ref;
      /* reference number of sparse vector in SVA, which is the first
       * row of matrix F */
#if 0 + 0
      int *fr_ptr = &sva->ptr[fr_ref-1];
      /* fr_ptr[0] is not used;
       * fr_ptr[i], 1 <= i <= n, is pointer to i-th row in SVA */
      int *fr_len = &sva->len[fr_ref-1];
      /* fr_len[0] is not used;
       * fr_len[i], 1 <= i <= n, is length of i-th row */
#endif
      /*--------------------------------------------------------------*/
      /* matrix F in column-wise format */
      /* during the factorization process this object is constructed
         by columns */
      int fc_ref;
      /* reference number of sparse vector in SVA, which is the first
       * column of matrix F */
#if 0 + 0
      int *fc_ptr = &sva->ptr[fc_ref-1];
      /* fc_ptr[0] is not used;
       * fc_ptr[j], 1 <= j <= n, is pointer to j-th column in SVA */
      int *fc_len = &sva->len[fc_ref-1];
      /* fc_len[0] is not used;
       * fc_len[j], 1 <= j <= n, is length of j-th column */
#endif
      /*--------------------------------------------------------------*/
      /* matrix V in row-wise format */
      int vr_ref;
      /* reference number of sparse vector in SVA, which is the first
       * row of matrix V */
#if 0 + 0
      int *vr_ptr = &sva->ptr[vr_ref-1];
      /* vr_ptr[0] is not used;
       * vr_ptr[i], 1 <= i <= n, is pointer to i-th row in SVA */
      int *vr_len = &sva->len[vr_ref-1];
      /* vr_len[0] is not used;
       * vr_len[i], 1 <= i <= n, is length of i-th row */
      int *vr_cap = &sva->cap[vr_ref-1];
      /* vr_cap[0] is not used;
       * vr_cap[i], 1 <= i <= n, is capacity of i-th row */
#endif
      double *vr_piv; /* double vr_piv[1+n]; */
      /* vr_piv[0] is not used;
       * vr_piv[i], 1 <= i <= n, is pivot element of i-th row */
      /*--------------------------------------------------------------*/
      /* matrix V in column-wise format */
      /* during the factorization process this object contains only the
       * patterns (row indices) of columns of the active submatrix */
      int vc_ref;
      /* reference number of sparse vector in SVA, which is the first
       * column of matrix V */
#if 0 + 0
      int *vc_ptr = &sva->ptr[vc_ref-1];
      /* vc_ptr[0] is not used;
       * vc_ptr[j], 1 <= j <= n, is pointer to j-th column in SVA */
      int *vc_len = &sva->len[vc_ref-1];
      /* vc_len[0] is not used;
       * vc_len[j], 1 <= j <= n, is length of j-th column */
      int *vc_cap = &sva->cap[vc_ref-1];
      /* vc_cap[0] is not used;
       * vc_cap[j], 1 <= j <= n, is capacity of j-th column */
#endif
      /*--------------------------------------------------------------*/
      /* matrix P */
      int *pp_ind; /* int pp_ind[1+n]; */
      /* pp_ind[i] = j means that P[i,j] = 1 */
      int *pp_inv; /* int pp_inv[1+n]; */
      /* pp_inv[j] = i means that P[i,j] = 1 */
      /* if i-th row or column of matrix F is i'-th row or column of
       * matrix L, or if i-th row of matrix V is i'-th row of matrix U,
       * then pp_ind[i] = i' and pp_inv[i'] = i */
      /*--------------------------------------------------------------*/
      /* matrix Q */
      int *qq_ind; /* int qq_ind[1+n]; */
      /* qq_ind[i] = j means that Q[i,j] = 1 */
      int *qq_inv; /* int qq_inv[1+n]; */
      /* qq_inv[j] = i means that Q[i,j] = 1 */
      /* if j-th column of matrix V is j'-th column of matrix U, then
       * qq_ind[j'] = j and qq_inv[j] = j' */
};

#define luf_swap_u_rows(i1, i2) \
      do \
      {  int j1, j2; \
         j1 = pp_inv[i1], j2 = pp_inv[i2]; \
         pp_ind[j1] = i2, pp_inv[i2] = j1; \
         pp_ind[j2] = i1, pp_inv[i1] = j2; \
      } while (0)
/* swap rows i1 and i2 of matrix U = P'* V * Q' */

#define luf_swap_u_cols(j1, j2) \
      do \
      {  int i1, i2; \
         i1 = qq_ind[j1], i2 = qq_ind[j2]; \
         qq_ind[j1] = i2, qq_inv[i2] = j1; \
         qq_ind[j2] = i1, qq_inv[i1] = j2; \
      } while (0)
/* swap columns j1 and j2 of matrix U = P'* V * Q' */

#define luf_store_v_cols _glp_luf_store_v_cols
int luf_store_v_cols(LUF *luf, int (*col)(void *info, int j, int ind[],
      double val[]), void *info, int ind[], double val[]);
/* store matrix V = A in column-wise format */

#define luf_check_all _glp_luf_check_all
void luf_check_all(LUF *luf, int k);
/* check LU-factorization before k-th elimination step */

#define luf_build_v_rows _glp_luf_build_v_rows
void luf_build_v_rows(LUF *luf, int len[/*1+n*/]);
/* build matrix V in row-wise format */

#define luf_build_f_rows _glp_luf_build_f_rows
void luf_build_f_rows(LUF *luf, int len[/*1+n*/]);
/* build matrix F in row-wise format */

#define luf_build_v_cols _glp_luf_build_v_cols
void luf_build_v_cols(LUF *luf, int updat, int len[/*1+n*/]);
/* build matrix V in column-wise format */

#define luf_check_f_rc _glp_luf_check_f_rc
void luf_check_f_rc(LUF *luf);
/* check rows and columns of matrix F */

#define luf_check_v_rc _glp_luf_check_v_rc
void luf_check_v_rc(LUF *luf);
/* check rows and columns of matrix V */

#define luf_f_solve _glp_luf_f_solve
void luf_f_solve(LUF *luf, double x[/*1+n*/]);
/* solve system F * x = b */

#define luf_ft_solve _glp_luf_ft_solve
void luf_ft_solve(LUF *luf, double x[/*1+n*/]);
/* solve system F' * x = b */

#define luf_v_solve _glp_luf_v_solve
void luf_v_solve(LUF *luf, double b[/*1+n*/], double x[/*1+n*/]);
/* solve system V * x = b */

#define luf_vt_solve _glp_luf_vt_solve
void luf_vt_solve(LUF *luf, double b[/*1+n*/], double x[/*1+n*/]);
/* solve system V' * x = b */

#define luf_vt_solve1 _glp_luf_vt_solve1
void luf_vt_solve1(LUF *luf, double e[/*1+n*/], double y[/*1+n*/]);
/* solve system V' * y = e' to cause growth in y */

#define luf_estimate_norm _glp_luf_estimate_norm
double luf_estimate_norm(LUF *luf, double w1[/*1+n*/], double
      w2[/*1+n*/]);
/* estimate 1-norm of inv(A) */

/***********************************************************************
*  luf_store_v_cols - store matrix V = A in column-wise format
*
*  This routine stores matrix V = A in column-wise format, where A is
*  the original matrix to be factorized.
*
*  On exit the routine returns the number of non-zeros in matrix V. */

int luf_store_v_cols(LUF *luf, int (*col)(void *info, int j, int ind[],
      double val[]), void *info, int ind[], double val[])
{     int n = luf->n;
      SVA *sva = luf->sva;
      int *sv_ind = sva->ind;
      double *sv_val = sva->val;
      int vc_ref = luf->vc_ref;
      int *vc_ptr = &sva->ptr[vc_ref-1];
      int *vc_len = &sva->len[vc_ref-1];
      int *vc_cap = &sva->cap[vc_ref-1];
      int j, len, ptr, nnz;
      nnz = 0;
      for (j = 1; j <= n; j++)
      {  /* get j-th column */
         len = col(info, j, ind, val);
         xassert(0 <= len && len <= n);
         /* enlarge j-th column capacity */
         if (vc_cap[j] < len)
         {  if (sva->r_ptr - sva->m_ptr < len)
            {  sva_more_space(sva, len);
               sv_ind = sva->ind;
               sv_val = sva->val;
            }
            sva_enlarge_cap(sva, vc_ref-1+j, len, 0);
         }
         /* store j-th column */
         ptr = vc_ptr[j];
         memcpy(&sv_ind[ptr], &ind[1], len * sizeof(int));
         memcpy(&sv_val[ptr], &val[1], len * sizeof(double));
         vc_len[j] = len;
         nnz += len;
      }
      return nnz;
}

/***********************************************************************
*  luf_check_all - check LU-factorization before k-th elimination step
*
*  This routine checks that before performing k-th elimination step,
*  1 <= k <= n+1, all components of the LU-factorization are correct.
*
*  In case of k = n+1, i.e. after last elimination step, it is assumed
*  that rows of F and columns of V are *not* built yet.
*
*  NOTE: For testing/debugging only. */

void luf_check_all(LUF *luf, int k)
{     int n = luf->n;
      SVA *sva = luf->sva;
      int *sv_ind = sva->ind;
      double *sv_val = sva->val;
      int fr_ref = luf->fr_ref;
      int *fr_len = &sva->len[fr_ref-1];
      int fc_ref = luf->fc_ref;
      int *fc_ptr = &sva->ptr[fc_ref-1];
      int *fc_len = &sva->len[fc_ref-1];
      int vr_ref = luf->vr_ref;
      int *vr_ptr = &sva->ptr[vr_ref-1];
      int *vr_len = &sva->len[vr_ref-1];
      int vc_ref = luf->vc_ref;
      int *vc_ptr = &sva->ptr[vc_ref-1];
      int *vc_len = &sva->len[vc_ref-1];
      int *pp_ind = luf->pp_ind;
      int *pp_inv = luf->pp_inv;
      int *qq_ind = luf->qq_ind;
      int *qq_inv = luf->qq_inv;
      int i, ii, i_ptr, i_end, j, jj, j_ptr, j_end;
      xassert(n > 0);
      xassert(1 <= k && k <= n+1);
      /* check permutation matrix P */
      for (i = 1; i <= n; i++)
      {  ii = pp_ind[i];
         xassert(1 <= ii && ii <= n);
         xassert(pp_inv[ii] == i);
      }
      /* check permutation matrix Q */
      for (j = 1; j <= n; j++)
      {  jj = qq_inv[j];
         xassert(1 <= jj && jj <= n);
         xassert(qq_ind[jj] == j);
      }
      /* check row-wise representation of matrix F */
      for (i = 1; i <= n; i++)
         xassert(fr_len[i] == 0);
      /* check column-wise representation of matrix F */
      for (j = 1; j <= n; j++)
      {  /* j-th column of F = jj-th column of L */
         jj = pp_ind[j];
         if (jj < k)
         {  j_ptr = fc_ptr[j];
            j_end = j_ptr + fc_len[j];
            for (; j_ptr < j_end; j_ptr++)
            {  i = sv_ind[j_ptr];
               xassert(1 <= i && i <= n);
               ii = pp_ind[i]; /* f[i,j] = l[ii,jj] */
               xassert(ii > jj);
               xassert(sv_val[j_ptr] != 0.0);
            }
         }
         else /* jj >= k */
            xassert(fc_len[j] == 0);
      }
      /* check row-wise representation of matrix V */
      for (i = 1; i <= n; i++)
      {  /* i-th row of V = ii-th row of U */
         ii = pp_ind[i];
         i_ptr = vr_ptr[i];
         i_end = i_ptr + vr_len[i];
         for (; i_ptr < i_end; i_ptr++)
         {  j = sv_ind[i_ptr];
            xassert(1 <= j && j <= n);
            jj = qq_inv[j]; /* v[i,j] = u[ii,jj] */
            if (ii < k)
               xassert(jj > ii);
            else /* ii >= k */
            {  xassert(jj >= k);
               /* find v[i,j] in j-th column */
               j_ptr = vc_ptr[j];
               j_end = j_ptr + vc_len[j];
               for (; sv_ind[j_ptr] != i; j_ptr++)
                  /* nop */;
               xassert(j_ptr < j_end);
            }
            xassert(sv_val[i_ptr] != 0.0);
         }
      }
      /* check column-wise representation of matrix V */
      for (j = 1; j <= n; j++)
      {  /* j-th column of V = jj-th column of U */
         jj = qq_inv[j];
         if (jj < k)
            xassert(vc_len[j] == 0);
         else /* jj >= k */
         {  j_ptr = vc_ptr[j];
            j_end = j_ptr + vc_len[j];
            for (; j_ptr < j_end; j_ptr++)
            {  i = sv_ind[j_ptr];
               ii = pp_ind[i]; /* v[i,j] = u[ii,jj] */
               xassert(ii >= k);
               /* find v[i,j] in i-th row */
               i_ptr = vr_ptr[i];
               i_end = i_ptr + vr_len[i];
               for (; sv_ind[i_ptr] != j; i_ptr++)
                  /* nop */;
               xassert(i_ptr < i_end);
            }
         }
      }
      return;
}

/***********************************************************************
*  luf_build_v_rows - build matrix V in row-wise format
*
*  This routine builds the row-wise representation of matrix V in the
*  left part of SVA using its column-wise representation.
*
*  NOTE: On entry to the routine all rows of matrix V should have zero
*        capacity.
*
*  The working array len should have at least 1+n elements (len[0] is
*  not used). */

void luf_build_v_rows(LUF *luf, int len[/*1+n*/])
{     int n = luf->n;
      SVA *sva = luf->sva;
      int *sv_ind = sva->ind;
      double *sv_val = sva->val;
      int vr_ref = luf->vr_ref;
      int *vr_ptr = &sva->ptr[vr_ref-1];
      int *vr_len = &sva->len[vr_ref-1];
      int vc_ref = luf->vc_ref;
      int *vc_ptr = &sva->ptr[vc_ref-1];
      int *vc_len = &sva->len[vc_ref-1];
      int i, j, end, nnz, ptr, ptr1;
      /* calculate the number of non-zeros in each row of matrix V and
       * the total number of non-zeros */
      nnz = 0;
      for (i = 1; i <= n; i++)
         len[i] = 0;
      for (j = 1; j <= n; j++)
      {  nnz += vc_len[j];
         for (end = (ptr = vc_ptr[j]) + vc_len[j]; ptr < end; ptr++)
            len[sv_ind[ptr]]++;
      }
      /* we need at least nnz free locations in SVA */
      if (sva->r_ptr - sva->m_ptr < nnz)
      {  sva_more_space(sva, nnz);
         sv_ind = sva->ind;
         sv_val = sva->val;
      }
      /* reserve locations for rows of matrix V */
      for (i = 1; i <= n; i++)
      {  if (len[i] > 0)
            sva_enlarge_cap(sva, vr_ref-1+i, len[i], 0);
         vr_len[i] = len[i];
      }
      /* walk thru column of matrix V and build its rows */
      for (j = 1; j <= n; j++)
      {  for (end = (ptr = vc_ptr[j]) + vc_len[j]; ptr < end; ptr++)
         {  i = sv_ind[ptr];
            sv_ind[ptr1 = vr_ptr[i] + (--len[i])] = j;
            sv_val[ptr1] = sv_val[ptr];
         }
      }
      return;
}

/***********************************************************************
*  luf_build_f_rows - build matrix F in row-wise format
*
*  This routine builds the row-wise representation of matrix F in the
*  right part of SVA using its column-wise representation.
*
*  NOTE: On entry to the routine all rows of matrix F should have zero
*        capacity.
*
*  The working array len should have at least 1+n elements (len[0] is
*  not used). */

void luf_build_f_rows(LUF *luf, int len[/*1+n*/])
{     int n = luf->n;
      SVA *sva = luf->sva;
      int *sv_ind = sva->ind;
      double *sv_val = sva->val;
      int fr_ref = luf->fr_ref;
      int *fr_ptr = &sva->ptr[fr_ref-1];
      int *fr_len = &sva->len[fr_ref-1];
      int fc_ref = luf->fc_ref;
      int *fc_ptr = &sva->ptr[fc_ref-1];
      int *fc_len = &sva->len[fc_ref-1];
      int i, j, end, nnz, ptr, ptr1;
      /* calculate the number of non-zeros in each row of matrix F and
       * the total number of non-zeros (except diagonal elements) */
      nnz = 0;
      for (i = 1; i <= n; i++)
         len[i] = 0;
      for (j = 1; j <= n; j++)
      {  nnz += fc_len[j];
         for (end = (ptr = fc_ptr[j]) + fc_len[j]; ptr < end; ptr++)
            len[sv_ind[ptr]]++;
      }
      /* we need at least nnz free locations in SVA */
      if (sva->r_ptr - sva->m_ptr < nnz)
      {  sva_more_space(sva, nnz);
         sv_ind = sva->ind;
         sv_val = sva->val;
      }
      /* reserve locations for rows of matrix F */
      for (i = 1; i <= n; i++)
      {  if (len[i] > 0)
            sva_reserve_cap(sva, fr_ref-1+i, len[i]);
         fr_len[i] = len[i];
      }
      /* walk through columns of matrix F and build its rows */
      for (j = 1; j <= n; j++)
      {  for (end = (ptr = fc_ptr[j]) + fc_len[j]; ptr < end; ptr++)
         {  i = sv_ind[ptr];
            sv_ind[ptr1 = fr_ptr[i] + (--len[i])] = j;
            sv_val[ptr1] = sv_val[ptr];
         }
      }
      return;
}

/***********************************************************************
*  luf_build_v_cols - build matrix V in column-wise format
*
*  This routine builds the column-wise representation of matrix V in
*  the left (if the flag updat is set) or right (if the flag updat is
*  clear) part of SVA using its row-wise representation.
*
*  NOTE: On entry to the routine all columns of matrix V should have
*        zero capacity.
*
*  The working array len should have at least 1+n elements (len[0] is
*  not used). */

void luf_build_v_cols(LUF *luf, int updat, int len[/*1+n*/])
{     int n = luf->n;
      SVA *sva = luf->sva;
      int *sv_ind = sva->ind;
      double *sv_val = sva->val;
      int vr_ref = luf->vr_ref;
      int *vr_ptr = &sva->ptr[vr_ref-1];
      int *vr_len = &sva->len[vr_ref-1];
      int vc_ref = luf->vc_ref;
      int *vc_ptr = &sva->ptr[vc_ref-1];
      int *vc_len = &sva->len[vc_ref-1];
      int i, j, end, nnz, ptr, ptr1;
      /* calculate the number of non-zeros in each column of matrix V
       * and the total number of non-zeros (except pivot elements) */
      nnz = 0;
      for (j = 1; j <= n; j++)
         len[j] = 0;
      for (i = 1; i <= n; i++)
      {  nnz += vr_len[i];
         for (end = (ptr = vr_ptr[i]) + vr_len[i]; ptr < end; ptr++)
            len[sv_ind[ptr]]++;
      }
      /* we need at least nnz free locations in SVA */
      if (sva->r_ptr - sva->m_ptr < nnz)
      {  sva_more_space(sva, nnz);
         sv_ind = sva->ind;
         sv_val = sva->val;
      }
      /* reserve locations for columns of matrix V */
      for (j = 1; j <= n; j++)
      {  if (len[j] > 0)
         {  if (updat)
               sva_enlarge_cap(sva, vc_ref-1+j, len[j], 0);
            else
               sva_reserve_cap(sva, vc_ref-1+j, len[j]);
         }
         vc_len[j] = len[j];
      }
      /* walk through rows of matrix V and build its columns */
      for (i = 1; i <= n; i++)
      {  for (end = (ptr = vr_ptr[i]) + vr_len[i]; ptr < end; ptr++)
         {  j = sv_ind[ptr];
            sv_ind[ptr1 = vc_ptr[j] + (--len[j])] = i;
            sv_val[ptr1] = sv_val[ptr];
         }
      }
      return;
}

/***********************************************************************
*  luf_check_f_rc - check rows and columns of matrix F
*
*  This routine checks that the row- and column-wise representations
*  of matrix F are identical.
*
*  NOTE: For testing/debugging only. */

void luf_check_f_rc(LUF *luf)
{     int n = luf->n;
      SVA *sva = luf->sva;
      int *sv_ind = sva->ind;
      double *sv_val = sva->val;
      int fr_ref = luf->fr_ref;
      int *fr_ptr = &sva->ptr[fr_ref-1];
      int *fr_len = &sva->len[fr_ref-1];
      int fc_ref = luf->fc_ref;
      int *fc_ptr = &sva->ptr[fc_ref-1];
      int *fc_len = &sva->len[fc_ref-1];
      int i, i_end, i_ptr, j, j_end, j_ptr;
      /* walk thru rows of matrix F */
      for (i = 1; i <= n; i++)
      {  for (i_end = (i_ptr = fr_ptr[i]) + fr_len[i];
            i_ptr < i_end; i_ptr++)
         {  j = sv_ind[i_ptr];
            /* find element f[i,j] in j-th column of matrix F */
            for (j_end = (j_ptr = fc_ptr[j]) + fc_len[j];
               sv_ind[j_ptr] != i; j_ptr++)
               /* nop */;
            xassert(j_ptr < j_end);
            xassert(sv_val[i_ptr] == sv_val[j_ptr]);
            /* mark element f[i,j] */
            sv_ind[j_ptr] = -i;
         }
      }
      /* walk thru column of matix F and check that all elements has
         been marked */
      for (j = 1; j <= n; j++)
      {  for (j_end = (j_ptr = fc_ptr[j]) + fc_len[j];
            j_ptr < j_end; j_ptr++)
         {  xassert((i = sv_ind[j_ptr]) < 0);
            /* unmark element f[i,j] */
            sv_ind[j_ptr] = -i;
         }
      }
      return;
}

/***********************************************************************
*  luf_check_v_rc - check rows and columns of matrix V
*
*  This routine checks that the row- and column-wise representations
*  of matrix V are identical.
*
*  NOTE: For testing/debugging only. */

void luf_check_v_rc(LUF *luf)
{     int n = luf->n;
      SVA *sva = luf->sva;
      int *sv_ind = sva->ind;
      double *sv_val = sva->val;
      int vr_ref = luf->vr_ref;
      int *vr_ptr = &sva->ptr[vr_ref-1];
      int *vr_len = &sva->len[vr_ref-1];
      int vc_ref = luf->vc_ref;
      int *vc_ptr = &sva->ptr[vc_ref-1];
      int *vc_len = &sva->len[vc_ref-1];
      int i, i_end, i_ptr, j, j_end, j_ptr;
      /* walk thru rows of matrix V */
      for (i = 1; i <= n; i++)
      {  for (i_end = (i_ptr = vr_ptr[i]) + vr_len[i];
            i_ptr < i_end; i_ptr++)
         {  j = sv_ind[i_ptr];
            /* find element v[i,j] in j-th column of matrix V */
            for (j_end = (j_ptr = vc_ptr[j]) + vc_len[j];
               sv_ind[j_ptr] != i; j_ptr++)
               /* nop */;
            xassert(j_ptr < j_end);
            xassert(sv_val[i_ptr] == sv_val[j_ptr]);
            /* mark element v[i,j] */
            sv_ind[j_ptr] = -i;
         }
      }
      /* walk thru column of matix V and check that all elements has
         been marked */
      for (j = 1; j <= n; j++)
      {  for (j_end = (j_ptr = vc_ptr[j]) + vc_len[j];
            j_ptr < j_end; j_ptr++)
         {  xassert((i = sv_ind[j_ptr]) < 0);
            /* unmark element v[i,j] */
            sv_ind[j_ptr] = -i;
         }
      }
      return;
}

/***********************************************************************
*  luf_f_solve - solve system F * x = b
*
*  This routine solves the system F * x = b, where the matrix F is the
*  left factor of the sparse LU-factorization.
*
*  On entry the array x should contain elements of the right-hand side
*  vector b in locations x[1], ..., x[n], where n is the order of the
*  matrix F. On exit this array will contain elements of the solution
*  vector x in the same locations. */

void luf_f_solve(LUF *luf, double x[/*1+n*/])
{     int n = luf->n;
      SVA *sva = luf->sva;
      int *sv_ind = sva->ind;
      double *sv_val = sva->val;
      int fc_ref = luf->fc_ref;
      int *fc_ptr = &sva->ptr[fc_ref-1];
      int *fc_len = &sva->len[fc_ref-1];
      int *pp_inv = luf->pp_inv;
      int j, k, ptr, end;
      double x_j;
      for (k = 1; k <= n; k++)
      {  /* k-th column of L = j-th column of F */
         j = pp_inv[k];
         /* x[j] is already computed */
         /* walk thru j-th column of matrix F and substitute x[j] into
          * other equations */
         if ((x_j = x[j]) != 0.0)
         {  for (end = (ptr = fc_ptr[j]) + fc_len[j]; ptr < end; ptr++)
               x[sv_ind[ptr]] -= sv_val[ptr] * x_j;
         }
      }
      return;
}

/***********************************************************************
*  luf_ft_solve - solve system F' * x = b
*
*  This routine solves the system F' * x = b, where F' is a matrix
*  transposed to the matrix F, which is the left factor of the sparse
*  LU-factorization.
*
*  On entry the array x should contain elements of the right-hand side
*  vector b in locations x[1], ..., x[n], where n is the order of the
*  matrix F. On exit this array will contain elements of the solution
*  vector x in the same locations. */

void luf_ft_solve(LUF *luf, double x[/*1+n*/])
{     int n = luf->n;
      SVA *sva = luf->sva;
      int *sv_ind = sva->ind;
      double *sv_val = sva->val;
      int fr_ref = luf->fr_ref;
      int *fr_ptr = &sva->ptr[fr_ref-1];
      int *fr_len = &sva->len[fr_ref-1];
      int *pp_inv = luf->pp_inv;
      int i, k, ptr, end;
      double x_i;
      for (k = n; k >= 1; k--)
      {  /* k-th column of L' = i-th row of F */
         i = pp_inv[k];
         /* x[i] is already computed */
         /* walk thru i-th row of matrix F and substitute x[i] into
          * other equations */
         if ((x_i = x[i]) != 0.0)
         {  for (end = (ptr = fr_ptr[i]) + fr_len[i]; ptr < end; ptr++)
               x[sv_ind[ptr]] -= sv_val[ptr] * x_i;
         }
      }
      return;
}

/***********************************************************************
*  luf_v_solve - solve system V * x = b
*
*  This routine solves the system V * x = b, where the matrix V is the
*  right factor of the sparse LU-factorization.
*
*  On entry the array b should contain elements of the right-hand side
*  vector b in locations b[1], ..., b[n], where n is the order of the
*  matrix V. On exit the array x will contain elements of the solution
*  vector x in locations x[1], ..., x[n]. Note that the array b will be
*  clobbered on exit. */

void luf_v_solve(LUF *luf, double b[/*1+n*/], double x[/*1+n*/])
{     int n = luf->n;
      SVA *sva = luf->sva;
      int *sv_ind = sva->ind;
      double *sv_val = sva->val;
      double *vr_piv = luf->vr_piv;
      int vc_ref = luf->vc_ref;
      int *vc_ptr = &sva->ptr[vc_ref-1];
      int *vc_len = &sva->len[vc_ref-1];
      int *pp_inv = luf->pp_inv;
      int *qq_ind = luf->qq_ind;
      int i, j, k, ptr, end;
      double x_j;
      for (k = n; k >= 1; k--)
      {  /* k-th row of U = i-th row of V */
         /* k-th column of U = j-th column of V */
         i = pp_inv[k];
         j = qq_ind[k];
         /* compute x[j] = b[i] / u[k,k], where u[k,k] = v[i,j];
          * walk through j-th column of matrix V and substitute x[j]
          * into other equations */
         if ((x_j = x[j] = b[i] / vr_piv[i]) != 0.0)
         {  for (end = (ptr = vc_ptr[j]) + vc_len[j]; ptr < end; ptr++)
               b[sv_ind[ptr]] -= sv_val[ptr] * x_j;
         }
      }
      return;
}

/***********************************************************************
*  luf_vt_solve - solve system V' * x = b
*
*  This routine solves the system V' * x = b, where V' is a matrix
*  transposed to the matrix V, which is the right factor of the sparse
*  LU-factorization.
*
*  On entry the array b should contain elements of the right-hand side
*  vector b in locations b[1], ..., b[n], where n is the order of the
*  matrix V. On exit the array x will contain elements of the solution
*  vector x in locations x[1], ..., x[n]. Note that the array b will be
*  clobbered on exit. */

void luf_vt_solve(LUF *luf, double b[/*1+n*/], double x[/*1+n*/])
{     int n = luf->n;
      SVA *sva = luf->sva;
      int *sv_ind = sva->ind;
      double *sv_val = sva->val;
      double *vr_piv = luf->vr_piv;
      int vr_ref = luf->vr_ref;
      int *vr_ptr = &sva->ptr[vr_ref-1];
      int *vr_len = &sva->len[vr_ref-1];
      int *pp_inv = luf->pp_inv;
      int *qq_ind = luf->qq_ind;
      int i, j, k, ptr, end;
      double x_i;
      for (k = 1; k <= n; k++)
      {  /* k-th row of U' = j-th column of V */
         /* k-th column of U' = i-th row of V */
         i = pp_inv[k];
         j = qq_ind[k];
         /* compute x[i] = b[j] / u'[k,k], where u'[k,k] = v[i,j];
          * walk through i-th row of matrix V and substitute x[i] into
          * other equations */
         if ((x_i = x[i] = b[j] / vr_piv[i]) != 0.0)
         {  for (end = (ptr = vr_ptr[i]) + vr_len[i]; ptr < end; ptr++)
               b[sv_ind[ptr]] -= sv_val[ptr] * x_i;
         }
      }
      return;
}

/***********************************************************************
*  luf_vt_solve1 - solve system V' * y = e' to cause growth in y
*
*  This routine is a special version of luf_vt_solve. It solves the
*  system V'* y = e' = e + delta e, where V' is a matrix transposed to
*  the matrix V, e is the specified right-hand side vector, and delta e
*  is a vector of +1 and -1 chosen to cause growth in the solution
*  vector y.
*
*  On entry the array e should contain elements of the right-hand side
*  vector e in locations e[1], ..., e[n], where n is the order of the
*  matrix V. On exit the array y will contain elements of the solution
*  vector y in locations y[1], ..., y[n]. Note that the array e will be
*  clobbered on exit. */

void luf_vt_solve1(LUF *luf, double e[/*1+n*/], double y[/*1+n*/])
{     int n = luf->n;
      SVA *sva = luf->sva;
      int *sv_ind = sva->ind;
      double *sv_val = sva->val;
      double *vr_piv = luf->vr_piv;
      int vr_ref = luf->vr_ref;
      int *vr_ptr = &sva->ptr[vr_ref-1];
      int *vr_len = &sva->len[vr_ref-1];
      int *pp_inv = luf->pp_inv;
      int *qq_ind = luf->qq_ind;
      int i, j, k, ptr, end;
      double e_j, y_i;
      for (k = 1; k <= n; k++)
      {  /* k-th row of U' = j-th column of V */
         /* k-th column of U' = i-th row of V */
         i = pp_inv[k];
         j = qq_ind[k];
         /* determine e'[j] = e[j] + delta e[j] */
         e_j = (e[j] >= 0.0 ? e[j] + 1.0 : e[j] - 1.0);
         /* compute y[i] = e'[j] / u'[k,k], where u'[k,k] = v[i,j] */
         y_i = y[i] = e_j / vr_piv[i];
         /* walk through i-th row of matrix V and substitute y[i] into
          * other equations */
         for (end = (ptr = vr_ptr[i]) + vr_len[i]; ptr < end; ptr++)
            e[sv_ind[ptr]] -= sv_val[ptr] * y_i;
      }
      return;
}

/***********************************************************************
*  luf_estimate_norm - estimate 1-norm of inv(A)
*
*  This routine estimates 1-norm of inv(A) by one step of inverse
*  iteration for the small singular vector as described in [1]. This
*  involves solving two systems of equations:
*
*     A'* y = e,
*
*     A * z = y,
*
*  where A' is a matrix transposed to A, and e is a vector of +1 and -1
*  chosen to cause growth in y. Then
*
*     estimate 1-norm of inv(A) = (1-norm of z) / (1-norm of y)
*
*  REFERENCES
*
*  1. G.E.Forsythe, M.A.Malcolm, C.B.Moler. Computer Methods for
*     Mathematical Computations. Prentice-Hall, Englewood Cliffs, N.J.,
*     pp. 30-62 (subroutines DECOMP and SOLVE). */

double luf_estimate_norm(LUF *luf, double w1[/*1+n*/], double
      w2[/*1+n*/])
{     int n = luf->n;
      double *e = w1;
      double *y = w2;
      double *z = w1;
      int i;
      double y_norm, z_norm;
      /* y = inv(A') * e = inv(F') * inv(V') * e */
      /* compute y' = inv(V') * e to cause growth in y' */
      for (i = 1; i <= n; i++)
         e[i] = 0.0;
      luf_vt_solve1(luf, e, y);
      /* compute y = inv(F') * y' */
      luf_ft_solve(luf, y);
      /* compute 1-norm of y = sum |y[i]| */
      y_norm = 0.0;
      for (i = 1; i <= n; i++)
         y_norm += (y[i] >= 0.0 ? +y[i] : -y[i]);
      /* z = inv(A) * y = inv(V) * inv(F) * y */
      /* compute z' = inv(F) * y */
      luf_f_solve(luf, y);
      /* compute z = inv(V) * z' */
      luf_v_solve(luf, y, z);
      /* compute 1-norm of z = sum |z[i]| */
      z_norm = 0.0;
      for (i = 1; i <= n; i++)
         z_norm += (z[i] >= 0.0 ? +z[i] : -z[i]);
      /* estimate 1-norm of inv(A) = (1-norm of z) / (1-norm of y) */
      return z_norm / y_norm;
}



typedef struct SGF SGF;

struct SGF
{     /* sparse Gaussian factorizer workspace */
      LUF *luf;
      /* LU-factorization being computed */
      /*--------------------------------------------------------------*/
      /* to efficiently choose pivot elements according to Markowitz
       * strategy, the search technique proposed by Iain Duff is used;
       * it is based on using two families of sets {R[0], ..., R[n]}
       * and {C[0], ..., C[n]}, where R[k] and C[k], 0 <= k <= n, are,
       * respectively, sets of rows and columns of the active submatrix
       * of matrix V having k non-zeros (i.e. whose length is k); each
       * set R[k] and C[k] is implemented as a doubly linked list */
      int *rs_head; /* int rs_head[1+n]; */
      /* rs_head[k], 0 <= k <= n, is the number of first row, which
       * has k non-zeros in the active submatrix */
      int *rs_prev; /* int rs_prev[1+n]; */
      /* rs_prev[0] is not used;
       * rs_prev[i], 1 <= i <= n, is the number of previous row, which
       * has the same number of non-zeros as i-th row;
       * rs_prev[i] < 0 means that i-th row is inactive */
      int *rs_next; /* int rs_next[1+n]; */
      /* rs_next[0] is not used;
       * rs_next[i], 1 <= i <= n, is the number of next row, which has
       * the same number of non-zeros as i-th row;
       * rs_next[i] < 0 means that i-th row is inactive */
      int *cs_head; /* int cs_head[1+n]; */
      /* cs_head[k], 0 <= k <= n, is the number of first column, which
       * has k non-zeros in the active submatrix */
      int *cs_prev; /* int cs_prev[1+n]; */
      /* cs_prev[0] is not used;
       * cs_prev[j], 1 <= j <= n, is the number of previous column,
       * which has the same number of non-zeros as j-th column;
       * cs_prev[j] < 0 means that j-th column is inactive */
      int *cs_next; /* int cs_next[1+n]; */
      /* cs_next[0] is not used;
       * cs_next[j], 1 <= j <= n, is the number of next column, which
       * has the same number of non-zeros as j-th column;
       * cs_next[j] < 0 means that j-th column is inactive */
      /* NOTE: cs_prev[j] = cs_next[j] = j means that j-th column was
       *       temporarily removed from corresponding set C[k] by the
       *       pivoting routine according to Uwe Suhl's heuristic */
      /*--------------------------------------------------------------*/
      /* working arrays */
      double *vr_max; /* int vr_max[1+n]; */
      /* vr_max[0] is not used;
       * vr_max[i], 1 <= i <= n, is used only if i-th row of matrix V
       * is active (i.e. belongs to the active submatrix), and is the
       * largest magnitude of elements in that row; if vr_max[i] < 0,
       * the largest magnitude is unknown yet */
      char *flag; /* char flag[1+n]; */
      /* boolean working array */
      double *work; /* double work[1+n]; */
      /* floating-point working array */
      /*--------------------------------------------------------------*/
      /* control parameters */
      int updat;
      /* if this flag is set, the matrix V is assumed to be updatable;
       * in this case factorized (non-active) part of V is stored in
       * the left part of SVA rather than in its right part */
      double piv_tol;
      /* threshold pivoting tolerance, 0 < piv_tol < 1; element v[i,j]
       * of the active submatrix fits to be pivot if it satisfies to
       * the stability criterion |v[i,j]| >= piv_tol * max |v[i,*]|,
       * i.e. if it is not very small in the magnitude among other
       * elements in the same row; decreasing this parameter gives
       * better sparsity at the expense of numerical accuracy and vice
       * versa */
      int piv_lim;
      /* maximal allowable number of pivot candidates to be considered;
       * if piv_lim pivot candidates have been considered, the pivoting
       * routine terminates the search with the best candidate found */
      int suhl;
      /* if this flag is set, the pivoting routine applies a heuristic
       * proposed by Uwe Suhl: if a column of the active submatrix has
       * no eligible pivot candidates (i.e. all its elements do not
       * satisfy to the stability criterion), the routine excludes it
       * from futher consideration until it becomes column singleton;
       * in many cases this allows reducing the time needed to choose
       * the pivot */
      double eps_tol;
      /* epsilon tolerance; each element of the active submatrix, whose
       * magnitude is less than eps_tol, is replaced by exact zero */
#if 0 /* FIXME */
      double den_lim;
      /* density limit; if the density of the active submatrix reaches
       * this limit, the factorization routine switches from sparse to
       * dense mode */
#endif
};

#define sgf_activate_row(i) \
      do \
      {  int len = vr_len[i]; \
         rs_prev[i] = 0; \
         rs_next[i] = rs_head[len]; \
         if (rs_next[i] != 0) \
            rs_prev[rs_next[i]] = i; \
         rs_head[len] = i; \
      } while (0)
/* include i-th row of matrix V in active set R[len] */

#define sgf_deactivate_row(i) \
      do \
      {  if (rs_prev[i] == 0) \
            rs_head[vr_len[i]] = rs_next[i]; \
         else \
            rs_next[rs_prev[i]] = rs_next[i]; \
         if (rs_next[i] == 0) \
            ; \
         else \
            rs_prev[rs_next[i]] = rs_prev[i]; \
         rs_prev[i] = rs_next[i] = -1; \
      } while (0)
/* remove i-th row of matrix V from active set R[len] */

#define sgf_activate_col(j) \
      do \
      {  int len = vc_len[j]; \
         cs_prev[j] = 0; \
         cs_next[j] = cs_head[len]; \
         if (cs_next[j] != 0) \
            cs_prev[cs_next[j]] = j; \
         cs_head[len] = j; \
      } while (0)
/* include j-th column of matrix V in active set C[len] */

#define sgf_deactivate_col(j) \
      do \
      {  if (cs_prev[j] == 0) \
            cs_head[vc_len[j]] = cs_next[j]; \
         else \
            cs_next[cs_prev[j]] = cs_next[j]; \
         if (cs_next[j] == 0) \
            ; \
         else \
            cs_prev[cs_next[j]] = cs_prev[j]; \
         cs_prev[j] = cs_next[j] = -1; \
      } while (0)
/* remove j-th column of matrix V from active set C[len] */

#define sgf_reduce_nuc _glp_sgf_reduce_nuc
int sgf_reduce_nuc(LUF *luf, int *k1, int *k2, int cnt[/*1+n*/],
      int list[/*1+n*/]);
/* initial reordering to minimize nucleus size */

#define sgf_singl_phase _glp_sgf_singl_phase
int sgf_singl_phase(LUF *luf, int k1, int k2, int updat,
      int ind[/*1+n*/], double val[/*1+n*/]);
/* compute LU-factorization (singleton phase) */

#define sgf_choose_pivot _glp_sgf_choose_pivot
int sgf_choose_pivot(SGF *sgf, int *p, int *q);
/* choose pivot element v[p,q] */

#define sgf_eliminate _glp_sgf_eliminate
int sgf_eliminate(SGF *sgf, int p, int q);
/* perform gaussian elimination */

#define sgf_dense_lu _glp_sgf_dense_lu
int sgf_dense_lu(int n, double a[], int r[], int c[], double eps);
/* compute dense LU-factorization with full pivoting */

#define sgf_dense_phase _glp_sgf_dense_phase
int sgf_dense_phase(LUF *luf, int k, int updat);
/* compute LU-factorization (dense phase) */

#define sgf_factorize _glp_sgf_factorize
int sgf_factorize(SGF *sgf, int singl);
/* compute LU-factorization (main routine) */


/***********************************************************************
*  sgf_reduce_nuc - initial reordering to minimize nucleus size
*
*  On entry to this routine it is assumed that V = A and F = P = Q = I,
*  where A is the original matrix to be factorized. It is also assumed
*  that matrix V = A is stored in both row- and column-wise formats.
*
*  This routine performs (implicit) non-symmetric permutations of rows
*  and columns of matrix U = P'* V * Q' to reduce it to the form:
*
*        1     k1    k2    n
*     1  x x x x x x x x x x
*        . x x x x x x x x x
*        . . x x x x x x x x
*     k1 . . . * * * * x x x
*        . . . * * * * x x x
*        . . . * * * * x x x
*     k2 . . . * * * * x x x
*        . . . . . . . x x x
*        . . . . . . . . x x
*     n  . . . . . . . . . x
*
*  where non-zeros in rows and columns k1, k1+1, ..., k2 constitute so
*  called nucleus ('*'), whose size is minimized by the routine.
*
*  The numbers k1 and k2 are returned by the routine on exit. Usually,
*  if the nucleus exists, 1 <= k1 < k2 <= n. However, if the resultant
*  matrix U is upper triangular (has no nucleus), k1 = n+1 and k2 = n.
*
*  Note that the routines sgf_choose_pivot and sgf_eliminate perform
*  exactly the same transformations (by processing row and columns
*  singletons), so preliminary minimization of the nucleus may not be
*  used. However, processing row and column singletons by the routines
*  sgf_minimize_nuc and sgf_singl_phase is more efficient. */

#if 1 /* 21/II-2016 */
/* Normally this routine returns zero. If the matrix is structurally
*  singular, the routine returns non-zero. */
#endif

int sgf_reduce_nuc(LUF *luf, int *k1_, int *k2_, int cnt[/*1+n*/],
      int list[/*1+n*/])
{     int n = luf->n;
      SVA *sva = luf->sva;
      int *sv_ind = sva->ind;
      int vr_ref = luf->vr_ref;
      int *vr_ptr = &sva->ptr[vr_ref-1];
      int *vr_len = &sva->len[vr_ref-1];
      int vc_ref = luf->vc_ref;
      int *vc_ptr = &sva->ptr[vc_ref-1];
      int *vc_len = &sva->len[vc_ref-1];
      int *pp_ind = luf->pp_ind;
      int *pp_inv = luf->pp_inv;
      int *qq_ind = luf->qq_ind;
      int *qq_inv = luf->qq_inv;
      int i, ii, j, jj, k1, k2, ns, ptr, end;
      /* initial nucleus is U = V = A */
      k1 = 1, k2 = n;
      /*--------------------------------------------------------------*/
      /* process column singletons                                    */
      /*--------------------------------------------------------------*/
      /* determine initial counts of columns of V and initialize list
       * of active column singletons */
      ns = 0; /* number of active column singletons */
      for (j = 1; j <= n; j++)
      {  if ((cnt[j] = vc_len[j]) == 1)
            list[++ns] = j;
      }
      /* process active column singletons */
      while (ns > 0)
      {  /* column singleton is in j-th column of V */
         j = list[ns--];
#if 1 /* 21/II-2016 */
         if (cnt[j] == 0)
         {  /* j-th column in the current nucleus is actually empty */
            /* this happened because on a previous step in the nucleus
             * there were two or more identical column singletons (that
             * means structural singularity), so removing one of them
             * from the nucleus made other columns empty */
            return 1;
         }
#endif
         /* find i-th row of V containing column singleton */
         ptr = vc_ptr[j];
         end = ptr + vc_len[j];
         for (; pp_ind[i = sv_ind[ptr]] < k1; ptr++)
            /* nop */;
         xassert(ptr < end);
         /* permute rows and columns of U to move column singleton to
          * position u[k1,k1] */
         ii = pp_ind[i];
         luf_swap_u_rows(k1, ii);
         jj = qq_inv[j];
         luf_swap_u_cols(k1, jj);
         /* nucleus size decreased */
         k1++;
         /* walk thru i-th row of V and decrease column counts; this
          * may cause new column singletons to appear */
         ptr = vr_ptr[i];
         end = ptr + vr_len[i];
         for (; ptr < end; ptr++)
         {  if (--(cnt[j = sv_ind[ptr]]) == 1)
               list[++ns] = j;
         }
      }
      /* nucleus begins at k1-th row/column of U */
      if (k1 > n)
      {  /* U is upper triangular; no nucleus exist */
         goto done;
      }
      /*--------------------------------------------------------------*/
      /* process row singletons                                       */
      /*--------------------------------------------------------------*/
      /* determine initial counts of rows of V and initialize list of
       * active row singletons */
      ns = 0; /* number of active row singletons */
      for (i = 1; i <= n; i++)
      {  if (pp_ind[i] < k1)
         {  /* corresponding row of U is above its k1-th row; set its
             * count to zero to prevent including it in active list */
            cnt[i] = 0;
         }
         else if ((cnt[i] = vr_len[i]) == 1)
            list[++ns] = i;
      }
      /* process active row singletons */
      while (ns > 0)
      {  /* row singleton is in i-th row of V */
         i = list[ns--];
#if 1 /* 21/II-2016 */
         if (cnt[i] == 0)
         {  /* i-th row in the current nucleus is actually empty */
            /* (see comments above for similar case of empty column) */
            return 2;
         }
#endif
         /* find j-th column of V containing row singleton */
         ptr = vr_ptr[i];
         end = ptr + vr_len[i];
         for (; qq_inv[j = sv_ind[ptr]] > k2; ptr++)
            /* nop */;
         xassert(ptr < end);
         /* permute rows and columns of U to move row singleton to
          * position u[k2,k2] */
         ii = pp_ind[i];
         luf_swap_u_rows(k2, ii);
         jj = qq_inv[j];
         luf_swap_u_cols(k2, jj);
         /* nucleus size decreased */
         k2--;
         /* walk thru j-th column of V and decrease row counts; this
          * may cause new row singletons to appear */
         ptr = vc_ptr[j];
         end = ptr + vc_len[j];
         for (; ptr < end; ptr++)
         {  if (--(cnt[i = sv_ind[ptr]]) == 1)
               list[++ns] = i;
         }
      }
      /* nucleus ends at k2-th row/column of U */
      xassert(k1 < k2);
done: *k1_ = k1, *k2_ = k2;
      return 0;
}

/***********************************************************************
*  sgf_singl_phase - compute LU-factorization (singleton phase)
*
*  It is assumed that on entry to the routine L = P'* F * P = F = I
*  and matrix U = P'* V * Q' has the following structure (provided by
*  the routine sgf_reduce_nuc):
*
*        1     k1    k2    n
*     1  a a a b b b b c c c
*        . a a b b b b c c c
*        . . a b b b b c c c
*     k1 . . . * * * * d d d
*        . . . * * * * d d d
*        . . . * * * * d d d
*     k2 . . . * * * * d d d
*        . . . . . . . e e e
*        . . . . . . . . e e
*     n  . . . . . . . . . e
*
*  First, the routine performs (implicit) symmetric permutations of
*  rows and columns of matrix U to place them in the following order:
*
*     1, 2, ..., k1-1; n, n-1, ..., k2+1; k1, k1+1, ..., k2
*
*  This changes the structure of matrix U as follows:
*
*        1     k1    k2'   n
*     1  a a a c c c b b b b
*        . a a c c c b b b b
*        . . a c c c b b b b
*     k1 . . . e . . . . . .
*        . . . e e . . . . .
*        . . . e e e . . . .
*     k2'. . . d d d * * * *
*        . . . d d d * * * *
*        . . . d d d * * * *
*     n  . . . d d d * * * *
*
*  where k2' = n - k2 + k1.
*
*  Then the routine performs elementary gaussian transformations to
*  eliminate subdiagonal elements in columns k1, ..., k2'-1 of U. The
*  effect is the same as if the routine sgf_eliminate would be called
*  for k = 1, ..., k2'-1 using diagonal elements u[k,k] as pivots.
*
*  After elimination matrices L and U becomes the following:
*
*        1     k1   k2'    n        1     k1   k2'    n
*     1  1 . . . . . . . . .     1  a a a c c c b b b b
*        . 1 . . . . . . . .        . a a c c c b b b b
*        . . 1 . . . . . . .        . . a c c c b b b b
*     k1 . . . 1 . . . . . .     k1 . . . e . . . . . .
*        . . . e'1 . . . . .        . . . . e . . . . .
*        . . . e'e'1 . . . .        . . . . . e . . . .
*     k2'. . . d'd'd'1 . . .     k2'. . . . . . * * * *
*        . . . d'd'd'. 1 . .        . . . . . . * * * *
*        . . . d'd'd'. . 1 .        . . . . . . * * * *
*     n  . . . d'd'd'. . . 1     n  . . . . . . * * * *
*
*             matrix L                   matrix U
*
*  where columns k1, ..., k2'-1 of L consist of subdiagonal elements
*  of initial matrix U divided by pivots u[k,k].
*
*  On exit the routine returns k2', the elimination step number, from
*  which computing of the factorization should be continued. Note that
*  k2' = n+1 means that matrix U is already upper triangular. */

int sgf_singl_phase(LUF *luf, int k1, int k2, int updat,
      int ind[/*1+n*/], double val[/*1+n*/])
{     int n = luf->n;
      SVA *sva = luf->sva;
      int *sv_ind = sva->ind;
      double *sv_val = sva->val;
      int fc_ref = luf->fc_ref;
      int *fc_ptr = &sva->ptr[fc_ref-1];
      int *fc_len = &sva->len[fc_ref-1];
      int vr_ref = luf->vr_ref;
      int *vr_ptr = &sva->ptr[vr_ref-1];
      int *vr_len = &sva->len[vr_ref-1];
      double *vr_piv = luf->vr_piv;
      int vc_ref = luf->vc_ref;
      int *vc_ptr = &sva->ptr[vc_ref-1];
      int *vc_len = &sva->len[vc_ref-1];
      int *pp_ind = luf->pp_ind;
      int *pp_inv = luf->pp_inv;
      int *qq_ind = luf->qq_ind;
      int *qq_inv = luf->qq_inv;
      int i, j, k, ptr, ptr1, end, len;
      double piv;
      /* (see routine sgf_reduce_nuc) */
      xassert((1 <= k1 && k1 < k2 && k2 <= n)
         || (k1 == n+1 && k2 == n));
      /* perform symmetric permutations of rows/columns of U */
      for (k = k1; k <= k2; k++)
         pp_ind[pp_inv[k]] = qq_inv[qq_ind[k]] = k - k2 + n;
      for (k = k2+1; k <= n; k++)
         pp_ind[pp_inv[k]] = qq_inv[qq_ind[k]] = n - k + k1;
      for (k = 1; k <= n; k++)
         pp_inv[pp_ind[k]] = qq_ind[qq_inv[k]] = k;
      /* determine k2' */
      k2 = n - k2 + k1;
      /* process rows and columns of V corresponding to rows and
       * columns 1, ..., k1-1 of U */
      for (k = 1; k < k1; k++)
      {  /* k-th row of U = i-th row of V */
         i = pp_inv[k];
         /* find pivot u[k,k] = v[i,j] in i-th row of V */
         ptr = vr_ptr[i];
         end = ptr + vr_len[i];
         for (; qq_inv[sv_ind[ptr]] != k; ptr++)
            /* nop */;
         xassert(ptr < end);
         /* store pivot */
         vr_piv[i] = sv_val[ptr];
         /* and remove it from i-th row of V */
         sv_ind[ptr] = sv_ind[end-1];
         sv_val[ptr] = sv_val[end-1];
         vr_len[i]--;
         /* clear column of V corresponding to k-th column of U */
         vc_len[qq_ind[k]] = 0;
      }
      /* clear rows of V corresponding to rows k1, ..., k2'-1 of U */
      for (k = k1; k < k2; k++)
         vr_len[pp_inv[k]] = 0;
      /* process rows and columns of V corresponding to rows and
       * columns k2', ..., n of U */
      for (k = k2; k <= n; k++)
      {  /* k-th row of U = i-th row of V */
         i = pp_inv[k];
         /* remove elements from i-th row of V that correspond to
          * elements u[k,k1], ..., u[k,k2'-1] */
         ptr = ptr1 = vr_ptr[i];
         end = ptr + vr_len[i];
         for (; ptr < end; ptr++)
         {  if (qq_inv[sv_ind[ptr]] >= k2)
            {  sv_ind[ptr1] = sv_ind[ptr];
               sv_val[ptr1] = sv_val[ptr];
               ptr1++;
            }
         }
         vr_len[i] = ptr1 - vr_ptr[i];
         /* k-th column of U = j-th column of V */
         j = qq_ind[k];
         /* remove elements from j-th column of V that correspond to
          * elements u[1,k], ..., u[k1-1,k] */
         ptr = ptr1 = vc_ptr[j];
         end = ptr + vc_len[j];
         for (; ptr < end; ptr++)
         {  if (pp_ind[sv_ind[ptr]] >= k2)
               /* element value is not needed in this case */
               sv_ind[ptr1++] = sv_ind[ptr];
         }
         vc_len[j] = ptr1 - vc_ptr[j];
      }
      /* process columns of V corresponding to columns k1, ..., k2'-1
       * of U, build columns of F */
      for (k = k1; k < k2; k++)
      {  /* k-th column of U = j-th column of V */
         j = qq_ind[k];
         /* remove elements from j-th column of V that correspond to
          * pivot (diagonal) element u[k,k] and subdiagonal elements
          * u[k+1,k], ..., u[n,k]; subdiagonal elements are stored for
          * further addition to matrix F */
         len = 0;
         piv = 0.0;
         ptr = vc_ptr[j];
         end = ptr + vc_len[j];
         for (; ptr < end; ptr++)
         {  i = sv_ind[ptr]; /* v[i,j] */
            if (pp_ind[i] == k)
            {  /* store pivot v[i,j] = u[k,k] */
               piv = vr_piv[i] = sv_val[ptr];
            }
            else if (pp_ind[i] > k)
            {  /* store subdiagonal element v[i,j] = u[i',k] */
               len++;
               ind[len] = i;
               val[len] = sv_val[ptr];
            }
         }
         /* clear j-th column of V = k-th column of U */
         vc_len[j] = 0;
         /* build k-th column of L = j-th column of F */
         j = pp_inv[k];
         xassert(piv != 0.0);
         if (len > 0)
         {  if (sva->r_ptr - sva->m_ptr < len)
            {  sva_more_space(sva, len);
               sv_ind = sva->ind;
               sv_val = sva->val;
            }
            sva_reserve_cap(sva, fc_ref-1+j, len);
            for (ptr = fc_ptr[j], ptr1 = 1; ptr1 <= len; ptr++, ptr1++)
            {  sv_ind[ptr] = ind[ptr1];
               sv_val[ptr] = val[ptr1] / piv;
            }
            fc_len[j] = len;
         }
      }
      /* if it is not planned to update matrix V, relocate all its
       * non-active rows corresponding to rows 1, ..., k2'-1 of U to
       * the right (static) part of SVA */
      if (!updat)
      {  for (k = 1; k < k2; k++)
         {  i = pp_inv[k];
            len = vr_len[i];
            if (sva->r_ptr - sva->m_ptr < len)
            {  sva_more_space(sva, len);
               sv_ind = sva->ind;
               sv_val = sva->val;
            }
            sva_make_static(sva, vr_ref-1+i);
         }
      }
      /* elimination steps 1, ..., k2'-1 have been performed */
      return k2;
}

/***********************************************************************
*  sgf_choose_pivot - choose pivot element v[p,q]
*
*  This routine chooses pivot element v[p,q], k <= p, q <= n, in the
*  active submatrix of matrix V = P * U * Q, where k is the number of
*  current elimination step, 1 <= k <= n.
*
*  It is assumed that on entry to the routine matrix U = P'* V * Q' has
*  the following partially triangularized form:
*
*        1       k         n
*     1  x x x x x x x x x x
*        . x x x x x x x x x
*        . . x x x x x x x x
*        . . . x x x x x x x
*     k  . . . . * * * * * *
*        . . . . * * * * * *
*        . . . . * * * * * *
*        . . . . * * * * * *
*        . . . . * * * * * *
*     n  . . . . * * * * * *
*
*  where rows and columns k, k+1, ..., n belong to the active submatrix
*  (its elements are marked by '*').
*
*  Since the matrix U is not stored, the routine works with the matrix
*  V = P * U * Q. It is assumed that the row-wise representation
*  corresponds to the matrix V, but the column-wise representation
*  corresponds to the active submatrix of the matrix V, i.e. elements,
*  which are not in the active submatrix, are not included in column
*  vectors. It is also assumed that each active row of the matrix V is
*  in the set R[len], where len is the number of non-zeros in the row,
*  and each active column of the matrix V is in the set C[len], where
*  len is the number of non-zeros in the column (in the latter case
*  only elements of the active submatrix are counted; such elements are
*  marked by '*' on the figure above).
*
*  For the reason of numerical stability the routine applies so called
*  threshold pivoting proposed by J.Reid. It is assumed that an element
*  v[i,j] can be selected as a pivot candidate if it is not very small
*  (in magnitude) among other elements in the same row, i.e. if it
*  satisfies to the stability condition |v[i,j]| >= tol * max|v[i,*]|,
*  where 0 < tol < 1 is a given tolerance.
*
*  In order to keep sparsity of the matrix V the routine uses Markowitz
*  strategy, trying to choose such element v[p,q], which satisfies to
*  the stability condition (see above) and has smallest Markowitz cost
*  (nr[p]-1) * (nc[q]-1), where nr[p] and nc[q] are, resp., numbers of
*  non-zeros in p-th row and q-th column of the active submatrix.
*
*  In order to reduce the search, i.e. not to walk through all elements
*  of the active submatrix, the routine uses a technique proposed by
*  I.Duff. This technique is based on using the sets R[len] and C[len]
*  of active rows and columns.
*
*  If the pivot element v[p,q] has been chosen, the routine stores its
*  indices to locations *p and *q and returns zero. Otherwise, non-zero
*  is returned. */

int sgf_choose_pivot(SGF *sgf, int *p_, int *q_)
{     LUF *luf = sgf->luf;
      int n = luf->n;
      SVA *sva = luf->sva;
      int *sv_ind = sva->ind;
      double *sv_val = sva->val;
      int vr_ref = luf->vr_ref;
      int *vr_ptr = &sva->ptr[vr_ref-1];
      int *vr_len = &sva->len[vr_ref-1];
      int vc_ref = luf->vc_ref;
      int *vc_ptr = &sva->ptr[vc_ref-1];
      int *vc_len = &sva->len[vc_ref-1];
      int *rs_head = sgf->rs_head;
      int *rs_next = sgf->rs_next;
      int *cs_head = sgf->cs_head;
      int *cs_prev = sgf->cs_prev;
      int *cs_next = sgf->cs_next;
      double *vr_max = sgf->vr_max;
      double piv_tol = sgf->piv_tol;
      int piv_lim = sgf->piv_lim;
      int suhl = sgf->suhl;
      int i, i_ptr, i_end, j, j_ptr, j_end, len, min_i, min_j, min_len,
         ncand, next_j, p, q;
      double best, big, cost, temp;
      /* no pivot candidate has been chosen so far */
      p = q = 0, best = DBL_MAX, ncand = 0;
      /* if the active submatrix contains a column having the only
       * non-zero element (column singleton), choose it as the pivot */
      j = cs_head[1];
      if (j != 0)
      {  xassert(vc_len[j] == 1);
         p = sv_ind[vc_ptr[j]], q = j;
         goto done;
      }
      /* if the active submatrix contains a row having the only
       * non-zero element (row singleton), choose it as the pivot */
      i = rs_head[1];
      if (i != 0)
      {  xassert(vr_len[i] == 1);
         p = i, q = sv_ind[vr_ptr[i]];
         goto done;
      }
      /* the active submatrix contains no singletons; walk thru its
       * other non-empty rows and columns */
      for (len = 2; len <= n; len++)
      {  /* consider active columns containing len non-zeros */
         for (j = cs_head[len]; j != 0; j = next_j)
         {  /* save the number of next column of the same length */
            next_j = cs_next[j];
            /* find an element in j-th column, which is placed in the
             * row with minimal number of non-zeros and satisfies to
             * the stability condition (such element may not exist) */
            min_i = min_j = 0, min_len = INT_MAX;
            for (j_end = (j_ptr = vc_ptr[j]) + vc_len[j];
               j_ptr < j_end; j_ptr++)
            {  /* get row index of v[i,j] */
               i = sv_ind[j_ptr];
               /* if i-th row is not shorter, skip v[i,j] */
               if (vr_len[i] >= min_len)
                  continue;
               /* big := max|v[i,*]| */
               if ((big = vr_max[i]) < 0.0)
               {  /* largest magnitude is unknown; compute it */
                  for (i_end = (i_ptr = vr_ptr[i]) + vr_len[i];
                     i_ptr < i_end; i_ptr++)
                  {  if ((temp = sv_val[i_ptr]) < 0.0)
                        temp = -temp;
                     if (big < temp)
                        big = temp;
                  }
                  xassert(big > 0.0);
                  vr_max[i] = big;
               }
               /* find v[i,j] in i-th row */
               for (i_end = (i_ptr = vr_ptr[i]) + vr_len[i];
                  sv_ind[i_ptr] != j; i_ptr++)
                  /* nop */;
               xassert(i_ptr < i_end);
               /* if |v[i,j]| < piv_tol * max|v[i,*]|, skip v[i,j] */
               if ((temp = sv_val[i_ptr]) < 0.0)
                  temp = -temp;
               if (temp < piv_tol * big)
                  continue;
               /* v[i,j] is a better candidate */
               min_i = i, min_j = j, min_len = vr_len[i];
               /* if Markowitz cost of v[i,j] is not greater than
                * (len-1)**2, v[i,j] can be chosen as the pivot right
                * now; this heuristic reduces the search and works well
                * in many cases */
               if (min_len <= len)
               {  p = min_i, q = min_j;
                  goto done;
               }
            }
            /* j-th column has been scanned */
            if (min_i != 0)
            {  /* element v[min_i,min_j] is a next pivot candidate */
               ncand++;
               /* compute its Markowitz cost */
               cost = (double)(min_len - 1) * (double)(len - 1);
               /* if this element is better, choose it as the pivot */
               if (cost < best)
                  p = min_i, q = min_j, best = cost;
               /* if piv_lim candidates were considered, terminate
                * the search, because it is doubtful that a much better
                * candidate will be found */
               if (ncand == piv_lim)
                  goto done;
            }
            else if (suhl)
            {  /* j-th column has no eligible elements that satisfy to
                * the stability criterion; Uwe Suhl suggests to exclude
                * such column from further considerations until it
                * becomes a column singleton; in hard cases this may
                * significantly reduce the time needed to choose the
                * pivot element */
               sgf_deactivate_col(j);
               cs_prev[j] = cs_next[j] = j;
            }
         }
         /* consider active rows containing len non-zeros */
         for (i = rs_head[len]; i != 0; i = rs_next[i])
         {  /* big := max|v[i,*]| */
            if ((big = vr_max[i]) < 0.0)
            {  /* largest magnitude is unknown; compute it */
               for (i_end = (i_ptr = vr_ptr[i]) + vr_len[i];
                  i_ptr < i_end; i_ptr++)
               {  if ((temp = sv_val[i_ptr]) < 0.0)
                     temp = -temp;
                  if (big < temp)
                     big = temp;
               }
               xassert(big > 0.0);
               vr_max[i] = big;
            }
            /* find an element in i-th row, which is placed in the
             * column with minimal number of non-zeros and satisfies to
             * the stability condition (such element always exists) */
            min_i = min_j = 0, min_len = INT_MAX;
            for (i_end = (i_ptr = vr_ptr[i]) + vr_len[i];
               i_ptr < i_end; i_ptr++)
            {  /* get column index of v[i,j] */
               j = sv_ind[i_ptr];
               /* if j-th column is not shorter, skip v[i,j] */
               if (vc_len[j] >= min_len)
                  continue;
               /* if |v[i,j]| < piv_tol * max|v[i,*]|, skip v[i,j] */
               if ((temp = sv_val[i_ptr]) < 0.0)
                  temp = -temp;
               if (temp < piv_tol * big)
                  continue;
               /* v[i,j] is a better candidate */
               min_i = i, min_j = j, min_len = vc_len[j];
               /* if Markowitz cost of v[i,j] is not greater than
                * (len-1)**2, v[i,j] can be chosen as the pivot right
                * now; this heuristic reduces the search and works well
                * in many cases */
               if (min_len <= len)
               {  p = min_i, q = min_j;
                  goto done;
               }
            }
            /* i-th row has been scanned */
            if (min_i != 0)
            {  /* element v[min_i,min_j] is a next pivot candidate */
               ncand++;
               /* compute its Markowitz cost */
               cost = (double)(len - 1) * (double)(min_len - 1);
               /* if this element is better, choose it as the pivot */
               if (cost < best)
                  p = min_i, q = min_j, best = cost;
               /* if piv_lim candidates were considered, terminate
                * the search, because it is doubtful that a much better
                * candidate will be found */
               if (ncand == piv_lim)
                  goto done;
            }
            else
            {  /* this can never be */
               xassert(min_i != min_i);
            }
         }
      }
done: /* report the pivot to the factorization routine */
      *p_ = p, *q_ = q;
      return (p == 0);
}

/***********************************************************************
*  sgf_eliminate - perform gaussian elimination
*
*  This routine performs elementary gaussian transformations in order
*  to eliminate subdiagonal elements in k-th column of matrix
*  U = P'* V * Q' using pivot element u[k,k], where k is the number of
*  current elimination step, 1 <= k <= n.
*
*  The parameters p and q specify, resp., row and column indices of the
*  pivot element v[p,q] = u[k,k].
*
*  On entry the routine assumes that partially triangularized matrices
*  L = P'* F * P and U = P'* V * Q' have the following structure:
*
*        1       k         n       1        k         n
*     1  1 . . . . . . . . .     1  x x x x x x x x x x
*        x 1 . . . . . . . .        . x x x x x x x x x
*        x x 1 . . . . . . .        . . x x x x x x x x
*        x x x 1 . . . . . .        . . . x x x x x x x
*     k  x x x x 1 . . . . .     k  . . . . * * * * * *
*        x x x x _ 1 . . . .        . . . . # * * * * *
*        x x x x _ . 1 . . .        . . . . # * * * * *
*        x x x x _ . . 1 . .        . . . . # * * * * *
*        x x x x _ . . . 1 .        . . . . # * * * * *
*     n  x x x x _ . . . . 1     n  . . . . # * * * * *
*
*             matrix L                   matrix U
*
*  where rows and columns k, k+1, ..., n of matrix U constitute the
*  active submatrix. Elements to be eliminated are marked by '#', and
*  other elements of the active submatrix are marked by '*'. May note
*  that each eliminated non-zero element u[i,k] of matrix U gives
*  corresponding non-zero element l[i,k] of matrix L (marked by '_').
*
*  Actually all operations are performed on matrix V. It is assumed
*  that the row-wise representation corresponds to matrix V, but the
*  column-wise representation corresponds to the active submatrix of
*  matrix V (or, more precisely, to its pattern, because only row
*  indices for columns of the active submatrix are used on this stage).
*
*  Let u[k,k] = v[p,q] be the pivot. In order to eliminate subdiagonal
*  elements u[i',k] = v[i,q], i'= k+1, k+2, ..., n, the routine applies
*  the following elementary gaussian transformations:
*
*     (i-th row of V) := (i-th row of V) - f[i,p] * (p-th row of V),
*
*  where f[i,p] = v[i,q] / v[p,q] is a gaussian multiplier stored to
*  p-th column of matrix F to keep the main equality A = F * V
*  (corresponding elements l[i',k] of matrix L are marked by '_' on the
*  figure above).
*
*  NOTE: On entry to the routine the working arrays flag and work
*        should contain zeros. This status is retained by the routine
*        on exit. */

int sgf_eliminate(SGF *sgf, int p, int q)
{     LUF *luf = sgf->luf;
      int n = luf->n;
      SVA *sva = luf->sva;
      int *sv_ind = sva->ind;
      double *sv_val = sva->val;
      int fc_ref = luf->fc_ref;
      int *fc_ptr = &sva->ptr[fc_ref-1];
      int *fc_len = &sva->len[fc_ref-1];
      int vr_ref = luf->vr_ref;
      int *vr_ptr = &sva->ptr[vr_ref-1];
      int *vr_len = &sva->len[vr_ref-1];
      int *vr_cap = &sva->cap[vr_ref-1];
      double *vr_piv = luf->vr_piv;
      int vc_ref = luf->vc_ref;
      int *vc_ptr = &sva->ptr[vc_ref-1];
      int *vc_len = &sva->len[vc_ref-1];
      int *vc_cap = &sva->cap[vc_ref-1];
      int *rs_head = sgf->rs_head;
      int *rs_prev = sgf->rs_prev;
      int *rs_next = sgf->rs_next;
      int *cs_head = sgf->cs_head;
      int *cs_prev = sgf->cs_prev;
      int *cs_next = sgf->cs_next;
      double *vr_max = sgf->vr_max;
      char *flag = sgf->flag;
      double *work = sgf->work;
      double eps_tol = sgf->eps_tol;
      int nnz_diff = 0;
      int fill, i, i_ptr, i_end, j, j_ptr, j_end, ptr, len, loc, loc1;
      double vpq, fip, vij;
      xassert(1 <= p && p <= n);
      xassert(1 <= q && q <= n);
      /* remove p-th row from the active set; this row will never
       * return there */
      sgf_deactivate_row(p);
      /* process p-th (pivot) row */
      ptr = 0;
      for (i_end = (i_ptr = vr_ptr[p]) + vr_len[p];
         i_ptr < i_end; i_ptr++)
      {  /* get column index of v[p,j] */
         j = sv_ind[i_ptr];
         if (j == q)
         {  /* save pointer to pivot v[p,q] */
            ptr = i_ptr;
         }
         else
         {  /* store v[p,j], j != q, to working array */
            flag[j] = 1;
            work[j] = sv_val[i_ptr];
         }
         /* remove j-th column from the active set; q-th column will
          * never return there while other columns will return to the
          * active set with new length */
         if (cs_next[j] == j)
         {  /* j-th column was marked by the pivoting routine according
             * to Uwe Suhl's suggestion and is already inactive */
            xassert(cs_prev[j] == j);
         }
         else
            sgf_deactivate_col(j);
         nnz_diff -= vc_len[j];
         /* find and remove v[p,j] from j-th column */
         for (j_end = (j_ptr = vc_ptr[j]) + vc_len[j];
            sv_ind[j_ptr] != p; j_ptr++)
            /* nop */;
         xassert(j_ptr < j_end);
         sv_ind[j_ptr] = sv_ind[j_end-1];
         vc_len[j]--;
      }
      /* save pivot v[p,q] and remove it from p-th row */
      xassert(ptr > 0);
      vpq = vr_piv[p] = sv_val[ptr];
      sv_ind[ptr] = sv_ind[i_end-1];
      sv_val[ptr] = sv_val[i_end-1];
      vr_len[p]--;
      /* if it is not planned to update matrix V, relocate p-th row to
       * the right (static) part of SVA */
      if (!sgf->updat)
      {  len = vr_len[p];
         if (sva->r_ptr - sva->m_ptr < len)
         {  sva_more_space(sva, len);
            sv_ind = sva->ind;
            sv_val = sva->val;
         }
         sva_make_static(sva, vr_ref-1+p);
      }
      /* copy the pattern (row indices) of q-th column of the active
       * submatrix (from which v[p,q] has been just removed) to p-th
       * column of matrix F (without unity diagonal element) */
      len = vc_len[q];
      if (len > 0)
      {  if (sva->r_ptr - sva->m_ptr < len)
         {  sva_more_space(sva, len);
            sv_ind = sva->ind;
            sv_val = sva->val;
         }
         sva_reserve_cap(sva, fc_ref-1+p, len);
         memcpy(&sv_ind[fc_ptr[p]], &sv_ind[vc_ptr[q]],
            len * sizeof(int));
         fc_len[p] = len;
      }
      /* make q-th column of the active submatrix empty */
      vc_len[q] = 0;
      /* transform non-pivot rows of the active submatrix */
      for (loc = fc_len[p]-1; loc >= 0; loc--)
      {  /* get row index of v[i,q] = row index of f[i,p] */
         i = sv_ind[fc_ptr[p] + loc];
         xassert(i != p); /* v[p,q] was removed */
         /* remove i-th row from the active set; this row will return
          * there with new length */
         sgf_deactivate_row(i);
         /* find v[i,q] in i-th row */
         for (i_end = (i_ptr = vr_ptr[i]) + vr_len[i];
            sv_ind[i_ptr] != q; i_ptr++)
            /* nop */;
         xassert(i_ptr < i_end);
         /* compute gaussian multiplier f[i,p] = v[i,q] / v[p,q] */
         fip = sv_val[fc_ptr[p] + loc] = sv_val[i_ptr] / vpq;
         /* remove v[i,q] from i-th row */
         sv_ind[i_ptr] = sv_ind[i_end-1];
         sv_val[i_ptr] = sv_val[i_end-1];
         vr_len[i]--;
         /* perform elementary gaussian transformation:
          * (i-th row) := (i-th row) - f[i,p] * (p-th row)
          * note that p-th row of V, which is in the working array,
          * doesn't contain pivot v[p,q], and i-th row of V doesn't
          * contain v[i,q] to be eliminated */
         /* walk thru i-th row and transform existing elements */
         fill = vr_len[p];
         for (i_end = (i_ptr = ptr = vr_ptr[i]) + vr_len[i];
            i_ptr < i_end; i_ptr++)
         {  /* get column index and value of v[i,j] */
            j = sv_ind[i_ptr];
            vij = sv_val[i_ptr];
            if (flag[j])
            {  /* v[p,j] != 0 */
               flag[j] = 0, fill--;
               /* v[i,j] := v[i,j] - f[i,p] * v[p,j] */
               vij -= fip * work[j];
               if (-eps_tol < vij && vij < +eps_tol)
               {  /* new v[i,j] is close to zero; remove it from the
                   * active submatrix, i.e. replace it by exact zero */
                  /* find and remove v[i,j] from j-th column */
                  for (j_end = (j_ptr = vc_ptr[j]) + vc_len[j];
                     sv_ind[j_ptr] != i; j_ptr++)
                     /* nop */;
                  xassert(j_ptr < j_end);
                  sv_ind[j_ptr] = sv_ind[j_end-1];
                  vc_len[j]--;
                  continue;
               }
            }
            /* keep new v[i,j] in i-th row */
            sv_ind[ptr] = j;
            sv_val[ptr] = vij;
            ptr++;
         }
         /* (new length of i-th row may decrease because of numerical
          * cancellation) */
         vr_len[i] = len = ptr - vr_ptr[i];
         /* now flag[*] is the pattern of the set v[p,*] \ v[i,*], and
          * fill is the number of non-zeros in this set */
         if (fill == 0)
         {  /* no fill-in occurs */
            /* walk thru p-th row and restore the column flags */
            for (i_end = (i_ptr = vr_ptr[p]) + vr_len[p];
               i_ptr < i_end; i_ptr++)
               flag[sv_ind[i_ptr]] = 1; /* v[p,j] != 0 */
            goto skip;
         }
         /* up to fill new non-zero elements may appear in i-th row due
          * to fill-in; reserve locations for these elements (note that
          * actual length of i-th row is currently stored in len) */
         if (vr_cap[i] < len + fill)
         {  if (sva->r_ptr - sva->m_ptr < len + fill)
            {  sva_more_space(sva, len + fill);
               sv_ind = sva->ind;
               sv_val = sva->val;
            }
            sva_enlarge_cap(sva, vr_ref-1+i, len + fill, 0);
         }
         vr_len[i] += fill;
         /* walk thru p-th row and add new elements to i-th row */
         for (loc1 = vr_len[p]-1; loc1 >= 0; loc1--)
         {  /* get column index of v[p,j] */
            j = sv_ind[vr_ptr[p] + loc1];
            if (!flag[j])
            {  /* restore j-th column flag */
               flag[j] = 1;
               /* v[i,j] was computed earlier on transforming existing
                * elements of i-th row */
               continue;
            }
            /* v[i,j] := 0 - f[i,p] * v[p,j] */
            vij = - fip * work[j];
            if (-eps_tol < vij && vij < +eps_tol)
            {  /* new v[i,j] is close to zero; do not add it to the
                * active submatrix, i.e. replace it by exact zero */
               continue;
            }
            /* add new v[i,j] to i-th row */
            sv_ind[ptr = vr_ptr[i] + (len++)] = j;
            sv_val[ptr] = vij;
            /* add new v[i,j] to j-th column */
            if (vc_cap[j] == vc_len[j])
            {  /* we reserve extra locations in j-th column to reduce
                * further relocations of that column */
#if 1 /* FIXME */
               /* use control parameter to specify the number of extra
                * locations reserved */
               int need = vc_len[j] + 10;
#endif
               if (sva->r_ptr - sva->m_ptr < need)
               {  sva_more_space(sva, need);
                  sv_ind = sva->ind;
                  sv_val = sva->val;
               }
               sva_enlarge_cap(sva, vc_ref-1+j, need, 1);
            }
            sv_ind[vc_ptr[j] + (vc_len[j]++)] = i;
         }
         /* set final length of i-th row just transformed */
         xassert(len <= vr_len[i]);
         vr_len[i] = len;
skip:    /* return i-th row to the active set with new length */
         sgf_activate_row(i);
         /* since i-th row has been changed, largest magnitude of its
          * elements becomes unknown */
         vr_max[i] = -1.0;
      }
      /* walk thru p-th (pivot) row */
      for (i_end = (i_ptr = vr_ptr[p]) + vr_len[p];
         i_ptr < i_end; i_ptr++)
      {  /* get column index of v[p,j] */
         j = sv_ind[i_ptr];
         xassert(j != q); /* v[p,q] was removed */
         /* return j-th column to the active set with new length */
         if (cs_next[j] == j && vc_len[j] != 1)
         {  /* j-th column was marked by the pivoting routine and it is
             * still not a column singleton, so leave it incative */
            xassert(cs_prev[j] == j);
         }
         else
            sgf_activate_col(j);
         nnz_diff += vc_len[j];
         /* restore zero content of the working arrays */
         flag[j] = 0;
         work[j] = 0.0;
      }
      /* return the difference between the numbers of non-zeros in the
       * active submatrix on entry and on exit, resp. */
      return nnz_diff;
}

/***********************************************************************
*  sgf_dense_lu - compute dense LU-factorization with full pivoting
*
*  This routine performs Gaussian elimination with full pivoting to
*  compute dense LU-factorization of the specified matrix A of order n
*  in the form:
*
*     A = P * L * U * Q,                                             (1)
*
*  where L is lower triangular matrix with unit diagonal, U is upper
*  triangular matrix, P and Q are permutation matrices.
*
*  On entry to the routine elements of matrix A = (a[i,j]) should be
*  placed in the array elements a[0], ..., a[n^2-1] in dense row-wise
*  format. On exit from the routine matrix A is replaced by factors L
*  and U as follows:
*
*       u[1,1]   u[1,2]  ...  u[1,n-1]   u[1,n]
*       l[2,1]   u[2,2]  ...  u[2,n-1]   u[2,n]
*        . . . . . . . . . . . . . .
*     l[n-1,1] l[n-1,2]     u[n-1,n-1] u[n-1,n]
*       l[n,1]   l[n,2]  ...  l[n,n-1]   u[n,n]
*
*  The unit diagonal elements of L are not stored.
*
*  Information on permutations of rows and columns of active submatrix
*  during factorization is accumulated by the routine as follows. Every
*  time the routine permutes rows i and i' or columns j and j', it also
*  permutes elements r[i-1] and r[i'-1] or c[j-1] and c[j'-1], resp.
*  Thus, on entry to the routine elements r[0], r[1], ..., r[n-1] and
*  c[0], c[1], ..., c[n-1] should be initialized by some integers that
*  identify rows and columns of the original matrix A.
*
*  If the factorization has been successfully computed, the routine
*  returns zero. Otherwise, if on k-th elimination step, 1 <= k <= n,
*  all elements of the active submatrix are close to zero, the routine
*  returns k, in which case a partial factorization is stored in the
*  array a. */

int sgf_dense_lu(int n, double a_[], int r[], int c[], double eps)
{     /* non-optimized version */
      int i, j, k, p, q, ref;
      double akk, big, temp;
#     define a(i,j) a_[(i)*n+(j)]
      /* initially U = A, L = P = Q = I */
      /* main elimination loop */
      for (k = 0; k < n; k++)
      {  /* choose pivot u[p,q], k <= p, q <= n */
         p = q = -1, big = eps;
         for (i = k; i < n; i++)
         {  for (j = k; j < n; j++)
            {  /* temp = |u[i,j]| */
               if ((temp = a(i,j)) < 0.0)
                  temp = -temp;
               if (big < temp)
                  p = i, q = j, big = temp;
            }
         }
         if (p < 0)
         {  /* k-th elimination step failed */
            return k+1;
         }
         /* permute rows k and p */
         if (k != p)
         {  for (j = 0; j < n; j++)
               temp = a(k,j), a(k,j) = a(p,j), a(p,j) = temp;
            ref = r[k], r[k] = r[p], r[p] = ref;
         }
         /* permute columns k and q */
         if (k != q)
         {  for (i = 0; i < n; i++)
               temp = a(i,k), a(i,k) = a(i,q), a(i,q) = temp;
            ref = c[k], c[k] = c[q], c[q] = ref;
         }
         /* now pivot is in position u[k,k] */
         akk = a(k,k);
         /* eliminate subdiagonal elements u[k+1,k], ..., u[n,k] */
         for (i = k+1; i < n; i++)
         {  if (a(i,k) != 0.0)
            {  /* gaussian multiplier l[i,k] := u[i,k] / u[k,k] */
               temp = (a(i,k) /= akk);
               /* (i-th row) := (i-th row) - l[i,k] * (k-th row) */
               for (j = k+1; j < n; j++)
                  a(i,j) -= temp * a(k,j);
            }
         }
      }
#     undef a
      return 0;
}

/***********************************************************************
*  sgf_dense_phase - compute LU-factorization (dense phase)
*
*  This routine performs dense phase of computing LU-factorization.
*
*  The aim is two-fold. First, the main factorization routine switches
*  to dense phase when the active submatrix is relatively dense, so
*  using dense format allows significantly reduces overheads needed to
*  maintain sparse data structures. And second, that is more important,
*  on dense phase full pivoting is used (rather than partial pivoting)
*  that allows improving numerical stability, since round-off errors
*  tend to increase on last steps of the elimination process.
*
*  On entry the routine assumes that elimination steps 1, 2, ..., k-1
*  have been performed, so partially transformed matrices L = P'* F * P
*  and U = P'* V * Q' have the following structure:
*
*        1       k         n       1        k         n
*     1  1 . . . . . . . . .     1  x x x x x x x x x x
*        x 1 . . . . . . . .        . x x x x x x x x x
*        x x 1 . . . . . . .        . . x x x x x x x x
*        x x x 1 . . . . . .        . . . x x x x x x x
*     k  x x x x 1 . . . . .     k  . . . . * * * * * *
*        x x x x . 1 . . . .        . . . . * * * * * *
*        x x x x . . 1 . . .        . . . . * * * * * *
*        x x x x . . . 1 . .        . . . . * * * * * *
*        x x x x . . . . 1 .        . . . . * * * * * *
*     n  x x x x . . . . . 1     n  . . . . * * * * * *
*
*             matrix L                   matrix U
*
*  where rows and columns k, k+1, ..., n of matrix U constitute the
*  active submatrix A~, whose elements are marked by '*'.
*
*  The routine copies the active submatrix A~ to a working array in
*  dense format, compute dense factorization A~ = P~* L~* U~* Q~ using
*  full pivoting, and then copies non-zero elements of factors L~ and
*  U~ back to factors L and U (more precisely, to factors F and V).
*
*  If the factorization has been successfully computed, the routine
*  returns zero. Otherwise, if on k-th elimination step, 1 <= k <= n,
*  all elements of the active submatrix are close to zero, the routine
*  returns k (information on linearly dependent rows/columns in this
*  case is provided by matrices P and Q). */

int sgf_dense_phase(LUF *luf, int k, int updat)
{     int n = luf->n;
      SVA *sva = luf->sva;
      int *sv_ind = sva->ind;
      double *sv_val = sva->val;
      int fc_ref = luf->fc_ref;
      int *fc_ptr = &sva->ptr[fc_ref-1];
      int *fc_len = &sva->len[fc_ref-1];
      int *fc_cap = &sva->cap[fc_ref-1];
      int vr_ref = luf->vr_ref;
      int *vr_ptr = &sva->ptr[vr_ref-1];
      int *vr_len = &sva->len[vr_ref-1];
      int *vr_cap = &sva->cap[vr_ref-1];
      double *vr_piv = luf->vr_piv;
      int vc_ref = luf->vc_ref;
      int *vc_len = &sva->len[vc_ref-1];
      int *pp_inv = luf->pp_inv;
      int *pp_ind = luf->pp_ind;
      int *qq_ind = luf->qq_ind;
      int *qq_inv = luf->qq_inv;
      int a_end, a_ptr, end, i, ia, ii, j, ja, jj, ka, len, na, ne,
         need, ptr;
      double *a_;
      xassert(1 <= k && k <= n);
      /* active columns of V are not longer needed; make them empty */
      for (jj = k; jj <= n; jj++)
      {  /* jj is number of active column of U = P'* V * Q' */
         vc_len[qq_ind[jj]] = 0;
      }
      /* determine order of active submatrix A~ of matrix U */
      na = n - k + 1;
      xassert(1 <= na && na <= n);
      /* determine number of elements in dense triangular factor (L~ or
       * U~), except diagonal elements */
      ne = na * (na - 1) / 2;
      /* we allocate active submatrix A~ in free (middle) part of SVA;
       * to avoid defragmentation that could destroy A~ we also should
       * reserve ne locations to build rows of V from rows of U~ and ne
       * locations to build columns of F from columns of L~ */
      need = na * na + ne + ne;
      if (sva->r_ptr - sva->m_ptr < need)
      {  sva_more_space(sva, need);
         sv_ind = sva->ind;
         sv_val = sva->val;
      }
      /* free (middle) part of SVA is structured as follows:
       * end of left (dynamic) part
       * ne free locations for new rows of V
       * na free locations for active submatrix A~
       * unused locations, if any
       * ne free locations for new columns of F
       * beginning of right (static) part */
      a_ptr = sva->m_ptr + ne;
      a_end = a_ptr + na * na;
      /* copy active submatrix A~ from matrix V to working array in
       * dense row-wise format */
      a_ = &sva->val[a_ptr];
#     define a(ia, ja) a_[((ia) - 1) * na + ((ja) - 1)]
      for (ia = 1; ia <= na; ia++)
      {  /* clear ia-th row of A~ */
         for (ja = 1; ja <= na; ja++)
            a(ia, ja) = 0.0;
         /* ia-th row of A~ = (k-1+ia)-th row of U = i-th row of V */
         i = pp_inv[k-1+ia];
         ptr = vr_ptr[i];
         end = ptr + vr_len[i];
         for (; ptr < end; ptr++)
            a(ia, qq_inv[sv_ind[ptr]]-k+1) = sv_val[ptr];
         /* i-th row of V is no longer needed; make it empty */
         vr_len[i] = 0;
      }
      /* compute dense factorization A~ = P~* L~* U~* Q~ */
#if 1 /* FIXME: epsilon tolerance */
      ka = sgf_dense_lu(na, &a(1, 1), &pp_inv[k], &qq_ind[k], 1e-20);
#endif
      /* rows of U with numbers pp_inv[k, k+1, ..., n] were permuted
       * due to row permutations of A~; update matrix P using P~ */
      for (ii = k; ii <= n; ii++)
         pp_ind[pp_inv[ii]] = ii;
      /* columns of U with numbers qq_ind[k, k+1, ..., n] were permuted
       * due to column permutations of A~; update matrix Q using Q~ */
      for (jj = k; jj <= n; jj++)
         qq_inv[qq_ind[jj]] = jj;
      /* check if dense factorization is complete */
      if (ka != 0)
      {  /* A~ is singular to working precision */
         /* information on linearly dependent rows/columns is provided
          * by matrices P and Q */
         xassert(1 <= ka && ka <= na);
         return k - 1 + ka;
      }
      /* build new rows of V from rows of U~ */
      for (ia = 1; ia <= na; ia++)
      {  /* ia-th row of U~ = (k-1+ia)-th row of U = i-th row of V */
         i = pp_inv[k-1+ia];
         xassert(vr_len[i] == 0);
         /* store diagonal element u~[ia,ia] */
         vr_piv[i] = a(ia, ia);
         /* determine number of non-zero non-diagonal elements in ia-th
          * row of U~ */
         len = 0;
         for (ja = ia+1; ja <= na; ja++)
         {  if (a(ia, ja) != 0.0)
               len++;
         }
         /* reserve len locations for i-th row of matrix V in left
          * (dynamic) part of SVA */
         if (vr_cap[i] < len)
         {  /* there should be enough room in free part of SVA */
            xassert(sva->r_ptr - sva->m_ptr >= len);
            sva_enlarge_cap(sva, vr_ref-1+i, len, 0);
            /* left part of SVA should not overlap matrix A~ */
            xassert(sva->m_ptr <= a_ptr);
         }
         /* copy non-zero non-diaginal elements of ia-th row of U~ to
          * i-th row of V */
         ptr = vr_ptr[i];
         for (ja = ia+1; ja <= na; ja++)
         {  if (a(ia, ja) != 0.0)
            {  sv_ind[ptr] = qq_ind[k-1+ja];
               sv_val[ptr] = a(ia, ja);
               ptr++;
            }
         }
         xassert(ptr - vr_ptr[i] == len);
         vr_len[i] = len;
      }
      /* build new columns of F from columns of L~ */
      for (ja = 1; ja <= na; ja++)
      {  /* ja-th column of L~ = (k-1+ja)-th column of L = j-th column
          * of F */
         j = pp_inv[k-1+ja];
         xassert(fc_len[j] == 0);
         xassert(fc_cap[j] == 0);
         /* determine number of non-zero non-diagonal elements in ja-th
          * column of L~ */
         len = 0;
         for (ia = ja+1; ia <= na; ia++)
         {  if (a(ia, ja) != 0.0)
               len++;
         }
         /* reserve len locations for j-th column of matrix F in right
          * (static) part of SVA */
         /* there should be enough room in free part of SVA */
         xassert(sva->r_ptr - sva->m_ptr >= len);
         if (len > 0)
            sva_reserve_cap(sva, fc_ref-1+j, len);
         /* right part of SVA should not overlap matrix A~ */
         xassert(a_end <= sva->r_ptr);
         /* copy non-zero non-diagonal elements of ja-th column of L~
          * to j-th column of F */
         ptr = fc_ptr[j];
         for (ia = ja+1; ia <= na; ia++)
         {  if (a(ia, ja) != 0.0)
            {  sv_ind[ptr] = pp_inv[k-1+ia];
               sv_val[ptr] = a(ia, ja);
               ptr++;
            }
         }
         xassert(ptr - fc_ptr[j] == len);
         fc_len[j] = len;
      }
      /* factors L~ and U~ are no longer needed */
#     undef a
      /* if it is not planned to update matrix V, relocate all its new
       * rows to the right (static) part of SVA */
      if (!updat)
      {  for (ia = 1; ia <= na; ia++)
         {  i = pp_inv[k-1+ia];
            len = vr_len[i];
            if (sva->r_ptr - sva->m_ptr < len)
            {  sva_more_space(sva, len);
               sv_ind = sva->ind;
               sv_val = sva->val;
            }
            sva_make_static(sva, vr_ref-1+i);
         }
      }
      return 0;
}

/***********************************************************************
*  sgf_factorize - compute LU-factorization (main routine)
*
*  This routine computes sparse LU-factorization of specified matrix A
*  using Gaussian elimination.
*
*  On entry to the routine matrix V = A should be stored in column-wise
*  format.
*
*  If the factorization has been successfully computed, the routine
*  returns zero. Otherwise, if on k-th elimination step, 1 <= k <= n,
*  all elements of the active submatrix are close to zero, the routine
*  returns k (information on linearly dependent rows/columns in this
*  case is provided by matrices P and Q). */

#if 1 /* 21/II-2016 */
/* If the matrix A is structurally singular, the routine returns -1.
*  NOTE: This case can be detected only if the singl flag is set. */
#endif

int sgf_factorize(SGF *sgf, int singl)
{     LUF *luf = sgf->luf;
      int n = luf->n;
      SVA *sva = luf->sva;
      int vr_ref = luf->vr_ref;
      int *vr_len = &sva->len[vr_ref-1];
      double *vr_piv = luf->vr_piv;
      int vc_ref = luf->vc_ref;
      int *vc_len = &sva->len[vc_ref-1];
      int *pp_ind = luf->pp_ind;
      int *pp_inv = luf->pp_inv;
      int *qq_ind = luf->qq_ind;
      int *qq_inv = luf->qq_inv;
      int *rs_head = sgf->rs_head;
      int *rs_prev = sgf->rs_prev;
      int *rs_next = sgf->rs_next;
      int *cs_head = sgf->cs_head;
      int *cs_prev = sgf->cs_prev;
      int *cs_next = sgf->cs_next;
      double *vr_max = sgf->vr_max;
      char *flag = sgf->flag;
      double *work = sgf->work;
      int i, j, k, k1, k2, p, q, nnz;
      /* build matrix V = A in row-wise format */
      luf_build_v_rows(luf, rs_prev);
      /* P := Q := I, so V = U = A, F = L = I */
      for (k = 1; k <= n; k++)
      {  vr_piv[k] = 0.0;
         pp_ind[k] = pp_inv[k] = qq_ind[k] = qq_inv[k] = k;
      }
#ifdef GLP_DEBUG
      sva_check_area(sva);
      luf_check_all(luf, 1);
#endif
      /* perform singleton phase, if required */
      if (!singl)
      {  /* assume that nucleus is entire matrix U */
         k2 = 1;
      }
      else
      {  /* minimize nucleus size */
#if 0 /* 21/II-2016 */
         sgf_reduce_nuc(luf, &k1, &k2, rs_prev, rs_next);
#else
         if (sgf_reduce_nuc(luf, &k1, &k2, rs_prev, rs_next))
            return -1;
#endif
#ifdef GLP_DEBUG
         xprintf("n = %d; k1 = %d; k2 = %d\n", n, k1, k2);
#endif
         /* perform singleton phase */
         k2 = sgf_singl_phase(luf, k1, k2, sgf->updat, rs_prev, work);
      }
#ifdef GLP_DEBUG
      sva_check_area(sva);
      luf_check_all(luf, k2);
#endif
      /* initialize working arrays */
      rs_head[0] = cs_head[0] = 0;
      for (k = 1; k <= n; k++)
      {  rs_head[k] = cs_head[k] = 0;
         vr_max[k] = -1.0;
         flag[k] = 0;
         work[k] = 0.0;
      }
      /* build lists of active rows and columns of matrix V; determine
       * number of non-zeros in initial active submatrix */
      nnz = 0;
      for (k = k2; k <= n; k++)
      {  i = pp_inv[k];
         sgf_activate_row(i);
         nnz += vr_len[i];
         j = qq_ind[k];
         sgf_activate_col(j);
      }
      /* main factorization loop */
      for (k = k2; k <= n; k++)
      {  int na;
         double den;
         /* calculate density of active submatrix */
         na = n - k + 1; /* order of active submatrix */
#if 0 /* 21/VIII-2014 */
         den = (double)nnz / (double)(na * na);
#else
         den = (double)nnz / ((double)(na) * (double)(na));
#endif
         /* if active submatrix is relatively dense, switch to dense
          * phase */
#if 1 /* FIXME */
         if (na >= 5 && den >= 0.71)
         {
#ifdef GLP_DEBUG
            xprintf("na = %d; nnz = %d; den = %g\n", na, nnz, den);
#endif
            break;
         }
#endif
         /* choose pivot v[p,q] */
         if (sgf_choose_pivot(sgf, &p, &q) != 0)
            return k; /* failure */
         /* u[i,j] = v[p,q], k <= i, j <= n */
         i = pp_ind[p];
         xassert(k <= i && i <= n);
         j = qq_inv[q];
         xassert(k <= j && j <= n);
         /* move u[i,j] to position u[k,k] by implicit permutations of
          * rows and columns of matrix U */
         luf_swap_u_rows(k, i);
         luf_swap_u_cols(k, j);
         /* perform gaussian elimination */
         nnz += sgf_eliminate(sgf, p, q);
      }
#if 1 /* FIXME */
      if (k <= n)
      {  /* continue computing factorization in dense mode */
#ifdef GLP_DEBUG
         sva_check_area(sva);
         luf_check_all(luf, k);
#endif
         k = sgf_dense_phase(luf, k, sgf->updat);
         if (k != 0)
            return k; /* failure */
      }
#endif
#ifdef GLP_DEBUG
      sva_check_area(sva);
      luf_check_all(luf, n+1);
#endif
      /* defragment SVA; currently all columns of V are empty, so they
       * will have zero capacity as required by luf_build_v_cols */
      sva_defrag_area(sva);
      /* build matrix F in row-wise format */
      luf_build_f_rows(luf, rs_head);
      /* build matrix V in column-wise format */
      luf_build_v_cols(luf, sgf->updat, rs_head);
      return 0;
}


typedef struct LUFINT LUFINT;

struct LUFINT
{     /* interface to LU-factorization */
      int n_max;
      /* maximal value of n (increased automatically) */
      int valid;
      /* factorization is valid only if this flag is set */
      SVA *sva;
      /* sparse vector area (SVA) */
      LUF *luf;
      /* sparse LU-factorization */
      SGF *sgf;
      /* sparse Gaussian factorizer workspace */
      /*--------------------------------------------------------------*/
      /* control parameters */
      int sva_n_max, sva_size;
      /* parameters passed to sva_create_area */
      int delta_n0, delta_n;
      /* if n_max = 0, set n_max = n + delta_n0
       * if n_max < n, set n_max = n + delta_n */
      int sgf_updat;
      double sgf_piv_tol;
      int sgf_piv_lim;
      int sgf_suhl;
      double sgf_eps_tol;
      /* factorizer control parameters */
};

#define lufint_create _glp_lufint_create
LUFINT *lufint_create(void);
/* create interface to LU-factorization */

#define lufint_factorize _glp_lufint_factorize
int lufint_factorize(LUFINT *fi, int n, int (*col)(void *info, int j,
      int ind[], double val[]), void *info);
/* compute LU-factorization of specified matrix A */

#define lufint_delete _glp_lufint_delete
void lufint_delete(LUFINT *fi);
/* delete interface to LU-factorization */




/***********************************************************************
*  The structure FHV describes sparse updatable FHV-factorization.
*
*  The FHV-factorization has the following format:
*
*     A = F * H * V,                                                 (1)
*
*     F = P0 * L * P0',                                              (2)
*
*     H = H[1] * H[2] * ... * H[nfs],                                (3)
*
*     V = P * U * Q,                                                 (4)
*
*  where: A is a given (unsymmetric) square matrix; F, H, V are matrix
*  factors actually computed; L is a lower triangular matrix with unity
*  diagonal; U is an upper tringular matrix; H[k], k = 1, 2, ..., nfs,
*  is a row-like factor, which differs from unity matrix only in one
*  row called a non-trivial row; P0, P, Q are permutation matrices; and
*  P0' is a matrix transposed to P0.
*
*  Matrices F, V, P, Q are stored in the underlying LUF object.
*
*  Non-trivial rows of factors H[k] are stored as sparse vectors in the
*  right (static) part of the sparse vector area (SVA). Note that unity
*  diagonal elements of non-trivial rows are not stored.
*
*  Matrix P0 is stored in the same way as matrix P.
*
*  Matrices L and U are completely defined by matrices F, V, P, and Q,
*  and therefore not stored explicitly. */

typedef struct FHV FHV;

struct FHV
{     /* FHV-factorization */
      LUF *luf;
      /* LU-factorization (contains matrices F, V, P, Q) */
      /*--------------------------------------------------------------*/
      /* matrix H in the form of eta file */
      int nfs_max;
      /* maximal number of row-like factors (this limits the number of
       * updates of the factorization) */
      int nfs;
      /* current number of row-like factors, 0 <= nfs <= nfs_max */
      int *hh_ind; /* int hh_ind[1+nfs_max]; */
      /* hh_ind[0] is not used;
       * hh_ind[k], 1 <= k <= nfs, is number of non-trivial row of
       * factor H[k] */
      int hh_ref;
      /* reference number of sparse vector in SVA, which is non-trivial
       * row of factor H[1] */
#if 0 + 0
      int *hh_ptr = &sva->ptr[hh_ref-1];
      /* hh_ptr[0] is not used;
       * hh_ptr[k], 1 <= k <= nfs, is pointer to non-trivial row of
       * factor H[k] */
      int *hh_len = &sva->len[hh_ref-1];
      /* hh_len[0] is not used;
       * hh_len[k], 1 <= k <= nfs, is number of non-zero elements in
       * non-trivial row of factor H[k] */
#endif
      /*--------------------------------------------------------------*/
      /* matrix P0 */
      int *p0_ind; /* int p0_ind[1+n]; */
      /* p0_ind[i] = j means that P0[i,j] = 1 */
      int *p0_inv; /* int p0_inv[1+n]; */
      /* p0_inv[j] = i means that P0[i,j] = 1 */
};

#define fhv_ft_update _glp_fhv_ft_update
int fhv_ft_update(FHV *fhv, int q, int aq_len, const int aq_ind[],
      const double aq_val[], int ind[/*1+n*/], double val[/*1+n*/],
      double work[/*1+n*/]);
/* update FHV-factorization (Forrest-Tomlin) */

#define fhv_h_solve _glp_fhv_h_solve
void fhv_h_solve(FHV *fhv, double x[/*1+n*/]);
/* solve system H * x = b */

#define fhv_ht_solve _glp_fhv_ht_solve
void fhv_ht_solve(FHV *fhv, double x[/*1+n*/]);
/* solve system H' * x = b */


/***********************************************************************
*  fhv_ft_update - update FHV-factorization (Forrest-Tomlin)
*
*  This routine updates FHV-factorization of the original matrix A
*  after replacing its j-th column by a new one. The routine is based
*  on the method proposed by Forrest and Tomlin [1].
*
*  The parameter q specifies the number of column of A, which has been
*  replaced, 1 <= q <= n, where n is the order of A.
*
*  Row indices and numerical values of non-zero elements of the new
*  j-th column of A should be placed in locations aq_ind[1], ...,
*  aq_ind[aq_len] and aq_val[1], ..., aq_val[aq_len], respectively,
*  where aq_len is the number of non-zeros. Neither zero nor duplicate
*  elements are allowed.
*
*  The working arrays ind, val, and work should have at least 1+n
*  elements (0-th elements are not used).
*
*  RETURNS
*
*  0  The factorization has been successfully updated.
*
*  1  New matrix U = P'* V * Q' is upper triangular with zero diagonal
*     element u[s,s]. (Elimination was not performed.)
*
*  2  New matrix U = P'* V * Q' is upper triangular, and its diagonal
*     element u[s,s] or u[t,t] is too small in magnitude. (Elimination
*     was not performed.)
*
*  3  The same as 2, but after performing elimination.
*
*  4  The factorization has not been updated, because maximal number of
*     updates has been reached.
*
*  5  Accuracy test failed for the updated factorization.
*
*  BACKGROUND
*
*  The routine is based on the updating method proposed by Forrest and
*  Tomlin [1].
*
*  Let q-th column of the original matrix A have been replaced by new
*  column A[q]. Then, to keep the equality A = F * H * V, q-th column
*  of matrix V should be replaced by column V[q] = inv(F * H) * A[q].
*  From the standpoint of matrix U = P'* V * Q' such replacement is
*  equivalent to replacement of s-th column of matrix U, where s is
*  determined from q by permutation matrix Q. Thus, matrix U loses its
*  upper triangular form and becomes the following:
*
*        1   s       t   n
*     1  x x * x x x x x x
*        . x * x x x x x x
*     s  . . * x x x x x x
*        . . * x x x x x x
*        . . * . x x x x x
*        . . * . . x x x x
*     t  . . * . . . x x x
*        . . . . . . . x x
*     n  . . . . . . . . x
*
*  where t is largest row index of a non-zero element in s-th column.
*
*  The routine makes matrix U upper triangular as follows. First, it
*  moves rows and columns s+1, ..., t by one position to the left and
*  upwards, resp., and moves s-th row and s-th column to position t.
*  Due to such symmetric permutations matrix U becomes the following
*  (note that all diagonal elements remain on the diagonal, and element
*  u[s,s] becomes u[t,t]):
*
*        1   s       t   n
*     1  x x x x x x * x x
*        . x x x x x * x x
*     s  . . x x x x * x x
*        . . . x x x * x x
*        . . . . x x * x x
*        . . . . . x * x x
*     t  . . x x x x * x x
*        . . . . . . . x x
*     n  . . . . . . . . x
*
*  Then the routine performs gaussian elimination to eliminate
*  subdiagonal elements u[t,s], ..., u[t,t-1] using diagonal elements
*  u[s,s], ..., u[t-1,t-1] as pivots. During the elimination process
*  the routine permutes neither rows nor columns, so only t-th row is
*  changed. Should note that actually all operations are performed on
*  matrix V = P * U * Q, since matrix U is not stored.
*
*  To keep the equality A = F * H * V, the routine appends new row-like
*  factor H[k] to matrix H, and every time it applies elementary
*  gaussian transformation to eliminate u[t,j'] = v[p,j] using pivot
*  u[j',j'] = v[i,j], it also adds new element f[p,j] = v[p,j] / v[i,j]
*  (gaussian multiplier) to factor H[k], which initially is a unity
*  matrix. At the end of elimination process the row-like factor H[k]
*  may look as follows:
*
*        1               n          1   s       t   n
*     1  1 . . . . . . . .       1  1 . . . . . . . .
*        . 1 . . . . . . .          . 1 . . . . . . .
*        . . 1 . . . . . .       s  . . 1 . . . . . .
*     p  . x x 1 . x . x .          . . . 1 . . . . .
*        . . . . 1 . . . .          . . . . 1 . . . .
*        . . . . . 1 . . .          . . . . . 1 . . .
*        . . . . . . 1 . .       t  . . x x x x 1 . .
*        . . . . . . . 1 .          . . . . . . . 1 .
*     n  . . . . . . . . 1       n  . . . . . . . . 1
*
*              H[k]                 inv(P) * H[k] * P
*
*  If, however, s = t, no elimination is needed, in which case no new
*  row-like factor is created.
*
*  REFERENCES
*
*  1. J.J.H.Forrest and J.A.Tomlin, "Updated triangular factors of the
*     basis to maintain sparsity in the product form simplex method,"
*     Math. Prog. 2 (1972), pp. 263-78. */

int fhv_ft_update(FHV *fhv, int q, int aq_len, const int aq_ind[],
      const double aq_val[], int ind[/*1+n*/], double val[/*1+n*/],
      double work[/*1+n*/])
{     LUF *luf = fhv->luf;
      int n = luf->n;
      SVA *sva = luf->sva;
      int *sv_ind = sva->ind;
      double *sv_val = sva->val;
      int vr_ref = luf->vr_ref;
      int *vr_ptr = &sva->ptr[vr_ref-1];
      int *vr_len = &sva->len[vr_ref-1];
      int *vr_cap = &sva->cap[vr_ref-1];
      double *vr_piv = luf->vr_piv;
      int vc_ref = luf->vc_ref;
      int *vc_ptr = &sva->ptr[vc_ref-1];
      int *vc_len = &sva->len[vc_ref-1];
      int *vc_cap = &sva->cap[vc_ref-1];
      int *pp_ind = luf->pp_ind;
      int *pp_inv = luf->pp_inv;
      int *qq_ind = luf->qq_ind;
      int *qq_inv = luf->qq_inv;
      int *hh_ind = fhv->hh_ind;
      int hh_ref = fhv->hh_ref;
      int *hh_ptr = &sva->ptr[hh_ref-1];
      int *hh_len = &sva->len[hh_ref-1];
#if 1 /* FIXME */
      const double eps_tol = DBL_EPSILON;
      const double vpq_tol = 1e-5;
      const double err_tol = 1e-10;
#endif
      int end, i, i_end, i_ptr, j, j_end, j_ptr, k, len, nnz, p, p_end,
         p_ptr, ptr, q_end, q_ptr, s, t;
      double f, vpq, temp;
      /*--------------------------------------------------------------*/
      /* replace current q-th column of matrix V by new one           */
      /*--------------------------------------------------------------*/
      xassert(1 <= q && q <= n);
      /* convert new q-th column of matrix A to dense format */
      for (i = 1; i <= n; i++)
         val[i] = 0.0;
      xassert(0 <= aq_len && aq_len <= n);
      for (k = 1; k <= aq_len; k++)
      {  i = aq_ind[k];
         xassert(1 <= i && i <= n);
         xassert(val[i] == 0.0);
         xassert(aq_val[k] != 0.0);
         val[i] = aq_val[k];
      }
      /* compute new q-th column of matrix V:
       * new V[q] = inv(F * H) * (new A[q]) */
      luf->pp_ind = fhv->p0_ind;
      luf->pp_inv = fhv->p0_inv;
      luf_f_solve(luf, val);
      luf->pp_ind = pp_ind;
      luf->pp_inv = pp_inv;
      fhv_h_solve(fhv, val);
      /* q-th column of V = s-th column of U */
      s = qq_inv[q];
      /* determine row number of element v[p,q] that corresponds to
       * diagonal element u[s,s] */
      p = pp_inv[s];
      /* convert new q-th column of V to sparse format;
       * element v[p,q] = u[s,s] is not included in the element list
       * and stored separately */
      vpq = 0.0;
      len = 0;
      for (i = 1; i <= n; i++)
      {  temp = val[i];
#if 1 /* FIXME */
         if (-eps_tol < temp && temp < +eps_tol)
#endif
            /* nop */;
         else if (i == p)
            vpq = temp;
         else
         {  ind[++len] = i;
            val[len] = temp;
         }
      }
      /* clear q-th column of matrix V */
      for (q_end = (q_ptr = vc_ptr[q]) + vc_len[q];
         q_ptr < q_end; q_ptr++)
      {  /* get row index of v[i,q] */
         i = sv_ind[q_ptr];
         /* find and remove v[i,q] from i-th row */
         for (i_end = (i_ptr = vr_ptr[i]) + vr_len[i];
            sv_ind[i_ptr] != q; i_ptr++)
            /* nop */;
         xassert(i_ptr < i_end);
         sv_ind[i_ptr] = sv_ind[i_end-1];
         sv_val[i_ptr] = sv_val[i_end-1];
         vr_len[i]--;
      }
      /* now q-th column of matrix V is empty */
      vc_len[q] = 0;
      /* put new q-th column of V (except element v[p,q] = u[s,s]) in
       * column-wise format */
      if (len > 0)
      {  if (vc_cap[q] < len)
         {  if (sva->r_ptr - sva->m_ptr < len)
            {  sva_more_space(sva, len);
               sv_ind = sva->ind;
               sv_val = sva->val;
            }
            sva_enlarge_cap(sva, vc_ref-1+q, len, 0);
         }
         ptr = vc_ptr[q];
         memcpy(&sv_ind[ptr], &ind[1], len * sizeof(int));
         memcpy(&sv_val[ptr], &val[1], len * sizeof(double));
         vc_len[q] = len;
      }
      /* put new q-th column of V (except element v[p,q] = u[s,s]) in
       * row-wise format, and determine largest row number t such that
       * u[s,t] != 0 */
      t = (vpq == 0.0 ? 0 : s);
      for (k = 1; k <= len; k++)
      {  /* get row index of v[i,q] */
         i = ind[k];
         /* put v[i,q] to i-th row */
         if (vr_cap[i] == vr_len[i])
         {  /* reserve extra locations in i-th row to reduce further
             * relocations of that row */
#if 1 /* FIXME */
            int need = vr_len[i] + 5;
#endif
            if (sva->r_ptr - sva->m_ptr < need)
            {  sva_more_space(sva, need);
               sv_ind = sva->ind;
               sv_val = sva->val;
            }
            sva_enlarge_cap(sva, vr_ref-1+i, need, 0);
         }
         sv_ind[ptr = vr_ptr[i] + (vr_len[i]++)] = q;
         sv_val[ptr] = val[k];
         /* v[i,q] is non-zero; increase t */
         if (t < pp_ind[i])
            t = pp_ind[i];
      }
      /*--------------------------------------------------------------*/
      /* check if matrix U is already upper triangular                */
      /*--------------------------------------------------------------*/
      /* check if there is a spike in s-th column of matrix U, which
       * is q-th column of matrix V */
      if (s >= t)
      {  /* no spike; matrix U is already upper triangular */
         /* store its diagonal element u[s,s] = v[p,q] */
         vr_piv[p] = vpq;
         if (s > t)
         {  /* matrix U is structurally singular, because its diagonal
             * element u[s,s] = v[p,q] is exact zero */
            xassert(vpq == 0.0);
            return 1;
         }
#if 1 /* FIXME */
         else if (-vpq_tol < vpq && vpq < +vpq_tol)
#endif
         {  /* matrix U is not well conditioned, because its diagonal
             * element u[s,s] = v[p,q] is too small in magnitude */
            return 2;
         }
         else
         {  /* normal case */
            return 0;
         }
      }
      /*--------------------------------------------------------------*/
      /* perform implicit symmetric permutations of rows and columns  */
      /* of matrix U                                                  */
      /*--------------------------------------------------------------*/
      /* currently v[p,q] = u[s,s] */
      xassert(p == pp_inv[s] && q == qq_ind[s]);
      for (k = s; k < t; k++)
      {  pp_ind[pp_inv[k] = pp_inv[k+1]] = k;
         qq_inv[qq_ind[k] = qq_ind[k+1]] = k;
      }
      /* now v[p,q] = u[t,t] */
      pp_ind[pp_inv[t] = p] = qq_inv[qq_ind[t] = q] = t;
      /*--------------------------------------------------------------*/
      /* check if matrix U is already upper triangular                */
      /*--------------------------------------------------------------*/
      /* check if there is a spike in t-th row of matrix U, which is
       * p-th row of matrix V */
      for (p_end = (p_ptr = vr_ptr[p]) + vr_len[p];
         p_ptr < p_end; p_ptr++)
      {  if (qq_inv[sv_ind[p_ptr]] < t)
            break; /* spike detected */
      }
      if (p_ptr == p_end)
      {  /* no spike; matrix U is already upper triangular */
         /* store its diagonal element u[t,t] = v[p,q] */
         vr_piv[p] = vpq;
#if 1 /* FIXME */
         if (-vpq_tol < vpq && vpq < +vpq_tol)
#endif
         {  /* matrix U is not well conditioned, because its diagonal
             * element u[t,t] = v[p,q] is too small in magnitude */
            return 2;
         }
         else
         {  /* normal case */
            return 0;
         }
      }
      /*--------------------------------------------------------------*/
      /* copy p-th row of matrix V, which is t-th row of matrix U, to */
      /* working array                                                */
      /*--------------------------------------------------------------*/
      /* copy p-th row of matrix V, including element v[p,q] = u[t,t],
       * to the working array in dense format and remove these elements
       * from matrix V; since no pivoting is used, only this row will
       * change during elimination */
      for (j = 1; j <= n; j++)
         work[j] = 0.0;
      work[q] = vpq;
      for (p_end = (p_ptr = vr_ptr[p]) + vr_len[p];
         p_ptr < p_end; p_ptr++)
      {  /* get column index of v[p,j] and store this element to the
          * working array */
         work[j = sv_ind[p_ptr]] = sv_val[p_ptr];
         /* find and remove v[p,j] from j-th column */
         for (j_end = (j_ptr = vc_ptr[j]) + vc_len[j];
            sv_ind[j_ptr] != p; j_ptr++)
            /* nop */;
         xassert(j_ptr < j_end);
         sv_ind[j_ptr] = sv_ind[j_end-1];
         sv_val[j_ptr] = sv_val[j_end-1];
         vc_len[j]--;
      }
      /* now p-th row of matrix V is temporarily empty */
      vr_len[p] = 0;
      /*--------------------------------------------------------------*/
      /* perform gaussian elimination                                 */
      /*--------------------------------------------------------------*/
      /* transform p-th row of matrix V stored in working array, which
       * is t-th row of matrix U, to eliminate subdiagonal elements
       * u[t,s], ..., u[t,t-1]; corresponding gaussian multipliers will
       * form non-trivial row of new row-like factor */
      nnz = 0; /* number of non-zero gaussian multipliers */
      for (k = s; k < t; k++)
      {  /* diagonal element u[k,k] = v[i,j] is used as pivot */
         i = pp_inv[k], j = qq_ind[k];
         /* take subdiagonal element u[t,k] = v[p,j] */
         temp = work[j];
#if 1 /* FIXME */
         if (-eps_tol < temp && temp < +eps_tol)
            continue;
#endif
         /* compute and save gaussian multiplier:
          * f := u[t,k] / u[k,k] = v[p,j] / v[i,j] */
         ind[++nnz] = i;
         val[nnz] = f = work[j] / vr_piv[i];
         /* gaussian transformation to eliminate u[t,k] = v[p,j]:
          * (p-th row of V) := (p-th row of V) - f * (i-th row of V) */
         for (i_end = (i_ptr = vr_ptr[i]) + vr_len[i];
            i_ptr < i_end; i_ptr++)
            work[sv_ind[i_ptr]] -= f * sv_val[i_ptr];
      }
      /* now matrix U is again upper triangular */
#if 1 /* FIXME */
      if (-vpq_tol < work[q] && work[q] < +vpq_tol)
#endif
      {  /* however, its new diagonal element u[t,t] = v[p,q] is too
          * small in magnitude */
         return 3;
      }
      /*--------------------------------------------------------------*/
      /* create new row-like factor H[k] and add to eta file H        */
      /*--------------------------------------------------------------*/
      /* (nnz = 0 means that all subdiagonal elements were too small
       * in magnitude) */
      if (nnz > 0)
      {  if (fhv->nfs == fhv->nfs_max)
         {  /* maximal number of row-like factors has been reached */
            return 4;
         }
         k = ++(fhv->nfs);
         hh_ind[k] = p;
         /* store non-trivial row of H[k] in right (dynamic) part of
          * SVA (diagonal unity element is not stored) */
         if (sva->r_ptr - sva->m_ptr < nnz)
         {  sva_more_space(sva, nnz);
            sv_ind = sva->ind;
            sv_val = sva->val;
         }
         sva_reserve_cap(sva, fhv->hh_ref-1+k, nnz);
         ptr = hh_ptr[k];
         memcpy(&sv_ind[ptr], &ind[1], nnz * sizeof(int));
         memcpy(&sv_val[ptr], &val[1], nnz * sizeof(double));
         hh_len[k] = nnz;
      }
      /*--------------------------------------------------------------*/
      /* copy transformed p-th row of matrix V, which is t-th row of  */
      /* matrix U, from working array back to matrix V                */
      /*--------------------------------------------------------------*/
      /* copy elements of transformed p-th row of matrix V, which are
       * non-diagonal elements u[t,t+1], ..., u[t,n] of matrix U, from
       * working array to corresponding columns of matrix V (note that
       * diagonal element u[t,t] = v[p,q] not copied); also transform
       * p-th row of matrix V to sparse format */
      len = 0;
      for (k = t+1; k <= n; k++)
      {  /* j-th column of V = k-th column of U */
         j = qq_ind[k];
         /* take non-diagonal element v[p,j] = u[t,k] */
         temp = work[j];
#if 1 /* FIXME */
         if (-eps_tol < temp && temp < +eps_tol)
            continue;
#endif
         /* add v[p,j] to j-th column of matrix V */
         if (vc_cap[j] == vc_len[j])
         {  /* reserve extra locations in j-th column to reduce further
             * relocations of that column */
#if 1 /* FIXME */
            int need = vc_len[j] + 5;
#endif
            if (sva->r_ptr - sva->m_ptr < need)
            {  sva_more_space(sva, need);
               sv_ind = sva->ind;
               sv_val = sva->val;
            }
            sva_enlarge_cap(sva, vc_ref-1+j, need, 0);
         }
         sv_ind[ptr = vc_ptr[j] + (vc_len[j]++)] = p;
         sv_val[ptr] = temp;
         /* store element v[p,j] = u[t,k] to working sparse vector */
         ind[++len] = j;
         val[len] = temp;
      }
      /* copy elements from working sparse vector to p-th row of matrix
       * V (this row is currently empty) */
      if (vr_cap[p] < len)
      {  if (sva->r_ptr - sva->m_ptr < len)
         {  sva_more_space(sva, len);
            sv_ind = sva->ind;
            sv_val = sva->val;
         }
         sva_enlarge_cap(sva, vr_ref-1+p, len, 0);
      }
      ptr = vr_ptr[p];
      memcpy(&sv_ind[ptr], &ind[1], len * sizeof(int));
      memcpy(&sv_val[ptr], &val[1], len * sizeof(double));
      vr_len[p] = len;
      /* store new diagonal element u[t,t] = v[p,q] */
      vr_piv[p] = work[q];
      /*--------------------------------------------------------------*/
      /* perform accuracy test (only if new H[k] was added)           */
      /*--------------------------------------------------------------*/
      if (nnz > 0)
      {  /* copy p-th (non-trivial) row of row-like factor H[k] (except
          * unity diagonal element) to working array in dense format */
         for (j = 1; j <= n; j++)
            work[j] = 0.0;
         k = fhv->nfs;
         for (end = (ptr = hh_ptr[k]) + hh_len[k]; ptr < end; ptr++)
            work[sv_ind[ptr]] = sv_val[ptr];
         /* compute inner product of p-th (non-trivial) row of matrix
          * H[k] and q-th column of matrix V */
         temp = vr_piv[p]; /* 1 * v[p,q] */
         ptr = vc_ptr[q];
         end = ptr + vc_len[q];
         for (; ptr < end; ptr++)
            temp += work[sv_ind[ptr]] * sv_val[ptr];
         /* inner product should be equal to element v[p,q] *before*
          * matrix V was transformed */
         /* compute relative error */
         temp = fabs(vpq - temp) / (1.0 + fabs(vpq));
#if 1 /* FIXME */
         if (temp > err_tol)
#endif
         {  /* relative error is too large */
            return 5;
         }
      }
      /* factorization has been successfully updated */
      return 0;
}

/***********************************************************************
*  fhv_h_solve - solve system H * x = b
*
*  This routine solves the system H * x = b, where the matrix H is the
*  middle factor of the sparse updatable FHV-factorization.
*
*  On entry the array x should contain elements of the right-hand side
*  vector b in locations x[1], ..., x[n], where n is the order of the
*  matrix H. On exit this array will contain elements of the solution
*  vector x in the same locations. */

void fhv_h_solve(FHV *fhv, double x[/*1+n*/])
{     SVA *sva = fhv->luf->sva;
      int *sv_ind = sva->ind;
      double *sv_val = sva->val;
      int nfs = fhv->nfs;
      int *hh_ind = fhv->hh_ind;
      int hh_ref = fhv->hh_ref;
      int *hh_ptr = &sva->ptr[hh_ref-1];
      int *hh_len = &sva->len[hh_ref-1];
      int i, k, end, ptr;
      double x_i;
      for (k = 1; k <= nfs; k++)
      {  x_i = x[i = hh_ind[k]];
         for (end = (ptr = hh_ptr[k]) + hh_len[k]; ptr < end; ptr++)
            x_i -= sv_val[ptr] * x[sv_ind[ptr]];
         x[i] = x_i;
      }
      return;
}

/***********************************************************************
*  fhv_ht_solve - solve system H' * x = b
*
*  This routine solves the system H' * x = b, where H' is a matrix
*  transposed to the matrix H, which is the middle factor of the sparse
*  updatable FHV-factorization.
*
*  On entry the array x should contain elements of the right-hand side
*  vector b in locations x[1], ..., x[n], where n is the order of the
*  matrix H. On exit this array will contain elements of the solution
*  vector x in the same locations. */

void fhv_ht_solve(FHV *fhv, double x[/*1+n*/])
{     SVA *sva = fhv->luf->sva;
      int *sv_ind = sva->ind;
      double *sv_val = sva->val;
      int nfs = fhv->nfs;
      int *hh_ind = fhv->hh_ind;
      int hh_ref = fhv->hh_ref;
      int *hh_ptr = &sva->ptr[hh_ref-1];
      int *hh_len = &sva->len[hh_ref-1];
      int k, end, ptr;
      double x_j;
      for (k = nfs; k >= 1; k--)
      {  if ((x_j = x[hh_ind[k]]) == 0.0)
            continue;
         for (end = (ptr = hh_ptr[k]) + hh_len[k]; ptr < end; ptr++)
            x[sv_ind[ptr]] -= sv_val[ptr] * x_j;
      }
      return;
}





LUFINT *lufint_create(void)
{     /* create interface to LU-factorization */
      LUFINT *fi;
      fi = talloc(1, LUFINT);
      fi->n_max = 0;
      fi->valid = 0;
      fi->sva = NULL;
      fi->luf = NULL;
      fi->sgf = NULL;
      fi->sva_n_max = fi->sva_size = 0;
      fi->delta_n0 = fi->delta_n = 0;
      fi->sgf_updat = 0;
      fi->sgf_piv_tol = 0.10;
      fi->sgf_piv_lim = 4;
      fi->sgf_suhl = 1;
      fi->sgf_eps_tol = DBL_EPSILON;
      return fi;
}

int lufint_factorize(LUFINT *fi, int n, int (*col)(void *info, int j,
      int ind[], double val[]), void *info)
{     /* compute LU-factorization of specified matrix A */
      SVA *sva;
      LUF *luf;
      SGF *sgf;
      int k;
      xassert(n > 0);
      fi->valid = 0;
      /* create sparse vector area (SVA), if necessary */
      sva = fi->sva;
      if (sva == NULL)
      {  int sva_n_max = fi->sva_n_max;
         int sva_size = fi->sva_size;
         if (sva_n_max == 0)
            sva_n_max = 4 * n;
         if (sva_size == 0)
            sva_size = 10 * n;
         sva = fi->sva = sva_create_area(sva_n_max, sva_size);
      }
      /* allocate/reallocate underlying objects, if necessary */
      if (fi->n_max < n)
      {  int n_max = fi->n_max;
         if (n_max == 0)
            n_max = fi->n_max = n + fi->delta_n0;
         else
            n_max = fi->n_max = n + fi->delta_n;
         xassert(n_max >= n);
         /* allocate/reallocate LU-factorization (LUF) */
         luf = fi->luf;
         if (luf == NULL)
         {  luf = fi->luf = talloc(1, LUF);
            memset(luf, 0, sizeof(LUF));
            luf->sva = sva;
         }
         else
         {  tfree(luf->vr_piv);
            tfree(luf->pp_ind);
            tfree(luf->pp_inv);
            tfree(luf->qq_ind);
            tfree(luf->qq_inv);
         }
         luf->vr_piv = talloc(1+n_max, double);
         luf->pp_ind = talloc(1+n_max, int);
         luf->pp_inv = talloc(1+n_max, int);
         luf->qq_ind = talloc(1+n_max, int);
         luf->qq_inv = talloc(1+n_max, int);
         /* allocate/reallocate factorizer workspace (SGF) */
         sgf = fi->sgf;
         if (sgf == NULL)
         {  sgf = fi->sgf = talloc(1, SGF);
            memset(sgf, 0, sizeof(SGF));
            sgf->luf = luf;
         }
         else
         {  tfree(sgf->rs_head);
            tfree(sgf->rs_prev);
            tfree(sgf->rs_next);
            tfree(sgf->cs_head);
            tfree(sgf->cs_prev);
            tfree(sgf->cs_next);
            tfree(sgf->vr_max);
            tfree(sgf->flag);
            tfree(sgf->work);
         }
         sgf->rs_head = talloc(1+n_max, int);
         sgf->rs_prev = talloc(1+n_max, int);
         sgf->rs_next = talloc(1+n_max, int);
         sgf->cs_head = talloc(1+n_max, int);
         sgf->cs_prev = talloc(1+n_max, int);
         sgf->cs_next = talloc(1+n_max, int);
         sgf->vr_max = talloc(1+n_max, double);
         sgf->flag = talloc(1+n_max, char);
         sgf->work = talloc(1+n_max, double);
      }
      luf = fi->luf;
      sgf = fi->sgf;
#if 1 /* FIXME */
      /* initialize SVA */
      sva->n = 0;
      sva->m_ptr = 1;
      sva->r_ptr = sva->size + 1;
      sva->head = sva->tail = 0;
#endif
      /* allocate sparse vectors in SVA */
      luf->n = n;
      luf->fr_ref = sva_alloc_vecs(sva, n);
      luf->fc_ref = sva_alloc_vecs(sva, n);
      luf->vr_ref = sva_alloc_vecs(sva, n);
      luf->vc_ref = sva_alloc_vecs(sva, n);
      /* store matrix V = A in column-wise format */
      luf_store_v_cols(luf, col, info, sgf->rs_prev, sgf->work);
      /* setup factorizer control parameters */
      sgf->updat = fi->sgf_updat;
      sgf->piv_tol = fi->sgf_piv_tol;
      sgf->piv_lim = fi->sgf_piv_lim;
      sgf->suhl = fi->sgf_suhl;
      sgf->eps_tol = fi->sgf_eps_tol;
      /* compute LU-factorization of specified matrix A */
      k = sgf_factorize(sgf, 1);
      if (k == 0)
         fi->valid = 1;
      return k;
}

void lufint_delete(LUFINT *fi)
{     /* delete interface to LU-factorization */
      SVA *sva = fi->sva;
      LUF *luf = fi->luf;
      SGF *sgf = fi->sgf;
      if (sva != NULL)
         sva_delete_area(sva);
      if (luf != NULL)
      {  tfree(luf->vr_piv);
         tfree(luf->pp_ind);
         tfree(luf->pp_inv);
         tfree(luf->qq_ind);
         tfree(luf->qq_inv);
         tfree(luf);
      }
      if (sgf != NULL)
      {  tfree(sgf->rs_head);
         tfree(sgf->rs_prev);
         tfree(sgf->rs_next);
         tfree(sgf->cs_head);
         tfree(sgf->cs_prev);
         tfree(sgf->cs_next);
         tfree(sgf->vr_max);
         tfree(sgf->flag);
         tfree(sgf->work);
         tfree(sgf);
      }
      tfree(fi);
      return;
}
typedef struct FHVINT FHVINT;

struct FHVINT
{     /* interface to FHV-factorization */
      int valid;
      /* factorization is valid only if this flag is set */
      FHV fhv;
      /* FHV-factorization */
      LUFINT *lufi;
      /* interface to underlying LU-factorization */
      /*--------------------------------------------------------------*/
      /* control parameters */
      int nfs_max;
      /* required maximal number of row-like factors */
};

#define fhvint_create _glp_fhvint_create
FHVINT *fhvint_create(void);
/* create interface to FHV-factorization */

#define fhvint_factorize _glp_fhvint_factorize
int fhvint_factorize(FHVINT *fi, int n, int (*col)(void *info, int j,
      int ind[], double val[]), void *info);
/* compute FHV-factorization of specified matrix A */

#define fhvint_update _glp_fhvint_update
int fhvint_update(FHVINT *fi, int j, int len, const int ind[],
      const double val[]);
/* update FHV-factorization after replacing j-th column of A */

#define fhvint_ftran _glp_fhvint_ftran
void fhvint_ftran(FHVINT *fi, double x[]);
/* solve system A * x = b */

#define fhvint_btran _glp_fhvint_btran
void fhvint_btran(FHVINT *fi, double x[]);
/* solve system A'* x = b */

#define fhvint_estimate _glp_fhvint_estimate
double fhvint_estimate(FHVINT *fi);
/* estimate 1-norm of inv(A) */

#define fhvint_delete _glp_fhvint_delete
void fhvint_delete(FHVINT *fi);
/* delete interface to FHV-factorization */



FHVINT *fhvint_create(void)
{     /* create interface to FHV-factorization */
      FHVINT *fi;
      fi = talloc(1, FHVINT);
      memset(fi, 0, sizeof(FHVINT));
      fi->lufi = lufint_create();
      return fi;
}

int fhvint_factorize(FHVINT *fi, int n, int (*col)(void *info, int j,
      int ind[], double val[]), void *info)
{     /* compute FHV-factorization of specified matrix A */
      int nfs_max, old_n_max, n_max, k, ret;
      xassert(n > 0);
      fi->valid = 0;
      /* get required value of nfs_max */
      nfs_max = fi->nfs_max;
      if (nfs_max == 0)
         nfs_max = 100;
      xassert(nfs_max > 0);
      /* compute factorization of specified matrix A */
      old_n_max = fi->lufi->n_max;
      fi->lufi->sva_n_max = 4 * n + nfs_max;
      fi->lufi->sgf_updat = 1;
      ret = lufint_factorize(fi->lufi, n, col, info);
      n_max = fi->lufi->n_max;
      /* allocate/reallocate arrays, if necessary */
      if (fi->fhv.nfs_max != nfs_max)
      {  if (fi->fhv.hh_ind != NULL)
            tfree(fi->fhv.hh_ind);
         fi->fhv.hh_ind = talloc(1+nfs_max, int);
      }
      if (old_n_max < n_max)
      {  if (fi->fhv.p0_ind != NULL)
            tfree(fi->fhv.p0_ind);
         if (fi->fhv.p0_inv != NULL)
            tfree(fi->fhv.p0_inv);
         fi->fhv.p0_ind = talloc(1+n_max, int);
         fi->fhv.p0_inv = talloc(1+n_max, int);
      }
      /* initialize FHV-factorization */
      fi->fhv.luf = fi->lufi->luf;
      fi->fhv.nfs_max = nfs_max;
      /* H := I */
      fi->fhv.nfs = 0;
      fi->fhv.hh_ref = sva_alloc_vecs(fi->lufi->sva, nfs_max);
      /* P0 := P */
      for (k = 1; k <= n; k++)
      {  fi->fhv.p0_ind[k] = fi->fhv.luf->pp_ind[k];
         fi->fhv.p0_inv[k] = fi->fhv.luf->pp_inv[k];
      }
      /* set validation flag */
      if (ret == 0)
         fi->valid = 1;
      return ret;
}

int fhvint_update(FHVINT *fi, int j, int len, const int ind[],
      const double val[])
{     /* update FHV-factorization after replacing j-th column of A */
      SGF *sgf = fi->lufi->sgf;
      int *ind1 = sgf->rs_next;
      double *val1 = sgf->vr_max;
      double *work = sgf->work;
      int ret;
      xassert(fi->valid);
      ret = fhv_ft_update(&fi->fhv, j, len, ind, val, ind1, val1, work);
      if (ret != 0)
         fi->valid = 0;
      return ret;
}

void fhvint_ftran(FHVINT *fi, double x[])
{     /* solve system A * x = b */
      FHV *fhv = &fi->fhv;
      LUF *luf = fhv->luf;
      int n = luf->n;
      int *pp_ind = luf->pp_ind;
      int *pp_inv = luf->pp_inv;
      SGF *sgf = fi->lufi->sgf;
      double *work = sgf->work;
      xassert(fi->valid);
      /* A = F * H * V */
      /* x = inv(A) * b = inv(V) * inv(H) * inv(F) * b */
      luf->pp_ind = fhv->p0_ind;
      luf->pp_inv = fhv->p0_inv;
      luf_f_solve(luf, x);
      luf->pp_ind = pp_ind;
      luf->pp_inv = pp_inv;
      fhv_h_solve(fhv, x);
      luf_v_solve(luf, x, work);
      memcpy(&x[1], &work[1], n * sizeof(double));
      return;
}

void fhvint_btran(FHVINT *fi, double x[])
{     /* solve system A'* x = b */
      FHV *fhv = &fi->fhv;
      LUF *luf = fhv->luf;
      int n = luf->n;
      int *pp_ind = luf->pp_ind;
      int *pp_inv = luf->pp_inv;
      SGF *sgf = fi->lufi->sgf;
      double *work = sgf->work;
      xassert(fi->valid);
      /* A' = (F * H * V)' = V'* H'* F' */
      /* x = inv(A') * b = inv(F') * inv(H') * inv(V') * b */
      luf_vt_solve(luf, x, work);
      fhv_ht_solve(fhv, work);
      luf->pp_ind = fhv->p0_ind;
      luf->pp_inv = fhv->p0_inv;
      luf_ft_solve(luf, work);
      luf->pp_ind = pp_ind;
      luf->pp_inv = pp_inv;
      memcpy(&x[1], &work[1], n * sizeof(double));
      return;
}

double fhvint_estimate(FHVINT *fi)
{     /* estimate 1-norm of inv(A) */
      double norm;
      xassert(fi->valid);
      xassert(fi->fhv.nfs == 0);
      norm = luf_estimate_norm(fi->fhv.luf, fi->lufi->sgf->vr_max,
         fi->lufi->sgf->work);
      return norm;
}

void fhvint_delete(FHVINT *fi)
{     /* delete interface to FHV-factorization */
      lufint_delete(fi->lufi);
      if (fi->fhv.hh_ind != NULL)
         tfree(fi->fhv.hh_ind);
      if (fi->fhv.p0_ind != NULL)
         tfree(fi->fhv.p0_ind);
      if (fi->fhv.p0_inv != NULL)
         tfree(fi->fhv.p0_inv);
      tfree(fi);
      return;
}


/***********************************************************************
*  The structure BTF describes BT-factorization, which is sparse block
*  triangular LU-factorization.
*
*  The BT-factorization has the following format:
*
*     A = P * A~ * Q,                                                (1)
*
*  where A is a given (unsymmetric) square matrix, A~ is an upper block
*  triangular matrix (see below), P and Q are permutation matrices. All
*  the matrices have the same order n.
*
*  The matrix A~, which is a permuted version of the original matrix A,
*  has the following structure:
*
*     A~[1,1]  A~[1,2]  ...  A~[1,num-1]      A~[1,num]
*
*              A~[2,2]  ...  A~[2,num-1]      A~[2,num]
*
*                       . . . . . . . . .                            (2)
*
*                        A~[num-1,num-1]  A~[num-1,num]
*
*                                           A~[num,num]
*
*  where A~[i,j] is a submatrix called a "block," num is the number of
*  blocks. Each diagonal block A~[k,k] is a non-singular square matrix,
*  and each subdiagonal block A~[i,j], i > j, is a zero submatrix, thus
*  A~ is an upper block triangular matrix.
*
*  Permutation matrices P and Q are stored in ordinary arrays in both
*  row- and column-like formats.
*
*  The original matrix A is stored in both row- and column-wise sparse
*  formats in the associated sparse vector area (SVA). Should note that
*  elements of all diagonal blocks A~[k,k] in matrix A are set to zero
*  (i.e. removed), so only elements of non-diagonal blocks are stored.
*
*  Each diagonal block A~[k,k], 1 <= k <= num, is stored in the form of
*  LU-factorization (see the module LUF). */

typedef struct BTF BTF;

struct BTF
{     /* sparse block triangular LU-factorization */
      int n;
      /* order of matrices A, A~, P, Q */
      SVA *sva;
      /* associated sparse vector area used to store rows and columns
       * of matrix A as well as sparse vectors for LU-factorizations of
       * all diagonal blocks A~[k,k] */
      /*--------------------------------------------------------------*/
      /* matrix P */
      int *pp_ind; /* int pp_ind[1+n]; */
      /* pp_ind[i] = j means that P[i,j] = 1 */
      int *pp_inv; /* int pp_inv[1+n]; */
      /* pp_inv[j] = i means that P[i,j] = 1 */
      /* if i-th row of matrix A is i'-th row of matrix A~, then
       * pp_ind[i] = i' and pp_inv[i'] = i */
      /*--------------------------------------------------------------*/
      /* matrix Q */
      int *qq_ind; /* int qq_ind[1+n]; */
      /* qq_ind[i] = j means that Q[i,j] = 1 */
      int *qq_inv; /* int qq_inv[1+n]; */
      /* qq_inv[j] = i means that Q[i,j] = 1 */
      /* if j-th column of matrix A is j'-th column of matrix A~, then
       * qq_ind[j'] = j and qq_inv[j] = j' */
      /*--------------------------------------------------------------*/
      /* block triangular structure of matrix A~ */
      int num;
      /* number of diagonal blocks, 1 <= num <= n */
      int *beg; /* int beg[1+num+1]; */
      /* beg[0] is not used;
       * beg[k], 1 <= k <= num, is index of first row/column of k-th
       * block of matrix A~;
       * beg[num+1] is always n+1;
       * note that order (size) of k-th diagonal block can be computed
       * as beg[k+1] - beg[k] */
      /*--------------------------------------------------------------*/
      /* original matrix A in row-wise format */
      /* NOTE: elements of all diagonal blocks A~[k,k] are removed */
      int ar_ref;
      /* reference number of sparse vector in SVA, which is the first
       * row of matrix A */
#if 0 + 0
      int *ar_ptr = &sva->ptr[ar_ref-1];
      /* ar_ptr[0] is not used;
       * ar_ptr[i], 1 <= i <= n, is pointer to i-th row in SVA */
      int *ar_len = &sva->ptr[ar_ref-1];
      /* ar_len[0] is not used;
       * ar_len[i], 1 <= i <= n, is length of i-th row */
#endif
      /*--------------------------------------------------------------*/
      /* original matrix A in column-wise format */
      /* NOTE: elements of all diagonal blocks A~[k,k] are removed */
      int ac_ref;
      /* reference number of sparse vector in SVA, which is the first
       * column of matrix A */
#if 0 + 0
      int *ac_ptr = &sva->ptr[ac_ref-1];
      /* ac_ptr[0] is not used;
       * ac_ptr[j], 1 <= j <= n, is pointer to j-th column in SVA */
      int *ac_len = &sva->ptr[ac_ref-1];
      /* ac_len[0] is not used;
       * ac_len[j], 1 <= j <= n, is length of j-th column */
#endif
      /*--------------------------------------------------------------*/
      /* LU-factorizations of diagonal blocks A~[k,k] */
      /* to decrease overhead expenses similar arrays for all LUFs are
       * packed into a single array; for example, elements fr_ptr[1],
       * ..., fr_ptr[n1], where n1 = beg[2] - beg[1], are related to
       * LUF for first diagonal block A~[1,1], elements fr_ptr[n1+1],
       * ..., fr_ptr[n1+n2], where n2 = beg[3] - beg[2], are related to
       * LUF for second diagonal block A~[2,2], etc.; in other words,
       * elements related to LUF for k-th diagonal block A~[k,k] have
       * indices beg[k], beg[k]+1, ..., beg[k+1]-1 */
      /* for details about LUF see description of the LUF module */
      int fr_ref;
      /* reference number of sparse vector in SVA, which is the first
         row of matrix F for first diagonal block A~[1,1] */
      int fc_ref;
      /* reference number of sparse vector in SVA, which is the first
         column of matrix F for first diagonal block A~[1,1] */
      int vr_ref;
      /* reference number of sparse vector in SVA, which is the first
         row of matrix V for first diagonal block A~[1,1] */
      double *vr_piv; /* double vr_piv[1+n]; */
      /* vr_piv[0] is not used;
         vr_piv[1,...,n] are pivot elements for all diagonal blocks */
      int vc_ref;
      /* reference number of sparse vector in SVA, which is the first
         column of matrix V for first diagonal block A~[1,1] */
      int *p1_ind; /* int p1_ind[1+n]; */
      int *p1_inv; /* int p1_inv[1+n]; */
      int *q1_ind; /* int q1_ind[1+n]; */
      int *q1_inv; /* int q1_inv[1+n]; */
      /* permutation matrices P and Q for all diagonal blocks */
};

#define btf_store_a_cols _glp_btf_store_a_cols
int btf_store_a_cols(BTF *btf, int (*col)(void *info, int j, int ind[],
      double val[]), void *info, int ind[], double val[]);
/* store pattern of matrix A in column-wise format */

#define btf_make_blocks _glp_btf_make_blocks
int btf_make_blocks(BTF *btf);
/* permutations to block triangular form */

#define btf_check_blocks _glp_btf_check_blocks
void btf_check_blocks(BTF *btf);
/* check structure of matrix A~ */

#define btf_build_a_rows _glp_btf_build_a_rows
void btf_build_a_rows(BTF *btf, int len[/*1+n*/]);
/* build matrix A in row-wise format */

#define btf_a_solve _glp_btf_a_solve
void btf_a_solve(BTF *btf, double b[/*1+n*/], double x[/*1+n*/],
      double w1[/*1+n*/], double w2[/*1+n*/]);
/* solve system A * x = b */

#define btf_at_solve _glp_btf_at_solve
void btf_at_solve(BTF *btf, double b[/*1+n*/], double x[/*1+n*/],
      double w1[/*1+n*/], double w2[/*1+n*/]);
/* solve system A'* x = b */

#define btf_at_solve1 _glp_btf_at_solve1
void btf_at_solve1(BTF *btf, double e[/*1+n*/], double y[/*1+n*/],
      double w1[/*1+n*/], double w2[/*1+n*/]);
/* solve system A'* y = e' to cause growth in y */

#define btf_estimate_norm _glp_btf_estimate_norm
double btf_estimate_norm(BTF *btf, double w1[/*1+n*/], double
      w2[/*1+n*/], double w3[/*1+n*/], double w4[/*1+n*/]);
/* estimate 1-norm of inv(A) */


#define mc13d _glp_mc13d
int mc13d(int n, const int icn[], const int ip[], const int lenr[],
      int ior[], int ib[], int lowl[], int numb[], int prev[]);
/* permutations to block triangular form */


/***********************************************************************
*  NAME
*
*  mc13d - permutations to block triangular form
*
*  SYNOPSIS
*
*  #include "mc13d.h"
*  int mc13d(int n, const int icn[], const int ip[], const int lenr[],
*     int ior[], int ib[], int lowl[], int numb[], int prev[]);
*
*  DESCRIPTION
*
*  Given the column numbers of the nonzeros in each row of the sparse
*  matrix, the routine mc13d finds a symmetric permutation that makes
*  the matrix block lower triangular.
*
*  INPUT PARAMETERS
*
*  n     order of the matrix.
*
*  icn   array containing the column indices of the non-zeros. Those
*        belonging to a single row must be contiguous but the ordering
*        of column indices within each row is unimportant and wasted
*        space between rows is permitted.
*
*  ip    ip[i], i = 1,2,...,n, is the position in array icn of the
*        first column index of a non-zero in row i.
*
*  lenr  lenr[i], i = 1,2,...,n, is the number of non-zeros in row i.
*
*  OUTPUT PARAMETERS
*
*  ior   ior[i], i = 1,2,...,n, gives the position on the original
*        ordering of the row or column which is in position i in the
*        permuted form.
*
*  ib    ib[i], i = 1,2,...,num, is the row number in the permuted
*        matrix of the beginning of block i, 1 <= num <= n.
*
*  WORKING ARRAYS
*
*  arp   working array of length [1+n], where arp[0] is not used.
*        arp[i] is one less than the number of unsearched edges leaving
*        node i. At the end of the algorithm it is set to a permutation
*        which puts the matrix in block lower triangular form.
*
*  ib    working array of length [1+n], where ib[0] is not used.
*        ib[i] is the position in the ordering of the start of the ith
*        block. ib[n+1-i] holds the node number of the ith node on the
*        stack.
*
*  lowl  working array of length [1+n], where lowl[0] is not used.
*        lowl[i] is the smallest stack position of any node to which a
*        path from node i has been found. It is set to n+1 when node i
*        is removed from the stack.
*
*  numb  working array of length [1+n], where numb[0] is not used.
*        numb[i] is the position of node i in the stack if it is on it,
*        is the permuted order of node i for those nodes whose final
*        position has been found and is otherwise zero.
*
*  prev  working array of length [1+n], where prev[0] is not used.
*        prev[i] is the node at the end of the path when node i was
*        placed on the stack.
*
*  RETURNS
*
*  The routine mc13d returns num, the number of blocks found. */

int mc13d(int n, const int icn[], const int ip[], const int lenr[],
      int ior[], int ib[], int lowl[], int numb[], int prev[])
{     int *arp = ior;
      int dummy, i, i1, i2, icnt, ii, isn, ist, ist1, iv, iw, j, lcnt,
         nnm1, num, stp;
      /* icnt is the number of nodes whose positions in final ordering
       * have been found. */
      icnt = 0;
      /* num is the number of blocks that have been found. */
      num = 0;
      nnm1 = n + n - 1;
      /* Initialization of arrays. */
      for (j = 1; j <= n; j++)
      {  numb[j] = 0;
         arp[j] = lenr[j] - 1;
      }
      for (isn = 1; isn <= n; isn++)
      {  /* Look for a starting node. */
         if (numb[isn] != 0) continue;
         iv = isn;
         /* ist is the number of nodes on the stack ... it is the stack
          * pointer. */
         ist = 1;
         /* Put node iv at beginning of stack. */
         lowl[iv] = numb[iv] = 1;
         ib[n] = iv;
         /* The body of this loop puts a new node on the stack or
          * backtracks. */
         for (dummy = 1; dummy <= nnm1; dummy++)
         {  i1 = arp[iv];
            /* Have all edges leaving node iv been searched? */
            if (i1 >= 0)
            {  i2 = ip[iv] + lenr[iv] - 1;
               i1 = i2 - i1;
               /* Look at edges leaving node iv until one enters a new
                * node or all edges are exhausted. */
               for (ii = i1; ii <= i2; ii++)
               {  iw = icn[ii];
                  /* Has node iw been on stack already? */
                  if (numb[iw] == 0) goto L70;
                  /* Update value of lowl[iv] if necessary. */
                  if (lowl[iw] < lowl[iv]) lowl[iv] = lowl[iw];
               }
               /* There are no more edges leaving node iv. */
               arp[iv] = -1;
            }
            /* Is node iv the root of a block? */
            if (lowl[iv] < numb[iv]) goto L60;
            /* Order nodes in a block. */
            num++;
            ist1 = n + 1 - ist;
            lcnt = icnt + 1;
            /* Peel block off the top of the stack starting at the top
             * and working down to the root of the block. */
            for (stp = ist1; stp <= n; stp++)
            {  iw = ib[stp];
               lowl[iw] = n + 1;
               numb[iw] = ++icnt;
               if (iw == iv) break;
            }
            ist = n - stp;
            ib[num] = lcnt;
            /* Are there any nodes left on the stack? */
            if (ist != 0) goto L60;
            /* Have all the nodes been ordered? */
            if (icnt < n) break;
            goto L100;
L60:        /* Backtrack to previous node on path. */
            iw = iv;
            iv = prev[iv];
            /* Update value of lowl[iv] if necessary. */
            if (lowl[iw] < lowl[iv]) lowl[iv] = lowl[iw];
            continue;
L70:        /* Put new node on the stack. */
            arp[iv] = i2 - ii - 1;
            prev[iw] = iv;
            iv = iw;
            lowl[iv] = numb[iv] = ++ist;
            ib[n+1-ist] = iv;
         }
      }
L100: /* Put permutation in the required form. */
      for (i = 1; i <= n; i++)
         arp[numb[i]] = i;
      return num;
}

/**********************************************************************/

#ifdef GLP_TEST

void test(int n, int ipp);

int main(void)
{     /* test program for routine mc13d */
      test( 1,   0);
      test( 2,   1);
      test( 2,   2);
      test( 3,   3);
      test( 4,   4);
      test( 5,  10);
      test(10,  10);
      test(10,  20);
      test(20,  20);
      test(20,  50);
      test(50,  50);
      test(50, 200);
      return 0;
}

void fa01bs(int max, int *nrand);

void setup(int n, char a[1+50][1+50], int ip[], int icn[], int lenr[]);

void test(int n, int ipp)
{     int ip[1+50], icn[1+1000], ior[1+50], ib[1+51], iw[1+150],
         lenr[1+50];
      char a[1+50][1+50], hold[1+100];
      int i, ii, iblock, ij, index, j, jblock, jj, k9, num;
      xprintf("\n\n\nMatrix is of order %d and has %d off-diagonal non-"
         "zeros\n", n, ipp);
      for (j = 1; j <= n; j++)
      {  for (i = 1; i <= n; i++)
            a[i][j] = 0;
         a[j][j] = 1;
      }
      for (k9 = 1; k9 <= ipp; k9++)
      {  /* these statements should be replaced by calls to your
          * favorite random number generator to place two pseudo-random
          * numbers between 1 and n in the variables i and j */
         for (;;)
         {  fa01bs(n, &i);
            fa01bs(n, &j);
            if (!a[i][j]) break;
         }
         a[i][j] = 1;
      }
      /* setup converts matrix a[i,j] to required sparsity-oriented
       * storage format */
      setup(n, a, ip, icn, lenr);
      num = mc13d(n, icn, ip, lenr, ior, ib, &iw[0], &iw[n], &iw[n+n]);
      /* output reordered matrix with blocking to improve clarity */
      xprintf("\nThe reordered matrix which has %d block%s is of the fo"
         "rm\n", num, num == 1 ? "" : "s");
      ib[num+1] = n + 1;
      index = 100;
      iblock = 1;
      for (i = 1; i <= n; i++)
      {  for (ij = 1; ij <= index; ij++)
            hold[ij] = ' ';
         if (i == ib[iblock])
         {  xprintf("\n");
            iblock++;
         }
         jblock = 1;
         index = 0;
         for (j = 1; j <= n; j++)
         {  if (j == ib[jblock])
            {  hold[++index] = ' ';
               jblock++;
            }
            ii = ior[i];
            jj = ior[j];
            hold[++index] = (char)(a[ii][jj] ? 'X' : '0');
         }
         xprintf("%.*s\n", index, &hold[1]);
      }
      xprintf("\nThe starting point for each block is given by\n");
      for (i = 1; i <= num; i++)
      {  if ((i - 1) % 12 == 0) xprintf("\n");
         xprintf(" %4d", ib[i]);
      }
      xprintf("\n");
      return;
}

void setup(int n, char a[1+50][1+50], int ip[], int icn[], int lenr[])
{     int i, j, ind;
      for (i = 1; i <= n; i++)
         lenr[i] = 0;
      ind = 1;
      for (i = 1; i <= n; i++)
      {  ip[i] = ind;
         for (j = 1; j <= n; j++)
         {  if (a[i][j])
            {  lenr[i]++;
               icn[ind++] = j;
            }
         }
      }
      return;
}

double g = 1431655765.0;

double fa01as(int i)
{     /* random number generator */
      g = fmod(g * 9228907.0, 4294967296.0);
      if (i >= 0)
         return g / 4294967296.0;
      else
         return 2.0 * g / 4294967296.0 - 1.0;
}

void fa01bs(int max, int *nrand)
{     *nrand = (int)(fa01as(1) * (double)max) + 1;
      return;
}
#endif

#define mc21a _glp_mc21a
int mc21a(int n, const int icn[], const int ip[], const int lenr[],
      int iperm[], int pr[], int arp[], int cv[], int out[]);
/* permutations for zero-free diagonal */


/***********************************************************************
*  NAME
*
*  mc21a - permutations for zero-free diagonal
*
*  SYNOPSIS
*
*  #include "mc21a.h"
*  int mc21a(int n, const int icn[], const int ip[], const int lenr[],
*     int iperm[], int pr[], int arp[], int cv[], int out[]);
*
*  DESCRIPTION
*
*  Given the pattern of nonzeros of a sparse matrix, the routine mc21a
*  attempts to find a permutation of its rows that makes the matrix have
*  no zeros on its diagonal.
*
*  INPUT PARAMETERS
*
*  n     order of matrix.
*
*  icn   array containing the column indices of the non-zeros. Those
*        belonging to a single row must be contiguous but the ordering
*        of column indices within each row is unimportant and wasted
*        space between rows is permitted.
*
*  ip    ip[i], i = 1,2,...,n, is the position in array icn of the
*        first column index of a non-zero in row i.
*
*  lenr  lenr[i], i = 1,2,...,n, is the number of non-zeros in row i.
*
*  OUTPUT PARAMETER
*
*  iperm contains permutation to make diagonal have the smallest
*        number of zeros on it. Elements (iperm[i], i), i = 1,2,...,n,
*        are non-zero at the end of the algorithm unless the matrix is
*        structurally singular. In this case, (iperm[i], i) will be
*        zero for n - numnz entries.
*
*  WORKING ARRAYS
*
*  pr    working array of length [1+n], where pr[0] is not used.
*        pr[i] is the previous row to i in the depth first search.
*
*  arp   working array of length [1+n], where arp[0] is not used.
*        arp[i] is one less than the number of non-zeros in row i which
*        have not been scanned when looking for a cheap assignment.
*
*  cv    working array of length [1+n], where cv[0] is not used.
*        cv[i] is the most recent row extension at which column i was
*        visited.
*
*  out   working array of length [1+n], where out[0] is not used.
*        out[i] is one less than the number of non-zeros in row i
*        which have not been scanned during one pass through the main
*        loop.
*
*  RETURNS
*
*  The routine mc21a returns numnz, the number of non-zeros on diagonal
*  of permuted matrix. */

int mc21a(int n, const int icn[], const int ip[], const int lenr[],
      int iperm[], int pr[], int arp[], int cv[], int out[])
{     int i, ii, in1, in2, j, j1, jord, k, kk, numnz;
      /* Initialization of arrays. */
      for (i = 1; i <= n; i++)
      {  arp[i] = lenr[i] - 1;
         cv[i] = iperm[i] = 0;
      }
      numnz = 0;
      /* Main loop. */
      /* Each pass round this loop either results in a new assignment
       * or gives a row with no assignment. */
      for (jord = 1; jord <= n; jord++)
      {  j = jord;
         pr[j] = -1;
         for (k = 1; k <= jord; k++)
         {  /* Look for a cheap assignment. */
            in1 = arp[j];
            if (in1 >= 0)
            {  in2 = ip[j] + lenr[j] - 1;
               in1 = in2 - in1;
               for (ii = in1; ii <= in2; ii++)
               {  i = icn[ii];
                  if (iperm[i] == 0) goto L110;
               }
               /* No cheap assignment in row. */
               arp[j] = -1;
            }
            /* Begin looking for assignment chain starting with row j.*/
            out[j] = lenr[j] - 1;
            /* Inner loop. Extends chain by one or backtracks. */
            for (kk = 1; kk <= jord; kk++)
            {  in1 = out[j];
               if (in1 >= 0)
               {  in2 = ip[j] + lenr[j] - 1;
                  in1 = in2 - in1;
                  /* Forward scan. */
                  for (ii = in1; ii <= in2; ii++)
                  {  i = icn[ii];
                     if (cv[i] != jord)
                     {  /* Column i has not yet been accessed during
                         * this pass. */
                        j1 = j;
                        j = iperm[i];
                        cv[i] = jord;
                        pr[j] = j1;
                        out[j1] = in2 - ii - 1;
                        goto L100;
                     }
                  }
               }
               /* Backtracking step. */
               j = pr[j];
               if (j == -1) goto L130;
            }
L100:       ;
         }
L110:    /* New assignment is made. */
         iperm[i] = j;
         arp[j] = in2 - ii - 1;
         numnz++;
         for (k = 1; k <= jord; k++)
         {  j = pr[j];
            if (j == -1) break;
            ii = ip[j] + lenr[j] - out[j] - 2;
            i = icn[ii];
            iperm[i] = j;
         }
L130:    ;
      }
      /* If matrix is structurally singular, we now complete the
       * permutation iperm. */
      if (numnz < n)
      {  for (i = 1; i <= n; i++)
            arp[i] = 0;
         k = 0;
         for (i = 1; i <= n; i++)
         {  if (iperm[i] == 0)
               out[++k] = i;
            else
               arp[iperm[i]] = i;
         }
         k = 0;
         for (i = 1; i <= n; i++)
         {  if (arp[i] == 0)
               iperm[out[++k]] = i;
         }
      }
      return numnz;
}

/**********************************************************************/

#ifdef GLP_TEST

int sing;

void ranmat(int m, int n, int icn[], int iptr[], int nnnp1, int *knum,
      int iw[]);

void fa01bs(int max, int *nrand);

int main(void)
{     /* test program for the routine mc21a */
      /* these runs on random matrices cause all possible statements in
       * mc21a to be executed */
      int i, iold, j, j1, j2, jj, knum, l, licn, n, nov4, num, numnz;
      int ip[1+21], icn[1+1000], iperm[1+20], lenr[1+20], iw1[1+80];
      licn = 1000;
      /* run on random matrices of orders 1 through 20 */
      for (n = 1; n <= 20; n++)
      {  nov4 = n / 4;
         if (nov4 < 1) nov4 = 1;
L10:     fa01bs(nov4, &l);
         knum = l * n;
         /* knum is requested number of non-zeros in random matrix */
         if (knum > licn) goto L10;
         /* if sing is false, matrix is guaranteed structurally
          * non-singular */
         sing = ((n / 2) * 2 == n);
         /* call to subroutine to generate random matrix */
         ranmat(n, n, icn, ip, n+1, &knum, iw1);
         /* knum is now actual number of non-zeros in random matrix */
         if (knum > licn) goto L10;
         xprintf("n = %2d; nz = %4d; sing = %d\n", n, knum, sing);
         /* set up array of row lengths */
         for (i = 1; i <= n; i++)
            lenr[i] = ip[i+1] - ip[i];
         /* call to mc21a */
         numnz = mc21a(n, icn, ip, lenr, iperm, &iw1[0], &iw1[n],
            &iw1[n+n], &iw1[n+n+n]);
         /* testing to see if there are numnz non-zeros on the diagonal
          * of the permuted matrix. */
         num = 0;
         for (i = 1; i <= n; i++)
         {  iold = iperm[i];
            j1 = ip[iold];
            j2 = j1 + lenr[iold] - 1;
            if (j2 < j1) continue;
            for (jj = j1; jj <= j2; jj++)
            {  j = icn[jj];
               if (j == i)
               {  num++;
                  break;
               }
            }
         }
         if (num != numnz)
            xprintf("Failure in mc21a, numnz = %d instead of %d\n",
               numnz, num);
      }
      return 0;
}

void ranmat(int m, int n, int icn[], int iptr[], int nnnp1, int *knum,
      int iw[])
{     /* subroutine to generate random matrix */
      int i, ii, inum, j, lrow, matnum;
      inum = (*knum / n) * 2;
      if (inum > n-1) inum = n-1;
      matnum = 1;
      /* each pass through this loop generates a row of the matrix */
      for (j = 1; j <= m; j++)
      {  iptr[j] = matnum;
         if (!(sing || j > n))
            icn[matnum++] = j;
         if (n == 1) continue;
         for (i = 1; i <= n; i++) iw[i] = 0;
         if (!sing) iw[j] = 1;
         fa01bs(inum, &lrow);
         lrow--;
         if (lrow == 0) continue;
         /* lrow off-diagonal non-zeros in row j of the matrix */
         for (ii = 1; ii <= lrow; ii++)
         {  for (;;)
            {  fa01bs(n, &i);
               if (iw[i] != 1) break;
            }
            iw[i] = 1;
            icn[matnum++] = i;
         }
      }
      for (i = m+1; i <= nnnp1; i++)
         iptr[i] = matnum;
      *knum = matnum - 1;
      return;
}

double g = 1431655765.0;

double fa01as(int i)
{     /* random number generator */
      g = fmod(g * 9228907.0, 4294967296.0);
      if (i >= 0)
         return g / 4294967296.0;
      else
         return 2.0 * g / 4294967296.0 - 1.0;
}

void fa01bs(int max, int *nrand)
{     *nrand = (int)(fa01as(1) * (double)max) + 1;
      return;
}
#endif

/***********************************************************************
*  btf_store_a_cols - store pattern of matrix A in column-wise format
*
*  This routine stores the pattern (that is, only indices of non-zero
*  elements) of the original matrix A in column-wise format.
*
*  On exit the routine returns the number of non-zeros in matrix A. */

int btf_store_a_cols(BTF *btf, int (*col)(void *info, int j, int ind[],
      double val[]), void *info, int ind[], double val[])
{     int n = btf->n;
      SVA *sva = btf->sva;
      int *sv_ind = sva->ind;
      int ac_ref = btf->ac_ref;
      int *ac_ptr = &sva->ptr[ac_ref-1];
      int *ac_len = &sva->len[ac_ref-1];
      int j, len, ptr, nnz;
      nnz = 0;
      for (j = 1; j <= n; j++)
      {  /* get j-th column */
         len = col(info, j, ind, val);
         xassert(0 <= len && len <= n);
         /* reserve locations for j-th column */
         if (len > 0)
         {  if (sva->r_ptr - sva->m_ptr < len)
            {  sva_more_space(sva, len);
               sv_ind = sva->ind;
            }
            sva_reserve_cap(sva, ac_ref+(j-1), len);
         }
         /* store pattern of j-th column */
         ptr = ac_ptr[j];
         memcpy(&sv_ind[ptr], &ind[1], len * sizeof(int));
         ac_len[j] = len;
         nnz += len;
      }
      return nnz;
}

/***********************************************************************
*  btf_make_blocks - permutations to block triangular form
*
*  This routine analyzes the pattern of the original matrix A and
*  determines permutation matrices P and Q such that A = P * A~* Q,
*  where A~ is an upper block triangular matrix.
*
*  On exit the routine returns symbolic rank of matrix A. */

int btf_make_blocks(BTF *btf)
{     int n = btf->n;
      SVA *sva = btf->sva;
      int *sv_ind = sva->ind;
      int *pp_ind = btf->pp_ind;
      int *pp_inv = btf->pp_inv;
      int *qq_ind = btf->qq_ind;
      int *qq_inv = btf->qq_inv;
      int *beg = btf->beg;
      int ac_ref = btf->ac_ref;
      int *ac_ptr = &sva->ptr[ac_ref-1];
      int *ac_len = &sva->len[ac_ref-1];
      int i, j, rank, *iperm, *pr, *arp, *cv, *out, *ip, *lenr, *lowl,
         *numb, *prev;
      /* determine column permutation matrix M such that matrix A * M
       * has zero-free diagonal */
      iperm = qq_inv; /* matrix M */
      pr  = btf->p1_ind; /* working array */
      arp = btf->p1_inv; /* working array */
      cv  = btf->q1_ind; /* working array */
      out = btf->q1_inv; /* working array */
      rank = mc21a(n, sv_ind, ac_ptr, ac_len, iperm, pr, arp, cv, out);
      xassert(0 <= rank && rank <= n);
      if (rank < n)
      {  /* A is structurally singular (rank is its symbolic rank) */
         goto done;
      }
      /* build pattern of matrix A * M */
      ip   = pp_ind; /* working array */
      lenr = qq_ind; /* working array */
      for (j = 1; j <= n; j++)
      {  ip[j] = ac_ptr[iperm[j]];
         lenr[j] = ac_len[iperm[j]];
      }
      /* determine symmetric permutation matrix S such that matrix
       * S * (A * M) * S' = A~ is upper block triangular */
      lowl = btf->p1_ind; /* working array */
      numb = btf->p1_inv; /* working array */
      prev = btf->q1_ind; /* working array */
      btf->num =
         mc13d(n, sv_ind, ip, lenr, pp_inv, beg, lowl, numb, prev);
      xassert(beg[1] == 1);
      beg[btf->num+1] = n+1;
      /* A * M = S' * A~ * S ==> A = S' * A~ * (S * M') */
      /* determine permutation matrix P = S' */
      for (j = 1; j <= n; j++)
         pp_ind[pp_inv[j]] = j;
      /* determine permutation matrix Q = S * M' = P' * M' */
      for (i = 1; i <= n; i++)
         qq_ind[i] = iperm[pp_inv[i]];
      for (i = 1; i <= n; i++)
         qq_inv[qq_ind[i]] = i;
done: return rank;
}

/***********************************************************************
*  btf_check_blocks - check structure of matrix A~
*
*  This routine checks that structure of upper block triangular matrix
*  A~ is correct.
*
*  NOTE: For testing/debugging only. */

void btf_check_blocks(BTF *btf)
{     int n = btf->n;
      SVA *sva = btf->sva;
      int *sv_ind = sva->ind;
      int *pp_ind = btf->pp_ind;
      int *pp_inv = btf->pp_inv;
      int *qq_ind = btf->qq_ind;
      int *qq_inv = btf->qq_inv;
      int num = btf->num;
      int *beg = btf->beg;
      int ac_ref = btf->ac_ref;
      int *ac_ptr = &sva->ptr[ac_ref-1];
      int *ac_len = &sva->len[ac_ref-1];
      int i, ii, j, jj, k, size, ptr, end, diag;
      xassert(n > 0);
      /* check permutation matrices P and Q */
      for (k = 1; k <= n; k++)
      {  xassert(1 <= pp_ind[k] && pp_ind[k] <= n);
         xassert(pp_inv[pp_ind[k]] == k);
         xassert(1 <= qq_ind[k] && qq_ind[k] <= n);
         xassert(qq_inv[qq_ind[k]] == k);
      }
      /* check that matrix A~ is upper block triangular with non-zero
       * diagonal */
      xassert(1 <= num && num <= n);
      xassert(beg[1] == 1);
      xassert(beg[num+1] == n+1);
      /* walk thru blocks of A~ */
      for (k = 1; k <= num; k++)
      {  /* determine size of k-th block */
         size = beg[k+1] - beg[k];
         xassert(size >= 1);
         /* walk thru columns of k-th block */
         for (jj = beg[k]; jj < beg[k+1]; jj++)
         {  diag = 0;
            /* jj-th column of A~ = j-th column of A */
            j = qq_ind[jj];
            /* walk thru elements of j-th column of A */
            ptr = ac_ptr[j];
            end = ptr + ac_len[j];
            for (; ptr < end; ptr++)
            {  /* determine row index of a[i,j] */
               i = sv_ind[ptr];
               /* i-th row of A = ii-th row of A~ */
               ii = pp_ind[i];
               /* a~[ii,jj] should not be below k-th block */
               xassert(ii < beg[k+1]);
               if (ii == jj)
               {  /* non-zero diagonal element of A~ encountered */
                  diag = 1;
               }
            }
            xassert(diag);
         }
      }
      return;
}

/***********************************************************************
*  btf_build_a_rows - build matrix A in row-wise format
*
*  This routine builds the row-wise representation of matrix A in the
*  right part of SVA using its column-wise representation.
*
*  The working array len should have at least 1+n elements (len[0] is
*  not used). */

void btf_build_a_rows(BTF *btf, int len[/*1+n*/])
{     int n = btf->n;
      SVA *sva = btf->sva;
      int *sv_ind = sva->ind;
      double *sv_val = sva->val;
      int ar_ref = btf->ar_ref;
      int *ar_ptr = &sva->ptr[ar_ref-1];
      int *ar_len = &sva->len[ar_ref-1];
      int ac_ref = btf->ac_ref;
      int *ac_ptr = &sva->ptr[ac_ref-1];
      int *ac_len = &sva->len[ac_ref-1];
      int i, j, end, nnz, ptr, ptr1;
      /* calculate the number of non-zeros in each row of matrix A and
       * the total number of non-zeros */
      nnz = 0;
      for (i = 1; i <= n; i++)
         len[i] = 0;
      for (j = 1; j <= n; j++)
      {  nnz += ac_len[j];
         for (end = (ptr = ac_ptr[j]) + ac_len[j]; ptr < end; ptr++)
            len[sv_ind[ptr]]++;
      }
      /* we need at least nnz free locations in SVA */
      if (sva->r_ptr - sva->m_ptr < nnz)
      {  sva_more_space(sva, nnz);
         sv_ind = sva->ind;
         sv_val = sva->val;
      }
      /* reserve locations for rows of matrix A */
      for (i = 1; i <= n; i++)
      {  if (len[i] > 0)
            sva_reserve_cap(sva, ar_ref-1+i, len[i]);
         ar_len[i] = len[i];
      }
      /* walk thru columns of matrix A and build its rows */
      for (j = 1; j <= n; j++)
      {  for (end = (ptr = ac_ptr[j]) + ac_len[j]; ptr < end; ptr++)
         {  i = sv_ind[ptr];
            sv_ind[ptr1 = ar_ptr[i] + (--len[i])] = j;
            sv_val[ptr1] = sv_val[ptr];
         }
      }
      return;
}

/***********************************************************************
*  btf_a_solve - solve system A * x = b
*
*  This routine solves the system A * x = b, where A is the original
*  matrix.
*
*  On entry the array b should contain elements of the right-hand size
*  vector b in locations b[1], ..., b[n], where n is the order of the
*  matrix A. On exit the array x will contain elements of the solution
*  vector in locations x[1], ..., x[n]. Note that the array b will be
*  clobbered on exit.
*
*  The routine also uses locations [1], ..., [max_size] of two working
*  arrays w1 and w2, where max_size is the maximal size of diagonal
*  blocks in BT-factorization (max_size <= n). */

void btf_a_solve(BTF *btf, double b[/*1+n*/], double x[/*1+n*/],
      double w1[/*1+n*/], double w2[/*1+n*/])
{     SVA *sva = btf->sva;
      int *sv_ind = sva->ind;
      double *sv_val = sva->val;
      int *pp_inv = btf->pp_inv;
      int *qq_ind = btf->qq_ind;
      int num = btf->num;
      int *beg = btf->beg;
      int ac_ref = btf->ac_ref;
      int *ac_ptr = &sva->ptr[ac_ref-1];
      int *ac_len = &sva->len[ac_ref-1];
      double *bb = w1;
      double *xx = w2;
      LUF luf;
      int i, j, jj, k, beg_k, flag;
      double t;
      for (k = num; k >= 1; k--)
      {  /* determine order of diagonal block A~[k,k] */
         luf.n = beg[k+1] - (beg_k = beg[k]);
         if (luf.n == 1)
         {  /* trivial case */
            /* solve system A~[k,k] * X[k] = B[k] */
            t = x[qq_ind[beg_k]] =
               b[pp_inv[beg_k]] / btf->vr_piv[beg_k];
            /* substitute X[k] into other equations */
            if (t != 0.0)
            {  int ptr = ac_ptr[qq_ind[beg_k]];
               int end = ptr + ac_len[qq_ind[beg_k]];
               for (; ptr < end; ptr++)
                  b[sv_ind[ptr]] -= sv_val[ptr] * t;
            }
         }
         else
         {  /* general case */
            /* construct B[k] */
            flag = 0;
            for (i = 1; i <= luf.n; i++)
            {  if ((bb[i] = b[pp_inv[i + (beg_k-1)]]) != 0.0)
                  flag = 1;
            }
            /* solve system A~[k,k] * X[k] = B[k] */
            if (!flag)
            {  /* B[k] = 0, so X[k] = 0 */
               for (j = 1; j <= luf.n; j++)
                  x[qq_ind[j + (beg_k-1)]] = 0.0;
               continue;
            }
            luf.sva = sva;
            luf.fr_ref = btf->fr_ref + (beg_k-1);
            luf.fc_ref = btf->fc_ref + (beg_k-1);
            luf.vr_ref = btf->vr_ref + (beg_k-1);
            luf.vr_piv = btf->vr_piv + (beg_k-1);
            luf.vc_ref = btf->vc_ref + (beg_k-1);
            luf.pp_ind = btf->p1_ind + (beg_k-1);
            luf.pp_inv = btf->p1_inv + (beg_k-1);
            luf.qq_ind = btf->q1_ind + (beg_k-1);
            luf.qq_inv = btf->q1_inv + (beg_k-1);
            luf_f_solve(&luf, bb);
            luf_v_solve(&luf, bb, xx);
            /* store X[k] and substitute it into other equations */
            for (j = 1; j <= luf.n; j++)
            {  jj = j + (beg_k-1);
               t = x[qq_ind[jj]] = xx[j];
               if (t != 0.0)
               {  int ptr = ac_ptr[qq_ind[jj]];
                  int end = ptr + ac_len[qq_ind[jj]];
                  for (; ptr < end; ptr++)
                     b[sv_ind[ptr]] -= sv_val[ptr] * t;
               }
            }
         }
      }
      return;
}

/***********************************************************************
*  btf_at_solve - solve system A'* x = b
*
*  This routine solves the system A'* x = b, where A' is a matrix
*  transposed to the original matrix A.
*
*  On entry the array b should contain elements of the right-hand size
*  vector b in locations b[1], ..., b[n], where n is the order of the
*  matrix A. On exit the array x will contain elements of the solution
*  vector in locations x[1], ..., x[n]. Note that the array b will be
*  clobbered on exit.
*
*  The routine also uses locations [1], ..., [max_size] of two working
*  arrays w1 and w2, where max_size is the maximal size of diagonal
*  blocks in BT-factorization (max_size <= n). */

void btf_at_solve(BTF *btf, double b[/*1+n*/], double x[/*1+n*/],
      double w1[/*1+n*/], double w2[/*1+n*/])
{     SVA *sva = btf->sva;
      int *sv_ind = sva->ind;
      double *sv_val = sva->val;
      int *pp_inv = btf->pp_inv;
      int *qq_ind = btf->qq_ind;
      int num = btf->num;
      int *beg = btf->beg;
      int ar_ref = btf->ar_ref;
      int *ar_ptr = &sva->ptr[ar_ref-1];
      int *ar_len = &sva->len[ar_ref-1];
      double *bb = w1;
      double *xx = w2;
      LUF luf;
      int i, j, jj, k, beg_k, flag;
      double t;
      for (k = 1; k <= num; k++)
      {  /* determine order of diagonal block A~[k,k] */
         luf.n = beg[k+1] - (beg_k = beg[k]);
         if (luf.n == 1)
         {  /* trivial case */
            /* solve system A~'[k,k] * X[k] = B[k] */
            t = x[pp_inv[beg_k]] =
               b[qq_ind[beg_k]] / btf->vr_piv[beg_k];
            /* substitute X[k] into other equations */
            if (t != 0.0)
            {  int ptr = ar_ptr[pp_inv[beg_k]];
               int end = ptr + ar_len[pp_inv[beg_k]];
               for (; ptr < end; ptr++)
                  b[sv_ind[ptr]] -= sv_val[ptr] * t;
            }
         }
         else
         {  /* general case */
            /* construct B[k] */
            flag = 0;
            for (i = 1; i <= luf.n; i++)
            {  if ((bb[i] = b[qq_ind[i + (beg_k-1)]]) != 0.0)
                  flag = 1;
            }
            /* solve system A~'[k,k] * X[k] = B[k] */
            if (!flag)
            {  /* B[k] = 0, so X[k] = 0 */
               for (j = 1; j <= luf.n; j++)
                  x[pp_inv[j + (beg_k-1)]] = 0.0;
               continue;
            }
            luf.sva = sva;
            luf.fr_ref = btf->fr_ref + (beg_k-1);
            luf.fc_ref = btf->fc_ref + (beg_k-1);
            luf.vr_ref = btf->vr_ref + (beg_k-1);
            luf.vr_piv = btf->vr_piv + (beg_k-1);
            luf.vc_ref = btf->vc_ref + (beg_k-1);
            luf.pp_ind = btf->p1_ind + (beg_k-1);
            luf.pp_inv = btf->p1_inv + (beg_k-1);
            luf.qq_ind = btf->q1_ind + (beg_k-1);
            luf.qq_inv = btf->q1_inv + (beg_k-1);
            luf_vt_solve(&luf, bb, xx);
            luf_ft_solve(&luf, xx);
            /* store X[k] and substitute it into other equations */
            for (j = 1; j <= luf.n; j++)
            {  jj = j + (beg_k-1);
               t = x[pp_inv[jj]] = xx[j];
               if (t != 0.0)
               {  int ptr = ar_ptr[pp_inv[jj]];
                  int end = ptr + ar_len[pp_inv[jj]];
                  for (; ptr < end; ptr++)
                     b[sv_ind[ptr]] -= sv_val[ptr] * t;
               }
            }
         }
      }
      return;
}

/***********************************************************************
*  btf_at_solve1 - solve system A'* y = e' to cause growth in y
*
*  This routine is a special version of btf_at_solve. It solves the
*  system A'* y = e' = e + delta e, where A' is a matrix transposed to
*  the original matrix A, e is the specified right-hand side vector,
*  and delta e is a vector of +1 and -1 chosen to cause growth in the
*  solution vector y.
*
*  On entry the array e should contain elements of the right-hand size
*  vector e in locations e[1], ..., e[n], where n is the order of the
*  matrix A. On exit the array y will contain elements of the solution
*  vector in locations y[1], ..., y[n]. Note that the array e will be
*  clobbered on exit.
*
*  The routine also uses locations [1], ..., [max_size] of two working
*  arrays w1 and w2, where max_size is the maximal size of diagonal
*  blocks in BT-factorization (max_size <= n). */

void btf_at_solve1(BTF *btf, double e[/*1+n*/], double y[/*1+n*/],
      double w1[/*1+n*/], double w2[/*1+n*/])
{     SVA *sva = btf->sva;
      int *sv_ind = sva->ind;
      double *sv_val = sva->val;
      int *pp_inv = btf->pp_inv;
      int *qq_ind = btf->qq_ind;
      int num = btf->num;
      int *beg = btf->beg;
      int ar_ref = btf->ar_ref;
      int *ar_ptr = &sva->ptr[ar_ref-1];
      int *ar_len = &sva->len[ar_ref-1];
      double *ee = w1;
      double *yy = w2;
      LUF luf;
      int i, j, jj, k, beg_k, ptr, end;
      double e_k, y_k;
      for (k = 1; k <= num; k++)
      {  /* determine order of diagonal block A~[k,k] */
         luf.n = beg[k+1] - (beg_k = beg[k]);
         if (luf.n == 1)
         {  /* trivial case */
            /* determine E'[k] = E[k] + delta E[k] */
            e_k = e[qq_ind[beg_k]];
            e_k = (e_k >= 0.0 ? e_k + 1.0 : e_k - 1.0);
            /* solve system A~'[k,k] * Y[k] = E[k] */
            y_k = y[pp_inv[beg_k]] = e_k / btf->vr_piv[beg_k];
            /* substitute Y[k] into other equations */
            ptr = ar_ptr[pp_inv[beg_k]];
            end = ptr + ar_len[pp_inv[beg_k]];
            for (; ptr < end; ptr++)
               e[sv_ind[ptr]] -= sv_val[ptr] * y_k;
         }
         else
         {  /* general case */
            /* construct E[k] */
            for (i = 1; i <= luf.n; i++)
               ee[i] = e[qq_ind[i + (beg_k-1)]];
            /* solve system A~'[k,k] * Y[k] = E[k] + delta E[k] */
            luf.sva = sva;
            luf.fr_ref = btf->fr_ref + (beg_k-1);
            luf.fc_ref = btf->fc_ref + (beg_k-1);
            luf.vr_ref = btf->vr_ref + (beg_k-1);
            luf.vr_piv = btf->vr_piv + (beg_k-1);
            luf.vc_ref = btf->vc_ref + (beg_k-1);
            luf.pp_ind = btf->p1_ind + (beg_k-1);
            luf.pp_inv = btf->p1_inv + (beg_k-1);
            luf.qq_ind = btf->q1_ind + (beg_k-1);
            luf.qq_inv = btf->q1_inv + (beg_k-1);
            luf_vt_solve1(&luf, ee, yy);
            luf_ft_solve(&luf, yy);
            /* store Y[k] and substitute it into other equations */
            for (j = 1; j <= luf.n; j++)
            {  jj = j + (beg_k-1);
               y_k = y[pp_inv[jj]] = yy[j];
               ptr = ar_ptr[pp_inv[jj]];
               end = ptr + ar_len[pp_inv[jj]];
               for (; ptr < end; ptr++)
                  e[sv_ind[ptr]] -= sv_val[ptr] * y_k;
            }
         }
      }
      return;
}

/***********************************************************************
*  btf_estimate_norm - estimate 1-norm of inv(A)
*
*  This routine estimates 1-norm of inv(A) by one step of inverse
*  iteration for the small singular vector as described in [1]. This
*  involves solving two systems of equations:
*
*     A'* y = e,
*
*     A * z = y,
*
*  where A' is a matrix transposed to A, and e is a vector of +1 and -1
*  chosen to cause growth in y. Then
*
*     estimate 1-norm of inv(A) = (1-norm of z) / (1-norm of y)
*
*  REFERENCES
*
*  1. G.E.Forsythe, M.A.Malcolm, C.B.Moler. Computer Methods for
*     Mathematical Computations. Prentice-Hall, Englewood Cliffs, N.J.,
*     pp. 30-62 (subroutines DECOMP and SOLVE). */

double btf_estimate_norm(BTF *btf, double w1[/*1+n*/], double
      w2[/*1+n*/], double w3[/*1+n*/], double w4[/*1+n*/])
{     int n = btf->n;
      double *e = w1;
      double *y = w2;
      double *z = w1;
      int i;
      double y_norm, z_norm;
      /* compute y = inv(A') * e to cause growth in y */
      for (i = 1; i <= n; i++)
         e[i] = 0.0;
      btf_at_solve1(btf, e, y, w3, w4);
      /* compute 1-norm of y = sum |y[i]| */
      y_norm = 0.0;
      for (i = 1; i <= n; i++)
         y_norm += (y[i] >= 0.0 ? +y[i] : -y[i]);
      /* compute z = inv(A) * y */
      btf_a_solve(btf, y, z, w3, w4);
      /* compute 1-norm of z = sum |z[i]| */
      z_norm = 0.0;
      for (i = 1; i <= n; i++)
         z_norm += (z[i] >= 0.0 ? +z[i] : -z[i]);
      /* estimate 1-norm of inv(A) = (1-norm of z) / (1-norm of y) */
      return z_norm / y_norm;
}

/***********************************************************************
*  The structure IFU describes dense updatable IFU-factorization.
*
*  The IFU-factorization has the following format:
*
*     A = inv(F) * U,                                                (1)
*
*  where A is a given (unsymmetric) nxn square matrix, F is a square
*  matrix, U is an upper triangular matrix. Obviously, the equality (1)
*  is equivalent to the following equality:
*
*     F * A = U.                                                     (2)
*
*  It is assumed that matrix A is small and dense, so matrices F and U
*  are stored by rows in dense format as follows:
*
*        1         n       n_max      1         n       n_max
*      1 * * * * * * x x x x        1 * * * * * * x x x x
*        * * * * * * x x x x          ? * * * * * x x x x
*        * * * * * * x x x x          ? ? * * * * x x x x
*        * * * * * * x x x x          ? ? ? * * * x x x x
*        * * * * * * x x x x          ? ? ? ? * * x x x x
*      n * * * * * * x x x x        n ? ? ? ? ? * x x x x
*        x x x x x x x x x x          x x x x x x x x x x
*        x x x x x x x x x x          x x x x x x x x x x
*        x x x x x x x x x x          x x x x x x x x x x
*  n_max x x x x x x x x x x    n_max x x x x x x x x x x
*
*             matrix F                     matrix U
*
*  where '*' are matrix elements, '?' are unused locations, 'x' are
*  reserved locations. */

typedef struct IFU IFU;

struct IFU
{     /* IFU-factorization */
      int n_max;
      /* maximal order of matrices A, F, U; n_max >= 1 */
      int n;
      /* current order of matrices A, F, U; 0 <= n <= n_max */
      double *f; /* double f[n_max*n_max]; */
      /* matrix F stored by rows */
      double *u; /* double u[n_max*n_max]; */
      /* matrix U stored by rows */
};

#define ifu_expand _glp_ifu_expand
void ifu_expand(IFU *ifu, double c[/*1+n*/], double r[/*1+n*/],
      double d);
/* expand IFU-factorization */

#define ifu_bg_update _glp_ifu_bg_update
int ifu_bg_update(IFU *ifu, double c[/*1+n*/], double r[/*1+n*/],
      double d);
/* update IFU-factorization (Bartels-Golub) */

#define ifu_gr_update _glp_ifu_gr_update
int ifu_gr_update(IFU *ifu, double c[/*1+n*/], double r[/*1+n*/],
      double d);
/* update IFU-factorization (Givens rotations) */

#define ifu_a_solve _glp_ifu_a_solve
void ifu_a_solve(IFU *ifu, double x[/*1+n*/], double w[/*1+n*/]);
/* solve system A * x = b */

#define ifu_at_solve _glp_ifu_at_solve
void ifu_at_solve(IFU *ifu, double x[/*1+n*/], double w[/*1+n*/]);
/* solve system A'* x = b */


/***********************************************************************
*  ifu_expand - expand IFU-factorization
*
*  This routine expands the IFU-factorization of the matrix A according
*  to the following expansion of A:
*
*             ( A  c )
*     new A = (      )
*             ( r' d )
*
*  where c[1,...,n] is a new column, r[1,...,n] is a new row, and d is
*  a new diagonal element.
*
*  From the main equality F * A = U it follows that:
*
*     ( F  0 ) ( A  c )   ( FA  Fc )   ( U  Fc )
*     (      ) (      ) = (        ) = (       ),
*     ( 0  1 ) ( r' d )   ( r'   d )   ( r'  d )
*
*  thus,
*
*             ( F  0 )           ( U  Fc )
*     new F = (      ),  new U = (       ).
*             ( 0  1 )           ( r'  d )
*
*  Note that the resulting matrix U loses its upper triangular form due
*  to row spike r', which should be eliminated. */

void ifu_expand(IFU *ifu, double c[/*1+n*/], double r[/*1+n*/],
      double d)
{     /* non-optimized version */
      int n_max = ifu->n_max;
      int n = ifu->n;
      double *f_ = ifu->f;
      double *u_ = ifu->u;
      int i, j;
      double t;
#     define f(i,j) f_[(i)*n_max+(j)]
#     define u(i,j) u_[(i)*n_max+(j)]
      xassert(0 <= n && n < n_max);
      /* adjust indexing */
      c++, r++;
      /* set new zero column of matrix F */
      for (i = 0; i < n; i++)
         f(i,n) = 0.0;
      /* set new zero row of matrix F */
      for (j = 0; j < n; j++)
         f(n,j) = 0.0;
      /* set new unity diagonal element of matrix F */
      f(n,n) = 1.0;
      /* set new column of matrix U to vector (old F) * c */
      for (i = 0; i < n; i++)
      {  /* u[i,n] := (i-th row of old F) * c */
         t = 0.0;
         for (j = 0; j < n; j++)
            t += f(i,j) * c[j];
         u(i,n) = t;
      }
      /* set new row of matrix U to vector r */
      for (j = 0; j < n; j++)
         u(n,j) = r[j];
      /* set new diagonal element of matrix U to scalar d */
      u(n,n) = d;
      /* increase factorization order */
      ifu->n++;
#     undef f
#     undef u
      return;
}

/***********************************************************************
*  ifu_bg_update - update IFU-factorization (Bartels-Golub)
*
*  This routine updates IFU-factorization of the matrix A according to
*  its expansion (see comments to the routine ifu_expand). The routine
*  is based on the method proposed by Bartels and Golub [1].
*
*  RETURNS
*
*  0  The factorization has been successfully updated.
*
*  1  On some elimination step diagional element u[k,k] to be used as
*     pivot is too small in magnitude.
*
*  2  Diagonal element u[n,n] is too small in magnitude (at the end of
*     update).
*
*  REFERENCES
*
*  1. R.H.Bartels, G.H.Golub, "The Simplex Method of Linear Programming
*     Using LU-decomposition", Comm. ACM, 12, pp. 266-68, 1969. */

int ifu_bg_update(IFU *ifu, double c[/*1+n*/], double r[/*1+n*/],
      double d)
{     /* non-optimized version */
      int n_max = ifu->n_max;
      int n = ifu->n;
      double *f_ = ifu->f;
      double *u_ = ifu->u;
#if 1 /* FIXME */
      double tol = 1e-5;
#endif
      int j, k;
      double t;
#     define f(i,j) f_[(i)*n_max+(j)]
#     define u(i,j) u_[(i)*n_max+(j)]
      /* expand factorization */
      ifu_expand(ifu, c, r, d);
      /* NOTE: n keeps its old value */
      /* eliminate spike (non-zero subdiagonal elements) in last row of
       * matrix U */
      for (k = 0; k < n; k++)
      {  /* if |u[k,k]| < |u[n,k]|, interchange k-th and n-th rows to
          * provide |u[k,k]| >= |u[n,k]| for numeric stability */
         if (fabs(u(k,k)) < fabs(u(n,k)))
         {  /* interchange k-th and n-th rows of matrix U */
            for (j = k; j <= n; j++)
               t = u(k,j), u(k,j) = u(n,j), u(n,j) = t;
            /* interchange k-th and n-th rows of matrix F to keep the
             * main equality F * A = U */
            for (j = 0; j <= n; j++)
               t = f(k,j), f(k,j) = f(n,j), f(n,j) = t;
         }
         /* now |u[k,k]| >= |u[n,k]| */
         /* check if diagonal element u[k,k] can be used as pivot */
         if (fabs(u(k,k)) < tol)
         {  /* u[k,k] is too small in magnitude */
            return 1;
         }
         /* if u[n,k] = 0, elimination is not needed */
         if (u(n,k) == 0.0)
            continue;
         /* compute gaussian multiplier t = u[n,k] / u[k,k] */
         t = u(n,k) / u(k,k);
         /* apply gaussian transformation to eliminate u[n,k] */
         /* (n-th row of U) := (n-th row of U) - t * (k-th row of U) */
         for (j = k+1; j <= n; j++)
            u(n,j) -= t * u(k,j);
         /* apply the same transformation to matrix F to keep the main
          * equality F * A = U */
         for (j = 0; j <= n; j++)
            f(n,j) -= t * f(k,j);
      }
      /* now matrix U is upper triangular */
      if (fabs(u(n,n)) < tol)
      {  /* u[n,n] is too small in magnitude */
         return 2;
      }
#     undef f
#     undef u
      return 0;
}

/***********************************************************************
*  The routine givens computes the parameters of Givens plane rotation
*  c = cos(teta) and s = sin(teta) such that:
*
*     ( c -s ) ( a )   ( r )
*     (      ) (   ) = (   ) ,
*     ( s  c ) ( b )   ( 0 )
*
*  where a and b are given scalars.
*
*  REFERENCES
*
*  G.H.Golub, C.F.Van Loan, "Matrix Computations", 2nd ed. */

static void givens(double a, double b, double *c, double *s)
{     /* non-optimized version */
      double t;
      if (b == 0.0)
         (*c) = 1.0, (*s) = 0.0;
      else if (fabs(a) <= fabs(b))
         t = - a / b, (*s) = 1.0 / sqrt(1.0 + t * t), (*c) = (*s) * t;
      else
         t = - b / a, (*c) = 1.0 / sqrt(1.0 + t * t), (*s) = (*c) * t;
      return;
}

/***********************************************************************
*  ifu_gr_update - update IFU-factorization (Givens rotations)
*
*  This routine updates IFU-factorization of the matrix A according to
*  its expansion (see comments to the routine ifu_expand). The routine
*  is based on Givens plane rotations [1].
*
*  RETURNS
*
*  0  The factorization has been successfully updated.
*
*  1  On some elimination step both elements u[k,k] and u[n,k] are too
*     small in magnitude.
*
*  2  Diagonal element u[n,n] is too small in magnitude (at the end of
*     update).
*
*  REFERENCES
*
*  1. G.H.Golub, C.F.Van Loan, "Matrix Computations", 2nd ed. */

int ifu_gr_update(IFU *ifu, double c[/*1+n*/], double r[/*1+n*/],
      double d)
{     /* non-optimized version */
      int n_max = ifu->n_max;
      int n = ifu->n;
      double *f_ = ifu->f;
      double *u_ = ifu->u;
#if 1 /* FIXME */
      double tol = 1e-5;
#endif
      int j, k;
      double cs, sn;
#     define f(i,j) f_[(i)*n_max+(j)]
#     define u(i,j) u_[(i)*n_max+(j)]
      /* expand factorization */
      ifu_expand(ifu, c, r, d);
      /* NOTE: n keeps its old value */
      /* eliminate spike (non-zero subdiagonal elements) in last row of
       * matrix U */
      for (k = 0; k < n; k++)
      {  /* check if elements u[k,k] and u[n,k] are eligible */
         if (fabs(u(k,k)) < tol && fabs(u(n,k)) < tol)
         {  /* both u[k,k] and u[n,k] are too small in magnitude */
            return 1;
         }
         /* if u[n,k] = 0, elimination is not needed */
         if (u(n,k) == 0.0)
            continue;
         /* compute parameters of Givens plane rotation */
         givens(u(k,k), u(n,k), &cs, &sn);
         /* apply Givens rotation to k-th and n-th rows of matrix U to
          * eliminate u[n,k] */
         for (j = k; j <= n; j++)
         {  double ukj = u(k,j), unj = u(n,j);
            u(k,j) = cs * ukj - sn * unj;
            u(n,j) = sn * ukj + cs * unj;
         }
         /* apply the same transformation to matrix F to keep the main
          * equality F * A = U */
         for (j = 0; j <= n; j++)
         {  double fkj = f(k,j), fnj = f(n,j);
            f(k,j) = cs * fkj - sn * fnj;
            f(n,j) = sn * fkj + cs * fnj;
         }
      }
      /* now matrix U is upper triangular */
      if (fabs(u(n,n)) < tol)
      {  /* u[n,n] is too small in magnitude */
         return 2;
      }
#     undef f
#     undef u
      return 0;
}

/***********************************************************************
*  ifu_a_solve - solve system A * x = b
*
*  This routine solves the system A * x = b, where the matrix A is
*  specified by its IFU-factorization.
*
*  Using the main equality F * A = U we have:
*
*     A * x = b  =>  F * A * x = F * b  =>  U * x = F * b  =>
*
*     x = inv(U) * F * b.
*
*  On entry the array x should contain elements of the right-hand side
*  vector b in locations x[1], ..., x[n], where n is the order of the
*  matrix A. On exit this array will contain elements of the solution
*  vector x in the same locations.
*
*  The working array w should have at least 1+n elements (0-th element
*  is not used). */

void ifu_a_solve(IFU *ifu, double x[/*1+n*/], double w[/*1+n*/])
{     /* non-optimized version */
      int n_max = ifu->n_max;
      int n = ifu->n;
      double *f_ = ifu->f;
      double *u_ = ifu->u;
      int i, j;
      double t;
#     define f(i,j) f_[(i)*n_max+(j)]
#     define u(i,j) u_[(i)*n_max+(j)]
      xassert(0 <= n && n <= n_max);
      /* adjust indexing */
      x++, w++;
      /* y := F * b */
      memcpy(w, x, n * sizeof(double));
      for (i = 0; i < n; i++)
      {  /* y[i] := (i-th row of F) * b */
         t = 0.0;
         for (j = 0; j < n; j++)
            t += f(i,j) * w[j];
         x[i] = t;
      }
      /* x := inv(U) * y */
      for (i = n-1; i >= 0; i--)
      {  t = x[i];
         for (j = i+1; j < n; j++)
            t -= u(i,j) * x[j];
         x[i] = t / u(i,i);
      }
#     undef f
#     undef u
      return;
}

/***********************************************************************
*  ifu_at_solve - solve system A'* x = b
*
*  This routine solves the system A'* x = b, where A' is a matrix
*  transposed to the matrix A, specified by its IFU-factorization.
*
*  Using the main equality F * A = U, from which it follows that
*  A'* F' = U', we have:
*
*     A'* x = b  =>  A'* F'* inv(F') * x = b  =>
*
*     U'* inv(F') * x = b  =>  inv(F') * x = inv(U') * b  =>
*
*     x = F' * inv(U') * b.
*
*  On entry the array x should contain elements of the right-hand side
*  vector b in locations x[1], ..., x[n], where n is the order of the
*  matrix A. On exit this array will contain elements of the solution
*  vector x in the same locations.
*
*  The working array w should have at least 1+n elements (0-th element
*  is not used). */

void ifu_at_solve(IFU *ifu, double x[/*1+n*/], double w[/*1+n*/])
{     /* non-optimized version */
      int n_max = ifu->n_max;
      int n = ifu->n;
      double *f_ = ifu->f;
      double *u_ = ifu->u;
      int i, j;
      double t;
#     define f(i,j) f_[(i)*n_max+(j)]
#     define u(i,j) u_[(i)*n_max+(j)]
      xassert(0 <= n && n <= n_max);
      /* adjust indexing */
      x++, w++;
      /* y := inv(U') * b */
      for (i = 0; i < n; i++)
      {  t = (x[i] /= u(i,i));
         for (j = i+1; j < n; j++)
            x[j] -= u(i,j) * t;
      }
      /* x := F'* y */
      for (j = 0; j < n; j++)
      {  /* x[j] := (j-th column of F) * y */
         t = 0.0;
         for (i = 0; i < n; i++)
            t += f(i,j) * x[i];
         w[j] = t;
      }
      memcpy(x, w, n * sizeof(double));
#     undef f
#     undef u
      return;
}


/***********************************************************************
*  The structure SCF describes sparse updatable factorization based on
*  Schur complement.
*
*  The SCF-factorization has the following format:
*
*     ( A   A1~ )     ( A0  A1 )       ( R0    ) ( S0  S )
*     (         ) = P (        ) Q = P (       ) (       ) Q,        (1)
*     ( A2~ A3~ )     ( A2  A3 )       ( R   I ) (     C )
*
*  where:
*
*  A is current (unsymmetric) square matrix (not stored);
*
*  A1~, A2~, A3~ are some additional matrices (not stored);
*
*  A0 is initial (unsymmetric) square matrix (not stored);
*
*  A1, A2, A3 are some additional matrices (not stored);
*
*  R0 and S0 are matrices that define factorization of the initial
*  matrix A0 = R0 * S0 (stored in an invertable form);
*
*  R is a matrix defined from R * S0 = A2, so R = A2 * inv(S0) (stored
*  in row-wise sparse format);
*
*  S is a matrix defined from R0 * S = A1, so S = inv(R0) * A1 (stored
*  in column-wise sparse format);
*
*  C is Schur complement (to matrix A0) defined from R * S + C = A3,
*  so C = A3 - R * S = A3 - A2 * inv(A0) * A1 (stored in an invertable
*  form).
*
*  P, Q are permutation matrices (stored in both row- and column-like
*  formats). */

typedef struct SCF SCF;

struct SCF
{     /* Schur-complement-based factorization */
      int n;
      /* order of current matrix A */
      /*--------------------------------------------------------------*/
      /* initial matrix A0 = R0 * S0 of order n0 in invertable form */
      int n0;
      /* order of matrix A0 */
      int type;
      /* type of factorization used:
       * 1 - LU-factorization (R0 = F0, S0 = V0)
       * 2 - BT-factorization (R0 = I, S0 = A0) */
      union
      {  LUF *luf; /* type = 1 */
         BTF *btf; /* type = 2 */
      }  a0;
      /* factorization of matrix A0 */
      /*--------------------------------------------------------------*/
      /* augmented matrix (A0, A1; A2, A3) of order n0+nn */
      int nn_max;
      /* maximal number of additional rows and columns in the augmented
       * matrix (this limits the number of updates) */
      int nn;
      /* current number of additional rows and columns in the augmented
       * matrix, 0 <= nn <= nn_max */
      SVA *sva;
      /* associated sparse vector area (SVA) used to store rows of
       * matrix R and columns of matrix S */
      /*--------------------------------------------------------------*/
      /* nn*n0-matrix R in row-wise format */
      int rr_ref;
      /* reference number of sparse vector in SVA, which is the first
       * row of matrix R */
#if 0 + 0
      int *rr_ptr = &sva->ptr[rr_ref-1];
      /* rr_ptr[0] is not used;
       * rr_ptr[i], 1 <= i <= nn, is pointer to i-th row in SVA;
       * rr_ptr[nn+1,...,nn_max] are reserved locations */
      int *rr_len = &sva->len[rr_ref-1];
      /* rr_len[0] is not used;
       * rr_len[i], 1 <= i <= nn, is length of i-th row;
       * rr_len[nn+1,...,nn_max] are reserved locations */
#endif
      /*--------------------------------------------------------------*/
      /* n0*nn-matrix S in column-wise format */
      int ss_ref;
      /* reference number of sparse vector in SVA, which is the first
       * column of matrix S */
#if 0 + 0
      int *ss_ptr = &sva->ptr[ss_ref-1];
      /* ss_ptr[0] is not used;
       * ss_ptr[j], 1 <= j <= nn, is pointer to j-th column in SVA;
       * ss_ptr[nn+1,...,nn_max] are reserved locations */
      int *ss_len = &sva->len[ss_ref-1];
      /* ss_len[0] is not used;
       * ss_len[j], 1 <= j <= nn, is length of j-th column;
       * ss_len[nn+1,...,nn_max] are reserved locations */
#endif
      /*--------------------------------------------------------------*/
      /* Schur complement C of order nn in invertable form */
      IFU ifu;
      /* IFU-factorization of matrix C */
      /*--------------------------------------------------------------*/
      /* permutation matrix P of order n0+nn */
      int *pp_ind; /* int pp_ind[1+n0+nn_max]; */
      /* pp_ind[i] = j means that P[i,j] = 1 */
      int *pp_inv; /* int pp_inv[1+n0+nn_max]; */
      /* pp_inv[j] = i means that P[i,j] = 1 */
      /*--------------------------------------------------------------*/
      /* permutation matrix Q of order n0+nn */
      int *qq_ind; /* int qq_ind[1+n0+nn_max]; */
      /* qq_ind[i] = j means that Q[i,j] = 1 */
      int *qq_inv; /* int qq_inv[1+n0+nn_max]; */
      /* qq_inv[j] = i means that Q[i,j] = 1 */
};

#define scf_swap_q_cols(j1, j2) \
      do \
      {  int i1, i2; \
         i1 = qq_inv[j1], i2 = qq_inv[j2]; \
         qq_ind[i1] = j2, qq_inv[j2] = i1; \
         qq_ind[i2] = j1, qq_inv[j1] = i2; \
      }  while (0)
/* swap columns j1 and j2 of permutation matrix Q */

#define scf_r0_solve _glp_scf_r0_solve
void scf_r0_solve(SCF *scf, int tr, double x[/*1+n0*/]);
/* solve system R0 * x = b or R0'* x = b */

#define scf_s0_solve _glp_scf_s0_solve
void scf_s0_solve(SCF *scf, int tr, double x[/*1+n0*/],
      double w1[/*1+n0*/], double w2[/*1+n0*/], double w3[/*1+n0*/]);
/* solve system S0 * x = b or S0'* x = b */

#define scf_r_prod _glp_scf_r_prod
void scf_r_prod(SCF *scf, double y[/*1+nn*/], double a, const double
      x[/*1+n0*/]);
/* compute product y := y + alpha * R * x */

#define scf_rt_prod _glp_scf_rt_prod
void scf_rt_prod(SCF *scf, double y[/*1+n0*/], double a, const double
      x[/*1+nn*/]);
/* compute product y := y + alpha * R'* x */

#define scf_s_prod _glp_scf_s_prod
void scf_s_prod(SCF *scf, double y[/*1+n0*/], double a, const double
      x[/*1+nn*/]);
/* compute product y := y + alpha * S * x */

#define scf_st_prod _glp_scf_st_prod
void scf_st_prod(SCF *scf, double y[/*1+nn*/], double a, const double
      x[/*1+n0*/]);
/* compute product y := y + alpha * S'* x */

#define scf_a_solve _glp_scf_a_solve
void scf_a_solve(SCF *scf, double x[/*1+n*/],
      double w[/*1+n0+nn*/], double work1[/*1+max(n0,nn)*/],
      double work2[/*1+n*/], double work3[/*1+n*/]);
/* solve system A * x = b */

#define scf_at_solve _glp_scf_at_solve
void scf_at_solve(SCF *scf, double x[/*1+n*/],
      double w[/*1+n0+nn*/], double work1[/*1+max(n0,nn)*/],
      double work2[/*1+n*/], double work3[/*1+n*/]);
/* solve system A'* x = b */

#define scf_add_r_row _glp_scf_add_r_row
void scf_add_r_row(SCF *scf, const double w[/*1+n0*/]);
/* add new row to matrix R */

#define scf_add_s_col _glp_scf_add_s_col
void scf_add_s_col(SCF *scf, const double v[/*1+n0*/]);
/* add new column to matrix S */

#define scf_update_aug _glp_scf_update_aug
int scf_update_aug(SCF *scf, double b[/*1+n0*/], double d[/*1+n0*/],
      double f[/*1+nn*/], double g[/*1+nn*/], double h, int upd,
      double w1[/*1+n0*/], double w2[/*1+n0*/], double w3[/*1+n0*/]);
/* update factorization of augmented matrix */




typedef struct BTFINT BTFINT;

struct BTFINT
{     /* interface to BT-factorization */
      int n_max;
      /* maximal value of n (increased automatically) */
      int valid;
      /* factorization is valid only if this flag is set */
      SVA *sva;
      /* sparse vector area (SVA) */
      BTF *btf;
      /* sparse block triangular LU-factorization */
      SGF *sgf;
      /* sparse Gaussian factorizer workspace */
      /*--------------------------------------------------------------*/
      /* control parameters */
      int sva_n_max, sva_size;
      /* parameters passed to sva_create_area */
      int delta_n0, delta_n;
      /* if n_max = 0, set n_max = n + delta_n0
       * if n_max < n, set n_max = n + delta_n */
      double sgf_piv_tol;
      int sgf_piv_lim;
      int sgf_suhl;
      double sgf_eps_tol;
      /* factorizer control parameters */
};

#define btfint_create _glp_btfint_create
BTFINT *btfint_create(void);
/* create interface to BT-factorization */

#define btfint_factorize _glp_btfint_factorize
int btfint_factorize(BTFINT *fi, int n, int (*col)(void *info, int j,
      int ind[], double val[]), void *info);
/* compute BT-factorization of specified matrix A */

#define btfint_delete _glp_btfint_delete
void btfint_delete(BTFINT *fi);
/* delete interface to BT-factorization */

BTFINT *btfint_create(void)
{     /* create interface to BT-factorization */
      BTFINT *fi;
      fi = talloc(1, BTFINT);
      fi->n_max = 0;
      fi->valid = 0;
      fi->sva = NULL;
      fi->btf = NULL;
      fi->sgf = NULL;
      fi->sva_n_max = fi->sva_size = 0;
      fi->delta_n0 = fi->delta_n = 0;
      fi->sgf_piv_tol = 0.10;
      fi->sgf_piv_lim = 4;
      fi->sgf_suhl = 1;
      fi->sgf_eps_tol = DBL_EPSILON;
      return fi;
}

static void factorize_triv(BTFINT *fi, int k, int (*col)(void *info,
      int j, int ind[], double val[]), void *info)
{     /* compute LU-factorization of diagonal block A~[k,k] and store
       * corresponding columns of matrix A except elements of A~[k,k]
       * (trivial case when the block has unity size) */
      SVA *sva = fi->sva;
      int *sv_ind = sva->ind;
      double *sv_val = sva->val;
      BTF *btf = fi->btf;
      int *pp_inv = btf->pp_inv;
      int *qq_ind = btf->qq_ind;
      int *beg = btf->beg;
      int ac_ref = btf->ac_ref;
      int *ac_ptr = &sva->ptr[ac_ref-1];
      int *ac_len = &sva->len[ac_ref-1];
      SGF *sgf = fi->sgf;
      int *ind = (int *)sgf->vr_max; /* working array */
      double *val = sgf->work; /* working array */
      int i, j, t, len, ptr, beg_k;
      /* diagonal block A~[k,k] has the only element in matrix A~,
       * which is a~[beg[k],beg[k]] = a[i,j] */
      beg_k = beg[k];
      i = pp_inv[beg_k];
      j = qq_ind[beg_k];
      /* get j-th column of A */
      len = col(info, j, ind, val);
      /* find element a[i,j] = a~[beg[k],beg[k]] in j-th column */
      for (t = 1; t <= len; t++)
      {  if (ind[t] == i)
            break;
      }
      xassert(t <= len);
      /* compute LU-factorization of diagonal block A~[k,k], where
       * F = (1), V = (a[i,j]), P = Q = (1) (see the module LUF) */
#if 1 /* FIXME */
      xassert(val[t] != 0.0);
#endif
      btf->vr_piv[beg_k] = val[t];
      btf->p1_ind[beg_k] = btf->p1_inv[beg_k] = 1;
      btf->q1_ind[beg_k] = btf->q1_inv[beg_k] = 1;
      /* remove element a[i,j] = a~[beg[k],beg[k]] from j-th column */
      memmove(&ind[t], &ind[t+1], (len-t) * sizeof(int));
      memmove(&val[t], &val[t+1], (len-t) * sizeof(double));
      len--;
      /* and store resulting j-th column of A into BTF */
      if (len > 0)
      {  /* reserve locations for j-th column of A */
         if (sva->r_ptr - sva->m_ptr < len)
         {  sva_more_space(sva, len);
            sv_ind = sva->ind;
            sv_val = sva->val;
         }
         sva_reserve_cap(sva, ac_ref+(j-1), len);
         /* store j-th column of A (except elements of A~[k,k]) */
         ptr = ac_ptr[j];
         memcpy(&sv_ind[ptr], &ind[1], len * sizeof(int));
         memcpy(&sv_val[ptr], &val[1], len * sizeof(double));
         ac_len[j] = len;
      }
      return;
}

static int factorize_block(BTFINT *fi, int k, int (*col)(void *info,
      int j, int ind[], double val[]), void *info)
{     /* compute LU-factorization of diagonal block A~[k,k] and store
       * corresponding columns of matrix A except elements of A~[k,k]
       * (general case) */
      SVA *sva = fi->sva;
      int *sv_ind = sva->ind;
      double *sv_val = sva->val;
      BTF *btf = fi->btf;
      int *pp_ind = btf->pp_ind;
      int *qq_ind = btf->qq_ind;
      int *beg = btf->beg;
      int ac_ref = btf->ac_ref;
      int *ac_ptr = &sva->ptr[ac_ref-1];
      int *ac_len = &sva->len[ac_ref-1];
      SGF *sgf = fi->sgf;
      int *ind = (int *)sgf->vr_max; /* working array */
      double *val = sgf->work; /* working array */
      LUF luf;
      int *vc_ptr, *vc_len, *vc_cap;
      int i, ii, j, jj, t, len, cnt, ptr, beg_k;
      /* construct fake LUF for LU-factorization of A~[k,k] */
      sgf->luf = &luf;
      luf.n = beg[k+1] - (beg_k = beg[k]);
      luf.sva = sva;
      luf.fr_ref = btf->fr_ref + (beg_k-1);
      luf.fc_ref = btf->fc_ref + (beg_k-1);
      luf.vr_ref = btf->vr_ref + (beg_k-1);
      luf.vr_piv = btf->vr_piv + (beg_k-1);
      luf.vc_ref = btf->vc_ref + (beg_k-1);
      luf.pp_ind = btf->p1_ind + (beg_k-1);
      luf.pp_inv = btf->p1_inv + (beg_k-1);
      luf.qq_ind = btf->q1_ind + (beg_k-1);
      luf.qq_inv = btf->q1_inv + (beg_k-1);
      /* process columns of k-th block of matrix A~ */
      vc_ptr = &sva->ptr[luf.vc_ref-1];
      vc_len = &sva->len[luf.vc_ref-1];
      vc_cap = &sva->cap[luf.vc_ref-1];
      for (jj = 1; jj <= luf.n; jj++)
      {  /* jj-th column of A~ = j-th column of A */
         j = qq_ind[jj + (beg_k-1)];
         /* get j-th column of A */
         len = col(info, j, ind, val);
         /* move elements of diagonal block A~[k,k] to the beginning of
          * the column list */
         cnt = 0;
         for (t = 1; t <= len; t++)
         {  /* i = row index of element a[i,j] */
            i = ind[t];
            /* i-th row of A = ii-th row of A~ */
            ii = pp_ind[i];
            if (ii >= beg_k)
            {  /* a~[ii,jj] = a[i,j] is in diagonal block A~[k,k] */
               double temp;
               cnt++;
               ind[t] = ind[cnt];
               ind[cnt] = ii - (beg_k-1); /* local index */
               temp = val[t], val[t] = val[cnt], val[cnt] = temp;
            }
         }
         /* first cnt elements in the column list give jj-th column of
          * diagonal block A~[k,k], which is initial matrix V in LUF */
         /* enlarge capacity of jj-th column of V = A~[k,k] */
         if (vc_cap[jj] < cnt)
         {  if (sva->r_ptr - sva->m_ptr < cnt)
            {  sva_more_space(sva, cnt);
               sv_ind = sva->ind;
               sv_val = sva->val;
            }
            sva_enlarge_cap(sva, luf.vc_ref+(jj-1), cnt, 0);
         }
         /* store jj-th column of V = A~[k,k] */
         ptr = vc_ptr[jj];
         memcpy(&sv_ind[ptr], &ind[1], cnt * sizeof(int));
         memcpy(&sv_val[ptr], &val[1], cnt * sizeof(double));
         vc_len[jj] = cnt;
         /* other (len-cnt) elements in the column list are stored in
          * j-th column of the original matrix A */
         len -= cnt;
         if (len > 0)
         {  /* reserve locations for j-th column of A */
            if (sva->r_ptr - sva->m_ptr < len)
            {  sva_more_space(sva, len);
               sv_ind = sva->ind;
               sv_val = sva->val;
            }
            sva_reserve_cap(sva, ac_ref-1+j, len);
            /* store j-th column of A (except elements of A~[k,k]) */
            ptr = ac_ptr[j];
            memcpy(&sv_ind[ptr], &ind[cnt+1], len * sizeof(int));
            memcpy(&sv_val[ptr], &val[cnt+1], len * sizeof(double));
            ac_len[j] = len;
         }
      }
      /* compute LU-factorization of diagonal block A~[k,k]; may note
       * that A~[k,k] is irreducible (strongly connected), so singleton
       * phase will have no effect */
      k = sgf_factorize(sgf, 0 /* disable singleton phase */);
      /* now left (dynamic) part of SVA should be empty (wichtig!) */
      xassert(sva->m_ptr == 1);
      return k;
}

int btfint_factorize(BTFINT *fi, int n, int (*col)(void *info, int j,
      int ind[], double val[]), void *info)
{     /* compute BT-factorization of specified matrix A */
      SVA *sva;
      BTF *btf;
      SGF *sgf;
      int k, rank;
      xassert(n > 0);
      fi->valid = 0;
      /* create sparse vector area (SVA), if necessary */
      sva = fi->sva;
      if (sva == NULL)
      {  int sva_n_max = fi->sva_n_max;
         int sva_size = fi->sva_size;
         if (sva_n_max == 0)
            sva_n_max = 6 * n;
         if (sva_size == 0)
            sva_size = 10 * n;
         sva = fi->sva = sva_create_area(sva_n_max, sva_size);
      }
      /* allocate/reallocate underlying objects, if necessary */
      if (fi->n_max < n)
      {  int n_max = fi->n_max;
         if (n_max == 0)
            n_max = fi->n_max = n + fi->delta_n0;
         else
            n_max = fi->n_max = n + fi->delta_n;
         xassert(n_max >= n);
         /* allocate/reallocate block triangular factorization (BTF) */
         btf = fi->btf;
         if (btf == NULL)
         {  btf = fi->btf = talloc(1, BTF);
            memset(btf, 0, sizeof(BTF));
            btf->sva = sva;
         }
         else
         {  tfree(btf->pp_ind);
            tfree(btf->pp_inv);
            tfree(btf->qq_ind);
            tfree(btf->qq_inv);
            tfree(btf->beg);
            tfree(btf->vr_piv);
            tfree(btf->p1_ind);
            tfree(btf->p1_inv);
            tfree(btf->q1_ind);
            tfree(btf->q1_inv);
         }
         btf->pp_ind = talloc(1+n_max, int);
         btf->pp_inv = talloc(1+n_max, int);
         btf->qq_ind = talloc(1+n_max, int);
         btf->qq_inv = talloc(1+n_max, int);
         btf->beg = talloc(1+n_max+1, int);
         btf->vr_piv = talloc(1+n_max, double);
         btf->p1_ind = talloc(1+n_max, int);
         btf->p1_inv = talloc(1+n_max, int);
         btf->q1_ind = talloc(1+n_max, int);
         btf->q1_inv = talloc(1+n_max, int);
         /* allocate/reallocate factorizer workspace (SGF) */
         /* (note that for SGF we could use the size of largest block
          * rather than n_max) */
         sgf = fi->sgf;
         sgf = fi->sgf;
         if (sgf == NULL)
         {  sgf = fi->sgf = talloc(1, SGF);
            memset(sgf, 0, sizeof(SGF));
         }
         else
         {  tfree(sgf->rs_head);
            tfree(sgf->rs_prev);
            tfree(sgf->rs_next);
            tfree(sgf->cs_head);
            tfree(sgf->cs_prev);
            tfree(sgf->cs_next);
            tfree(sgf->vr_max);
            tfree(sgf->flag);
            tfree(sgf->work);
         }
         sgf->rs_head = talloc(1+n_max, int);
         sgf->rs_prev = talloc(1+n_max, int);
         sgf->rs_next = talloc(1+n_max, int);
         sgf->cs_head = talloc(1+n_max, int);
         sgf->cs_prev = talloc(1+n_max, int);
         sgf->cs_next = talloc(1+n_max, int);
         sgf->vr_max = talloc(1+n_max, double);
         sgf->flag = talloc(1+n_max, char);
         sgf->work = talloc(1+n_max, double);
      }
      btf = fi->btf;
      btf->n = n;
      sgf = fi->sgf;
#if 1 /* FIXME */
      /* initialize SVA */
      sva->n = 0;
      sva->m_ptr = 1;
      sva->r_ptr = sva->size + 1;
      sva->head = sva->tail = 0;
#endif
      /* store pattern of original matrix A in column-wise format */
      btf->ac_ref = sva_alloc_vecs(btf->sva, btf->n);
      btf_store_a_cols(btf, col, info, btf->pp_ind, btf->vr_piv);
#ifdef GLP_DEBUG
      sva_check_area(sva);
#endif
      /* analyze pattern of original matrix A and determine permutation
       * matrices P and Q such that A = P * A~* Q, where A~ is an upper
       * block triangular matrix */
      rank = btf_make_blocks(btf);
      if (rank != n)
      {  /* original matrix A is structurally singular */
         return 1;
      }
#ifdef GLP_DEBUG
      btf_check_blocks(btf);
#endif
#if 1 /* FIXME */
      /* initialize SVA */
      sva->n = 0;
      sva->m_ptr = 1;
      sva->r_ptr = sva->size + 1;
      sva->head = sva->tail = 0;
#endif
      /* allocate sparse vectors in SVA */
      btf->ar_ref = sva_alloc_vecs(btf->sva, btf->n);
      btf->ac_ref = sva_alloc_vecs(btf->sva, btf->n);
      btf->fr_ref = sva_alloc_vecs(btf->sva, btf->n);
      btf->fc_ref = sva_alloc_vecs(btf->sva, btf->n);
      btf->vr_ref = sva_alloc_vecs(btf->sva, btf->n);
      btf->vc_ref = sva_alloc_vecs(btf->sva, btf->n);
      /* setup factorizer control parameters */
      sgf->updat = 0; /* wichtig! */
      sgf->piv_tol = fi->sgf_piv_tol;
      sgf->piv_lim = fi->sgf_piv_lim;
      sgf->suhl = fi->sgf_suhl;
      sgf->eps_tol = fi->sgf_eps_tol;
      /* compute LU-factorizations of diagonal blocks A~[k,k] and also
       * store corresponding columns of matrix A except elements of all
       * blocks A~[k,k] */
      for (k = 1; k <= btf->num; k++)
      {  if (btf->beg[k+1] - btf->beg[k] == 1)
         {  /* trivial case (A~[k,k] has unity order) */
            factorize_triv(fi, k, col, info);
         }
         else
         {  /* general case */
            if (factorize_block(fi, k, col, info) != 0)
               return 2; /* factorization of A~[k,k] failed */
         }
      }
#ifdef GLP_DEBUG
      sva_check_area(sva);
#endif
      /* build row-wise representation of matrix A */
      btf_build_a_rows(fi->btf, fi->sgf->rs_head);
#ifdef GLP_DEBUG
      sva_check_area(sva);
#endif
      /* BT-factorization has been successfully computed */
      fi->valid = 1;
      return 0;
}

void btfint_delete(BTFINT *fi)
{     /* delete interface to BT-factorization */
      SVA *sva = fi->sva;
      BTF *btf = fi->btf;
      SGF *sgf = fi->sgf;
      if (sva != NULL)
         sva_delete_area(sva);
      if (btf != NULL)
      {  tfree(btf->pp_ind);
         tfree(btf->pp_inv);
         tfree(btf->qq_ind);
         tfree(btf->qq_inv);
         tfree(btf->beg);
         tfree(btf->vr_piv);
         tfree(btf->p1_ind);
         tfree(btf->p1_inv);
         tfree(btf->q1_ind);
         tfree(btf->q1_inv);
         tfree(btf);
      }
      if (sgf != NULL)
      {  tfree(sgf->rs_head);
         tfree(sgf->rs_prev);
         tfree(sgf->rs_next);
         tfree(sgf->cs_head);
         tfree(sgf->cs_prev);
         tfree(sgf->cs_next);
         tfree(sgf->vr_max);
         tfree(sgf->flag);
         tfree(sgf->work);
         tfree(sgf);
      }
      tfree(fi);
      return;
}




typedef struct SCFINT SCFINT;

struct SCFINT
{     /* interface to SC-factorization */
      int valid;
      /* factorization is valid only if this flag is set */
      SCF scf;
      /* Schur-complement based factorization */
      union
      {  LUFINT *lufi; /* scf.type = 1 */
         BTFINT *btfi; /* scf.type = 2 */
      }  u;
      /* interface to factorize initial matrix A0 */
      /*--------------------------------------------------------------*/
      /* working arrays */
      double *w1; /* double w1[1+n0_max]; */
      double *w2; /* double w2[1+n0_max]; */
      double *w3; /* double w3[1+n0_max]; */
      double *w4; /* double w4[1+n0_max+nn_max]; */
      double *w5; /* double w5[1+n0_max+nn_max]; */
      /*--------------------------------------------------------------*/
      /* control parameters */
      int nn_max;
      /* required maximal number of updates */
};

//#define scfint_create _glp_scfint_create
SCFINT *scfint_create(int type);
/* create interface to SC-factorization */

//#define scfint_factorize _glp_scfint_factorize
int scfint_factorize(SCFINT *fi, int n, int (*col)(void *info, int j,
      int ind[], double val[]), void *info);
/* compute SC-factorization of specified matrix A */

//#define scfint_update _glp_scfint_update
int scfint_update(SCFINT *fi, int upd, int j, int len, const int ind[],
      const double val[]);
/* update SC-factorization after replacing j-th column of A */

//#define scfint_ftran _glp_scfint_ftran
void scfint_ftran(SCFINT *fi, double x[]);
/* solve system A * x = b */

//#define scfint_btran _glp_scfint_btran
void scfint_btran(SCFINT *fi, double x[]);
/* solve system A'* x = b */

//#define scfint_estimate _glp_scfint_estimate
double scfint_estimate(SCFINT *fi);
/* estimate 1-norm of inv(A) */

//#define scfint_delete _glp_scfint_delete
void scfint_delete(SCFINT *fi);
/* delete interface to SC-factorization */

SCFINT *scfint_create(int type)
{     /* create interface to SC-factorization */
      SCFINT *fi;
      fi = talloc(1, SCFINT);
      memset(fi, 0, sizeof(SCFINT));
      switch ((fi->scf.type = type))
      {  case 1:
            fi->u.lufi = lufint_create();
            break;
         case 2:
            fi->u.btfi = btfint_create();
            break;
         default:
            xassert(type != type);
      }
      return fi;
}

int scfint_factorize(SCFINT *fi, int n, int (*col)(void *info, int j,
      int ind[], double val[]), void *info)
{     /* compute SC-factorization of specified matrix A */
      int nn_max, old_n0_max, n0_max, k, ret;
      xassert(n > 0);
      fi->valid = 0;
      /* get required value of nn_max */
      nn_max = fi->nn_max;
      if (nn_max == 0)
         nn_max = 100;
      xassert(nn_max > 0);
      /* compute factorization of specified matrix A */
      switch (fi->scf.type)
      {  case 1:
            old_n0_max = fi->u.lufi->n_max;
            fi->u.lufi->sva_n_max = 4 * n + 2 * nn_max;
            ret = lufint_factorize(fi->u.lufi, n, col, info);
            n0_max = fi->u.lufi->n_max;
            fi->scf.sva = fi->u.lufi->sva;
            fi->scf.a0.luf = fi->u.lufi->luf;
            break;
         case 2:
            old_n0_max = fi->u.btfi->n_max;
            fi->u.btfi->sva_n_max = 6 * n + 2 * nn_max;
            ret = btfint_factorize(fi->u.btfi, n, col, info);
            n0_max = fi->u.btfi->n_max;
            fi->scf.sva = fi->u.btfi->sva;
            fi->scf.a0.btf = fi->u.btfi->btf;
            break;
         default:
            xassert(fi != fi);
      }
      /* allocate/reallocate arrays, if necessary */
      if (old_n0_max < n0_max)
      {  if (fi->w1 != NULL)
            tfree(fi->w1);
         if (fi->w2 != NULL)
            tfree(fi->w2);
         if (fi->w3 != NULL)
            tfree(fi->w3);
         fi->w1 = talloc(1+n0_max, double);
         fi->w2 = talloc(1+n0_max, double);
         fi->w3 = talloc(1+n0_max, double);
      }
      if (fi->scf.nn_max != nn_max)
      {  if (fi->scf.ifu.f != NULL)
            tfree(fi->scf.ifu.f);
         if (fi->scf.ifu.u != NULL)
            tfree(fi->scf.ifu.u);
         fi->scf.ifu.f = talloc(nn_max * nn_max, double);
         fi->scf.ifu.u = talloc(nn_max * nn_max, double);
      }
      if (old_n0_max < n0_max || fi->scf.nn_max != nn_max)
      {  if (fi->scf.pp_ind != NULL)
            tfree(fi->scf.pp_ind);
         if (fi->scf.pp_inv != NULL)
            tfree(fi->scf.pp_inv);
         if (fi->scf.qq_ind != NULL)
            tfree(fi->scf.qq_ind);
         if (fi->scf.qq_inv != NULL)
            tfree(fi->scf.qq_inv);
         if (fi->w4 != NULL)
            tfree(fi->w4);
         if (fi->w5 != NULL)
            tfree(fi->w5);
         fi->scf.pp_ind = talloc(1+n0_max+nn_max, int);
         fi->scf.pp_inv = talloc(1+n0_max+nn_max, int);
         fi->scf.qq_ind = talloc(1+n0_max+nn_max, int);
         fi->scf.qq_inv = talloc(1+n0_max+nn_max, int);
         fi->w4 = talloc(1+n0_max+nn_max, double);
         fi->w5 = talloc(1+n0_max+nn_max, double);
      }
      /* initialize SC-factorization */
      fi->scf.n = n;
      fi->scf.n0 = n;
      fi->scf.nn_max = nn_max;
      fi->scf.nn = 0;
      fi->scf.rr_ref = sva_alloc_vecs(fi->scf.sva, nn_max);
      fi->scf.ss_ref = sva_alloc_vecs(fi->scf.sva, nn_max);
      fi->scf.ifu.n_max = nn_max;
      fi->scf.ifu.n = 0;
      for (k = 1; k <= n; k++)
      {  fi->scf.pp_ind[k] = k;
         fi->scf.pp_inv[k] = k;
         fi->scf.qq_ind[k] = k;
         fi->scf.qq_inv[k] = k;
      }
      /* set validation flag */
      if (ret == 0)
         fi->valid = 1;
      return ret;
}

int scfint_update(SCFINT *fi, int upd, int j, int len, const int ind[],
      const double val[])
{     /* update SC-factorization after replacing j-th column of A */
      int n = fi->scf.n;
      int n0 = fi->scf.n0;
      int nn = fi->scf.nn;
      int *pp_ind = fi->scf.pp_ind;
      int *qq_ind = fi->scf.qq_ind;
      int *qq_inv = fi->scf.qq_inv;
      double *bf = fi->w4;
      double *dg = fi->w5;
      int k, t, ret;
      xassert(fi->valid);
      xassert(0 <= n && n <= n0+nn);
      /* (b, f) := inv(P) * (beta, 0) */
      for (k = 1; k <= n0+nn; k++)
         bf[k] = 0.0;
      for (t = 1; t <= len; t++)
      {  k = ind[t];
         xassert(1 <= k && k <= n);
#if 1 /* FIXME: currently P = I */
         xassert(pp_ind[k] == k);
#endif
         xassert(bf[k] == 0.0);
         xassert(val[t] != 0.0);
         bf[k] = val[t];
      }
      /* (d, g) := Q * (cj, 0) */
      for (k = 1; k <= n0+nn; k++)
         dg[k] = 0.0;
      xassert(1 <= j && j <= n);
      dg[fi->scf.qq_inv[j]] = 1;
      /* update factorization of augmented matrix */
      ret = scf_update_aug(&fi->scf, &bf[0], &dg[0], &bf[n0], &dg[n0],
         0.0, upd, fi->w1, fi->w2, fi->w3);
      if (ret == 0)
      {  /* swap j-th and last columns of new matrix Q */
         scf_swap_q_cols(j, n0+nn+1);
      }
      else
      {  /* updating failed */
         fi->valid = 0;
      }
      return ret;
}

void scfint_ftran(SCFINT *fi, double x[])
{     /* solve system A * x = b */
      xassert(fi->valid);
      scf_a_solve(&fi->scf, x, fi->w4, fi->w5, fi->w1, fi->w2);
      return;
}

void scfint_btran(SCFINT *fi, double x[])
{     /* solve system A'* x = b */
      xassert(fi->valid);
      scf_at_solve(&fi->scf, x, fi->w4, fi->w5, fi->w1, fi->w2);
      return;
}

double scfint_estimate(SCFINT *fi)
{     /* estimate 1-norm of inv(A) */
      double norm;
      xassert(fi->valid);
      xassert(fi->scf.n == fi->scf.n0);
      switch (fi->scf.type)
      {  case 1:
            norm = luf_estimate_norm(fi->scf.a0.luf, fi->w1, fi->w2);
            break;
         case 2:
            norm = btf_estimate_norm(fi->scf.a0.btf, fi->w1, fi->w2,
               fi->w3, fi->w4);
            break;
         default:
            xassert(fi != fi);
      }
      return norm;
}

void scfint_delete(SCFINT *fi)
{     /* delete interface to SC-factorization */
      switch (fi->scf.type)
      {  case 1:
            lufint_delete(fi->u.lufi);
            break;
         case 2:
            btfint_delete(fi->u.btfi);
            break;
         default:
            xassert(fi != fi);
      }
      if (fi->scf.ifu.f != NULL)
         tfree(fi->scf.ifu.f);
      if (fi->scf.ifu.u != NULL)
         tfree(fi->scf.ifu.u);
      if (fi->scf.pp_ind != NULL)
         tfree(fi->scf.pp_ind);
      if (fi->scf.pp_inv != NULL)
         tfree(fi->scf.pp_inv);
      if (fi->scf.qq_ind != NULL)
         tfree(fi->scf.qq_ind);
      if (fi->scf.qq_inv != NULL)
         tfree(fi->scf.qq_inv);
      if (fi->w1 != NULL)
         tfree(fi->w1);
      if (fi->w2 != NULL)
         tfree(fi->w2);
      if (fi->w3 != NULL)
         tfree(fi->w3);
      if (fi->w4 != NULL)
         tfree(fi->w4);
      if (fi->w5 != NULL)
         tfree(fi->w5);
      tfree(fi);
      return;
}






















typedef struct BFD BFD;

/* return codes: */
#define BFD_ESING    1  /* singular matrix */
#define BFD_ECOND    2  /* ill-conditioned matrix */
#define BFD_ECHECK   3  /* insufficient accuracy */
#define BFD_ELIMIT   4  /* update limit reached */
#if 0 /* 05/III-2014 */
#define BFD_EROOM    5  /* SVA overflow */
#endif

#define bfd_create_it _glp_bfd_create_it
BFD *bfd_create_it(void);
/* create LP basis factorization */

#if 0 /* 08/III-2014 */
#define bfd_set_parm _glp_bfd_set_parm
void bfd_set_parm(BFD *bfd, const void *parm);
/* change LP basis factorization control parameters */
#endif

#define bfd_get_bfcp _glp_bfd_get_bfcp
void bfd_get_bfcp(BFD *bfd, void /* glp_bfcp */ *parm);
/* retrieve LP basis factorization control parameters */

#define bfd_set_bfcp _glp_bfd_set_bfcp
void bfd_set_bfcp(BFD *bfd, const void /* glp_bfcp */ *parm);
/* change LP basis factorization control parameters */

#define bfd_factorize _glp_bfd_factorize
int bfd_factorize(BFD *bfd, int m, /*const int bh[],*/ int (*col)
      (void *info, int j, int ind[], double val[]), void *info);
/* compute LP basis factorization */

#if 1 /* 21/IV-2014 */
#define bfd_condest _glp_bfd_condest
double bfd_condest(BFD *bfd);
/* estimate condition of B */
#endif

#define bfd_ftran _glp_bfd_ftran
void bfd_ftran(BFD *bfd, double x[]);
/* perform forward transformation (solve system B*x = b) */

#if 1 /* 30/III-2016 */
#define bfd_ftran_s _glp_bfd_ftran_s
void bfd_ftran_s(BFD *bfd, FVS *x);
/* sparse version of bfd_ftran */
#endif

#define bfd_btran _glp_bfd_btran
void bfd_btran(BFD *bfd, double x[]);
/* perform backward transformation (solve system B'*x = b) */

#if 1 /* 30/III-2016 */
#define bfd_btran_s _glp_bfd_btran_s
void bfd_btran_s(BFD *bfd, FVS *x);
/* sparse version of bfd_btran */
#endif

#define bfd_update _glp_bfd_update
int bfd_update(BFD *bfd, int j, int len, const int ind[], const double
      val[]);
/* update LP basis factorization */

#define bfd_get_count _glp_bfd_get_count
int bfd_get_count(BFD *bfd);
/* determine factorization update count */

#define bfd_delete_it _glp_bfd_delete_it
void bfd_delete_it(BFD *bfd);
/* delete LP basis factorization */


struct BFD
{     /* LP basis factorization driver */
      int valid;
      /* factorization is valid only if this flag is set */
      int type;
      /* type of factorization used:
         0 - interface not established yet
         1 - FHV-factorization
         2 - Schur-complement-based factorization */
      union
      {  void *none;   /* type = 0 */
         FHVINT *fhvi; /* type = 1 */
         SCFINT *scfi; /* type = 2 */
      }  u;
      /* interface to factorization of LP basis */
      glp_bfcp parm;
      /* factorization control parameters */
#ifdef GLP_DEBUG
      SPM *B;
      /* current basis (for testing/debugging only) */
#endif
      int upd_cnt;
      /* factorization update count */
#if 1 /* 21/IV-2014 */
      double b_norm;
      /* 1-norm of matrix B */
      double i_norm;
      /* estimated 1-norm of matrix inv(B) */
#endif
};

BFD *bfd_create_it(void)
{     /* create LP basis factorization */
      BFD *bfd;
#ifdef GLP_DEBUG
      xprintf("bfd_create_it: warning: debugging version used\n");
#endif
      bfd = talloc(1, BFD);
      bfd->valid = 0;
      bfd->type = 0;
      bfd->u.none = NULL;
      bfd_set_bfcp(bfd, NULL);
#ifdef GLP_DEBUG
      bfd->B = NULL;
#endif
      bfd->upd_cnt = 0;
      return bfd;
}

#if 0 /* 08/III-2014 */
void bfd_set_parm(BFD *bfd, const void *parm)
{     /* change LP basis factorization control parameters */
      memcpy(&bfd->parm, parm, sizeof(glp_bfcp));
      return;
}
#endif

void bfd_get_bfcp(BFD *bfd, void /* glp_bfcp */ *parm)
{     /* retrieve LP basis factorization control parameters */
      memcpy(parm, &bfd->parm, sizeof(glp_bfcp));
      return;
}

void bfd_set_bfcp(BFD *bfd, const void /* glp_bfcp */ *parm)
{     /* change LP basis factorization control parameters */
      if (parm == NULL)
      {  /* reset to default */
         memset(&bfd->parm, 0, sizeof(glp_bfcp));
         bfd->parm.type = GLP_BF_LUF + GLP_BF_FT;
         bfd->parm.piv_tol = 0.10;
         bfd->parm.piv_lim = 4;
         bfd->parm.suhl = 1;
         bfd->parm.eps_tol = DBL_EPSILON;
         bfd->parm.nfs_max = 100;
         bfd->parm.nrs_max = 70;
      }
      else
         memcpy(&bfd->parm, parm, sizeof(glp_bfcp));
      return;
}

#if 1 /* 21/IV-2014 */
struct bfd_info
{     BFD *bfd;
      int (*col)(void *info, int j, int ind[], double val[]);
      void *info;
};

static int bfd_col(void *info_, int j, int ind[], double val[])
{     struct bfd_info *info = info_;
      int t, len;
      double sum;
      len = info->col(info->info, j, ind, val);
      sum = 0.0;
      for (t = 1; t <= len; t++)
      {  if (val[t] >= 0.0)
            sum += val[t];
         else
            sum -= val[t];
      }
      if (info->bfd->b_norm < sum)
         info->bfd->b_norm = sum;
      return len;
}
#endif

int bfd_factorize(BFD *bfd, int m, /*const int bh[],*/ int (*col1)
      (void *info, int j, int ind[], double val[]), void *info1)
{     /* compute LP basis factorization */
#if 1 /* 21/IV-2014 */
      struct bfd_info info;
#endif
      int type, ret;
      /*xassert(bh == bh);*/
      /* invalidate current factorization */
      bfd->valid = 0;
      /* determine required factorization type */
      switch (bfd->parm.type)
      {  case GLP_BF_LUF + GLP_BF_FT:
            type = 1;
            break;
         case GLP_BF_LUF + GLP_BF_BG:
         case GLP_BF_LUF + GLP_BF_GR:
         case GLP_BF_BTF + GLP_BF_BG:
         case GLP_BF_BTF + GLP_BF_GR:
            type = 2;
            break;
         default:
            xassert(bfd != bfd);
      }
      /* delete factorization interface, if necessary */
      switch (bfd->type)
      {  case 0:
            break;
         case 1:
            if (type != 1)
            {  bfd->type = 0;
               fhvint_delete(bfd->u.fhvi);
               bfd->u.fhvi = NULL;
            }
            break;
         case 2:
            if (type != 2)
            {  bfd->type = 0;
               scfint_delete(bfd->u.scfi);
               bfd->u.scfi = NULL;
            }
            break;
         default:
            xassert(bfd != bfd);
      }
      /* establish factorization interface, if necessary */
      if (bfd->type == 0)
      {  switch (type)
         {  case 1:
               bfd->type = 1;
               xassert(bfd->u.fhvi == NULL);
               bfd->u.fhvi = fhvint_create();
               break;
            case 2:
               bfd->type = 2;
               xassert(bfd->u.scfi == NULL);
               if (!(bfd->parm.type & GLP_BF_BTF))
                  bfd->u.scfi = scfint_create(1);
               else
                  bfd->u.scfi = scfint_create(2);
               break;
            default:
               xassert(type != type);
         }
      }
      /* try to compute factorization */
#if 1 /* 21/IV-2014 */
      bfd->b_norm = bfd->i_norm = 0.0;
      info.bfd = bfd;
      info.col = col1;
      info.info = info1;
#endif
      switch (bfd->type)
      {  case 1:
            bfd->u.fhvi->lufi->sgf_piv_tol = bfd->parm.piv_tol;
            bfd->u.fhvi->lufi->sgf_piv_lim = bfd->parm.piv_lim;
            bfd->u.fhvi->lufi->sgf_suhl = bfd->parm.suhl;
            bfd->u.fhvi->lufi->sgf_eps_tol = bfd->parm.eps_tol;
            bfd->u.fhvi->nfs_max = bfd->parm.nfs_max;
            ret = fhvint_factorize(bfd->u.fhvi, m, bfd_col, &info);
#if 1 /* FIXME */
            if (ret == 0)
               bfd->i_norm = fhvint_estimate(bfd->u.fhvi);
            else
               ret = BFD_ESING;
#endif
            break;
         case 2:
            if (bfd->u.scfi->scf.type == 1)
            {  bfd->u.scfi->u.lufi->sgf_piv_tol = bfd->parm.piv_tol;
               bfd->u.scfi->u.lufi->sgf_piv_lim = bfd->parm.piv_lim;
               bfd->u.scfi->u.lufi->sgf_suhl = bfd->parm.suhl;
               bfd->u.scfi->u.lufi->sgf_eps_tol = bfd->parm.eps_tol;
            }
            else if (bfd->u.scfi->scf.type == 2)
            {  bfd->u.scfi->u.btfi->sgf_piv_tol = bfd->parm.piv_tol;
               bfd->u.scfi->u.btfi->sgf_piv_lim = bfd->parm.piv_lim;
               bfd->u.scfi->u.btfi->sgf_suhl = bfd->parm.suhl;
               bfd->u.scfi->u.btfi->sgf_eps_tol = bfd->parm.eps_tol;
            }
            else
               xassert(bfd != bfd);
            bfd->u.scfi->nn_max = bfd->parm.nrs_max;
            ret = scfint_factorize(bfd->u.scfi, m, bfd_col, &info);
#if 1 /* FIXME */
            if (ret == 0)
               bfd->i_norm = scfint_estimate(bfd->u.scfi);
            else
               ret = BFD_ESING;
#endif
            break;
         default:
            xassert(bfd != bfd);
      }
#ifdef GLP_DEBUG
      /* save specified LP basis */
      if (bfd->B != NULL)
         spm_delete_mat(bfd->B);
      bfd->B = spm_create_mat(m, m);
      {  int *ind = talloc(1+m, int);
         double *val = talloc(1+m, double);
         int j, k, len;
         for (j = 1; j <= m; j++)
         {  len = col(info, j, ind, val);
            for (k = 1; k <= len; k++)
               spm_new_elem(bfd->B, ind[k], j, val[k]);
         }
         tfree(ind);
         tfree(val);
      }
#endif
      if (ret == 0)
      {  /* factorization has been successfully computed */
         double cond;
         bfd->valid = 1;
#ifdef GLP_DEBUG
         cond = bfd_condest(bfd);
         if (cond > 1e9)
            xprintf("bfd_factorize: warning: cond(B) = %g\n", cond);
#endif
      }
#ifdef GLP_DEBUG
      xprintf("bfd_factorize: m = %d; ret = %d\n", m, ret);
#endif
      bfd->upd_cnt = 0;
      return ret;
}

#if 0 /* 21/IV-2014 */
double bfd_estimate(BFD *bfd)
{     /* estimate 1-norm of inv(B) */
      double norm;
      xassert(bfd->valid);
      xassert(bfd->upd_cnt == 0);
      switch (bfd->type)
      {  case 1:
            norm = fhvint_estimate(bfd->u.fhvi);
            break;
         case 2:
            norm = scfint_estimate(bfd->u.scfi);
            break;
         default:
            xassert(bfd != bfd);
      }
      return norm;
}
#endif

#if 1 /* 21/IV-2014 */
double bfd_condest(BFD *bfd)
{     /* estimate condition of B */
      double cond;
      xassert(bfd->valid);
      /*xassert(bfd->upd_cnt == 0);*/
      cond = bfd->b_norm * bfd->i_norm;
      if (cond < 1.0)
         cond = 1.0;
      return cond;
}
#endif

void bfd_ftran(BFD *bfd, double x[])
{     /* perform forward transformation (solve system B * x = b) */
#ifdef GLP_DEBUG
      SPM *B = bfd->B;
      int m = B->m;
      double *b = talloc(1+m, double);
      SPME *e;
      int k;
      double s, relerr, maxerr;
      for (k = 1; k <= m; k++)
         b[k] = x[k];
#endif
      xassert(bfd->valid);
      switch (bfd->type)
      {  case 1:
            fhvint_ftran(bfd->u.fhvi, x);
            break;
         case 2:
            scfint_ftran(bfd->u.scfi, x);
            break;
         default:
            xassert(bfd != bfd);
      }
#ifdef GLP_DEBUG
      maxerr = 0.0;
      for (k = 1; k <= m; k++)
      {  s = 0.0;
         for (e = B->row[k]; e != NULL; e = e->r_next)
            s += e->val * x[e->j];
         relerr = (b[k] - s) / (1.0 + fabs(b[k]));
         if (maxerr < relerr)
            maxerr = relerr;
      }
      if (maxerr > 1e-8)
         xprintf("bfd_ftran: maxerr = %g; relative error too large\n",
            maxerr);
      tfree(b);
#endif
      return;
}

#if 1 /* 30/III-2016 */
void bfd_ftran_s(BFD *bfd, FVS *x)
{     /* sparse version of bfd_ftran */
      /* (sparse mode is not implemented yet) */
      int n = x->n;
      int *ind = x->ind;
      double *vec = x->vec;
      int j, nnz = 0;
      bfd_ftran(bfd, vec);
      for (j = n; j >= 1; j--)
      {  if (vec[j] != 0.0)
            ind[++nnz] = j;
      }
      x->nnz = nnz;
      return;
}
#endif

void bfd_btran(BFD *bfd, double x[])
{     /* perform backward transformation (solve system B'* x = b) */
#ifdef GLP_DEBUG
      SPM *B = bfd->B;
      int m = B->m;
      double *b = talloc(1+m, double);
      SPME *e;
      int k;
      double s, relerr, maxerr;
      for (k = 1; k <= m; k++)
         b[k] = x[k];
#endif
      xassert(bfd->valid);
      switch (bfd->type)
      {  case 1:
            fhvint_btran(bfd->u.fhvi, x);
            break;
         case 2:
            scfint_btran(bfd->u.scfi, x);
            break;
         default:
            xassert(bfd != bfd);
      }
#ifdef GLP_DEBUG
      maxerr = 0.0;
      for (k = 1; k <= m; k++)
      {  s = 0.0;
         for (e = B->col[k]; e != NULL; e = e->c_next)
            s += e->val * x[e->i];
         relerr = (b[k] - s) / (1.0 + fabs(b[k]));
         if (maxerr < relerr)
            maxerr = relerr;
      }
      if (maxerr > 1e-8)
         xprintf("bfd_btran: maxerr = %g; relative error too large\n",
            maxerr);
      tfree(b);
#endif
      return;
}

#if 1 /* 30/III-2016 */
void bfd_btran_s(BFD *bfd, FVS *x)
{     /* sparse version of bfd_btran */
      /* (sparse mode is not implemented yet) */
      int n = x->n;
      int *ind = x->ind;
      double *vec = x->vec;
      int j, nnz = 0;
      bfd_btran(bfd, vec);
      for (j = n; j >= 1; j--)
      {  if (vec[j] != 0.0)
            ind[++nnz] = j;
      }
      x->nnz = nnz;
      return;
}
#endif

int bfd_update(BFD *bfd, int j, int len, const int ind[], const double
      val[])
{     /* update LP basis factorization */
      int ret;
      xassert(bfd->valid);
      switch (bfd->type)
      {  case 1:
            ret = fhvint_update(bfd->u.fhvi, j, len, ind, val);
#if 1 /* FIXME */
            switch (ret)
            {  case 0:
                  break;
               case 1:
                  ret = BFD_ESING;
                  break;
               case 2:
               case 3:
                  ret = BFD_ECOND;
                  break;
               case 4:
                  ret = BFD_ELIMIT;
                  break;
               case 5:
                  ret = BFD_ECHECK;
                  break;
               default:
                  xassert(ret != ret);
            }
#endif
            break;
         case 2:
            switch (bfd->parm.type & 0x0F)
            {  case GLP_BF_BG:
                  ret = scfint_update(bfd->u.scfi, 1, j, len, ind, val);
                  break;
               case GLP_BF_GR:
                  ret = scfint_update(bfd->u.scfi, 2, j, len, ind, val);
                  break;
               default:
                  xassert(bfd != bfd);
            }
#if 1 /* FIXME */
            switch (ret)
            {  case 0:
                  break;
               case 1:
                  ret = BFD_ELIMIT;
                  break;
               case 2:
                  ret = BFD_ECOND;
                  break;
               default:
                  xassert(ret != ret);
            }
#endif
            break;
         default:
            xassert(bfd != bfd);
      }
      if (ret != 0)
      {  /* updating factorization failed */
         bfd->valid = 0;
      }
#ifdef GLP_DEBUG
      /* save updated LP basis */
      {  SPME *e;
         int k;
         for (e = bfd->B->col[j]; e != NULL; e = e->c_next)
            e->val = 0.0;
         spm_drop_zeros(bfd->B, 0.0);
         for (k = 1; k <= len; k++)
            spm_new_elem(bfd->B, ind[k], j, val[k]);
      }
#endif
      if (ret == 0)
         bfd->upd_cnt++;
      return ret;
}

int bfd_get_count(BFD *bfd)
{     /* determine factorization update count */
      return bfd->upd_cnt;
}

void bfd_delete_it(BFD *bfd)
{     /* delete LP basis factorization */
      switch (bfd->type)
      {  case 0:
            break;
         case 1:
            fhvint_delete(bfd->u.fhvi);
            break;
         case 2:
            scfint_delete(bfd->u.scfi);
            break;
         default:
            xassert(bfd != bfd);
      }
#ifdef GLP_DEBUG
      if (bfd->B != NULL)
         spm_delete_mat(bfd->B);
#endif
      tfree(bfd);
      return;
}




#if 1 /* 28/III-2016 */
#define GLP_UNDOC 1
#endif

typedef struct GLPROW GLPROW;
typedef struct GLPCOL GLPCOL;
typedef struct GLPAIJ GLPAIJ;

#define GLP_PROB_MAGIC 0xD7D9D6C2

struct glp_prob
{     /* LP/MIP problem object */
      unsigned magic;
      /* magic value used for debugging */
      DMP *pool;
      /* memory pool to store problem object components */
      glp_tree *tree;
      /* pointer to the search tree; set by the MIP solver when this
         object is used in the tree as a core MIP object */
#if 0 /* 08/III-2014 */
      void *parms;
      /* reserved for backward compatibility */
#endif
      /*--------------------------------------------------------------*/
      /* LP/MIP data */
      char *name;
      /* problem name (1 to 255 chars); NULL means no name is assigned
         to the problem */
      char *obj;
      /* objective function name (1 to 255 chars); NULL means no name
         is assigned to the objective function */
      int dir;
      /* optimization direction flag (objective "sense"):
         GLP_MIN - minimization
         GLP_MAX - maximization */
      double c0;
      /* constant term of the objective function ("shift") */
      int m_max;
      /* length of the array of rows (enlarged automatically) */
      int n_max;
      /* length of the array of columns (enlarged automatically) */
      int m;
      /* number of rows, 0 <= m <= m_max */
      int n;
      /* number of columns, 0 <= n <= n_max */
      int nnz;
      /* number of non-zero constraint coefficients, nnz >= 0 */
      GLPROW **row; /* GLPROW *row[1+m_max]; */
      /* row[i], 1 <= i <= m, is a pointer to i-th row */
      GLPCOL **col; /* GLPCOL *col[1+n_max]; */
      /* col[j], 1 <= j <= n, is a pointer to j-th column */
      AVL *r_tree;
      /* row index to find rows by their names; NULL means this index
         does not exist */
      AVL *c_tree;
      /* column index to find columns by their names; NULL means this
         index does not exist */
      /*--------------------------------------------------------------*/
      /* basis factorization (LP) */
      int valid;
      /* the factorization is valid only if this flag is set */
      int *head; /* int head[1+m_max]; */
      /* basis header (valid only if the factorization is valid);
         head[i] = k is the ordinal number of auxiliary (1 <= k <= m)
         or structural (m+1 <= k <= m+n) variable which corresponds to
         i-th basic variable xB[i], 1 <= i <= m */
#if 0 /* 08/III-2014 */
      glp_bfcp *bfcp;
      /* basis factorization control parameters; may be NULL */
#endif
      BFD *bfd; /* BFD bfd[1:m,1:m]; */
      /* basis factorization driver; may be NULL */
      /*--------------------------------------------------------------*/
      /* basic solution (LP) */
      int pbs_stat;
      /* primal basic solution status:
         GLP_UNDEF  - primal solution is undefined
         GLP_FEAS   - primal solution is feasible
         GLP_INFEAS - primal solution is infeasible
         GLP_NOFEAS - no primal feasible solution exists */
      int dbs_stat;
      /* dual basic solution status:
         GLP_UNDEF  - dual solution is undefined
         GLP_FEAS   - dual solution is feasible
         GLP_INFEAS - dual solution is infeasible
         GLP_NOFEAS - no dual feasible solution exists */
      double obj_val;
      /* objective function value */
      int it_cnt;
      /* simplex method iteration count; increases by one on performing
         one simplex iteration */
      int some;
      /* ordinal number of some auxiliary or structural variable having
         certain property, 0 <= some <= m+n */
      /*--------------------------------------------------------------*/
      /* interior-point solution (LP) */
      int ipt_stat;
      /* interior-point solution status:
         GLP_UNDEF  - interior solution is undefined
         GLP_OPT    - interior solution is optimal
         GLP_INFEAS - interior solution is infeasible
         GLP_NOFEAS - no feasible solution exists */
      double ipt_obj;
      /* objective function value */
      /*--------------------------------------------------------------*/
      /* integer solution (MIP) */
      int mip_stat;
      /* integer solution status:
         GLP_UNDEF  - integer solution is undefined
         GLP_OPT    - integer solution is optimal
         GLP_FEAS   - integer solution is feasible
         GLP_NOFEAS - no integer solution exists */
      double mip_obj;
      /* objective function value */
};

struct GLPROW
{     /* LP/MIP row (auxiliary variable) */
      int i;
      /* ordinal number (1 to m) assigned to this row */
      char *name;
      /* row name (1 to 255 chars); NULL means no name is assigned to
         this row */
      AVLNODE *node;
      /* pointer to corresponding node in the row index; NULL means
         that either the row index does not exist or this row has no
         name assigned */
#if 1 /* 20/IX-2008 */
      int level;
      unsigned char origin;
      unsigned char klass;
#endif
      int type;
      /* type of the auxiliary variable:
         GLP_FR - free variable
         GLP_LO - variable with lower bound
         GLP_UP - variable with upper bound
         GLP_DB - double-bounded variable
         GLP_FX - fixed variable */
      double lb; /* non-scaled */
      /* lower bound; if the row has no lower bound, lb is zero */
      double ub; /* non-scaled */
      /* upper bound; if the row has no upper bound, ub is zero */
      /* if the row type is GLP_FX, ub is equal to lb */
      GLPAIJ *ptr; /* non-scaled */
      /* pointer to doubly linked list of constraint coefficients which
         are placed in this row */
      double rii;
      /* diagonal element r[i,i] of scaling matrix R for this row;
         if the scaling is not used, r[i,i] is 1 */
      int stat;
      /* status of the auxiliary variable:
         GLP_BS - basic variable
         GLP_NL - non-basic variable on lower bound
         GLP_NU - non-basic variable on upper bound
         GLP_NF - non-basic free variable
         GLP_NS - non-basic fixed variable */
      int bind;
      /* if the auxiliary variable is basic, head[bind] refers to this
         row, otherwise, bind is 0; this attribute is valid only if the
         basis factorization is valid */
      double prim; /* non-scaled */
      /* primal value of the auxiliary variable in basic solution */
      double dual; /* non-scaled */
      /* dual value of the auxiliary variable in basic solution */
      double pval; /* non-scaled */
      /* primal value of the auxiliary variable in interior solution */
      double dval; /* non-scaled */
      /* dual value of the auxiliary variable in interior solution */
      double mipx; /* non-scaled */
      /* primal value of the auxiliary variable in integer solution */
};

struct GLPCOL
{     /* LP/MIP column (structural variable) */
      int j;
      /* ordinal number (1 to n) assigned to this column */
      char *name;
      /* column name (1 to 255 chars); NULL means no name is assigned
         to this column */
      AVLNODE *node;
      /* pointer to corresponding node in the column index; NULL means
         that either the column index does not exist or the column has
         no name assigned */
      int kind;
      /* kind of the structural variable:
         GLP_CV - continuous variable
         GLP_IV - integer or binary variable */
      int type;
      /* type of the structural variable:
         GLP_FR - free variable
         GLP_LO - variable with lower bound
         GLP_UP - variable with upper bound
         GLP_DB - double-bounded variable
         GLP_FX - fixed variable */
      double lb; /* non-scaled */
      /* lower bound; if the column has no lower bound, lb is zero */
      double ub; /* non-scaled */
      /* upper bound; if the column has no upper bound, ub is zero */
      /* if the column type is GLP_FX, ub is equal to lb */
      double coef; /* non-scaled */
      /* objective coefficient at the structural variable */
      GLPAIJ *ptr; /* non-scaled */
      /* pointer to doubly linked list of constraint coefficients which
         are placed in this column */
      double sjj;
      /* diagonal element s[j,j] of scaling matrix S for this column;
         if the scaling is not used, s[j,j] is 1 */
      int stat;
      /* status of the structural variable:
         GLP_BS - basic variable
         GLP_NL - non-basic variable on lower bound
         GLP_NU - non-basic variable on upper bound
         GLP_NF - non-basic free variable
         GLP_NS - non-basic fixed variable */
      int bind;
      /* if the structural variable is basic, head[bind] refers to
         this column; otherwise, bind is 0; this attribute is valid only
         if the basis factorization is valid */
      double prim; /* non-scaled */
      /* primal value of the structural variable in basic solution */
      double dual; /* non-scaled */
      /* dual value of the structural variable in basic solution */
      double pval; /* non-scaled */
      /* primal value of the structural variable in interior solution */
      double dval; /* non-scaled */
      /* dual value of the structural variable in interior solution */
      double mipx; /* non-scaled */
      /* primal value of the structural variable in integer solution */
};

struct GLPAIJ
{     /* constraint coefficient a[i,j] */
      GLPROW *row;
      /* pointer to row, where this coefficient is placed */
      GLPCOL *col;
      /* pointer to column, where this coefficient is placed */
      double val;
      /* numeric (non-zero) value of this coefficient */
      GLPAIJ *r_prev;
      /* pointer to previous coefficient in the same row */
      GLPAIJ *r_next;
      /* pointer to next coefficient in the same row */
      GLPAIJ *c_prev;
      /* pointer to previous coefficient in the same column */
      GLPAIJ *c_next;
      /* pointer to next coefficient in the same column */
};

#if 1 /* 28/III-2016 */
#define GLP_UNDOC 1
#endif

typedef struct GLPROW GLPROW;
typedef struct GLPCOL GLPCOL;
typedef struct GLPAIJ GLPAIJ;

#define GLP_PROB_MAGIC 0xD7D9D6C2

struct glp_prob
{     /* LP/MIP problem object */
      unsigned magic;
      /* magic value used for debugging */
      DMP *pool;
      /* memory pool to store problem object components */
      glp_tree *tree;
      /* pointer to the search tree; set by the MIP solver when this
         object is used in the tree as a core MIP object */
#if 0 /* 08/III-2014 */
      void *parms;
      /* reserved for backward compatibility */
#endif
      /*--------------------------------------------------------------*/
      /* LP/MIP data */
      char *name;
      /* problem name (1 to 255 chars); NULL means no name is assigned
         to the problem */
      char *obj;
      /* objective function name (1 to 255 chars); NULL means no name
         is assigned to the objective function */
      int dir;
      /* optimization direction flag (objective "sense"):
         GLP_MIN - minimization
         GLP_MAX - maximization */
      double c0;
      /* constant term of the objective function ("shift") */
      int m_max;
      /* length of the array of rows (enlarged automatically) */
      int n_max;
      /* length of the array of columns (enlarged automatically) */
      int m;
      /* number of rows, 0 <= m <= m_max */
      int n;
      /* number of columns, 0 <= n <= n_max */
      int nnz;
      /* number of non-zero constraint coefficients, nnz >= 0 */
      GLPROW **row; /* GLPROW *row[1+m_max]; */
      /* row[i], 1 <= i <= m, is a pointer to i-th row */
      GLPCOL **col; /* GLPCOL *col[1+n_max]; */
      /* col[j], 1 <= j <= n, is a pointer to j-th column */
      AVL *r_tree;
      /* row index to find rows by their names; NULL means this index
         does not exist */
      AVL *c_tree;
      /* column index to find columns by their names; NULL means this
         index does not exist */
      /*--------------------------------------------------------------*/
      /* basis factorization (LP) */
      int valid;
      /* the factorization is valid only if this flag is set */
      int *head; /* int head[1+m_max]; */
      /* basis header (valid only if the factorization is valid);
         head[i] = k is the ordinal number of auxiliary (1 <= k <= m)
         or structural (m+1 <= k <= m+n) variable which corresponds to
         i-th basic variable xB[i], 1 <= i <= m */
#if 0 /* 08/III-2014 */
      glp_bfcp *bfcp;
      /* basis factorization control parameters; may be NULL */
#endif
      BFD *bfd; /* BFD bfd[1:m,1:m]; */
      /* basis factorization driver; may be NULL */
      /*--------------------------------------------------------------*/
      /* basic solution (LP) */
      int pbs_stat;
      /* primal basic solution status:
         GLP_UNDEF  - primal solution is undefined
         GLP_FEAS   - primal solution is feasible
         GLP_INFEAS - primal solution is infeasible
         GLP_NOFEAS - no primal feasible solution exists */
      int dbs_stat;
      /* dual basic solution status:
         GLP_UNDEF  - dual solution is undefined
         GLP_FEAS   - dual solution is feasible
         GLP_INFEAS - dual solution is infeasible
         GLP_NOFEAS - no dual feasible solution exists */
      double obj_val;
      /* objective function value */
      int it_cnt;
      /* simplex method iteration count; increases by one on performing
         one simplex iteration */
      int some;
      /* ordinal number of some auxiliary or structural variable having
         certain property, 0 <= some <= m+n */
      /*--------------------------------------------------------------*/
      /* interior-point solution (LP) */
      int ipt_stat;
      /* interior-point solution status:
         GLP_UNDEF  - interior solution is undefined
         GLP_OPT    - interior solution is optimal
         GLP_INFEAS - interior solution is infeasible
         GLP_NOFEAS - no feasible solution exists */
      double ipt_obj;
      /* objective function value */
      /*--------------------------------------------------------------*/
      /* integer solution (MIP) */
      int mip_stat;
      /* integer solution status:
         GLP_UNDEF  - integer solution is undefined
         GLP_OPT    - integer solution is optimal
         GLP_FEAS   - integer solution is feasible
         GLP_NOFEAS - no integer solution exists */
      double mip_obj;
      /* objective function value */
};

struct GLPROW
{     /* LP/MIP row (auxiliary variable) */
      int i;
      /* ordinal number (1 to m) assigned to this row */
      char *name;
      /* row name (1 to 255 chars); NULL means no name is assigned to
         this row */
      AVLNODE *node;
      /* pointer to corresponding node in the row index; NULL means
         that either the row index does not exist or this row has no
         name assigned */
#if 1 /* 20/IX-2008 */
      int level;
      unsigned char origin;
      unsigned char klass;
#endif
      int type;
      /* type of the auxiliary variable:
         GLP_FR - free variable
         GLP_LO - variable with lower bound
         GLP_UP - variable with upper bound
         GLP_DB - double-bounded variable
         GLP_FX - fixed variable */
      double lb; /* non-scaled */
      /* lower bound; if the row has no lower bound, lb is zero */
      double ub; /* non-scaled */
      /* upper bound; if the row has no upper bound, ub is zero */
      /* if the row type is GLP_FX, ub is equal to lb */
      GLPAIJ *ptr; /* non-scaled */
      /* pointer to doubly linked list of constraint coefficients which
         are placed in this row */
      double rii;
      /* diagonal element r[i,i] of scaling matrix R for this row;
         if the scaling is not used, r[i,i] is 1 */
      int stat;
      /* status of the auxiliary variable:
         GLP_BS - basic variable
         GLP_NL - non-basic variable on lower bound
         GLP_NU - non-basic variable on upper bound
         GLP_NF - non-basic free variable
         GLP_NS - non-basic fixed variable */
      int bind;
      /* if the auxiliary variable is basic, head[bind] refers to this
         row, otherwise, bind is 0; this attribute is valid only if the
         basis factorization is valid */
      double prim; /* non-scaled */
      /* primal value of the auxiliary variable in basic solution */
      double dual; /* non-scaled */
      /* dual value of the auxiliary variable in basic solution */
      double pval; /* non-scaled */
      /* primal value of the auxiliary variable in interior solution */
      double dval; /* non-scaled */
      /* dual value of the auxiliary variable in interior solution */
      double mipx; /* non-scaled */
      /* primal value of the auxiliary variable in integer solution */
};

struct GLPCOL
{     /* LP/MIP column (structural variable) */
      int j;
      /* ordinal number (1 to n) assigned to this column */
      char *name;
      /* column name (1 to 255 chars); NULL means no name is assigned
         to this column */
      AVLNODE *node;
      /* pointer to corresponding node in the column index; NULL means
         that either the column index does not exist or the column has
         no name assigned */
      int kind;
      /* kind of the structural variable:
         GLP_CV - continuous variable
         GLP_IV - integer or binary variable */
      int type;
      /* type of the structural variable:
         GLP_FR - free variable
         GLP_LO - variable with lower bound
         GLP_UP - variable with upper bound
         GLP_DB - double-bounded variable
         GLP_FX - fixed variable */
      double lb; /* non-scaled */
      /* lower bound; if the column has no lower bound, lb is zero */
      double ub; /* non-scaled */
      /* upper bound; if the column has no upper bound, ub is zero */
      /* if the column type is GLP_FX, ub is equal to lb */
      double coef; /* non-scaled */
      /* objective coefficient at the structural variable */
      GLPAIJ *ptr; /* non-scaled */
      /* pointer to doubly linked list of constraint coefficients which
         are placed in this column */
      double sjj;
      /* diagonal element s[j,j] of scaling matrix S for this column;
         if the scaling is not used, s[j,j] is 1 */
      int stat;
      /* status of the structural variable:
         GLP_BS - basic variable
         GLP_NL - non-basic variable on lower bound
         GLP_NU - non-basic variable on upper bound
         GLP_NF - non-basic free variable
         GLP_NS - non-basic fixed variable */
      int bind;
      /* if the structural variable is basic, head[bind] refers to
         this column; otherwise, bind is 0; this attribute is valid only
         if the basis factorization is valid */
      double prim; /* non-scaled */
      /* primal value of the structural variable in basic solution */
      double dual; /* non-scaled */
      /* dual value of the structural variable in basic solution */
      double pval; /* non-scaled */
      /* primal value of the structural variable in interior solution */
      double dval; /* non-scaled */
      /* dual value of the structural variable in interior solution */
      double mipx; /* non-scaled */
      /* primal value of the structural variable in integer solution */
};

struct GLPAIJ
{     /* constraint coefficient a[i,j] */
      GLPROW *row;
      /* pointer to row, where this coefficient is placed */
      GLPCOL *col;
      /* pointer to column, where this coefficient is placed */
      double val;
      /* numeric (non-zero) value of this coefficient */
      GLPAIJ *r_prev;
      /* pointer to previous coefficient in the same row */
      GLPAIJ *r_next;
      /* pointer to next coefficient in the same row */
      GLPAIJ *c_prev;
      /* pointer to previous coefficient in the same column */
      GLPAIJ *c_next;
      /* pointer to next coefficient in the same column */
};




#if 1 /* 29/II-2016 by Chris */
/*----------------------------------------------------------------------
Subject: Mir cut generation performance improvement
From: Chris Matrakidis <cmatraki@gmail.com>
To: Andrew Makhorin <mao@gnu.org>, help-glpk <help-glpk@gnu.org>

Andrew,

I noticed that mir cut generation takes considerable time on some large
problems (like rocII-4-11 from miplib). The attached patch makes two
improvements that considerably improve performance in such instances:
1. A lot of time was spent on generating a temporary vector in function
aggregate_row. It is a lot faster to reuse an existing vector.
2. A search for an element in the same function was done in row order,
where using the elements in the order they are in the column is more
efficient. This changes the generated cuts in some cases, but seems
neutral overall (0.3% less cuts in a test set of 64 miplib instances).

Best Regards,

Chris Matrakidis
----------------------------------------------------------------------*/
#endif

typedef struct SPV SPV;

struct SPV
{     /* sparse vector v = (v[j]) */
      int n;
      /* dimension, n >= 0 */
      int nnz;
      /* number of non-zero components, 0 <= nnz <= n */
      int *pos; /* int pos[1+n]; */
      /* pos[j] = k, 1 <= j <= n, is position of (non-zero) v[j] in the
         arrays ind and val, where 1 <= k <= nnz; pos[j] = 0 means that
         v[j] is structural zero */
      int *ind; /* int ind[1+n]; */
      /* ind[k] = j, 1 <= k <= nnz, is index of v[j] */
      double *val; /* double val[1+n]; */
      /* val[k], 1 <= k <= nnz, is a numeric value of v[j] */
};

#define spv_create_vec _glp_spv_create_vec
SPV *spv_create_vec(int n);
/* create sparse vector */

#define spv_check_vec _glp_spv_check_vec
void spv_check_vec(SPV *v);
/* check that sparse vector has correct representation */

#define spv_get_vj _glp_spv_get_vj
double spv_get_vj(SPV *v, int j);
/* retrieve component of sparse vector */

#define spv_set_vj _glp_spv_set_vj
void spv_set_vj(SPV *v, int j, double val);
/* set/change component of sparse vector */

#define spv_clear_vec _glp_spv_clear_vec
void spv_clear_vec(SPV *v);
/* set all components of sparse vector to zero */

#define spv_clean_vec _glp_spv_clean_vec
void spv_clean_vec(SPV *v, double eps);
/* remove zero or small components from sparse vector */

#define spv_copy_vec _glp_spv_copy_vec
void spv_copy_vec(SPV *x, SPV *y);
/* copy sparse vector (x := y) */

#define spv_linear_comb _glp_spv_linear_comb
void spv_linear_comb(SPV *x, double a, SPV *y);
/* compute linear combination (x := x + a * y) */

#define spv_delete_vec _glp_spv_delete_vec
void spv_delete_vec(SPV *v);
/* delete sparse vector */


/***********************************************************************
*  NAME
*
*  spv_create_vec - create sparse vector
*
*  SYNOPSIS
*
*  #include "glpios.h"
*  SPV *spv_create_vec(int n);
*
*  DESCRIPTION
*
*  The routine spv_create_vec creates a sparse vector of dimension n,
*  which initially is a null vector.
*
*  RETURNS
*
*  The routine returns a pointer to the vector created. */

SPV *spv_create_vec(int n)
{     SPV *v;
      xassert(n >= 0);
      v = xmalloc(sizeof(SPV));
      v->n = n;
      v->nnz = 0;
      v->pos = xcalloc(1+n, sizeof(int));
      memset(&v->pos[1], 0, n * sizeof(int));
      v->ind = xcalloc(1+n, sizeof(int));
      v->val = xcalloc(1+n, sizeof(double));
      return v;
}

/***********************************************************************
*  NAME
*
*  spv_check_vec - check that sparse vector has correct representation
*
*  SYNOPSIS
*
*  #include "glpios.h"
*  void spv_check_vec(SPV *v);
*
*  DESCRIPTION
*
*  The routine spv_check_vec checks that a sparse vector specified by
*  the parameter v has correct representation.
*
*  NOTE
*
*  Complexity of this operation is O(n). */

void spv_check_vec(SPV *v)
{     int j, k, nnz;
      xassert(v->n >= 0);
      nnz = 0;
      for (j = v->n; j >= 1; j--)
      {  k = v->pos[j];
         xassert(0 <= k && k <= v->nnz);
         if (k != 0)
         {  xassert(v->ind[k] == j);
            nnz++;
         }
      }
      xassert(v->nnz == nnz);
      return;
}

/***********************************************************************
*  NAME
*
*  spv_get_vj - retrieve component of sparse vector
*
*  SYNOPSIS
*
*  #include "glpios.h"
*  double spv_get_vj(SPV *v, int j);
*
*  RETURNS
*
*  The routine spv_get_vj returns j-th component of a sparse vector
*  specified by the parameter v. */

double spv_get_vj(SPV *v, int j)
{     int k;
      xassert(1 <= j && j <= v->n);
      k = v->pos[j];
      xassert(0 <= k && k <= v->nnz);
      return (k == 0 ? 0.0 : v->val[k]);
}

/***********************************************************************
*  NAME
*
*  spv_set_vj - set/change component of sparse vector
*
*  SYNOPSIS
*
*  #include "glpios.h"
*  void spv_set_vj(SPV *v, int j, double val);
*
*  DESCRIPTION
*
*  The routine spv_set_vj assigns val to j-th component of a sparse
*  vector specified by the parameter v. */

void spv_set_vj(SPV *v, int j, double val)
{     int k;
      xassert(1 <= j && j <= v->n);
      k = v->pos[j];
      if (val == 0.0)
      {  if (k != 0)
         {  /* remove j-th component */
            v->pos[j] = 0;
            if (k < v->nnz)
            {  v->pos[v->ind[v->nnz]] = k;
               v->ind[k] = v->ind[v->nnz];
               v->val[k] = v->val[v->nnz];
            }
            v->nnz--;
         }
      }
      else
      {  if (k == 0)
         {  /* create j-th component */
            k = ++(v->nnz);
            v->pos[j] = k;
            v->ind[k] = j;
         }
         v->val[k] = val;
      }
      return;
}

/***********************************************************************
*  NAME
*
*  spv_clear_vec - set all components of sparse vector to zero
*
*  SYNOPSIS
*
*  #include "glpios.h"
*  void spv_clear_vec(SPV *v);
*
*  DESCRIPTION
*
*  The routine spv_clear_vec sets all components of a sparse vector
*  specified by the parameter v to zero. */

void spv_clear_vec(SPV *v)
{     int k;
      for (k = 1; k <= v->nnz; k++)
         v->pos[v->ind[k]] = 0;
      v->nnz = 0;
      return;
}

/***********************************************************************
*  NAME
*
*  spv_clean_vec - remove zero or small components from sparse vector
*
*  SYNOPSIS
*
*  #include "glpios.h"
*  void spv_clean_vec(SPV *v, double eps);
*
*  DESCRIPTION
*
*  The routine spv_clean_vec removes zero components and components
*  whose magnitude is less than eps from a sparse vector specified by
*  the parameter v. If eps is 0.0, only zero components are removed. */

void spv_clean_vec(SPV *v, double eps)
{     int k, nnz;
      nnz = 0;
      for (k = 1; k <= v->nnz; k++)
      {  if (fabs(v->val[k]) == 0.0 || fabs(v->val[k]) < eps)
         {  /* remove component */
            v->pos[v->ind[k]] = 0;
         }
         else
         {  /* keep component */
            nnz++;
            v->pos[v->ind[k]] = nnz;
            v->ind[nnz] = v->ind[k];
            v->val[nnz] = v->val[k];
         }
      }
      v->nnz = nnz;
      return;
}

/***********************************************************************
*  NAME
*
*  spv_copy_vec - copy sparse vector (x := y)
*
*  SYNOPSIS
*
*  #include "glpios.h"
*  void spv_copy_vec(SPV *x, SPV *y);
*
*  DESCRIPTION
*
*  The routine spv_copy_vec copies a sparse vector specified by the
*  parameter y to a sparse vector specified by the parameter x. */

void spv_copy_vec(SPV *x, SPV *y)
{     int j;
      xassert(x != y);
      xassert(x->n == y->n);
      spv_clear_vec(x);
      x->nnz = y->nnz;
      memcpy(&x->ind[1], &y->ind[1], x->nnz * sizeof(int));
      memcpy(&x->val[1], &y->val[1], x->nnz * sizeof(double));
      for (j = 1; j <= x->nnz; j++)
         x->pos[x->ind[j]] = j;
      return;
}

/***********************************************************************
*  NAME
*
*  spv_linear_comb - compute linear combination (x := x + a * y)
*
*  SYNOPSIS
*
*  #include "glpios.h"
*  void spv_linear_comb(SPV *x, double a, SPV *y);
*
*  DESCRIPTION
*
*  The routine spv_linear_comb computes the linear combination
*
*     x := x + a * y,
*
*  where x and y are sparse vectors, a is a scalar. */

void spv_linear_comb(SPV *x, double a, SPV *y)
{     int j, k;
      double xj, yj;
      xassert(x != y);
      xassert(x->n == y->n);
      for (k = 1; k <= y->nnz; k++)
      {  j = y->ind[k];
         xj = spv_get_vj(x, j);
         yj = y->val[k];
         spv_set_vj(x, j, xj + a * yj);
      }
      return;
}

/***********************************************************************
*  NAME
*
*  spv_delete_vec - delete sparse vector
*
*  SYNOPSIS
*
*  #include "glpios.h"
*  void spv_delete_vec(SPV *v);
*
*  DESCRIPTION
*
*  The routine spv_delete_vec deletes a sparse vector specified by the
*  parameter v freeing all the memory allocated to this object. */

void spv_delete_vec(SPV *v)
{     /* delete sparse vector */
      xfree(v->pos);
      xfree(v->ind);
      xfree(v->val);
      xfree(v);
      return;
}
#define MIR_DEBUG 0

#define MAXAGGR 5
/* maximal number of rows that can be aggregated */


typedef struct glp_mir glp_mir;
struct glp_mir
{     /* MIR cut generator working area */
      /*--------------------------------------------------------------*/
      /* global information valid for the root subproblem */
      int m;
      /* number of rows (in the root subproblem) */
      int n;
      /* number of columns */
      char *skip; /* char skip[1+m]; */
      /* skip[i], 1 <= i <= m, is a flag that means that row i should
         not be used because (1) it is not suitable, or (2) because it
         has been used in the aggregated constraint */
      char *isint; /* char isint[1+m+n]; */
      /* isint[k], 1 <= k <= m+n, is a flag that means that variable
         x[k] is integer (otherwise, continuous) */
      double *lb; /* double lb[1+m+n]; */
      /* lb[k], 1 <= k <= m+n, is lower bound of x[k]; -DBL_MAX means
         that x[k] has no lower bound */
      int *vlb; /* int vlb[1+m+n]; */
      /* vlb[k] = k', 1 <= k <= m+n, is the number of integer variable,
         which defines variable lower bound x[k] >= lb[k] * x[k']; zero
         means that x[k] has simple lower bound */
      double *ub; /* double ub[1+m+n]; */
      /* ub[k], 1 <= k <= m+n, is upper bound of x[k]; +DBL_MAX means
         that x[k] has no upper bound */
      int *vub; /* int vub[1+m+n]; */
      /* vub[k] = k', 1 <= k <= m+n, is the number of integer variable,
         which defines variable upper bound x[k] <= ub[k] * x[k']; zero
         means that x[k] has simple upper bound */
      /*--------------------------------------------------------------*/
      /* current (fractional) point to be separated */
      double *x; /* double x[1+m+n]; */
      /* x[k] is current value of auxiliary (1 <= k <= m) or structural
         (m+1 <= k <= m+n) variable */
      /*--------------------------------------------------------------*/
      /* aggregated constraint sum a[k] * x[k] = b, which is a linear
         combination of original constraints transformed to equalities
         by introducing auxiliary variables */
      int agg_cnt;
      /* number of rows (original constraints) used to build aggregated
         constraint, 1 <= agg_cnt <= MAXAGGR */
      int *agg_row; /* int agg_row[1+MAXAGGR]; */
      /* agg_row[k], 1 <= k <= agg_cnt, is the row number used to build
         aggregated constraint */
      SPV *agg_vec; /* SPV agg_vec[1:m+n]; */
      /* sparse vector of aggregated constraint coefficients, a[k] */
      double agg_rhs;
      /* right-hand side of the aggregated constraint, b */
      /*--------------------------------------------------------------*/
      /* bound substitution flags for modified constraint */
      char *subst; /* char subst[1+m+n]; */
      /* subst[k], 1 <= k <= m+n, is a bound substitution flag used for
         variable x[k]:
         '?' - x[k] is missing in modified constraint
         'L' - x[k] = (lower bound) + x'[k]
         'U' - x[k] = (upper bound) - x'[k] */
      /*--------------------------------------------------------------*/
      /* modified constraint sum a'[k] * x'[k] = b', where x'[k] >= 0,
         derived from aggregated constraint by substituting bounds;
         note that due to substitution of variable bounds there may be
         additional terms in the modified constraint */
      SPV *mod_vec; /* SPV mod_vec[1:m+n]; */
      /* sparse vector of modified constraint coefficients, a'[k] */
      double mod_rhs;
      /* right-hand side of the modified constraint, b' */
      /*--------------------------------------------------------------*/
      /* cutting plane sum alpha[k] * x[k] <= beta */
      SPV *cut_vec; /* SPV cut_vec[1:m+n]; */
      /* sparse vector of cutting plane coefficients, alpha[k] */
      double cut_rhs;
      /* right-hand size of the cutting plane, beta */
};

/***********************************************************************
*  NAME
*
*  glp_mir_init - create and initialize MIR cut generator
*
*  SYNOPSIS
*
*  glp_mir *glp_mir_init(glp_prob *P);
*
*  DESCRIPTION
*
*  This routine creates and initializes the MIR cut generator for the
*  specified problem object.
*
*  RETURNS
*
*  The routine returns a pointer to the MIR cut generator workspace. */

static void set_row_attrib(glp_prob *mip, glp_mir *mir)
{     /* set global row attributes */
      int m = mir->m;
      int k;
      for (k = 1; k <= m; k++)
      {  GLPROW *row = mip->row[k];
         mir->skip[k] = 0;
         mir->isint[k] = 0;
         switch (row->type)
         {  case GLP_FR:
               mir->lb[k] = -DBL_MAX, mir->ub[k] = +DBL_MAX; break;
            case GLP_LO:
               mir->lb[k] = row->lb, mir->ub[k] = +DBL_MAX; break;
            case GLP_UP:
               mir->lb[k] = -DBL_MAX, mir->ub[k] = row->ub; break;
            case GLP_DB:
               mir->lb[k] = row->lb, mir->ub[k] = row->ub; break;
            case GLP_FX:
               mir->lb[k] = mir->ub[k] = row->lb; break;
            default:
               xassert(row != row);
         }
         mir->vlb[k] = mir->vub[k] = 0;
      }
      return;
}

static void set_col_attrib(glp_prob *mip, glp_mir *mir)
{     /* set global column attributes */
      int m = mir->m;
      int n = mir->n;
      int k;
      for (k = m+1; k <= m+n; k++)
      {  GLPCOL *col = mip->col[k-m];
         switch (col->kind)
         {  case GLP_CV:
               mir->isint[k] = 0; break;
            case GLP_IV:
               mir->isint[k] = 1; break;
            default:
               xassert(col != col);
         }
         switch (col->type)
         {  case GLP_FR:
               mir->lb[k] = -DBL_MAX, mir->ub[k] = +DBL_MAX; break;
            case GLP_LO:
               mir->lb[k] = col->lb, mir->ub[k] = +DBL_MAX; break;
            case GLP_UP:
               mir->lb[k] = -DBL_MAX, mir->ub[k] = col->ub; break;
            case GLP_DB:
               mir->lb[k] = col->lb, mir->ub[k] = col->ub; break;
            case GLP_FX:
               mir->lb[k] = mir->ub[k] = col->lb; break;
            default:
               xassert(col != col);
         }
         mir->vlb[k] = mir->vub[k] = 0;
      }
      return;
}

static void set_var_bounds(glp_prob *mip, glp_mir *mir)
{     /* set variable bounds */
      int m = mir->m;
      GLPAIJ *aij;
      int i, k1, k2;
      double a1, a2;
      for (i = 1; i <= m; i++)
      {  /* we need the row to be '>= 0' or '<= 0' */
         if (!(mir->lb[i] == 0.0 && mir->ub[i] == +DBL_MAX ||
               mir->lb[i] == -DBL_MAX && mir->ub[i] == 0.0)) continue;
         /* take first term */
         aij = mip->row[i]->ptr;
         if (aij == NULL) continue;
         k1 = m + aij->col->j, a1 = aij->val;
         /* take second term */
         aij = aij->r_next;
         if (aij == NULL) continue;
         k2 = m + aij->col->j, a2 = aij->val;
         /* there must be only two terms */
         if (aij->r_next != NULL) continue;
         /* interchange terms, if needed */
         if (!mir->isint[k1] && mir->isint[k2])
            ;
         else if (mir->isint[k1] && !mir->isint[k2])
         {  k2 = k1, a2 = a1;
            k1 = m + aij->col->j, a1 = aij->val;
         }
         else
         {  /* both terms are either continuous or integer */
            continue;
         }
         /* x[k2] should be double-bounded */
         if (mir->lb[k2] == -DBL_MAX || mir->ub[k2] == +DBL_MAX ||
             mir->lb[k2] == mir->ub[k2]) continue;
         /* change signs, if necessary */
         if (mir->ub[i] == 0.0) a1 = - a1, a2 = - a2;
         /* now the row has the form a1 * x1 + a2 * x2 >= 0, where x1
            is continuous, x2 is integer */
         if (a1 > 0.0)
         {  /* x1 >= - (a2 / a1) * x2 */
            if (mir->vlb[k1] == 0)
            {  /* set variable lower bound for x1 */
               mir->lb[k1] = - a2 / a1;
               mir->vlb[k1] = k2;
               /* the row should not be used */
               mir->skip[i] = 1;
            }
         }
         else /* a1 < 0.0 */
         {  /* x1 <= - (a2 / a1) * x2 */
            if (mir->vub[k1] == 0)
            {  /* set variable upper bound for x1 */
               mir->ub[k1] = - a2 / a1;
               mir->vub[k1] = k2;
               /* the row should not be used */
               mir->skip[i] = 1;
            }
         }
      }
      return;
}

static void mark_useless_rows(glp_prob *mip, glp_mir *mir)
{     /* mark rows which should not be used */
      int m = mir->m;
      GLPAIJ *aij;
      int i, k, nv;
      for (i = 1; i <= m; i++)
      {  /* free rows should not be used */
         if (mir->lb[i] == -DBL_MAX && mir->ub[i] == +DBL_MAX)
         {  mir->skip[i] = 1;
            continue;
         }
         nv = 0;
         for (aij = mip->row[i]->ptr; aij != NULL; aij = aij->r_next)
         {  k = m + aij->col->j;
            /* rows with free variables should not be used */
            if (mir->lb[k] == -DBL_MAX && mir->ub[k] == +DBL_MAX)
            {  mir->skip[i] = 1;
               break;
            }
            /* rows with integer variables having infinite (lower or
               upper) bound should not be used */
            if (mir->isint[k] && mir->lb[k] == -DBL_MAX ||
                mir->isint[k] && mir->ub[k] == +DBL_MAX)
            {  mir->skip[i] = 1;
               break;
            }
            /* count non-fixed variables */
            if (!(mir->vlb[k] == 0 && mir->vub[k] == 0 &&
                  mir->lb[k] == mir->ub[k])) nv++;
         }
         /* rows with all variables fixed should not be used */
         if (nv == 0)
         {  mir->skip[i] = 1;
            continue;
         }
      }
      return;
}

glp_mir *glp_mir_init(glp_prob *mip)
{     /* create and initialize MIR cut generator */
      int m = mip->m;
      int n = mip->n;
      glp_mir *mir;
#if MIR_DEBUG
      xprintf("ios_mir_init: warning: debug mode enabled\n");
#endif
      /* allocate working area */
      mir = xmalloc(sizeof(glp_mir));
      mir->m = m;
      mir->n = n;
      mir->skip = xcalloc(1+m, sizeof(char));
      mir->isint = xcalloc(1+m+n, sizeof(char));
      mir->lb = xcalloc(1+m+n, sizeof(double));
      mir->vlb = xcalloc(1+m+n, sizeof(int));
      mir->ub = xcalloc(1+m+n, sizeof(double));
      mir->vub = xcalloc(1+m+n, sizeof(int));
      mir->x = xcalloc(1+m+n, sizeof(double));
      mir->agg_row = xcalloc(1+MAXAGGR, sizeof(int));
      mir->agg_vec = spv_create_vec(m+n);
      mir->subst = xcalloc(1+m+n, sizeof(char));
      mir->mod_vec = spv_create_vec(m+n);
      mir->cut_vec = spv_create_vec(m+n);
      /* set global row attributes */
      set_row_attrib(mip, mir);
      /* set global column attributes */
      set_col_attrib(mip, mir);
      /* set variable bounds */
      set_var_bounds(mip, mir);
      /* mark rows which should not be used */
      mark_useless_rows(mip, mir);
      return mir;
}

/***********************************************************************
*  NAME
*
*  glp_mir_gen - generate mixed integer rounding (MIR) cuts
*
*  SYNOPSIS
*
*  int glp_mir_gen(glp_prob *P, glp_mir *mir, glp_prob *pool);
*
*  DESCRIPTION
*
*  This routine attempts to generate mixed integer rounding (MIR) cuts
*  for current basic solution to the specified problem object.
*
*  The cutting plane inequalities generated by the routine are added to
*  the specified cut pool.
*
*  RETURNS
*
*  The routine returns the number of cuts that have been generated and
*  added to the cut pool. */

static void get_current_point(glp_prob *mip, glp_mir *mir)
{     /* obtain current point */
      int m = mir->m;
      int n = mir->n;
      int k;
      for (k = 1; k <= m; k++)
         mir->x[k] = mip->row[k]->prim;
      for (k = m+1; k <= m+n; k++)
         mir->x[k] = mip->col[k-m]->prim;
      return;
}

#if MIR_DEBUG
static void check_current_point(glp_mir *mir)
{     /* check current point */
      int m = mir->m;
      int n = mir->n;
      int k, kk;
      double lb, ub, eps;
      for (k = 1; k <= m+n; k++)
      {  /* determine lower bound */
         lb = mir->lb[k];
         kk = mir->vlb[k];
         if (kk != 0)
         {  xassert(lb != -DBL_MAX);
            xassert(!mir->isint[k]);
            xassert(mir->isint[kk]);
            lb *= mir->x[kk];
         }
         /* check lower bound */
         if (lb != -DBL_MAX)
         {  eps = 1e-6 * (1.0 + fabs(lb));
            xassert(mir->x[k] >= lb - eps);
         }
         /* determine upper bound */
         ub = mir->ub[k];
         kk = mir->vub[k];
         if (kk != 0)
         {  xassert(ub != +DBL_MAX);
            xassert(!mir->isint[k]);
            xassert(mir->isint[kk]);
            ub *= mir->x[kk];
         }
         /* check upper bound */
         if (ub != +DBL_MAX)
         {  eps = 1e-6 * (1.0 + fabs(ub));
            xassert(mir->x[k] <= ub + eps);
         }
      }
      return;
}
#endif

static void initial_agg_row(glp_prob *mip, glp_mir *mir, int i)
{     /* use original i-th row as initial aggregated constraint */
      int m = mir->m;
      GLPAIJ *aij;
      xassert(1 <= i && i <= m);
      xassert(!mir->skip[i]);
      /* mark i-th row in order not to use it in the same aggregated
         constraint */
      mir->skip[i] = 2;
      mir->agg_cnt = 1;
      mir->agg_row[1] = i;
      /* use x[i] - sum a[i,j] * x[m+j] = 0, where x[i] is auxiliary
         variable of row i, x[m+j] are structural variables */
      spv_clear_vec(mir->agg_vec);
      spv_set_vj(mir->agg_vec, i, 1.0);
      for (aij = mip->row[i]->ptr; aij != NULL; aij = aij->r_next)
         spv_set_vj(mir->agg_vec, m + aij->col->j, - aij->val);
      mir->agg_rhs = 0.0;
#if MIR_DEBUG
      spv_check_vec(mir->agg_vec);
#endif
      return;
}

#if MIR_DEBUG
static void check_agg_row(glp_mir *mir)
{     /* check aggregated constraint */
      int m = mir->m;
      int n = mir->n;
      int j, k;
      double r, big;
      /* compute the residual r = sum a[k] * x[k] - b and determine
         big = max(1, |a[k]|, |b|) */
      r = 0.0, big = 1.0;
      for (j = 1; j <= mir->agg_vec->nnz; j++)
      {  k = mir->agg_vec->ind[j];
         xassert(1 <= k && k <= m+n);
         r += mir->agg_vec->val[j] * mir->x[k];
         if (big < fabs(mir->agg_vec->val[j]))
            big = fabs(mir->agg_vec->val[j]);
      }
      r -= mir->agg_rhs;
      if (big < fabs(mir->agg_rhs))
         big = fabs(mir->agg_rhs);
      /* the residual must be close to zero */
      xassert(fabs(r) <= 1e-6 * big);
      return;
}
#endif

static void subst_fixed_vars(glp_mir *mir)
{     /* substitute fixed variables into aggregated constraint */
      int m = mir->m;
      int n = mir->n;
      int j, k;
      for (j = 1; j <= mir->agg_vec->nnz; j++)
      {  k = mir->agg_vec->ind[j];
         xassert(1 <= k && k <= m+n);
         if (mir->vlb[k] == 0 && mir->vub[k] == 0 &&
             mir->lb[k] == mir->ub[k])
         {  /* x[k] is fixed */
            mir->agg_rhs -= mir->agg_vec->val[j] * mir->lb[k];
            mir->agg_vec->val[j] = 0.0;
         }
      }
      /* remove terms corresponding to fixed variables */
      spv_clean_vec(mir->agg_vec, DBL_EPSILON);
#if MIR_DEBUG
      spv_check_vec(mir->agg_vec);
#endif
      return;
}

static void bound_subst_heur(glp_mir *mir)
{     /* bound substitution heuristic */
      int m = mir->m;
      int n = mir->n;
      int j, k, kk;
      double d1, d2;
      for (j = 1; j <= mir->agg_vec->nnz; j++)
      {  k = mir->agg_vec->ind[j];
         xassert(1 <= k && k <= m+n);
         if (mir->isint[k]) continue; /* skip integer variable */
         /* compute distance from x[k] to its lower bound */
         kk = mir->vlb[k];
         if (kk == 0)
         {  if (mir->lb[k] == -DBL_MAX)
               d1 = DBL_MAX;
            else
               d1 = mir->x[k] - mir->lb[k];
         }
         else
         {  xassert(1 <= kk && kk <= m+n);
            xassert(mir->isint[kk]);
            xassert(mir->lb[k] != -DBL_MAX);
            d1 = mir->x[k] - mir->lb[k] * mir->x[kk];
         }
         /* compute distance from x[k] to its upper bound */
         kk = mir->vub[k];
         if (kk == 0)
         {  if (mir->vub[k] == +DBL_MAX)
               d2 = DBL_MAX;
            else
               d2 = mir->ub[k] - mir->x[k];
         }
         else
         {  xassert(1 <= kk && kk <= m+n);
            xassert(mir->isint[kk]);
            xassert(mir->ub[k] != +DBL_MAX);
            d2 = mir->ub[k] * mir->x[kk] - mir->x[k];
         }
         /* x[k] cannot be free */
         xassert(d1 != DBL_MAX || d2 != DBL_MAX);
         /* choose the bound which is closer to x[k] */
         xassert(mir->subst[k] == '?');
         if (d1 <= d2)
            mir->subst[k] = 'L';
         else
            mir->subst[k] = 'U';
      }
      return;
}

static void build_mod_row(glp_mir *mir)
{     /* substitute bounds and build modified constraint */
      int m = mir->m;
      int n = mir->n;
      int j, jj, k, kk;
      /* initially modified constraint is aggregated constraint */
      spv_copy_vec(mir->mod_vec, mir->agg_vec);
      mir->mod_rhs = mir->agg_rhs;
#if MIR_DEBUG
      spv_check_vec(mir->mod_vec);
#endif
      /* substitute bounds for continuous variables; note that due to
         substitution of variable bounds additional terms may appear in
         modified constraint */
      for (j = mir->mod_vec->nnz; j >= 1; j--)
      {  k = mir->mod_vec->ind[j];
         xassert(1 <= k && k <= m+n);
         if (mir->isint[k]) continue; /* skip integer variable */
         if (mir->subst[k] == 'L')
         {  /* x[k] = (lower bound) + x'[k] */
            xassert(mir->lb[k] != -DBL_MAX);
            kk = mir->vlb[k];
            if (kk == 0)
            {  /* x[k] = lb[k] + x'[k] */
               mir->mod_rhs -= mir->mod_vec->val[j] * mir->lb[k];
            }
            else
            {  /* x[k] = lb[k] * x[kk] + x'[k] */
               xassert(mir->isint[kk]);
               jj = mir->mod_vec->pos[kk];
               if (jj == 0)
               {  spv_set_vj(mir->mod_vec, kk, 1.0);
                  jj = mir->mod_vec->pos[kk];
                  mir->mod_vec->val[jj] = 0.0;
               }
               mir->mod_vec->val[jj] +=
                  mir->mod_vec->val[j] * mir->lb[k];
            }
         }
         else if (mir->subst[k] == 'U')
         {  /* x[k] = (upper bound) - x'[k] */
            xassert(mir->ub[k] != +DBL_MAX);
            kk = mir->vub[k];
            if (kk == 0)
            {  /* x[k] = ub[k] - x'[k] */
               mir->mod_rhs -= mir->mod_vec->val[j] * mir->ub[k];
            }
            else
            {  /* x[k] = ub[k] * x[kk] - x'[k] */
               xassert(mir->isint[kk]);
               jj = mir->mod_vec->pos[kk];
               if (jj == 0)
               {  spv_set_vj(mir->mod_vec, kk, 1.0);
                  jj = mir->mod_vec->pos[kk];
                  mir->mod_vec->val[jj] = 0.0;
               }
               mir->mod_vec->val[jj] +=
                  mir->mod_vec->val[j] * mir->ub[k];
            }
            mir->mod_vec->val[j] = - mir->mod_vec->val[j];
         }
         else
            xassert(k != k);
      }
#if MIR_DEBUG
      spv_check_vec(mir->mod_vec);
#endif
      /* substitute bounds for integer variables */
      for (j = 1; j <= mir->mod_vec->nnz; j++)
      {  k = mir->mod_vec->ind[j];
         xassert(1 <= k && k <= m+n);
         if (!mir->isint[k]) continue; /* skip continuous variable */
         xassert(mir->subst[k] == '?');
         xassert(mir->vlb[k] == 0 && mir->vub[k] == 0);
         xassert(mir->lb[k] != -DBL_MAX && mir->ub[k] != +DBL_MAX);
         if (fabs(mir->lb[k]) <= fabs(mir->ub[k]))
         {  /* x[k] = lb[k] + x'[k] */
            mir->subst[k] = 'L';
            mir->mod_rhs -= mir->mod_vec->val[j] * mir->lb[k];
         }
         else
         {  /* x[k] = ub[k] - x'[k] */
            mir->subst[k] = 'U';
            mir->mod_rhs -= mir->mod_vec->val[j] * mir->ub[k];
            mir->mod_vec->val[j] = - mir->mod_vec->val[j];
         }
      }
#if MIR_DEBUG
      spv_check_vec(mir->mod_vec);
#endif
      return;
}

#if MIR_DEBUG
static void check_mod_row(glp_mir *mir)
{     /* check modified constraint */
      int m = mir->m;
      int n = mir->n;
      int j, k, kk;
      double r, big, x;
      /* compute the residual r = sum a'[k] * x'[k] - b' and determine
         big = max(1, |a[k]|, |b|) */
      r = 0.0, big = 1.0;
      for (j = 1; j <= mir->mod_vec->nnz; j++)
      {  k = mir->mod_vec->ind[j];
         xassert(1 <= k && k <= m+n);
         if (mir->subst[k] == 'L')
         {  /* x'[k] = x[k] - (lower bound) */
            xassert(mir->lb[k] != -DBL_MAX);
            kk = mir->vlb[k];
            if (kk == 0)
               x = mir->x[k] - mir->lb[k];
            else
               x = mir->x[k] - mir->lb[k] * mir->x[kk];
         }
         else if (mir->subst[k] == 'U')
         {  /* x'[k] = (upper bound) - x[k] */
            xassert(mir->ub[k] != +DBL_MAX);
            kk = mir->vub[k];
            if (kk == 0)
               x = mir->ub[k] - mir->x[k];
            else
               x = mir->ub[k] * mir->x[kk] - mir->x[k];
         }
         else
            xassert(k != k);
         r += mir->mod_vec->val[j] * x;
         if (big < fabs(mir->mod_vec->val[j]))
            big = fabs(mir->mod_vec->val[j]);
      }
      r -= mir->mod_rhs;
      if (big < fabs(mir->mod_rhs))
         big = fabs(mir->mod_rhs);
      /* the residual must be close to zero */
      xassert(fabs(r) <= 1e-6 * big);
      return;
}
#endif

/***********************************************************************
*  mir_ineq - construct MIR inequality
*
*  Given the single constraint mixed integer set
*
*                    |N|
*     X = {(x,s) in Z    x R  : sum   a[j] * x[j] <= b + s},
*                    +      +  j in N
*
*  this routine constructs the mixed integer rounding (MIR) inequality
*
*     sum   alpha[j] * x[j] <= beta + gamma * s,
*    j in N
*
*  which is valid for X.
*
*  If the MIR inequality has been successfully constructed, the routine
*  returns zero. Otherwise, if b is close to nearest integer, there may
*  be numeric difficulties due to big coefficients; so in this case the
*  routine returns non-zero. */

static int mir_ineq(const int n, const double a[], const double b,
      double alpha[], double *beta, double *gamma)
{     int j;
      double f, t;
      if (fabs(b - floor(b + .5)) < 0.01)
         return 1;
      f = b - floor(b);
      for (j = 1; j <= n; j++)
      {  t = (a[j] - floor(a[j])) - f;
         if (t <= 0.0)
            alpha[j] = floor(a[j]);
         else
            alpha[j] = floor(a[j]) + t / (1.0 - f);
      }
      *beta = floor(b);
      *gamma = 1.0 / (1.0 - f);
      return 0;
}

/***********************************************************************
*  cmir_ineq - construct c-MIR inequality
*
*  Given the mixed knapsack set
*
*      MK              |N|
*     X   = {(x,s) in Z    x R  : sum   a[j] * x[j] <= b + s,
*                      +      +  j in N
*
*             x[j] <= u[j]},
*
*  a subset C of variables to be complemented, and a divisor delta > 0,
*  this routine constructs the complemented MIR (c-MIR) inequality
*
*     sum   alpha[j] * x[j] <= beta + gamma * s,
*    j in N
*                      MK
*  which is valid for X  .
*
*  If the c-MIR inequality has been successfully constructed, the
*  routine returns zero. Otherwise, if there is a risk of numerical
*  difficulties due to big coefficients (see comments to the routine
*  mir_ineq), the routine cmir_ineq returns non-zero. */

static int cmir_ineq(const int n, const double a[], const double b,
      const double u[], const char cset[], const double delta,
      double alpha[], double *beta, double *gamma)
{     int j;
      double *aa, bb;
      aa = alpha, bb = b;
      for (j = 1; j <= n; j++)
      {  aa[j] = a[j] / delta;
         if (cset[j])
            aa[j] = - aa[j], bb -= a[j] * u[j];
      }
      bb /= delta;
      if (mir_ineq(n, aa, bb, alpha, beta, gamma)) return 1;
      for (j = 1; j <= n; j++)
      {  if (cset[j])
            alpha[j] = - alpha[j], *beta += alpha[j] * u[j];
      }
      *gamma /= delta;
      return 0;
}

/***********************************************************************
*  cmir_sep - c-MIR separation heuristic
*
*  Given the mixed knapsack set
*
*      MK              |N|
*     X   = {(x,s) in Z    x R  : sum   a[j] * x[j] <= b + s,
*                      +      +  j in N
*
*             x[j] <= u[j]}
*
*                           *   *
*  and a fractional point (x , s ), this routine tries to construct
*  c-MIR inequality
*
*     sum   alpha[j] * x[j] <= beta + gamma * s,
*    j in N
*                      MK
*  which is valid for X   and has (desirably maximal) violation at the
*  fractional point given. This is attained by choosing an appropriate
*  set C of variables to be complemented and a divisor delta > 0, which
*  together define corresponding c-MIR inequality.
*
*  If a violated c-MIR inequality has been successfully constructed,
*  the routine returns its violation:
*
*                       *                      *
*     sum   alpha[j] * x [j] - beta - gamma * s ,
*    j in N
*
*  which is positive. In case of failure the routine returns zero. */

struct vset { int j; double v; };

static int cmir_cmp(const void *p1, const void *p2)
{     const struct vset *v1 = p1, *v2 = p2;
      if (v1->v < v2->v) return -1;
      if (v1->v > v2->v) return +1;
      return 0;
}

static double cmir_sep(const int n, const double a[], const double b,
      const double u[], const double x[], const double s,
      double alpha[], double *beta, double *gamma)
{     int fail, j, k, nv, v;
      double delta, eps, d_try[1+3], r, r_best;
      char *cset;
      struct vset *vset;
      /* allocate working arrays */
      cset = xcalloc(1+n, sizeof(char));
      vset = xcalloc(1+n, sizeof(struct vset));
      /* choose initial C */
      for (j = 1; j <= n; j++)
         cset[j] = (char)(x[j] >= 0.5 * u[j]);
      /* choose initial delta */
      r_best = delta = 0.0;
      for (j = 1; j <= n; j++)
      {  xassert(a[j] != 0.0);
         /* if x[j] is close to its bounds, skip it */
         eps = 1e-9 * (1.0 + fabs(u[j]));
         if (x[j] < eps || x[j] > u[j] - eps) continue;
         /* try delta = |a[j]| to construct c-MIR inequality */
         fail = cmir_ineq(n, a, b, u, cset, fabs(a[j]), alpha, beta,
            gamma);
         if (fail) continue;
         /* compute violation */
         r = - (*beta) - (*gamma) * s;
         for (k = 1; k <= n; k++) r += alpha[k] * x[k];
         if (r_best < r) r_best = r, delta = fabs(a[j]);
      }
      if (r_best < 0.001) r_best = 0.0;
      if (r_best == 0.0) goto done;
      xassert(delta > 0.0);
      /* try to increase violation by dividing delta by 2, 4, and 8,
         respectively */
      d_try[1] = delta / 2.0;
      d_try[2] = delta / 4.0;
      d_try[3] = delta / 8.0;
      for (j = 1; j <= 3; j++)
      {  /* construct c-MIR inequality */
         fail = cmir_ineq(n, a, b, u, cset, d_try[j], alpha, beta,
            gamma);
         if (fail) continue;
         /* compute violation */
         r = - (*beta) - (*gamma) * s;
         for (k = 1; k <= n; k++) r += alpha[k] * x[k];
         if (r_best < r) r_best = r, delta = d_try[j];
      }
      /* build subset of variables lying strictly between their bounds
         and order it by nondecreasing values of |x[j] - u[j]/2| */
      nv = 0;
      for (j = 1; j <= n; j++)
      {  /* if x[j] is close to its bounds, skip it */
         eps = 1e-9 * (1.0 + fabs(u[j]));
         if (x[j] < eps || x[j] > u[j] - eps) continue;
         /* add x[j] to the subset */
         nv++;
         vset[nv].j = j;
         vset[nv].v = fabs(x[j] - 0.5 * u[j]);
      }
      qsort(&vset[1], nv, sizeof(struct vset), cmir_cmp);
      /* try to increase violation by successively complementing each
         variable in the subset */
      for (v = 1; v <= nv; v++)
      {  j = vset[v].j;
         /* replace x[j] by its complement or vice versa */
         cset[j] = (char)!cset[j];
         /* construct c-MIR inequality */
         fail = cmir_ineq(n, a, b, u, cset, delta, alpha, beta, gamma);
         /* restore the variable */
         cset[j] = (char)!cset[j];
         /* do not replace the variable in case of failure */
         if (fail) continue;
         /* compute violation */
         r = - (*beta) - (*gamma) * s;
         for (k = 1; k <= n; k++) r += alpha[k] * x[k];
         if (r_best < r) r_best = r, cset[j] = (char)!cset[j];
      }
      /* construct the best c-MIR inequality chosen */
      fail = cmir_ineq(n, a, b, u, cset, delta, alpha, beta, gamma);
      xassert(!fail);
done: /* free working arrays */
      xfree(cset);
      xfree(vset);
      /* return to the calling routine */
      return r_best;
}

static double generate(glp_mir *mir)
{     /* try to generate violated c-MIR cut for modified constraint */
      int m = mir->m;
      int n = mir->n;
      int j, k, kk, nint;
      double s, *u, *x, *alpha, r_best = 0.0, b, beta, gamma;
      spv_copy_vec(mir->cut_vec, mir->mod_vec);
      mir->cut_rhs = mir->mod_rhs;
      /* remove small terms, which can appear due to substitution of
         variable bounds */
      spv_clean_vec(mir->cut_vec, DBL_EPSILON);
#if MIR_DEBUG
      spv_check_vec(mir->cut_vec);
#endif
      /* remove positive continuous terms to obtain MK relaxation */
      for (j = 1; j <= mir->cut_vec->nnz; j++)
      {  k = mir->cut_vec->ind[j];
         xassert(1 <= k && k <= m+n);
         if (!mir->isint[k] && mir->cut_vec->val[j] > 0.0)
            mir->cut_vec->val[j] = 0.0;
      }
      spv_clean_vec(mir->cut_vec, 0.0);
#if MIR_DEBUG
      spv_check_vec(mir->cut_vec);
#endif
      /* move integer terms to the beginning of the sparse vector and
         determine the number of integer variables */
      nint = 0;
      for (j = 1; j <= mir->cut_vec->nnz; j++)
      {  k = mir->cut_vec->ind[j];
         xassert(1 <= k && k <= m+n);
         if (mir->isint[k])
         {  double temp;
            nint++;
            /* interchange elements [nint] and [j] */
            kk = mir->cut_vec->ind[nint];
            mir->cut_vec->pos[k] = nint;
            mir->cut_vec->pos[kk] = j;
            mir->cut_vec->ind[nint] = k;
            mir->cut_vec->ind[j] = kk;
            temp = mir->cut_vec->val[nint];
            mir->cut_vec->val[nint] = mir->cut_vec->val[j];
            mir->cut_vec->val[j] = temp;
         }
      }
#if MIR_DEBUG
      spv_check_vec(mir->cut_vec);
#endif
      /* if there is no integer variable, nothing to generate */
      if (nint == 0) goto done;
      /* allocate working arrays */
      u = xcalloc(1+nint, sizeof(double));
      x = xcalloc(1+nint, sizeof(double));
      alpha = xcalloc(1+nint, sizeof(double));
      /* determine u and x */
      for (j = 1; j <= nint; j++)
      {  k = mir->cut_vec->ind[j];
         xassert(m+1 <= k && k <= m+n);
         xassert(mir->isint[k]);
         u[j] = mir->ub[k] - mir->lb[k];
         xassert(u[j] >= 1.0);
         if (mir->subst[k] == 'L')
            x[j] = mir->x[k] - mir->lb[k];
         else if (mir->subst[k] == 'U')
            x[j] = mir->ub[k] - mir->x[k];
         else
            xassert(k != k);
#if 0 /* 06/III-2016; notorious bug reported many times */
         xassert(x[j] >= -0.001);
#else
         if (x[j] < -0.001)
         {  xprintf("glp_mir_gen: warning: x[%d] = %g\n", j, x[j]);
            r_best = 0.0;
            goto skip;
         }
#endif
         if (x[j] < 0.0) x[j] = 0.0;
      }
      /* compute s = - sum of continuous terms */
      s = 0.0;
      for (j = nint+1; j <= mir->cut_vec->nnz; j++)
      {  double x;
         k = mir->cut_vec->ind[j];
         xassert(1 <= k && k <= m+n);
         /* must be continuous */
         xassert(!mir->isint[k]);
         if (mir->subst[k] == 'L')
         {  xassert(mir->lb[k] != -DBL_MAX);
            kk = mir->vlb[k];
            if (kk == 0)
               x = mir->x[k] - mir->lb[k];
            else
               x = mir->x[k] - mir->lb[k] * mir->x[kk];
         }
         else if (mir->subst[k] == 'U')
         {  xassert(mir->ub[k] != +DBL_MAX);
            kk = mir->vub[k];
            if (kk == 0)
               x = mir->ub[k] - mir->x[k];
            else
               x = mir->ub[k] * mir->x[kk] - mir->x[k];
         }
         else
            xassert(k != k);
#if 0 /* 06/III-2016; notorious bug reported many times */
         xassert(x >= -0.001);
#else
         if (x < -0.001)
         {  xprintf("glp_mir_gen: warning: x = %g\n", x);
            r_best = 0.0;
            goto skip;
         }
#endif
         if (x < 0.0) x = 0.0;
         s -= mir->cut_vec->val[j] * x;
      }
      xassert(s >= 0.0);
      /* apply heuristic to obtain most violated c-MIR inequality */
      b = mir->cut_rhs;
      r_best = cmir_sep(nint, mir->cut_vec->val, b, u, x, s, alpha,
         &beta, &gamma);
      if (r_best == 0.0) goto skip;
      xassert(r_best > 0.0);
      /* convert to raw cut */
      /* sum alpha[j] * x[j] <= beta + gamma * s */
      for (j = 1; j <= nint; j++)
         mir->cut_vec->val[j] = alpha[j];
      for (j = nint+1; j <= mir->cut_vec->nnz; j++)
      {  k = mir->cut_vec->ind[j];
         if (k <= m+n) mir->cut_vec->val[j] *= gamma;
      }
      mir->cut_rhs = beta;
#if MIR_DEBUG
      spv_check_vec(mir->cut_vec);
#endif
skip: /* free working arrays */
      xfree(u);
      xfree(x);
      xfree(alpha);
done: return r_best;
}

#if MIR_DEBUG
static void check_raw_cut(glp_mir *mir, double r_best)
{     /* check raw cut before back bound substitution */
      int m = mir->m;
      int n = mir->n;
      int j, k, kk;
      double r, big, x;
      /* compute the residual r = sum a[k] * x[k] - b and determine
         big = max(1, |a[k]|, |b|) */
      r = 0.0, big = 1.0;
      for (j = 1; j <= mir->cut_vec->nnz; j++)
      {  k = mir->cut_vec->ind[j];
         xassert(1 <= k && k <= m+n);
         if (mir->subst[k] == 'L')
         {  xassert(mir->lb[k] != -DBL_MAX);
            kk = mir->vlb[k];
            if (kk == 0)
               x = mir->x[k] - mir->lb[k];
            else
               x = mir->x[k] - mir->lb[k] * mir->x[kk];
         }
         else if (mir->subst[k] == 'U')
         {  xassert(mir->ub[k] != +DBL_MAX);
            kk = mir->vub[k];
            if (kk == 0)
               x = mir->ub[k] - mir->x[k];
            else
               x = mir->ub[k] * mir->x[kk] - mir->x[k];
         }
         else
            xassert(k != k);
         r += mir->cut_vec->val[j] * x;
         if (big < fabs(mir->cut_vec->val[j]))
            big = fabs(mir->cut_vec->val[j]);
      }
      r -= mir->cut_rhs;
      if (big < fabs(mir->cut_rhs))
         big = fabs(mir->cut_rhs);
      /* the residual must be close to r_best */
      xassert(fabs(r - r_best) <= 1e-6 * big);
      return;
}
#endif

static void back_subst(glp_mir *mir)
{     /* back substitution of original bounds */
      int m = mir->m;
      int n = mir->n;
      int j, jj, k, kk;
      /* at first, restore bounds of integer variables (because on
         restoring variable bounds of continuous variables we need
         original, not shifted, bounds of integer variables) */
      for (j = 1; j <= mir->cut_vec->nnz; j++)
      {  k = mir->cut_vec->ind[j];
         xassert(1 <= k && k <= m+n);
         if (!mir->isint[k]) continue; /* skip continuous */
         if (mir->subst[k] == 'L')
         {  /* x'[k] = x[k] - lb[k] */
            xassert(mir->lb[k] != -DBL_MAX);
            xassert(mir->vlb[k] == 0);
            mir->cut_rhs += mir->cut_vec->val[j] * mir->lb[k];
         }
         else if (mir->subst[k] == 'U')
         {  /* x'[k] = ub[k] - x[k] */
            xassert(mir->ub[k] != +DBL_MAX);
            xassert(mir->vub[k] == 0);
            mir->cut_rhs -= mir->cut_vec->val[j] * mir->ub[k];
            mir->cut_vec->val[j] = - mir->cut_vec->val[j];
         }
         else
            xassert(k != k);
      }
      /* now restore bounds of continuous variables */
      for (j = 1; j <= mir->cut_vec->nnz; j++)
      {  k = mir->cut_vec->ind[j];
         xassert(1 <= k && k <= m+n);
         if (mir->isint[k]) continue; /* skip integer */
         if (mir->subst[k] == 'L')
         {  /* x'[k] = x[k] - (lower bound) */
            xassert(mir->lb[k] != -DBL_MAX);
            kk = mir->vlb[k];
            if (kk == 0)
            {  /* x'[k] = x[k] - lb[k] */
               mir->cut_rhs += mir->cut_vec->val[j] * mir->lb[k];
            }
            else
            {  /* x'[k] = x[k] - lb[k] * x[kk] */
               jj = mir->cut_vec->pos[kk];
#if 0
               xassert(jj != 0);
#else
               if (jj == 0)
               {  spv_set_vj(mir->cut_vec, kk, 1.0);
                  jj = mir->cut_vec->pos[kk];
                  xassert(jj != 0);
                  mir->cut_vec->val[jj] = 0.0;
               }
#endif
               mir->cut_vec->val[jj] -= mir->cut_vec->val[j] *
                  mir->lb[k];
            }
         }
         else if (mir->subst[k] == 'U')
         {  /* x'[k] = (upper bound) - x[k] */
            xassert(mir->ub[k] != +DBL_MAX);
            kk = mir->vub[k];
            if (kk == 0)
            {  /* x'[k] = ub[k] - x[k] */
               mir->cut_rhs -= mir->cut_vec->val[j] * mir->ub[k];
            }
            else
            {  /* x'[k] = ub[k] * x[kk] - x[k] */
               jj = mir->cut_vec->pos[kk];
               if (jj == 0)
               {  spv_set_vj(mir->cut_vec, kk, 1.0);
                  jj = mir->cut_vec->pos[kk];
                  xassert(jj != 0);
                  mir->cut_vec->val[jj] = 0.0;
               }
               mir->cut_vec->val[jj] += mir->cut_vec->val[j] *
                  mir->ub[k];
            }
            mir->cut_vec->val[j] = - mir->cut_vec->val[j];
         }
         else
            xassert(k != k);
      }
#if MIR_DEBUG
      spv_check_vec(mir->cut_vec);
#endif
      return;
}

#if MIR_DEBUG
static void check_cut_row(glp_mir *mir, double r_best)
{     /* check the cut after back bound substitution or elimination of
         auxiliary variables */
      int m = mir->m;
      int n = mir->n;
      int j, k;
      double r, big;
      /* compute the residual r = sum a[k] * x[k] - b and determine
         big = max(1, |a[k]|, |b|) */
      r = 0.0, big = 1.0;
      for (j = 1; j <= mir->cut_vec->nnz; j++)
      {  k = mir->cut_vec->ind[j];
         xassert(1 <= k && k <= m+n);
         r += mir->cut_vec->val[j] * mir->x[k];
         if (big < fabs(mir->cut_vec->val[j]))
            big = fabs(mir->cut_vec->val[j]);
      }
      r -= mir->cut_rhs;
      if (big < fabs(mir->cut_rhs))
         big = fabs(mir->cut_rhs);
      /* the residual must be close to r_best */
      xassert(fabs(r - r_best) <= 1e-6 * big);
      return;
}
#endif

static void subst_aux_vars(glp_prob *mip, glp_mir *mir)
{     /* final substitution to eliminate auxiliary variables */
      int m = mir->m;
      int n = mir->n;
      GLPAIJ *aij;
      int j, k, kk, jj;
      for (j = mir->cut_vec->nnz; j >= 1; j--)
      {  k = mir->cut_vec->ind[j];
         xassert(1 <= k && k <= m+n);
         if (k > m) continue; /* skip structurals */
         for (aij = mip->row[k]->ptr; aij != NULL; aij = aij->r_next)
         {  kk = m + aij->col->j; /* structural */
            jj = mir->cut_vec->pos[kk];
            if (jj == 0)
            {  spv_set_vj(mir->cut_vec, kk, 1.0);
               jj = mir->cut_vec->pos[kk];
               mir->cut_vec->val[jj] = 0.0;
            }
            mir->cut_vec->val[jj] += mir->cut_vec->val[j] * aij->val;
         }
         mir->cut_vec->val[j] = 0.0;
      }
      spv_clean_vec(mir->cut_vec, 0.0);
      return;
}

static void add_cut(glp_mir *mir, glp_prob *pool)
{     /* add constructed cut inequality to the cut pool */
      int m = mir->m;
      int n = mir->n;
      int j, k, len;
      int *ind = xcalloc(1+n, sizeof(int));
      double *val = xcalloc(1+n, sizeof(double));
      len = 0;
      for (j = mir->cut_vec->nnz; j >= 1; j--)
      {  k = mir->cut_vec->ind[j];
         xassert(m+1 <= k && k <= m+n);
         len++, ind[len] = k - m, val[len] = mir->cut_vec->val[j];
      }
#if 0
#if 0
      ios_add_cut_row(tree, pool, GLP_RF_MIR, len, ind, val, GLP_UP,
         mir->cut_rhs);
#else
      glp_ios_add_row(tree, NULL, GLP_RF_MIR, 0, len, ind, val, GLP_UP,
         mir->cut_rhs);
#endif
#else
      {  int i;
         i = glp_add_rows(pool, 1);
         glp_set_row_bnds(pool, i, GLP_UP, 0, mir->cut_rhs);
         glp_set_mat_row(pool, i, len, ind, val);
      }
#endif
      xfree(ind);
      xfree(val);
      return;
}

#if 0 /* 29/II-2016 by Chris */
static int aggregate_row(glp_prob *mip, glp_mir *mir)
#else
static int aggregate_row(glp_prob *mip, glp_mir *mir, SPV *v)
#endif
{     /* try to aggregate another row */
      int m = mir->m;
      int n = mir->n;
      GLPAIJ *aij;
#if 0 /* 29/II-2016 by Chris */
      SPV *v;
#endif
      int ii, j, jj, k, kk, kappa = 0, ret = 0;
      double d1, d2, d, d_max = 0.0;
      /* choose appropriate structural variable in the aggregated row
         to be substituted */
      for (j = 1; j <= mir->agg_vec->nnz; j++)
      {  k = mir->agg_vec->ind[j];
         xassert(1 <= k && k <= m+n);
         if (k <= m) continue; /* skip auxiliary var */
         if (mir->isint[k]) continue; /* skip integer var */
         if (fabs(mir->agg_vec->val[j]) < 0.001) continue;
         /* compute distance from x[k] to its lower bound */
         kk = mir->vlb[k];
         if (kk == 0)
         {  if (mir->lb[k] == -DBL_MAX)
               d1 = DBL_MAX;
            else
               d1 = mir->x[k] - mir->lb[k];
         }
         else
         {  xassert(1 <= kk && kk <= m+n);
            xassert(mir->isint[kk]);
            xassert(mir->lb[k] != -DBL_MAX);
            d1 = mir->x[k] - mir->lb[k] * mir->x[kk];
         }
         /* compute distance from x[k] to its upper bound */
         kk = mir->vub[k];
         if (kk == 0)
         {  if (mir->vub[k] == +DBL_MAX)
               d2 = DBL_MAX;
            else
               d2 = mir->ub[k] - mir->x[k];
         }
         else
         {  xassert(1 <= kk && kk <= m+n);
            xassert(mir->isint[kk]);
            xassert(mir->ub[k] != +DBL_MAX);
            d2 = mir->ub[k] * mir->x[kk] - mir->x[k];
         }
         /* x[k] cannot be free */
         xassert(d1 != DBL_MAX || d2 != DBL_MAX);
         /* d = min(d1, d2) */
         d = (d1 <= d2 ? d1 : d2);
         xassert(d != DBL_MAX);
         /* should not be close to corresponding bound */
         if (d < 0.001) continue;
         if (d_max < d) d_max = d, kappa = k;
      }
      if (kappa == 0)
      {  /* nothing chosen */
         ret = 1;
         goto done;
      }
      /* x[kappa] has been chosen */
      xassert(m+1 <= kappa && kappa <= m+n);
      xassert(!mir->isint[kappa]);
      /* find another row, which have not been used yet, to eliminate
         x[kappa] from the aggregated row */
#if 0 /* 29/II-2016 by Chris */
      for (ii = 1; ii <= m; ii++)
      {  if (mir->skip[ii]) continue;
         for (aij = mip->row[ii]->ptr; aij != NULL; aij = aij->r_next)
            if (aij->col->j == kappa - m) break;
         if (aij != NULL && fabs(aij->val) >= 0.001) break;
#else
      ii = 0;
      for (aij = mip->col[kappa - m]->ptr; aij != NULL;
         aij = aij->c_next)
      {  if (aij->row->i > m) continue;
         if (mir->skip[aij->row->i]) continue;
         if (fabs(aij->val) >= 0.001)
         {  ii = aij->row->i;
            break;
         }
#endif
      }
#if 0 /* 29/II-2016 by Chris */
      if (ii > m)
#else
      if (ii == 0)
#endif
      {  /* nothing found */
         ret = 2;
         goto done;
      }
      /* row ii has been found; include it in the aggregated list */
      mir->agg_cnt++;
      xassert(mir->agg_cnt <= MAXAGGR);
      mir->agg_row[mir->agg_cnt] = ii;
      mir->skip[ii] = 2;
      /* v := new row */
#if 0 /* 29/II-2016 by Chris */
      v = ios_create_vec(m+n);
#else
      spv_clear_vec(v);
#endif
      spv_set_vj(v, ii, 1.0);
      for (aij = mip->row[ii]->ptr; aij != NULL; aij = aij->r_next)
         spv_set_vj(v, m + aij->col->j, - aij->val);
#if MIR_DEBUG
      spv_check_vec(v);
#endif
      /* perform gaussian elimination to remove x[kappa] */
      j = mir->agg_vec->pos[kappa];
      xassert(j != 0);
      jj = v->pos[kappa];
      xassert(jj != 0);
      spv_linear_comb(mir->agg_vec,
         - mir->agg_vec->val[j] / v->val[jj], v);
#if 0 /* 29/II-2016 by Chris */
      ios_delete_vec(v);
#endif
      spv_set_vj(mir->agg_vec, kappa, 0.0);
#if MIR_DEBUG
      spv_check_vec(mir->agg_vec);
#endif
done: return ret;
}

int glp_mir_gen(glp_prob *mip, glp_mir *mir, glp_prob *pool)
{     /* main routine to generate MIR cuts */
      int m = mir->m;
      int n = mir->n;
      int i, nnn = 0;
      double r_best;
#if 1 /* 29/II-2016 by Chris */
      SPV *work;
#endif
      xassert(mip->m >= m);
      xassert(mip->n == n);
      /* obtain current point */
      get_current_point(mip, mir);
#if MIR_DEBUG
      /* check current point */
      check_current_point(mir);
#endif
      /* reset bound substitution flags */
      memset(&mir->subst[1], '?', m+n);
#if 1 /* 29/II-2016 by Chris */
      work = spv_create_vec(m+n);
#endif
      /* try to generate a set of violated MIR cuts */
      for (i = 1; i <= m; i++)
      {  if (mir->skip[i]) continue;
         /* use original i-th row as initial aggregated constraint */
         initial_agg_row(mip, mir, i);
loop:    ;
#if MIR_DEBUG
         /* check aggregated row */
         check_agg_row(mir);
#endif
         /* substitute fixed variables into aggregated constraint */
         subst_fixed_vars(mir);
#if MIR_DEBUG
         /* check aggregated row */
         check_agg_row(mir);
#endif
#if MIR_DEBUG
         /* check bound substitution flags */
         {  int k;
            for (k = 1; k <= m+n; k++)
               xassert(mir->subst[k] == '?');
         }
#endif
         /* apply bound substitution heuristic */
         bound_subst_heur(mir);
         /* substitute bounds and build modified constraint */
         build_mod_row(mir);
#if MIR_DEBUG
         /* check modified row */
         check_mod_row(mir);
#endif
         /* try to generate violated c-MIR cut for modified row */
         r_best = generate(mir);
         if (r_best > 0.0)
         {  /* success */
#if MIR_DEBUG
            /* check raw cut before back bound substitution */
            check_raw_cut(mir, r_best);
#endif
            /* back substitution of original bounds */
            back_subst(mir);
#if MIR_DEBUG
            /* check the cut after back bound substitution */
            check_cut_row(mir, r_best);
#endif
            /* final substitution to eliminate auxiliary variables */
            subst_aux_vars(mip, mir);
#if MIR_DEBUG
            /* check the cut after elimination of auxiliaries */
            check_cut_row(mir, r_best);
#endif
            /* add constructed cut inequality to the cut pool */
            add_cut(mir, pool), nnn++;
         }
         /* reset bound substitution flags */
         {  int j, k;
            for (j = 1; j <= mir->mod_vec->nnz; j++)
            {  k = mir->mod_vec->ind[j];
               xassert(1 <= k && k <= m+n);
               xassert(mir->subst[k] != '?');
               mir->subst[k] = '?';
            }
         }
         if (r_best == 0.0)
         {  /* failure */
            if (mir->agg_cnt < MAXAGGR)
            {  /* try to aggregate another row */
#if 0 /* 29/II-2016 by Chris */
               if (aggregate_row(mip, mir) == 0) goto loop;
#else
               if (aggregate_row(mip, mir, work) == 0) goto loop;
#endif
            }
         }
         /* unmark rows used in the aggregated constraint */
         {  int k, ii;
            for (k = 1; k <= mir->agg_cnt; k++)
            {  ii = mir->agg_row[k];
               xassert(1 <= ii && ii <= m);
               xassert(mir->skip[ii] == 2);
               mir->skip[ii] = 0;
            }
         }
      }
#if 1 /* 29/II-2016 by Chris */
      spv_delete_vec(work);
#endif
      return nnn;
}

/***********************************************************************
*  NAME
*
*  glp_mir_free - delete MIR cut generator workspace
*
*  SYNOPSIS
*
*  void glp_mir_free(glp_mir *mir);
*
*  DESCRIPTION
*
*  This routine deletes the MIR cut generator workspace and frees all
*  the memory allocated to it. */

void glp_mir_free(glp_mir *mir)
{     xfree(mir->skip);
      xfree(mir->isint);
      xfree(mir->lb);
      xfree(mir->vlb);
      xfree(mir->ub);
      xfree(mir->vub);
      xfree(mir->x);
      xfree(mir->agg_row);
      spv_delete_vec(mir->agg_vec);
      xfree(mir->subst);
      spv_delete_vec(mir->mod_vec);
      spv_delete_vec(mir->cut_vec);
      xfree(mir);
      return;
}







/***********************************************************************
*  The structure CFG describes the conflict graph.
*
*  Conflict graph is an undirected graph G = (V, E), where V is a set
*  of vertices, E <= V x V is a set of edges. Each vertex v in V of the
*  conflict graph corresponds to a binary variable z[v], which is
*  either an original binary variable x[j] or its complement 1 - x[j].
*  Edge (v,w) in E means that z[v] and z[w] cannot take the value 1 at
*  the same time, i.e. it defines an inequality z[v] + z[w] <= 1, which
*  is assumed to be valid for original MIP.
*
*  Since the conflict graph may be dense, it is stored as an union of
*  its cliques rather than explicitly. */

#if 0 /* 08/III-2016 */
typedef struct CFG CFG;
#else
typedef struct glp_cfg CFG;
#endif
typedef struct CFGVLE CFGVLE;
typedef struct CFGCLE CFGCLE;

#if 0 /* 08/III-2016 */
struct CFG
#else
struct glp_cfg
#endif
{     /* conflict graph descriptor */
      int n;
      /* number of *all* variables (columns) in corresponding MIP */
      int *pos; /* int pos[1+n]; */
      /* pos[0] is not used;
       * pos[j] = v, 1 <= j <= n, means that vertex v corresponds to
       * original binary variable x[j], and pos[j] = 0 means that the
       * conflict graph has no such vertex */
      int *neg; /* int neg[1+n]; */
      /* neg[0] is not used;
       * neg[j] = v, 1 <= j <= n, means that vertex v corresponds to
       * complement of original binary variable x[j], and neg[j] = 0
       * means that the conflict graph has no such vertex */
      DMP *pool;
      /* memory pool to allocate elements of the conflict graph */
      int nv_max;
      /* maximal number of vertices in the conflict graph */
      int nv;
      /* current number of vertices in the conflict graph */
      int *ref; /* int ref[1+nv_max]; */
      /* ref[v] = j, 1 <= v <= nv, means that vertex v corresponds
       * either to original binary variable x[j] or to its complement,
       * i.e. either pos[j] = v or neg[j] = v */
      CFGVLE **vptr; /* CFGVLE *vptr[1+nv_max]; */
      /* vptr[v], 1 <= v <= nv, is an initial pointer to the list of
       * vertices adjacent to vertex v */
      CFGCLE **cptr; /* CFGCLE *cptr[1+nv_max]; */
      /* cptr[v], 1 <= v <= nv, is an initial pointer to the list of
       * cliques that contain vertex v */
};

struct CFGVLE
{     /* vertex list element */
      int v;
      /* vertex number, 1 <= v <= nv */
      CFGVLE *next;
      /* pointer to next vertex list element */
};

struct CFGCLE
{     /* clique list element */
      CFGVLE *vptr;
      /* initial pointer to the list of clique vertices */
      CFGCLE *next;
      /* pointer to next clique list element */
};

#define cfg_create_graph _glp_cfg_create_graph
CFG *cfg_create_graph(int n, int nv_max);
/* create conflict graph */

#define cfg_add_clique _glp_cfg_add_clique
void cfg_add_clique(CFG *G, int size, const int ind[]);
/* add clique to conflict graph */

#define cfg_get_adjacent _glp_cfg_get_adjacent
int cfg_get_adjacent(CFG *G, int v, int ind[]);
/* get vertices adjacent to specified vertex */

#define cfg_expand_clique _glp_cfg_expand_clique
int cfg_expand_clique(CFG *G, int c_len, int c_ind[]);
/* expand specified clique to maximal clique */

#define cfg_check_clique _glp_cfg_check_clique
void cfg_check_clique(CFG *G, int c_len, const int c_ind[]);
/* check clique in conflict graph */

#define cfg_delete_graph _glp_cfg_delete_graph
void cfg_delete_graph(CFG *G);
/* delete conflict graph */

#define cfg_build_graph _glp_cfg_build_graph
CFG *cfg_build_graph(void /* glp_prob */ *P);
/* build conflict graph */

#define cfg_find_clique _glp_cfg_find_clique
int cfg_find_clique(void /* glp_prob */ *P, CFG *G, int ind[],
      double *sum);
/* find maximum weight clique in conflict graph */

/***********************************************************************
*  cfg_create_graph - create conflict graph
*
*  This routine creates the conflict graph, which initially is empty,
*  and returns a pointer to the graph descriptor.
*
*  The parameter n specifies the number of *all* variables in MIP, for
*  which the conflict graph will be built.
*
*  The parameter nv_max specifies maximal number of vertices in the
*  conflict graph. It should be the double number of binary variables
*  in corresponding MIP. */

CFG *cfg_create_graph(int n, int nv_max)
{     CFG *G;
      xassert(n >= 0);
      xassert(0 <= nv_max && nv_max <= n + n);
      G = talloc(1, CFG);
      G->n = n;
      G->pos = talloc(1+n, int);
      memset(&G->pos[1], 0, n * sizeof(int));
      G->neg = talloc(1+n, int);
      memset(&G->neg[1], 0, n * sizeof(int));
      G->pool = dmp_create_pool();
      G->nv_max = nv_max;
      G->nv = 0;
      G->ref = talloc(1+nv_max, int);
      G->vptr = talloc(1+nv_max, CFGVLE *);
      G->cptr = talloc(1+nv_max, CFGCLE *);
      return G;
}

/***********************************************************************
*  cfg_add_clique - add clique to conflict graph
*
*  This routine adds a clique to the conflict graph.
*
*  The parameter size specifies the clique size, size >= 2. Note that
*  any edge can be considered as a clique of size 2.
*
*  The array ind specifies vertices constituting the clique in elements
*  ind[k], 1 <= k <= size:
*
*  ind[k] = +j means a vertex of the conflict graph that corresponds to
*  original binary variable x[j], 1 <= j <= n.
*
*  ind[k] = -j means a vertex of the conflict graph that corresponds to
*  complement of original binary variable x[j], 1 <= j <= n.
*
*  Note that if both vertices for x[j] and (1 - x[j]) have appeared in
*  the conflict graph, the routine automatically adds an edge incident
*  to these vertices. */

static void add_edge(CFG *G, int v, int w)
{     /* add clique of size 2 */
      DMP *pool = G->pool;
      int nv = G->nv;
      CFGVLE **vptr = G->vptr;
      CFGVLE *vle;
      xassert(1 <= v && v <= nv);
      xassert(1 <= w && w <= nv);
      xassert(v != w);
      vle = dmp_talloc(pool, CFGVLE);
      vle->v = w;
      vle->next = vptr[v];
      vptr[v] = vle;
      vle = dmp_talloc(pool, CFGVLE);
      vle->v = v;
      vle->next = vptr[w];
      vptr[w] = vle;
      return;
}

void cfg_add_clique(CFG *G, int size, const int ind[])
{     int n = G->n;
      int *pos = G->pos;
      int *neg = G->neg;
      DMP *pool = G->pool;
      int nv_max = G->nv_max;
      int *ref = G->ref;
      CFGVLE **vptr = G->vptr;
      CFGCLE **cptr = G->cptr;
      int j, k, v;
      xassert(2 <= size && size <= nv_max);
      /* add new vertices to the conflict graph */
      for (k = 1; k <= size; k++)
      {  j = ind[k];
         if (j > 0)
         {  /* vertex corresponds to x[j] */
            xassert(1 <= j && j <= n);
            if (pos[j] == 0)
            {  /* no such vertex exists; add it */
               v = pos[j] = ++(G->nv);
               xassert(v <= nv_max);
               ref[v] = j;
               vptr[v] = NULL;
               cptr[v] = NULL;
               if (neg[j] != 0)
               {  /* now both vertices for x[j] and (1 - x[j]) exist */
                  add_edge(G, v, neg[j]);
               }
            }
         }
         else
         {  /* vertex corresponds to (1 - x[j]) */
            j = -j;
            xassert(1 <= j && j <= n);
            if (neg[j] == 0)
            {  /* no such vertex exists; add it */
               v = neg[j] = ++(G->nv);
               xassert(v <= nv_max);
               ref[v] = j;
               vptr[v] = NULL;
               cptr[v] = NULL;
               if (pos[j] != 0)
               {  /* now both vertices for x[j] and (1 - x[j]) exist */
                  add_edge(G, v, pos[j]);
               }
            }
         }
      }
      /* add specified clique to the conflict graph */
      if (size == 2)
         add_edge(G,
            ind[1] > 0 ? pos[+ind[1]] : neg[-ind[1]],
            ind[2] > 0 ? pos[+ind[2]] : neg[-ind[2]]);
      else
      {  CFGVLE *vp, *vle;
         CFGCLE *cle;
         /* build list of clique vertices */
         vp = NULL;
         for (k = 1; k <= size; k++)
         {  vle = dmp_talloc(pool, CFGVLE);
            vle->v = ind[k] > 0 ? pos[+ind[k]] : neg[-ind[k]];
            vle->next = vp;
            vp = vle;
         }
         /* attach the clique to all its vertices */
         for (k = 1; k <= size; k++)
         {  cle = dmp_talloc(pool, CFGCLE);
            cle->vptr = vp;
            v = ind[k] > 0 ? pos[+ind[k]] : neg[-ind[k]];
            cle->next = cptr[v];
            cptr[v] = cle;
         }
      }
      return;
}

/***********************************************************************
*  cfg_get_adjacent - get vertices adjacent to specified vertex
*
*  This routine stores numbers of all vertices adjacent to specified
*  vertex v of the conflict graph in locations ind[1], ..., ind[len],
*  and returns len, 1 <= len <= nv-1, where nv is the total number of
*  vertices in the conflict graph.
*
*  Note that the conflict graph defined by this routine has neither
*  self-loops nor multiple edges. */

int cfg_get_adjacent(CFG *G, int v, int ind[])
{     int nv = G->nv;
      int *ref = G->ref;
      CFGVLE **vptr = G->vptr;
      CFGCLE **cptr = G->cptr;
      CFGVLE *vle;
      CFGCLE *cle;
      int k, w, len;
      xassert(1 <= v && v <= nv);
      len = 0;
      /* walk thru the list of adjacent vertices */
      for (vle = vptr[v]; vle != NULL; vle = vle->next)
      {  w = vle->v;
         xassert(1 <= w && w <= nv);
         xassert(w != v);
         if (ref[w] > 0)
         {  ind[++len] = w;
            ref[w] = -ref[w];
         }
      }
      /* walk thru the list of incident cliques */
      for (cle = cptr[v]; cle != NULL; cle = cle->next)
      {  /* walk thru the list of clique vertices */
         for (vle = cle->vptr; vle != NULL; vle = vle->next)
         {  w = vle->v;
            xassert(1 <= w && w <= nv);
            if (w != v && ref[w] > 0)
            {  ind[++len] = w;
               ref[w] = -ref[w];
            }
         }
      }
      xassert(1 <= len && len < nv);
      /* unmark vertices included in the resultant adjacency list */
      for (k = 1; k <= len; k++)
      {  w = ind[k];
         ref[w] = -ref[w];
      }
      return len;
}

/***********************************************************************
*  cfg_expand_clique - expand specified clique to maximal clique
*
*  Given some clique in the conflict graph this routine expands it to
*  a maximal clique by including in it new vertices.
*
*  On entry vertex indices constituting the initial clique should be
*  stored in locations c_ind[1], ..., c_ind[c_len], where c_len is the
*  initial clique size. On exit the routine stores new vertex indices
*  to locations c_ind[c_len+1], ..., c_ind[c_len'], where c_len' is the
*  size of the maximal clique found, and returns c_len'.
*
*  ALGORITHM
*
*  Let G = (V, E) be a graph, C within V be a current clique to be
*  expanded, and D within V \ C be a subset of vertices adjacent to all
*  vertices from C. On every iteration the routine chooses some vertex
*  v in D, includes it into C, and removes from D the vertex v as well
*  as all vertices not adjacent to v. Initially C is empty and D = V.
*  Iterations repeat until D becomes an empty set. Obviously, the final
*  set C is a maximal clique in G.
*
*  Now let C0 be an initial clique, and we want C0 to be a subset of
*  the final maximal clique C. To provide this condition the routine
*  starts constructing C by choosing only such vertices v in D, which
*  are in C0, until all vertices from C0 have been included in C. May
*  note that if on some iteration C0 \ C is non-empty (i.e. if not all
*  vertices from C0 have been included in C), C0 \ C is a subset of D,
*  because C0 is a clique. */

static int intersection(int d_len, int d_ind[], int d_pos[], int len,
      const int ind[])
{     /* compute intersection D := D inter W, where W is some specified
       * set of vertices */
      int k, t, v, new_len;
      /* walk thru vertices in W and mark vertices in D */
      for (t = 1; t <= len; t++)
      {  /* v in W */
         v = ind[t];
         /* determine position of v in D */
         k = d_pos[v];
         if (k != 0)
         {  /* v in D */
            xassert(d_ind[k] == v);
            /* mark v to keep it in D */
            d_ind[k] = -v;
         }
      }
      /* remove all unmarked vertices from D */
      new_len = 0;
      for (k = 1; k <= d_len; k++)
      {  /* v in D */
         v = d_ind[k];
         if (v < 0)
         {  /* v is marked; keep it */
            v = -v;
            new_len++;
            d_ind[new_len] = v;
            d_pos[v] = new_len;
         }
         else
         {  /* v is not marked; remove it */
            d_pos[v] = 0;
         }
      }
      return new_len;
}

int cfg_expand_clique(CFG *G, int c_len, int c_ind[])
{     int nv = G->nv;
      int d_len, *d_ind, *d_pos, len, *ind;
      int k, v;
      xassert(0 <= c_len && c_len <= nv);
      /* allocate working arrays */
      d_ind = talloc(1+nv, int);
      d_pos = talloc(1+nv, int);
      ind = talloc(1+nv, int);
      /* initialize C := 0, D := V */
      d_len = nv;
      for (k = 1; k <= nv; k++)
         d_ind[k] = d_pos[k] = k;
      /* expand C by vertices of specified initial clique C0 */
      for (k = 1; k <= c_len; k++)
      {  /* v in C0 */
         v = c_ind[k];
         xassert(1 <= v && v <= nv);
         /* since C0 is clique, v should be in D */
         xassert(d_pos[v] != 0);
         /* W := set of vertices adjacent to v */
         len = cfg_get_adjacent(G, v, ind);
         /* D := D inter W */
         d_len = intersection(d_len, d_ind, d_pos, len, ind);
         /* since v not in W, now v should be not in D */
         xassert(d_pos[v] == 0);
      }
      /* expand C by some other vertices until D is empty */
      while (d_len > 0)
      {  /* v in D */
         v = d_ind[1];
         xassert(1 <= v && v <= nv);
         /* note that v is adjacent to all vertices in C (by design),
          * so add v to C */
         c_ind[++c_len] = v;
         /* W := set of vertices adjacent to v */
         len = cfg_get_adjacent(G, v, ind);
         /* D := D inter W */
         d_len = intersection(d_len, d_ind, d_pos, len, ind);
         /* since v not in W, now v should be not in D */
         xassert(d_pos[v] == 0);
      }
      /* free working arrays */
      tfree(d_ind);
      tfree(d_pos);
      tfree(ind);
      /* bring maximal clique to calling routine */
      return c_len;
}

/***********************************************************************
*  cfg_check_clique - check clique in conflict graph
*
*  This routine checks that vertices of the conflict graph specified
*  in locations c_ind[1], ..., c_ind[c_len] constitute a clique.
*
*  NOTE: for testing/debugging only. */

void cfg_check_clique(CFG *G, int c_len, const int c_ind[])
{     int nv = G->nv;
      int k, kk, v, w, len, *ind;
      char *flag;
      ind = talloc(1+nv, int);
      flag = talloc(1+nv, char);
      memset(&flag[1], 0, nv);
      /* walk thru clique vertices */
      xassert(c_len >= 0);
      for (k = 1; k <= c_len; k++)
      {  /* get clique vertex v */
         v = c_ind[k];
         xassert(1 <= v && v <= nv);
         /* get vertices adjacent to vertex v */
         len = cfg_get_adjacent(G, v, ind);
         for (kk = 1; kk <= len; kk++)
         {  w = ind[kk];
            xassert(1 <= w && w <= nv);
            xassert(w != v);
            flag[w] = 1;
         }
         /* check that all clique vertices other than v are adjacent
            to v */
         for (kk = 1; kk <= c_len; kk++)
         {  w = c_ind[kk];
            xassert(1 <= w && w <= nv);
            if (w != v)
               xassert(flag[w]);
         }
         /* reset vertex flags */
         for (kk = 1; kk <= len; kk++)
            flag[ind[kk]] = 0;
      }
      tfree(ind);
      tfree(flag);
      return;
}

/***********************************************************************
*  cfg_delete_graph - delete conflict graph
*
*  This routine deletes the conflict graph by freeing all the memory
*  allocated to this program object. */

void cfg_delete_graph(CFG *G)
{     tfree(G->pos);
      tfree(G->neg);
      dmp_delete_pool(G->pool);
      tfree(G->ref);
      tfree(G->vptr);
      tfree(G->cptr);
      tfree(G);
      return;
}

















typedef struct IOSLOT IOSLOT;
typedef struct IOSNPD IOSNPD;
typedef struct IOSBND IOSBND;
typedef struct IOSTAT IOSTAT;
typedef struct IOSROW IOSROW;
typedef struct IOSAIJ IOSAIJ;
typedef struct IOSPOOL IOSPOOL;
typedef struct IOSCUT IOSCUT;

struct glp_tree
{     /* branch-and-bound tree */
      int magic;
      /* magic value used for debugging */
      DMP *pool;
      /* memory pool to store all IOS components */
      int n;
      /* number of columns (variables) */
      /*--------------------------------------------------------------*/
      /* problem components corresponding to the original MIP and its
         LP relaxation (used to restore the original problem object on
         exit from the solver) */
      int orig_m;
      /* number of rows */
      unsigned char *orig_type; /* uchar orig_type[1+orig_m+n]; */
      /* types of all variables */
      double *orig_lb; /* double orig_lb[1+orig_m+n]; */
      /* lower bounds of all variables */
      double *orig_ub; /* double orig_ub[1+orig_m+n]; */
      /* upper bounds of all variables */
      unsigned char *orig_stat; /* uchar orig_stat[1+orig_m+n]; */
      /* statuses of all variables */
      double *orig_prim; /* double orig_prim[1+orig_m+n]; */
      /* primal values of all variables */
      double *orig_dual; /* double orig_dual[1+orig_m+n]; */
      /* dual values of all variables */
      double orig_obj;
      /* optimal objective value for LP relaxation */
      /*--------------------------------------------------------------*/
      /* branch-and-bound tree */
      int nslots;
      /* length of the array of slots (enlarged automatically) */
      int avail;
      /* index of the first free slot; 0 means all slots are in use */
      IOSLOT *slot; /* IOSLOT slot[1+nslots]; */
      /* array of slots:
         slot[0] is not used;
         slot[p], 1 <= p <= nslots, either contains a pointer to some
         node of the branch-and-bound tree, in which case p is used on
         API level as the reference number of corresponding subproblem,
         or is free; all free slots are linked into single linked list;
         slot[1] always contains a pointer to the root node (it is free
         only if the tree is empty) */
      IOSNPD *head;
      /* pointer to the head of the active list */
      IOSNPD *tail;
      /* pointer to the tail of the active list */
      /* the active list is a doubly linked list of active subproblems
         which correspond to leaves of the tree; all subproblems in the
         active list are ordered chronologically (each a new subproblem
         is always added to the tail of the list) */
      int a_cnt;
      /* current number of active nodes (including the current one) */
      int n_cnt;
      /* current number of all (active and inactive) nodes */
      int t_cnt;
      /* total number of nodes including those which have been already
         removed from the tree; this count is increased by one whenever
         a new node is created and never decreased */
      /*--------------------------------------------------------------*/
      /* problem components corresponding to the root subproblem */
      int root_m;
      /* number of rows */
      unsigned char *root_type; /* uchar root_type[1+root_m+n]; */
      /* types of all variables */
      double *root_lb; /* double root_lb[1+root_m+n]; */
      /* lower bounds of all variables */
      double *root_ub; /* double root_ub[1+root_m+n]; */
      /* upper bounds of all variables */
      unsigned char *root_stat; /* uchar root_stat[1+root_m+n]; */
      /* statuses of all variables */
      /*--------------------------------------------------------------*/
      /* current subproblem and its LP relaxation */
      IOSNPD *curr;
      /* pointer to the current subproblem (which can be only active);
         NULL means the current subproblem does not exist */
      glp_prob *mip;
      /* original problem object passed to the solver; if the current
         subproblem exists, its LP segment corresponds to LP relaxation
         of the current subproblem; if the current subproblem does not
         exist, its LP segment corresponds to LP relaxation of the root
         subproblem (note that the root subproblem may differ from the
         original MIP, because it may be preprocessed and/or may have
         additional rows) */
      unsigned char *non_int; /* uchar non_int[1+n]; */
      /* these column flags are set each time when LP relaxation of the
         current subproblem has been solved;
         non_int[0] is not used;
         non_int[j], 1 <= j <= n, is j-th column flag; if this flag is
         set, corresponding variable is required to be integer, but its
         value in basic solution is fractional */
      /*--------------------------------------------------------------*/
      /* problem components corresponding to the parent (predecessor)
         subproblem for the current subproblem; used to inspect changes
         on freezing the current subproblem */
      int pred_m;
      /* number of rows */
      int pred_max;
      /* length of the following four arrays (enlarged automatically),
         pred_max >= pred_m + n */
      unsigned char *pred_type; /* uchar pred_type[1+pred_m+n]; */
      /* types of all variables */
      double *pred_lb; /* double pred_lb[1+pred_m+n]; */
      /* lower bounds of all variables */
      double *pred_ub; /* double pred_ub[1+pred_m+n]; */
      /* upper bounds of all variables */
      unsigned char *pred_stat; /* uchar pred_stat[1+pred_m+n]; */
      /* statuses of all variables */
      /****************************************************************/
      /* built-in cut generators segment */
      IOSPOOL *local;
      /* local cut pool */
#if 0 /* 06/III-2016 */
      void *mir_gen;
#else
      glp_mir *mir_gen;
#endif
      /* pointer to working area used by the MIR cut generator */
#if 0 /* 08/III-2016 */
      void *clq_gen;
      /* pointer to working area used by the clique cut generator */
#else
      glp_cfg *clq_gen;
      /* pointer to conflict graph used by the clique cut generator */
#endif
      /*--------------------------------------------------------------*/
      void *pcost;
      /* pointer to working area used on pseudocost branching */
      int *iwrk; /* int iwrk[1+n]; */
      /* working array */
      double *dwrk; /* double dwrk[1+n]; */
      /* working array */
      /*--------------------------------------------------------------*/
      /* control parameters and statistics */
      const glp_iocp *parm;
      /* copy of control parameters passed to the solver */
#if 0 /* 10/VI-2013 */
      glp_long tm_beg;
#else
      double tm_beg;
#endif
      /* starting time of the search, in seconds; the total time of the
         search is the difference between xtime() and tm_beg */
#if 0 /* 10/VI-2013 */
      glp_long tm_lag;
#else
      double tm_lag;
#endif
      /* the most recent time, in seconds, at which the progress of the
         the search was displayed */
      int sol_cnt;
      /* number of integer feasible solutions found */
#if 1 /* 11/VII-2013 */
      void *P; /* glp_prob *P; */
      /* problem passed to glp_intopt */
      void *npp; /* NPP *npp; */
      /* preprocessor workspace or NULL */
      const char *save_sol;
      /* filename (template) to save every new solution */
      int save_cnt;
      /* count to generate filename */
#endif
      /*--------------------------------------------------------------*/
      /* advanced solver interface */
      int reason;
      /* flag indicating the reason why the callback routine is being
         called (see glpk.h) */
      int stop;
      /* flag indicating that the callback routine requires premature
         termination of the search */
      int next_p;
      /* reference number of active subproblem selected to continue
         the search; 0 means no subproblem has been selected */
      int reopt;
      /* flag indicating that the current LP relaxation needs to be
         re-optimized */
      int reinv;
      /* flag indicating that some (non-active) rows were removed from
         the current LP relaxation, so if there no new rows appear, the
         basis must be re-factorized */
      int br_var;
      /* the number of variable chosen to branch on */
      int br_sel;
      /* flag indicating which branch (subproblem) is suggested to be
         selected to continue the search:
         GLP_DN_BRNCH - select down-branch
         GLP_UP_BRNCH - select up-branch
         GLP_NO_BRNCH - use general selection technique */
      int child;
      /* subproblem reference number corresponding to br_sel */
};

struct IOSLOT
{     /* node subproblem slot */
      IOSNPD *node;
      /* pointer to subproblem descriptor; NULL means free slot */
      int next;
      /* index of another free slot (only if this slot is free) */
};

struct IOSNPD
{     /* node subproblem descriptor */
      int p;
      /* subproblem reference number (it is the index to corresponding
         slot, i.e. slot[p] points to this descriptor) */
      IOSNPD *up;
      /* pointer to the parent subproblem; NULL means this node is the
         root of the tree, in which case p = 1 */
      int level;
      /* node level (the root node has level 0) */
      int count;
      /* if count = 0, this subproblem is active; if count > 0, this
         subproblem is inactive, in which case count is the number of
         its child subproblems */
      /* the following three linked lists are destroyed on reviving and
         built anew on freezing the subproblem: */
      IOSBND *b_ptr;
      /* linked list of rows and columns of the parent subproblem whose
         types and bounds were changed */
      IOSTAT *s_ptr;
      /* linked list of rows and columns of the parent subproblem whose
         statuses were changed */
      IOSROW *r_ptr;
      /* linked list of rows (cuts) added to the parent subproblem */
      int solved;
      /* how many times LP relaxation of this subproblem was solved;
         for inactive subproblem this count is always non-zero;
         for active subproblem, which is not current, this count may be
         non-zero, if the subproblem was temporarily suspended */
      double lp_obj;
      /* optimal objective value to LP relaxation of this subproblem;
         on creating a subproblem this value is inherited from its
         parent; for the root subproblem, which has no parent, this
         value is initially set to -DBL_MAX (minimization) or +DBL_MAX
         (maximization); each time the subproblem is re-optimized, this
         value is appropriately changed */
      double bound;
      /* local lower (minimization) or upper (maximization) bound for
         integer optimal solution to *this* subproblem; this bound is
         local in the sense that only subproblems in the subtree rooted
         at this node cannot have better integer feasible solutions;
         on creating a subproblem its local bound is inherited from its
         parent and then can be made stronger (never weaker); for the
         root subproblem its local bound is initially set to -DBL_MAX
         (minimization) or +DBL_MAX (maximization) and then improved as
         the root LP relaxation has been solved */
      /* the following two quantities are defined only if LP relaxation
         of this subproblem was solved at least once (solved > 0): */
      int ii_cnt;
      /* number of integer variables whose value in optimal solution to
         LP relaxation of this subproblem is fractional */
      double ii_sum;
      /* sum of integer infeasibilities */
#if 1 /* 30/XI-2009 */
      int changed;
      /* how many times this subproblem was re-formulated (by adding
         cutting plane constraints) */
#endif
      int br_var;
      /* ordinal number of branching variable, 1 <= br_var <= n, used
         to split this subproblem; 0 means that either this subproblem
         is active or branching was made on a constraint */
      double br_val;
      /* (fractional) value of branching variable in optimal solution
         to final LP relaxation of this subproblem */
      void *data; /* char data[tree->cb_size]; */
      /* pointer to the application-specific data */
      IOSNPD *temp;
      /* working pointer used by some routines */
      IOSNPD *prev;
      /* pointer to previous subproblem in the active list */
      IOSNPD *next;
      /* pointer to next subproblem in the active list */
};

struct IOSBND
{     /* bounds change entry */
      int k;
      /* ordinal number of corresponding row (1 <= k <= m) or column
         (m+1 <= k <= m+n), where m and n are the number of rows and
         columns, resp., in the parent subproblem */
      unsigned char type;
      /* new type */
      double lb;
      /* new lower bound */
      double ub;
      /* new upper bound */
      IOSBND *next;
      /* pointer to next entry for the same subproblem */
};

struct IOSTAT
{     /* status change entry */
      int k;
      /* ordinal number of corresponding row (1 <= k <= m) or column
         (m+1 <= k <= m+n), where m and n are the number of rows and
         columns, resp., in the parent subproblem */
      unsigned char stat;
      /* new status */
      IOSTAT *next;
      /* pointer to next entry for the same subproblem */
};

struct IOSROW
{     /* row (constraint) addition entry */
      char *name;
      /* row name or NULL */
      unsigned char origin;
      /* row origin flag (see glp_attr.origin) */
      unsigned char klass;
      /* row class descriptor (see glp_attr.klass) */
      unsigned char type;
      /* row type (GLP_LO, GLP_UP, etc.) */
      double lb;
      /* row lower bound */
      double ub;
      /* row upper bound */
      IOSAIJ *ptr;
      /* pointer to the row coefficient list */
      double rii;
      /* row scale factor */
      unsigned char stat;
      /* row status (GLP_BS, GLP_NL, etc.) */
      IOSROW *next;
      /* pointer to next entry for the same subproblem */
};

struct IOSAIJ
{     /* constraint coefficient */
      int j;
      /* variable (column) number, 1 <= j <= n */
      double val;
      /* non-zero coefficient value */
      IOSAIJ *next;
      /* pointer to next coefficient for the same row */
};

struct IOSPOOL
{     /* cut pool */
      int size;
      /* pool size = number of cuts in the pool */
      IOSCUT *head;
      /* pointer to the first cut */
      IOSCUT *tail;
      /* pointer to the last cut */
      int ord;
      /* ordinal number of the current cut, 1 <= ord <= size */
      IOSCUT *curr;
      /* pointer to the current cut */
};

struct IOSCUT
{     /* cut (cutting plane constraint) */
      char *name;
      /* cut name or NULL */
      unsigned char klass;
      /* cut class descriptor (see glp_attr.klass) */
      IOSAIJ *ptr;
      /* pointer to the cut coefficient list */
      unsigned char type;
      /* cut type:
         GLP_LO: sum a[j] * x[j] >= b
         GLP_UP: sum a[j] * x[j] <= b
         GLP_FX: sum a[j] * x[j]  = b */
      double rhs;
      /* cut right-hand side */
      IOSCUT *prev;
      /* pointer to previous cut */
      IOSCUT *next;
      /* pointer to next cut */
};

#define ios_create_tree _glp_ios_create_tree
glp_tree *ios_create_tree(glp_prob *mip, const glp_iocp *parm);
/* create branch-and-bound tree */

#define ios_revive_node _glp_ios_revive_node
void ios_revive_node(glp_tree *tree, int p);
/* revive specified subproblem */

#define ios_freeze_node _glp_ios_freeze_node
void ios_freeze_node(glp_tree *tree);
/* freeze current subproblem */

#define ios_clone_node _glp_ios_clone_node
void ios_clone_node(glp_tree *tree, int p, int nnn, int ref[]);
/* clone specified subproblem */

#define ios_delete_node _glp_ios_delete_node
void ios_delete_node(glp_tree *tree, int p);
/* delete specified subproblem */

#define ios_delete_tree _glp_ios_delete_tree
void ios_delete_tree(glp_tree *tree);
/* delete branch-and-bound tree */

#define ios_eval_degrad _glp_ios_eval_degrad
void ios_eval_degrad(glp_tree *tree, int j, double *dn, double *up);
/* estimate obj. degrad. for down- and up-branches */

#define ios_round_bound _glp_ios_round_bound
double ios_round_bound(glp_tree *tree, double bound);
/* improve local bound by rounding */

#define ios_is_hopeful _glp_ios_is_hopeful
int ios_is_hopeful(glp_tree *tree, double bound);
/* check if subproblem is hopeful */

#define ios_best_node _glp_ios_best_node
int ios_best_node(glp_tree *tree);
/* find active node with best local bound */

#define ios_relative_gap _glp_ios_relative_gap
double ios_relative_gap(glp_tree *tree);
/* compute relative mip gap */

#define ios_solve_node _glp_ios_solve_node
int ios_solve_node(glp_tree *tree);
/* solve LP relaxation of current subproblem */

#define ios_create_pool _glp_ios_create_pool
IOSPOOL *ios_create_pool(glp_tree *tree);
/* create cut pool */

#define ios_add_row _glp_ios_add_row
int ios_add_row(glp_tree *tree, IOSPOOL *pool,
      const char *name, int klass, int flags, int len, const int ind[],
      const double val[], int type, double rhs);
/* add row (constraint) to the cut pool */

#define ios_find_row _glp_ios_find_row
IOSCUT *ios_find_row(IOSPOOL *pool, int i);
/* find row (constraint) in the cut pool */

#define ios_del_row _glp_ios_del_row
void ios_del_row(glp_tree *tree, IOSPOOL *pool, int i);
/* remove row (constraint) from the cut pool */

#define ios_clear_pool _glp_ios_clear_pool
void ios_clear_pool(glp_tree *tree, IOSPOOL *pool);
/* remove all rows (constraints) from the cut pool */

#define ios_delete_pool _glp_ios_delete_pool
void ios_delete_pool(glp_tree *tree, IOSPOOL *pool);
/* delete cut pool */

#if 1 /* 11/VII-2013 */
#define ios_process_sol _glp_ios_process_sol
void ios_process_sol(glp_tree *T);
/* process integer feasible solution just found */
#endif

#define ios_preprocess_node _glp_ios_preprocess_node
int ios_preprocess_node(glp_tree *tree, int max_pass);
/* preprocess current subproblem */

#define ios_driver _glp_ios_driver
int ios_driver(glp_tree *tree);
/* branch-and-bound driver */

#define ios_cov_gen _glp_ios_cov_gen
void ios_cov_gen(glp_tree *tree);
/* generate mixed cover cuts */

#define ios_pcost_init _glp_ios_pcost_init
void *ios_pcost_init(glp_tree *tree);
/* initialize working data used on pseudocost branching */

#define ios_pcost_branch _glp_ios_pcost_branch
int ios_pcost_branch(glp_tree *T, int *next);
/* choose branching variable with pseudocost branching */

#define ios_pcost_update _glp_ios_pcost_update
void ios_pcost_update(glp_tree *tree);
/* update history information for pseudocost branching */

#define ios_pcost_free _glp_ios_pcost_free
void ios_pcost_free(glp_tree *tree);
/* free working area used on pseudocost branching */

#define ios_feas_pump _glp_ios_feas_pump
void ios_feas_pump(glp_tree *T);
/* feasibility pump heuristic */

#if 1 /* 25/V-2013 */
#define ios_proxy_heur _glp_ios_proxy_heur
void ios_proxy_heur(glp_tree *T);
/* proximity search heuristic */
#endif

#define ios_process_cuts _glp_ios_process_cuts
void ios_process_cuts(glp_tree *T);
/* process cuts stored in the local cut pool */

#define ios_choose_node _glp_ios_choose_node
int ios_choose_node(glp_tree *T);
/* select subproblem to continue the search */

#define ios_choose_var _glp_ios_choose_var
int ios_choose_var(glp_tree *T, int *next);
/* select variable to branch on */

/* CAUTION: DO NOT CHANGE THE LIMITS BELOW */

#define M_MAX 100000000 /* = 100*10^6 */
/* maximal number of rows in the problem object */

#define N_MAX 100000000 /* = 100*10^6 */
/* maximal number of columns in the problem object */

#define NNZ_MAX 500000000 /* = 500*10^6 */
/* maximal number of constraint coefficients in the problem object */

/***********************************************************************
*  NAME
*
*  glp_create_prob - create problem object
*
*  SYNOPSIS
*
*  glp_prob *glp_create_prob(void);
*
*  DESCRIPTION
*
*  The routine glp_create_prob creates a new problem object, which is
*  initially "empty", i.e. has no rows and columns.
*
*  RETURNS
*
*  The routine returns a pointer to the object created, which should be
*  used in any subsequent operations on this object. */

static void create_prob(glp_prob *lp)
{     lp->magic = GLP_PROB_MAGIC;
      lp->pool = dmp_create_pool();
#if 0 /* 08/III-2014 */
#if 0 /* 17/XI-2009 */
      lp->cps = xmalloc(sizeof(struct LPXCPS));
      lpx_reset_parms(lp);
#else
      lp->parms = NULL;
#endif
#endif
      lp->tree = NULL;
#if 0
      lp->lwa = 0;
      lp->cwa = NULL;
#endif
      /* LP/MIP data */
      lp->name = NULL;
      lp->obj = NULL;
      lp->dir = GLP_MIN;
      lp->c0 = 0.0;
      lp->m_max = 100;
      lp->n_max = 200;
      lp->m = lp->n = 0;
      lp->nnz = 0;
      lp->row = xcalloc(1+lp->m_max, sizeof(GLPROW *));
      lp->col = xcalloc(1+lp->n_max, sizeof(GLPCOL *));
      lp->r_tree = lp->c_tree = NULL;
      /* basis factorization */
      lp->valid = 0;
      lp->head = xcalloc(1+lp->m_max, sizeof(int));
#if 0 /* 08/III-2014 */
      lp->bfcp = NULL;
#endif
      lp->bfd = NULL;
      /* basic solution (LP) */
      lp->pbs_stat = lp->dbs_stat = GLP_UNDEF;
      lp->obj_val = 0.0;
      lp->it_cnt = 0;
      lp->some = 0;
      /* interior-point solution (LP) */
      lp->ipt_stat = GLP_UNDEF;
      lp->ipt_obj = 0.0;
      /* integer solution (MIP) */
      lp->mip_stat = GLP_UNDEF;
      lp->mip_obj = 0.0;
      return;
}

glp_prob *glp_create_prob(void)
{     glp_prob *lp;
      lp = xmalloc(sizeof(glp_prob));
      create_prob(lp);
      return lp;
}

/***********************************************************************
*  NAME
*
*  glp_set_prob_name - assign (change) problem name
*
*  SYNOPSIS
*
*  void glp_set_prob_name(glp_prob *lp, const char *name);
*
*  DESCRIPTION
*
*  The routine glp_set_prob_name assigns a given symbolic name (1 up to
*  255 characters) to the specified problem object.
*
*  If the parameter name is NULL or empty string, the routine erases an
*  existing symbolic name of the problem object. */

void glp_set_prob_name(glp_prob *lp, const char *name)
{     glp_tree *tree = lp->tree;
      if (tree != NULL && tree->reason != 0)
         xerror("glp_set_prob_name: operation not allowed\n");
      if (lp->name != NULL)
      {  dmp_free_atom(lp->pool, lp->name, strlen(lp->name)+1);
         lp->name = NULL;
      }
      if (!(name == NULL || name[0] == '\0'))
      {  int k;
         for (k = 0; name[k] != '\0'; k++)
         {  if (k == 256)
               xerror("glp_set_prob_name: problem name too long\n");
            if (iscntrl((unsigned char)name[k]))
               xerror("glp_set_prob_name: problem name contains invalid"
                  " character(s)\n");
         }
         lp->name = dmp_get_atom(lp->pool, strlen(name)+1);
         strcpy(lp->name, name);
      }
      return;
}

/***********************************************************************
*  NAME
*
*  glp_set_obj_name - assign (change) objective function name
*
*  SYNOPSIS
*
*  void glp_set_obj_name(glp_prob *lp, const char *name);
*
*  DESCRIPTION
*
*  The routine glp_set_obj_name assigns a given symbolic name (1 up to
*  255 characters) to the objective function of the specified problem
*  object.
*
*  If the parameter name is NULL or empty string, the routine erases an
*  existing name of the objective function. */

void glp_set_obj_name(glp_prob *lp, const char *name)
{     glp_tree *tree = lp->tree;
      if (tree != NULL && tree->reason != 0)
         xerror("glp_set_obj_name: operation not allowed\n");
     if (lp->obj != NULL)
      {  dmp_free_atom(lp->pool, lp->obj, strlen(lp->obj)+1);
         lp->obj = NULL;
      }
      if (!(name == NULL || name[0] == '\0'))
      {  int k;
         for (k = 0; name[k] != '\0'; k++)
         {  if (k == 256)
               xerror("glp_set_obj_name: objective name too long\n");
            if (iscntrl((unsigned char)name[k]))
               xerror("glp_set_obj_name: objective name contains invali"
                  "d character(s)\n");
         }
         lp->obj = dmp_get_atom(lp->pool, strlen(name)+1);
         strcpy(lp->obj, name);
      }
      return;
}

/***********************************************************************
*  NAME
*
*  glp_set_obj_dir - set (change) optimization direction flag
*
*  SYNOPSIS
*
*  void glp_set_obj_dir(glp_prob *lp, int dir);
*
*  DESCRIPTION
*
*  The routine glp_set_obj_dir sets (changes) optimization direction
*  flag (i.e. "sense" of the objective function) as specified by the
*  parameter dir:
*
*  GLP_MIN - minimization;
*  GLP_MAX - maximization. */

void glp_set_obj_dir(glp_prob *lp, int dir)
{     glp_tree *tree = lp->tree;
      if (tree != NULL && tree->reason != 0)
         xerror("glp_set_obj_dir: operation not allowed\n");
     if (!(dir == GLP_MIN || dir == GLP_MAX))
         xerror("glp_set_obj_dir: dir = %d; invalid direction flag\n",
            dir);
      lp->dir = dir;
      return;
}

/***********************************************************************
*  NAME
*
*  glp_add_rows - add new rows to problem object
*
*  SYNOPSIS
*
*  int glp_add_rows(glp_prob *lp, int nrs);
*
*  DESCRIPTION
*
*  The routine glp_add_rows adds nrs rows (constraints) to the specified
*  problem object. New rows are always added to the end of the row list,
*  so the ordinal numbers of existing rows remain unchanged.
*
*  Being added each new row is initially free (unbounded) and has empty
*  list of the constraint coefficients.
*
*  RETURNS
*
*  The routine glp_add_rows returns the ordinal number of the first new
*  row added to the problem object. */

int glp_add_rows(glp_prob *lp, int nrs)
{     glp_tree *tree = lp->tree;
      GLPROW *row;
      int m_new, i;
      /* determine new number of rows */
      if (nrs < 1)
         xerror("glp_add_rows: nrs = %d; invalid number of rows\n",
            nrs);
      if (nrs > M_MAX - lp->m)
         xerror("glp_add_rows: nrs = %d; too many rows\n", nrs);
      m_new = lp->m + nrs;
      /* increase the room, if necessary */
      if (lp->m_max < m_new)
      {  GLPROW **save = lp->row;
         while (lp->m_max < m_new)
         {  lp->m_max += lp->m_max;
            xassert(lp->m_max > 0);
         }
         lp->row = xcalloc(1+lp->m_max, sizeof(GLPROW *));
         memcpy(&lp->row[1], &save[1], lp->m * sizeof(GLPROW *));
         xfree(save);
         /* do not forget about the basis header */
         xfree(lp->head);
         lp->head = xcalloc(1+lp->m_max, sizeof(int));
      }
      /* add new rows to the end of the row list */
      for (i = lp->m+1; i <= m_new; i++)
      {  /* create row descriptor */
         lp->row[i] = row = dmp_get_atom(lp->pool, sizeof(GLPROW));
         row->i = i;
         row->name = NULL;
         row->node = NULL;
#if 1 /* 20/IX-2008 */
         row->level = 0;
         row->origin = 0;
         row->klass = 0;
         if (tree != NULL)
         {  switch (tree->reason)
            {  case 0:
                  break;
               case GLP_IROWGEN:
                  xassert(tree->curr != NULL);
                  row->level = tree->curr->level;
                  row->origin = GLP_RF_LAZY;
                  break;
               case GLP_ICUTGEN:
                  xassert(tree->curr != NULL);
                  row->level = tree->curr->level;
                  row->origin = GLP_RF_CUT;
                  break;
               default:
                  xassert(tree != tree);
            }
         }
#endif
         row->type = GLP_FR;
         row->lb = row->ub = 0.0;
         row->ptr = NULL;
         row->rii = 1.0;
         row->stat = GLP_BS;
#if 0
         row->bind = -1;
#else
         row->bind = 0;
#endif
         row->prim = row->dual = 0.0;
         row->pval = row->dval = 0.0;
         row->mipx = 0.0;
      }
      /* set new number of rows */
      lp->m = m_new;
      /* invalidate the basis factorization */
      lp->valid = 0;
#if 1
      if (tree != NULL && tree->reason != 0) tree->reopt = 1;
#endif
      /* return the ordinal number of the first row added */
      return m_new - nrs + 1;
}

/***********************************************************************
*  NAME
*
*  glp_add_cols - add new columns to problem object
*
*  SYNOPSIS
*
*  int glp_add_cols(glp_prob *lp, int ncs);
*
*  DESCRIPTION
*
*  The routine glp_add_cols adds ncs columns (structural variables) to
*  the specified problem object. New columns are always added to the end
*  of the column list, so the ordinal numbers of existing columns remain
*  unchanged.
*
*  Being added each new column is initially fixed at zero and has empty
*  list of the constraint coefficients.
*
*  RETURNS
*
*  The routine glp_add_cols returns the ordinal number of the first new
*  column added to the problem object. */

int glp_add_cols(glp_prob *lp, int ncs)
{     glp_tree *tree = lp->tree;
      GLPCOL *col;
      int n_new, j;
      if (tree != NULL && tree->reason != 0)
         xerror("glp_add_cols: operation not allowed\n");
      /* determine new number of columns */
      if (ncs < 1)
         xerror("glp_add_cols: ncs = %d; invalid number of columns\n",
            ncs);
      if (ncs > N_MAX - lp->n)
         xerror("glp_add_cols: ncs = %d; too many columns\n", ncs);
      n_new = lp->n + ncs;
      /* increase the room, if necessary */
      if (lp->n_max < n_new)
      {  GLPCOL **save = lp->col;
         while (lp->n_max < n_new)
         {  lp->n_max += lp->n_max;
            xassert(lp->n_max > 0);
         }
         lp->col = xcalloc(1+lp->n_max, sizeof(GLPCOL *));
         memcpy(&lp->col[1], &save[1], lp->n * sizeof(GLPCOL *));
         xfree(save);
      }
      /* add new columns to the end of the column list */
      for (j = lp->n+1; j <= n_new; j++)
      {  /* create column descriptor */
         lp->col[j] = col = dmp_get_atom(lp->pool, sizeof(GLPCOL));
         col->j = j;
         col->name = NULL;
         col->node = NULL;
         col->kind = GLP_CV;
         col->type = GLP_FX;
         col->lb = col->ub = 0.0;
         col->coef = 0.0;
         col->ptr = NULL;
         col->sjj = 1.0;
         col->stat = GLP_NS;
#if 0
         col->bind = -1;
#else
         col->bind = 0; /* the basis may remain valid */
#endif
         col->prim = col->dual = 0.0;
         col->pval = col->dval = 0.0;
         col->mipx = 0.0;
      }
      /* set new number of columns */
      lp->n = n_new;
      /* return the ordinal number of the first column added */
      return n_new - ncs + 1;
}

/***********************************************************************
*  NAME
*
*  glp_set_row_name - assign (change) row name
*
*  SYNOPSIS
*
*  void glp_set_row_name(glp_prob *lp, int i, const char *name);
*
*  DESCRIPTION
*
*  The routine glp_set_row_name assigns a given symbolic name (1 up to
*  255 characters) to i-th row (auxiliary variable) of the specified
*  problem object.
*
*  If the parameter name is NULL or empty string, the routine erases an
*  existing name of i-th row. */

void glp_set_row_name(glp_prob *lp, int i, const char *name)
{     glp_tree *tree = lp->tree;
      GLPROW *row;
      if (!(1 <= i && i <= lp->m))
         xerror("glp_set_row_name: i = %d; row number out of range\n",
            i);
      row = lp->row[i];
      if (tree != NULL && tree->reason != 0)
      {  xassert(tree->curr != NULL);
         xassert(row->level == tree->curr->level);
      }
      if (row->name != NULL)
      {  if (row->node != NULL)
         {  xassert(lp->r_tree != NULL);
            avl_delete_node(lp->r_tree, row->node);
            row->node = NULL;
         }
         dmp_free_atom(lp->pool, row->name, strlen(row->name)+1);
         row->name = NULL;
      }
      if (!(name == NULL || name[0] == '\0'))
      {  int k;
         for (k = 0; name[k] != '\0'; k++)
         {  if (k == 256)
               xerror("glp_set_row_name: i = %d; row name too long\n",
                  i);
            if (iscntrl((unsigned char)name[k]))
               xerror("glp_set_row_name: i = %d: row name contains inva"
                  "lid character(s)\n", i);
         }
         row->name = dmp_get_atom(lp->pool, strlen(name)+1);
         strcpy(row->name, name);
         if (lp->r_tree != NULL)
         {  xassert(row->node == NULL);
            row->node = avl_insert_node(lp->r_tree, row->name);
            avl_set_node_link(row->node, row);
         }
      }
      return;
}

/***********************************************************************
*  NAME
*
*  glp_set_col_name - assign (change) column name
*
*  SYNOPSIS
*
*  void glp_set_col_name(glp_prob *lp, int j, const char *name);
*
*  DESCRIPTION
*
*  The routine glp_set_col_name assigns a given symbolic name (1 up to
*  255 characters) to j-th column (structural variable) of the specified
*  problem object.
*
*  If the parameter name is NULL or empty string, the routine erases an
*  existing name of j-th column. */

void glp_set_col_name(glp_prob *lp, int j, const char *name)
{     glp_tree *tree = lp->tree;
      GLPCOL *col;
      if (tree != NULL && tree->reason != 0)
         xerror("glp_set_col_name: operation not allowed\n");
      if (!(1 <= j && j <= lp->n))
         xerror("glp_set_col_name: j = %d; column number out of range\n"
            , j);
      col = lp->col[j];
      if (col->name != NULL)
      {  if (col->node != NULL)
         {  xassert(lp->c_tree != NULL);
            avl_delete_node(lp->c_tree, col->node);
            col->node = NULL;
         }
         dmp_free_atom(lp->pool, col->name, strlen(col->name)+1);
         col->name = NULL;
      }
      if (!(name == NULL || name[0] == '\0'))
      {  int k;
         for (k = 0; name[k] != '\0'; k++)
         {  if (k == 256)
               xerror("glp_set_col_name: j = %d; column name too long\n"
                  , j);
            if (iscntrl((unsigned char)name[k]))
               xerror("glp_set_col_name: j = %d: column name contains i"
                  "nvalid character(s)\n", j);
         }
         col->name = dmp_get_atom(lp->pool, strlen(name)+1);
         strcpy(col->name, name);
         if (lp->c_tree != NULL && col->name != NULL)
         {  xassert(col->node == NULL);
            col->node = avl_insert_node(lp->c_tree, col->name);
            avl_set_node_link(col->node, col);
         }
      }
      return;
}

/***********************************************************************
*  NAME
*
*  glp_set_row_bnds - set (change) row bounds
*
*  SYNOPSIS
*
*  void glp_set_row_bnds(glp_prob *lp, int i, int type, double lb,
*     double ub);
*
*  DESCRIPTION
*
*  The routine glp_set_row_bnds sets (changes) the type and bounds of
*  i-th row (auxiliary variable) of the specified problem object.
*
*  Parameters type, lb, and ub specify the type, lower bound, and upper
*  bound, respectively, as follows:
*
*     Type           Bounds        Comments
*     ------------------------------------------------------
*     GLP_FR   -inf <  x <  +inf   Free variable
*     GLP_LO     lb <= x <  +inf   Variable with lower bound
*     GLP_UP   -inf <  x <=  ub    Variable with upper bound
*     GLP_DB     lb <= x <=  ub    Double-bounded variable
*     GLP_FX           x  =  lb    Fixed variable
*
*  where x is the auxiliary variable associated with i-th row.
*
*  If the row has no lower bound, the parameter lb is ignored. If the
*  row has no upper bound, the parameter ub is ignored. If the row is
*  an equality constraint (i.e. the corresponding auxiliary variable is
*  of fixed type), only the parameter lb is used while the parameter ub
*  is ignored. */

void glp_set_row_bnds(glp_prob *lp, int i, int type, double lb,
      double ub)
{     GLPROW *row;
      if (!(1 <= i && i <= lp->m))
         xerror("glp_set_row_bnds: i = %d; row number out of range\n",
            i);
      row = lp->row[i];
      row->type = type;
      switch (type)
      {  case GLP_FR:
            row->lb = row->ub = 0.0;
            if (row->stat != GLP_BS) row->stat = GLP_NF;
            break;
         case GLP_LO:
            row->lb = lb, row->ub = 0.0;
            if (row->stat != GLP_BS) row->stat = GLP_NL;
            break;
         case GLP_UP:
            row->lb = 0.0, row->ub = ub;
            if (row->stat != GLP_BS) row->stat = GLP_NU;
            break;
         case GLP_DB:
            row->lb = lb, row->ub = ub;
            if (!(row->stat == GLP_BS ||
                  row->stat == GLP_NL || row->stat == GLP_NU))
               row->stat = (fabs(lb) <= fabs(ub) ? GLP_NL : GLP_NU);
            break;
         case GLP_FX:
            row->lb = row->ub = lb;
            if (row->stat != GLP_BS) row->stat = GLP_NS;
            break;
         default:
            xerror("glp_set_row_bnds: i = %d; type = %d; invalid row ty"
               "pe\n", i, type);
      }
      return;
}

/***********************************************************************
*  NAME
*
*  glp_set_col_bnds - set (change) column bounds
*
*  SYNOPSIS
*
*  void glp_set_col_bnds(glp_prob *lp, int j, int type, double lb,
*     double ub);
*
*  DESCRIPTION
*
*  The routine glp_set_col_bnds sets (changes) the type and bounds of
*  j-th column (structural variable) of the specified problem object.
*
*  Parameters type, lb, and ub specify the type, lower bound, and upper
*  bound, respectively, as follows:
*
*     Type           Bounds        Comments
*     ------------------------------------------------------
*     GLP_FR   -inf <  x <  +inf   Free variable
*     GLP_LO     lb <= x <  +inf   Variable with lower bound
*     GLP_UP   -inf <  x <=  ub    Variable with upper bound
*     GLP_DB     lb <= x <=  ub    Double-bounded variable
*     GLP_FX           x  =  lb    Fixed variable
*
*  where x is the structural variable associated with j-th column.
*
*  If the column has no lower bound, the parameter lb is ignored. If the
*  column has no upper bound, the parameter ub is ignored. If the column
*  is of fixed type, only the parameter lb is used while the parameter
*  ub is ignored. */

void glp_set_col_bnds(glp_prob *lp, int j, int type, double lb,
      double ub)
{     GLPCOL *col;
      if (!(1 <= j && j <= lp->n))
         xerror("glp_set_col_bnds: j = %d; column number out of range\n"
            , j);
      col = lp->col[j];
      col->type = type;
      switch (type)
      {  case GLP_FR:
            col->lb = col->ub = 0.0;
            if (col->stat != GLP_BS) col->stat = GLP_NF;
            break;
         case GLP_LO:
            col->lb = lb, col->ub = 0.0;
            if (col->stat != GLP_BS) col->stat = GLP_NL;
            break;
         case GLP_UP:
            col->lb = 0.0, col->ub = ub;
            if (col->stat != GLP_BS) col->stat = GLP_NU;
            break;
         case GLP_DB:
            col->lb = lb, col->ub = ub;
            if (!(col->stat == GLP_BS ||
                  col->stat == GLP_NL || col->stat == GLP_NU))
               col->stat = (fabs(lb) <= fabs(ub) ? GLP_NL : GLP_NU);
            break;
         case GLP_FX:
            col->lb = col->ub = lb;
            if (col->stat != GLP_BS) col->stat = GLP_NS;
            break;
         default:
            xerror("glp_set_col_bnds: j = %d; type = %d; invalid column"
               " type\n", j, type);
      }
      return;
}

/***********************************************************************
*  NAME
*
*  glp_set_obj_coef - set (change) obj. coefficient or constant term
*
*  SYNOPSIS
*
*  void glp_set_obj_coef(glp_prob *lp, int j, double coef);
*
*  DESCRIPTION
*
*  The routine glp_set_obj_coef sets (changes) objective coefficient at
*  j-th column (structural variable) of the specified problem object.
*
*  If the parameter j is 0, the routine sets (changes) the constant term
*  ("shift") of the objective function. */

void glp_set_obj_coef(glp_prob *lp, int j, double coef)
{     glp_tree *tree = lp->tree;
      if (tree != NULL && tree->reason != 0)
         xerror("glp_set_obj_coef: operation not allowed\n");
      if (!(0 <= j && j <= lp->n))
         xerror("glp_set_obj_coef: j = %d; column number out of range\n"
            , j);
      if (j == 0)
         lp->c0 = coef;
      else
         lp->col[j]->coef = coef;
      return;
}

/***********************************************************************
*  NAME
*
*  glp_set_mat_row - set (replace) row of the constraint matrix
*
*  SYNOPSIS
*
*  void glp_set_mat_row(glp_prob *lp, int i, int len, const int ind[],
*     const double val[]);
*
*  DESCRIPTION
*
*  The routine glp_set_mat_row stores (replaces) the contents of i-th
*  row of the constraint matrix of the specified problem object.
*
*  Column indices and numeric values of new row elements must be placed
*  in locations ind[1], ..., ind[len] and val[1], ..., val[len], where
*  0 <= len <= n is the new length of i-th row, n is the current number
*  of columns in the problem object. Elements with identical column
*  indices are not allowed. Zero elements are allowed, but they are not
*  stored in the constraint matrix.
*
*  If the parameter len is zero, the parameters ind and/or val can be
*  specified as NULL. */

void glp_set_mat_row(glp_prob *lp, int i, int len, const int ind[],
      const double val[])
{     glp_tree *tree = lp->tree;
      GLPROW *row;
      GLPCOL *col;
      GLPAIJ *aij, *next;
      int j, k;
      /* obtain pointer to i-th row */
      if (!(1 <= i && i <= lp->m))
         xerror("glp_set_mat_row: i = %d; row number out of range\n",
            i);
      row = lp->row[i];
      if (tree != NULL && tree->reason != 0)
      {  xassert(tree->curr != NULL);
         xassert(row->level == tree->curr->level);
      }
      /* remove all existing elements from i-th row */
      while (row->ptr != NULL)
      {  /* take next element in the row */
         aij = row->ptr;
         /* remove the element from the row list */
         row->ptr = aij->r_next;
         /* obtain pointer to corresponding column */
         col = aij->col;
         /* remove the element from the column list */
         if (aij->c_prev == NULL)
            col->ptr = aij->c_next;
         else
            aij->c_prev->c_next = aij->c_next;
         if (aij->c_next == NULL)
            ;
         else
            aij->c_next->c_prev = aij->c_prev;
         /* return the element to the memory pool */
         dmp_free_atom(lp->pool, aij, sizeof(GLPAIJ)), lp->nnz--;
         /* if the corresponding column is basic, invalidate the basis
            factorization */
         if (col->stat == GLP_BS) lp->valid = 0;
      }
      /* store new contents of i-th row */
      if (!(0 <= len && len <= lp->n))
         xerror("glp_set_mat_row: i = %d; len = %d; invalid row length "
            "\n", i, len);
      if (len > NNZ_MAX - lp->nnz)
         xerror("glp_set_mat_row: i = %d; len = %d; too many constraint"
            " coefficients\n", i, len);
      for (k = 1; k <= len; k++)
      {  /* take number j of corresponding column */
         j = ind[k];
         /* obtain pointer to j-th column */
         if (!(1 <= j && j <= lp->n))
            xerror("glp_set_mat_row: i = %d; ind[%d] = %d; column index"
               " out of range\n", i, k, j);
         col = lp->col[j];
         /* if there is element with the same column index, it can only
            be found in the beginning of j-th column list */
         if (col->ptr != NULL && col->ptr->row->i == i)
            xerror("glp_set_mat_row: i = %d; ind[%d] = %d; duplicate co"
               "lumn indices not allowed\n", i, k, j);
         /* create new element */
         aij = dmp_get_atom(lp->pool, sizeof(GLPAIJ)), lp->nnz++;
         aij->row = row;
         aij->col = col;
         aij->val = val[k];
         /* add the new element to the beginning of i-th row and j-th
            column lists */
         aij->r_prev = NULL;
         aij->r_next = row->ptr;
         aij->c_prev = NULL;
         aij->c_next = col->ptr;
         if (aij->r_next != NULL) aij->r_next->r_prev = aij;
         if (aij->c_next != NULL) aij->c_next->c_prev = aij;
         row->ptr = col->ptr = aij;
         /* if the corresponding column is basic, invalidate the basis
            factorization */
         if (col->stat == GLP_BS && aij->val != 0.0) lp->valid = 0;
      }
      /* remove zero elements from i-th row */
      for (aij = row->ptr; aij != NULL; aij = next)
      {  next = aij->r_next;
         if (aij->val == 0.0)
         {  /* remove the element from the row list */
            if (aij->r_prev == NULL)
               row->ptr = next;
            else
               aij->r_prev->r_next = next;
            if (next == NULL)
               ;
            else
               next->r_prev = aij->r_prev;
            /* remove the element from the column list */
            xassert(aij->c_prev == NULL);
            aij->col->ptr = aij->c_next;
            if (aij->c_next != NULL) aij->c_next->c_prev = NULL;
            /* return the element to the memory pool */
            dmp_free_atom(lp->pool, aij, sizeof(GLPAIJ)), lp->nnz--;
         }
      }
      return;
}

/***********************************************************************
*  NAME
*
*  glp_set_mat_col - set (replace) column of the constraint matrix
*
*  SYNOPSIS
*
*  void glp_set_mat_col(glp_prob *lp, int j, int len, const int ind[],
*     const double val[]);
*
*  DESCRIPTION
*
*  The routine glp_set_mat_col stores (replaces) the contents of j-th
*  column of the constraint matrix of the specified problem object.
*
*  Row indices and numeric values of new column elements must be placed
*  in locations ind[1], ..., ind[len] and val[1], ..., val[len], where
*  0 <= len <= m is the new length of j-th column, m is the current
*  number of rows in the problem object. Elements with identical column
*  indices are not allowed. Zero elements are allowed, but they are not
*  stored in the constraint matrix.
*
*  If the parameter len is zero, the parameters ind and/or val can be
*  specified as NULL. */

void glp_set_mat_col(glp_prob *lp, int j, int len, const int ind[],
      const double val[])
{     glp_tree *tree = lp->tree;
      GLPROW *row;
      GLPCOL *col;
      GLPAIJ *aij, *next;
      int i, k;
      if (tree != NULL && tree->reason != 0)
         xerror("glp_set_mat_col: operation not allowed\n");
      /* obtain pointer to j-th column */
      if (!(1 <= j && j <= lp->n))
         xerror("glp_set_mat_col: j = %d; column number out of range\n",
            j);
      col = lp->col[j];
      /* remove all existing elements from j-th column */
      while (col->ptr != NULL)
      {  /* take next element in the column */
         aij = col->ptr;
         /* remove the element from the column list */
         col->ptr = aij->c_next;
         /* obtain pointer to corresponding row */
         row = aij->row;
         /* remove the element from the row list */
         if (aij->r_prev == NULL)
            row->ptr = aij->r_next;
         else
            aij->r_prev->r_next = aij->r_next;
         if (aij->r_next == NULL)
            ;
         else
            aij->r_next->r_prev = aij->r_prev;
         /* return the element to the memory pool */
         dmp_free_atom(lp->pool, aij, sizeof(GLPAIJ)), lp->nnz--;
      }
      /* store new contents of j-th column */
      if (!(0 <= len && len <= lp->m))
         xerror("glp_set_mat_col: j = %d; len = %d; invalid column leng"
            "th\n", j, len);
      if (len > NNZ_MAX - lp->nnz)
         xerror("glp_set_mat_col: j = %d; len = %d; too many constraint"
            " coefficients\n", j, len);
      for (k = 1; k <= len; k++)
      {  /* take number i of corresponding row */
         i = ind[k];
         /* obtain pointer to i-th row */
         if (!(1 <= i && i <= lp->m))
            xerror("glp_set_mat_col: j = %d; ind[%d] = %d; row index ou"
               "t of range\n", j, k, i);
         row = lp->row[i];
         /* if there is element with the same row index, it can only be
            found in the beginning of i-th row list */
         if (row->ptr != NULL && row->ptr->col->j == j)
            xerror("glp_set_mat_col: j = %d; ind[%d] = %d; duplicate ro"
               "w indices not allowed\n", j, k, i);
         /* create new element */
         aij = dmp_get_atom(lp->pool, sizeof(GLPAIJ)), lp->nnz++;
         aij->row = row;
         aij->col = col;
         aij->val = val[k];
         /* add the new element to the beginning of i-th row and j-th
            column lists */
         aij->r_prev = NULL;
         aij->r_next = row->ptr;
         aij->c_prev = NULL;
         aij->c_next = col->ptr;
         if (aij->r_next != NULL) aij->r_next->r_prev = aij;
         if (aij->c_next != NULL) aij->c_next->c_prev = aij;
         row->ptr = col->ptr = aij;
      }
      /* remove zero elements from j-th column */
      for (aij = col->ptr; aij != NULL; aij = next)
      {  next = aij->c_next;
         if (aij->val == 0.0)
         {  /* remove the element from the row list */
            xassert(aij->r_prev == NULL);
            aij->row->ptr = aij->r_next;
            if (aij->r_next != NULL) aij->r_next->r_prev = NULL;
            /* remove the element from the column list */
            if (aij->c_prev == NULL)
               col->ptr = next;
            else
               aij->c_prev->c_next = next;
            if (next == NULL)
               ;
            else
               next->c_prev = aij->c_prev;
            /* return the element to the memory pool */
            dmp_free_atom(lp->pool, aij, sizeof(GLPAIJ)), lp->nnz--;
         }
      }
      /* if j-th column is basic, invalidate the basis factorization */
      if (col->stat == GLP_BS) lp->valid = 0;
      return;
}

/***********************************************************************
*  NAME
*
*  glp_load_matrix - load (replace) the whole constraint matrix
*
*  SYNOPSIS
*
*  void glp_load_matrix(glp_prob *lp, int ne, const int ia[],
*     const int ja[], const double ar[]);
*
*  DESCRIPTION
*
*  The routine glp_load_matrix loads the constraint matrix passed in
*  the arrays ia, ja, and ar into the specified problem object. Before
*  loading the current contents of the constraint matrix is destroyed.
*
*  Constraint coefficients (elements of the constraint matrix) must be
*  specified as triplets (ia[k], ja[k], ar[k]) for k = 1, ..., ne,
*  where ia[k] is the row index, ja[k] is the column index, ar[k] is a
*  numeric value of corresponding constraint coefficient. The parameter
*  ne specifies the total number of (non-zero) elements in the matrix
*  to be loaded. Coefficients with identical indices are not allowed.
*  Zero coefficients are allowed, however, they are not stored in the
*  constraint matrix.
*
*  If the parameter ne is zero, the parameters ia, ja, and ar can be
*  specified as NULL. */

void glp_load_matrix(glp_prob *lp, int ne, const int ia[],
      const int ja[], const double ar[])
{     glp_tree *tree = lp->tree;
      GLPROW *row;
      GLPCOL *col;
      GLPAIJ *aij, *next;
      int i, j, k;
      if (tree != NULL && tree->reason != 0)
         xerror("glp_load_matrix: operation not allowed\n");
      /* clear the constraint matrix */
      for (i = 1; i <= lp->m; i++)
      {  row = lp->row[i];
         while (row->ptr != NULL)
         {  aij = row->ptr;
            row->ptr = aij->r_next;
            dmp_free_atom(lp->pool, aij, sizeof(GLPAIJ)), lp->nnz--;
         }
      }
      xassert(lp->nnz == 0);
      for (j = 1; j <= lp->n; j++) lp->col[j]->ptr = NULL;
      /* load the new contents of the constraint matrix and build its
         row lists */
      if (ne < 0)
         xerror("glp_load_matrix: ne = %d; invalid number of constraint"
            " coefficients\n", ne);
      if (ne > NNZ_MAX)
         xerror("glp_load_matrix: ne = %d; too many constraint coeffici"
            "ents\n", ne);
      for (k = 1; k <= ne; k++)
      {  /* take indices of new element */
         i = ia[k], j = ja[k];
         /* obtain pointer to i-th row */
         if (!(1 <= i && i <= lp->m))
            xerror("glp_load_matrix: ia[%d] = %d; row index out of rang"
               "e\n", k, i);
         row = lp->row[i];
         /* obtain pointer to j-th column */
         if (!(1 <= j && j <= lp->n))
            xerror("glp_load_matrix: ja[%d] = %d; column index out of r"
               "ange\n", k, j);
         col = lp->col[j];
         /* create new element */
         aij = dmp_get_atom(lp->pool, sizeof(GLPAIJ)), lp->nnz++;
         aij->row = row;
         aij->col = col;
         aij->val = ar[k];
         /* add the new element to the beginning of i-th row list */
         aij->r_prev = NULL;
         aij->r_next = row->ptr;
         if (aij->r_next != NULL) aij->r_next->r_prev = aij;
         row->ptr = aij;
      }
      xassert(lp->nnz == ne);
      /* build column lists of the constraint matrix and check elements
         with identical indices */
      for (i = 1; i <= lp->m; i++)
      {  for (aij = lp->row[i]->ptr; aij != NULL; aij = aij->r_next)
         {  /* obtain pointer to corresponding column */
            col = aij->col;
            /* if there is element with identical indices, it can only
               be found in the beginning of j-th column list */
            if (col->ptr != NULL && col->ptr->row->i == i)
            {  for (k = 1; k <= ne; k++)
                  if (ia[k] == i && ja[k] == col->j) break;
               xerror("glp_load_mat: ia[%d] = %d; ja[%d] = %d; duplicat"
                  "e indices not allowed\n", k, i, k, col->j);
            }
            /* add the element to the beginning of j-th column list */
            aij->c_prev = NULL;
            aij->c_next = col->ptr;
            if (aij->c_next != NULL) aij->c_next->c_prev = aij;
            col->ptr = aij;
         }
      }
      /* remove zero elements from the constraint matrix */
      for (i = 1; i <= lp->m; i++)
      {  row = lp->row[i];
         for (aij = row->ptr; aij != NULL; aij = next)
         {  next = aij->r_next;
            if (aij->val == 0.0)
            {  /* remove the element from the row list */
               if (aij->r_prev == NULL)
                  row->ptr = next;
               else
                  aij->r_prev->r_next = next;
               if (next == NULL)
                  ;
               else
                  next->r_prev = aij->r_prev;
               /* remove the element from the column list */
               if (aij->c_prev == NULL)
                  aij->col->ptr = aij->c_next;
               else
                  aij->c_prev->c_next = aij->c_next;
               if (aij->c_next == NULL)
                  ;
               else
                  aij->c_next->c_prev = aij->c_prev;
               /* return the element to the memory pool */
               dmp_free_atom(lp->pool, aij, sizeof(GLPAIJ)), lp->nnz--;
            }
         }
      }
      /* invalidate the basis factorization */
      lp->valid = 0;
      return;
}

/***********************************************************************
*  NAME
*
*  glp_check_dup - check for duplicate elements in sparse matrix
*
*  SYNOPSIS
*
*  int glp_check_dup(int m, int n, int ne, const int ia[],
*     const int ja[]);
*
*  DESCRIPTION
*
*  The routine glp_check_dup checks for duplicate elements (that is,
*  elements with identical indices) in a sparse matrix specified in the
*  coordinate format.
*
*  The parameters m and n specifies, respectively, the number of rows
*  and columns in the matrix, m >= 0, n >= 0.
*
*  The parameter ne specifies the number of (structurally) non-zero
*  elements in the matrix, ne >= 0.
*
*  Elements of the matrix are specified as doublets (ia[k],ja[k]) for
*  k = 1,...,ne, where ia[k] is a row index, ja[k] is a column index.
*
*  The routine glp_check_dup can be used prior to a call to the routine
*  glp_load_matrix to check that the constraint matrix to be loaded has
*  no duplicate elements.
*
*  RETURNS
*
*  The routine glp_check_dup returns one of the following values:
*
*   0 - the matrix has no duplicate elements;
*
*  -k - indices ia[k] or/and ja[k] are out of range;
*
*  +k - element (ia[k],ja[k]) is duplicate. */

int glp_check_dup(int m, int n, int ne, const int ia[], const int ja[])
{     int i, j, k, *ptr, *next, ret;
      char *flag;
      if (m < 0)
         xerror("glp_check_dup: m = %d; invalid parameter\n");
      if (n < 0)
         xerror("glp_check_dup: n = %d; invalid parameter\n");
      if (ne < 0)
         xerror("glp_check_dup: ne = %d; invalid parameter\n");
      if (ne > 0 && ia == NULL)
         xerror("glp_check_dup: ia = %p; invalid parameter\n", ia);
      if (ne > 0 && ja == NULL)
         xerror("glp_check_dup: ja = %p; invalid parameter\n", ja);
      for (k = 1; k <= ne; k++)
      {  i = ia[k], j = ja[k];
         if (!(1 <= i && i <= m && 1 <= j && j <= n))
         {  ret = -k;
            goto done;
         }
      }
      if (m == 0 || n == 0)
      {  ret = 0;
         goto done;
      }
      /* allocate working arrays */
      ptr = xcalloc(1+m, sizeof(int));
      next = xcalloc(1+ne, sizeof(int));
      flag = xcalloc(1+n, sizeof(char));
      /* build row lists */
      for (i = 1; i <= m; i++)
         ptr[i] = 0;
      for (k = 1; k <= ne; k++)
      {  i = ia[k];
         next[k] = ptr[i];
         ptr[i] = k;
      }
      /* clear column flags */
      for (j = 1; j <= n; j++)
         flag[j] = 0;
      /* check for duplicate elements */
      for (i = 1; i <= m; i++)
      {  for (k = ptr[i]; k != 0; k = next[k])
         {  j = ja[k];
            if (flag[j])
            {  /* find first element (i,j) */
               for (k = 1; k <= ne; k++)
                  if (ia[k] == i && ja[k] == j) break;
               xassert(k <= ne);
               /* find next (duplicate) element (i,j) */
               for (k++; k <= ne; k++)
                  if (ia[k] == i && ja[k] == j) break;
               xassert(k <= ne);
               ret = +k;
               goto skip;
            }
            flag[j] = 1;
         }
         /* clear column flags */
         for (k = ptr[i]; k != 0; k = next[k])
            flag[ja[k]] = 0;
      }
      /* no duplicate element found */
      ret = 0;
skip: /* free working arrays */
      xfree(ptr);
      xfree(next);
      xfree(flag);
done: return ret;
}

/***********************************************************************
*  NAME
*
*  glp_sort_matrix - sort elements of the constraint matrix
*
*  SYNOPSIS
*
*  void glp_sort_matrix(glp_prob *P);
*
*  DESCRIPTION
*
*  The routine glp_sort_matrix sorts elements of the constraint matrix
*  rebuilding its row and column linked lists. On exit from the routine
*  the constraint matrix is not changed, however, elements in the row
*  linked lists become ordered by ascending column indices, and the
*  elements in the column linked lists become ordered by ascending row
*  indices. */

void glp_sort_matrix(glp_prob *P)
{     GLPAIJ *aij;
      int i, j;
      if (P == NULL || P->magic != GLP_PROB_MAGIC)
         xerror("glp_sort_matrix: P = %p; invalid problem object\n",
            P);
      /* rebuild row linked lists */
      for (i = P->m; i >= 1; i--)
         P->row[i]->ptr = NULL;
      for (j = P->n; j >= 1; j--)
      {  for (aij = P->col[j]->ptr; aij != NULL; aij = aij->c_next)
         {  i = aij->row->i;
            aij->r_prev = NULL;
            aij->r_next = P->row[i]->ptr;
            if (aij->r_next != NULL) aij->r_next->r_prev = aij;
            P->row[i]->ptr = aij;
         }
      }
      /* rebuild column linked lists */
      for (j = P->n; j >= 1; j--)
         P->col[j]->ptr = NULL;
      for (i = P->m; i >= 1; i--)
      {  for (aij = P->row[i]->ptr; aij != NULL; aij = aij->r_next)
         {  j = aij->col->j;
            aij->c_prev = NULL;
            aij->c_next = P->col[j]->ptr;
            if (aij->c_next != NULL) aij->c_next->c_prev = aij;
            P->col[j]->ptr = aij;
         }
      }
      return;
}

/***********************************************************************
*  NAME
*
*  glp_del_rows - delete rows from problem object
*
*  SYNOPSIS
*
*  void glp_del_rows(glp_prob *lp, int nrs, const int num[]);
*
*  DESCRIPTION
*
*  The routine glp_del_rows deletes rows from the specified problem
*  object. Ordinal numbers of rows to be deleted should be placed in
*  locations num[1], ..., num[nrs], where nrs > 0.
*
*  Note that deleting rows involves changing ordinal numbers of other
*  rows remaining in the problem object. New ordinal numbers of the
*  remaining rows are assigned under the assumption that the original
*  order of rows is not changed. */

void glp_del_rows(glp_prob *lp, int nrs, const int num[])
{     glp_tree *tree = lp->tree;
      GLPROW *row;
      int i, k, m_new;
      /* mark rows to be deleted */
      if (!(1 <= nrs && nrs <= lp->m))
         xerror("glp_del_rows: nrs = %d; invalid number of rows\n",
            nrs);
      for (k = 1; k <= nrs; k++)
      {  /* take the number of row to be deleted */
         i = num[k];
         /* obtain pointer to i-th row */
         if (!(1 <= i && i <= lp->m))
            xerror("glp_del_rows: num[%d] = %d; row number out of range"
               "\n", k, i);
         row = lp->row[i];
         if (tree != NULL && tree->reason != 0)
         {  if (!(tree->reason == GLP_IROWGEN ||
                  tree->reason == GLP_ICUTGEN))
               xerror("glp_del_rows: operation not allowed\n");
            xassert(tree->curr != NULL);
            if (row->level != tree->curr->level)
               xerror("glp_del_rows: num[%d] = %d; invalid attempt to d"
                  "elete row created not in current subproblem\n", k,i);
            if (row->stat != GLP_BS)
               xerror("glp_del_rows: num[%d] = %d; invalid attempt to d"
                  "elete active row (constraint)\n", k, i);
            tree->reinv = 1;
         }
         /* check that the row is not marked yet */
         if (row->i == 0)
            xerror("glp_del_rows: num[%d] = %d; duplicate row numbers n"
               "ot allowed\n", k, i);
         /* erase symbolic name assigned to the row */
         glp_set_row_name(lp, i, NULL);
         xassert(row->node == NULL);
         /* erase corresponding row of the constraint matrix */
         glp_set_mat_row(lp, i, 0, NULL, NULL);
         xassert(row->ptr == NULL);
         /* mark the row to be deleted */
         row->i = 0;
      }
      /* delete all marked rows from the row list */
      m_new = 0;
      for (i = 1; i <= lp->m; i++)
      {  /* obtain pointer to i-th row */
         row = lp->row[i];
         /* check if the row is marked */
         if (row->i == 0)
         {  /* it is marked, delete it */
            dmp_free_atom(lp->pool, row, sizeof(GLPROW));
         }
         else
         {  /* it is not marked; keep it */
            row->i = ++m_new;
            lp->row[row->i] = row;
         }
      }
      /* set new number of rows */
      lp->m = m_new;
      /* invalidate the basis factorization */
      lp->valid = 0;
      return;
}

/***********************************************************************
*  NAME
*
*  glp_del_cols - delete columns from problem object
*
*  SYNOPSIS
*
*  void glp_del_cols(glp_prob *lp, int ncs, const int num[]);
*
*  DESCRIPTION
*
*  The routine glp_del_cols deletes columns from the specified problem
*  object. Ordinal numbers of columns to be deleted should be placed in
*  locations num[1], ..., num[ncs], where ncs > 0.
*
*  Note that deleting columns involves changing ordinal numbers of
*  other columns remaining in the problem object. New ordinal numbers
*  of the remaining columns are assigned under the assumption that the
*  original order of columns is not changed. */

void glp_del_cols(glp_prob *lp, int ncs, const int num[])
{     glp_tree *tree = lp->tree;
      GLPCOL *col;
      int j, k, n_new;
      if (tree != NULL && tree->reason != 0)
         xerror("glp_del_cols: operation not allowed\n");
      /* mark columns to be deleted */
      if (!(1 <= ncs && ncs <= lp->n))
         xerror("glp_del_cols: ncs = %d; invalid number of columns\n",
            ncs);
      for (k = 1; k <= ncs; k++)
      {  /* take the number of column to be deleted */
         j = num[k];
         /* obtain pointer to j-th column */
         if (!(1 <= j && j <= lp->n))
            xerror("glp_del_cols: num[%d] = %d; column number out of ra"
               "nge", k, j);
         col = lp->col[j];
         /* check that the column is not marked yet */
         if (col->j == 0)
            xerror("glp_del_cols: num[%d] = %d; duplicate column number"
               "s not allowed\n", k, j);
         /* erase symbolic name assigned to the column */
         glp_set_col_name(lp, j, NULL);
         xassert(col->node == NULL);
         /* erase corresponding column of the constraint matrix */
         glp_set_mat_col(lp, j, 0, NULL, NULL);
         xassert(col->ptr == NULL);
         /* mark the column to be deleted */
         col->j = 0;
         /* if it is basic, invalidate the basis factorization */
         if (col->stat == GLP_BS) lp->valid = 0;
      }
      /* delete all marked columns from the column list */
      n_new = 0;
      for (j = 1; j <= lp->n; j++)
      {  /* obtain pointer to j-th column */
         col = lp->col[j];
         /* check if the column is marked */
         if (col->j == 0)
         {  /* it is marked; delete it */
            dmp_free_atom(lp->pool, col, sizeof(GLPCOL));
         }
         else
         {  /* it is not marked; keep it */
            col->j = ++n_new;
            lp->col[col->j] = col;
         }
      }
      /* set new number of columns */
      lp->n = n_new;
      /* if the basis header is still valid, adjust it */
      if (lp->valid)
      {  int m = lp->m;
         int *head = lp->head;
         for (j = 1; j <= n_new; j++)
         {  k = lp->col[j]->bind;
            if (k != 0)
            {  xassert(1 <= k && k <= m);
               head[k] = m + j;
            }
         }
      }
      return;
}

/***********************************************************************
*  NAME
*
*  glp_copy_prob - copy problem object content
*
*  SYNOPSIS
*
*  void glp_copy_prob(glp_prob *dest, glp_prob *prob, int names);
*
*  DESCRIPTION
*
*  The routine glp_copy_prob copies the content of the problem object
*  prob to the problem object dest.
*
*  The parameter names is a flag. If it is non-zero, the routine also
*  copies all symbolic names; otherwise, if it is zero, symbolic names
*  are not copied. */

void glp_copy_prob(glp_prob *dest, glp_prob *prob, int names)
{     glp_tree *tree = dest->tree;
      glp_bfcp bfcp;
      int i, j, len, *ind;
      double *val;
      if (tree != NULL && tree->reason != 0)
         xerror("glp_copy_prob: operation not allowed\n");
      if (dest == prob)
         xerror("glp_copy_prob: copying problem object to itself not al"
            "lowed\n");
      if (!(names == GLP_ON || names == GLP_OFF))
         xerror("glp_copy_prob: names = %d; invalid parameter\n",
            names);
      glp_erase_prob(dest);
      if (names && prob->name != NULL)
         glp_set_prob_name(dest, prob->name);
      if (names && prob->obj != NULL)
         glp_set_obj_name(dest, prob->obj);
      dest->dir = prob->dir;
      dest->c0 = prob->c0;
      if (prob->m > 0)
         glp_add_rows(dest, prob->m);
      if (prob->n > 0)
         glp_add_cols(dest, prob->n);
      glp_get_bfcp(prob, &bfcp);
      glp_set_bfcp(dest, &bfcp);
      dest->pbs_stat = prob->pbs_stat;
      dest->dbs_stat = prob->dbs_stat;
      dest->obj_val = prob->obj_val;
      dest->some = prob->some;
      dest->ipt_stat = prob->ipt_stat;
      dest->ipt_obj = prob->ipt_obj;
      dest->mip_stat = prob->mip_stat;
      dest->mip_obj = prob->mip_obj;
      for (i = 1; i <= prob->m; i++)
      {  GLPROW *to = dest->row[i];
         GLPROW *from = prob->row[i];
         if (names && from->name != NULL)
            glp_set_row_name(dest, i, from->name);
         to->type = from->type;
         to->lb = from->lb;
         to->ub = from->ub;
         to->rii = from->rii;
         to->stat = from->stat;
         to->prim = from->prim;
         to->dual = from->dual;
         to->pval = from->pval;
         to->dval = from->dval;
         to->mipx = from->mipx;
      }
      ind = xcalloc(1+prob->m, sizeof(int));
      val = xcalloc(1+prob->m, sizeof(double));
      for (j = 1; j <= prob->n; j++)
      {  GLPCOL *to = dest->col[j];
         GLPCOL *from = prob->col[j];
         if (names && from->name != NULL)
            glp_set_col_name(dest, j, from->name);
         to->kind = from->kind;
         to->type = from->type;
         to->lb = from->lb;
         to->ub = from->ub;
         to->coef = from->coef;
         len = glp_get_mat_col(prob, j, ind, val);
         glp_set_mat_col(dest, j, len, ind, val);
         to->sjj = from->sjj;
         to->stat = from->stat;
         to->prim = from->prim;
         to->dual = from->dual;
         to->pval = from->pval;
         to->dval = from->dval;
         to->mipx = from->mipx;
      }
      xfree(ind);
      xfree(val);
      return;
}

/***********************************************************************
*  NAME
*
*  glp_erase_prob - erase problem object content
*
*  SYNOPSIS
*
*  void glp_erase_prob(glp_prob *lp);
*
*  DESCRIPTION
*
*  The routine glp_erase_prob erases the content of the specified
*  problem object. The effect of this operation is the same as if the
*  problem object would be deleted with the routine glp_delete_prob and
*  then created anew with the routine glp_create_prob, with exception
*  that the handle (pointer) to the problem object remains valid. */

static void delete_prob(glp_prob *lp);

void glp_erase_prob(glp_prob *lp)
{     glp_tree *tree = lp->tree;
      if (tree != NULL && tree->reason != 0)
         xerror("glp_erase_prob: operation not allowed\n");
      delete_prob(lp);
      create_prob(lp);
      return;
}

/***********************************************************************
*  NAME
*
*  glp_delete_prob - delete problem object
*
*  SYNOPSIS
*
*  void glp_delete_prob(glp_prob *lp);
*
*  DESCRIPTION
*
*  The routine glp_delete_prob deletes the specified problem object and
*  frees all the memory allocated to it. */

static void delete_prob(glp_prob *lp)
{     lp->magic = 0x3F3F3F3F;
      dmp_delete_pool(lp->pool);
#if 0 /* 08/III-2014 */
#if 0 /* 17/XI-2009 */
      xfree(lp->cps);
#else
      if (lp->parms != NULL) xfree(lp->parms);
#endif
#endif
      xassert(lp->tree == NULL);
#if 0
      if (lp->cwa != NULL) xfree(lp->cwa);
#endif
      xfree(lp->row);
      xfree(lp->col);
      if (lp->r_tree != NULL) avl_delete_tree(lp->r_tree);
      if (lp->c_tree != NULL) avl_delete_tree(lp->c_tree);
      xfree(lp->head);
#if 0 /* 08/III-2014 */
      if (lp->bfcp != NULL) xfree(lp->bfcp);
#endif
      if (lp->bfd != NULL) bfd_delete_it(lp->bfd);
      return;
}

void glp_delete_prob(glp_prob *lp)
{     glp_tree *tree = lp->tree;
      if (tree != NULL && tree->reason != 0)
         xerror("glp_delete_prob: operation not allowed\n");
      delete_prob(lp);
      xfree(lp);
      return;
}

/* eof */












/* eof */
// コンパイル方法
//
// gcc -o sample glpk-mip-sample.c -lglpk -lm
// ./sample

/*
Maximize
 obj: x1 + 2 x2 + 3 x3 + x4
Subject To
 c1: 0 <= - x1 + x2 + x3 + 10 x4 <= 20
 c2: 0 <= x1 - 3 x2 + x3 <= 30
 c3: x2 - 3.5 x4 = 0
Bounds
 0 <= x1 <= 40
 0 <= x2
 0 <= x3
 2 <= x4 <= 3
General # Integer Variables
 x3
 x4
End
*/

int main(void)
{
  glp_prob *mip = glp_create_prob();
  glp_set_prob_name(mip, "sample");
  glp_set_obj_dir(mip, GLP_MAX);

  // 拘束条件
  // 具体的な関数は後で
  glp_add_rows(mip, 3); // 拘束条件の数
  glp_set_row_name(mip, 1, "c1"); glp_set_row_bnds(mip, 1, GLP_DB, 0.0, 20.0);
  glp_set_row_name(mip, 2, "c2"); glp_set_row_bnds(mip, 2, GLP_DB, 0.0, 30.0);
  glp_set_row_name(mip, 3, "c3"); glp_set_row_bnds(mip, 3, GLP_FX, 0.0, 0);

  // 変数
  // 変数そのものにかかる拘束は、拘束条件ではなくてこちらで管理
  glp_add_cols(mip, 4); // 変数の数
  glp_set_col_name(mip, 1, "x1");
  glp_set_col_bnds(mip, 1, GLP_DB, 0.0, 40.0); glp_set_obj_coef(mip, 1, 1.0);
  glp_set_col_name(mip, 2, "x2");
  glp_set_col_bnds(mip, 2, GLP_LO, 0.0, 0.0); glp_set_obj_coef(mip, 2, 2.0);
  glp_set_col_name(mip, 3, "x3");
  glp_set_col_bnds(mip, 3, GLP_LO, 0.0, 0.0); glp_set_obj_coef(mip, 3, 3.0);
  glp_set_col_kind(mip, 3, GLP_IV); // 整数値としての宣言
  glp_set_col_name(mip, 4, "x4");
  glp_set_col_bnds(mip, 4, GLP_DB, 2.0, 3.0); glp_set_obj_coef(mip, 4, 1.0);
  glp_set_col_kind(mip, 4, GLP_IV); // 整数値としての宣言

  int ia[1+9], ja[1+9];
  double ar[1+9];
  ia[1]=1,ja[1]=1,ar[1]=-1;   // a[1,1] = -1
  ia[2]=1,ja[2]=2,ar[2]=1;    // a[1,2] = 1
  ia[3]=1,ja[3]=3,ar[3]=1;    // a[1,3] = 1
  ia[4]=1,ja[4]=4,ar[4]=10;   // a[1,4] = 10
  ia[5]=2,ja[5]=1,ar[5]=1;    // a[2,1] = 1
  ia[6]=2,ja[6]=2,ar[6]=-3;   // a[2,2] = -3
  ia[7]=2,ja[7]=3,ar[7]=1;    // a[2,3] = 1
  ia[8]=3,ja[8]=2,ar[8]=1;    // a[3,2] = 1
  ia[9]=3,ja[9]=4,ar[9]=-3.5; // a[3,4] = -3.5
  glp_load_matrix(mip, 9, ia, ja, ar);

  glp_iocp parm;
  glp_init_iocp(&parm);
  parm.presolve = GLP_ON;
  int err = glp_intopt(mip, &parm);

  double z = glp_mip_obj_val(mip);
  double x1 = glp_mip_col_val(mip, 1);
  double x2 = glp_mip_col_val(mip, 2);
  double x3 = glp_mip_col_val(mip, 3);
  double x4 = glp_mip_col_val(mip, 4);
  printf("\nz = %g; x1 = %g; x2 = %g; x3 = %g, x4 = %g\n", z, x1, x2, x3, x4);
  // z = 122.5; x1 = 40; x2 = 10.5; x3 = 19.5, x4 = 3

  glp_delete_prob(mip);
  return 0;
}
