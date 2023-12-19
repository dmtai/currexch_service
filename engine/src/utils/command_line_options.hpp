#pragma once

#include <memory>
#include <string>

namespace engine::utils {

class CommandLineOptions {
 public:
  virtual ~CommandLineOptions() = default;
  virtual const std::string& GetConfigPath() const noexcept = 0;
};

class CommandLineOptionsImpl : public CommandLineOptions {
 public:
  CommandLineOptionsImpl() = default;
  explicit CommandLineOptionsImpl(std::string path) noexcept;
  const std::string& GetConfigPath() const noexcept override;

 private:
  std::string path_;
};

using CommandLineOptionsConstPtr = std::shared_ptr<const CommandLineOptions>;

CommandLineOptionsConstPtr ParseCommandLine(int argc, const char* const argv[]);

}  // namespace engine::utils