#!/usr/bin/python3

import Pyro4.naming
import sys

# Starts the name server on the given host and port
def start_ns(host, port):
    Pyro4.naming.startNSloop(host = host, port = int(port))

if __name__ == "__main__":
    start_ns(sys.argv[1], sys.argv[2])