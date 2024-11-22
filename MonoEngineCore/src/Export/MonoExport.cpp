#include "mepch.h"
#include "MonoExport.h"
#include "Object.bindings.h"
#include "GameObject.bindings.h"

void RegisterMonoFunctions()
{
	ExportObjectBindings();
	ExportGameObjectBindings();
}
