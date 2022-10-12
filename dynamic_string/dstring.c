#include "dstring.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>


dstring ds_init(const char* s)
{
	// Precondition: s ar ej NULL - testas med en assert
	assert(s != NULL);
	/* Tips:
	   - Allokera (t.ex. med malloc) tillrackligt med minne for att fa plats med s,
         glom inte att det maste finnas plats f�r \0 (nolltermineringen).
         Glom inte att testa sa att allokeringen lyckades
	   - Det �r tillatet att anv�nda inbyggda strangfunktioner (som t.ex strcpy())
	   - Kom ihag att anvanda assert() */
	char* temp;
	temp = (char*)malloc(sizeof(char) * (strlen(s) + 1));
	assert(temp != NULL);
	strcpy(temp, s);
	assert(strlen(temp) == strlen(s));
	// Postcondition: returvardet innehaller samma strang som 's' - beh�ver inte testas med assert
	assert(!(strcmp(temp, s)));
	return temp; // Ersatt denna rad. Den ar just nu med for att programmet ska kompilera
}

int ds_concat(dstring* dest, dstring src)
{
    // Precondition: dest �r ej NULL
	// Precondition: *dest ar ej NULL
	// Precondition: src ar ej NULL
    // Preconditions testas med asserts.
	assert(dest != NULL);
	assert(*dest != NULL);
	assert(src != NULL);
	/* Tips: 
	   - Tank p� att *dest redan ar en dynamiskt allokerad strang
	   - Tank p� att vi kommer att behova allokera mer minne for att fa plats med den har langre strangen.
         Minnet som *dest pekar pa behover utokas (realloc), hur stort ska det "nya" minnet bli?
         Glom inte att nolltermineringen maste fa plats.
         Nar du allokerar minne dynamiskt maste du alltid testa sa att allokeringen lyckades innan du
         borjar jobba med minnet.
       - Det ar tillatet att anvanda inbyggda funktioner (som t.ex. strcat()).
    */
	int test1 = strlen(src), test2 = strlen(*dest);

	int result = 0, len = (strlen(*dest) + strlen(src) + 1);
	dstring temp = (dstring)realloc(*dest, sizeof(char)*len);
	assert(temp != NULL);
	if (temp != NULL)
	{
		result = 1;
		*dest = temp;
		strcat(*dest, src);
	}
	else
		return result;
	// Postcondition: *dest innehaller den gamla strangen ihopslagen med src - beh�ver inte testas med assert.
	return result; // Ersatt denna rad. Den ar just nu med for att programmet ska kompilera, vad ar det som ska returneras?.
}

void ds_truncate(dstring* dest, unsigned int truncated_len)
{
	// Precondition: dest �r ej NULL, *dest ar ej NULL
    // l�ngden (truncateLength) f�r inte vara negativ
    // Preconditions testas med asserts
	assert(*dest != NULL);
	assert(dest != NULL);
	assert(truncated_len > 0);

	dstring temporary;
	temporary = (char*)malloc(sizeof(char) * (strlen(*dest) + 1));
	if (temporary == NULL) 
	{
		printf("Error allocating memory\n");
		return;
	}
	strcpy(temporary, *dest);
	/* Tips:
	   - Anvand realloc for att frigora det overflodiga minnet
         (realloc frigor forst minnet och allokerar sedan nytt for den angivna storleken)
	   - glom inte nolltermineringen
     
     Vad h�nder om truncateLength �r l�ngre �n vad s�ngen �r?*/
	
	if (truncated_len < strlen(*dest))
	{
		*(*dest + truncated_len) = '\0';
		dstring temp = (dstring)realloc(*dest, (truncated_len + 1) * sizeof(char));
		assert(temp != NULL);
		if (temp != NULL)
		{
			*dest = temp;
			assert(strlen(*dest) == truncated_len);
		}
		else
		{
			printf("Error allocating memory\n");
			strcpy(*dest, temporary);
			free(temporary);
			return;
		}
	}
	else
	{
		free(temporary);
		return;
	}
	// Postcondition: *dest ar inte langre an 'truncated_len' tecken - beh�ver inte testas med assert
	assert(strlen(*dest) == truncated_len);
	free(temporary);
}

void ds_print(dstring s, FILE* file)
{
	// Precondition: file ar inte NULL - maste testas innan du forsoker skriva till filen
    /*Tank p� att filpekaren kopplas till filen innan funktionen anropas*/
	assert(file != NULL);

	/* filpekaren far inte stangas, filen ska fungera som vanligt efter anropet */
    /*Den har funktionen ska vara generell, du ska kunna anvanda den for att skriva ut till
     en fil eller for att skriva ut i konsollen (pa skarmen). Om den anvands for att skriva till
     en extern fil sa kan du anta att filen oppnas och stangs innan respektive efter anrop till
     funktionen. Om den anvands for att skriva ut pa skarmen kan du anta att du far stdout till den
     andra parametern.
     
     Du ska inte skriva olika kod for att skriva till fil och till konsoll. Samma kod ska fungera for bada.*/
	int length = strlen(s);
	for (int i = 0; i < length; i++)
	{
		fprintf(file, "%c", s[i]);
	}
}

void ds_delete(dstring* del)
{
	// Precondition: del ar inte NULL
	assert(*del != NULL);
	free(*del);
	*del = NULL;
	// Postcondition: *del ar NULL och minnet ar frigjort - beh�ver inte testas med assert
}

size_t ds_copy(dstring* dest, dstring src)
{
	if (src == NULL || strlen(src) == 0)
	{
		// error
		return 0;
	}
	dstring tmp = (dstring)malloc(strlen(src) + 1);
	if (!tmp)
		return 0;
	*dest = tmp;
	strcpy(*dest, src);
	return 1;
}