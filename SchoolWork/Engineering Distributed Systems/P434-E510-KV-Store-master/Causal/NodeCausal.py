#!/usr/bin/python3

import redis
import os
import threading
import Pyro4
import Pyro4.naming
import sys
import time

# Node class that points to the redis server on the local machine
# Primary-based implementation for causal consistency

class Node(object):

    # Initializes the connection to the indicated redis instance
    # via ip/hostname & port
    def __init__(self, r_ip, r_port, name, nameserver):
        self.r_instance = redis.Redis(host = r_ip, port = r_port)
        self.name = name
        self.pid = os.getpid()
        self.timestamp = 0
        # This refers to Mr in the algorithm on the slides
        # Max timestamp of all writes received by this replica
        self.max_write_timestamp = 0
        self.name_server = nameserver

    # Updates the logical timestamp
    @Pyro4.expose
    def update_timestamp(self, ts_1, ts_2):
        self.timestamp = max(ts_1, ts_2) + 1

    # Get primary
    # Returns the dictionary containing the sequencer
    @Pyro4.expose
    def get_primary(self):
        return self.name_server.list(metadata_all={"primary"})

    # Get all names from name server
    # Returns dict name-to-uri
    @Pyro4.expose
    def get_all_replicas(self):
        return self.name_server.list(metadata_all={"replica"})

    # Sends message to all replicas
    @Pyro4.expose
    def send_to_all(self, msg_timestamp, msg_key, msg_value):
        replica_dict = self.get_all_replicas()
        msg = (msg_timestamp, msg_key, msg_value)
        # For testing purposes, to slow propagation
        time.sleep(1)
        for r in replica_dict.keys():
            replica = Pyro4.Proxy(replica_dict[r])
            replica.rcv_msg(msg)
            # For testing purposes, to slow propagation
            time.sleep(1)

    # Receives write messages from primary
    @Pyro4.expose
    def rcv_msg(self, msg):
        msg_timestamp, msg_key, msg_value = msg
        self.update_timestamp(msg_timestamp, self.timestamp)
        self.max_write_timestamp = self.timestamp
        self.r_instance.set(msg_key, msg_value)
        print(f"Replica at pid: {self.pid} has set key:\"{msg_key}\" and value:\"{msg_value}\"")


    # Does a local read on the redis instance and returns the
    # value if there is one.
    @Pyro4.expose
    def get(self, key, client_timestamp):
        self.update_timestamp(client_timestamp, self.timestamp)
        while True:
            # If return rule is satisfied, return
            if self.max_write_timestamp > client_timestamp:
                db_ret = self.r_instance.get(key)
                if db_ret is None:
                    return None
                else:
                    return self.r_instance.get(key).decode('utf-8')
            # Else, wait
            else:
                print(f"Node at pid:{self.pid} has not received the update yet.")
                # Enforce delay for testing
                time.sleep(1)
                continue


    # Causal consistency implemented with primary-based protocol
    # This assumes all writes go to the primary (assume 1 primary for all obj)
    @Pyro4.expose
    def set(self, key, value, client_timestamp):
        # Updates timestamp
        self.update_timestamp(client_timestamp, self.timestamp)
        # If the node is the primary, local write
        if self.name == "primary":
            self.r_instance.set(key, value)
            # Call to propogate then immediately return
            msg = (self.timestamp, key, value)
            send = threading.Thread(target=self.send_to_all, args=msg)
            send.start()
            return self.timestamp, f"Ok, key: {key} and value: {value} have been set on the primary with pid:{self.pid}."

# Starts and daemonize this node.
def main(r_ip, r_port, ns_ip, ns_port, name):
    daemon = Pyro4.Daemon()
    ns = Pyro4.locateNS(host=ns_ip, port=int(ns_port))
    node = Node(r_ip, int(r_port), name, ns)
    uri = daemon.register(node)
    if name == "primary":
        ns.register("primary", uri, metadata={"primary"})
    else:
        ns.register(name, uri, metadata={"replica"})
    daemon.requestLoop()

if __name__ == '__main__':
    main(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5])
