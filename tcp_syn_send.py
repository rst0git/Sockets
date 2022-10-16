#!/usr/bin/env python3
import socket
import sys

from scapy.all import IP, TCP, Ether, get_if_hwaddr, get_if_list, sendp


def main():
    if len(sys.argv) != 3:
        print('pass 2 arguments: <destination> "<message>"')
        sys.exit(1)

    addr = socket.gethostbyname(sys.argv[1])
    iface = "eth0"

    print(f"sending on interface  {iface} to {str(addr)}")

    pkt = Ether(src=get_if_hwaddr(iface), dst='ff:ff:ff:ff:ff:ff')
    pkt /= IP(dst=addr)
    pkt /= TCP(dport=1234, sport=1235, flags='S')
    pkt /= sys.argv[2]
    pkt.show2()

    sendp(pkt, iface=iface, verbose=False)


if __name__ == '__main__':
    main()
