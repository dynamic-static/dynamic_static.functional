
/*
==========================================
  Copyright (c) 2016-2021 dynamic_static
      Licensed under the MIT license
    http://opensource.org/licenses/MIT
==========================================
*/

#pragma once

#include <functional>

namespace dst {

/**
Alias for a std::function<> with variadic parameters and no return value
*/
template <typename ...Args>
using Action = std::function<void(Args...)>;

} // namespace dst
