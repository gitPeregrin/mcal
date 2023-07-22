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
#include <unistd.h>
#include <time.h>
#include <sys/ioctl.h>

/*
 * 		ANSI DEFS
 */

//30-40 normal colors
//40-50 background colors
enum colors{
	BLACK = 30 , RED , GREEN , YELLOW , BLUE ,
	MAGENTA , CYAN , LGRAY , L_GRAY , GRAY , BG = 10
};

#define SetColor(color)		printf("\033[%dm",color)
#define strDEF				"\033[0m"
#define strRED				"\033[31m"
#define strBLUE				"\033[34m"
#define strBGRED			"\033[41m"
#define WhiteSpace			puts("\b \b")



/*
 * 		OTHER MACRO DEFS
 */

//debug macro
#define _time_start_		clock_t _old_ = clock();
#define _time_stop_			clock_t _now_ = clock();\
							printf("\nTime check:" strRED "%s %lums\n"\
							strDEF,__func__,_now_-_old_);
#define _debug(var)			printf(#var "=%d\n" ,var)

//terminal size
#define _getTermSize		struct winsize w;ioctl(STDOUT_FILENO,\
							TIOCGWINSZ, &w)
#define COLUMNS				(w.ws_col)
#define ROWS				(w.ws_row)

//aliases
#define ROOT				start_task_chain
#define TASK				cur_task
#define OTSK				old_task
#define ONEX				OTSK->next
#define OSTR				OTSK->raw_str
#define ODAY				OTSK->day
#define OMON				OTSK->month
#define OYER				OTSK->year
#define OFLG				OTSK->isRepeatable
#define OMSG				OTSK->msg
#define TFLG				TASK->isRepeatable
#define TMSG				TASK->msg
#define TDAY				TASK->day
#define TMON				TASK->month
#define TYER				TASK->year
#define TNEX				TASK->next
#define NDAY				clock->tm_mday
#define NMON				clock->tm_mon
#define NYER				clock->tm_year
#define RESULT				OTSK

/*
 * 		CONST DEFS
 */

#define START_Y				(1900)
#define BUFF_SIZE			(256)
#define FALSE 				(0)
#define TRUE 				(1)


/*
 * 		FUNCTIONS
 */

struct task * init_task(char str[BUFF_SIZE]); 	// собираем из строки структуру
void preload_tasks(void);				// читаем таски из файла
void print_task(struct task * cur_task);
void lock_chain(struct task * ct);


/*
 * 		GLOBAL VARS
 */

struct task {
	char raw_str[300];
	int day;			// день (1-31)
	int month;			// месяц (1-12)
	int year;			// год (2023 например)
	char isRepeatable;	// флажки повтора (D,M,Y,N)
	char * msg;			// текст задания
	struct task * next;	// указатель на следующее задание
};

struct task * start_task_chain;
struct task * cur_task;

int first = TRUE;

/*
 * 		MAIN
 */

int main(void) {

	char * month[] = {"январь","февраль","март","апрель","май","июнь",
			"июль","август","сентябрь","октябрь","ноябрь","декабрь"};

	char * head_str = " пн  вт  ср  чт  пт  " strRED "сб  вс" strDEF;
	const int week = 7;

	time_t now; time(&now);
	struct tm * clock = localtime(&now);

	int def_wday = NDAY%week;
	int start_wday = clock->tm_wday;
	int year = START_Y + NYER;

	int m_size[] = {31,
			year%4 || year%100 || year%400 ? 28 : 29,
			31,30,31,30,31,31,30,31,30,31};

	_getTermSize;
	preload_tasks();

	// вычисляем с какого дня недели начинается первое число месяца
	for(int i = 0; i<=def_wday;i++)
		start_wday == 1 ? start_wday = week: start_wday--;

	/*
	 * 	START DRAWING
	 */

	#define line	COLUMNS
	#define r_s		COLUMNS-28
	#define l_s		28
	#define w_s		7

	int left_size = 28;

	char left[l_s * w_s];
	char right[r_s];
	puts(right);

	/*
	 * текстовые паттерны для обработки
	 *
	 * 		@ - будущее событие - @
	 * 		# - настоящее 		- #
	 * 		$ - прошлое			- $
	 */



	while(1){
		switch(TFLG){
		case 'D': // если событие каждый день
			sprintf(right,"#%s#",TMSG);
			puts("case D");
			break;
		case 'M': // если событие каждый месяц
			if(TDAY > NDAY)
				sprintf(right,"@%s@",TMSG);
			else if(TDAY == NDAY)
				sprintf(right,"#%s#",TMSG);
			else
				sprintf(right,"$%s$",TMSG);
			break;
		case 'Y': // если событие каждый год
			if(TMON == NMON+1){
				if(TDAY > NDAY)
					sprintf(right,"@%s@",TMSG);
				else if(TDAY == NDAY)
					sprintf(right,"#%s#",TMSG);
				else
					sprintf(right,"$%s$",TMSG);
			}
			break;
		case 'N': // событие без повторений
			if(TYER == NYER+START_Y)
				if(TMON == NMON+1){
					if(TDAY > NDAY)
						sprintf(right,"@%s@",TMSG);
					else if(TDAY == NDAY)
						sprintf(right,"#%s#",TMSG);
					else
						sprintf(right,"$%s$",TMSG);
				}
			break;
		}
		if(TNEX != NULL)
			TASK = TNEX;
		else
			break;
	}

	sprintf(left," %-25s %d",month[NMON],NYER+START_Y);
	puts(left);
	sprintf(left,"%s",head_str);

	puts(left);

	//TODO нужно сделать заполнение строки текстом
	//TODO а потом уже puts() всего текста на экран

	/*
	 * 	END DRAWING
	 */

	WhiteSpace;

	return 0;
}

/*
 * 		DESTRUCTOR
 */

__attribute__((destructor)) void d(void){
	struct task * OTSK = TASK = ROOT;
	while(ROOT != NULL){
		if(TNEX != NULL){
			OTSK = TASK;
			TASK = TNEX;
		}else if(TNEX == NULL){
			if(OTSK != ROOT){
				free(TASK);
				TASK = OTSK;
				TNEX = NULL;
				OTSK = TASK = ROOT;
			}else{
				free(ROOT);
				OTSK = TASK = ROOT = NULL;
			}
		}
	}
}

/*
 * 		END MAIN
 */

void preload_tasks(void){
	FILE * TFIL = fopen("tasks.src","r");
	if(TFIL == NULL){
		TFIL = fopen("tasks.src","a");
		fclose(TFIL);
		TFIL = fopen("tasks.src","r");
	}

	char str[BUFF_SIZE] = {0};

	while(fgets(str,BUFF_SIZE,TFIL) != NULL){
		TASK = init_task(str);
		if(first){ROOT = TASK; first=FALSE;}
		lock_chain(TASK);
	}

	TASK = ROOT;
	fclose(TFIL);
}


struct task * init_task(char str[BUFF_SIZE]){
	struct task * RESULT = malloc(sizeof(struct task));
	unsigned char attr = 0, start_to_read = 0;

	sprintf(OSTR,"%s",str);

	for(int i=0;i<BUFF_SIZE;i++){

		if(str[i] == ' '){
			attr++;
			str[i] = '\0';

			switch(attr){
			case 1:
				ODAY = atoi(&str[start_to_read]);
				start_to_read = i+1;
				break;
			case 2:
				OMON = atoi(&str[start_to_read]);
				start_to_read = i+1;
				break;
			case 3:
				OYER = atoi(&str[start_to_read]);
				start_to_read = i+1;
				break;
			case 4:
				OFLG = str[start_to_read];
				start_to_read = i+1;
				break;
			case 5:
				OMSG = OSTR+start_to_read;
				break;
			}


			str[i] = ' ';


		}
	}

	return RESULT;
}


void print_task(struct task * cur_task) {
	printf(strRED "%p:\n" strDEF, 	TASK);
	printf("Day: %d\n", 			TDAY);
	printf("Month: %d\n",			TMON);
	printf("Year: %d\n",			TYER);
	printf("Flag: %c\n",			TFLG);
	printf("Msg: %s\n", 			TMSG);
	printf("next->%p\n",			TNEX);
}

void lock_chain(struct task * TASK){
	struct task * OTSK = ROOT;
	while(OTSK != NULL){
		if(OTSK == TASK) return;
		if(ONEX == NULL) {
			ONEX = TASK;
			return;
		}else{
			OTSK = ONEX;
		}
	}
}
