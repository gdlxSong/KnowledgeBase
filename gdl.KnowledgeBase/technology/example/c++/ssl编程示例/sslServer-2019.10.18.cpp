#include <stdio.h>
#include <openssl/evp.h>
#include <openssl/x509.h>
#include <openssl/ssl.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <sys/socket.h>
#include <netinet/in.h>  
#include <openssl/ssl.h>
#include<iostream>


#define SIZE_BUFFER 2048
#define TERMINATED -1
#define CERTIFICATE_FILE "server.crt"
#define PRIVATE_KEY 	 "server.key"
#define CHECK_NULLPTR(p, msg) if(NULL == (p)){std::cout<<(msg)<<std::endl; return TERMINATED;}
#define CHECK_EQ_1(value, msg) if(1 != value){std::cout<<(msg)<<std::endl; return TERMINATED;}
#define CHECK_ERR(val, msg) if(-1 == val){std::cout<<(msg)<<std::endl; return TERMINATED;}
#define PUT_MSG(msg) {std::cout<<msg<<std::endl;}
char response[] = "HTTP/1.1 200 OK\r\ncontent-type:text/html; charset=utf-8\r\ncontent-length:5\r\n\r\nhttps";


int main(int argc, char** argv){


	int iret = 0;
	socklen_t saddrlen = sizeof(struct sockaddr);
	struct sockaddr_in saddr_server, saddr_client;
	char buf[SIZE_BUFFER];



	//首先是初始化环境.
	SSL_library_init();

	//加载一个ssl/tls版本环境.
	SSL_METHOD *psmethod = SSLv23_server_method();
	CHECK_NULLPTR(psmethod, "call SSLv23_server_method faild.");

	//创建一个SSL的通信上下文.
	SSL_CTX* psctx = SSL_CTX_new(psmethod);
	CHECK_NULLPTR(psctx, "create SSL_CTX faild.");

	//是否验证对方呢. 不验证客户端.
	SSL_CTX_set_verify(psctx, SSL_VERIFY_NONE, NULL);

	//加载服务端证书.
	iret = SSL_CTX_use_certificate_file(psctx, CERTIFICATE_FILE, SSL_FILETYPE_PEM);
	CHECK_EQ_1(iret, "call SSL_CTX_use_certificate_file faild.");

	//加载服务端密钥.
	iret = SSL_CTX_use_PrivateKey_file(psctx, PRIVATE_KEY, SSL_FILETYPE_PEM);
	CHECK_EQ_1(iret, "call SSL_CTX_use_PrivateKey_file faild.");

	//----------------------自此握手的环境就基本创建完成.


	//可以先建立tcp/ip连接.
	int fd_listen = socket(AF_INET, SOCK_STREAM, 0);
	CHECK_ERR(fd_listen, "create listen socket faild.");

	//initializer socket.
	memset(&saddr_server, 0, sizeof(saddr_server));
	saddr_server.sin_family = AF_INET;
	saddr_server.sin_port = htons(443);
	saddr_server.sin_addr.s_addr = INADDR_ANY;

	//bind.
	iret = bind(fd_listen, reinterpret_cast<struct sockaddr*>(&saddr_server), saddrlen);
	CHECK_ERR(iret, "bind address faild.");

	//listen.
	iret = listen(fd_listen, 5);
	CHECK_ERR(iret, "listen socket faild.");

	//accept.
	int fd_client = accept(fd_listen, reinterpret_cast<struct sockaddr*>(&saddr_client), &saddrlen);
	CHECK_ERR(fd_client, "accept faild.");

	//啦啦啦，可以开始建立ssl/tls连接了.
	//首先创建一个SSL
	SSL *pssl = SSL_new(psctx);
	CHECK_NULLPTR(pssl, "create ssl faild.");

	//set socket to SSL
	iret = SSL_set_fd(pssl, fd_client);
	CHECK_EQ_1(iret, "call SSL_set_fd faild.");

	//然后就可以开始握手了.
	iret = SSL_accept(pssl);
	CHECK_EQ_1(iret, "The TLS/SSL handshake was not successful.");


	//如果握手成功,那么就可以传输消息了.
	//首先接受消息.
	iret = SSL_read(pssl, buf, SIZE_BUFFER-1);
	if(iret > 0){
		std::cout<<"got "<<iret<<" bytes data:"<<std::endl;
		std::cout<<buf<<std::endl;
	}
	else{
		PUT_MSG("SLL_read faild.");
	}
	iret = SSL_write(pssl, response, strlen(response));
	if(0 < iret){
		std::cout<<"send messga."<<std::endl;
	}

	SSL_shutdown(pssl);
	close(fd_client);
	SSL_free(pssl);
	SSL_CTX_free(psctx);

	return 0;
}