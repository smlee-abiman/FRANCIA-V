#!/usr/bin/env python3

import argparse
import datetime
import random
import selectors
import socket
import struct
import sys

from typing import Dict

DEFAULT_HOST = '0.0.0.0'
DEFAULT_PORT = 1234

DEFAULT_MONITORING_INTERVAL = 0.025

STATE_HANDSHAKING = 'HANDSHAKING'
STATE_READY = 'READY'
STATE_MONITORING = 'MONITORING'
STATE_DISCONNECTED = 'DISCONNECTED'

def log(msg):
    # type: (str) -> None
    sys.stderr.write(msg + '\n')
    sys.stderr.flush()


class Server(object):

    def __init__(self, address, monitoring_interval=DEFAULT_MONITORING_INTERVAL,
                 update=True):
        self._selector = selectors.DefaultSelector()
        self._server_sock = socket.socket()
        self._client_conns = {}  # type: Dict[socket.socket, str]
        self._address = address
        self._monitoring_interval = monitoring_interval
        self._update = update
        self._axis_data = [(0, 0, 0, 0)] * 8
        self._timestamp = float(random.randint(0, 1 << 28))

    def run(self):
        try:
            self._server_sock.bind(self._address)
            self._server_sock.listen(5)
            self._selector.register(self._server_sock, selectors.EVENT_READ,
                                    self._accept)
            log('listening at %s:%d' % self._address)
            while True:
                timeout = None
                if self._any_monitoring():
                    timeout = self._monitoring_interval
                for key, events in self._selector.select(timeout):
                    cb = key.data
                    cb(key.fileobj)
                self._update_monitoring()
        finally:
            self._selector.close()
            self._server_sock.close()
            for conn in self._client_conns:
                conn.close()

    def _accept(self, sock):
        assert sock is self._server_sock
        conn, addr = sock.accept()
        log('accepted connection from %s:%d' % addr)
        self._client_conns[conn] = STATE_HANDSHAKING
        self._selector.register(conn, selectors.EVENT_READ, self._recv)

    def _recv(self, conn):
        state = self._client_conns[conn]
        cb = self._STATES_MAP[state]
        try:
            new_state = cb(self, conn, state)
        except socket.error:
            new_state = STATE_DISCONNECTED
        if new_state == STATE_DISCONNECTED:
            log('client disconnected')
            self._client_conns.pop(conn)
            self._selector.unregister(conn)
            conn.close()
        else:
            self._client_conns[conn] = new_state

    def _handle_handshaking(self, conn, state):
        msg = conn.recv(1024)
        if not msg:
            return STATE_DISCONNECTED
        if msg == b'CMD_CONNECT\n':
            conn.sendall(b'CONNECT:0,8\n')
            return STATE_READY
        log('unhandled message: %r' % msg)
        return state

    def _handle_ready(self, conn, state):
        msg = conn.recv(1024)
        if not msg:
            return STATE_DISCONNECTED
        if msg == b'CMD_AXIS_MON_START:9\n':
            conn.sendall(b'ACK\n')
            return STATE_MONITORING
        log('unhandled message: %r' % msg)
        return state

    def _handle_monitoring(self, conn, state):
        msg = conn.recv(1024)
        if not msg:
            return STATE_DISCONNECTED
        log('unhandled message: %r' % msg)
        return state

    def _any_monitoring(self):
        return any(state == STATE_MONITORING
                   for state in self._client_conns.values())

    def _update_monitoring_data(self):
        new_axis_data = []
        for pos, speed, torque, err in self._axis_data:
            pos += random.randint(-1000, +1000)
            speed = min(max(speed + random.randint(-200, +200), 0), 5000)
            torque = min(max(torque + random.randint(-100, +100), -3000), +3000)
            err = random.randint(0, 32767)
            new_axis_data.append((pos, speed, torque, err))
        self._axis_data = new_axis_data
        self._timestamp += random.random() / 2.

    def _update_monitoring(self):
        if self._update:
            self._update_monitoring_data()

        data = build_monitoring_response(self._axis_data, self._timestamp)
        for conn, state in self._client_conns.items():
            if state != STATE_MONITORING:
                continue
            try:
                conn.sendall(data)
            except socket.error:
                pass

    _STATES_MAP = {
        STATE_HANDSHAKING: _handle_handshaking,
        STATE_READY: _handle_ready,
        STATE_MONITORING: _handle_monitoring,
    }


def build_monitoring_response(axis_data, timestamp):
    packed = [b'AXIS_MON    ']
    for pos, speed, torque, err in axis_data:
        packed.append(struct.pack('<llhh', pos, speed, torque, err))
    t = datetime.datetime.fromtimestamp(timestamp)
    packed.append(struct.pack(
        '<HBBBBBB', t.year, t.month, t.day, t.hour, t.minute, t.second, 0))
    return b''.join(packed)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('-n', '--monitoring-interval', metavar='SEC', type=float,
                    default=DEFAULT_MONITORING_INTERVAL,
                    help='send monitor response at the specified interval')
    ap.add_argument('--no-update', dest='update', action='store_false',
                    default=True,
                    help='do not update monitor data')
    args = ap.parse_args()
    try:
        server = Server((DEFAULT_HOST, DEFAULT_PORT),
                        monitoring_interval=args.monitoring_interval,
                        update=args.update)
        server.run()
    except KeyboardInterrupt:
        pass


if __name__ == '__main__':
    main()
