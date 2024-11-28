#pragma once
#include "Mono/MonoTypes.h"

class GameObject;
MonoObjectPtr MonoAddComponentWithType(GameObject& go, MonoObject* systemTypeInstance);
