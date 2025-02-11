/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_ATOMIC64_64_H
#define _ASM_X86_ATOMIC64_64_H

#include <linux/types.h>
#include <asm/alternative.h>
#include <asm/cmpxchg.h>

/* The 64-bit atomic type */

#define ATOMIC64_INIT(i)	{ (i) }

/**
 * arch_atomic64_read - read atomic64 variable
 * @v: pointer of type atomic64_t
 *
 * Atomically reads the value of @v.
 * Doesn't imply a read memory barrier.
 */
static inline long arch_atomic64_read(const atomic64_t *v)
{
	return READ_ONCE((v)->counter);
}

/**
 * arch_atomic64_set - set atomic64 variable
 * @v: pointer to type atomic64_t
 * @i: required value
 *
 * Atomically sets the value of @v to @i.
 */
static inline void arch_atomic64_set(atomic64_t *v, long i)
{
	WRITE_ONCE(v->counter, i);
}

/**
 * arch_atomic64_add - add integer to atomic64 variable
 * @i: integer value to add
 * @v: pointer to type atomic64_t
 *
 * Atomically adds @i to @v.
 */
static __always_inline void arch_atomic64_add(long i, atomic64_t *v)
{
	asm volatile(LOCK_PREFIX "addq %1,%0"
		     : "=m" (v->counter)
		     : "er" (i), "m" (v->counter) : "memory");
}

/**
 * arch_atomic64_sub - subtract the atomic64 variable
 * @i: integer value to subtract
 * @v: pointer to type atomic64_t
 *
 * Atomically subtracts @i from @v.
 */
static inline void arch_atomic64_sub(long i, atomic64_t *v)
{
	asm volatile(LOCK_PREFIX "subq %1,%0"
		     : "=m" (v->counter)
		     : "er" (i), "m" (v->counter) : "memory");
}

/**
 * arch_atomic64_sub_and_test - subtract value from variable and test result
 * @i: integer value to subtract
 * @v: pointer to type atomic64_t
 *
 * Atomically subtracts @i from @v and returns
 * true if the result is zero, or false for all
 * other cases.
 */
#define arch_atomic64_sub_and_test arch_atomic64_sub_and_test
static inline bool arch_atomic64_sub_and_test(long i, atomic64_t *v)
{
	GEN_BINARY_RMWcc(LOCK_PREFIX "subq", v->counter, "er", i, "%0", e);
}
#define arch_atomic64_sub_and_test arch_atomic64_sub_and_test

/**
 * arch_atomic64_inc - increment atomic64 variable
 * @v: pointer to type atomic64_t
 *
 * Atomically increments @v by 1.
 */
#define arch_atomic64_inc arch_atomic64_inc
static __always_inline void arch_atomic64_inc(atomic64_t *v)
{
	asm volatile(LOCK_PREFIX "incq %0"
		     : "=m" (v->counter)
		     : "m" (v->counter) : "memory");
}
#define arch_atomic64_inc arch_atomic64_inc

/**
 * arch_atomic64_dec - decrement atomic64 variable
 * @v: pointer to type atomic64_t
 *
 * Atomically decrements @v by 1.
 */
#define arch_atomic64_dec arch_atomic64_dec
static __always_inline void arch_atomic64_dec(atomic64_t *v)
{
	asm volatile(LOCK_PREFIX "decq %0"
		     : "=m" (v->counter)
		     : "m" (v->counter) : "memory");
}
#define arch_atomic64_dec arch_atomic64_dec

/**
 * arch_atomic64_dec_and_test - decrement and test
 * @v: pointer to type atomic64_t
 *
 * Atomically decrements @v by 1 and
 * returns true if the result is 0, or false for all other
 * cases.
 */
#define arch_atomic64_dec_and_test arch_atomic64_dec_and_test
static inline bool arch_atomic64_dec_and_test(atomic64_t *v)
{
	GEN_UNARY_RMWcc(LOCK_PREFIX "decq", v->counter, "%0", e);
}
#define arch_atomic64_dec_and_test arch_atomic64_dec_and_test

/**
 * arch_atomic64_inc_and_test - increment and test
 * @v: pointer to type atomic64_t
 *
 * Atomically increments @v by 1
 * and returns true if the result is zero, or false for all
 * other cases.
 */
#define arch_atomic64_inc_and_test arch_atomic64_inc_and_test
static inline bool arch_atomic64_inc_and_test(atomic64_t *v)
{
	GEN_UNARY_RMWcc(LOCK_PREFIX "incq", v->counter, "%0", e);
}
#define arch_atomic64_inc_and_test arch_atomic64_inc_and_test

/**
 * arch_atomic64_add_negative - add and test if negative
 * @i: integer value to add
 * @v: pointer to type atomic64_t
 *
 * Atomically adds @i to @v and returns true
 * if the result is negative, or false when
 * result is greater than or equal to zero.
 */
#define arch_atomic64_add_negative arch_atomic64_add_negative
static inline bool arch_atomic64_add_negative(long i, atomic64_t *v)
{
	GEN_BINARY_RMWcc(LOCK_PREFIX "addq", v->counter, "er", i, "%0", s);
}
#define arch_atomic64_add_negative arch_atomic64_add_negative

/**
 * arch_atomic64_add_return - add and return
 * @i: integer value to add
 * @v: pointer to type atomic64_t
 *
 * Atomically adds @i to @v and returns @i + @v
 */
static __always_inline long arch_atomic64_add_return(long i, atomic64_t *v)
{
	return i + xadd(&v->counter, i);
}

static inline long arch_atomic64_sub_return(long i, atomic64_t *v)
{
	return arch_atomic64_add_return(-i, v);
}

static inline long arch_atomic64_fetch_add(long i, atomic64_t *v)
{
	return xadd(&v->counter, i);
}

static inline long arch_atomic64_fetch_sub(long i, atomic64_t *v)
{
	return xadd(&v->counter, -i);
}

static inline long arch_atomic64_cmpxchg(atomic64_t *v, long old, long new)
{
	return arch_cmpxchg(&v->counter, old, new);
}

#define arch_atomic64_try_cmpxchg arch_atomic64_try_cmpxchg
static __always_inline bool arch_atomic64_try_cmpxchg(atomic64_t *v, s64 *old, long new)
{
	return try_cmpxchg(&v->counter, old, new);
}

static inline long arch_atomic64_xchg(atomic64_t *v, long new)
{
	return arch_xchg(&v->counter, new);
}

static inline void arch_atomic64_and(long i, atomic64_t *v)
{
	asm volatile(LOCK_PREFIX "andq %1,%0"
			: "+m" (v->counter)
			: "er" (i)
			: "memory");
}

static inline long arch_atomic64_fetch_and(long i, atomic64_t *v)
{
	s64 val = arch_atomic64_read(v);

	do {
	} while (!arch_atomic64_try_cmpxchg(v, &val, val & i));
	return val;
}

static inline void arch_atomic64_or(long i, atomic64_t *v)
{
	asm volatile(LOCK_PREFIX "orq %1,%0"
			: "+m" (v->counter)
			: "er" (i)
			: "memory");
}

static inline long arch_atomic64_fetch_or(long i, atomic64_t *v)
{
	s64 val = arch_atomic64_read(v);

	do {
	} while (!arch_atomic64_try_cmpxchg(v, &val, val | i));
	return val;
}

static inline void arch_atomic64_xor(long i, atomic64_t *v)
{
	asm volatile(LOCK_PREFIX "xorq %1,%0"
			: "+m" (v->counter)
			: "er" (i)
			: "memory");
}

static inline long arch_atomic64_fetch_xor(long i, atomic64_t *v)
{
	s64 val = arch_atomic64_read(v);

	do {
	} while (!arch_atomic64_try_cmpxchg(v, &val, val ^ i));
	return val;
}

#endif /* _ASM_X86_ATOMIC64_64_H */
