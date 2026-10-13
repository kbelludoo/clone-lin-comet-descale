/* Independent C11 oracle for Compound III accrualDescaleFactor. See kbelludoo/lin-open proofs. */
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BASE_ACCRUAL 1000000ULL
#define WAD 1000000000000000000ULL
static uint64_t muldiv_i128(uint64_t a, uint64_t b, uint64_t d) {
    __uint128_t q; if (d == 0) return 0;
    q = ((__uint128_t)a * (__uint128_t)b) / (__uint128_t)d;
    if (q > UINT64_MAX) return 0; return (uint64_t)q;
}
static uint64_t factor_u(uint64_t base_scale) {
    if (base_scale < BASE_ACCRUAL) return 0;
    return muldiv_i128(base_scale, 1, BASE_ACCRUAL);
}
static uint64_t accrued_u(uint64_t principal, uint64_t index_delta, uint64_t tracking_scale, uint64_t base_scale) {
    uint64_t f, step;
    if (principal == 0 || tracking_scale == 0) return 0;
    f = factor_u(base_scale); if (f == 0) return 0;
    step = muldiv_i128(principal, index_delta, tracking_scale);
    return muldiv_i128(step, 1, f);
}
int main(int argc, char **argv) {
    if (argc >= 2 && strcmp(argv[1], "selftest") == 0) {
        if (factor_u(1000000ULL) != 1ULL) return 1;
        if (factor_u(1000000000000000000ULL) != 1000000000000ULL) return 1;
        if (accrued_u(1000000ULL, 1000000000000000ULL, 1000000000000000ULL, 1000000ULL) != 1000000ULL) return 1;
        if (accrued_u(1000000000000000000ULL, 1000000000000000ULL, 1000000000000000ULL, 1000000000000000000ULL) != 1000000ULL) return 1;
        printf("PASS\n"); return 0;
    }
    fprintf(stderr, "usage: comet_descale_c11 selftest\n"); return 2;
}
