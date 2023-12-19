#include "command_line_options.hpp"
#include <boost/program_options.hpp>
#include <iostream>

namespace engine::utils {

CommandLineOptionsImpl::CommandLineOptionsImpl(std::string path) noexcept
    : path_{std::move(path)} {};

const std::string& CommandLineOptionsImpl::GetConfigPath() const noexcept {
  return path_;
}

CommandLineOptionsConstPtr ParseCommandLine(int argc,
                                            const char* const argv[]) {
  namespace opt = boost::program_options;
  opt::options_description desc("All options");
  desc.add_options()("config", opt::value<std::string>()->required(),
                     "path to config")("help", "help message");
  opt::variables_map vm;
  opt::store(opt::parse_command_line(argc, argv, desc), vm);
  if (vm.count("help")) {
    std::cout << desc << "\n";
    return nullptr;
  }
  opt::notify(vm);
  return std::make_shared<const CommandLineOptionsImpl>(
      CommandLineOptionsImpl{vm["config"].as<std::string>()});
}

}  // namespace engine::utils