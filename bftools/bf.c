/*

	bf.c: A Brainf*** interpreter

	Copyright 2003 Erik Bosman <ejbosman@cs.vu.nl>

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEMORY_UNIT char
//#define MEMORY_UNIT unsigned short int

#define INITIAL_MEMORY	1024*sizeof(MEMORY_UNIT)
#define MEMORY_INCREASE	1024*sizeof(MEMORY_UNIT)

typedef struct
{
	char type;
	long quantity;
	void *loop;
	void *next;
} Instruction;


FILE *programFile;
MEMORY_UNIT *memory, *limit, *p;
Instruction *program;


// Error messages


int die(char *msg)
{
	fprintf(stderr, "Error: %s\n", msg);
	exit(1);
	return 1; //Avoid warnings
}


// Dynamic memory allocation


void initialize_memory(size_t size)
{
	(memory = (MEMORY_UNIT *)malloc(size)) || die("Cannot allocate memory");
	limit = memory + size;
	memset(memory, 0, size);
}


void expand_memory(size_t size)
{
	size_t memsize = limit - memory + size;
	size_t p_relative = p - memory;

	(memory = (MEMORY_UNIT *)realloc(memory, memsize)) || die("Cannot allocate memory");

	limit = memory + memsize;
	p = p_relative + memory;
	memset(limit - size, 0, size);
}


void free_list(Instruction *list)
{
	Instruction *next;

	while (list->type != ']')
	{
		next = list->next;
		if (list->type == '[') free_list(list->loop);
		free(list);
		list = next;
	}
	free(list);
}


// Loading


Instruction *load()
{
	char c, c2=0;
	long l;

	Instruction *i = (Instruction *)malloc(sizeof(Instruction));
	Instruction *first = i;

	while (!feof(programFile))
	{
		c = fgetc(programFile);
		i->type = c;
		l = 1;
		switch (c)
		{
			case '[':
				i->loop = load();
				break;
			case ']':
				return first;
			case '<': case '>': case '+': case '-': case ',': case '.':
				if (!feof(programFile))
				{
					while (!feof(programFile) && (c2 = fgetc(programFile)) == c) l++;
					ungetc(c2, programFile);
				}
				i->quantity = l;
				break;
			default:
				continue;
		}
		i->next = (Instruction *)malloc(sizeof(Instruction));
		i = i->next;
	}
	i->type = ']';
	return first;
}


// Program execution


void execute(Instruction *i);


void run(Instruction *list)
{
	while (list->type != ']')
	{
		execute(list);
		list = list->next;
	}
}


void execute(Instruction *i)
{
	long l;

	switch (i->type)
	{
		case '>':
			p += i->quantity;
			while(p >= limit) expand_memory(MEMORY_INCREASE);
			return;
		case '<':
			if (p - i->quantity < memory) die("Negative memory pointer");
			p -= i->quantity;
			return;
		case '+':
			*p += i->quantity;
			return;
		case '-':
			*p -= i->quantity;
			return;
		case '.':
			for (l=0; l<i->quantity; l++) putchar(*p);
			return;
		case ',':
			for (l=0; l<i->quantity; l++) *p = getchar();
			return;
		case '[':
			while (*p) run(i->loop);
			return;
		default:
			return;
	}
}


// Main


int main(int argc, char **argv)
{
	if (argc == 1)
	{
		fprintf(stderr, "usage: %s file\n", argv[0]);
		exit(1);
	}

	if (!(programFile = fopen(argv[1], "r")))
	{
		fprintf(stderr, "Error: Cannot open file: %s\n", argv[1]);
		exit(1);
	}

	initialize_memory(INITIAL_MEMORY);

	p = memory + 0;

	program = load();

	run(program);

	free_list(program);

	free(memory);

	fclose(programFile);

	exit(0);
}
