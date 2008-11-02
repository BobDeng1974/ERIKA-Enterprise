/* ###*B*###
 * Copyright (C) Roberto Bucher (SUPSI- Lugano)
 *               Simone Mannori (Scilab / INRIA / DIGITEO)
 *
 * Copyright (C) METALAU Project (INRIA)
 * ###*E*### */
 
 
/************************************/
/** Beginning of file s2s_common.c **/
/************************************/

/* Informations:
 * this file gathers all code which must be provided by user in 
 * order to be able to compile a C generated file.
 **
 * User may (and is encouraged to) modify this file at will. 
 ** 
 * Here must be defined the code corresponding to the sensors and actuator
 * translated from a scicos superbloc. Sensor and actuator code correspond
 * to the following functions :
 * void sensor(scicos_block *block, int flag)
 * void actuator(scicos_block *block, int flag)
 **
 * we provide default sensors/actuators in order to allow to build fast tests.
 * 
 */

#include "scicos_block4.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "memory.h"
#include <assert.h>

/*******************************************************************
 * here are the files used for matrix multiplication
 * the code might be redundant but is needed in many blocs :
 * 'csslti', 'gainblk', 'dsslti' for instance
 *******************************************************************/

/****************************************************/
/***************** dmmul.c file *********************/
/****************************************************/

/* Table of constant values */

static double c_b4 = 1.;
static double c_b5 = 0.;
static int c__1 = 1;

int dgemm();
int dmmul(double *a, int *na, double *b, int *nb, double *c__, 
	  int *nc, int *l, int *m, int *n)
{
  int a_dim1, a_offset, b_dim1, b_offset, c_dim1, c_offset;
    
/*     PURPOSE */
/*        computes the matrix product C = A * B */
/*            C   =   A   *   B */
/*          (l,n)   (l,m) * (m,n) */

/*     PARAMETERS */
/*        input */
/*        ----- */
/*        A : (double) array (l, m) with leading dim na */

/*        B : (double) array (m, n) with leading dim nb */

/*        na, nb, nc, l, m, n : integers */

/*        output */
/*        ------ */
/*        C : (double) array (l, n) with leading dim nc */

/*     NOTE */
/*        (original version substituted by a call to the blas dgemm) */
    /* Parameter adjustments */
    a_dim1 = *na;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    c_dim1 = *nc;
    c_offset = 1 + c_dim1 * 1;
    c__ -= c_offset;
    b_dim1 = *nb;
    b_offset = 1 + b_dim1 * 1;
    b -= b_offset;

    /* Function Body */
    dgemm("n", "n", l, n, m, &c_b4, &a[a_offset], na, &b[b_offset], nb, &
	    c_b5, &c__[c_offset], nc);
} /* dmmul */

int dgemm(char *transa, char *transb, int *m, int *n, int *k, double *alpha, 
	  double *a, int *lda, double *b, int *ldb,double *beta, double *c, 
	  int *ldc)
{
  /* System generated locals */
  int i__1, i__2, i__3;
  /* Local variables */
  static int info;
  static long int nota, notb;
  static double temp;
  static int i, j, l, ncola;
  long int lsame(char *, char *);
  static int nrowa, nrowb;  
  int xerbla(char *, int *);
  
  /*     .. Scalar Arguments .. */
  /*     .. Array Arguments .. */
  /*     .. */
  
  /*  Purpose */
  /*  ======= */
  
  /*  DGEMM  performs one of the matrix-matrix operations */
  
  /*     C := alpha*op( A )*op( B ) + beta*C, */
  
  /*  where  op( X ) is one of */
  
  /*     op( X ) = X   or   op( X ) = X', */
  
  /*  alpha and beta are scalars, and A, B and C are matrices, with op( A ) */
  /*  an m by k matrix,  op( B )  a  k by n matrix and  C an m by n matrix. */
  
  /*  Parameters */
  /*  ========== */
  
  /*  TRANSA - CHARACTER*1. */
  /*           On entry, TRANSA specifies the form of op( A ) to be used in */
  /*           the matrix multiplication as follows: */
  
  /*              TRANSA = 'N' or 'n',  op( A ) = A. */
  
  /*              TRANSA = 'T' or 't',  op( A ) = A'. */
  
  /*              TRANSA = 'C' or 'c',  op( A ) = A'. */
  
  /*           Unchanged on exit. */
  
  /*  TRANSB - CHARACTER*1. */
  /*           On entry, TRANSB specifies the form of op( B ) to be used in */
  /*           the matrix multiplication as follows: */
  
  /*              TRANSB = 'N' or 'n',  op( B ) = B. */
  
  /*              TRANSB = 'T' or 't',  op( B ) = B'. */
  
  /*              TRANSB = 'C' or 'c',  op( B ) = B'. */
  
  /*           Unchanged on exit. */
  
  /*  M      - INTEGER. */
  /*           On entry,  M  specifies  the number  of rows  of the  matrix */
  /*           op( A )  and of the  matrix  C.  M  must  be at least  zero. */
  /*           Unchanged on exit. */
  
  /*  N      - INTEGER. */
  /*           On entry,  N  specifies the number  of columns of the matrix */
  /*           op( B ) and the number of columns of the matrix C. N must be */
  /*           at least zero. */
  /*           Unchanged on exit. */
  
  /*  K      - INTEGER. */
  /*           On entry,  K  specifies  the number of columns of the matrix */
  /*           op( A ) and the number of rows of the matrix op( B ). K must */
  /*           be at least  zero. */
  /*           Unchanged on exit. */
  
  /*  ALPHA  - DOUBLE PRECISION. */
  /*           On entry, ALPHA specifies the scalar alpha. */
  /*           Unchanged on exit. */
  
  /*  A      - DOUBLE PRECISION array of DIMENSION ( LDA, ka ), where ka is */
  /*           k  when  TRANSA = 'N' or 'n',  and is  m  otherwise. */
  /*           Before entry with  TRANSA = 'N' or 'n',  the leading  m by k */
  /*           part of the array  A  must contain the matrix  A,  otherwise */
  /*           the leading  k by m  part of the array  A  must contain  the */
  /*           matrix A. */
  /*           Unchanged on exit. */
  
  /*  LDA    - INTEGER. */
  /*           On entry, LDA specifies the first dimension of A as declared */
  /*           in the calling (sub) program. When  TRANSA = 'N' or 'n' then */
  /*           LDA must be at least  max( 1, m ), otherwise  LDA must be at */
  /*           least  max( 1, k ). */
  /*           Unchanged on exit. */
  
  /*  B      - DOUBLE PRECISION array of DIMENSION ( LDB, kb ), where kb is */
  /*           n  when  TRANSB = 'N' or 'n',  and is  k  otherwise. */
  /*           Before entry with  TRANSB = 'N' or 'n',  the leading  k by n */
  /*           part of the array  B  must contain the matrix  B,  otherwise */
  /*           the leading  n by k  part of the array  B  must contain  the */
  /*           matrix B. */
  /*           Unchanged on exit. */
  
  /*  LDB    - INTEGER. */
  /*           On entry, LDB specifies the first dimension of B as declared */
  /*           in the calling (sub) program. When  TRANSB = 'N' or 'n' then */
  /*           LDB must be at least  max( 1, k ), otherwise  LDB must be at */
  /*           least  max( 1, n ). */
  /*           Unchanged on exit. */
  
  /*  BETA   - DOUBLE PRECISION. */
  /*           On entry,  BETA  specifies the scalar  beta.  When  BETA  is */
  /*           supplied as zero then C need not be set on input. */
  /*           Unchanged on exit. */
  
  /*  C      - DOUBLE PRECISION array of DIMENSION ( LDC, n ). */
  /*           Before entry, the leading  m by n  part of the array  C must */
  /*           contain the matrix  C,  except when  beta  is zero, in which */
  /*           case C need not be set on entry. */
  /*           On exit, the array  C  is overwritten by the  m by n  matrix */
  /*           ( alpha*op( A )*op( B ) + beta*C ). */
  
  /*  LDC    - INTEGER. */
  /*           On entry, LDC specifies the first dimension of C as declared */
  /*           in  the  calling  (sub)  program.   LDC  must  be  at  least */
  /*           max( 1, m ). */
  /*           Unchanged on exit. */
  
  
  /*  Level 3 Blas routine. */
  
  /*  -- Written on 8-February-1989. */
  /*     Jack Dongarra, Argonne National Laboratory. */
  /*     Iain Duff, AERE Harwell. */
  /*     Jeremy Du Croz, Numerical Algorithms Group Ltd. */
  /*     Sven Hammarling, Numerical Algorithms Group Ltd. */
  
  
  /*     .. External Functions .. */
  /*     .. External Subroutines .. */
  /*     .. Intrinsic Functions .. */
  /*     .. Local Scalars .. */
  /*     .. Parameters .. */
  /*     .. */
  /*     .. Executable Statements .. */
  
  /*     Set  NOTA  and  NOTB  as  true if  A  and  B  respectively are not */
  /*     transposed and set  NROWA, NCOLA and  NROWB  as the number of rows */
  /*     and  columns of  A  and the  number of  rows  of  B  respectively. */
  
#define A(I,J) a[(I)-1 + ((J)-1)* ( *lda)]
#define B(I,J) b[(I)-1 + ((J)-1)* ( *ldb)]
#define C(I,J) c[(I)-1 + ((J)-1)* ( *ldc)]
  
  nota = lsame(transa, "N");
  notb = lsame(transb, "N");
  if (nota) {
    nrowa = *m;
    ncola = *k;
  } else {
    nrowa = *k;
    ncola = *m;
  }
  if (notb) {
    nrowb = *k;
  } else {
    nrowb = *n;
  }
  
  /*     Test the input parameters. */
  
  info = 0;
  if (! nota && ! lsame(transa, "C") && ! lsame(transa, "T")) {
    info = 1;
  } else if (! notb && ! lsame(transb, "C") && ! lsame(transb,"T")) {
    info = 2;
  } else if (*m < 0) {
    info = 3;
  } else if (*n < 0) {
    info = 4;
  } else if (*k < 0) {
    info = 5;
  } else if (*lda < max(1,nrowa)) {
    info = 8;
  } else if (*ldb < max(1,nrowb)) {
    info = 10;
  } else if (*ldc < max(1,*m)) {
    info = 13;
  }
  if (info != 0) {
    xerbla("DGEMM ", &info);
    return 0;
  }
  
  /*     Quick return if possible. */
  
  if (*m == 0 || *n == 0 || (*alpha == 0. || *k == 0) && *beta == 1.) {
    return 0;
  }
  
  /*     And if  alpha.eq.zero. */
  
  if (*alpha == 0.) {
    if (*beta == 0.) {
      i__1 = *n;
      for (j = 1; j <= *n; ++j) {
	i__2 = *m;
	for (i = 1; i <= *m; ++i) {
	  C(i,j) = 0.;
	  /* L10: */
	}
	/* L20: */
      }
    } else {
      i__1 = *n;
      for (j = 1; j <= *n; ++j) {
	i__2 = *m;
	for (i = 1; i <= *m; ++i) {
	  C(i,j) = *beta * C(i,j);
	  /* L30: */
	}
	/* L40: */
      }
    }
    return 0;
  }
  
  /*     Start the operations. */
  
  if (notb) {
    if (nota) {
      
      /*           Form  C := alpha*A*B + beta*C. */
      
      i__1 = *n;
      for (j = 1; j <= *n; ++j) {
	if (*beta == 0.) {
	  i__2 = *m;
	  for (i = 1; i <= *m; ++i) {
	    C(i,j) = 0.;
	    /* L50: */
	  }
	} else if (*beta != 1.) {
	  i__2 = *m;
	  for (i = 1; i <= *m; ++i) {
	    C(i,j) = *beta * C(i,j);
	    /* L60: */
	  }
	}
	i__2 = *k;
	for (l = 1; l <= *k; ++l) {
	  if (B(l,j) != 0.) {
	    temp = *alpha * B(l,j);
	    i__3 = *m;
	    for (i = 1; i <= *m; ++i) {
	      C(i,j) += temp * A(i,l);
	      /* L70: */
	    }
	  }
	  /* L80: */
	}
	/* L90: */
      }
    } else {
      
      /*           Form  C := alpha*A'*B + beta*C */
      
      i__1 = *n;
      for (j = 1; j <= *n; ++j) {
	i__2 = *m;
	for (i = 1; i <= *m; ++i) {
	  temp = 0.;
	  i__3 = *k;
	  for (l = 1; l <= *k; ++l) {
	    temp += A(l,i) * B(l,j);
	    /* L100: */
	  }
	  if (*beta == 0.) {
	    C(i,j) = *alpha * temp;
	  } else {
	    C(i,j) = *alpha * temp + *beta * C(i,j);
	  }
	  /* L110: */
	}
	/* L120: */
      }
    }
  } else {
    if (nota) {
      
      /*           Form  C := alpha*A*B' + beta*C */
      
      i__1 = *n;
      for (j = 1; j <= *n; ++j) {
	if (*beta == 0.) {
	  i__2 = *m;
	  for (i = 1; i <= *m; ++i) {
	    C(i,j) = 0.;
	    /* L130: */
	  }
	} else if (*beta != 1.) {
	  i__2 = *m;
	  for (i = 1; i <= *m; ++i) {
	    C(i,j) = *beta * C(i,j);
	    /* L140: */
	  }
	}
	i__2 = *k;
	for (l = 1; l <= *k; ++l) {
	  if (B(j,l) != 0.) {
	    temp = *alpha * B(j,l);
	    i__3 = *m;
	    for (i = 1; i <= *m; ++i) {
	      C(i,j) += temp * A(i,l);
	      /* L150: */
	    }
	  }
	  /* L160: */
	}
	/* L170: */
      }
    } else {
      
      /*           Form  C := alpha*A'*B' + beta*C */
      
      i__1 = *n;
      for (j = 1; j <= *n; ++j) {
	i__2 = *m;
	for (i = 1; i <= *m; ++i) {
	  temp = 0.;
	  i__3 = *k;
	  for (l = 1; l <= *k; ++l) {
	    temp += A(l,i) * B(j,l);
	    /* L180: */
	  }
	  if (*beta == 0.) {
	    C(i,j) = *alpha * temp;
	  } else {
	    C(i,j) = *alpha * temp + *beta * C(i,j);
	  }
	  /* L190: */
	}
	/* L200: */
      }
    }
  }
  
  return 0;
  
  /*     End of DGEMM . */
  
} /* dgemm */

long int lsame(char *ca, char *cb)
{
  /* System generated locals */
  long int ret_val;

  /* Local variables */
  static int inta, intb, zcode;
  

  /*  -- LAPACK auxiliary routine (version 2.0) -- */
  /*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd., */
  /*     Courant Institute, Argonne National Lab, and Rice University */
  /*     January 31, 1994 */
  
  /*     .. Scalar Arguments .. */
  /*     .. */
  
  /*  Purpose */
  /*  ======= */
  
  /*  LSAME returns .TRUE. if CA is the same letter as CB regardless of */
  /*  case. */
  
  /*  Arguments */
  /*  ========= */
  
  /*  CA      (input) CHARACTER*1 */
  /*  CB      (input) CHARACTER*1 */
  /*          CA and CB specify the single characters to be compared. */
  
  /* ===================================================================== */
  
  /*     .. Intrinsic Functions .. */
  /*     .. */
  /*     .. Local Scalars .. */
  /*     .. */
  /*     .. Executable Statements .. */
  
  /*     Test if the characters are equal */
  
  ret_val = *(unsigned char *)ca == *(unsigned char *)cb;
  if (ret_val) {
    return ret_val;
  }
  
  /*     Now test for equivalence if both characters are alphabetic. */
  
  zcode = 'Z';
  
  /*     Use 'Z' rather than 'A' so that ASCII can be detected on Prime */
  /*     machines, on which ICHAR returns a value with bit 8 set. */
  /*     ICHAR('A') on Prime machines returns 193 which is the same as */
  /*     ICHAR('A') on an EBCDIC machine. */
  
  inta = *(unsigned char *)ca;
  intb = *(unsigned char *)cb;
  
  if (zcode == 90 || zcode == 122) {
    
    /*        ASCII is assumed - ZCODE is the ASCII code of either lower or */
    /*        upper case 'Z'. */
    
    if (inta >= 97 && inta <= 122) {
      inta += -32;
    }
    if (intb >= 97 && intb <= 122) {
      intb += -32;
    }
    
  } else if (zcode == 233 || zcode == 169) {
    
    /*        EBCDIC is assumed - ZCODE is the EBCDIC code of either lower or */
    /*        upper case 'Z'. */
    
    if (inta >= 129 && inta <= 137 || inta >= 145 && inta <= 153 || inta 
	>= 162 && inta <= 169) {
      inta += 64;
    }
    if (intb >= 129 && intb <= 137 || intb >= 145 && intb <= 153 || intb 
	>= 162 && intb <= 169) {
      intb += 64;
    }
    
  } else if (zcode == 218 || zcode == 250) {
    
    /*        ASCII is assumed, on Prime machines - ZCODE is the ASCII code */
    /*        plus 128 of either lower or upper case 'Z'. */
    
    if (inta >= 225 && inta <= 250) {
      inta += -32;
    }
    if (intb >= 225 && intb <= 250) {
      intb += -32;
    }
  }
  ret_val = inta == intb;
  
  /*     RETURN */
  
  /*     End of LSAME */
  
  return ret_val;
} /* lsame */

int xerbla(char *srname, int *info)
{
  
  /*  -- LAPACK auxiliary routine (version 3.0) -- */
  /*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd., */
  /*     Courant Institute, Argonne National Lab, and Rice University */
  /*     September 30, 1994 */
  
  /*     .. Scalar Arguments .. */
  /*     .. */
  
  /*  Purpose */
  /*  ======= */
  
  /*  XERBLA  is an error handler for the LAPACK routines. */
  /*  It is called by an LAPACK routine if an input parameter has an */
  /*  invalid value.  A message is printed and execution stops. */
  
  /*  Installers may consider modifying the STOP statement in order to */
  /*  call system-specific exception-handling facilities. */
  
  /*  Arguments */
  /*  ========= */
  
  /*  SRNAME  (input) CHARACTER*6 */
  /*          The name of the routine which called XERBLA. */
  
  /*  INFO    (input) INTEGER */
  /*          The position of the invalid parameter in the parameter list */
  /*          of the calling routine. */
  
  /* ===================================================================== */
  
  printf("** On entry to %6s, parameter number %2i had an illegal value\n",
	 srname, *info);
  
  
  /*     End of XERBLA */
  return 0; 
} /* xerbla */



/****************************************************/
/***************** dmmul1.c file *********************/
/****************************************************/



/* Table of constant values */

// static int c__1 = 1;

int dmmul1(double *a, int *na, double *b, int *nb, double *c__, 
	   int *nc, int *l, int *m, int *n)
{
  /* System generated locals */
  int i__1, i__2;
  
  /* Local variables */
  double ddot();
  static int i__, j, ib, ic;
  
  /* !but */
  /*     ce sous programme effectue le produit matriciel: */
  /*     c=c+a*b . */
  /* !liste d'appel */
  
  /*     subroutine dmmul1(a,na,b,nb,c,nc,l,m,n) */
  /*     double precision a(na,m),b(nb,n),c(nc,n) */
  /*     integer na,nb,nc,l,m,n */
  
  /*     a            tableau de taille na*m contenant la matrice a */
  /*     na           nombre de lignes du tableau a dans le programme appel */
  /*     b,nb,c,nc    definitions similaires a celles de a,na */
  /*     l            nombre de ligne des matrices a et c */
  /*     m            nombre de colonnes de a et de lignes de b */
  /*     n            nombre de colonnes de b et c */
  /* !sous programmes utilises */
  /*     neant */
  /* ! */
  /*     Copyright INRIA */
  
  /* Parameter adjustments */
  --c__;
  --b;
  --a;
  
  /* Function Body */
  ib = 1;
  ic = 0;
  i__1 = *n;
  for (j = 1; j <= i__1; ++j) {
    i__2 = *l;
    for (i__ = 1; i__ <= i__2; ++i__) {
      /* L20: */
      c__[ic + i__] += ddot(m, &a[i__], na, &b[ib], &c__1);
    }
    ic += *nc;
    ib += *nb;
    /* L30: */
  }
  return 0;
} /* dmmul1 */

double ddot(int *n, double *dx, int *incx, double *dy, int *incy)
{
  /* System generated locals */
  int i__1;
  double ret_val;
  
  /* Local variables */
  static int i__, m;
  static double dtemp;
  static int ix, iy, mp1;
  
  
  /*     forms the dot product of two vectors. */
  /*     uses unrolled loops for increments equal to one. */
  /*     jack dongarra, linpack, 3/11/78. */
  /*     modified 12/3/93, array(1) declarations changed to array(*) */
  
  
  /* Parameter adjustments */
  --dy;
  --dx;
  
  /* Function Body */
  ret_val = 0.;
  dtemp = 0.;
  if (*n <= 0) {
    return ret_val;
  }
  if (*incx == 1 && *incy == 1) {
    goto L20;
  }
  
  /*        code for unequal increments or equal increments */
  /*          not equal to 1 */
  
  ix = 1;
  iy = 1;
  if (*incx < 0) {
    ix = (-(*n) + 1) * *incx + 1;
  }
  if (*incy < 0) {
    iy = (-(*n) + 1) * *incy + 1;
  }
  i__1 = *n;
  for (i__ = 1; i__ <= i__1; ++i__) {
    dtemp += dx[ix] * dy[iy];
    ix += *incx;
    iy += *incy;
    /* L10: */
  }
  ret_val = dtemp;
  return ret_val;
  
  /*        code for both increments equal to 1 */
  
  
  /*        clean-up loop */
  
 L20:
  m = *n % 5;
  if (m == 0) {
    goto L40;
  }
  i__1 = m;
  for (i__ = 1; i__ <= i__1; ++i__) {
    dtemp += dx[i__] * dy[i__];
    /* L30: */
  }
  if (*n < 5) {
    goto L60;
  }
 L40:
  mp1 = m + 1;
  i__1 = *n;
  for (i__ = mp1; i__ <= i__1; i__ += 5) {
    dtemp = dtemp + dx[i__] * dy[i__] + dx[i__ + 1] * dy[i__ + 1] + dx[
								       i__ + 2] * dy[i__ + 2] + dx[i__ + 3] * dy[i__ + 3] + dx[i__ + 
															       4] * dy[i__ + 4];
    /* L50: */
  }
 L60:
  ret_val = dtemp;
  return ret_val;
} /* ddot */

