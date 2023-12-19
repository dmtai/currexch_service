import requests


currencies = [{
  "code": "INR",
  "currency_id": 10,
  "full_name": "Indian Rupee",
  "sign": "₹"
}, {
  "code": "TRY",
  "currency_id": 9,
  "full_name": "Turkish lira",
  "sign": "₺"
}, {
  "code": "IRR",
  "currency_id": 8,
  "full_name": "Iranian rial",
  "sign": "IR"
}, {
  "code": "BRL",
  "currency_id": 7,
  "full_name": "Brazilian real",
  "sign": "R$"
}, {
  "code": "RON",
  "currency_id": 6,
  "full_name": "Romanian leu",
  "sign": "L"
}, {
  "code": "CNY",
  "currency_id": 5,
  "full_name": "Renminbi",
  "sign": "¥"
}, {
  "code": "CUP",
  "currency_id": 4,
  "full_name": "Cuban peso",
  "sign": "₱"
}, {
  "code": "RUB",
  "currency_id": 3,
  "full_name": "Russian Ruble",
  "sign": "₽"
}, {
  "code": "EUR",
  "currency_id": 2,
  "full_name": "Euro",
  "sign": "€"
}, {
  "code": "USD",
  "currency_id": 1,
  "full_name": "US Dollar",
  "sign": "$"
}]


def test_get_currencies(service_url):
    response = requests.get(service_url + "currencies")
    assert response.status_code == 200

    response_body = response.json()
    
    for curr in currencies:
       assert curr in response_body