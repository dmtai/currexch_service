import pytest
import psycopg2
import subprocess
from pytest_postgresql.janitor import DatabaseJanitor
import time


def pytest_addoption(parser):
    parser.addini("currexch_service_port", "service port")
    parser.addini("currexch_service_host", "service host")
    parser.addini("currexch_service_config", "service conig")
    parser.addini("currexch_service_db_schema", "service schema")
    parser.addini("currexch_service_path", "service path")
    parser.addini("currexch_service_db_data", "db data")

@pytest.fixture(scope="session")
def config(request):
    return {"service_config": request.config.getini("currexch_service_config"), 
           "service_db_schema": request.config.getini("currexch_service_db_schema"),
           "service_db_data": request.config.getini("currexch_service_db_data"), 
           "service_host": request.config.getini("currexch_service_host"), 
           "service_port": request.config.getini("currexch_service_port"),
           "service_path": request.config.getini("currexch_service_path")}


@pytest.fixture(autouse=True, scope="session")
def database(postgresql_proc, config):
    janitor = DatabaseJanitor(
        postgresql_proc.user,
        postgresql_proc.host,
        postgresql_proc.port,
        postgresql_proc.dbname,
        postgresql_proc.version,
        postgresql_proc.password,
    )
    janitor.init()
    db_connection = psycopg2.connect(
        dbname=postgresql_proc.dbname,
        user=postgresql_proc.user,
        password=postgresql_proc.password,
        host=postgresql_proc.host,
        port=postgresql_proc.port,
    )
    
    with open(config["service_db_schema"], "r") as pg_schema, \
          open(config["service_db_data"], "r") as db_data:
      with db_connection.cursor() as cur:
          cur.execute(pg_schema.read())
          cur.execute(db_data.read()) 
          db_connection.commit()

    yield
    janitor.drop()


@pytest.fixture(autouse=True, scope="session")
def start_service(database, config):
    proc = subprocess.Popen(
        [config["service_path"], 
         "--config", 
         config["service_config"]]
    )
    time.sleep(2)
    yield
    proc.terminate()

@pytest.fixture(scope="session")
def service_url(config):
    return "http://{0}:{1}/".format(config["service_host"], 
                                    config["service_port"])