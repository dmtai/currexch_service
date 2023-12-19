#pragma once

#include <string>

namespace engine::http_server {

enum class ContentType {
  kApplicationJson,
  kTextHtml,
  kTextCss,
  kTextPlain,
  kApplicationXml,
  kImagePng,
  kImageGif,
  kImageJpeg,
  kImageBmp,
  kAudioMpeg,
  kTextJavascript,
  kImageSvg,
  kTextXml,
  kImageXIco,
  kApplicationOctetStream,
  kApplicationXWWWFormUrlencoded
};

std::string_view ToString(ContentType content_type);
ContentType GetContentTypeByFileExtenstion(std::string_view extension);

}  // namespace engine::http_server