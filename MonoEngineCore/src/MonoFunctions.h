DO_API(MonoDomain*, mono_jit_init_version, (const char* file, const char* runtime_version))
DO_API(char*, mono_get_runtime_build_info, ())

#undef DO_API
#undef DO_API_NO_RETURN
#undef DO_API_OPTIONAL