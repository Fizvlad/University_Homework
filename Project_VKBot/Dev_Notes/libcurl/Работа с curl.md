# Работа с библиотекой curl для C++

## Общая схема работы
**Подключение**  
`#include <curl/curl.h>`

**Инициализация**  
```
    CURL *curl; // Объект curl  
    curl = curl_easy_init(); // Инициализация
```  
*easy* в данном случае обозначает, что используется однопоточная версия. Также есть версия *multi* для асинхронной работы.

**Настройка**  
Настройка происходит с помощью вызова функций вида  
`CURLcode curl_easy_setopt(CURL *handle, CURLoption option, parameter);`  
*Подробнее позже.*

**Запрос**  
`CURLcode curl_easy_perform(CURL * easy_handle);`  
Производит запрос по ранее заданным настройкам.  
Можно использовать один и тот же handle для нескольких запросов подряд *(но не одновременно)*. Иногда это даже полезно и приводит к ускорению работы.

**Закрытие соединения**  
`void curl_easy_cleanup(CURL * handle);`  
**Обязательно** должен быть вызван после выполнения всех действий. Закрывает все соединения.

--------------------

## Опции запроса

### Поведение

*CURLOPT_VERBOSE* - Display verbose information

*CURLOPT_HEADER* - Include the header in the body output

*CURLOPT_NOPROGRESS* - Shut off the progress meter

*CURLOPT_NOSIGNAL* - Do not install signal handlers

*CURLOPT_WILDCARDMATCH* - Transfer multiple files according to a file name pattern

### Callback

*CURLOPT_WRITEFUNCTION* - Callback for writing data

*CURLOPT_WRITEDATA* - Data pointer to pass to the write callback

*CURLOPT_READFUNCTION* - Callback for reading data

*CURLOPT_READDATA* - Data pointer to pass to the read callback

*CURLOPT_IOCTLFUNCTION* - Callback for I/O operations

*CURLOPT_IOCTLDATA* - Data pointer to pass to the I/O callback

*CURLOPT_SEEKFUNCTION* - Callback for seek operations

*CURLOPT_SEEKDATA* - Data pointer to pass to the seek callback

*CURLOPT_SOCKOPTFUNCTION* - Callback for sockopt operations

*CURLOPT_SOCKOPTDATA* - Data pointer to pass to the sockopt callback

*CURLOPT_OPENSOCKETFUNCTION* - Callback for socket creation

*CURLOPT_OPENSOCKETDATA* - Data pointer to pass to the open socket callback

*CURLOPT_CLOSESOCKETFUNCTION* - Callback for closing socket

*CURLOPT_CLOSESOCKETDATA* - Data pointer to pass to the close socket callback

*CURLOPT_PROGRESSFUNCTION* - OBSOLETE callback for progress meter

*CURLOPT_PROGRESSDATA* - Data pointer to pass to the progress meter callback

*CURLOPT_XFERINFOFUNCTION* - Callback for progress meter

*CURLOPT_XFERINFODATA* - Data pointer to pass to the progress meter callback

*CURLOPT_HEADERFUNCTION* - Callback for writing received headers

*CURLOPT_HEADERDATA* - Data pointer to pass to the header callback

*CURLOPT_DEBUGFUNCTION* - Callback for debug information

*CURLOPT_DEBUGDATA* - Data pointer to pass to the debug callback

*CURLOPT_SSL_CTX_FUNCTION* - Callback for SSL context logic

*CURLOPT_SSL_CTX_DATA* - Data pointer to pass to the SSL context callback

*CURLOPT_CONV_TO_NETWORK_FUNCTION* - Callback for code base conversion

*CURLOPT_CONV_FROM_NETWORK_FUNCTION* - Callback for code base conversion

*CURLOPT_CONV_FROM_UTF8_FUNCTION* - Callback for code base conversion

*CURLOPT_INTERLEAVEFUNCTION* - Callback for RTSP interleaved data

*CURLOPT_INTERLEAVEDATA* - Data pointer to pass to the RTSP interleave callback

*CURLOPT_CHUNK_BGN_FUNCTION* - Callback for wildcard download start of chunk

*CURLOPT_CHUNK_END_FUNCTION* - Callback for wildcard download end of chunk

*CURLOPT_CHUNK_DATA* - Data pointer to pass to the chunk callbacks

*CURLOPT_FNMATCH_FUNCTION* - Callback for wildcard matching

*CURLOPT_FNMATCH_DATA* - Data pointer to pass to the wildcard matching callback

*CURLOPT_SUPPRESS_CONNECT_HEADERS* - Suppress proxy CONNECT response headers from user callbacks

### Ошибки

*CURLOPT_ERRORBUFFER* - Error message buffer

*CURLOPT_STDERR* - stderr replacement stream

*CURLOPT_FAILONERROR* - Fail on HTTP 4xx errors

*CURLOPT_KEEP_SENDING_ON_ERROR* - Keep sending on HTTP >= 300 errors

### Сетевые настройки

*CURLOPT_URL* - URL to work on

*CURLOPT_PATH_AS_IS* - Disable squashing /../ and /./ sequences in the path

*CURLOPT_PROTOCOLS* - Allowed protocols

*CURLOPT_REDIR_PROTOCOLS* - Protocols to allow redirects to

*CURLOPT_DEFAULT_PROTOCOL* - Default protocol

*CURLOPT_PROXY* - Proxy to use

*CURLOPT_PRE_PROXY* - Socks proxy to use

*CURLOPT_PROXYPORT* - Proxy port to use

*CURLOPT_PROXYTYPE* - Proxy type

*CURLOPT_NOPROXY* - Filter out hosts from proxy use

*CURLOPT_HTTPPROXYTUNNEL* - Tunnel through the HTTP proxy

*CURLOPT_CONNECT_TO* - Connect to a specific host and port

*CURLOPT_SOCKS5_AUTH* - Socks5 authentication methods

*CURLOPT_SOCKS5_GSSAPI_SERVICE* - Socks5 GSSAPI service name

*CURLOPT_SOCKS5_GSSAPI_NEC* - Socks5 GSSAPI NEC mode

*CURLOPT_PROXY_SERVICE_NAME* - Proxy authentication service name

*CURLOPT_SERVICE_NAME* - Authentication service name

*CURLOPT_INTERFACE* - Bind connection locally to this

*CURLOPT_LOCALPORT* - Bind connection locally to this port

*CURLOPT_LOCALPORTRANGE* - Bind connection locally to port range

*CURLOPT_DNS_CACHE_TIMEOUT* - Timeout for DNS cache

*CURLOPT_DNS_USE_GLOBAL_CACHE* - OBSOLETE Enable global DNS cache

*CURLOPT_BUFFERSIZE* - Ask for alternate buffer size

*CURLOPT_PORT* - Port number to connect to

*CURLOPT_TCP_FASTOPEN* - Enable TFO, TCP Fast Open

*CURLOPT_TCP_NODELAY* - Disable the Nagle algorithm

*CURLOPT_ADDRESS_SCOPE* - IPv6 scope for local addresses

*CURLOPT_TCP_KEEPALIVE* - Enable TCP keep-alive

*CURLOPT_TCP_KEEPIDLE* - Idle time before sending keep-alive

*CURLOPT_TCP_KEEPINTVL* - Interval between keep-alive probes

*CURLOPT_UNIX_SOCKET_PATH* - Path to a Unix domain socket

*CURLOPT_ABSTRACT_UNIX_SOCKET* - Path to an abstract Unix domain socket

### Аутентификация

*CURLOPT_NETRC* - Enable .netrc parsing

*CURLOPT_NETRC_FILE* - .netrc file name

*CURLOPT_USERPWD* - User name and password

*CURLOPT_PROXYUSERPWD* - Proxy user name and password

*CURLOPT_USERNAME* - User name

*CURLOPT_PASSWORD* - Password

*CURLOPT_LOGIN_OPTIONS* - Login options

*CURLOPT_PROXYUSERNAME* - Proxy user name

*CURLOPT_PROXYPASSWORD* - Proxy password

*CURLOPT_HTTPAUTH - HTTP* server authentication methods

*CURLOPT_TLSAUTH_USERNAME* - TLS authentication user name

*CURLOPT_PROXY_TLSAUTH_USERNAME* - Proxy TLS authentication user name

*CURLOPT_TLSAUTH_PASSWORD* - TLS authentication password

*CURLOPT_PROXY_TLSAUTH_PASSWORD* - Proxy TLS authentication password

*CURLOPT_TLSAUTH_TYPE* - TLS authentication methods

*CURLOPT_PROXY_TLSAUTH_TYPE* - Proxy TLS authentication methods

*CURLOPT_PROXYAUTH* - HTTP proxy authentication methods

*CURLOPT_SASL_IR* - Enable SASL initial response

*CURLOPT_XOAUTH2_BEARER* - OAuth2 bearer token

### HTTP

*CURLOPT_AUTOREFERER* - Automatically set Referer: header

*CURLOPT_ACCEPT_ENCODING* - Accept-Encoding and automatic decompressing data

*CURLOPT_TRANSFER_ENCODING* - Request Transfer-Encoding

*CURLOPT_FOLLOWLOCATION* - Follow HTTP redirects

*CURLOPT_UNRESTRICTED_AUTH* - Do not restrict authentication to original host

*CURLOPT_MAXREDIRS* - Maximum number of redirects to follow

*CURLOPT_POSTREDIR* - How to act on redirects after POST

*CURLOPT_PUT* - Issue a HTTP PUT request

*CURLOPT_POST* - Issue a HTTP POST request

*CURLOPT_POSTFIELDS* - Send a POST with this data

*CURLOPT_POSTFIELDSIZE* - The POST data is this big

*CURLOPT_POSTFIELDSIZE_LARGE* - The POST data is this big

*CURLOPT_COPYPOSTFIELDS* - Send a POST with this data - and copy it

*CURLOPT_HTTPPOST* - Multipart formpost HTTP POST

*CURLOPT_REFERER* - Referer: header

*CURLOPT_USERAGENT* - User-Agent: header

*CURLOPT_HTTPHEADER* - Custom HTTP headers

*CURLOPT_HEADEROPT* - Control custom headers

*CURLOPT_PROXYHEADER* - Custom HTTP headers sent to proxy

*CURLOPT_HTTP200ALIASES* - Alternative versions of 200 OK

*CURLOPT_COOKIE* - Cookie(s) to send

*CURLOPT_COOKIEFILE* - File to read cookies from

*CURLOPT_COOKIEJAR* - File to write cookies to

*CURLOPT_COOKIESESSION* - Start a new cookie session

*CURLOPT_COOKIELIST* - Add or control cookies

*CURLOPT_HTTPGET* - Do a HTTP GET request

*CURLOPT_REQUEST_TARGET* - Set the request target

*CURLOPT_HTTP_VERSION* - HTTP version to use

*CURLOPT_IGNORE_CONTENT_LENGTH* - Ignore Content-Length

*CURLOPT_HTTP_CONTENT_DECODING* - Disable Content decoding

*CURLOPT_HTTP_TRANSFER_DECODING* - Disable Transfer decoding

*CURLOPT_EXPECT_100_TIMEOUT_MS* - 100-continue timeout

*CURLOPT_PIPEWAIT* - Wait on connection to pipeline on it

*CURLOPT_STREAM_DEPENDS* - This HTTP/2 stream depends on another

*CURLOPT_STREAM_DEPENDS_E* - This HTTP/2 stream depends on another exclusively

*CURLOPT_STREAM_WEIGHT* - Set this HTTP/2 stream's weight

### SMTP

*CURLOPT_MAIL_FROM* - Address of the sender

*CURLOPT_MAIL_RCPT* - Address of the recipients

*CURLOPT_MAIL_AUTH* - Authentication address

### TFTP

*CURLOPT_TFTP_BLKSIZE* - TFTP block size

*CURLOPT_TFTP_NO_OPTIONS* - Do not send TFTP options requests

### FTP

*CURLOPT_FTPPORT* - Use active FTP

*CURLOPT_QUOTE* - Commands to run before transfer

*CURLOPT_POSTQUOTE* - Commands to run after transfer

*CURLOPT_PREQUOTE* - Commands to run just before transfer

*CURLOPT_APPEND* - Append to remote file

*CURLOPT_FTP_USE_EPRT* - Use EPTR

*CURLOPT_FTP_USE_EPSV* - Use EPSV

*CURLOPT_FTP_USE_PRET* - Use PRET

*CURLOPT_FTP_CREATE_MISSING_DIRS* - Create missing directories on the remote server

*CURLOPT_FTP_RESPONSE_TIMEOUT* - Timeout for FTP responses

*CURLOPT_FTP_ALTERNATIVE_TO_USER* - Alternative to USER

*CURLOPT_FTP_SKIP_PASV_IP* - Ignore the IP address in the PASV response

*CURLOPT_FTPSSLAUTH* - Control how to do TLS

*CURLOPT_FTP_SSL_CCC* - Back to non-TLS again after authentication

*CURLOPT_FTP_ACCOUNT* - Send ACCT command

*CURLOPT_FTP_FILEMETHOD* - Specify how to reach files

### RTSP

*CURLOPT_RTSP_REQUEST* - RTSP request

*CURLOPT_RTSP_SESSION_ID* - RTSP session-id

*CURLOPT_RTSP_STREAM_URI* - RTSP stream URI

*CURLOPT_RTSP_TRANSPORT* - RTSP Transport: header

*CURLOPT_RTSP_CLIENT_CSEQ* - Client CSEQ number

*CURLOPT_RTSP_SERVER_CSEQ* - CSEQ number for RTSP Server->Client request

### Протокол

*CURLOPT_TRANSFERTEXT* - Use text transfer

*CURLOPT_PROXY_TRANSFER_MODE* - Add transfer mode to URL over proxy

*CURLOPT_CRLF* - Convert newlines

*CURLOPT_RANGE* - Range requests

*CURLOPT_RESUME_FROM* - Resume a transfer

*CURLOPT_RESUME_FROM_LARGE* - Resume a transfer

*CURLOPT_CUSTOMREQUEST* - Custom request/method

*CURLOPT_FILETIME* - Request file modification date and time

*CURLOPT_DIRLISTONLY* - List only

*CURLOPT_NOBODY* - Do not get the body contents

*CURLOPT_INFILESIZE* - Size of file to send

*CURLOPT_INFILESIZE_LARGE* - Size of file to send

*CURLOPT_UPLOAD* - Upload data

*CURLOPT_MIMEPOST* - Post/send MIME data

*CURLOPT_MAXFILESIZE* - Maximum file size to get

*CURLOPT_MAXFILESIZE_LARGE* - Maximum file size to get

*CURLOPT_TIMECONDITION* - Make a time conditional request

*CURLOPT_TIMEVALUE* - Time value for the time conditional request

### Настройки соединения

*CURLOPT_TIMEOUT* - Timeout for the entire request

*CURLOPT_TIMEOUT_MS* - Millisecond timeout for the entire request

*CURLOPT_LOW_SPEED_LIMIT* - Low speed limit to abort transfer

*CURLOPT_LOW_SPEED_TIME* - Time to be below the speed to trigger low speed abort

*CURLOPT_MAX_SEND_SPEED_LARGE* - Cap the upload speed to this

*CURLOPT_MAX_RECV_SPEED_LARGE* - Cap the download speed to this

*CURLOPT_MAXCONNECTS* - Maximum number of connections in the connection pool

*CURLOPT_FRESH_CONNECT* - Use a new connection

*CURLOPT_FORBID_REUSE* - Prevent subsequent connections from re-using this

*CURLOPT_CONNECTTIMEOUT* - Timeout for the connection phase

*CURLOPT_CONNECTTIMEOUT_MS* - Millisecond timeout for the connection phase

*CURLOPT_IPRESOLVE* - IP version to resolve to

*CURLOPT_CONNECT_ONLY* - Only connect, nothing else

*CURLOPT_USE_SSL* - Use TLS/SSL

*CURLOPT_RESOLVE* - Provide fixed/fake name resolves

*CURLOPT_DNS_INTERFACE* - Bind name resolves to this interface

*CURLOPT_DNS_LOCAL_IP4* - Bind name resolves to this IP4 address

*CURLOPT_DNS_LOCAL_IP6* - Bind name resolves to this IP6 address

*CURLOPT_DNS_SERVERS* - Preferred DNS servers

*CURLOPT_ACCEPTTIMEOUT_MS* - Timeout for waiting for the server's connect back to be accepted

### SSL и безопасность

*CURLOPT_SSLCERT* - Client cert

*CURLOPT_PROXY_SSLCERT* - Proxy client cert

*CURLOPT_SSLCERTTYPE* - Client cert type

*CURLOPT_PROXY_SSLCERTTYPE* - Proxy client cert type

*CURLOPT_SSLKEY* - Client key

*CURLOPT_PROXY_SSLKEY* - Proxy client key

*CURLOPT_SSLKEYTYPE* - Client key type

*CURLOPT_PROXY_SSLKEYTYPE* - Proxy client key type

*CURLOPT_KEYPASSWD* - Client key password

*CURLOPT_PROXY_KEYPASSWD* - Proxy client key password

*CURLOPT_SSL_ENABLE_ALPN* - Enable use of ALPN

*CURLOPT_SSL_ENABLE_NPN* - Enable use of NPN

*CURLOPT_SSLENGINE* - Use identifier with SSL engine

*CURLOPT_SSLENGINE_DEFAULT* - Default SSL engine

*CURLOPT_SSL_FALSESTART* - Enable TLS False Start

*CURLOPT_SSLVERSION* - SSL version to use

*CURLOPT_PROXY_SSLVERSION* - Proxy SSL version to use

*CURLOPT_SSL_VERIFYHOST* - Verify the host name in the SSL certificate

*CURLOPT_PROXY_SSL_VERIFYHOST* - Verify the host name in the proxy SSL certificate

*CURLOPT_SSL_VERIFYPEER* - Verify the SSL certificate

*CURLOPT_PROXY_SSL_VERIFYPEER* - Verify the proxy SSL certificate

*CURLOPT_SSL_VERIFYSTATUS* - Verify the SSL certificate's status

*CURLOPT_CAINFO* - CA cert bundle

*CURLOPT_PROXY_CAINFO* - Proxy CA cert bundle

*CURLOPT_ISSUERCERT* - Issuer certificate

*CURLOPT_CAPATH* - Path to CA cert bundle

*CURLOPT_PROXY_CAPATH* - Path to proxy CA cert bundle

*CURLOPT_CRLFILE* - Certificate Revocation List

*CURLOPT_PROXY_CRLFILE* - Proxy Certificate Revocation List

*CURLOPT_CERTINFO* - Extract certificate info

*CURLOPT_PINNEDPUBLICKEY* - Set pinned SSL public key

*CURLOPT_PROXY_PINNEDPUBLICKEY* - Set the proxy's pinned SSL public key

*CURLOPT_RANDOM_FILE* - Provide source for entropy random data

*CURLOPT_EGDSOCKET* - Identify EGD socket for entropy

*CURLOPT_SSL_CIPHER_LIST* - Ciphers to use

*CURLOPT_PROXY_SSL_CIPHER_LIST* - Proxy ciphers to use

*CURLOPT_SSL_SESSIONID_CACHE* - Disable SSL session-id cache

*CURLOPT_SSL_OPTIONS* - Control SSL behavior

*CURLOPT_PROXY_SSL_OPTIONS* - Control proxy SSL behavior

*CURLOPT_KRBLEVEL* - Kerberos security level

*CURLOPT_GSSAPI_DELEGATION* - Disable GSS-API delegation

### SSH

*CURLOPT_SSH_AUTH_TYPES* - SSH authentication types

*CURLOPT_SSH_COMPRESSION* - Enable SSH compression

*CURLOPT_SSH_HOST_PUBLIC_KEY_MD5* - MD5 of host's public key

*CURLOPT_SSH_PUBLIC_KEYFILE* - File name of public key

*CURLOPT_SSH_PRIVATE_KEYFILE* - File name of private key

*CURLOPT_SSH_KNOWNHOSTS* - File name with known hosts

*CURLOPT_SSH_KEYFUNCTION* - Callback for known hosts handling

*CURLOPT_SSH_KEYDATA* - Custom pointer to pass to ssh key callback

### Прочее

*CURLOPT_PRIVATE* - Private pointer to store

*CURLOPT_SHARE* - Share object to use

*CURLOPT_NEW_FILE_PERMS* - Mode for creating new remote files

*CURLOPT_NEW_DIRECTORY_PERMS* - Mode for creating new remote directories

### Telnet

*CURLOPT_TELNETOPTIONS* - TELNET options
