#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ), s {}, _pushed(), _poped(), _closed(false) {}

void Writer::push( string data )
{
  int len = std::min(capacity_ - s.size(), data.size());
  s += data.substr(0, len);
  _pushed += len;
}

void Writer::close()
{
  // Your code here.
  _closed = true;
}

bool Writer::is_closed() const
{
  return _closed; // Your code here.
}

uint64_t Writer::available_capacity() const
{
  return capacity_ - s.size(); // Your code here.
}

uint64_t Writer::bytes_pushed() const
{
  return _pushed; // Your code here.
}

string_view Reader::peek() const
{
  return string_view(s.data(), s.size()); // Your code here.
}

void Reader::pop( uint64_t len )
{
  s.erase(s.begin(), s.begin() + len);
  _poped += len;
}

bool Reader::is_finished() const
{
  return _closed and s.empty(); // Your code here.
}

uint64_t Reader::bytes_buffered() const
{
  return s.size(); // Your code here.
}

uint64_t Reader::bytes_popped() const
{
  return _poped; // Your code here.
}
