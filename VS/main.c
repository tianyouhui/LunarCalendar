#include <windows.h>
#include <stdio.h>
#include "../Classes/LunarCalendar.h"

int main(int argc, char **argv)
{
	char *lunar = NULL;
	LunarCalendar *calendar = LCCreateLunarCalendar();
	lunar = (char *)LCLunarDate(calendar);
	printf("%s", lunar);
	lunar = (char *)LCEightWord(calendar);
	printf("%s\n", lunar);
	LCReleaseLunarCalendar(calendar);
	system("pause");
	return 0;
}