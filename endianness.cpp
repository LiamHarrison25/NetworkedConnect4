#include "endianness.h"
#ifdef WIN32
#include <WinSock2.h>
#else
#include <arpa/inet.h>
#endif

uint16_t user_htons(const uint16_t& obj) { return htons(obj); }
uint32_t user_htonl(const uint32_t& obj) { return htonl(obj); }
uint64_t user_htonll(const uint64_t& obj) { return htonll(obj); }
uint16_t user_ntohs(const uint16_t& obj) { return ntohs(obj); }
uint32_t user_ntohl(const uint32_t& obj) { return ntohl(obj); }
uint64_t user_ntohll(const uint64_t& obj) { return ntohll(obj); }
