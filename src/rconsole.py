import socket
import code
import sys
import __builtin__

class RemoteShell(code.InteractiveConsole):
    def __init__(self, io, locals=None):
        self.io = io
        code.InteractiveConsole.__init__(self, locals=locals, filename='<RemoteShell>')

    def raw_input(self, prompt='8===>'):
        self.io.write(prompt)
        result = self.io.readline().rstrip()
        if result == '\x04':
            raise EOFError
        return result

    def write(self, msg):
        self.io.write(msg)

class DisplayHook(object):
    def __init__(self, dest):
        self.dest = dest
    def __call__(self, value):
        if value is not None:
            print >> self.dest, value
            __builtin__._ = value

def run_server(port=1337):
    print "starting server on port {}".format(port)
    print "stop the server with CTRL+C"
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.bind(('', 1337))

    print "listening for connections..."
    while True:
        sock.listen(0)
        try:
            conn, _ = sock.accept()
        except KeyboardInterrupt:
            print "Shutting down server..."
            sock.shutdown(socket.SHUT_RDWR)
            sock.close()
            print "...Done."
            break

        print "Accepted client connection. Attaching display hook"
        io = conn.makefile('rw',0)
        oldhook = sys.displayhook
        sys.displayhook = DisplayHook(io)
    
        print "Starting shell"
        try:
            shell = RemoteShell(io)
            shell.interact()
        finally:
            sys.displayhook = oldhook
            print "I/O restored, closing connection"
            conn.shutdown(socket.SHUT_RDWR)
            conn.close()


if __name__ == "__main__":
    run_server()

        
        
