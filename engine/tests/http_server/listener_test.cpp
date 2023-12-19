#include "src/http_server/listener.hpp"
#include <gtest/gtest.h>
#include <atomic>
#include <string>
#include <thread>
#include "../mocks/http_server/mocks.hpp"
#include "src/utils/config.hpp"
#include "src/utils/net.hpp"

namespace engine::http_server {

namespace {

net::io_context io;
std::atomic_bool is_accepted{false};
std::atomic_bool is_listener_started{false};

template <typename T>
class TestSession {
 public:
  TestSession(tcp::socket&& socket, std::string ip, T request_handler,
              const ServerContext& server_context) {}

  void Run() {
    is_accepted.exchange(true);
    io.stop();
  }
};

class TestRequestHandler {};

}  // namespace

TEST(ListenerTest, Run) {
  const auto address = net::ip::make_address("0.0.0.0");
  const net::ip::port_type port = 6666;
  MockServerContext server_context;
  TestRequestHandler request_handler;
  tcp::endpoint ep{address, port};

  const auto server_func = [&]() {
    net::post(io, [&] {
      std::make_shared<Listener<TestSession, TestRequestHandler>>(
          io, ep, request_handler, server_context)
          ->Run();
      is_listener_started.exchange(true);
      is_listener_started.notify_all();
    });
    io.run_for(std::chrono::seconds(3));
  };
  std::thread server_thread{server_func};

  is_listener_started.wait(false);
  tcp::socket socket(io);
  socket.connect(ep);

  server_thread.join();
  ASSERT_EQ(true, is_accepted.load());
}

}  // namespace engine::http_server