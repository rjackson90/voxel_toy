import socket
import code
import sys

class RemoteShell(code.InteractiveConsole):
    """This class overrides methods of code.InteractiveConsole in order to 
    provide an interactive remote console which uses a plain socket for
    communication. The socket passed to the constructor should be a connected
    TCP socket in non-blocking mode. A blocking socket will not break the shell,
    but will make it unusable for embedding in other applications which have
    other tasks to be done.
    """
    def __init__(self, sock, context, banner=None):
        code.InteractiveConsole.__init__(self, context)
        self.sock = sock
        self.banner = banner
        self.print_banner()

    def tick(self):
        """This method should be called from the embedding application in order
        to execute one cycle of the read-eval-print loop. In order to maintain
        a responsive shell for the client, this method should be called 
        fairly frequently. This method returns True if it is OK to keep using 
        the shell, or False if EOF was received and the socket closed.
        """
        try:
            # Read text from the socket. Return if there's no input this tick
            text = self.readline(1024)
            if text is None:
                return True
            text = text.rstrip()

            # If the input is 0x04 (EOF char) the client wants to terminate
            if '\x04' in text:
                self.cleanup()
                return False

            # Redirect output streams over the socket, execute input text
            sys.stdout = self
            sys.stderr = self
            more = self.push(text)
            sys.stdout = sys.__stdout__
            sys.stderr = sys.__stderr__
       
            # Display an appropriate prompt
            if more:
                self.write(sys.ps2+' ')
            else:
                self.write(sys.ps1+' ')

        except socket.error, e:
            print "[DEBUG] Socket error {}: {}".format(e.errno, e.strerror)
            raise IOError
        except IOError:
            return False
        return True

    def write(self, data):
        try:
            self.sock.sendall(data)
        except socket.error, e:
            print "[DEBUG] Socket error on send. {}: {}".format(*e.args)
            raise IOError

    def readline(self, size):
        try:
            text = self.sock.recv(size)
            if text is not None:
                return text

        except socket.error, e:
            if e.errno != 11:
                print "[DEBUG] Socket error on recv. {}: {}".format(*e.args)
                raise IOError

    def print_banner(self):
        """Set shell prompts if they are not already set, print the startup
        banner.
        """
        try:
            sys.ps1
        except AttributeError:
            sys.ps1 = ">>>"
        
        try:
            sys.ps2
        except AttributeError:
            sys.ps2="..."
        
        cprt = "Type 'help', 'copyright', 'credits', or 'license' for more information."
        if self.banner is None:
            self.write("Python {} on {}\n{}\n({})\n".format(sys.version, sys.platform,
                cprt, self.__class__.__name__))
        else:
            self.write("{}\n".format(self.banner))
        
        self.write(sys.ps1+' ')

    def cleanup(self):
        """Close the connection to the client."""
        self.sock.shutdown(socket.SHUT_RDWR)
        self.sock.close()


def listen(port=1337):
    """Start listening for incoming connections on the given port. This
    fucntion returns the listening socket, which can be used to accept
    connections when ready.
    """
    print "Opening non-blocking TCP socket on port {}".format(port)
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.bind(('',port))
    sock.settimeout(0)
    sock.listen(0)
    return sock

def accept(sock, context=globals()):
    """Accept an incoming connection on the given socket, and start a remote
    interactive interpreter session. The 'sock' parameter gives the socket
    from which connections should be accepted. The 'context' parameter gives
    the module dict to be used by the remote shell. The default context is this
    module's global names. This function returns None if no client is
    immediately available, or an instance of a RemoteShell if a connection was
    established
    """
    try:
        conn, addr = sock.accept()
        conn.settimeout(0)
    except socket.error:
        return None
    except socket.timeout:
        return None

    print "Accepted client connection from {}.".format(addr)
    print "Starting Console"
    try:
        shell = RemoteShell(conn, context)
    except socket.error:
        print "Connection closed by client"
        shell.cleanup()

    return shell
