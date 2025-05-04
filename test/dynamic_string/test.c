#include "test.h"


typedef struct test_case_container
{
    char* string;
    uint target_split_count;
}testCase;

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


    // split string edge cases:

    char delim = '.';

    assert(delim == '.');
    testCase edge_cases[15] = 
    {
        { .string = ".ello", .target_split_count = 1 },
        { .string = "hell.", .target_split_count = 1 },
        { .string = "he...", .target_split_count = 1 },
        { .string = "...lo", .target_split_count = 1 },
        { .string = "he..o", .target_split_count = 2 },
        { .string = "h...o", .target_split_count = 2 },
        { .string = ".....", .target_split_count = 0 },
        { .string = "aaa.he...", .target_split_count = 2 },
        { .string = "...lo.sdf.", .target_split_count = 2 },
        { .string = ".hell.", .target_split_count = 1 },
        { .string = ".he..o.", .target_split_count = 2 },
        { .string = "aaa.he...sdfw...wfe.e...ef..f.", .target_split_count = 7 },
        { .string = ".....lo.sdf...sdfl.sdf..wl..w.f.e..d...d.", .target_split_count = 10 },
        { .string = "aaa.he...sdfw...wfe.e...ef..f.aa", .target_split_count = 8 },
        { .string = "........_.....{.....V....(T.bM.Q.Y.U..?.v.4i6.-.j1.A..&..X..)N.B*mu1.5v).-s.>!p.1.K4o.F]+MA2.2.' I61", .target_split_count = 26 }
    };

    for (int i = 0; i < 15; i++)
    {
        dstring edge_case = ds_new_string_initialize(edge_cases[i].string);
        mdarray results2 = ds_split(edge_case, delim);
        for (int j = 0; j < results2.used; j++)
        {
            dstring* str = (dstring*)results2.array[j];
            printf("%s\n", str->string);
        }
        assert(results2.used == edge_cases[i].target_split_count);
    }

	printf("\n");
	printf("Tests passed.\n");
}

#define MAX_LEN 100
#define MIN_LEN 50 // changing this to a smaller number might not be a good idea, check code below...

dstring new()
{
    dstring s;
    s.string = NULL;
    s.length = 0;
    s.size = 0;
    return s;
}

int* create_dyn_int(int value)
{
    int* new_int = (int*)malloc(sizeof(int));
    *new_int = value;
    return new_int;
}

void free_dyn_int(void* value)
{
    int* num = (int*)value;
    free(num);
    value = NULL;
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
		operation = rand() % 3;

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
        case 2:
            char delim = '.';
            /* assert of min len due to assumptions made in asserts later:
            * there can be 10 delims inserted (see max below)
            * delim must not be beside each other
            *   => worst case: 10 * 2 + 1 character must be in string
            * delim cannot be at index 0 or string.length
            *   => prepend and append a character (+2)
            * string min length = 10 * 2 + 1 + 2 = 23
            */
            assert(MIN_LEN >= 23); // having min as 23 is probably still not a good idea
            // in case there are no "." in the string (for testing replace function)
            s.string[s.length / 2] = delim;
            printf("before replace: \"%s\"\n", s);
            ds_replace_char(s, delim, 'A');
            printf("after replace: \"%s\"\n", s);
            assert(ds_count_char(s, '.') == 0);
            int min = 2, max = 10;
            int number_of_delims = min + rand() % (max - min);
            for (int i = 0; i < number_of_delims; i++)
            {
                int index = 0;
                do
                {
                    index = rand() % s.length;
                } while(
                    ds_at(s, index) == delim ||
                    index == 0 ||
                    index == s.length - 1 ||
                    ds_at(s, index + 1) == delim ||
                    ds_at(s, index - 1) == delim);
                s.string[index] = delim;
            }
            printf("after adding %d new delims \"%c\": %s\n", number_of_delims, delim, s);

            assert(ds_count_char(s, delim) == number_of_delims);

            mdarray results = ds_split(s, delim);

            pcred();
            printf("split ");
            pcreset();

            printf("%s", s);

            pcred();
            printf(" = ");
            pcreset();

            for (uint i = 0; i < results.used; i++)
            {
                const dstring* split_result = mda_at(&results, i);
                printf("%s", split_result->string);

                if (i+1 == results.used)
                    break;

                pcred();
                printf(" . ");
                pcreset();
            }

            assert(results.used == number_of_delims + 1);
            mda_free(&results);

            printf("\n");

            assert(delim == '.');
            dstring str = ds_random(100);
            printf("[random delim placement tests] before replace: \"%s\"\n", str);
            ds_replace_char(str, delim, 'A');
            printf("[random delim placement tests] after replace: \"%s\"\n", str);
            assert(ds_count_char(str, '.') == 0);

            min = 30;
            max = 100;
            number_of_delims = min + rand() % (max - min);
            mdarray indecies = mda_create_empty();
            mda_init(&indecies, 100, &free_dyn_int);
            for (int i = 0; i < 100; i++)
            {
                int* num = create_dyn_int(i);
                mda_insert(&indecies, num);
            }
            for (int i = 0; i < number_of_delims; i++)
            {
                int index = rand() % indecies.used;
                int* delim_index = (int*)mda_remove_at(&indecies, index);
                str.string[*delim_index] = delim;
                free_dyn_int(delim_index);
            }
            mda_free(&indecies);
            printf("[random delim placement tests] after adding %d new delims \"%c\": %s\n", number_of_delims, delim, str);

            assert(ds_count_char(str, delim) == number_of_delims);

            results = ds_split(str, delim);

            printf("[random delim placement tests] ");
            pcred();
            printf("split ");
            pcreset();

            printf("%s", str);

            pcred();
            printf(" = ");
            pcreset();

            for (uint i = 0; i < results.used; i++)
            {
                const dstring* split_result = mda_at(&results, i);
                printf("%s", split_result->string);

                if (i+1 == results.used)
                    break;

                pcred();
                printf(" . ");
                pcreset();
            }

            printf("\n");

            ds_delete(&str);
            mda_free(&results);

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