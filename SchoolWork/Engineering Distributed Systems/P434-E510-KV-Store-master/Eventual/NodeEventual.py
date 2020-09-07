#!/usr/bin/python3

import redis
import os
import threading
import Pyro4
import Pyro4.naming
import sys
import time

# Node class that points to the redis server on the local machine
# Eventual consistency

class Node(object):

    # Initializes the connection to the indicated redis instance
    # via ip/hostname & port
    def __init__(self, r_ip, r_port, name, nameserver):
        self.r_instance = redis.Redis(host = r_ip, port = r_port)
        self.name = name
        self.pid = os.getpid()
        self.name_server = nameserver

    # Get all names from name server
    # Returns dict name-to-uri
    @Pyro4.expose
    def get_all_nodes(self):
        return self.name_server.list(metadata_all={"node"})

    # Sends message to all replicas
    @Pyro4.expose
    def send_to_all(self, init_uri, msg_key, msg_value):
        nodes_dict = self.get_all_nodes()
        # For testing purposes, to illustrate slow propagation.
        time.sleep(1)
        for n in nodes_dict.keys():
            target_uri = nodes_dict[n]
            if target_uri == init_uri:
                # Don't send back to the initiator of the broadcast
                continue
            else:
                node = Pyro4.Proxy(target_uri)
                msg = (msg_key, msg_value)
                node.rcv_msg(msg)
                # For testing purposes, to illustrate slow propagation.
                time.sleep(1)

    # Receives write messages and stores on redis instance
    @Pyro4.expose
    def rcv_msg(self, msg):
        msg_key, msg_value = msg
        self.r_instance.set(msg_key, msg_value)
        print(f"Node at pid: {self.pid} has set key:\"{msg_key}\" and value:\"{msg_value}\"")

    # Does a local read on the redis instance and returns the
    # value if there is one.
    @Pyro4.expose
    def get(self, key):
        db_ret = self.r_instance.get(key)
        if db_ret is None:
            return None
        else:
            return self.r_instance.get(key).decode('utf-8')

    # Eventual consistency
    # Sets on the local redis instance, then return ok while lazily propagating
    @Pyro4.expose
    def set(self, key, value):
        self.r_instance.set(key, value)
        # Get own URI
        nodes_dict = self.name_server.list()
        uri = nodes_dict[self.name]
        # Call to propogate then immediately return
        msg = (uri, key, value)
        send = threading.Thread(target=self.send_to_all, args=msg)
        send.start()
        # Return
        return f"Ok, key: {key} and value: {value} have been set on the primary with pid:{self.pid}."

# Starts and daemonize this node.
def main(r_ip, r_port, ns_ip, ns_port, name):
    daemon = Pyro4.Daemon()
    ns = Pyro4.locateNS(host=ns_ip, port=int(ns_port))
    node = Node(r_ip, int(r_port), name, ns)
    uri = daemon.register(node)
    ns.register(name, uri, metadata={"node"})
    daemon.requestLoop()

if __name__ == '__main__':
    main(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5])
