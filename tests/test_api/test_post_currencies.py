import requests


def test_post_currencies(service_url):
    data = "name=Lek&code=ALL&sign=¤".encode("utf-8")
    headers = {"Content-type": "application/x-www-form-urlencoded"}
    response = requests.post(service_url + "currencies", headers=headers, data=data)
    assert response.status_code == 200

    response_body = response.json()
    assert response_body["code"] == "ALL"
    assert response_body["full_name"] == "Lek"

def test_post_currencies_without_required_field(service_url):
    data = "name=Lek&code=ALL".encode("utf-8")
    headers = {"Content-type": "application/x-www-form-urlencoded"}
    response = requests.post(service_url + "currencies", headers=headers, data=data)
    assert response.status_code == 400


def test_post_existing_currencies(service_url):
    data = "name=Lek&code=USD&sign=f".encode("utf-8")
    headers = {"Content-type": "application/x-www-form-urlencoded"}
    response = requests.post(service_url + "currencies", headers=headers, data=data)
    assert response.status_code == 409