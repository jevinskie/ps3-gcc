/* Vector API for GNU compiler.
   Copyright (C) 2004 Free Software Foundation, Inc.
   Contributed by Nathan Sidwell <nathan@codesourcery.com>

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "ggc.h"
#include "vec.h"
#include "errors.h"
#include "coretypes.h"
#include "tree.h"

struct vec_prefix 
{
  size_t num;
  size_t alloc;
  void *vec[1];
};

/* Ensure there are at least RESERVE free slots in VEC, if RESERVE !=
   ~0u. If RESERVE == ~0u increase the current allocation
   exponentially.  VEC can be NULL, to create a new vector.  */

void *
vec_p_reserve (void *vec, size_t reserve MEM_STAT_DECL)
{
  return vec_o_reserve (vec, reserve,
			offsetof (struct vec_prefix, vec), sizeof (void *)
			PASS_MEM_STAT);
}

/* Ensure there are at least RESERVE free slots in VEC, if RESERVE !=
   ~0u.  If RESERVE == ~0u, increase the current allocation
   exponentially.  VEC can be NULL, in which case a new vector is
   created.  The vector's trailing array is at VEC_OFFSET offset and
   consistes of ELT_SIZE sized elements.  */

void *
vec_o_reserve (void *vec, size_t reserve, size_t vec_offset, size_t elt_size
	       MEM_STAT_DECL)
{
  struct vec_prefix *pfx = vec;
  size_t alloc;

  if (reserve + 1)
    alloc = (pfx ? pfx->num : 0) + reserve;
  else
    alloc = pfx ? pfx->alloc * 2 : 4;
  
  if (!pfx || pfx->alloc < alloc)
    {
      vec = ggc_realloc_stat (vec, vec_offset + alloc * elt_size
			      PASS_MEM_STAT);
      ((struct vec_prefix *)vec)->alloc = alloc;
      if (!pfx)
	((struct vec_prefix *)vec)->num = 0;
    }
  
  return vec;
}

#if ENABLE_CHECKING
/* Issue a vector domain error, and then fall over.  */

void
vec_assert_fail (const char *op, const char *struct_name,
		 const char *file, size_t line, const char *function)
{
  internal_error ("vector %s %s domain error, in %s at %s:%u",
		  struct_name, op, function, function,
		  trim_filename (file), line);
}
#endif
