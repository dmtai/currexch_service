#include "http_session.hpp"
#include <chrono>
#include "server_context.hpp"
#include "utils/net.hpp"

namespace engine::http_server {

HttpSessionBase::HttpSessionBase(tcp::socket&& socket,
                                 const ServerContext& server_context)
    : stream_{std::move(socket)}, server_context_{server_context} {}

void HttpSessionBase::Run() {
  net::dispatch(
      stream_.get_executor(),
      beast::bind_front_handler(&HttpSessionBase::Read, GetSharedThis()));
  LOG(debug, "HTTP-session for ip={} was created", GetIP());
}

void HttpSessionBase::Read() {
  request_ = {};
  stream_.expires_after(std::chrono::milliseconds(
      server_context_.GetConfig().general_.session_timeout_));
  http::async_read(
      stream_, buffer_, request_,
      beast::bind_front_handler(&HttpSessionBase::OnRead, GetSharedThis()));
}

void HttpSessionBase::OnRead(beast::error_code ec,
                             [[maybe_unused]] std::size_t bytes_read) {
  if (ec == http::error::end_of_stream) {
    return Close();
  }
  if (ec) {
    LOG(error, "Error writing to session socket. Code={} msg={}", ec.value(),
        ec.message());
    return;
  }
  LOG(info, "Request received: ip={} uri={} method={} ", GetIP(),
      static_cast<std::string_view>(request_.target()),
      static_cast<std::string_view>(http::to_string(request_.method())));

  last_request_receipt_time_ = std::chrono::steady_clock::now();

  HandleRequest(std::move(request_));
}

void HttpSessionBase::OnWrite(bool close, beast::error_code ec,
                              [[maybe_unused]] std::size_t bytes_written) {
  if (ec) {
    LOG(error, "Error writing to session socket. Code={} msg={}", ec.value(),
        ec.message());
    return;
  }

  if (close) {
    return Close();
  }
  Read();
}

void HttpSessionBase::Close() {
  boost::system::error_code ec;
  stream_.socket().shutdown(tcp::socket::shutdown_send, ec);
  if (ec) {
    LOG(debug, "Session socket(ip={}) shutdown error. Code={} msg={}", GetIP(),
        ec.value(), ec.message());
    return;
  }
  LOG(debug, "HTTP-session for ip={} was destroyed", GetIP());
}

}  // namespace engine::http_server