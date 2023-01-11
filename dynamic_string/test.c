#include "test.h"

void test()
{
	dstring str1, str2;
	str1 = ds_new_string_initialize("Department of ");
	str2 = ds_new_string_initialize("Redundancy ");
	dstring str3 = ds_concat(str1, str2); 

	assert(str1 != NULL);
	assert(str2 != NULL);
	assert(strlen(str1) == 14);
	assert(strlen(str2) == 11); 
	assert(strlen(str3) == 25); 

	ds_print(str3, stdout);

	ds_print(str1, stdout);	
	ds_truncate(&str1, 10);	
    assert(strlen(str1) == 10);
    assert(strlen(str2) == 11);
	ds_print(str1, stdout);	

	ds_delete(&str1);
	ds_delete(&str2);

	assert(str1 == NULL);
	assert(str2 == NULL);

	dstring r = ds_random(10);
	ds_print(r, stdout);
	ds_delete(&r);

	printf("\n");
	printf("Tests passed.\n");
}

#define MAX_LEN 100
#define MIN_LEN 5

void auto_tests(int tests)
{
	dstring s = NULL;
	dstring temp = NULL;
	dstring other = NULL;
	int operation;
	int len;

	for (int i = 0; i < tests; i++)
	{
		len = rand() % (MAX_LEN - MIN_LEN) + MIN_LEN;
		s = ds_random(len);
		operation = rand() % 2;

		switch (operation)
		{
		case 0:
			len = rand() % MAX_LEN; 
			temp = ds_random(len);
			assert(strlen(temp) == len);
			int len2 = strlen(s);
			other = ds_concat(s, temp);

			assert(strlen(other) == len + len2);
			assert(strlen(temp) == len);
			assert(strlen(s) == len2);

			printf ("%s + %s = %s\n", s, temp, other);
			break;
		case 1:
			len = rand() % (strlen(s)-1)+1;
			ds_copy(&other, s);
			assert(strcmp(other, s) == 0);

			ds_truncate(&s, len);
			assert(strlen(s) == len);

			printf ("%s trunc --> %s\n", other, s);
			break;
		default:
			break;
		}

		if (other != NULL)
		{
			ds_delete(&other);
			other = NULL;
		}
		if (temp != NULL)
		{
			ds_delete(&temp);
			temp = NULL;
		}
		if (s != NULL)
		{
			ds_delete(&s);
			s = NULL;
		}
	}
	printf("\n");
}