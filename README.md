Service for currency exchange. For create the service, an engine was developed. The engine consists of a http-server(http parsing, routing, async request handling, http-handlers, http-filters) on boost::asio and utilities(async work with postgres(on ozo lib), caches, logging, configs).
#### Build and run
```
git clone --recurse-submodule https://github.com/dmtai/currexch_service.git
cd currexch_service

# Run the service with docker-compose
make docker-run
```
