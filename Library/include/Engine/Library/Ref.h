#pragma once
#include <Engine/Library/Common.h>
#include <memory>

template<typename T>
typedef std::shared_ptr Ref;

template<typename T>
typedef std::make_shared New_Ref;