#include "MonoApiTest.h"

#include <iostream>
#include <fstream>

MonoDomain* s_RootDomain;
MonoDomain* s_AppDomain;
MonoAssembly* s_AppAssembly;
namespace MonoApiTest {

	MonoDomain* InitMono()
	{
		std::cout << "MonoTest!\n";
		std::cout << "InitMono\n";
		mono_set_dirs(R"(C:\Program Files\Mono\lib)", R"(C:\Program Files\Mono\etc)");
		MonoDomain* domain = mono_jit_init("MonoTest");
		s_RootDomain = domain;
		std::cout << "Mono Runtime version: " << mono_get_runtime_build_info() << "\n";

		auto appDomain = mono_domain_create_appdomain(const_cast<char*>("MyAppDomain"), nullptr);
		mono_domain_set(appDomain, true);
		s_AppDomain = appDomain;

		auto assembly = LoadAssembly("assembly/MonoScript.dll");
		s_AppAssembly = assembly;
		PrintAssemblyTypes(assembly);

		auto obj = InstantiateClass("MonoScript", "ScriptTest");
		TestCallMethod(obj);
		TestField(obj);

		mono_jit_cleanup(domain);
		return nullptr;
		;
	}

	char* ReadBytes(const std::string& filepath, uint32_t* outSize)
	{
		std::ifstream in(filepath, std::ios::binary | std::ios::ate);
		if (!in)
			return nullptr;

		auto end = in.tellg();
		in.seekg(0, std::ios::beg);
		uint32_t size = (uint32_t)(end - in.tellg());
		if (size == 0)
			return nullptr;

		auto buffer = new char[size];
		in.read(buffer, size);

		*outSize = size;
		return buffer;
	}

	MonoAssembly* LoadAssembly(const std::string& assemblyPath)
	{
		uint32_t fileSize = 0;
		char* fileData = ReadBytes(assemblyPath, &fileSize);

		MonoImageOpenStatus status;
		MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);
		if (status != MONO_IMAGE_OK)
		{
			std::cerr << "[Error] " << mono_image_strerror(status) << '\n';
			return nullptr;
		}

		MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
		mono_image_close(image);

		delete[] fileData;

		return assembly;
	}

	void PrintAssemblyTypes(MonoAssembly* assembly)
	{
		auto image = mono_assembly_get_image(assembly);
		{
			const auto assemblyTable = mono_image_get_table_info(image, MONO_TABLE_ASSEMBLY);
			uint32_t acols[MONO_ASSEMBLY_SIZE];
			mono_metadata_decode_row(assemblyTable, 0, acols, MONO_ASSEMBLY_SIZE);
			printf("Assembly version: %d.%d\n", acols[MONO_ASSEMBLY_MAJOR_VERSION], acols[MONO_ASSEMBLY_MINOR_VERSION]);
		}

		// Log all types
		const auto typeTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int32_t typeCount = mono_table_info_get_rows(typeTable);

		for (auto i = 0; i < typeCount; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeTable, i, cols, MONO_TYPEDEF_SIZE);

			const auto nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			const auto name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
			printf("%s-%s\n", nameSpace, name);
		}
	}

	MonoClass* GetClassInAssembly(MonoAssembly* assembly, const char* name_space, const char* className)
	{
		MonoImage* image = mono_assembly_get_image(assembly);
		MonoClass* klass /* class */ = mono_class_from_name(image, name_space, className);
		if (klass == nullptr)
			return nullptr;

		return klass;
	}

	MonoObject* InstantiateClass(const char* name_space, const char* className)
	{
		auto klass = GetClassInAssembly(s_AppAssembly, name_space, className);
		auto obj = mono_object_new(s_AppDomain, klass);
		mono_runtime_object_init(obj);
		return obj;
	}

	void TestCallMethod(MonoObject* obj)
	{
		auto klass = mono_object_get_class(obj);
		auto argumentLessMethod = mono_class_get_method_from_name(klass, "PrintMessage", 0);
		if (argumentLessMethod)
		{
			mono_runtime_invoke(argumentLessMethod, obj, nullptr, nullptr);
		}

		auto oneArgumentMethod = mono_class_get_method_from_name(klass, "PrintMessage", 1);
		if (oneArgumentMethod)
		{
			int value = 5;
			void* args = &value;

			mono_runtime_invoke(oneArgumentMethod, obj, &args, nullptr);
		}

		auto staticMethod = mono_class_get_method_from_name(klass, "PrintMessage", 2);;
		if (staticMethod)
		{
			void* args[2];
			args[0] = mono_string_new(s_AppDomain, "The value");
			args[1] = mono_string_new(s_AppDomain, "The message");
			mono_runtime_invoke(staticMethod, nullptr, args, nullptr);
		}
	}

	void TestField(MonoObject* obj)
	{
		auto klass = mono_object_get_class(obj);
		auto intField = mono_class_get_field_from_name(klass, "PublicIntVal");
		auto nameField = mono_class_get_field_from_name(klass, "_name");
		auto nameProperty = mono_class_get_property_from_name(klass, "Name");

		int intValue;
		mono_field_get_value(obj, intField, &intValue);
		std::cout << "Int value: " << intValue << '\n';
		intValue += 10;
		mono_field_set_value(obj, intField, &intValue);

		auto name = mono_property_get_value(nameProperty, obj, nullptr, nullptr);
		std::string nameStr = mono_string_to_utf8(mono_object_to_string(name, nullptr));
		std::cout << "Name property: " << nameStr << '\n';

		nameStr += ">> C++ concat";
		auto nameValue = mono_string_new(s_AppDomain, nameStr.c_str());
		mono_property_set_value(nameProperty, obj, (void**)&nameValue, nullptr);

		auto argumentLessMethod = mono_class_get_method_from_name(klass, "PrintMessage", 0);
		if (argumentLessMethod)
		{
			mono_runtime_invoke(argumentLessMethod, obj, nullptr, nullptr);
		}
	}
}