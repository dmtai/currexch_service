#pragma once

#include <filesystem>
#include <memory>
#include "content_type.hpp"
#include "http_session.hpp"
#include "http_status.hpp"
#include "utils/net.hpp"
#include "utils/utils.hpp"

namespace engine::http_server {

class HttpResponse {
 public:
  virtual ~HttpResponse() = default;

  virtual void SetContentType(ContentType content_type) = 0;
  virtual void SetKeepAlive(bool keep_alive) = 0;
  virtual void SetStatusCode(HttpStatus http_status) = 0;
  virtual void SetHttpVersion(unsigned http_version) = 0;
  virtual void Send(const std::string& data = "") = 0;
  virtual void SendFile(const std::filesystem::path& path) = 0;
  virtual void Close() = 0;
};

template <typename T>
class HttpResponseImpl : public HttpResponse {
 public:
  using BeastResponse = http::response<http::string_body>;
  using SessionPtr = std::shared_ptr<T>;

  HttpResponseImpl(SessionPtr session) : session_{std::move(session)} {}

  void SetContentType(ContentType content_type) override {
    response_.set(http::field::content_type, ToString(content_type));
  }

  void SetKeepAlive(bool keep_alive) override {
    response_.keep_alive(keep_alive);
  }

  void SetStatusCode(HttpStatus http_status) override {
    response_.result(HttpStatusEnumToBeastStatus(http_status));
  }

  void SetHttpVersion(unsigned http_version) override {
    response_.version(http_version);
  }

  void Send(const std::string& data) override {
    if (!data.empty()) {
      response_.body() = data;
    }
    response_.prepare_payload();
    session_->Write(std::move(response_));
  }

  void SendFile(const std::filesystem::path& path) override {
    auto file_body = utils::OpenFileBody(path);

    http::response<http::file_body> response;
    response.version(response_.version());
    response.result(response_.result());
    response.keep_alive(response_.keep_alive());
    response.set(
        http::field::content_type,
        ToString(GetContentTypeByFileExtenstion(path.extension().string())));
    response.body() = std::move(file_body);
    response.prepare_payload();

    session_->Write(std::move(response));
  }

  void Close() override { session_->Close(); }

 private:
  const SessionPtr session_;
  BeastResponse response_;
};

using HttpResponsePtr = std::shared_ptr<HttpResponse>;

HttpResponsePtr MakeHttpResponse(
    HttpSessionBasePtr session, bool keep_alive, unsigned http_version,
    ContentType content_type = ContentType::kApplicationJson);

http::response<http::string_body> MakeBeastResponse(
    http::status status, unsigned http_version, bool keep_alive,
    std::string_view content,
    ContentType content_type = ContentType::kApplicationJson);

}  // namespace engine::http_server