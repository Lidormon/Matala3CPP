#ifndef __EXCEPTIONS_HPP__
#define __EXCEPTIONS_HPP__

#include<string>

#define THROW_INVALID_RANGE(var, min, max) { \
   if ((var) < (min) || (var) > (max)) { \
      throw "Invalid"#var"(="+std::to_string(var)+") value, out of range!"; \
   } \
}

#define THROW_NULLPTR(var) { \
   if ((var) == nullptr) { \
      throw "Unexpected null pointer at '" #var "'!"; \
   } \
}

#endif