import subprocess
# Simple function to set volume to satelite
# Arg1 : ip address string
# Arg2 : Volume string


def set_satelite_volume(ip,volume):

    p1 = subprocess.Popen(['./subwoofer-vol-sender',ip,volume], stdout=subprocess.PIPE)
    output = p1.communicate()[0]
    print output
    return

set_satelite_volume('192.168.1.18',sys.argv[1]);
