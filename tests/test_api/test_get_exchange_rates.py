import requests


exchange_rates = [{
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
}, {
  "baseCurrency": {
    "code": "USD",
    "currency_id": 1,
    "full_name": "US Dollar",
    "sign": "$"
  },
  "id": 3,
  "rate": 1.4,
  "targetCurrency": {
    "code": "BRL",
    "currency_id": 7,
    "full_name": "Brazilian real",
    "sign": "R$"
  }
}, {
  "baseCurrency": {
    "code": "RON",
    "currency_id": 6,
    "full_name": "Romanian leu",
    "sign": "L"
  },
  "id": 2,
  "rate": 1.3,
  "targetCurrency": {
    "code": "IRR",
    "currency_id": 8,
    "full_name": "Iranian rial",
    "sign": "IR"
  }
}, {
  "baseCurrency": {
    "code": "CUP",
    "currency_id": 4,
    "full_name": "Cuban peso",
    "sign": "₱"
  },
  "id": 1,
  "rate": 1.6,
  "targetCurrency": {
    "code": "RUB",
    "currency_id": 3,
    "full_name": "Russian Ruble",
    "sign": "₽"
  }
}]


def test_get_exchange_rates(service_url):
    response = requests.get(service_url + "exchangerates")
    assert response.status_code == 200

    response_body = response.json()
    
    for exch_rate in exchange_rates:
       assert exch_rate in response_body