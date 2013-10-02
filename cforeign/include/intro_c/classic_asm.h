#ifndef CLASSIC_ASM_H
#define CLASSIC_ASM_H

#ifdef __cplusplus
extern "C" {
#endif

unsigned long fact_asm_i(const unsigned int n);
unsigned long fact_asm_lp(const unsigned int n);

float expt_asm_i(const float b, const float n);
float expt_asm_lp(const float b, const float n);


#ifdef __cplusplus
}
#endif

#endif // CLASSIC_ASM_H

