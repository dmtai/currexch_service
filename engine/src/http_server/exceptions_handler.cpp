#include "exceptions_handler.hpp"
#include "http_status.hpp"
#include "utils/logger.hpp"

namespace engine::http_server {

namespace {

void ReportHttpError(HttpResponse& response, const HttpException& ex) {
  response.SetStatusCode(ex.GetHttpStatus());
  response.Send(ex.what());
}

void ReportError(const HttpRequest& request, HttpResponse& response,
                 const std::exception& ex) {
  LOG(debug, "std::exception in \"{}\" http-handler or http-filter: {}",
      request.GetUri(), ex.what());
  response.SetStatusCode(HttpStatus::kInternalServerError);
  response.Send();
}

void ReportUnkownError(const HttpRequest& request, HttpResponse& response) {
  LOG(error, "Unknown exception in \"{}\" handler", request.GetUri());
  response.Close();
}

}  // namespace

void HandleException(const HttpRequest& request, HttpResponse& response,
                     std::exception_ptr ex_ptr) {
  try {
    if (ex_ptr) {
      std::rethrow_exception(ex_ptr);
    } else {
      throw std::exception();
    }
  } catch (const HttpException& ex) {
    ReportHttpError(response, ex);
  } catch (const std::exception& ex) {
    ReportError(request, response, ex);
  } catch (...) {
    ReportUnkownError(request, response);
  }
}

void ExceptionsHandlerImpl::operator()(const HttpRequest& request,
                                       HttpResponse& response,
                                       std::exception_ptr ex_ptr) const {
  HandleException(request, response, ex_ptr);
}

}  // namespace engine::http_server