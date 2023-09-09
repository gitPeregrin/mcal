/*
 * bits.h
 *
 *  Created on: 28 июл. 2023 г.
 *      Author: gitPeregrin
 */

#ifndef BITS_H_
#define BITS_H_

//def types
#ifndef bool
typedef _Bool bool;
#endif
#ifndef lint
typedef long long lint;
#endif
#ifndef uint
typedef unsigned int uint;
#endif
#ifndef uchar
typedef unsigned char uchar;
#endif

//def prots
uint 	powi	(uchar, uchar);
bool	getbit	(lint, uchar);
void	setbit	(lint *, uchar, bool);

uint powi(uchar num, uchar deg){
  if (!deg)
	  return (uint)1;
  if (deg & 1)
	  return powi(num, deg - 1) * num;
  num = powi(num, deg >> 1);
  return num * num;
}

bool getbit(lint num, uchar b){
  uint pwrd = powi(2,b);
  return (num & pwrd) >> b;
}

void setbit(lint * num, uchar b, bool exp){
  uint pwrd = (powi(2,b));
  if(getbit(*num,b) == exp) return;
  *num += (*num & pwrd) >> b ? -1 * pwrd : pwrd;
}

#endif /* BITS_H_ */
