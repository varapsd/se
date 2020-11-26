/*
*Functions for calculating probability distributions
*/

#ifndef _DIST_
#define _DIST_

double kf_lgamma(double z);
double kf_betai_aux(double a, double b, double x);
double gsl_cdf_fdist_P (const double x, const double nu1, const double nu2);

#endif