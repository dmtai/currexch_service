import requests


def test_get_exchange(service_url):
    params = "from=CUP&to=RUB&amount=10".encode("utf-8")
    headers = {"Content-type": "application/x-www-form-urlencoded"}
    response = requests.get(service_url + "exchange", headers=headers, params=params)
    assert response.status_code == 200

    response_body = response.json()
    assert response_body["baseCurrency"]["code"] == "CUP"
    assert response_body["targetCurrency"]["code"] == "RUB"
    assert response_body["rate"] == 1.6
    assert response_body["convertedAmount"] == 16


def test_get_exchange_by_reverse_rate(service_url):
    params = "from=RUB&to=CUP&amount=10".encode("utf-8")
    headers = {"Content-type": "application/x-www-form-urlencoded"}
    response = requests.get(service_url + "exchange", headers=headers, params=params)
    assert response.status_code == 200

    response_body = response.json()
    assert response_body["baseCurrency"]["code"] == "RUB"
    assert response_body["targetCurrency"]["code"] == "CUP"
    assert response_body["rate"] == 0.62
    assert response_body["convertedAmount"] == 6.25


def test_get_exchange_by_usd(service_url):
    params = "from=CUP&to=BRL&amount=10".encode("utf-8")
    headers = {"Content-type": "application/x-www-form-urlencoded"}
    response = requests.get(service_url + "exchange", headers=headers, params=params)
    assert response.status_code == 200

    response_body = response.json()
    assert response_body["baseCurrency"]["code"] == "CUP"
    assert response_body["targetCurrency"]["code"] == "BRL"
    assert response_body["rate"] == 0.78
    assert response_body["convertedAmount"] == 7.78


def test_get_exchange_without_required_field(service_url):
    params = "from=USD&to=AUD".encode("utf-8")
    headers = {"Content-type": "application/x-www-form-urlencoded"}
    response = requests.get(service_url + "exchange", headers=headers, params=params)
    assert response.status_code == 400