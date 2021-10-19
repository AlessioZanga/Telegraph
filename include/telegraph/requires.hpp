#pragma once

#define require_base_of(A, B) typename std::enable_if<std::is_base_of<A, B>::value>::type
#define require_iter_value_type(I, T) typename std::enable_if<std::is_same<typename std::iterator_traits<I>::value_type, T>::value, int>::type
