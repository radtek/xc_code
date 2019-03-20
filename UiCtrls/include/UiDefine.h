
#pragma once

#define MIN_VALUE (1e-9) 
#define IS_DOUBLE_ZERO(d) (fabs(d) < MIN_VALUE)
#define IS_DOUBLE_EQUAL(d1, d2) (IS_DOUBLE_ZERO(d1 - d2))

#define IS_DOUBLE_GREAT(d1, d2) ( !IS_DOUBLE_EQUAL(d1, d2) && (d1 > d2) )
#define IS_DOUBLE_LESS(d1, d2) ( !IS_DOUBLE_EQUAL(d1, d2) && (d1 < d2) )
