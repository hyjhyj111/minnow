#include "byte_stream.hh"
#include <cstddef>

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ) {}

bool Writer::is_closed() const
{
  // Your code here.
  return close_;
}

void Writer::push( string data )
{
  // Your code here.
  int len = std::min( available_capacity(), data.size() );
  pushed_ += len;
  if ( data.size() > static_cast<size_t>( len ) ) {
    data = data.substr( 0, len );
  }
  buff_ += data;
  return;
}

void Writer::close()
{
  // Your code here.
  close_ = true;
}

uint64_t Writer::available_capacity() const
{
  // Your code here.
  return capacity_ - buff_.size();
}

uint64_t Writer::bytes_pushed() const
{
  // Your code here.
  return pushed_;
}

bool Reader::is_finished() const
{
  // Your code here.
  return close_ && buff_.empty();
}

uint64_t Reader::bytes_popped() const
{
  // Your code here.
  return poped_;
}

string_view Reader::peek() const
{
  // Your code here.
  return buff_;
}

void Reader::pop( uint64_t len )
{
  // Your code here.
  poped_ += len;
  buff_ = buff_.substr( len );
}

uint64_t Reader::bytes_buffered() const
{
  // Your code here.
  return buff_.size();
}
