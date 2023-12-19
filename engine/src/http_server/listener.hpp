#pragma once

#include <memory>
#include "http_session.hpp"
#include "request_handler.hpp"
#include "server.hpp"
#include "utils/net.hpp"

namespace engine::http_server {

template <template <typename> typename Session, typename RequestHandler>
class Listener
    : public std::enable_shared_from_this<Listener<Session, RequestHandler>> {
 public:
  template <typename Handler>
  Listener(net::io_context& io_context, const tcp::endpoint& endpoint,
           Handler&& request_handler, const ServerContext& server_ctx)
      : io_context_{io_context},
        acceptor_{net::make_strand(io_context)},
        request_handler_{std::forward<Handler>(request_handler)},
        server_context_{server_ctx} {
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(net::socket_base::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen(net::socket_base::max_listen_connections);
  }

  void Run() { DoAccept(); }

 private:
  void DoAccept() {
    endpoint_ = {};
    acceptor_.async_accept(net::make_strand(io_context_), endpoint_,
                           beast::bind_front_handler(&Listener::OnAccept,
                                                     this->shared_from_this()));
  }

  void OnAccept(boost::system::error_code ec, tcp::socket socket) {
    using namespace std::literals;
    if (ec) {
      LOG(error, "Error accepting connection. Code={} Msg={}", ec.value(),
          ec.message());
      return DoAccept();
    }
    AsyncRunSession(std::move(socket));
    DoAccept();
  }

  void AsyncRunSession(tcp::socket&& socket) const {
    boost::system::error_code ec;
    const auto ip = endpoint_.address().to_string(ec);
    if (ec) {
      LOG(error, "Error converting IP to string. Code={} msg={}", ec.value(),
          ec.message());
    }
    std::make_shared<Session<RequestHandler>>(std::move(socket), ip,
                                              request_handler_, server_context_)
        ->Run();
  }

  net::io_context& io_context_;
  tcp::acceptor acceptor_;
  RequestHandler request_handler_;
  const ServerContext& server_context_;
  tcp::endpoint endpoint_;
};

template <template <typename> typename Session, typename Handler>
using ListenerPtr = std::shared_ptr<Listener<Session, Handler>>;

template <typename RequestHandler>
ListenerPtr<HttpSession, RequestHandler> MakeListener(
    net::io_context& io_context, const tcp::endpoint& endpoint,
    RequestHandler&& handler, const ServerContext& server_ctx) {
  return std::make_shared<Listener<HttpSession, std::decay_t<RequestHandler>>>(
      io_context, endpoint, std::forward<RequestHandler>(handler), server_ctx);
}

}  // namespace engine::http_server