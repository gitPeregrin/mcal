/*
 ============================================================================
 Name        : mcal.c
 Author      : gitPeregrin
 Version     : 1.0
 Copyright   : mikes calendar
 Description : console calendar
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define debug(var)			printf(#var "=%d\n" ,var);
#define START_Y				1900

/*
 * ANSI DEFS
 */


#define Clear				puts("\033[2J" "\033[H")
#define SetCurPos(x,y)		printf("\033[%d;%dH",y,x)
#define Erase				puts("\033[K")
#define HideCursor			puts("\e[?25l")
#define ShowCursor			puts("\e[?25h")
#define SetColor(color)		printf("\033[%dm",color)
#define SaveCurPos			puts("\033[s")
#define RestoreCurPos		puts("\033[u")
#define WhiteSpace			puts("\b \b")
#define MVU(n)				printf("\033[%dA",n)
#define MVD(n)				printf("\033[%dB",n)
#define MVR(n)				printf("\033[%dC",n)
#define MVL(n)				printf("\033[%dD",n)
#define MVDir(d,n)			switch(d){\
							case 52: MVU(n); break;\
							case 53: MVD(n); break;\
							case 51: MVL(n); break;\
							case 50: MVR(n); break;\
							default: Clear; break;}



//30-40 normal colors
//40-50 bg colors

enum colors{
	BLACK = 30 , RED , GREEN , YELLOW , BLUE ,
	MAGENTA , CYAN , LGRAY , L_GRAY , GRAY , BG = 10
};

enum move{
	RIGHT = 50, LEFT, UP, DOWN
};

/*
 * 	END ANSI DEFS
 */

int main(void) {

	char * month[] = {"январь","февраль","март","апрель","май","июнь",
			"июль","август","сентябрь","октябрь","ноябрь","декабрь"};


	char * head_str = " пн  вт  ср  чт  пт  \033[31mсб  вс\033[0m";
	const int week = 7;

	time_t now;
	time(&now);
	struct tm *clock = localtime(&now);
	int def_wday = clock->tm_mday%week;
	int start_wday = clock->tm_wday;
	int year = START_Y + clock->tm_year;

	int m_size[] = {31,
			year%4 == 0 && year%100 == 0 && year%400 == 0 ? 29 : 28,
			31,30,31,30,31,31,30,31,30,31};

	// вычисляем с какого дня недели начинается первое число месяца
	for(int i = 0; i<=def_wday;i++)
		start_wday == 1 ? start_wday = week: start_wday--;

	/*
	 * 	START DRAWING
	 */

	printf("\n %-25s %d\n\n",month[clock->tm_mon],year);


	puts(head_str);

	for(int day = 0; day<=m_size[clock->tm_mon]+start_wday; day++){
		if(day%week == 0) putchar('\n');

		if(day-start_wday == clock->tm_mday) SetColor(BG+LGRAY);
		if(day-start_wday<=0)	printf("    ");
		else 					printf(" %2d ",day-start_wday);
		SetColor(0);

	}

	/*
	 * 	END DRAWING
	 */

	getchar();
	return 0;
}
