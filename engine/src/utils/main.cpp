#include "main.hpp"
#include <boost/exception/diagnostic_information.hpp>
#include <iostream>
#include "command_line_options.hpp"
#include "http_server/config.hpp"
#include "logger.hpp"

namespace engine::utils {

Main::Main(const int argc, const char* const argv[])
    : argc_{argc}, argv_{argv} {}

int Main::Run() noexcept {
  try {
    const auto options = utils::ParseCommandLine(argc_, argv_);
    if (!options) {
      return 1;
    }

    const auto config_path = options->GetConfigPath();
    const auto config = std::make_shared<const http_server::config::Config>(
        !config_path.empty()
            ? http_server::config::ParseConfig(YAML::LoadFile(config_path))
            : http_server::config::Config{});

    logger::SetDefaultLogger(logger::MakeLogger(config->general_.log_path_,
                                                config->general_.log_level_));

    server_builder_.SetConfig(std::move(config));
    server_builder_.Build()->Run();
  } catch (const std::exception& ex) {
    std::cerr << ex.what() << '\n';
    return 1;
  } catch (...) {
    std::cerr << "Unknown exception!" << '\n'
              << boost::current_exception_diagnostic_information();
    return 1;
  }
  return 0;
}

}  // namespace engine::utils