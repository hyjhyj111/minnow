#include "reassembler.hh"
#include <cstdint>
#include <iterator>
#include <sys/types.h>

using namespace std;
using u64 = uint64_t;
void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring )
{
  auto& writer = output_.writer();
  if ( is_last_substring ) {
    eof = first_index + data.size();
    rec_eof = true;
    if ( rec_eof && index_ == eof ) {
      writer.close();
    }
  }
  if ( data.empty() ) {
    return;
  }

  // trim
  if ( first_index + data.size() <= index_ ) {
    return;
  }

  if ( first_index < index_ ) {
    data = data.substr( index_ - first_index );
    first_index = index_;
  }

  if ( first_index >= index_ + writer.available_capacity() ) {
    return;
  }
  int left_len = std::min( data.size(), index_ + writer.available_capacity() - first_index );
  data = data.substr( 0, left_len );

  // cover
  auto it = buff_.lower_bound( first_index );
  if ( it == buff_.end() || it->first != first_index ) {
    it = buff_.insert( { first_index, std::move( data ) } ).first;
    if ( it != buff_.begin() ) {
      it--;
    }
  } else {
    std::string& s = it->second;
    int len = data.size();
    if ( s.size() <= data.size() )
      s = std::move( data );
    else
      s = std::move( data ) + s.substr( len );
  }
  auto ne = std::next( it );
  while ( ne != buff_.end() ) {
    if ( it->first + it->second.size() < ne->first )
      break;
    if ( it->first + it->second.size() < ne->first + ne->second.size() ) {
      it->second += ne->second.substr( it->first + it->second.size() - ne->first );
    }
    ne = buff_.erase( ne );
  }

  // write
  while ( buff_.begin() != buff_.end() and buff_.begin()->first == index_ ) {
    it = buff_.begin();
    u64 len = it->second.size();
    writer.push( std::move( it->second ) );
    buff_.erase( buff_.begin() );
    index_ += len;
  }
  if ( rec_eof && index_ == eof ) {
    writer.close();
  }
}

uint64_t Reassembler::bytes_pending() const
{
  u64 ans = 0;
  for ( auto& [k, v] : buff_ ) {
    ans += v.size();
  }
  return ans;
}