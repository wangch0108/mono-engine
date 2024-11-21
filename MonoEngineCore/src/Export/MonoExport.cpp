#include "mepch.h"
#include "MonoExport.h"

#include "GameObject.bindings.h"

void RegisterMonoFunctions()
{
	ExportGameObjectBindings();
}
