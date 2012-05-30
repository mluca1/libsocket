# include <iostream>
# include <string>
# include <string.h>

# include "../headers/libinetsocket.h"
# include "../headers++/socket.hpp"
# include "../headers++/inetbase.hpp"

# include <unistd.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <stdio.h>

// oo wrapper around libinetsocket

namespace libsocket
{
	using std::string;

/************** inet_dgram class (inet UDP sockets) ************/

	class inet_dgram : public inet_socket
	{
		public:

		// I/O
		// O
		ssize_t sndto(const void* buf, size_t len, const char* dsthost, const char* dstport, int sndto_flags=0); // flags: sendto()

		// I
		ssize_t rcvfrom(void* buf, size_t len, char* srchost, size_t hostlen, char* srcport, size_t portlen, int rcvfrom_flags=0, bool numeric=false);
		ssize_t rcvfrom(void* buf, size_t len, string& srchost, string& srcport, int rcvfrom_flags=0, bool numeric=false);

		// Getters
	};

	// I/O

	// I

	ssize_t inet_dgram::rcvfrom(void* buf, size_t len, char* hostbuf, size_t hostbuflen, char* portbuf, size_t portbuflen, int rcvfrom_flags, bool numeric)
	{
		ssize_t bytes;
		int num = ((numeric == true) ? NUMERIC : 0);

		if ( -1 == sfd )
			throw inet_exception(__FILE__,__LINE__,"inet_dgram::rcvfrom() - Socket already closed!\n");

		if ( -1 == (bytes = recvfrom_inet_dgram_socket(sfd,buf,len,hostbuf,hostbuflen,portbuf,portbuflen,rcvfrom_flags,num)) )
			throw inet_exception(__FILE__,__LINE__,"inet_dgram::rcvfrom() - recvfrom() failed!\n");

		return bytes;
	}


	ssize_t inet_dgram::rcvfrom(void* buf, size_t len, string& srchost, string& srcport, int rcvfrom_flags, bool numeric)
	{
		ssize_t bytes;

		char* host = new char[1024];
		char* port = new char[64];

		memset(host,0,1024);
		memset(port,0,64);

		bytes = rcvfrom(buf,len,host,1024,port,64,rcvfrom_flags,numeric);

		srchost.resize(strlen(host));
		srcport.resize(strlen(port));

		srchost = host;
		srcport = port;

		delete[] host;
		delete[] port;

		return bytes;
	}

	// O

	ssize_t inet_dgram::sndto(const void* buf, size_t len, const char* dsthost, const char* dstport, int sndto_flags)
	{
		ssize_t bytes;

		if ( -1 == sfd )
			throw inet_exception(__FILE__,__LINE__,"inet_dgram::sendto() - Socket already closed!\n");

		if ( -1 == (bytes = sendto_inet_dgram_socket(sfd,buf,len,dsthost,dstport,sndto_flags)) )
			throw inet_exception(__FILE__,__LINE__,"inet_dgram::sndto() - Error at sendto\n");

		return bytes;
	}
}
