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

enum bool {
	false, true
};

/*
 * ANSI DEFS
 */

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

//30-40 normal colors
//40-50 bg colors

/*
 * 	END ANSI DEFS
 */

struct task {
	int day;			// день (1-31)
	int month;			// месяц (1-12)
	int year;			// год (2023 например)
	char isRepeatable;	// флажки повтора (D,M,Y,N)
	char * msg;			// текст задания
	struct task * next;	// указатель на следующее задание
} * start_task_chain, * cur_task;


enum bool first = true;
int task_count = 0;

struct task * init_task(char * str); 	// собираем из строки структуру
void preload_tasks(void);				// читаем таски из файла

int main(void) {

	preload_tasks();

	char * month[] = {"январь","февраль","март","апрель","май","июнь",
			"июль","август","сентябрь","октябрь","ноябрь","декабрь"};

	char * head_str = " пн  вт  ср  чт  пт  \033[31mсб  вс\033[0m";
	const int week = 7;

	time_t now; time(&now);
	struct tm *clock = localtime(&now);

	int def_wday = clock->tm_mday%week;
	int start_wday = clock->tm_wday;
	int year = START_Y + clock->tm_year;

	int m_size[] = {31,
			year%4 || year%100 || year%400 ? 28 : 29,
			31,30,31,30,31,31,30,31,30,31};

	// вычисляем с какого дня недели начинается первое число месяца
	for(int i = 0; i<=def_wday;i++)
		start_wday == 1 ? start_wday = week: start_wday--;

	/*
	 * 	START DRAWING
	 */

	//TODO нужно сделать заполнение строки текстом
	//TODO а потом уже puts() всего текста на экран

	/*
	 * 	END DRAWING
	 */

	WhiteSpace;
	return 0;
}

void preload_tasks(void){
	FILE * task_file = fopen("tasks.src","r");
	if(task_file == NULL){
		task_file = fopen("tasks.src","a");
		fclose(task_file);
		task_file = fopen("tasks.src","r");
	}

	/*
	 * 	FILE OPENED
	 */

	char str[300] = {0};
	while(fgets(str,300,task_file) != NULL){
		cur_task = init_task(str);
		if(first){start_task_chain = cur_task; first=false;}
		cur_task = cur_task->next;
	}

	cur_task = start_task_chain;

	/*
	 * 	CLOSE FILE RUN MAIN
	 */

	fclose(task_file);

}

struct task * init_task(char * str){
	struct task * RESULT = malloc(sizeof(struct task));
	int size;
	unsigned char attr = 0, start_to_read = 0;
	for(int i=0;i<300;i++){

		if(str[i] == ' '){
			attr++;
			str[i] = '\0';


			switch(attr){
			case 1:
				RESULT->day = atoi(&str[start_to_read]);
				start_to_read = i;
				break;
			case 2:
				RESULT->month = atoi(&str[start_to_read]);
				start_to_read = i;
				break;
			case 3:
				RESULT->year = atoi(&str[start_to_read]);
				start_to_read = i;
				break;
			case 4:
				RESULT->isRepeatable = str[start_to_read+1];
				start_to_read = i+1;
				break;
			case 5://TODO тут
				size = 300 - start_to_read;
				RESULT->msg = malloc(size);
				break;
			default:
				break;
			}

			str[i] = ' ';



		}

		// TODO не работает копирование текста

		if(attr > 4){
			RESULT->msg[i-start_to_read] = str[i];
		}
	}

	//дебаг


	printf("Day: %d\n", RESULT->day);
	printf("Month: %d\n", RESULT->month);
	printf("Year: %d\n", RESULT->year);
	printf("Flag: %c\n", RESULT->isRepeatable);
	printf(strRED "Msg: %s\n" strDEF, RESULT->msg);


	return RESULT;
}

