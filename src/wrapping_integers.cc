#include "wrapping_integers.hh"
#include <cstdint>

using namespace std;

Wrap32 Wrap32::wrap( uint64_t n, Wrap32 zero_point )
{
  // Your code here.
  return Wrap32 { static_cast<uint32_t>( n + zero_point.raw_value_ ) };
}

uint64_t Wrap32::unwrap( Wrap32 zero_point, uint64_t checkpoint ) const
{
  // Your code here.
  uint32_t offset = raw_value_ - zero_point.raw_value_; // offset = 0xFFFFFFFF
  uint64_t base = checkpoint & 0xFFFFFFFF00000000ULL;
  uint64_t now = base + offset;
  uint64_t pre = now - ( 1LL << 32 );
  uint64_t nex = now + ( 1LL << 32 );

  uint64_t d1 = now >= checkpoint ? now - checkpoint : checkpoint - now;
  uint64_t d2 = pre >= checkpoint ? pre - checkpoint : checkpoint - pre;
  uint64_t d3 = nex >= checkpoint ? nex - checkpoint : checkpoint - nex;
  if ( d1 <= d2 and d1 <= d3 ) {
    return now;
  } else if ( d2 <= d3 ) {
    return pre;
  } else {
    return nex;
  }
}
