A toy service for currency exchange. For create the service, an engine was developed. The engine consists of a http-server(http-parsing(on boost::beast), routing, async request handling, http-handlers, http-filters) on boost::asio and utilities(async work with postgres(on ozo lib), caches, logging, configs).
### Build and run
```
git clone --recurse-submodule https://github.com/dmtai/currexch_service.git
cd currexch_service

# Run the service with docker-compose
make docker-run
```
To build and run the service locally, you need to run Postgres and create the database using a script db/curr_exchange.sql. Set connection_info in ./configs/config.yaml. Next you need to install the сonan package manager(1.60). Then use it to install dependencies(see Makefile with helping commands). After that, build the project(using cmake) and start the service.
```
git clone --recurse-submodule https://github.com/dmtai/currexch_service.git
cd currexch_service

# Install dependencies
make conan-install-debug

make build-debug

# Run the service
build_debug/currexch_service --config configs/config.yaml
```
