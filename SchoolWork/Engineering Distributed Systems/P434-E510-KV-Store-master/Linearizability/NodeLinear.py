#!/usr/bin/python3

import redis
import os
import heapq
import Pyro4
import Pyro4.naming
import sys
import time
from multiprocessing import Lock

# Node class that points to the redis server on the local machine
# Uses sequencer based multi-casting to achieve total order broadcast
# Local read algorithm + Total order multicast on read for linearizability

class Node(object):

    # Initializes the connection to the indicated redis instance
    # via ip/hostname & port
    def __init__(self, r_ip, r_port, name, nameserver):
        self.r_instance = redis.Redis(host = r_ip, port = r_port)
        self.name = name
        self.pid = os.getpid()
        self.seq_num = 0
        self.max_seq = 0
        self.queue = []
        heapq.heapify(self.queue)
        self.name_server = nameserver
        self.blocking = False
        self.buffer = None
        self.lock = Lock()

    # Get sequencer
    # Returns the dictionary containing the sequencer
    def get_sequencer(self):
        return self.name_server.list(metadata_all={"sequencer"})

    # Get all names from name server
    # Returns dict name-to-uri
    @Pyro4.expose
    def get_all_replicas(self):
        return self.name_server.list(metadata_all={"replica"})

    # Sends message to the sequencer
    def send_to_seq(self, seq_uri, msg):
        # For testing puposes, transmission delay
        time.sleep(1)
        target = Pyro4.Proxy(seq_uri)
        target.seq_rcv_msg(msg)

    @Pyro4.expose
    # Sequencer broadcasts to all nodes after receiving initial broadcast
    def seq_rcv_msg(self, msg):
        self.lock.acquire()
        try:
            self.seq_num += 1
            print(f"Sequencer receives: {msg}. Broadcasting to all nodes.")
            replicas = self.get_all_replicas()
            # For testing puposes, transmission delay
            time.sleep(1)
            for r in replicas.keys():
                replica_uri = replicas[r]
                rep = Pyro4.Proxy(replica_uri)
                rep.rcv_broadcast(self.seq_num, msg)
                print(f"Sequencer broadcaster to {replica_uri}")
                # For testing puposes, transmission delay
                time.sleep(1)
            # Broadcast to self
            self.rcv_broadcast(self.seq_num, msg)
            print(f"Sequencer broadcaster to self")
            return
        finally:
            self.lock.release()

    @Pyro4.expose
    # Receives broadcast from sequencer
    def rcv_broadcast(self, seq_num, msg):
        # Place the message in a hold-back queue, ordered by sequence number
        # Bool indicates if the message is deliverable
        heapq.heappush(self.queue, (seq_num, msg))
        # Calls process to process the queue
        return self.process()

    # Processes the hold-back queue based on the maximum sequence number
    # Returns a boolean that is True when the process id matches the
    # pid of the process that called for the set/get, else False.
    def process(self):
        while True:
            if len(self.queue) != 0:
                # Pops the first item of the heap and checks if the item can be
                # processed based on the maximum local sequence number
                message = heapq.heappop(self.queue)
                message_seq = message[0]
                sender_pid, request, key, value = message[1]
                # Case where the item can be processed
                # i.e. it is at the front of the queue, and the
                # message number is 1 above the last processed message
                if message_seq - 1 == self.max_seq:
                    # Process the message
                    print(f"Node with pid: {self.pid} is processing message: {message}.")
                    if request == "set":
                        key = key.encode('utf-8')
                        value = value.encode('utf-8')
                        # Set to redis instance
                        self.r_instance.set(key, value)
                        # Check if it is the same process to unblock the
                        # current command.
                        if sender_pid == self.pid:
                            self.blocking = False
                        # Update self.max_seq to next number
                        self.max_seq = message_seq
                        continue
                    elif request == "get":
                        key = key.encode('utf-8')
                        query = self.r_instance.get(key)
                        if query is None:
                            self.buffer = None
                        else:
                            self.buffer = query.decode('utf-8')
                        # Check if it is the same process to unblock the
                        # current command.
                        if sender_pid == self.pid:
                            self.blocking = False
                        # Update self.max_seq to next number
                        self.max_seq = message_seq
                        continue
                # Case where the message cannot be processed
                # returns the message to the heap and pass.
                else:
                    heapq.heappush(self.queue, message)
                    break
            break

    @Pyro4.expose
    # Does a local read on the redis instance and returns the
    # value if there is one.
    def get(self, key):
        sequencer_dict = self.get_sequencer()
        # Block
        self.blocking = True
        # Send message to sequencer for total ordered broadcast
        self.send_to_seq(sequencer_dict["sequencer"], (self.pid, "get", key, None))
        while self.blocking:
            print("Blocking")
            time.sleep(1)
            continue
        result = self.buffer
        self.buffer = None
        return result

    @Pyro4.expose
    # Sequential consistency implemented with local read protocol
    # Writes (k,v) issues a totally ordered broadcast
    def set(self, key, value):
        msg = (self.pid, "set", key, value)
        sequencer_dict = self.get_sequencer()
        # Block
        self.blocking = True
        # Send message to sequencer for total ordered broadcast
        self.send_to_seq(sequencer_dict["sequencer"], msg)
        while self.blocking:
            print("Blocking")
            time.sleep(1)
            continue
        # Set complete, return ok
        return f"Ok, key: {key} and value: {value} have been set."

# Starts and daemonize this node.
def main(r_ip, r_port, ns_ip, ns_port, name):
    daemon = Pyro4.Daemon()
    ns = Pyro4.locateNS(host=ns_ip, port=int(ns_port))
    node = Node(r_ip, int(r_port), name, ns)
    uri = daemon.register(node)
    if name == "sequencer":
        ns.register("sequencer", uri, metadata={"sequencer"})
    else:
        ns.register(name, uri, metadata={"replica"})
    daemon.requestLoop()

if __name__ == '__main__':
    main(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5])
