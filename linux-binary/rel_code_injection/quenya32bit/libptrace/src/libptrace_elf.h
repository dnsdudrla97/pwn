/* libptrace, a process tracing and manipulation library.
 *
 * Dedicated to Merle Planten.
 *
 * Copyright (C) 2006-2008 Ronald Huizer
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#ifndef __LIBPTRACE_ELF_H

#include <link.h>

#define __RTLD_DLOPEN 0x80000000

int ptrace_elf_get_link_map_head(struct ptrace_context *pctx,
                                 struct link_map *map);
int ptrace_elf_get_link_map_next(struct ptrace_context *pctx,
                                 struct link_map *map,
				 struct link_map *next);
void *ptrace_elf_get_dynamic_entry(struct ptrace_context *pctx,
                                   struct link_map *map, Elf32_Sword tag);
void *ptrace_elf_get_symtab(struct ptrace_context *pctx,
                            struct link_map *map);
void *ptrace_elf_get_strtab(struct ptrace_context *pctx,
                            struct link_map *map);
void *ptrace_elf_get_hash(struct ptrace_context *pctx,
                          struct link_map *map);
int ptrace_elf_get_hash_chains(struct ptrace_context *pctx,
                               struct link_map *map, Elf32_Word *chains);
void *ptrace_elf_get_symbol_addr(struct ptrace_context *pctx,
                                 struct link_map *map, const char *symbol);
void *ptrace_dlopen(struct ptrace_context *pctx, const char *libname,
                    int flags);
void * ptrace_dlsym(struct ptrace_context *pctx, void *handle,
                    const char *symbol);
int ptrace_dlclose(struct ptrace_context *pctx, void *handle);
//ptrace_function_ptr_t
//ptrace_library_get_function_addr(struct ptrace_context *pctx,
//                                 ptrace_library_handle_t handle,
//                                 const char *symbol);
#endif	/* __LIBPTRACE_ELF_H */
