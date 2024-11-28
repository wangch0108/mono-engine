#pragma once

struct MonoDomain;
struct MonoAssembly;
struct MonoImage;
struct MonoClass;
struct MonoObject;
struct MonoMethodSignature;
struct MonoException;
struct MonoType;

#if USE_MONO_METADATA
struct MonoTableInfo;
enum
{
	MONO_TABLE_TYPEDEF = 2,

	MONO_TYPEDEF_NAME = 1,
	MONO_TYPEDEF_NAMESPACE = 2,
	MONO_TYPEDEF_SIZE = 6,
};
#endif

typedef unsigned short guint16;
typedef unsigned int guint32;
typedef unsigned short mono_unichar2; // NO _WIN32
typedef int mono_bool;

struct MonoString /* _MonoString */
{
	// MonoObject monoObject;
	void* monoObjectPart1;
	void* monoObjectPart2;
	int32_t length;
	mono_unichar2 firstCharacter;
};

struct MonoMethod /* _MonoMethod */
{
	guint16 flags;
	guint16 iflags;
};

typedef MonoObject* MonoNativeObjectPtr;
typedef MonoString* MonoNativeStringPtr;

struct MonoObjectNull
{
	MonoObjectNull() = default;

	operator MonoObject* () const { return MonoNativeObjectPtr(); }
	operator MonoString* () const { return MonoNativeStringPtr(); }

	bool operator==(const MonoNativeObjectPtr& obj) const { return obj == MonoObjectNull(); }
	bool operator!=(const MonoNativeObjectPtr& obj) const { return obj != MonoObjectNull(); }
};

/* inline */ constexpr MonoObjectNull GMonoObjectNull;
#define MONO_NULL GMonoObjectNull

/*
 * Unity实际把这部分抽象成一些基础类型, 像是ScriptingClassPtr等
 * 但是这里是关于Mono的实现示例，为了避免混淆，我尽量不做这样的封装
 */

class MonoClassPtr
{
public:
	MonoClassPtr() : _nativePtr(nullptr) {}
	MonoClassPtr(const MonoObjectNull&) : _nativePtr(nullptr) {}
	MonoClassPtr(MonoClass* pointer) : _nativePtr(pointer) {}

	void* Hash() const { return (void*)(uintptr_t)_nativePtr; }

	bool operator==(void* other) const { return _nativePtr == other; }
	bool operator!=(void* other) const { return _nativePtr != other; }
	bool operator==(const MonoObjectNull&) const { return _nativePtr == nullptr; }
	bool operator!=(const MonoObjectNull&) const { return _nativePtr != nullptr; }
	bool operator==(const MonoClassPtr& other) const { return _nativePtr == other._nativePtr; }
	bool operator!=(const MonoClassPtr& other) const { return _nativePtr != other._nativePtr; }
	operator MonoClass* () const { return _nativePtr; }
private:
	MonoClass* _nativePtr;
};

class MonoMethodPtr
{
public:
	MonoMethodPtr() : _nativePtr(nullptr) {}
	MonoMethodPtr(MonoMethod* pointer) : _nativePtr(pointer) {}

	inline MonoMethod* GetNativeMethod() const
	{
		return _nativePtr;
	}

	inline bool IsNull() const
	{
		return _nativePtr == nullptr;
	}

	bool IsInstance() const;

	bool operator==(void* other) const { return _nativePtr == other; }
	bool operator!=(void* other) const { return _nativePtr != other; }
	bool operator==(const MonoMethodPtr& other) const { return _nativePtr == other._nativePtr; }
	bool operator!=(const MonoMethodPtr& other) const { return _nativePtr != other._nativePtr; }

	operator MonoMethod* () const { return _nativePtr; }

private:
	MonoMethod* _nativePtr;
};

class MonoImagePtr
{
public:
	MonoImagePtr() : _nativePtr(nullptr) {}
	MonoImagePtr(const MonoObjectNull&) : _nativePtr(nullptr) {}
	MonoImagePtr(MonoImage* pointer) : _nativePtr(pointer) {}

	bool operator==(void* other) const { return _nativePtr == other; }
	bool operator!=(void* other) const { return _nativePtr != other; }
	bool operator==(const MonoObjectNull&) const { return _nativePtr == nullptr; }
	bool operator!=(const MonoObjectNull&) const { return _nativePtr != nullptr; }
	bool operator==(const MonoImagePtr& other) const { return _nativePtr == other._nativePtr; }
	bool operator!=(const MonoImagePtr& other) const { return _nativePtr != other._nativePtr; }

	operator MonoImage* () const { return _nativePtr; }

private:
	MonoImage* _nativePtr;
};

class MonoStringPtr;
class MonoObjectPtr
{
public:
	MonoObjectPtr() : _nativePtr(nullptr) {}
	MonoObjectPtr(const MonoObjectNull&) : _nativePtr(MonoObjectNull()) {}
	MonoObjectPtr(MonoObject* pointer) : _nativePtr(pointer) {}

	bool operator==(void* other) const { return _nativePtr == other; }
	bool operator!=(void* other) const { return _nativePtr != other; }
	bool operator==(const MonoObjectNull&) const { return _nativePtr == MonoObjectNull(); }
	bool operator!=(const MonoObjectNull&) const { return _nativePtr != MonoObjectNull(); }
	bool operator==(const MonoObjectPtr& other) const { return _nativePtr == other._nativePtr; }
	bool operator!=(const MonoObjectPtr& other) const { return _nativePtr != other._nativePtr; }

	operator bool() const { return _nativePtr != MonoObjectNull(); }
	operator MonoObject* () const { return _nativePtr; }
	MonoObject* GetNativePtr() const { return _nativePtr; }

	MonoStringPtr ToMonoString() const;
	MonoStringPtr ConvertToString() const;
protected:
	MonoObject* _nativePtr;
};

class MonoExceptionPtr : public MonoObjectPtr
{
public:
	MonoExceptionPtr() {}
	MonoExceptionPtr(const MonoObjectNull&) {}
	MonoExceptionPtr(MonoObjectPtr o) : MonoObjectPtr(o) {}
	MonoExceptionPtr(MonoException* e) : MonoObjectPtr((MonoObject*) e) {}

	operator MonoException*() const
	{
		return (MonoException*)_nativePtr;
	}
};

class MonoStringPtr /* : MonoObjectPtr */
{
public:
	MonoStringPtr() : _nativePtr(nullptr) {}
	MonoStringPtr(const MonoObjectNull&) : _nativePtr(MonoObjectNull()) {}
	MonoStringPtr(MonoString* pointer) : _nativePtr(pointer) {}

	bool operator==(void* other) const { return _nativePtr == other; }
	bool operator!=(void* other) const { return _nativePtr != other; }
	bool operator==(const MonoObjectNull&) const { return _nativePtr == MonoObjectNull(); }
	bool operator!=(const MonoObjectNull&) const { return _nativePtr != MonoObjectNull(); }
	bool operator==(const MonoStringPtr& other) const { return _nativePtr == other._nativePtr; }
	bool operator!=(const MonoStringPtr& other) const { return _nativePtr != other._nativePtr; }

	operator MonoString* () const { return _nativePtr; }

private:
	MonoString* _nativePtr;
};

#define INVALID_GC_HANDLE ((uint32_t)-1)
enum MonoGCHandleWeakness
{
	GCHANDLE_INVALID,
	GCHANDLE_WEAK,
	GCHANDLE_STRONG,
};

class MonoGCHandle
{
public:
	MonoGCHandle() { Acquire(MONO_NULL, GCHANDLE_INVALID); }
	MonoGCHandle(MonoObjectPtr obj, MonoGCHandleWeakness weakness) { Acquire(obj, weakness); }

	bool operator==(const MonoGCHandle& other) const { return _handle == other._handle; }
	bool operator!=(const MonoGCHandle& other) const { return _handle != other._handle; }

	__forceinline bool HasTarget() const;
	__forceinline MonoObjectPtr Resolve() const;

	void Release();
	void AcquireWeak(MonoObjectPtr obj);
	void AcquireStrong(MonoObjectPtr obj);

private:
	uint32_t _handle;
	MonoGCHandleWeakness _weakness;

	void Acquire(MonoObjectPtr obj, MonoGCHandleWeakness weakness);
	static MonoObjectPtr ResolveGCHandle(uint32_t handle);
};

__forceinline bool MonoGCHandle::HasTarget() const
{
	return _handle != INVALID_GC_HANDLE;
}

__forceinline MonoObjectPtr MonoGCHandle::Resolve() const
{
	if (!HasTarget())
		return MONO_NULL;
	return ResolveGCHandle(_handle);
}
