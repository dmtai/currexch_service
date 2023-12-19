#pragma once

#include <memory>
#include "server_context.hpp"
#include "utils/logger.hpp"
#include "utils/net.hpp"

namespace engine::http_server {

using HttpSessionBasePtr = std::shared_ptr<class HttpSessionBase>;

class HttpSessionBase {
 public:
  using BeastRequest = http::request<http::string_body>;

  HttpSessionBase(const HttpSessionBase&) = delete;
  HttpSessionBase& operator=(const HttpSessionBase&) = delete;
  virtual ~HttpSessionBase() = default;
  void Run();
  void Close();
  virtual const std::string& GetIP() const noexcept = 0;

  template <typename Body, typename Fields>
  void Write(http::response<Body, Fields>&& response) {
    using namespace std::chrono;
    LOG(info, "Response sent: ip={} status={} response_time={}", GetIP(),
        response.result_int(),
        std::chrono::round<milliseconds>(steady_clock::now() -
                                         last_request_receipt_time_)
            .count());

    const auto safe_response =
        std::make_shared<http::response<Body, Fields>>(std::move(response));
    http::async_write(stream_, *safe_response,
                      [safe_response, self = GetSharedThis()](
                          beast::error_code ec, std::size_t bytes_written) {
                        self->OnWrite(safe_response->need_eof(), ec,
                                      bytes_written);
                      });
  }

 protected:
  HttpSessionBase(tcp::socket&& socket, const ServerContext& server_context);
  beast::tcp_stream stream_;

 private:
  [[nodiscard]] virtual HttpSessionBasePtr GetSharedThis() = 0;
  virtual void HandleRequest(BeastRequest&& request) = 0;
  void Read();
  void OnRead(beast::error_code ec, std::size_t bytes_read);
  void OnWrite(bool close, beast::error_code ec, std::size_t bytes_written);

  std::chrono::steady_clock::time_point last_request_receipt_time_;
  beast::flat_buffer buffer_;
  BeastRequest request_;
  const ServerContext& server_context_;
};

template <typename RequestHandler>
class HttpSession
    : public HttpSessionBase,
      public std::enable_shared_from_this<HttpSession<RequestHandler>> {
 public:
  template <typename Handler>
  HttpSession(tcp::socket&& socket, std::string ip, Handler&& request_handler,
              const ServerContext& server_context)
      : HttpSessionBase{std::move(socket), server_context},
        ip_{std::move(ip)},
        request_handler_{std::forward<Handler>(request_handler)} {}

  const std::string& GetIP() const noexcept override { return ip_; }

 private:
  HttpSessionBasePtr GetSharedThis() override {
    return this->shared_from_this();
  }

  void HandleRequest(BeastRequest&& request) override {
    request_handler_(std::move(request), GetSharedThis());
  }

  const std::string ip_;
  const RequestHandler request_handler_;
};

}  // namespace engine::http_server