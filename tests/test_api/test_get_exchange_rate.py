import requests


def test_get_exchange_rate(service_url):
    response = requests.get(service_url + "exchangerate/USDCUP")
    assert response.status_code == 200

    response_body = response.json()
    
    assert response_body == {
        "baseCurrency": {
            "code": "USD",
            "currency_id": 1,
            "full_name": "US Dollar",
            "sign": "$"
        },
        "id": 4,
        "rate": 1.8,
        "targetCurrency": {
            "code": "CUP",
            "currency_id": 4,
            "full_name": "Cuban peso",
            "sign": "₱"
        }
    }


def test_get_unknown_exchange_rate(service_url):
    response = requests.get(service_url + "exchangerate/GHTRTT")
    assert response.status_code == 404


def test_get_empty_exchange_rate(service_url):
    response = requests.get(service_url + "exchangerate/")
    assert response.status_code == 400