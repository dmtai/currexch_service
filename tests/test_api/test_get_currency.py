import requests


def test_get_currency(service_url):
    response = requests.get(service_url + "currency/USD")
    assert response.status_code == 200

    response_body = response.json()
    
    assert response_body == {
      "code": "USD",
      "currency_id": 1,
      "full_name": "US Dollar",
      "sign": "$"
    }


def test_get_unknown_currency(service_url):
    response = requests.get(service_url + "currency/ASD")
    assert response.status_code == 404


def test_get_empty_currency(service_url):
    response = requests.get(service_url + "currency/")
    assert response.status_code == 400