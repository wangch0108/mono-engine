#pragma once

// Unused macros, test only

#define DETAIL_PP_EXPAND_2(A_, B_) A_ B_

/*
// macro expansion example(unity comment: visual c++ requires two levels to ensure proper macro expansion)
#define PP_CAT(a, b) a##b
#define PP_CAT2(a, b) PP_CAT(a, b)
#define PP_CAT3(a, b) PP_CAT2(a, b)

#define E(a) QQ a
#define QQ() Q

#define T() PP_CAT(_, E(())) // _E(())
#define T2() PP_CAT2(_, E(())) // _Q
#define T3() PP_CAT3(_, E(())) // _Q
*/

#define DETAIL_PP_VARG_CONCAT_Y(A_, B_) A_##B_
#define DETAIL_PP_VARG_CONCAT_X(A_, B_) DETAIL_PP_VARG_CONCAT_Y(A_, B_)
#define DETAIL_PP_VARG_CONCAT(A_, B_) DETAIL_PP_VARG_CONCAT_X(A_, B_)
#define DETAIL_PP_VARG_COUNT(ARG0_, ARG1_, ARG2_, ARG3_, ARG4_, ARG5_, ARG6_, ARG7_, ARG8_, ARG9_, ARG10_, ARG11_, ARG12_, ARG13_, ARG14_, ARG15_, ARG16_, ARG17_, ARG18_, ARG19_, RESULT_, ...) RESULT_

// 0 is not supported
#define PP_VARG_COUNT(...) \
	DETAIL_PP_EXPAND_2(DETAIL_PP_VARG_COUNT, (__VA_ARGS__, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1))

#define PP_VARG_SELECT_OVERLOAD(NAME_, ARGS_) \
	DETAIL_PP_EXPAND_2(DETAIL_PP_VARG_CONCAT(NAME_, PP_VARG_COUNT ARGS_), ARGS_)

#define IMPLEMENT_REGISTER_CLASS(...) PP_VARG_SELECT_OVERLOAD(IMPLEMENT_REGISTER_CLASS_,(__VA_ARGS__))

// This code appears in the unity source code for RTTI designs.

class Object;
class BASE /* BaseObject */
{
public:
	typedef BASE ThisType;
};

#define REGISTER_CLASS(TYPE_NAME_) \
public: \
	typedef ThisType Super; \
	typedef TYPE_NAME_ ThisType;

class DRIVER1 : BASE
{
	REGISTER_CLASS(DRIVER1)
};

class DRIVER2 : DRIVER1
{
	REGISTER_CLASS(DRIVER2)
};

 /*
  * Unity RTTI Registration Flow (Notes only; this has not been implemented for us)
  * 1. Use REGISTER_CLASS(TYPE_NAME_) to declare some method in the header file (e.g., MonoBehaviour.h).
  * 2. Use IMPLEMENT_REGISTER_CLASS (overload macros) to implement REGISTER_CLASS.
  *    2.1 Define a static TypeRegistrationDesc and initialize RTTI with static information (TypeName, Namespace, etc.).
  *    2.2 Reference the super RTTI.
  *    2.3 Implement the RegisterUnityClass method (setup factory, module, etc.).
  * 3. Call RegisterAllClasses (invoked in Application.InitializeProject at runtime) to register all Unity scripts to the TypeManager.
  *    3.1 Call GlobalRegisterType.
  *    3.2 Copy information from init RTTI to type RTTI (TypeContainer<TYPE_NAME_>::rtti).
  *    3.3 Cache <className, RTTI> map.
  *    3.4 Perform other things...
  */

template<typename T>
inline Object* NewObject()
{
	return new T();
}