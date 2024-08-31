#include "stdafx.h"
#include <string.h>
#include "txtStream.h"

void txtoStream::printf(char *fmt, char *str)
{
  if (level==0)
    return;
	if (fmt == nullptr || str == nullptr)
		return;
  snprintf(buf2, sizeof(buf2), fmt, str);
	write(buf2);
}

void txtoStream::printf(char *fmt, char *str, int n)
{
  if (level==0)
    return;
	if (fmt == nullptr || str == nullptr)
		return;
	snprintf(buf2, sizeof(buf2), fmt, str, n);
 write(buf2);
}

void txtoStream::printf(char *fmt, char *str, char *str2)
{
  if (level==0)
    return;
	if (fmt == nullptr || str == nullptr || str2 == nullptr)
		return;
	snprintf(buf2, sizeof(buf2), fmt, str, str2);
 write(buf2);
}

void txtoStream::printf(char *fmt, char *str, char *str2, int n)
{
  if (level==0)
    return;
	if (fmt == nullptr || str == nullptr ||  str2 == nullptr)
		return;
	snprintf(buf2, sizeof(buf2), fmt, str, str2, n);
	write(buf2);
}

void txtoStream::printf(char *fmt, int n)
{
  if (level==0)
    return;
	if (fmt == nullptr)
		return;
	snprintf(buf2, sizeof(buf2), fmt, n);
	write(buf2);
}

void txtoStream::printf(const char *fmt, int64_t n)
{
  if (level==0)
    return;
	if (fmt == nullptr)
		return;
	snprintf(buf2, sizeof(buf2), fmt, n);
	write(buf2);
}

void txtoStream::printf(char *fmt, int n, int m)
{
  if (level==0)
    return;
	if (fmt == nullptr)
		return;
	snprintf(buf2, sizeof(buf2), fmt, n, m);
	write(buf2);
}

void txtoStream::printf(char *fmt, int n, char *str)
{
  if (level==0)
    return;
	if (fmt == nullptr || str == nullptr)
		return;
	snprintf(buf2, sizeof(buf2), fmt, n, str);
	write(buf2);
}

void txtoStream::puts(const char *str)
{
  if (level==0)
    return;
	if (str == nullptr)
		return;
	while(*str) {
    putch(*str);
    str++;
  }
}

void txtoStream::writeAsHex(const void *buf, int64_t len)
{
	int n;
	char *cbuf = (char *)buf;

	if (buf == nullptr)
		return;
	printf("%04X:", len);
	for (n = 0; n < len; n++) {
		printf("%02X", (int)cbuf[n]);
	}
	printf("\n");
}

void txtiStream::readAsHex(const void *buf, int len)
{
	char cbuf[8];
	char *p;
	int n;
	int val;

	if (buf == nullptr)
		return;
	p = (char *)buf;
	read(cbuf, 5);
	for (n = 0; n < len; n++) {
		read(cbuf, 2);
		cbuf[2] = '\0';
		val = strtoul(cbuf, nullptr, 16);
		p[n] = val;
	}
}

