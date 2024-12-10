#pragma once

#define REGISTER_CLASS(TYPE_NAME_) \
public: \
	typedef ThisType Super; \
	typedef TYPE_NAME_ ThisType; \
