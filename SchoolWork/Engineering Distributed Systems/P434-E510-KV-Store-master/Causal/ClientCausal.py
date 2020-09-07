#!/usr/bin/python3


import os
import random
import Pyro4
import Pyro4.naming
import sys

class CausalClient(object):

    # Initializes the connection to the indicated redis instance
    # via ip/hostname & port
    def __init__(self, name, nameserver):
        self.name = name
        self.pid = os.getpid()
        self.timestamp = 0
        self.name_server = nameserver
        self.max_read_timestamp = 0
        self.key_last_write = {}

    # Updates the logical timestamp
    @Pyro4.expose
    def update_timestamp(self, ts_1, ts_2):
        self.timestamp = max(ts_1, ts_2) + 1

    # Get primary
    # Returns the dictionary containing the primary
    @Pyro4.expose
    def get_primary(self):
        return self.name_server.list(metadata_all={"primary"})

    # Get all replicas from name server
    # Returns dict name-to-uri
    @Pyro4.expose
    def get_all_replicas(self):
        return self.name_server.list(metadata_all={"replica"})

    # Issue a set request
    @Pyro4.expose
    def set(self, key, value):
        # Update local timestamp
        self.update_timestamp(0, self.timestamp)
        # Store last update for key
        self.key_last_write["key"] = self.timestamp
        # Gets the primary to set
        primary_dict = self.get_primary()
        primary = Pyro4.Proxy(primary_dict["primary"])
        ret_timestamp, ret_message = primary.set(key, value, self.timestamp)
        print(f"Primary has sent back timestamp of {ret_timestamp}.")
        self.timestamp = ret_timestamp
        return ret_message

    # Issue a get request
    @Pyro4.expose
    def get(self, key):
        # Update local timestamp
        self.update_timestamp(0, self.timestamp)
        # Calculate minimum acceptable timestamp
        min_acp_timestamp = max(self.max_read_timestamp, self.key_last_write.get(key, 0))
        # Update local max timestamp for the latest read
        self.max_read_timestamp = self.timestamp
        # Send to a random replica that will return value
        replica_dict = self.get_all_replicas()
        r = random.choice(list(replica_dict.keys()))
        replica = Pyro4.Proxy(replica_dict[r])
        print(f"Hitting replica {r} for key {key}...")
        return replica.get(key, min_acp_timestamp)

# Starts and daemonize this node.
def main(ns_ip, ns_port, name):
    daemon = Pyro4.Daemon()
    ns = Pyro4.locateNS(host=ns_ip, port=int(ns_port))
    client = CausalClient(name, ns)
    uri = daemon.register(client)
    ns.register(name, uri, metadata={"client"})
    daemon.requestLoop()

if __name__ == '__main__':
    main(sys.argv[1], sys.argv[2], sys.argv[3])