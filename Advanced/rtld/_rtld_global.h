struct t_rtld_global = {
  _dl_ns = {{
      _ns_loaded = 0x7fa368631168, 
      _ns_nloaded = 0x4, 
      _ns_main_searchlist = 0x7fa368631420, 
      _ns_global_scope_alloc = 0x0, 
      _ns_unique_sym_table = {
        lock = {
          mutex = {
            __data = {
              __lock = 0x0, 
              __count = 0x0, 
              __owner = 0x0, 
              __nusers = 0x0, 
              __kind = 0x1, 
              __spins = 0x0, 
              __elision = 0x0, 
              __list = {
                __prev = 0x0, 
                __next = 0x0
              }
            }, 
            __size = '\000' <repeats 16 times>, "\001", '\000' <repeats 22 times>, 
            __align = 0x0
          }
        }, 
        entries = 0x0, 
        size = 0x0, 
        n_elements = 0x0, 
        free = 0x0
      }, 
      _ns_debug = {
        r_version = 0x0, 
        r_map = 0x0, 
        r_brk = 0x0, 
        r_state = RT_CONSISTENT, 
        r_ldbase = 0x0
      }
    }, {
      _ns_loaded = 0x0, 
      _ns_nloaded = 0x0, 
      _ns_main_searchlist = 0x0, 
      _ns_global_scope_alloc = 0x0, 
      _ns_unique_sym_table = {
        lock = {
          mutex = {
            __data = {
              __lock = 0x0, 
              __count = 0x0, 
              __owner = 0x0, 
              __nusers = 0x0, 
              __kind = 0x0, 
              __spins = 0x0, 
              __elision = 0x0, 
              __list = {
                __prev = 0x0, 
                __next = 0x0
              }
            }, 
            __size = '\000' <repeats 39 times>, 
            __align = 0x0
          }
        }, 
        entries = 0x0, 
        size = 0x0, 
        n_elements = 0x0, 
        free = 0x0
      }, 
      _ns_debug = {
        r_version = 0x0, 
        r_map = 0x0, 
        r_brk = 0x0, 
        r_state = RT_CONSISTENT, 
        r_ldbase = 0x0
      }
    } <repeats 15 times>}, 
  _dl_nns = 0x1, 
  _dl_load_lock = {
    mutex = {
      __data = {
        __lock = 0x0, 
        __count = 0x0, 
        __owner = 0x0, 
        __nusers = 0x0, 
        __kind = 0x1, 
        __spins = 0x0, 
        __elision = 0x0, 
        __list = {
          __prev = 0x0, 
          __next = 0x0
        }
      }, 
      __size = '\000' <repeats 16 times>, "\001", '\000' <repeats 22 times>, 
      __align = 0x0
    }
  }, 
  _dl_load_write_lock = {
    mutex = {
      __data = {
        __lock = 0x0, 
        __count = 0x0, 
        __owner = 0x0, 
        __nusers = 0x0, 
        __kind = 0x1, 
        __spins = 0x0, 
        __elision = 0x0, 
        __list = {
          __prev = 0x0, 
          __next = 0x0
        }
      }, 
      __size = '\000' <repeats 16 times>, "\001", '\000' <repeats 22 times>, 
      __align = 0x0
    }
  }, 
  _dl_load_adds = 0x4, 
  _dl_initfirst = 0x0, 
  _dl_cpuclock_offset = 0x169735dc1b63, 
  _dl_profile_map = 0x0, 
  _dl_num_relocations = 0x56, 
  _dl_num_cache_relocations = 0x3, 
  _dl_all_dirs = 0x7fa368631c20, 
  _dl_error_catch_tsd = 0x7fa36840ac80 <_dl_initial_error_catch_tsd>, 
  _dl_rtld_map = {
    l_addr = 0x7fa36840a000, 
    l_name = 0x400238 "/lib64/ld-linux-x86-64.so.2", 
    l_ld = 0x7fa36862fe80, 
    l_next = 0x0, 
    l_prev = 0x7fa368627000, 
    l_real = 0x7fa3686309d8 <_rtld_global+2456>, 
    l_ns = 0x0, 
    l_libname = 0x7fa368631020 <_dl_rtld_libname>, 
    l_info = {0x0, 0x0, 0x7fa36862ff00, 0x7fa36862fef0, 0x7fa36862fe90, 0x7fa36862feb0, 0x7fa36862fec0, 
      0x7fa36862ff30, 0x7fa36862ff40, 0x7fa36862ff50, 0x7fa36862fed0, 0x7fa36862fee0, 0x0, 0x0, 0x7fa36862fe80, 0x0, 
      0x0, 0x0, 0x0, 0x0, 0x7fa36862ff10, 0x0, 0x0, 0x7fa36862ff20, 0x0 <repeats 12 times>, 0x7fa36862ff70, 
      0x7fa36862ff60, 0x0, 0x0, 0x7fa36862ff90, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x7fa36862ff80, 
      0x0 <repeats 25 times>, 0x7fa36862fea0}, 
    l_phdr = 0x7fa36840a040, 
    l_entry = 0x0, 
    l_phnum = 0x7, 
    l_ldnum = 0x0, 
    l_searchlist = {
      r_list = 0x0, 
      r_nlist = 0x0
    }, 
    l_symbolic_searchlist = {
      r_list = 0x0, 
      r_nlist = 0x0
    }, 
    l_loader = 0x0, 
    l_versions = 0x7fa368627868, 
    l_nversions = 0x6, 
    l_nbuckets = 0x11, 
    l_gnu_bitmask_idxbits = 0x3, 
    l_gnu_shift = 0x8, 
    l_gnu_bitmask = 0x7fa36840a2c0, 
    {
      l_gnu_buckets = 0x7fa36840a2e0, 
      l_chain = 0x7fa36840a2e0
    }, 
    {
      l_gnu_chain_zero = 0x7fa36840a31c, 
      l_buckets = 0x7fa36840a31c
    }, 
    l_direct_opencount = 0x0, 
    l_type = lt_library, 
    l_relocated = 0x1, 
    l_init_called = 0x1, 
    l_global = 0x1, 
    l_reserved = 0x0, 
    l_phdr_allocated = 0x0, 
    l_soname_added = 0x0, 
    l_faked = 0x0, 
    l_need_tls_init = 0x0, 
    l_auditing = 0x0, 
    l_audit_any_plt = 0x0, 
    l_removed = 0x0, 
    l_contiguous = 0x0, 
    l_symbolic_in_local_scope = 0x0, 
    l_free_initfini = 0x0, 
    l_rpath_dirs = {
      dirs = 0x0, 
      malloced = 0x0
    }, 
    l_reloc_result = 0x0, 
    l_versyms = 0x7fa36840a7f0, 
    l_origin = 0x0, 
    l_map_start = 0x7fa36840a000, 
    l_map_end = 0x7fa368631168, 
    l_text_end = 0x7fa368428790, 
    l_scope_mem = {0x0, 0x0, 0x0, 0x0}, 
    l_scope_max = 0x0, 
    l_scope = 0x0, 
    l_local_scope = {0x0, 0x0}, 
    l_file_id = {
      dev = 0x0, 
      ino = 0x0
    }, 
    l_runpath_dirs = {
      dirs = 0x0, 
      malloced = 0x0
    }, 
    l_initfini = 0x0, 
    l_reldeps = 0x0, 
    l_reldepsmax = 0x0, 
    l_used = 0x1, 
    l_feature_1 = 0x0, 
    l_flags_1 = 0x0, 
    l_flags = 0x0, 
    l_idx = 0x0, 
    l_mach = {
      plt = 0x0, 
      gotplt = 0x0, 
      tlsdesc_table = 0x0
    }, 
    l_lookup_cache = {
      sym = 0x7fa36840a450, 
      type_class = 0x1, 
      value = 0x7fa368627000, 
      ret = 0x7fa368046ba0
    }, 
    l_tls_initimage = 0x0, 
    l_tls_initimage_size = 0x0, 
    l_tls_blocksize = 0x0, 
    l_tls_align = 0x0, 
    l_tls_firstbyte_offset = 0x0, 
    l_tls_offset = 0x0, 
    l_tls_modid = 0x0, 
    l_tls_dtor_count = 0x0, 
    l_relro_addr = 0x225bc0, 
    l_relro_size = 0x440, 
    l_serial = 0x0, 
    l_audit = 0x7fa368630e48 <_rtld_global+3592>
  }, 
  audit_data = {{
      cookie = 0x0, 
      bindflags = 0x0
    } <repeats 16 times>}, 
  _dl_rtld_lock_recursive = 0x7fa36840ac90 <rtld_lock_default_lock_recursive>, 
  _dl_rtld_unlock_recursive = 0x7fa36840aca0 <rtld_lock_default_unlock_recursive>, 
  _dl_make_stack_executable_hook = 0x7fa36841e0b0 <__GI__dl_make_stack_executable>, 
  _dl_stack_flags = 0x6, 
  _dl_tls_dtv_gaps = 0x0, 
  _dl_tls_max_dtv_idx = 0x1, 
  _dl_tls_dtv_slotinfo_list = 0x7fa3686278f8, 
  _dl_tls_static_nelem = 0x1, 
  _dl_tls_static_size = 0x1000, 
  _dl_tls_static_used = 0x78, 
  _dl_tls_static_align = 0x40, 
  _dl_initial_dtv = 0x7fa368625010, 
  _dl_tls_generation = 0x1, 
  _dl_init_static_tls = 0x7fa368416100 <_dl_nothread_init_static_tls>, 
  _dl_wait_lookup_done = 0x0, 
  _dl_scope_free_list = 0x0
};