#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

#define MY_LENGTH(ptr) (*(size_t *)(ptr - 8))

int *first = 0;

#define OVERLAP(size) ((size + 7) & ~ (7))



void *divide(void *ptr, size_t size)
{
	void *empty = (ptr + size);

	if (MY_LENGTH(ptr) - size < 9 )
	{
	 	return ptr;
	}

	else if (MY_LENGTH(ptr) - size >= 9 )
	{
	MY_LENGTH(empty) = MY_LENGTH(ptr) - size;

	MY_LENGTH(ptr) = MY_LENGTH(ptr) - MY_LENGTH(empty);

	*(uintptr_t *)empty = (uintptr_t)first;

	first = empty;
	}

	return ptr;

}

void *find_empty(size_t size) {
	void *ptr = first;
	void *last = 0;

	if (!first)
	{
	return 0;
	}

	while (ptr && !(MY_LENGTH(ptr) >= size))
	{
		last = ptr;
		ptr = (void *)*( uintptr_t *)ptr;
	}

	if (ptr)
	{

	if (last)
	{
		*(uintptr_t *)last = *(uintptr_t *)ptr;

	}

	else
	{
		first = (void *)*(uintptr_t *)first;
	}

	void *empty = (ptr + size);

	if (MY_LENGTH(ptr) - size < 11 )
	{
	 	return ptr;
	}

	else if (MY_LENGTH(ptr) - size >=11 )
	{
	MY_LENGTH(empty) = MY_LENGTH(ptr) - size;

	MY_LENGTH(ptr) = MY_LENGTH(ptr) - MY_LENGTH(empty);

	*(uintptr_t *)empty = (uintptr_t)first;

	first = empty;
	}
	return ptr;
	}

	else
	{
		return 0;
	}
}

void *size_required(size_t size)
{

	void *ptr;
	ptr = sbrk(size)+8;
	MY_LENGTH(ptr) = size;
	return ptr;

}

void *mymalloc(size_t size)
{
	void *ptr = 0;

	if (size <= 0)
	{
	return 0;
	}

	size = OVERLAP(size) + 8;

	if (!first)
	{
 		if (size < 992)
		{
		ptr = sbrk(992)+8;
		MY_LENGTH(ptr) = 992;
		return divide(ptr, size);

		}

		else
		{
		ptr = sbrk(size)+8;
		MY_LENGTH(ptr) = size;

		}
	}
	else
	{

		ptr = find_empty(size);


		if (!ptr)
		{
		if (size < 992)
		{
		ptr = sbrk(992)+8;
		MY_LENGTH(ptr) = 992;
		return divide(ptr, size);

	}

	else
	{
		ptr = sbrk(size)+8;
		MY_LENGTH(ptr) = size;
	}
		}
	}
	return ptr;
}

void *mycalloc(size_t nmemb, size_t size)
{
    void *ptr = mymalloc(nmemb * size);

	if (ptr)
	{

    bzero(ptr, OVERLAP(nmemb * size));

    return ptr;
	}

	return 0;
}

void myfree(void *ptr)
{
	if (ptr == 0)
	{
		ptr = 0;
		return;
	}

	else if (!ptr)
	{
	return;
	}

	*(uintptr_t *)ptr = (uintptr_t)first;
	first = ptr;

	while (ptr)
	{
		void *last = 0;
		void *var = first;

		while (var && !((ptr + MY_LENGTH(ptr)) == var))
		{
				last = var;
				var = (void *)*(uintptr_t *)var;
		}

		if (var)
		{
			MY_LENGTH(ptr) = MY_LENGTH(ptr) + MY_LENGTH(var);

			if (last)
			{
				*(uintptr_t *)last = *(uintptr_t *)var;

			}
			else
			{
				first = (void *)*(uintptr_t *)var;
			}
		}

		ptr = (void *)*(uintptr_t *)ptr;
	}

	void *last = 0;
	void *btm = sbrk(0);

	while (ptr && (ptr + MY_LENGTH(ptr)) == btm)
	{
			ptr = (void *)*( uintptr_t *)ptr;

			last = ptr;

		}

		if (!ptr)
		{
		return;
		}

		if (!last)
		{
			first = (void *)*(uintptr_t *)first;
		}
		else
		{
			*(uintptr_t *)last = *(uintptr_t *)ptr;
		}
}

void *myrealloc(void *ptr, size_t size)
{
	if (!ptr)
	{
	return mymalloc(size);
    }

	size = OVERLAP(size) + 8;

	if (size < MY_LENGTH(ptr))
	{
	return divide(ptr, size);
    }

	if (size <= MY_LENGTH(ptr))
	{
	return ptr;
    }

	void *var;

	var = mymalloc(size);

	if (var)
    {
	memcpy(var, ptr, MY_LENGTH(ptr));
    myfree(ptr);
    return var;
	}
	return 0;
}


#if 1
void *malloc(size_t size) { return mymalloc(size); }
void *calloc(size_t nmemb, size_t size) { return mycalloc(nmemb, size); }
void *realloc(void *ptr, size_t size) { return myrealloc(ptr, size); }
void free(void *ptr) { myfree(ptr); }
#endif
