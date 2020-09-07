import subprocess
import os
import Pyro4
import signal
import time
import threading
import random

###########################################################
# Server initialization
###########################################################

pid = []

# Start name server
curr_dir = os.path.dirname(__file__)
server_py = curr_dir + os.sep + "PyroServer.py"
args = ["localhost", "2232"]
cmd = ["python3", server_py]
cmd.extend(args)
name_server = subprocess.Popen(cmd)
pid.append(name_server.pid)
print(f"Name server pid = {name_server.pid}")

time.sleep(1)

# This code block can be used to start the redis instance if
# the user does not already have it running.
# We assume that it has been configured and running.

# # Start redis server on port 6379
# cmd = ["redis-server", "/usr/local/etc/redis-1.conf"]
# r1 = subprocess.Popen(cmd)
# print(f"Redis server 1 pid = {r1.pid}")
# time.sleep(3)

# # Start redis server on port 6380
# cmd = ["redis-server", "/usr/local/etc/redis-2.conf"]
# r2 = subprocess.Popen(cmd)
# print(f"Redis server 2 pid = {r1.pid}")
# time.sleep(3)

# # Start redis server on port 6381
# cmd = ["redis-server", "/usr/local/etc/redis-3.conf"]
# r3 = subprocess.Popen(cmd)
# print(f"Redis server 3 pid = {r1.pid}")
# time.sleep(3)

# Indicates the port of the redis server
first_redis_port = 6379

# Starts all nodes
for i in range(3):
    # Starts sequencer
    if i == 0:
        curr_dir = os.path.dirname(__file__)
        node_py = curr_dir + os.sep + "NodeSequential.py"
        print("Spawning sequencer...")
        args = ["localhost", str(first_redis_port + i), "localhost", "2232", "sequencer"]
        cmd = ["python3", node_py]
        cmd.extend(args)
        n = subprocess.Popen(cmd)
        pid.append(n.pid)
        time.sleep(1)
    else:
        curr_dir = os.path.dirname(__file__)
        node_py = curr_dir + os.sep + "NodeSequential.py"
        node_name = "node" + str(i)
        print("Spawning " + node_name)
        args = ["localhost", str(first_redis_port + i), "localhost", "2232", node_name]
        cmd = ["python3", node_py]
        cmd.extend(args)
        n = subprocess.Popen(cmd)
        pid.append(n.pid)
        time.sleep(1)

#####################################################
# Example client activity & performance testing
# Single client hitting multiple nodes
#####################################################
# Locate the nodes.
# Shows the nodes blocking on current thread during a set.

# Stores results for aggregation
get_times = []
set_times = []

ns = Pyro4.locateNS("localhost", 2232)
ns_list = ns.list()
uri_str = ns_list['node1']
node1 = Pyro4.Proxy(uri_str)
uri_str = ns_list['node2']
node2 = Pyro4.Proxy(uri_str)
seq = Pyro4.Proxy(ns_list["sequencer"])

for i in range(20):

    print("--------------------------------------------------------------------")
    print("The next 3 get requests after the set should return the same value, ")
    print("as the set should be blocking.")
    print("--------------------------------------------------------------------")

    start = time.time()
    print(node2.set("test", str(random.randint(0, i))))
    stop = time.time()
    set_times.append((stop-start))

    start = time.time()
    print(node1.get("test"))
    stop = time.time()
    get_times.append((stop - start))

    start = time.time()
    print(node2.get("test"))
    stop = time.time()
    get_times.append((stop - start))

    start = time.time()
    print(seq.get("test"))
    stop = time.time()
    get_times.append((stop - start))

    print("--------------------------------------------------------------------")

print(f"The average time for a get request to return is {sum(get_times) / len(get_times)}")
print(f"The average time for a set request to return is {sum(set_times) / len(set_times)}")
print("--------------------------------------------------------------------")


time.sleep(1)

#####################################################
# Other tests
#####################################################
def test_get(key, node):
    ns = Pyro4.locateNS("localhost", 2232)
    ns_list = ns.list()
    uri_str = ns_list[node]
    n = Pyro4.Proxy(uri_str)
    print(n.get(key))

def test_set(key, value, node):
    ns = Pyro4.locateNS("localhost", 2232)
    ns_list = ns.list()
    uri_str = ns_list[node]
    n = Pyro4.Proxy(uri_str)
    return n.set(key, value)

# Runs two instances of the functions via threads to set and get.
# These should all run without issues, server should decide on an order
# Multiple concurrent sets
print("---Multiple sets---")
x = threading.Thread(target=test_set, args=("test2", "hello", "node1"))
y = threading.Thread(target=test_set, args=("test2", "hello world", "node2"))
x.start()
y.start()
print()

#####################################################
# Terminate.
time.sleep(10)

for id in pid:
    os.kill(id, signal.SIGKILL)
