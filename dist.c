#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "dist.h"


#define KF_GAMMA_EPS 1e-14
#define KF_TINY 1e-290

/* Log gamma function
 * \log{\Gamma(z)}
 * AS245, 2nd algorithm, http://lib.stat.cmu.edu/apstat/245
 */

double kf_lgamma(double z)
{
	double x = 0;
	x += 0.1659470187408462e-06 / (z+7);
	x += 0.9934937113930748e-05 / (z+6);
	x -= 0.1385710331296526     / (z+5);
	x += 12.50734324009056      / (z+4);
	x -= 176.6150291498386      / (z+3);
	x += 771.3234287757674      / (z+2);
	x -= 1259.139216722289      / (z+1);
	x += 676.5203681218835      / z;
	x += 0.9999999999995183;
	return log(x) - 5.58106146679532777 - z + (z-0.5) * log(z+6.5);
}

/* Regularized incomplete beta function. The method is taken from
 * Numerical Recipe in C, 2nd edition, section 6.4. The following web
 * page calculates the incomplete beta function, which equals
 * kf_betai(a,b,x) * gamma(a) * gamma(b) / gamma(a+b):
 *
 *   http://www.danielsoper.com/statcalc/calc36.aspx
 */

double kf_betai_aux(double a, double b, double x)
{
	double C, D, f;
	int j;
	if (x == 0.) return 0.;
	if (x == 1.) return 1.;
	f = 1.; C = f; D = 0.;
	// Modified Lentz's algorithm for computing continued fraction
	for (j = 1; j < 200; ++j) {
		double aa, d;
		int m = j>>1;
		aa = (j&1)? -(a + m) * (a + b + m) * x / ((a + 2*m) * (a + 2*m + 1))
			: m * (b - m) * x / ((a + 2*m - 1) * (a + 2*m));
		D = 1. + aa * D;
		if (D < KF_TINY) D = KF_TINY;
		C = 1. + aa / C;
		if (C < KF_TINY) C = KF_TINY;
		D = 1. / D;
		d = C * D;
		f *= d;
		if (fabs(d - 1.) < KF_GAMMA_EPS) break;
	}
	return exp(kf_lgamma(a+b) - kf_lgamma(a) - kf_lgamma(b) + a * log(x) + b * log(1.-x)) / a / f;
}

/* cdf/fdist.c
 *
 * Copyright (C) 2002 Przemyslaw Sliwa and Jason H. Stover.
 * Copyright (C) 2006, 2007 Brian Gough
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */


double gsl_cdf_fdist_P (const double x, const double nu1, const double nu2)
{
  double P;
  double r = nu2 / nu1;

  if (x < r)
    {
      double u = x / (r + x);

      P = kf_betai_aux(nu1 / 2.0, nu2 / 2.0, u);
    }
  else
    {
      double u = r / (r + x);

      P = kf_betai_aux(nu2 / 2.0, nu1 / 2.0, u);
    }

  return P;
}