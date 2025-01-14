#!/usr/bin/python

import requests, sys
import kv_pb2 as kv
from struct import pack

HOST = "http://localhost:9001"
HEADERS = {'content-type': 'application/grpc', 'Host': 'grpc'}
USAGE = """

envoy-python-client usage:
  ./client.py set <key> <value> - sets the <key> and <value>
  ./client.py get <key>         - gets the value for <key>
  """


class KVClient():

  def get(self, key):
    r = kv.GetRequest(key=key)

    # Build the gRPC frame
    data = r.SerializeToString()
    data = pack('!cI', b'\0', len(data)) + data

    resp = requests.post(f"{HOST}/kv.KV/Get", data=data, headers=HEADERS)

    return kv.GetResponse().FromString(resp.content[5:])

  def set(self, key, value):
    r = kv.SetRequest(key=key, value=value)
    data = r.SerializeToString()
    data = pack('!cI', b'\0', len(data)) + data

    return requests.post(f"{HOST}/kv.KV/Set", data=data, headers=HEADERS)


def run():
  if len(sys.argv) == 1:
    print(USAGE)

    sys.exit(0)

  cmd = sys.argv[1]

  client = KVClient()

  if cmd == "get":
    # ensure a key was provided
    if len(sys.argv) != 3:
      print(USAGE)
      sys.exit(1)

    # get the key to fetch
    key = sys.argv[2]

    # send the request to the server
    response = client.get(key)

    print(response.value)
    sys.exit(0)

  elif cmd == "set":
    # ensure a key and value were provided
    if len(sys.argv) < 4:
      print(USAGE)
      sys.exit(1)

    # get the key and the full text of value
    key = sys.argv[2]
    value = " ".join(sys.argv[3:])

    # send the request to the server
    response = client.set(key, value)

    print(f"setf {key} to {value}")


if __name__ == '__main__':
  run()
