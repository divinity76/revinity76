//////////////////////////////////////////////////////////////////////
// OpenTibia - an opensource roleplaying game
//////////////////////////////////////////////////////////////////////
// Various functions.
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include <string>
#include <cmath>
#include <sstream>

bool fileExists(char* filename)
{
	FILE *f = fopen(filename, "rb");
	bool exists = (f != NULL);
	if (f != NULL)
		fclose(f);

	return exists;
}

//////////////////////////////////////////////////
// get a random value between lowest_number and highest_number
int random_range(int lowest_number, int highest_number)
{
	if(lowest_number > highest_number){
		int nTmp = highest_number;
		highest_number = lowest_number;
		lowest_number = nTmp;
    }

    double range = highest_number - lowest_number + 1;
    return lowest_number + int(range * rand()/(RAND_MAX + 1.0));
}

//////////////////////////////////////////////////
// dump a part of the memory to stderr.
void hexdump(unsigned char *_data, int _len) {
    int i;
    for (; _len > 0; _data += 16, _len -= 16) {
        for (i = 0; i < 16 && i < _len; i++)
            fprintf(stderr, "%02x ", _data[i]);
        for (; i < 16; i++)
            fprintf(stderr, "   ");
        fprintf(stderr, " ");
        for (i = 0; i < 16 && i < _len; i++)
            fprintf(stderr, "%c", (_data[i] & 0x70) < 32 ? '�' : _data[i]);
        fprintf(stderr, "\n");
    }
}

#if 0
//////////////////////////////////////////////////
// Enable asynchronous function calls.
// This function does not wait for return of the called function;
// instead, this function returns immediately.
// The called function must be of type void *fn(void *).
// You can use the pointer to the function for anything you want to.
// Return: a thread handle.
pthread_t *detach(void *(*_fn)(void *), void *_arg) {
    pthread_t *thread = new pthread_t();
    if (pthread_create(thread, NULL, _fn, _arg))
        perror("pthread");
    return thread;
}
#endif

//////////////////////////////////////////////////
// Upcase a char.
char upchar(char c) {
    if (c >= 'a' && c <= 'z')
        return c - 'a' + 'A';
    else if (c == '�')
        return '�';
    else if (c == '�')
        return '�';
    else if (c == '�')
        return '�';
    else if (c == '�')
        return '�';
    else if (c == '�')
        return '�';
    else if (c == '�')
        return '�';
    else if (c == '�')
        return '�';
    else if (c == '�')
        return '�';
    else if (c == '�')
        return '�';
    else if (c == '�')
        return '�';
    else if (c == '�')
        return '�';
    else if (c == '�')
        return '�';
    else if (c == '�')
        return '�';
    else if (c == '�')
        return '�';
    else if (c == '�')
        return '�';
    else if (c == '�')
        return '�';
    else if (c == '�')
        return '�';
    else if (c == '�')
        return '�';
    else if (c == '�')
        return '�';
    else if (c == '�')
        return '�';
    else if (c == '�')
        return '�';
    else if (c == '�')
        return '�';
    else if (c == '�')
        return '�';
    else if (c == '�')
        return '�';
    else if (c == '�')
        return '�';
    else if (c == '�')
        return '�';
    else if (c == '�')
        return '�';
    else if (c == '�')
        return '�';
    else if (c == '�')
        return '�';
    else if (c == '�')
        return '�';
    else if (c == '�')
        return '�';
    else
        return c;
}

//////////////////////////////////////////////////
// Upcase a 0-terminated string.
void upper(char *upstr, char *str) {
    for (; *str; str++, upstr++)
        *upstr = upchar(*str);
    *upstr = '\0';
}


//////////////////////////////////////////////////
// Upcase a 0-terminated string, but at most n chars.
void upper(char *upstr, char *str, int n) {
    for (; *str && n; str++, upstr++, n--)
        *upstr = upchar(*str);
    if (n) *upstr = '\0';
}

int safe_atoi(const char* str)
{
	if (str)
		return atoi(str);
	else
		return 0;
}

double timer()
{
	static bool running = false;
	static _timeb start, end;

	if (!running)
	{
		_ftime(&start);
		running = true;
		return 0.0;
	}
	else
	{
		_ftime(&end);
		running = false;
		return (end.time-start.time)+(end.millitm-start.millitm)/1000.0;
	}
}

std::string article(const std::string& name)
{
	if (name.empty())
		return name;

	switch (upchar(name[0]))
	{
	case 'A':
	case 'E':
	case 'I':
	case 'O':
	case 'U':
		return std::string("an ") + name;
	default:
		return std::string("a ") + name;
	}
}

std::string str(int ticks)
{
	int hours = (int)floor(double(ticks)/(3600000.0));
	int minutes = (int)ceil((double(ticks) - double(hours)*3600000.0)/(60000.0));

	std::ostringstream info;
	info << hours << (hours==1? " hour " : " hours ") << minutes << (minutes==1? " minute" :" minutes");
	return info.str();
}
