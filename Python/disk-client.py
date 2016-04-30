import requests
import sys

class client:
	TOKEN = "d39d374558624520af9682fece0ad05c"
	URL = "https://cloud-api.yandex.net/v1/disk/resources"
	headers = { "Authorization" : "OAuth " + TOKEN }
	def ls (self, path):
		params = { "path" : path,
							 "fields" : "type,_embedded.items.path,_embedded.items.name,_embedded.items.type" }
		r = requests.get(self.URL, params=params, headers=self.headers)
		if r.status_code != 200:
			return (False, r.status_code)
		else:
			for filename in r.json()["_embedded"]["items"]:
				print(filename["name"])
			return (True, 0)
		
	def wait_status (self, link):
		while (true):
				status = requests.get(r.json()["href"], params=params, headers=self.headers)
				if (status["status"] == "success"):
					return (True, 0)
				if (status["status"] == "failure"):
					return (False, status.status_code)
	
	def rm (self, path):
		params = { "path" : path }
		r = requests.delete(self.URL, params=params, headers=self.headers)
		if (r.status_code == 204):
			return (True, 0)
		if (r.status_code == 202):
			return wait_status (r.json()["href"])
		return (False, r.status_code)
	
	def mv (self, path1, path2):
		params = { "from" : path1, "path" : path2 }
		r = requests.post(self.URL + "/move", params=params, headers=self.headers)
		if (r.status_code == 201):
			return (True, 0)
		if (r.status_code == 202):
			return wait_status(r.json()["href"])
		return (False, r.status_code)
	
	def cp (self, path1, path2):
		params = { "from" : path1, "path" : path2 }
		r = requests.post(self.URL + "/copy", params=params, headers=self.headers)
		if (r.status_code == 201):
			return (True, 0)
		if (r.status_code == 202):
			return wait_status(r.json()["href"])
		return (False, r.status_code) 
	
	def upload(self, path1, path2):
		params = { "path" : path2 }
		r = requests.get(self.URL + "/upload", params=params, headers=self.headers)
		if (r.status_code == 200):
			with open(path1, 'rb') as f:
				status = requests.put(r.json()["href"], data=f)
			return (True, 0)
		else:
			return (False, r.status_code)

	def download(self, path1, path2):
		params = { "path" : path1 }
		r = requests.get(self.URL + "/download", params=params, headers=self.headers)
		if (r.status_code == 200):
			f = open(path2, "wb")
			data = requests.get(r.json()["href"], params = {}, headers={})
			f.write(data.content)				
			return (True, 0)
		else:
			return (False, r.status_code)
