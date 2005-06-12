/*

	bfstrip.c: A Brainf*** program optimizer -sort of-

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

int c;

long count;

long newline_count;

void ask_newline()
{
	if (++newline_count == 72)
	{
		fputc('\n', stdout);
		newline_count = 0;
	}
}

void print_n_times_x(long n, int x)
{
	long i;

	if (n<0) for (i =-n; i>0; i--)
	{
		fputc(x-1, stdout);
		ask_newline();
	}
	if (n>0) for (i = n; i>0; i--)
	{
		fputc(x+1, stdout);
		ask_newline();
	}
}


void minimalize(int type)
{
	count += c - type;

	while (!feof(stdin))
	{
		c = fgetc(stdin);

		if (( c != '<' ) && ( c != '>' ) &&
			( c != '+' ) && ( c != '-' ) &&
			( c != '[' ) && ( c != ']' ) &&
			( c != ',' ) && ( c != '.' )) continue;

		if ( (c != type +1) && (c != type -1) )
		{
			ungetc(c, stdin);
			break;
		}

		count += c - type;
	}

	print_n_times_x(count, type);
}


int main(int argc, char **argv)
{

	newline_count = 0;

	while (!feof(stdin))
	{
		count = 0;

		c = fgetc(stdin);

		switch (c)
		{
			case '<': case '>':
				minimalize('=');
				break;
			case '+': case '-':
				minimalize(',');
				break;
			case '[': case ']': case ',': case '.':
				fputc(c, stdout);
				ask_newline();
				break;
			default:
				break;
		}
	}

	fputc('@',stdout);
	if (newline_count) fputc('\n', stdout);

	exit(0);
}

