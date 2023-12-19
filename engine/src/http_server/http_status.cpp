#include "http_status.hpp"
#include <unordered_map>
#include "utils/bimap.hpp"

namespace engine::http_server {

namespace {

using BeastStatusByHttpStatus = utils::UnorderedBiMap<HttpStatus, http::status>;

const auto beast_status_by_http_status = utils::MakeBimap<
    BeastStatusByHttpStatus>(
    {{HttpStatus::kContinue, http::status::continue_},
     {HttpStatus::kSwitchingProtocols, http::status::switching_protocols},
     {HttpStatus::kProcessing, http::status::processing},
     {HttpStatus::kOk, http::status::ok},
     {HttpStatus::kCreated, http::status::created},
     {HttpStatus::kAccepted, http::status::accepted},
     {HttpStatus::kNonAuthoritativeInformation,
      http::status::non_authoritative_information},
     {HttpStatus::kNoContent, http::status::no_content},
     {HttpStatus::kResetContent, http::status::reset_content},
     {HttpStatus::kPartialContent, http::status::partial_content},
     {HttpStatus::kMultiStatus, http::status::multi_status},
     {HttpStatus::kAlreadyReported, http::status::already_reported},
     {HttpStatus::kImUsed, http::status::im_used},
     {HttpStatus::kMultipleChoices, http::status::multiple_choices},
     {HttpStatus::kMovedPermanently, http::status::moved_permanently},
     {HttpStatus::kFound, http::status::found},
     {HttpStatus::kSeeOther, http::status::see_other},
     {HttpStatus::kNotModified, http::status::not_modified},
     {HttpStatus::kUseProxy, http::status::use_proxy},
     {HttpStatus::kTemporaryRedirect, http::status::temporary_redirect},
     {HttpStatus::kPermanentRedirect, http::status::permanent_redirect},
     {HttpStatus::kBadRequest, http::status::bad_request},
     {HttpStatus::kUnauthorized, http::status::unauthorized},
     {HttpStatus::kPaymentRequired, http::status::payment_required},
     {HttpStatus::kForbidden, http::status::forbidden},
     {HttpStatus::kNotFound, http::status::not_found},
     {HttpStatus::kMethodNotAllowed, http::status::method_not_allowed},
     {HttpStatus::kNotAcceptable, http::status::not_acceptable},
     {HttpStatus::kProxyAuthenticationRequired,
      http::status::proxy_authentication_required},
     {HttpStatus::kRequestTimeout, http::status::request_timeout},
     {HttpStatus::kConflict, http::status::conflict},
     {HttpStatus::kGone, http::status::gone},
     {HttpStatus::kLengthRequired, http::status::length_required},
     {HttpStatus::kPreconditionFailed, http::status::precondition_failed},
     {HttpStatus::kPayloadTooLarge, http::status::payload_too_large},
     {HttpStatus::kUriTooLong, http::status::uri_too_long},
     {HttpStatus::kUnsupportedMediaType, http::status::unsupported_media_type},
     {HttpStatus::kRangeNotSatisfiable, http::status::range_not_satisfiable},
     {HttpStatus::kExpectationFailed, http::status::expectation_failed},
     {HttpStatus::kMisdirectedRequest, http::status::misdirected_request},
     {HttpStatus::kUnprocessableEntity, http::status::unprocessable_entity},
     {HttpStatus::kLocked, http::status::locked},
     {HttpStatus::kFailedDependency, http::status::failed_dependency},
     {HttpStatus::kUpgradeRequired, http::status::upgrade_required},
     {HttpStatus::kPreconditionRequired, http::status::precondition_required},
     {HttpStatus::kTooManyRequests, http::status::too_many_requests},
     {HttpStatus::kRequestHeaderFieldsTooLarge,
      http::status::request_header_fields_too_large},
     {HttpStatus::kUnavailableForLegalReasons,
      http::status::unavailable_for_legal_reasons},
     {HttpStatus::kClientClosedRequest, http::status::client_closed_request},
     {HttpStatus::kInternalServerError, http::status::internal_server_error},
     {HttpStatus::kNotImplemented, http::status::not_implemented},
     {HttpStatus::kBadGateway, http::status::bad_gateway},
     {HttpStatus::kServiceUnavailable, http::status::service_unavailable},
     {HttpStatus::kGatewayTimeout, http::status::gateway_timeout},
     {HttpStatus::kHttpVersionNotSupported,
      http::status::http_version_not_supported},
     {HttpStatus::kVariantAlsoNegotiates,
      http::status::variant_also_negotiates},
     {HttpStatus::kInsufficientStorage, http::status::insufficient_storage},
     {HttpStatus::kLoopDetected, http::status::loop_detected},
     {HttpStatus::kNotExtended, http::status::not_extended},
     {HttpStatus::kNetworkAuthenticationRequired,
      http::status::network_authentication_required}});

}  // namespace

http::status HttpStatusEnumToBeastStatus(HttpStatus http_status) {
  const auto it = beast_status_by_http_status.left.find(http_status);
  if (it != beast_status_by_http_status.left.end()) {
    return it->second;
  }
  return http::status::unknown;
}

}  // namespace engine::http_server