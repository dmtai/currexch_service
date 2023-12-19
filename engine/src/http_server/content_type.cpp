#include "content_type.hpp"
#include "utils/bimap.hpp"

namespace engine::http_server {

namespace {

namespace content_types {

constexpr std::string_view kApplicationJson{"application/json"};
constexpr std::string_view kTextHtml{"text/html"};
constexpr std::string_view kTextCss{"text/css"};
constexpr std::string_view kTextPlain{"text/plain"};
constexpr std::string_view kApplicationXml{"application/xml"};
constexpr std::string_view kImagePng{"image/png"};
constexpr std::string_view kImageGif{"image/gif"};
constexpr std::string_view kImageJpeg{"image/jpeg"};
constexpr std::string_view kImageBmp{"image/bmp"};
constexpr std::string_view kAudioMpeg{"audio/mpeg"};
constexpr std::string_view kTextJavascript{"text/javascript"};
constexpr std::string_view kApplicationOctetStream{"application/octet-stream"};
constexpr std::string_view kImageSvg{"image/svg+xml"};
constexpr std::string_view kTextXml{"text/xml"};
constexpr std::string_view kImageXIcon{"image/x-icon"};
constexpr std::string_view kApplicationXWWWFormUrlencoded{
    "application/x-www-form-urlencoded"};

}  // namespace content_types

namespace file_extensions {

constexpr std::string_view kTxt = ".txt";
constexpr std::string_view kJson = ".json";
constexpr std::string_view kJpe = ".jpe";
constexpr std::string_view kJpeg = ".jpeg";
constexpr std::string_view kJpg = ".jpg";
constexpr std::string_view kHtm = ".htm";
constexpr std::string_view kHtml = ".html";
constexpr std::string_view kCss = ".css";
constexpr std::string_view kJs = ".js";
constexpr std::string_view kSvg = ".svg";
constexpr std::string_view kSvgz = ".svgz";
constexpr std::string_view kXml = ".xml";
constexpr std::string_view kPng = ".png";
constexpr std::string_view kGif = ".gif";
constexpr std::string_view kIco = ".ico";
constexpr std::string_view kBmp = ".bmp";
constexpr std::string_view kMp3 = ".mp3";
constexpr std::string_view kEmpty = "";

}  // namespace file_extensions

using StringByContentTypeEnum =
    utils::UnorderedBiMap<ContentType, std::string_view>;

const auto string_by_content_type_enum =
    utils::MakeBimap<StringByContentTypeEnum>(
        {{ContentType::kApplicationJson, content_types::kApplicationJson},
         {ContentType::kTextHtml, content_types::kTextHtml},
         {ContentType::kTextCss, content_types::kTextCss},
         {ContentType::kTextPlain, content_types::kTextPlain},
         {ContentType::kImagePng, content_types::kImagePng},
         {ContentType::kImageJpeg, content_types::kImageJpeg},
         {ContentType::kImageGif, content_types::kImageGif},
         {ContentType::kImageBmp, content_types::kImageBmp},
         {ContentType::kAudioMpeg, content_types::kAudioMpeg},
         {ContentType::kTextJavascript, content_types::kTextJavascript},
         {ContentType::kImageSvg, content_types::kImageSvg},
         {ContentType::kTextXml, content_types::kTextXml},
         {ContentType::kApplicationXWWWFormUrlencoded,
          content_types::kApplicationXWWWFormUrlencoded},
         {ContentType::kApplicationOctetStream,
          content_types::kApplicationOctetStream}});

using ContentTypeByFileExtension =
    utils::CaseInsensitiveUnorderedBiMap<std::string_view, ContentType>;

const auto content_type_by_file_extension =
    utils::MakeBimap<ContentTypeByFileExtension>(
        {{file_extensions::kTxt, ContentType::kTextPlain},
         {file_extensions::kJson, ContentType::kApplicationJson},
         {file_extensions::kJpe, ContentType::kImageJpeg},
         {file_extensions::kJpeg, ContentType::kImageJpeg},
         {file_extensions::kJpg, ContentType::kImageJpeg},
         {file_extensions::kHtm, ContentType::kTextHtml},
         {file_extensions::kHtml, ContentType::kTextHtml},
         {file_extensions::kCss, ContentType::kTextCss},
         {file_extensions::kJs, ContentType::kTextJavascript},
         {file_extensions::kSvg, ContentType::kImageSvg},
         {file_extensions::kSvgz, ContentType::kImageSvg},
         {file_extensions::kXml, ContentType::kTextXml},
         {file_extensions::kPng, ContentType::kImagePng},
         {file_extensions::kGif, ContentType::kImageGif},
         {file_extensions::kIco, ContentType::kImageXIco},
         {file_extensions::kBmp, ContentType::kImageBmp},
         {file_extensions::kMp3, ContentType::kAudioMpeg}});

}  // namespace

std::string_view ToString(ContentType content_type) {
  const auto it = string_by_content_type_enum.left.find(content_type);
  if (it != string_by_content_type_enum.left.end()) {
    return it->second;
  }
  return content_types::kApplicationOctetStream;
}

ContentType GetContentTypeByFileExtenstion(std::string_view extension) {
  const auto it = content_type_by_file_extension.left.find(extension);
  if (it != content_type_by_file_extension.left.end()) {
    return it->second;
  }
  return ContentType::kApplicationOctetStream;
}

}  // namespace engine::http_server