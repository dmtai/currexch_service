import requests


def test_patch_exchangerate(service_url):
    data = "rate=6.66".encode("utf-8")
    headers = {"Content-type": "application/x-www-form-urlencoded"}
    response = requests.patch(service_url + "exchangerate/RONIRR", headers=headers, data=data)
    assert response.status_code == 200
	
    response_body = response.json()
    assert response_body["rate"] == 6.66


def test_patch_exchangerate_without_required_field(service_url):
    data = "".encode("utf-8")
    headers = {"Content-type": "application/x-www-form-urlencoded"}
    response = requests.patch(service_url + "exchangerate/RONIRR", headers=headers, data=data)
    assert response.status_code == 400


def test_patch_unknown_exchangerate(service_url):
    data = "rate=0.123".encode("utf-8")
    headers = {"Content-type": "application/x-www-form-urlencoded"}
    response = requests.patch(service_url + "exchangerate/UNKNWN", headers=headers, data=data)
    assert response.status_code == 404