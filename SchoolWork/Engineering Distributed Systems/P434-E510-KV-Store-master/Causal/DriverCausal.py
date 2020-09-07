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
    # Starts primary
    if i == 0:
        print("Spawning primary...")
        curr_dir = os.path.dirname(__file__)
        node_py = curr_dir + os.sep + "NodeCausal.py"
        args = ["localhost", str(first_redis_port + i), "localhost", "2232", "primary"]
        cmd = ["python3", node_py]
        cmd.extend(args)
        n = subprocess.Popen(cmd)
        pid.append(n.pid)
        time.sleep(1)
    else:
        curr_dir = os.path.dirname(__file__)
        node_py = curr_dir + os.sep + "NodeCausal.py"
        node_name = "node" + str(i)
        print("Spawning " + node_name)
        args = ["localhost", str(first_redis_port + i), "localhost", "2232", node_name]
        cmd = ["python3", node_py]
        cmd.extend(args)
        n = subprocess.Popen(cmd)
        pid.append(n.pid)
        time.sleep(1)

# Starts client instances
for i in range(2):
    curr_dir = os.path.dirname(__file__)
    node_py = curr_dir + os.sep + "ClientCausal.py"
    client_name = "client" + str(i)
    print("Spawning " + client_name)
    args = ["localhost", "2232", client_name]
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
# May take a couple of runs to show the wait on read,
# as the client is set-up to hit a random replica.
# Shows the causal relationship.

# Stores results for aggregation
get_times = []
set_times = []

ns = Pyro4.locateNS("localhost", 2232)
ns_list = ns.list()
uri_str = ns_list['client0']
client1 = Pyro4.Proxy(uri_str)
uri_str = ns_list['client1']
client2 = Pyro4.Proxy(uri_str)

for i in range(20):
    print("Attempt: " + str(i + 1))
    start = time.time()
    print(client1.set("test", "whosat--"))
    stop = time.time()
    set_times.append((stop-start))

    start = time.time()
    print(client1.get("test"))
    stop = time.time()
    get_times.append((stop - start))

    start = time.time()
    print(client2.set("test", "hello world!"))
    stop = time.time()
    set_times.append((stop - start))

    start = time.time()
    print(client1.get("test"))
    stop = time.time()
    get_times.append((stop - start))

    start = time.time()
    print(client2.get("test"))
    stop = time.time()
    get_times.append((stop - start))

    time.sleep(3)

    print()

print(f"The average time for a get request to return is {sum(get_times) / len(get_times)}")
print(f"The average time for a set request to return is {sum(set_times) / len(set_times)}")

time.sleep(10)
#####################################################

# Terminate.
for id in pid:
    os.kill(id, signal.SIGKILL)

