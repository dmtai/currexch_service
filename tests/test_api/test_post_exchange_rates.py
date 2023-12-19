import requests


def test_post_exchangerates(service_url):
    data = "baseCurrencyCode=BRL&targetCurrencyCode=RON&rate=1.23".encode("utf-8")
    headers = {"Content-type": "application/x-www-form-urlencoded"}
    response = requests.post(service_url + "exchangerates", headers=headers, data=data)
    assert response.status_code == 200
	
    response_body = response.json()
    assert response_body["baseCurrency"]["code"] == "BRL"
    assert response_body["targetCurrency"]["code"] == "RON"
    assert response_body["rate"] == 1.23


def test_post_exchangerates_without_required_field(service_url):
    data = "baseCurrencyCode=BRL&targetCurrencyCode=RON".encode("utf-8")
    headers = {"Content-type": "application/x-www-form-urlencoded"}
    response = requests.post(service_url + "exchangerates", headers=headers, data=data)
    assert response.status_code == 400


def test_post_existing_exchangerates(service_url):
    data = "baseCurrencyCode=BRL&targetCurrencyCode=RON&rate=1.23".encode("utf-8")
    headers = {"Content-type": "application/x-www-form-urlencoded"}
    response = requests.post(service_url + "exchangerates", headers=headers, data=data)
    assert response.status_code == 409