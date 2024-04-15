#include <stdio.h>
#include <stdlib.h>
#include <openssl/bio.h> /* BasicInput/Output streams */
#include <openssl/err.h> /* errors */
#include <openssl/ssl.h> /* core library */

#define BuffSize 1024

void report_and_exit(const char* msg)
{ 
    perror(msg); 
    ERR_print_errors_fp(stderr); 
    exit(-1); 
}

void init_ssl()
{ 
    SSL_load_error_strings(); 
    SSL_library_init(); 
}

void cleanup(SSL_CTX* ctx, BIO* bio)
{ 
    SSL_CTX_free(ctx); 
    BIO_free_all(bio); 
}

void secure_connect(const char* host, const char *port)
{ 
    char hostport[BuffSize]; 
    char request[BuffSize]; 
    char response[BuffSize];

    const SSL_METHOD* method = TLS_client_method(); 
    if (NULL == method) report_and_exit("TLSv1_3_client_method...");

    SSL_CTX* ctx = SSL_CTX_new(method); 
    if (NULL == ctx) report_and_exit("SSL_CTX_new...");

    /* verify truststore, check cert */ 
    if (!SSL_CTX_load_verify_locations(ctx, 
                          "/etc/ssl/certs/ca-certificates.crt", /* CAFILE */ 
                          "/etc/ssl/certs/")) /* CAPATH */
        report_and_exit("SSL_CTX_load_verify_locations...");

    BIO* bio = BIO_new_ssl_connect(ctx); 
    if (NULL == bio) report_and_exit("BIO_new_ssl_connect...");

    SSL* ssl = NULL;
    BIO_get_ssl(bio, &ssl); /* session */ 

    /* link bio channel, SSL session, and server endpoint */ 

    sprintf(hostport, "%s:%s", host, port);

    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY); /* robustness */ 
    SSL_set_tlsext_host_name(ssl, host);
    BIO_set_conn_hostname(bio, hostport); /* prepare to connect */

    /* try to connect */ 
    int x;
    if ((x=BIO_do_connect(bio)) <= 0)
    { 
        printf("x=%d\n",x);
        cleanup(ctx, bio); 
        report_and_exit("BIO_do_connect..."); 
    }

    long verify_flag = SSL_get_verify_result(ssl); 
    if (verify_flag != X509_V_OK) 
    {
        printf("flag=%ld\n", verify_flag);
        fprintf(stderr, "##### Certificate verification error (%i) ...\n", 
            (int) verify_flag);

        // see /usr/include/openssl/x509_vfy.h

        exit(1);
    }

    printf ("SSL cert good\n");

    cleanup(ctx, bio); 
}

int main(int argc, char *argv[])
{ 
    init_ssl();

    const char *host = "www.google.com";
    const char *port = "443"; 
    if (argc>1) host=argv[1];
    if (argc>2) port=argv[2];
    fprintf(stderr, "Trying a HTTPS connection to %s:%s...\n", host, port); 
    secure_connect(host, port);

    return 0; 
}
