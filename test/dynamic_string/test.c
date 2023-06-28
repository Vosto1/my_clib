#include "test.h"

void test()
{
    char* cstr1 = "Department of ";
    char* cstr2 = "Redundancy ";
	dstring str1, str2;
	str1 = ds_new_string_initialize(cstr1);
	str2 = ds_new_string_initialize(cstr2);
	dstring str3 = ds_concat(str1, str2); 

	assert(str1.string != NULL);
	assert(str2.string != NULL);
	assert(str1.length == 14);
	assert(str2.length == 11); 
	assert(str3.length == 25); 
	assert(str1.size == 14+1);
	assert(str2.size == 11+1); 
	assert(str3.size == 25+1); 

	ds_print(str3, stdout);
    ds_delete(&str3);

	ds_print(str1, stdout);	
	ds_truncate(&str1, 10);	
    assert(str1.length == 10);
    assert(str2.length == 11);
    assert(str1.size == 10+1);
    assert(str2.size == 11+1);
	ds_print(str1, stdout);	


	ds_delete(&str1);
	ds_delete(&str2);

	assert(str1.string == NULL);
	assert(str2.string == NULL);

	dstring r = ds_random(10);
	ds_print(r, stdout);
	ds_delete(&r);

    dstring trimmed = s_remove_last_n(cstr1, 4);
    assert(strcmp(trimmed.string, "Department") == 0);
    ds_delete(&trimmed);

	printf("\n");
	printf("Tests passed.\n");
}

#define MAX_LEN 100
#define MIN_LEN 5

dstring new()
{
    dstring s;
    s.string = NULL;
    s.length = 0;
    s.size = 0;
    return s;
}

void auto_tests(int tests)
{
	dstring s = new();
	dstring temp = new();
	dstring other = new();
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
			assert(temp.length == len);
			assert(temp.size == len+1);
			int len2 = s.length;
			other = ds_concat(s, temp);

			assert(other.length == len + len2);
			assert(other.size == len + len2 + 1);
			assert(temp.length == len);
			assert(temp.size == len + 1);
			assert(s.length == len2);
			assert(s.size == len2 + 1);

            // print result
            printf("%s", s);
            pcred();
            printf(" + ");
            pcreset();
            printf("%s", temp);
            pcred();
            printf(" = ");
            pcreset();
            printf("%s\n", other);

			break;
		case 1:
			len = rand() % (s.length-1)+1;
			ds_copy(&other, s);
			assert(strcmp(other.string, s.string) == 0);

			ds_truncate(&s, len);
			assert(s.length == len);
			assert(s.size == len + 1);

            // print result
            printf("%s", other);
            pcred();
            printf(" trunc --> ");
            pcreset();
            printf("%s\n", s);

			break;
		default:
			break;
		}

		if (other.string != NULL)
		{
			ds_delete(&other);
			other = new();
		}
		if (temp.string != NULL)
		{
			ds_delete(&temp);
			temp = new();
		}
		if (s.string != NULL)
		{
			ds_delete(&s);
			s = new();
		}
	}
	printf("Tests passed\n");
}