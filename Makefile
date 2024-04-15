all: sslverify sslget

sslverify: sslverify.c
	$(CC) -o $@ $< -lssl -lcrypto

sslget: sslget.c
	$(CC) -o $@ $< -lssl -lcrypto
