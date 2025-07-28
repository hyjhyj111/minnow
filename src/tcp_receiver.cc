#include "tcp_receiver.hh"
#include "byte_stream.hh"
#include "wrapping_integers.hh"
#include <cstdint>
#include <iostream>
#include <optional>
#include <sys/types.h>
using namespace std;

void TCPReceiver::receive( TCPSenderMessage message )
{
  // Your code here.
  if ( message.RST ) {
    rec_syn = false;
    reassembler_.reader().set_error();
    return;
  }
  if ( message.SYN and !rec_syn ) {
    zero_wrap_ = Wrap32 { message.seqno };
    rec_syn = true;
  }
  if ( !rec_syn ) {
    return;
  }

  uint64_t checkpoint = writer().bytes_pushed();
  uint64_t stream_index = message.seqno.unwrap( zero_wrap_, checkpoint ); 
  if ( message.SYN ) {
    stream_index = 0;
  } else {
    stream_index -= 1;
  }
  reassembler_.insert( stream_index, std::move( message.payload ), message.FIN );
}

TCPReceiverMessage TCPReceiver::send() const
{
  // Your code here.
  TCPReceiverMessage response;
  if ( reassembler_.reader().has_error() ) {
    response.ackno = std::nullopt;
    response.RST = true;
    return response;
  }

  response.window_size = static_cast<uint16_t>(
    std::min( UINT16_MAX * ( uint64_t( 1 ) ), reassembler_.writer().available_capacity() ) );
  if ( !rec_syn ) {
    return response;
  }

  uint64_t ackno = reassembler_.writer().bytes_pushed() + 1 + static_cast<uint64_t>( writer().is_closed() );
  response.ackno = Wrap32::wrap( ackno, zero_wrap_ ); // +1 for SYN
  return response;
}
