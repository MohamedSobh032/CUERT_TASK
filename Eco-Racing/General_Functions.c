/*
 * General_Functions.c
 *
 * Created: 7/31/2023 9:12:30 PM
 *  Author: Mohamed
 */ 

#include "General_Functions.h"

char Is_Same_String(char arr[], char arr2[])
{
	int i = 0;
	for (; i < 10; i++)
	{
		if (arr[i] != arr2[i])
		{
			return 0;
		}
	}
	return 1;
}