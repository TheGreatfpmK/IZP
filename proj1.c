#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


/*
*	IZP project1 - Text editor
*	2018
*	Filip Macák
*/


//commands


void insert(char content[1001])		//command i , inserts text before the current line
{
	unsigned int con_length = strlen(content);
	for (unsigned int i = 1; i < con_length; i++)
	{
		putchar(content[i]);
	}
}

void before(char content[1001], char p_input[1001])		//command b, inserts text at the beginning of the current line
{
	unsigned int con_length = strlen(content), inp_length = strlen(p_input);
	for (unsigned int i = con_length - 2; i > 0; i--)
	{
		for (int j = inp_length - 1; j >= 0; j--)
		{
			p_input[j+1] = p_input[j];
		}
		p_input[0] = content[i];
		inp_length = strlen(p_input);
	}
}

void after(char content[1001], char p_input[1001])		//command a, inserts text at the end of the current line 
{
	unsigned int con_length = strlen(content), inp_length = strlen(p_input);
	for (unsigned int i = 1; i < con_length; i++)
	{
		p_input[i+inp_length-2] = content[i];
	}
}

void removeEOL(char input[1001])		//command r, removes end of line character at the current line
{
	if (input[strlen(input)-1] == '\n')
	{
		input[strlen(input)-1] = '\0';
	}
}

int GetNumber(char content[1001])
{
	char *ptr = NULL;
	int val;
	val = strtol(content+1, &ptr, 10);
	if (ptr[0] != '\n')
	{
		return -1;
	}
	return val;
}

void substitute(char content[1001], char p_input[1001], int type)	//command s, finds and substitutes string in current line
{																	//must be written as s:PATTERN:REPLACEMENT where ':' can be any character
	char pattern[1001], replacement[1001];
	unsigned int i = 2,  con_length = strlen(content), inp_length = strlen(p_input);
	int control = 0, str_pos, len_diff = 0, cycle = 0;
	char *p_str = NULL;
	while (content[i] != content[1])
	{
		i++;
		if (i == con_length)
		{
			control = 1;
			break;
		}
	}
	if (control)
	{
		return;
	}
	for (unsigned int j = 0; j < i - 2; j++)
	{
		pattern[j] = content[j+2];
		pattern[j+1] = '\0';					//niekedy bol pattern zle nacitany toto to vyriesilo
	}
	for (unsigned int j = 0; j < con_length - i - 2; j++)
	{
		replacement[j] = content[j+i+1];
	}
	while ((p_str = strstr(p_input + cycle, pattern)) != NULL)
	{
		str_pos = p_str - p_input;
		len_diff = con_length - 2 * i;
		inp_length = strlen(p_input);
		if (len_diff > 0)
		{
			for (unsigned int j = inp_length - 1; j > str_pos + i - 3; j--)
			{
				p_input[j+len_diff] = p_input[j];
			}

		}
		else
		{
			for (unsigned int j = str_pos + i - 2; j < inp_length; j++)
			{
				p_input[j+len_diff] = p_input[j];
			}
			for (int j = 0; j > len_diff; j--)
			{
				p_input[strlen(p_input)-1] = '\0';
			}
		}
		for (unsigned int j = 0; j < con_length - i - 2; j++)
		{
			p_input[str_pos+j] = replacement[j];
		}
		if (type)
		{
			return;
		}
		cycle = str_pos + strlen(replacement);
	}
	memset(replacement, 0, strlen(replacement));
	memset(pattern, 0, strlen(pattern));
}

//.
//.	TU POKRACUJ
//.

void next(char input[1001])			//command n, outputs current line and moves onto the next line
{									//can be called as nN where N represents how many lines will be printed
	fputs(input, stdout);
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "Error: Too many or no arguments. Use argument -help to see how to use this program\n");	//dopisat nejaku error spravu
		return 1;
	}

	if (strcmp(argv[1], "-help") == 0)
	{
		fprintf(stderr, "\nThis program is a text editor. It reads commands from text file and input from standard input.\nThe program expects exactly one argument which should be the path to the text file containing commands.\n\nHere is a list of supported commands:\niCONTENT (insert):\n\tinserts content onto a new line before the current line\nbCONTENT (before):\n\tinserts content at the start of the current line\naCONTENT (after):\n\tinserts content at the end of the current line\nd (delete):\n\tdeletes current line. Can be used as dN where N represents how many lines will be deleted\nr (removeEOL):\n\tremoves end of line character from the current line\ns:PATTERN:REPLACEMENT (substitute):\n\tsubstitutes first pattern found in the current line for replacement. ':' can be replaced by any character\nS:PATTERN:REPLACEMENT:\n\tsame as s but it is done for every pattern located in the current line\nn (next):\n\tprints out current line and moves onto the next one. Can be used as nN where N represents how many lines will be printed\nq (quit):\n\texits the program. Nothing will be printed or edited after this command is read\ngX (go to):\n\tjumps onto Xth command\n");
		return 0;
	}

	FILE *commandsfile;
	if ((commandsfile = fopen(argv[1], "r")) == NULL)
	{
		fprintf(stderr, "Error: File containing commands wasn't opened succesfully. Use argument -help to see how to use this program.\n");		//dopisat error spravu
		return 1;
	}

	char commands[100][1001];
	int i = 0;
	while (fgets(commands[i], 1002, commandsfile) != NULL)
	{
		i++;
	}
	i--;
	fclose(commandsfile);


	if (strcmp(commands[i], "\0") == 0)
	{
		i--;
	}

	char input[1001];
	int line = -1, control = 1, cycle = 0, edit = 0;	//control is used to get out of the loop to read another line from stdin 
	while (fgets(input, 1002, stdin) != NULL)			//cycle counts how many times will the current command be repeated (used for commands n and d)
	{													//edit controls if the current line was already edited
		control = 1;
		edit = 0;								
		while (control)
		{
			if (cycle == 0)
			{
				line++;
			}	
			if (line == i + 1)
			{
				do {next(input);}
				while (fgets(input, 1002, stdin) != NULL);
				return 0;
			}
			switch (commands[line][0])
			{
				case 'i':
					insert(commands[line]);
					break;

				case 'b':
					before(commands[line], input);
					break;

				case 'a':
					after(commands[line], input);
					break;

				case 'd':								//command d (delete), deletes current line
					if (cycle == 0)						//can be called as dN where N defines how many lines should be removed
					{
						cycle = GetNumber(commands[line]);
					}
					if (cycle == -1)
					{
						fprintf(stderr, "Error:Line %d: Command %c contains character, remove it before running the program again\n", line+1, commands[line][0]);
						return 1;
					}
					control = 0;
					break;

				case 'r':			
					removeEOL(input);
					break;

				case 's':
					substitute(commands[line], input, 1);
					break;

				case 'S':
					substitute(commands[line], input, 0);
					break;

				case 'n':
					next(input);
					if (cycle == 0)
					{
						cycle = GetNumber(commands[line]);
					}
					if (cycle == -1)
					{
						fprintf(stderr, "Error:Line %d: Command %c contains character, remove it before running the program again\n", line+1, commands[line][0]);
						return 1;
					}
					control = 0;
					break;

				case 'q':
					if (edit)
					{
						next(input);
					} 
					return 0;

				case 'g':
					if (GetNumber(commands[line]) == -1)
					{
						fprintf(stderr, "Error:Line %d: Command %c contains character, remove it before running the program again\n", line+1, commands[line][0]);
						return 1;
					}
					if (GetNumber(commands[line]) > i)
					{
						fprintf(stderr, "Error:Line %d: Command %c got incorrect argument %d as file with commands contains only %d line(s)\n", line+1, commands[line][0], GetNumber(commands[line]), i+1);
						return 1;
					}
					else
					{
						line = GetNumber(commands[line]) - 2;
					}
					break;

				default:
					fprintf(stderr, "Error:Line %d: Unknown command %s, remove it before running the program again\n", line+1, commands[line]);
					return 1;		
			}
			edit = 1;	
		}
		memset(input, 0, strlen(input));		//resets variable input in case the next input is shorter than the last one
		if (cycle > 0)
		{
			cycle--;
		}
	}
	return 0;
}