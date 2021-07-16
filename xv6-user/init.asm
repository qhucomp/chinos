
xv6-user/_init:     file format elf64-littleriscv


Disassembly of section .text:

0000000000000000 <main>:
#include "kernel/include/fcntl.h"
#include "xv6-user/user.h"

int
main(void)
{
   0:	1141                	addi	sp,sp,-16
   2:	e406                	sd	ra,8(sp)
   4:	e022                	sd	s0,0(sp)
   6:	0800                	addi	s0,sp,16
  dev(O_RDWR, CONSOLE, 0);
   8:	4601                	li	a2,0
   a:	4585                	li	a1,1
   c:	4509                	li	a0,2
   e:	00000097          	auipc	ra,0x0
  12:	3ae080e7          	jalr	942(ra) # 3bc <dev>
  dup(0);  // stdout
  16:	4501                	li	a0,0
  18:	00000097          	auipc	ra,0x0
  1c:	37e080e7          	jalr	894(ra) # 396 <dup>
  dup(0);  // stderr
  20:	4501                	li	a0,0
  22:	00000097          	auipc	ra,0x0
  26:	374080e7          	jalr	884(ra) # 396 <dup>
  // printf("testing...\n");
  if(clone() == 0) {
  2a:	00000097          	auipc	ra,0x0
  2e:	2f4080e7          	jalr	756(ra) # 31e <clone>
  32:	c111                	beqz	a0,36 <main+0x36>
    printf("ppid:%d",getppid());
  }
  while(1);
  34:	a001                	j	34 <main+0x34>
    printf("ppid:%d",getppid());
  36:	00000097          	auipc	ra,0x0
  3a:	396080e7          	jalr	918(ra) # 3cc <getppid>
  3e:	85aa                	mv	a1,a0
  40:	00001517          	auipc	a0,0x1
  44:	81850513          	addi	a0,a0,-2024 # 858 <malloc+0xea>
  48:	00000097          	auipc	ra,0x0
  4c:	666080e7          	jalr	1638(ra) # 6ae <printf>
  50:	b7d5                	j	34 <main+0x34>

0000000000000052 <strcpy>:
#include "kernel/include/fcntl.h"
#include "xv6-user/user.h"

char*
strcpy(char *s, const char *t)
{
  52:	1141                	addi	sp,sp,-16
  54:	e422                	sd	s0,8(sp)
  56:	0800                	addi	s0,sp,16
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
  58:	87aa                	mv	a5,a0
  5a:	0585                	addi	a1,a1,1
  5c:	0785                	addi	a5,a5,1
  5e:	fff5c703          	lbu	a4,-1(a1)
  62:	fee78fa3          	sb	a4,-1(a5)
  66:	fb75                	bnez	a4,5a <strcpy+0x8>
    ;
  return os;
}
  68:	6422                	ld	s0,8(sp)
  6a:	0141                	addi	sp,sp,16
  6c:	8082                	ret

000000000000006e <strcat>:

char*
strcat(char *s, const char *t)
{
  6e:	1141                	addi	sp,sp,-16
  70:	e422                	sd	s0,8(sp)
  72:	0800                	addi	s0,sp,16
  char *os = s;
  while(*s)
  74:	00054783          	lbu	a5,0(a0)
  78:	c385                	beqz	a5,98 <strcat+0x2a>
  7a:	87aa                	mv	a5,a0
    s++;
  7c:	0785                	addi	a5,a5,1
  while(*s)
  7e:	0007c703          	lbu	a4,0(a5)
  82:	ff6d                	bnez	a4,7c <strcat+0xe>
  while((*s++ = *t++))
  84:	0585                	addi	a1,a1,1
  86:	0785                	addi	a5,a5,1
  88:	fff5c703          	lbu	a4,-1(a1)
  8c:	fee78fa3          	sb	a4,-1(a5)
  90:	fb75                	bnez	a4,84 <strcat+0x16>
    ;
  return os;
}
  92:	6422                	ld	s0,8(sp)
  94:	0141                	addi	sp,sp,16
  96:	8082                	ret
  while(*s)
  98:	87aa                	mv	a5,a0
  9a:	b7ed                	j	84 <strcat+0x16>

000000000000009c <strcmp>:


int
strcmp(const char *p, const char *q)
{
  9c:	1141                	addi	sp,sp,-16
  9e:	e422                	sd	s0,8(sp)
  a0:	0800                	addi	s0,sp,16
  while(*p && *p == *q)
  a2:	00054783          	lbu	a5,0(a0)
  a6:	cf91                	beqz	a5,c2 <strcmp+0x26>
  a8:	0005c703          	lbu	a4,0(a1)
  ac:	00f71b63          	bne	a4,a5,c2 <strcmp+0x26>
    p++, q++;
  b0:	0505                	addi	a0,a0,1
  b2:	0585                	addi	a1,a1,1
  while(*p && *p == *q)
  b4:	00054783          	lbu	a5,0(a0)
  b8:	c789                	beqz	a5,c2 <strcmp+0x26>
  ba:	0005c703          	lbu	a4,0(a1)
  be:	fef709e3          	beq	a4,a5,b0 <strcmp+0x14>
  return (uchar)*p - (uchar)*q;
  c2:	0005c503          	lbu	a0,0(a1)
}
  c6:	40a7853b          	subw	a0,a5,a0
  ca:	6422                	ld	s0,8(sp)
  cc:	0141                	addi	sp,sp,16
  ce:	8082                	ret

00000000000000d0 <strlen>:

uint
strlen(const char *s)
{
  d0:	1141                	addi	sp,sp,-16
  d2:	e422                	sd	s0,8(sp)
  d4:	0800                	addi	s0,sp,16
  int n;

  for(n = 0; s[n]; n++)
  d6:	00054783          	lbu	a5,0(a0)
  da:	cf91                	beqz	a5,f6 <strlen+0x26>
  dc:	0505                	addi	a0,a0,1
  de:	87aa                	mv	a5,a0
  e0:	4685                	li	a3,1
  e2:	9e89                	subw	a3,a3,a0
    ;
  e4:	00f6853b          	addw	a0,a3,a5
  e8:	0785                	addi	a5,a5,1
  for(n = 0; s[n]; n++)
  ea:	fff7c703          	lbu	a4,-1(a5)
  ee:	fb7d                	bnez	a4,e4 <strlen+0x14>
  return n;
}
  f0:	6422                	ld	s0,8(sp)
  f2:	0141                	addi	sp,sp,16
  f4:	8082                	ret
  for(n = 0; s[n]; n++)
  f6:	4501                	li	a0,0
  f8:	bfe5                	j	f0 <strlen+0x20>

00000000000000fa <memset>:

void*
memset(void *dst, int c, uint n)
{
  fa:	1141                	addi	sp,sp,-16
  fc:	e422                	sd	s0,8(sp)
  fe:	0800                	addi	s0,sp,16
  char *cdst = (char *) dst;
  int i;
  for(i = 0; i < n; i++){
 100:	ce09                	beqz	a2,11a <memset+0x20>
 102:	87aa                	mv	a5,a0
 104:	fff6071b          	addiw	a4,a2,-1
 108:	1702                	slli	a4,a4,0x20
 10a:	9301                	srli	a4,a4,0x20
 10c:	0705                	addi	a4,a4,1
 10e:	972a                	add	a4,a4,a0
    cdst[i] = c;
 110:	00b78023          	sb	a1,0(a5)
 114:	0785                	addi	a5,a5,1
  for(i = 0; i < n; i++){
 116:	fee79de3          	bne	a5,a4,110 <memset+0x16>
  }
  return dst;
}
 11a:	6422                	ld	s0,8(sp)
 11c:	0141                	addi	sp,sp,16
 11e:	8082                	ret

0000000000000120 <strchr>:

char*
strchr(const char *s, char c)
{
 120:	1141                	addi	sp,sp,-16
 122:	e422                	sd	s0,8(sp)
 124:	0800                	addi	s0,sp,16
  for(; *s; s++)
 126:	00054783          	lbu	a5,0(a0)
 12a:	cf91                	beqz	a5,146 <strchr+0x26>
    if(*s == c)
 12c:	00f58a63          	beq	a1,a5,140 <strchr+0x20>
  for(; *s; s++)
 130:	0505                	addi	a0,a0,1
 132:	00054783          	lbu	a5,0(a0)
 136:	c781                	beqz	a5,13e <strchr+0x1e>
    if(*s == c)
 138:	feb79ce3          	bne	a5,a1,130 <strchr+0x10>
 13c:	a011                	j	140 <strchr+0x20>
      return (char*)s;
  return 0;
 13e:	4501                	li	a0,0
}
 140:	6422                	ld	s0,8(sp)
 142:	0141                	addi	sp,sp,16
 144:	8082                	ret
  return 0;
 146:	4501                	li	a0,0
 148:	bfe5                	j	140 <strchr+0x20>

000000000000014a <gets>:

char*
gets(char *buf, int max)
{
 14a:	711d                	addi	sp,sp,-96
 14c:	ec86                	sd	ra,88(sp)
 14e:	e8a2                	sd	s0,80(sp)
 150:	e4a6                	sd	s1,72(sp)
 152:	e0ca                	sd	s2,64(sp)
 154:	fc4e                	sd	s3,56(sp)
 156:	f852                	sd	s4,48(sp)
 158:	f456                	sd	s5,40(sp)
 15a:	f05a                	sd	s6,32(sp)
 15c:	ec5e                	sd	s7,24(sp)
 15e:	1080                	addi	s0,sp,96
 160:	8baa                	mv	s7,a0
 162:	8a2e                	mv	s4,a1
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
 164:	892a                	mv	s2,a0
 166:	4981                	li	s3,0
    cc = read(0, &c, 1);
    if(cc < 1)
      break;
    buf[i++] = c;
    if(c == '\n' || c == '\r')
 168:	4aa9                	li	s5,10
 16a:	4b35                	li	s6,13
  for(i=0; i+1 < max; ){
 16c:	0019849b          	addiw	s1,s3,1
 170:	0344d863          	ble	s4,s1,1a0 <gets+0x56>
    cc = read(0, &c, 1);
 174:	4605                	li	a2,1
 176:	faf40593          	addi	a1,s0,-81
 17a:	4501                	li	a0,0
 17c:	00000097          	auipc	ra,0x0
 180:	1ca080e7          	jalr	458(ra) # 346 <read>
    if(cc < 1)
 184:	00a05e63          	blez	a0,1a0 <gets+0x56>
    buf[i++] = c;
 188:	faf44783          	lbu	a5,-81(s0)
 18c:	00f90023          	sb	a5,0(s2)
    if(c == '\n' || c == '\r')
 190:	01578763          	beq	a5,s5,19e <gets+0x54>
 194:	0905                	addi	s2,s2,1
  for(i=0; i+1 < max; ){
 196:	89a6                	mv	s3,s1
    if(c == '\n' || c == '\r')
 198:	fd679ae3          	bne	a5,s6,16c <gets+0x22>
 19c:	a011                	j	1a0 <gets+0x56>
  for(i=0; i+1 < max; ){
 19e:	89a6                	mv	s3,s1
      break;
  }
  buf[i] = '\0';
 1a0:	99de                	add	s3,s3,s7
 1a2:	00098023          	sb	zero,0(s3)
  return buf;
}
 1a6:	855e                	mv	a0,s7
 1a8:	60e6                	ld	ra,88(sp)
 1aa:	6446                	ld	s0,80(sp)
 1ac:	64a6                	ld	s1,72(sp)
 1ae:	6906                	ld	s2,64(sp)
 1b0:	79e2                	ld	s3,56(sp)
 1b2:	7a42                	ld	s4,48(sp)
 1b4:	7aa2                	ld	s5,40(sp)
 1b6:	7b02                	ld	s6,32(sp)
 1b8:	6be2                	ld	s7,24(sp)
 1ba:	6125                	addi	sp,sp,96
 1bc:	8082                	ret

00000000000001be <stat>:

int
stat(const char *n, struct stat *st)
{
 1be:	1101                	addi	sp,sp,-32
 1c0:	ec06                	sd	ra,24(sp)
 1c2:	e822                	sd	s0,16(sp)
 1c4:	e426                	sd	s1,8(sp)
 1c6:	e04a                	sd	s2,0(sp)
 1c8:	1000                	addi	s0,sp,32
 1ca:	892e                	mv	s2,a1
  int fd;
  int r;

  fd = openat(n, O_RDONLY);
 1cc:	4581                	li	a1,0
 1ce:	00000097          	auipc	ra,0x0
 1d2:	1a0080e7          	jalr	416(ra) # 36e <openat>
  if(fd < 0)
 1d6:	02054563          	bltz	a0,200 <stat+0x42>
 1da:	84aa                	mv	s1,a0
    return -1;
  r = fstat(fd, st);
 1dc:	85ca                	mv	a1,s2
 1de:	00000097          	auipc	ra,0x0
 1e2:	19a080e7          	jalr	410(ra) # 378 <fstat>
 1e6:	892a                	mv	s2,a0
  close(fd);
 1e8:	8526                	mv	a0,s1
 1ea:	00000097          	auipc	ra,0x0
 1ee:	170080e7          	jalr	368(ra) # 35a <close>
  return r;
}
 1f2:	854a                	mv	a0,s2
 1f4:	60e2                	ld	ra,24(sp)
 1f6:	6442                	ld	s0,16(sp)
 1f8:	64a2                	ld	s1,8(sp)
 1fa:	6902                	ld	s2,0(sp)
 1fc:	6105                	addi	sp,sp,32
 1fe:	8082                	ret
    return -1;
 200:	597d                	li	s2,-1
 202:	bfc5                	j	1f2 <stat+0x34>

0000000000000204 <atoi>:

int
atoi(const char *s)
{
 204:	1141                	addi	sp,sp,-16
 206:	e422                	sd	s0,8(sp)
 208:	0800                	addi	s0,sp,16
  int n;
  int neg = 1;
  if (*s == '-') {
 20a:	00054703          	lbu	a4,0(a0)
 20e:	02d00793          	li	a5,45
  int neg = 1;
 212:	4805                	li	a6,1
  if (*s == '-') {
 214:	04f70363          	beq	a4,a5,25a <atoi+0x56>
    s++;
    neg = -1;
  }
  n = 0;
  while('0' <= *s && *s <= '9')
 218:	00054683          	lbu	a3,0(a0)
 21c:	fd06879b          	addiw	a5,a3,-48
 220:	0ff7f793          	andi	a5,a5,255
 224:	4725                	li	a4,9
 226:	02f76d63          	bltu	a4,a5,260 <atoi+0x5c>
  n = 0;
 22a:	4601                	li	a2,0
  while('0' <= *s && *s <= '9')
 22c:	45a5                	li	a1,9
    n = n*10 + *s++ - '0';
 22e:	0505                	addi	a0,a0,1
 230:	0026179b          	slliw	a5,a2,0x2
 234:	9fb1                	addw	a5,a5,a2
 236:	0017979b          	slliw	a5,a5,0x1
 23a:	9fb5                	addw	a5,a5,a3
 23c:	fd07861b          	addiw	a2,a5,-48
  while('0' <= *s && *s <= '9')
 240:	00054683          	lbu	a3,0(a0)
 244:	fd06871b          	addiw	a4,a3,-48
 248:	0ff77713          	andi	a4,a4,255
 24c:	fee5f1e3          	bleu	a4,a1,22e <atoi+0x2a>
  return n * neg;
}
 250:	02c8053b          	mulw	a0,a6,a2
 254:	6422                	ld	s0,8(sp)
 256:	0141                	addi	sp,sp,16
 258:	8082                	ret
    s++;
 25a:	0505                	addi	a0,a0,1
    neg = -1;
 25c:	587d                	li	a6,-1
 25e:	bf6d                	j	218 <atoi+0x14>
  n = 0;
 260:	4601                	li	a2,0
 262:	b7fd                	j	250 <atoi+0x4c>

0000000000000264 <memmove>:

void*
memmove(void *vdst, const void *vsrc, int n)
{
 264:	1141                	addi	sp,sp,-16
 266:	e422                	sd	s0,8(sp)
 268:	0800                	addi	s0,sp,16
  char *dst;
  const char *src;

  dst = vdst;
  src = vsrc;
  if (src > dst) {
 26a:	02b57663          	bleu	a1,a0,296 <memmove+0x32>
    while(n-- > 0)
 26e:	02c05163          	blez	a2,290 <memmove+0x2c>
 272:	fff6079b          	addiw	a5,a2,-1
 276:	1782                	slli	a5,a5,0x20
 278:	9381                	srli	a5,a5,0x20
 27a:	0785                	addi	a5,a5,1
 27c:	97aa                	add	a5,a5,a0
  dst = vdst;
 27e:	872a                	mv	a4,a0
      *dst++ = *src++;
 280:	0585                	addi	a1,a1,1
 282:	0705                	addi	a4,a4,1
 284:	fff5c683          	lbu	a3,-1(a1)
 288:	fed70fa3          	sb	a3,-1(a4)
    while(n-- > 0)
 28c:	fee79ae3          	bne	a5,a4,280 <memmove+0x1c>
    src += n;
    while(n-- > 0)
      *--dst = *--src;
  }
  return vdst;
}
 290:	6422                	ld	s0,8(sp)
 292:	0141                	addi	sp,sp,16
 294:	8082                	ret
    dst += n;
 296:	00c50733          	add	a4,a0,a2
    src += n;
 29a:	95b2                	add	a1,a1,a2
    while(n-- > 0)
 29c:	fec05ae3          	blez	a2,290 <memmove+0x2c>
 2a0:	fff6079b          	addiw	a5,a2,-1
 2a4:	1782                	slli	a5,a5,0x20
 2a6:	9381                	srli	a5,a5,0x20
 2a8:	fff7c793          	not	a5,a5
 2ac:	97ba                	add	a5,a5,a4
      *--dst = *--src;
 2ae:	15fd                	addi	a1,a1,-1
 2b0:	177d                	addi	a4,a4,-1
 2b2:	0005c683          	lbu	a3,0(a1)
 2b6:	00d70023          	sb	a3,0(a4)
    while(n-- > 0)
 2ba:	fef71ae3          	bne	a4,a5,2ae <memmove+0x4a>
 2be:	bfc9                	j	290 <memmove+0x2c>

00000000000002c0 <memcmp>:

int
memcmp(const void *s1, const void *s2, uint n)
{
 2c0:	1141                	addi	sp,sp,-16
 2c2:	e422                	sd	s0,8(sp)
 2c4:	0800                	addi	s0,sp,16
  const char *p1 = s1, *p2 = s2;
  while (n-- > 0) {
 2c6:	ce15                	beqz	a2,302 <memcmp+0x42>
 2c8:	fff6069b          	addiw	a3,a2,-1
    if (*p1 != *p2) {
 2cc:	00054783          	lbu	a5,0(a0)
 2d0:	0005c703          	lbu	a4,0(a1)
 2d4:	02e79063          	bne	a5,a4,2f4 <memcmp+0x34>
 2d8:	1682                	slli	a3,a3,0x20
 2da:	9281                	srli	a3,a3,0x20
 2dc:	0685                	addi	a3,a3,1
 2de:	96aa                	add	a3,a3,a0
      return *p1 - *p2;
    }
    p1++;
 2e0:	0505                	addi	a0,a0,1
    p2++;
 2e2:	0585                	addi	a1,a1,1
  while (n-- > 0) {
 2e4:	00d50d63          	beq	a0,a3,2fe <memcmp+0x3e>
    if (*p1 != *p2) {
 2e8:	00054783          	lbu	a5,0(a0)
 2ec:	0005c703          	lbu	a4,0(a1)
 2f0:	fee788e3          	beq	a5,a4,2e0 <memcmp+0x20>
      return *p1 - *p2;
 2f4:	40e7853b          	subw	a0,a5,a4
  }
  return 0;
}
 2f8:	6422                	ld	s0,8(sp)
 2fa:	0141                	addi	sp,sp,16
 2fc:	8082                	ret
  return 0;
 2fe:	4501                	li	a0,0
 300:	bfe5                	j	2f8 <memcmp+0x38>
 302:	4501                	li	a0,0
 304:	bfd5                	j	2f8 <memcmp+0x38>

0000000000000306 <memcpy>:

void *
memcpy(void *dst, const void *src, uint n)
{
 306:	1141                	addi	sp,sp,-16
 308:	e406                	sd	ra,8(sp)
 30a:	e022                	sd	s0,0(sp)
 30c:	0800                	addi	s0,sp,16
  return memmove(dst, src, n);
 30e:	00000097          	auipc	ra,0x0
 312:	f56080e7          	jalr	-170(ra) # 264 <memmove>
}
 316:	60a2                	ld	ra,8(sp)
 318:	6402                	ld	s0,0(sp)
 31a:	0141                	addi	sp,sp,16
 31c:	8082                	ret

000000000000031e <clone>:
# generated by usys.pl - do not edit
#include "kernel/include/sysnum.h"
.global clone
clone:
 li a7, SYS_clone
 31e:	0dc00893          	li	a7,220
 ecall
 322:	00000073          	ecall
 ret
 326:	8082                	ret

0000000000000328 <exit>:
.global exit
exit:
 li a7, SYS_exit
 328:	05d00893          	li	a7,93
 ecall
 32c:	00000073          	ecall
 ret
 330:	8082                	ret

0000000000000332 <wait4>:
.global wait4
wait4:
 li a7, SYS_wait4
 332:	10400893          	li	a7,260
 ecall
 336:	00000073          	ecall
 ret
 33a:	8082                	ret

000000000000033c <pipe2>:
.global pipe2
pipe2:
 li a7, SYS_pipe2
 33c:	03b00893          	li	a7,59
 ecall
 340:	00000073          	ecall
 ret
 344:	8082                	ret

0000000000000346 <read>:
.global read
read:
 li a7, SYS_read
 346:	03f00893          	li	a7,63
 ecall
 34a:	00000073          	ecall
 ret
 34e:	8082                	ret

0000000000000350 <write>:
.global write
write:
 li a7, SYS_write
 350:	04000893          	li	a7,64
 ecall
 354:	00000073          	ecall
 ret
 358:	8082                	ret

000000000000035a <close>:
.global close
close:
 li a7, SYS_close
 35a:	03900893          	li	a7,57
 ecall
 35e:	00000073          	ecall
 ret
 362:	8082                	ret

0000000000000364 <execve>:
.global execve
execve:
 li a7, SYS_execve
 364:	0dd00893          	li	a7,221
 ecall
 368:	00000073          	ecall
 ret
 36c:	8082                	ret

000000000000036e <openat>:
.global openat
openat:
 li a7, SYS_openat
 36e:	03800893          	li	a7,56
 ecall
 372:	00000073          	ecall
 ret
 376:	8082                	ret

0000000000000378 <fstat>:
.global fstat
fstat:
 li a7, SYS_fstat
 378:	05000893          	li	a7,80
 ecall
 37c:	00000073          	ecall
 ret
 380:	8082                	ret

0000000000000382 <mkdirat>:
.global mkdirat
mkdirat:
 li a7, SYS_mkdirat
 382:	02200893          	li	a7,34
 ecall
 386:	00000073          	ecall
 ret
 38a:	8082                	ret

000000000000038c <chdir>:
.global chdir
chdir:
 li a7, SYS_chdir
 38c:	03100893          	li	a7,49
 ecall
 390:	00000073          	ecall
 ret
 394:	8082                	ret

0000000000000396 <dup>:
.global dup
dup:
 li a7, SYS_dup
 396:	48dd                	li	a7,23
 ecall
 398:	00000073          	ecall
 ret
 39c:	8082                	ret

000000000000039e <getpid>:
.global getpid
getpid:
 li a7, SYS_getpid
 39e:	0ac00893          	li	a7,172
 ecall
 3a2:	00000073          	ecall
 ret
 3a6:	8082                	ret

00000000000003a8 <brk>:
.global brk
brk:
 li a7, SYS_brk
 3a8:	0d600893          	li	a7,214
 ecall
 3ac:	00000073          	ecall
 ret
 3b0:	8082                	ret

00000000000003b2 <nanosleep>:
.global nanosleep
nanosleep:
 li a7, SYS_nanosleep
 3b2:	06500893          	li	a7,101
 ecall
 3b6:	00000073          	ecall
 ret
 3ba:	8082                	ret

00000000000003bc <dev>:
.global dev
dev:
 li a7, SYS_dev
 3bc:	4885                	li	a7,1
 ecall
 3be:	00000073          	ecall
 ret
 3c2:	8082                	ret

00000000000003c4 <getcwd>:
.global getcwd
getcwd:
 li a7, SYS_getcwd
 3c4:	48c5                	li	a7,17
 ecall
 3c6:	00000073          	ecall
 ret
 3ca:	8082                	ret

00000000000003cc <getppid>:
.global getppid
getppid:
 li a7, SYS_getppid
 3cc:	0ad00893          	li	a7,173
 ecall
 3d0:	00000073          	ecall
 ret
 3d4:	8082                	ret

00000000000003d6 <putc>:

static char digits[] = "0123456789ABCDEF";

static void
putc(int fd, char c)
{
 3d6:	1101                	addi	sp,sp,-32
 3d8:	ec06                	sd	ra,24(sp)
 3da:	e822                	sd	s0,16(sp)
 3dc:	1000                	addi	s0,sp,32
 3de:	feb407a3          	sb	a1,-17(s0)
  write(fd, &c, 1);
 3e2:	4605                	li	a2,1
 3e4:	fef40593          	addi	a1,s0,-17
 3e8:	00000097          	auipc	ra,0x0
 3ec:	f68080e7          	jalr	-152(ra) # 350 <write>
}
 3f0:	60e2                	ld	ra,24(sp)
 3f2:	6442                	ld	s0,16(sp)
 3f4:	6105                	addi	sp,sp,32
 3f6:	8082                	ret

00000000000003f8 <printint>:

static void
printint(int fd, int xx, int base, int sgn)
{
 3f8:	7139                	addi	sp,sp,-64
 3fa:	fc06                	sd	ra,56(sp)
 3fc:	f822                	sd	s0,48(sp)
 3fe:	f426                	sd	s1,40(sp)
 400:	f04a                	sd	s2,32(sp)
 402:	ec4e                	sd	s3,24(sp)
 404:	0080                	addi	s0,sp,64
  char buf[16];
  int i, neg;
  uint x;

  neg = 0;
  if(sgn && xx < 0){
 406:	c299                	beqz	a3,40c <printint+0x14>
 408:	0005cd63          	bltz	a1,422 <printint+0x2a>
    neg = 1;
    x = -xx;
  } else {
    x = xx;
 40c:	2581                	sext.w	a1,a1
  neg = 0;
 40e:	4301                	li	t1,0
 410:	fc040713          	addi	a4,s0,-64
  }

  i = 0;
 414:	4801                	li	a6,0
  do{
    buf[i++] = digits[x % base];
 416:	2601                	sext.w	a2,a2
 418:	00000897          	auipc	a7,0x0
 41c:	44888893          	addi	a7,a7,1096 # 860 <digits>
 420:	a801                	j	430 <printint+0x38>
    x = -xx;
 422:	40b005bb          	negw	a1,a1
 426:	2581                	sext.w	a1,a1
    neg = 1;
 428:	4305                	li	t1,1
    x = -xx;
 42a:	b7dd                	j	410 <printint+0x18>
  }while((x /= base) != 0);
 42c:	85be                	mv	a1,a5
    buf[i++] = digits[x % base];
 42e:	8836                	mv	a6,a3
 430:	0018069b          	addiw	a3,a6,1
 434:	02c5f7bb          	remuw	a5,a1,a2
 438:	1782                	slli	a5,a5,0x20
 43a:	9381                	srli	a5,a5,0x20
 43c:	97c6                	add	a5,a5,a7
 43e:	0007c783          	lbu	a5,0(a5)
 442:	00f70023          	sb	a5,0(a4)
 446:	0705                	addi	a4,a4,1
  }while((x /= base) != 0);
 448:	02c5d7bb          	divuw	a5,a1,a2
 44c:	fec5f0e3          	bleu	a2,a1,42c <printint+0x34>
  if(neg)
 450:	00030b63          	beqz	t1,466 <printint+0x6e>
    buf[i++] = '-';
 454:	fd040793          	addi	a5,s0,-48
 458:	96be                	add	a3,a3,a5
 45a:	02d00793          	li	a5,45
 45e:	fef68823          	sb	a5,-16(a3)
 462:	0028069b          	addiw	a3,a6,2

  while(--i >= 0)
 466:	02d05963          	blez	a3,498 <printint+0xa0>
 46a:	89aa                	mv	s3,a0
 46c:	fc040793          	addi	a5,s0,-64
 470:	00d784b3          	add	s1,a5,a3
 474:	fff78913          	addi	s2,a5,-1
 478:	9936                	add	s2,s2,a3
 47a:	36fd                	addiw	a3,a3,-1
 47c:	1682                	slli	a3,a3,0x20
 47e:	9281                	srli	a3,a3,0x20
 480:	40d90933          	sub	s2,s2,a3
    putc(fd, buf[i]);
 484:	fff4c583          	lbu	a1,-1(s1)
 488:	854e                	mv	a0,s3
 48a:	00000097          	auipc	ra,0x0
 48e:	f4c080e7          	jalr	-180(ra) # 3d6 <putc>
 492:	14fd                	addi	s1,s1,-1
  while(--i >= 0)
 494:	ff2498e3          	bne	s1,s2,484 <printint+0x8c>
}
 498:	70e2                	ld	ra,56(sp)
 49a:	7442                	ld	s0,48(sp)
 49c:	74a2                	ld	s1,40(sp)
 49e:	7902                	ld	s2,32(sp)
 4a0:	69e2                	ld	s3,24(sp)
 4a2:	6121                	addi	sp,sp,64
 4a4:	8082                	ret

00000000000004a6 <vprintf>:
}

// Print to the given fd. Only understands %d, %x, %p, %s.
void
vprintf(int fd, const char *fmt, va_list ap)
{
 4a6:	7119                	addi	sp,sp,-128
 4a8:	fc86                	sd	ra,120(sp)
 4aa:	f8a2                	sd	s0,112(sp)
 4ac:	f4a6                	sd	s1,104(sp)
 4ae:	f0ca                	sd	s2,96(sp)
 4b0:	ecce                	sd	s3,88(sp)
 4b2:	e8d2                	sd	s4,80(sp)
 4b4:	e4d6                	sd	s5,72(sp)
 4b6:	e0da                	sd	s6,64(sp)
 4b8:	fc5e                	sd	s7,56(sp)
 4ba:	f862                	sd	s8,48(sp)
 4bc:	f466                	sd	s9,40(sp)
 4be:	f06a                	sd	s10,32(sp)
 4c0:	ec6e                	sd	s11,24(sp)
 4c2:	0100                	addi	s0,sp,128
  char *s;
  int c, i, state;

  state = 0;
  for(i = 0; fmt[i]; i++){
 4c4:	0005c483          	lbu	s1,0(a1)
 4c8:	18048d63          	beqz	s1,662 <vprintf+0x1bc>
 4cc:	8aaa                	mv	s5,a0
 4ce:	8b32                	mv	s6,a2
 4d0:	00158913          	addi	s2,a1,1
  state = 0;
 4d4:	4981                	li	s3,0
      if(c == '%'){
        state = '%';
      } else {
        putc(fd, c);
      }
    } else if(state == '%'){
 4d6:	02500a13          	li	s4,37
      if(c == 'd'){
 4da:	06400c13          	li	s8,100
        printint(fd, va_arg(ap, int), 10, 1);
      } else if(c == 'l') {
 4de:	06c00c93          	li	s9,108
        printint(fd, va_arg(ap, uint64), 10, 0);
      } else if(c == 'x') {
 4e2:	07800d13          	li	s10,120
        printint(fd, va_arg(ap, int), 16, 0);
      } else if(c == 'p') {
 4e6:	07000d93          	li	s11,112
    putc(fd, digits[x >> (sizeof(uint64) * 8 - 4)]);
 4ea:	00000b97          	auipc	s7,0x0
 4ee:	376b8b93          	addi	s7,s7,886 # 860 <digits>
 4f2:	a839                	j	510 <vprintf+0x6a>
        putc(fd, c);
 4f4:	85a6                	mv	a1,s1
 4f6:	8556                	mv	a0,s5
 4f8:	00000097          	auipc	ra,0x0
 4fc:	ede080e7          	jalr	-290(ra) # 3d6 <putc>
 500:	a019                	j	506 <vprintf+0x60>
    } else if(state == '%'){
 502:	01498f63          	beq	s3,s4,520 <vprintf+0x7a>
 506:	0905                	addi	s2,s2,1
  for(i = 0; fmt[i]; i++){
 508:	fff94483          	lbu	s1,-1(s2)
 50c:	14048b63          	beqz	s1,662 <vprintf+0x1bc>
    c = fmt[i] & 0xff;
 510:	0004879b          	sext.w	a5,s1
    if(state == 0){
 514:	fe0997e3          	bnez	s3,502 <vprintf+0x5c>
      if(c == '%'){
 518:	fd479ee3          	bne	a5,s4,4f4 <vprintf+0x4e>
        state = '%';
 51c:	89be                	mv	s3,a5
 51e:	b7e5                	j	506 <vprintf+0x60>
      if(c == 'd'){
 520:	05878063          	beq	a5,s8,560 <vprintf+0xba>
      } else if(c == 'l') {
 524:	05978c63          	beq	a5,s9,57c <vprintf+0xd6>
      } else if(c == 'x') {
 528:	07a78863          	beq	a5,s10,598 <vprintf+0xf2>
      } else if(c == 'p') {
 52c:	09b78463          	beq	a5,s11,5b4 <vprintf+0x10e>
        printptr(fd, va_arg(ap, uint64));
      } else if(c == 's'){
 530:	07300713          	li	a4,115
 534:	0ce78563          	beq	a5,a4,5fe <vprintf+0x158>
          s = "(null)";
        while(*s != 0){
          putc(fd, *s);
          s++;
        }
      } else if(c == 'c'){
 538:	06300713          	li	a4,99
 53c:	0ee78c63          	beq	a5,a4,634 <vprintf+0x18e>
        putc(fd, va_arg(ap, uint));
      } else if(c == '%'){
 540:	11478663          	beq	a5,s4,64c <vprintf+0x1a6>
        putc(fd, c);
      } else {
        // Unknown % sequence.  Print it to draw attention.
        putc(fd, '%');
 544:	85d2                	mv	a1,s4
 546:	8556                	mv	a0,s5
 548:	00000097          	auipc	ra,0x0
 54c:	e8e080e7          	jalr	-370(ra) # 3d6 <putc>
        putc(fd, c);
 550:	85a6                	mv	a1,s1
 552:	8556                	mv	a0,s5
 554:	00000097          	auipc	ra,0x0
 558:	e82080e7          	jalr	-382(ra) # 3d6 <putc>
      }
      state = 0;
 55c:	4981                	li	s3,0
 55e:	b765                	j	506 <vprintf+0x60>
        printint(fd, va_arg(ap, int), 10, 1);
 560:	008b0493          	addi	s1,s6,8
 564:	4685                	li	a3,1
 566:	4629                	li	a2,10
 568:	000b2583          	lw	a1,0(s6)
 56c:	8556                	mv	a0,s5
 56e:	00000097          	auipc	ra,0x0
 572:	e8a080e7          	jalr	-374(ra) # 3f8 <printint>
 576:	8b26                	mv	s6,s1
      state = 0;
 578:	4981                	li	s3,0
 57a:	b771                	j	506 <vprintf+0x60>
        printint(fd, va_arg(ap, uint64), 10, 0);
 57c:	008b0493          	addi	s1,s6,8
 580:	4681                	li	a3,0
 582:	4629                	li	a2,10
 584:	000b2583          	lw	a1,0(s6)
 588:	8556                	mv	a0,s5
 58a:	00000097          	auipc	ra,0x0
 58e:	e6e080e7          	jalr	-402(ra) # 3f8 <printint>
 592:	8b26                	mv	s6,s1
      state = 0;
 594:	4981                	li	s3,0
 596:	bf85                	j	506 <vprintf+0x60>
        printint(fd, va_arg(ap, int), 16, 0);
 598:	008b0493          	addi	s1,s6,8
 59c:	4681                	li	a3,0
 59e:	4641                	li	a2,16
 5a0:	000b2583          	lw	a1,0(s6)
 5a4:	8556                	mv	a0,s5
 5a6:	00000097          	auipc	ra,0x0
 5aa:	e52080e7          	jalr	-430(ra) # 3f8 <printint>
 5ae:	8b26                	mv	s6,s1
      state = 0;
 5b0:	4981                	li	s3,0
 5b2:	bf91                	j	506 <vprintf+0x60>
        printptr(fd, va_arg(ap, uint64));
 5b4:	008b0793          	addi	a5,s6,8
 5b8:	f8f43423          	sd	a5,-120(s0)
 5bc:	000b3983          	ld	s3,0(s6)
  putc(fd, '0');
 5c0:	03000593          	li	a1,48
 5c4:	8556                	mv	a0,s5
 5c6:	00000097          	auipc	ra,0x0
 5ca:	e10080e7          	jalr	-496(ra) # 3d6 <putc>
  putc(fd, 'x');
 5ce:	85ea                	mv	a1,s10
 5d0:	8556                	mv	a0,s5
 5d2:	00000097          	auipc	ra,0x0
 5d6:	e04080e7          	jalr	-508(ra) # 3d6 <putc>
 5da:	44c1                	li	s1,16
    putc(fd, digits[x >> (sizeof(uint64) * 8 - 4)]);
 5dc:	03c9d793          	srli	a5,s3,0x3c
 5e0:	97de                	add	a5,a5,s7
 5e2:	0007c583          	lbu	a1,0(a5)
 5e6:	8556                	mv	a0,s5
 5e8:	00000097          	auipc	ra,0x0
 5ec:	dee080e7          	jalr	-530(ra) # 3d6 <putc>
  for (i = 0; i < (sizeof(uint64) * 2); i++, x <<= 4)
 5f0:	0992                	slli	s3,s3,0x4
 5f2:	34fd                	addiw	s1,s1,-1
 5f4:	f4e5                	bnez	s1,5dc <vprintf+0x136>
        printptr(fd, va_arg(ap, uint64));
 5f6:	f8843b03          	ld	s6,-120(s0)
      state = 0;
 5fa:	4981                	li	s3,0
 5fc:	b729                	j	506 <vprintf+0x60>
        s = va_arg(ap, char*);
 5fe:	008b0993          	addi	s3,s6,8
 602:	000b3483          	ld	s1,0(s6)
        if(s == 0)
 606:	c085                	beqz	s1,626 <vprintf+0x180>
        while(*s != 0){
 608:	0004c583          	lbu	a1,0(s1)
 60c:	c9a1                	beqz	a1,65c <vprintf+0x1b6>
          putc(fd, *s);
 60e:	8556                	mv	a0,s5
 610:	00000097          	auipc	ra,0x0
 614:	dc6080e7          	jalr	-570(ra) # 3d6 <putc>
          s++;
 618:	0485                	addi	s1,s1,1
        while(*s != 0){
 61a:	0004c583          	lbu	a1,0(s1)
 61e:	f9e5                	bnez	a1,60e <vprintf+0x168>
        s = va_arg(ap, char*);
 620:	8b4e                	mv	s6,s3
      state = 0;
 622:	4981                	li	s3,0
 624:	b5cd                	j	506 <vprintf+0x60>
          s = "(null)";
 626:	00000497          	auipc	s1,0x0
 62a:	25248493          	addi	s1,s1,594 # 878 <digits+0x18>
        while(*s != 0){
 62e:	02800593          	li	a1,40
 632:	bff1                	j	60e <vprintf+0x168>
        putc(fd, va_arg(ap, uint));
 634:	008b0493          	addi	s1,s6,8
 638:	000b4583          	lbu	a1,0(s6)
 63c:	8556                	mv	a0,s5
 63e:	00000097          	auipc	ra,0x0
 642:	d98080e7          	jalr	-616(ra) # 3d6 <putc>
 646:	8b26                	mv	s6,s1
      state = 0;
 648:	4981                	li	s3,0
 64a:	bd75                	j	506 <vprintf+0x60>
        putc(fd, c);
 64c:	85d2                	mv	a1,s4
 64e:	8556                	mv	a0,s5
 650:	00000097          	auipc	ra,0x0
 654:	d86080e7          	jalr	-634(ra) # 3d6 <putc>
      state = 0;
 658:	4981                	li	s3,0
 65a:	b575                	j	506 <vprintf+0x60>
        s = va_arg(ap, char*);
 65c:	8b4e                	mv	s6,s3
      state = 0;
 65e:	4981                	li	s3,0
 660:	b55d                	j	506 <vprintf+0x60>
    }
  }
}
 662:	70e6                	ld	ra,120(sp)
 664:	7446                	ld	s0,112(sp)
 666:	74a6                	ld	s1,104(sp)
 668:	7906                	ld	s2,96(sp)
 66a:	69e6                	ld	s3,88(sp)
 66c:	6a46                	ld	s4,80(sp)
 66e:	6aa6                	ld	s5,72(sp)
 670:	6b06                	ld	s6,64(sp)
 672:	7be2                	ld	s7,56(sp)
 674:	7c42                	ld	s8,48(sp)
 676:	7ca2                	ld	s9,40(sp)
 678:	7d02                	ld	s10,32(sp)
 67a:	6de2                	ld	s11,24(sp)
 67c:	6109                	addi	sp,sp,128
 67e:	8082                	ret

0000000000000680 <fprintf>:

void
fprintf(int fd, const char *fmt, ...)
{
 680:	715d                	addi	sp,sp,-80
 682:	ec06                	sd	ra,24(sp)
 684:	e822                	sd	s0,16(sp)
 686:	1000                	addi	s0,sp,32
 688:	e010                	sd	a2,0(s0)
 68a:	e414                	sd	a3,8(s0)
 68c:	e818                	sd	a4,16(s0)
 68e:	ec1c                	sd	a5,24(s0)
 690:	03043023          	sd	a6,32(s0)
 694:	03143423          	sd	a7,40(s0)
  va_list ap;

  va_start(ap, fmt);
 698:	fe843423          	sd	s0,-24(s0)
  vprintf(fd, fmt, ap);
 69c:	8622                	mv	a2,s0
 69e:	00000097          	auipc	ra,0x0
 6a2:	e08080e7          	jalr	-504(ra) # 4a6 <vprintf>
}
 6a6:	60e2                	ld	ra,24(sp)
 6a8:	6442                	ld	s0,16(sp)
 6aa:	6161                	addi	sp,sp,80
 6ac:	8082                	ret

00000000000006ae <printf>:

void
printf(const char *fmt, ...)
{
 6ae:	711d                	addi	sp,sp,-96
 6b0:	ec06                	sd	ra,24(sp)
 6b2:	e822                	sd	s0,16(sp)
 6b4:	1000                	addi	s0,sp,32
 6b6:	e40c                	sd	a1,8(s0)
 6b8:	e810                	sd	a2,16(s0)
 6ba:	ec14                	sd	a3,24(s0)
 6bc:	f018                	sd	a4,32(s0)
 6be:	f41c                	sd	a5,40(s0)
 6c0:	03043823          	sd	a6,48(s0)
 6c4:	03143c23          	sd	a7,56(s0)
  va_list ap;

  va_start(ap, fmt);
 6c8:	00840613          	addi	a2,s0,8
 6cc:	fec43423          	sd	a2,-24(s0)
  vprintf(1, fmt, ap);
 6d0:	85aa                	mv	a1,a0
 6d2:	4505                	li	a0,1
 6d4:	00000097          	auipc	ra,0x0
 6d8:	dd2080e7          	jalr	-558(ra) # 4a6 <vprintf>
}
 6dc:	60e2                	ld	ra,24(sp)
 6de:	6442                	ld	s0,16(sp)
 6e0:	6125                	addi	sp,sp,96
 6e2:	8082                	ret

00000000000006e4 <free>:
static Header base;
static Header *freep;

void
free(void *ap)
{
 6e4:	1141                	addi	sp,sp,-16
 6e6:	e422                	sd	s0,8(sp)
 6e8:	0800                	addi	s0,sp,16
  Header *bp, *p;

  bp = (Header*)ap - 1;
 6ea:	ff050693          	addi	a3,a0,-16
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
 6ee:	00000797          	auipc	a5,0x0
 6f2:	19278793          	addi	a5,a5,402 # 880 <__bss_start>
 6f6:	639c                	ld	a5,0(a5)
 6f8:	a805                	j	728 <free+0x44>
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
      break;
  if(bp + bp->s.size == p->s.ptr){
    bp->s.size += p->s.ptr->s.size;
 6fa:	4618                	lw	a4,8(a2)
 6fc:	9db9                	addw	a1,a1,a4
 6fe:	feb52c23          	sw	a1,-8(a0)
    bp->s.ptr = p->s.ptr->s.ptr;
 702:	6398                	ld	a4,0(a5)
 704:	6318                	ld	a4,0(a4)
 706:	fee53823          	sd	a4,-16(a0)
 70a:	a091                	j	74e <free+0x6a>
  } else
    bp->s.ptr = p->s.ptr;
  if(p + p->s.size == bp){
    p->s.size += bp->s.size;
 70c:	ff852703          	lw	a4,-8(a0)
 710:	9e39                	addw	a2,a2,a4
 712:	c790                	sw	a2,8(a5)
    p->s.ptr = bp->s.ptr;
 714:	ff053703          	ld	a4,-16(a0)
 718:	e398                	sd	a4,0(a5)
 71a:	a099                	j	760 <free+0x7c>
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
 71c:	6398                	ld	a4,0(a5)
 71e:	00e7e463          	bltu	a5,a4,726 <free+0x42>
 722:	00e6ea63          	bltu	a3,a4,736 <free+0x52>
{
 726:	87ba                	mv	a5,a4
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
 728:	fed7fae3          	bleu	a3,a5,71c <free+0x38>
 72c:	6398                	ld	a4,0(a5)
 72e:	00e6e463          	bltu	a3,a4,736 <free+0x52>
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
 732:	fee7eae3          	bltu	a5,a4,726 <free+0x42>
  if(bp + bp->s.size == p->s.ptr){
 736:	ff852583          	lw	a1,-8(a0)
 73a:	6390                	ld	a2,0(a5)
 73c:	02059713          	slli	a4,a1,0x20
 740:	9301                	srli	a4,a4,0x20
 742:	0712                	slli	a4,a4,0x4
 744:	9736                	add	a4,a4,a3
 746:	fae60ae3          	beq	a2,a4,6fa <free+0x16>
    bp->s.ptr = p->s.ptr;
 74a:	fec53823          	sd	a2,-16(a0)
  if(p + p->s.size == bp){
 74e:	4790                	lw	a2,8(a5)
 750:	02061713          	slli	a4,a2,0x20
 754:	9301                	srli	a4,a4,0x20
 756:	0712                	slli	a4,a4,0x4
 758:	973e                	add	a4,a4,a5
 75a:	fae689e3          	beq	a3,a4,70c <free+0x28>
  } else
    p->s.ptr = bp;
 75e:	e394                	sd	a3,0(a5)
  freep = p;
 760:	00000717          	auipc	a4,0x0
 764:	12f73023          	sd	a5,288(a4) # 880 <__bss_start>
}
 768:	6422                	ld	s0,8(sp)
 76a:	0141                	addi	sp,sp,16
 76c:	8082                	ret

000000000000076e <malloc>:
  return freep;
}

void*
malloc(uint nbytes)
{
 76e:	7139                	addi	sp,sp,-64
 770:	fc06                	sd	ra,56(sp)
 772:	f822                	sd	s0,48(sp)
 774:	f426                	sd	s1,40(sp)
 776:	f04a                	sd	s2,32(sp)
 778:	ec4e                	sd	s3,24(sp)
 77a:	e852                	sd	s4,16(sp)
 77c:	e456                	sd	s5,8(sp)
 77e:	e05a                	sd	s6,0(sp)
 780:	0080                	addi	s0,sp,64
  Header *p, *prevp;
  uint nunits;

  nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
 782:	02051993          	slli	s3,a0,0x20
 786:	0209d993          	srli	s3,s3,0x20
 78a:	09bd                	addi	s3,s3,15
 78c:	0049d993          	srli	s3,s3,0x4
 790:	2985                	addiw	s3,s3,1
 792:	0009891b          	sext.w	s2,s3
  if((prevp = freep) == 0){
 796:	00000797          	auipc	a5,0x0
 79a:	0ea78793          	addi	a5,a5,234 # 880 <__bss_start>
 79e:	639c                	ld	a5,0(a5)
 7a0:	c795                	beqz	a5,7cc <malloc+0x5e>
    base.s.ptr = freep = prevp = &base;
    base.s.size = 0;
  }
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
 7a2:	6388                	ld	a0,0(a5)
    if(p->s.size >= nunits){
 7a4:	4518                	lw	a4,8(a0)
 7a6:	03277f63          	bleu	s2,a4,7e4 <malloc+0x76>
 7aa:	8a4e                	mv	s4,s3
 7ac:	0009879b          	sext.w	a5,s3
 7b0:	6705                	lui	a4,0x1
 7b2:	00e7f363          	bleu	a4,a5,7b8 <malloc+0x4a>
 7b6:	6a05                	lui	s4,0x1
 7b8:	000a0a9b          	sext.w	s5,s4
  p = brk(nu * sizeof(Header));
 7bc:	004a1a1b          	slliw	s4,s4,0x4
        p->s.size = nunits;
      }
      freep = prevp;
      return (void*)(p + 1);
    }
    if(p == freep)
 7c0:	00000497          	auipc	s1,0x0
 7c4:	0c048493          	addi	s1,s1,192 # 880 <__bss_start>
  if(p == (char*)-1)
 7c8:	5b7d                	li	s6,-1
 7ca:	a0bd                	j	838 <malloc+0xca>
    base.s.ptr = freep = prevp = &base;
 7cc:	00000517          	auipc	a0,0x0
 7d0:	0bc50513          	addi	a0,a0,188 # 888 <base>
 7d4:	00000797          	auipc	a5,0x0
 7d8:	0aa7b623          	sd	a0,172(a5) # 880 <__bss_start>
 7dc:	e108                	sd	a0,0(a0)
    base.s.size = 0;
 7de:	00052423          	sw	zero,8(a0)
    if(p->s.size >= nunits){
 7e2:	b7e1                	j	7aa <malloc+0x3c>
      if(p->s.size == nunits)
 7e4:	02e90a63          	beq	s2,a4,818 <malloc+0xaa>
        p->s.size -= nunits;
 7e8:	4137073b          	subw	a4,a4,s3
 7ec:	c518                	sw	a4,8(a0)
        p += p->s.size;
 7ee:	1702                	slli	a4,a4,0x20
 7f0:	9301                	srli	a4,a4,0x20
 7f2:	0712                	slli	a4,a4,0x4
 7f4:	953a                	add	a0,a0,a4
        p->s.size = nunits;
 7f6:	01352423          	sw	s3,8(a0)
      freep = prevp;
 7fa:	00000717          	auipc	a4,0x0
 7fe:	08f73323          	sd	a5,134(a4) # 880 <__bss_start>
      return (void*)(p + 1);
 802:	0541                	addi	a0,a0,16
      if((p = morecore(nunits)) == 0)
        return 0;
  }
}
 804:	70e2                	ld	ra,56(sp)
 806:	7442                	ld	s0,48(sp)
 808:	74a2                	ld	s1,40(sp)
 80a:	7902                	ld	s2,32(sp)
 80c:	69e2                	ld	s3,24(sp)
 80e:	6a42                	ld	s4,16(sp)
 810:	6aa2                	ld	s5,8(sp)
 812:	6b02                	ld	s6,0(sp)
 814:	6121                	addi	sp,sp,64
 816:	8082                	ret
        prevp->s.ptr = p->s.ptr;
 818:	6118                	ld	a4,0(a0)
 81a:	e398                	sd	a4,0(a5)
 81c:	bff9                	j	7fa <malloc+0x8c>
  hp->s.size = nu;
 81e:	01552423          	sw	s5,8(a0)
  free((void*)(hp + 1));
 822:	0541                	addi	a0,a0,16
 824:	00000097          	auipc	ra,0x0
 828:	ec0080e7          	jalr	-320(ra) # 6e4 <free>
  return freep;
 82c:	609c                	ld	a5,0(s1)
      if((p = morecore(nunits)) == 0)
 82e:	c395                	beqz	a5,852 <malloc+0xe4>
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
 830:	6388                	ld	a0,0(a5)
    if(p->s.size >= nunits){
 832:	4518                	lw	a4,8(a0)
 834:	fb2778e3          	bleu	s2,a4,7e4 <malloc+0x76>
    if(p == freep)
 838:	6098                	ld	a4,0(s1)
 83a:	87aa                	mv	a5,a0
 83c:	fea71ae3          	bne	a4,a0,830 <malloc+0xc2>
  p = brk(nu * sizeof(Header));
 840:	8552                	mv	a0,s4
 842:	00000097          	auipc	ra,0x0
 846:	b66080e7          	jalr	-1178(ra) # 3a8 <brk>
  if(p == (char*)-1)
 84a:	fd651ae3          	bne	a0,s6,81e <malloc+0xb0>
        return 0;
 84e:	4501                	li	a0,0
 850:	bf55                	j	804 <malloc+0x96>
 852:	4501                	li	a0,0
 854:	bf45                	j	804 <malloc+0x96>
