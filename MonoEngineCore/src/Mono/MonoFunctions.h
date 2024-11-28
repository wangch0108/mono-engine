
/*
 * All mono functions declaration and implement(lookup symbol from mono dynamic library)
 * DO_API(r, n, p) is result, name(symbol name), parameter
 */

#ifndef DO_API
#DO_API(r, n, p) r n p;
#endif

DO_API(MonoDomain*, mono_jit_init_version, (const char* file, const char* runtime_version))
DO_API(char*, mono_get_runtime_build_info, ())
DO_API(void, mono_set_dirs, (const char* assembly_dir, const char* config_dir))
DO_API(void, mono_config_parse, (const char* filename))

DO_API(MonoDomain*, mono_get_root_domain, (void)) // Get root domain
DO_API(MonoDomain*, mono_domain_get, (void)) // Get Activated domain(AppDomain)
DO_API(mono_bool, mono_domain_set, (MonoDomain* domain, mono_bool force))
DO_API(MonoDomain*, mono_domain_create_appdomain, (char* friendly_name, char* configuration_file))
DO_API(void, mono_domain_unload, (MonoDomain* domain))

DO_API(MonoAssembly*, mono_domain_assembly_open, (MonoDomain* domain, const char* name))
DO_API(MonoImage*, mono_image_open_from_data_with_name, (char* data, uint32_t data_len, mono_bool need_copy, int* status, mono_bool refonly, const char* name))
DO_API(MonoAssembly*, mono_assembly_load_from_full, (MonoImage* image, const char* fname, int* status, mono_bool refonly))
DO_API(void, mono_image_close, (MonoImage* image))
DO_API(MonoImage*, mono_assembly_get_image, (MonoAssembly* assembly))
DO_API(MonoImage*, mono_get_corlib, (void))

/* Class */
DO_API(MonoClass*, mono_class_get_parent, (MonoClass* klass))
DO_API(const char*, mono_class_get_name, (MonoClass* klass))
DO_API(MonoClass*, mono_class_from_name, (MonoImage* image, const char* name_space, const char* name))
DO_API(MonoClass*, mono_class_from_mono_type, (MonoType* type))
DO_API(mono_bool, mono_class_is_subclass_of, (MonoClass* klass, MonoClass* klassc, mono_bool check_interfaces))

/* Object */
DO_API(MonoString*, mono_string_new_wrapper, (const char* text))
DO_API(MonoString*, mono_object_to_string, (MonoObject* obj, MonoObject** exc))
DO_API(char*, mono_string_to_utf8, (MonoString* string_obj))
DO_API(MonoObject*, mono_object_new, (MonoDomain* domain, MonoClass* klass))
DO_API(void, mono_runtime_object_init, (MonoObject* this_obj))

/* Method */
DO_API(MonoMethod*, mono_class_get_methods, (MonoClass* klass, void** iter))
DO_API(MonoMethodSignature*, mono_method_signature, (MonoMethod* method))
DO_API(int, mono_signature_is_instance, (MonoMethodSignature* sig))
DO_API(const char*, mono_method_get_name, (MonoMethod* method))
DO_API(uint32_t, mono_signature_get_param_count, (MonoMethodSignature* sig))
DO_API(MonoMethod*, mono_class_get_method_from_name, (MonoClass* klass, const char* name, int param_count))
DO_API(MonoObject*, mono_runtime_invoke, (MonoMethod* method, void* obj, void** params, MonoObject** exc))

/* Invoke */
DO_API(void, mono_add_internal_call, (const char* name, const void* method))

/* Debug */
DO_API(void, mono_trace_set_level_string, (const char* value))
DO_API(void, mono_trace_set_mask_string, (const char* value);)
typedef void (*MonoLogCallback) (const char* log_domain, const char* log_level, const char* message, int fatal, void* user_data);
DO_API(void, mono_trace_set_log_handler, (MonoLogCallback callback, void* user_data))

#if USE_MONO_METADATA
DO_API(MonoTableInfo*, mono_image_get_table_info, (MonoImage* image, int table_id))
DO_API(int, mono_image_get_table_rows, (MonoImage* image, int table_id))
DO_API(int, mono_table_info_get_rows, (const MonoTableInfo* table))
DO_API(void, mono_metadata_decode_row, (const MonoTableInfo* t, int idx, uint32_t* res, int res_size))
DO_API(const char*, mono_metadata_string_heap, (MonoImage* meta, uint32_t table_index))
#endif

DO_API(void, mono_gc_collect, (int generation))
DO_API(int, mono_gc_max_generation, (void))
DO_API(uint32_t, mono_gchandle_new, (MonoObject* obj, mono_bool pinned))
DO_API(void, mono_gchandle_free, (uint32_t gchandle))
DO_API(MonoObject*, mono_gchandle_get_target, (uint32_t gchandle))
DO_API(uint32_t, mono_gchandle_new_weakref, (MonoObject* obj, mono_bool track_resurrection))

DO_API(void, mono_set_signal_chaining, (int chain_signals))
DO_API(int, mono_parse_default_optimizations, (const char* p))
DO_API(void, mono_set_defaults, (int verbose_level, guint32 opts))

#undef DO_API
#undef DO_API_OPTIONAL