#pragma once

#include "helpers.hh"

#include <optional>
#include <string>
#include <utility>

// https://stackoverflow.com/questions/33399594/making-a-user-defined-class-stdto-stringable

namespace minnow_conversions {
using std::to_string;

template<typename T>
std::string to_string( const std::optional<T>& v )
{
  if ( v.has_value() ) {
    return "Some(" + to_string( v.value() ) + ")";
  }

  return "None";
}

inline std::string to_string( bool b )
{
  return b ? "true" : "false";
}

inline std::string to_string( const std::string& str )
{
  return pretty_print( str );
}
} // namespace minnow_conversions

template<typename T>
concept MinnowStringable = requires( T t )
{
  minnow_conversions::to_string( t );
};

template<MinnowStringable T>
std::string to_string( T&& t )
{
  return minnow_conversions::to_string( std::forward<T>( t ) );
}
