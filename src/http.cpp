#include "http.h"

#define NAMEOF(var) removeScope((#var))

using namespace siweb::http;

std::map<std::string, std::string> http::content_type_mappings = {
    {"", "application/octet-stream"},
    {".html", "text/html"},
    {".htm", "text/html"},
    {".xml", "application/xml"},
    {".json", "application/json"},
    {".png", "image/png"},
    {".jpg", "image/jpeg"},
    {".jpeg", "image/jpeg"},
    {".gif", "image/gif"},
    {".css", "text/css"},
    {".txt", "text/plain"}};

static std::string removeScope(std::string str) {
    int count = 0;
    for (int i = 0; i < str.length() - 2; i++) {
        if (str[i] == ':' && str[i + 1] == ':') {
            return str.substr(i + 2);
        }
    }

    return str;
}

std::string http::HttpMethodToString(const httpMethod& method) {
    switch (method) {
        case httpMethod::GET:
            return NAMEOF(httpMethod::GET);
        case httpMethod::HEAD:
            return NAMEOF(httpMethod::HEAD);
        case httpMethod::POST:
            return NAMEOF(httpMethod::POST);
        case httpMethod::PUT:
            return NAMEOF(httpMethod::PUT);
        case httpMethod::DELETE:
            return NAMEOF(httpMethod::DELETE);
        case httpMethod::CONNECT:
            return NAMEOF(httpMethod::CONNECT);
        case httpMethod::OPTIONS:
            return NAMEOF(httpMethod::OPTIONS);
        case httpMethod::TRACE:
            return NAMEOF(httpMethod::TRACE);
        case httpMethod::PATCH:
            return NAMEOF(httpMethod::PATCH);
        default:
            exit(-1);
    }
}

std::string http::HttpStatusCodeToString(const httpStatusCode& statusCode) {
    switch (statusCode) {
        case httpStatusCode::Continue:
            return NAMEOF(httpStatusCode::Continue);
        case httpStatusCode::SwitchingProtocols:
            return NAMEOF(httpStatusCode::SwitchingProtocols);
        case httpStatusCode::Processing:
            return NAMEOF(httpStatusCode::Processing);
        case httpStatusCode::EarlyHints:
            return NAMEOF(httpStatusCode::EarlyHints);
        case httpStatusCode::Ok:
            return NAMEOF(httpStatusCode::Ok);
        case httpStatusCode::Created:
            return NAMEOF(httpStatusCode::Created);
        case httpStatusCode::Accepted:
            return NAMEOF(httpStatusCode::Accepted);
        case httpStatusCode::NonAuthoritativeInformation:
            return NAMEOF(httpStatusCode::NonAuthoritativeInformation);
        case httpStatusCode::NoContent:
            return NAMEOF(httpStatusCode::NoContent);
        case httpStatusCode::ResetContent:
            return NAMEOF(httpStatusCode::ResetContent);
        case httpStatusCode::PartialContent:
            return NAMEOF(httpStatusCode::PartialContent);
        case httpStatusCode::AlreadyReported:
            return NAMEOF(httpStatusCode::AlreadyReported);
        case httpStatusCode::IMUsed:
            return NAMEOF(httpStatusCode::IMUsed);
        case httpStatusCode::MultipleChoice:
            return NAMEOF(httpStatusCode::MultipleChoice);
        case httpStatusCode::MovedPermanently:
            return NAMEOF(httpStatusCode::MovedPermanently);
        case httpStatusCode::Found:
            return NAMEOF(httpStatusCode::Found);
        case httpStatusCode::SeeOther:
            return NAMEOF(httpStatusCode::SeeOther);
        case httpStatusCode::NotModified:
            return NAMEOF(httpStatusCode::NotModified);
        case httpStatusCode::UseProxy:
            return NAMEOF(httpStatusCode::UseProxy);
        case httpStatusCode::TemporaryRedirect:
            return NAMEOF(httpStatusCode::TemporaryRedirect);
        case httpStatusCode::PermanentRedirect:
            return NAMEOF(httpStatusCode::PermanentRedirect);
        case httpStatusCode::BadRequest:
            return NAMEOF(httpStatusCode::BadRequest);
        case httpStatusCode::Unauthorized:
            return NAMEOF(httpStatusCode::Unauthorized);
        case httpStatusCode::PaymentRequired:
            return NAMEOF(httpStatusCode::PaymentRequired);
        case httpStatusCode::Forbidden:
            return NAMEOF(httpStatusCode::Forbidden);
        case httpStatusCode::NotFound:
            return NAMEOF(httpStatusCode::NotFound);
        case httpStatusCode::MethodNotAllowed:
            return NAMEOF(httpStatusCode::MethodNotAllowed);
        case httpStatusCode::NotAcceptable:
            return NAMEOF(httpStatusCode::NotAcceptable);
        case httpStatusCode::ProxyAuthenticationRequired:
            return NAMEOF(httpStatusCode::ProxyAuthenticationRequired);
        case httpStatusCode::RequestTimeout:
            return NAMEOF(httpStatusCode::RequestTimeout);
        case httpStatusCode::Conflict:
            return NAMEOF(httpStatusCode::Conflict);
        case httpStatusCode::Gone:
            return NAMEOF(httpStatusCode::Gone);
        case httpStatusCode::LengthRequired:
            return NAMEOF(httpStatusCode::LengthRequired);
        case httpStatusCode::PreconditionFailed:
            return NAMEOF(httpStatusCode::PreconditionFailed);
        case httpStatusCode::PayloadTooLarge:
            return NAMEOF(httpStatusCode::PayloadTooLarge);
        case httpStatusCode::URITooLong:
            return NAMEOF(httpStatusCode::URITooLong);
        case httpStatusCode::UnsupportedMediaType:
            return NAMEOF(httpStatusCode::UnsupportedMediaType);
        case httpStatusCode::RequestedRangeNotSatisfieable:
            return NAMEOF(httpStatusCode::RequestedRangeNotSatisfieable);
        case httpStatusCode::ExpectationFailed:
            return NAMEOF(httpStatusCode::ExpectationFailed);
        case httpStatusCode::MisdirectedRequest:
            return NAMEOF(httpStatusCode::MisdirectedRequest);
        case httpStatusCode::UpgradeRequired:
            return NAMEOF(httpStatusCode::UpgradeRequired);
        case httpStatusCode::PreconditionRequired:
            return NAMEOF(httpStatusCode::PreconditionRequired);
        case httpStatusCode::TooManyRequests:
            return NAMEOF(httpStatusCode::TooManyRequests);
        case httpStatusCode::RequestHeaderFieldsTooLarge:
            return NAMEOF(httpStatusCode::RequestHeaderFieldsTooLarge);
        case httpStatusCode::UnavailableForLegalReasons:
            return NAMEOF(httpStatusCode::UnavailableForLegalReasons);
        case httpStatusCode::InternalServerError:
            return NAMEOF(httpStatusCode::InternalServerError);
        case httpStatusCode::NotImplemented:
            return NAMEOF(httpStatusCode::NotImplemented);
        case httpStatusCode::BadGateway:
            return NAMEOF(httpStatusCode::BadGateway);
        case httpStatusCode::ServiceUnavailable:
            return NAMEOF(httpStatusCode::ServiceUnavailable);
        case httpStatusCode::GatewayTimeout:
            return NAMEOF(httpStatusCode::GatewayTimeout);
        case httpStatusCode::HTTPVersionNotSupported:
            return NAMEOF(httpStatusCode::HTTPVersionNotSupported);
        case httpStatusCode::VariantAlsoNegotiates:
            return NAMEOF(httpStatusCode::VariantAlsoNegotiates);
        case httpStatusCode::InsufficientStorage:
            return NAMEOF(httpStatusCode::InsufficientStorage);
        case httpStatusCode::LoopDetected:
            return NAMEOF(httpStatusCode::LoopDetected);
        case httpStatusCode::NotExtended:
            return NAMEOF(httpStatusCode::NotExtended);
        case httpStatusCode::NetworkAuthenticationRequired:
            return NAMEOF(httpStatusCode::NetworkAuthenticationRequired);
        default:
            exit(-1);
    }
}