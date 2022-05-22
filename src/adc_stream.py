# Simple simulation of ADC feeding Linux FIFO
 
import math, time, os, signal, sys, random
 
fifo_name = "/tmp/adc.fifo"
ymax = 2.0
delay = 0.5
nchans = 1
npoints = 1000
running = True
fifo_fd = None
 
def remove(fname):
    if os.path.exists(fname):
        os.remove(fname)
 
def shutdown(sig=None, frame=None):
    print("\nClosing..")
    if fifo_fd:
        f.close()
    remove(fifo_name)
    sys.exit(0)
 
print("%u samples, %u channels, %3.0f S/s" % (npoints, nchans, npoints/delay))
remove(fifo_name)
data = npoints * [0]
n = 0;
signal.signal(signal.SIGINT, shutdown)
os.mkfifo(fifo_name)
try:
    f = open(fifo_name, "w")
except:
    running = False
while running:
    for c in range(0, npoints, nchans):
        data[c] = (math.sin((n*2 + c) / 10.0) + 1.2) * ymax / 4.0
        if nchans > 1:
            data[c+1] = (math.cos((n*2 + c) / 100.0) + 0.8) * data[c]
            data[c+1] += random.random() / 4.0
    n += 1
    s = ",".join([("%1.3f" % d) for d in data])
    try:
        f.write(s + "\n")
        f.flush()
    except:
        running = False
    sys.stdout.write('.')
    sys.stdout.flush()
    time.sleep(delay)
shutdown()
