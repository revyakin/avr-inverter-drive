#ifndef SLIP_H
#define SLIP_H

#define PKG_MAX_LEN 16

void send_package(char len);
int recv_package(void);

extern unsigned char package[PKG_MAX_LEN];

#endif /* SLIP_H */