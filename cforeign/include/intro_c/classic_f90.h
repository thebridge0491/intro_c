#ifndef CLASSIC_F90_H
#define CLASSIC_F90_H

#ifdef __cplusplus
extern "C" {
#endif

unsigned long fact_f90_i(const unsigned int n);
unsigned long fact_f90_lp(const unsigned int n);

float expt_f90_i(const float b, const float n);
float expt_f90_lp(const float b, const float n);

#ifdef __cplusplus
}
#endif

#endif // CLASSIC_F90_H

