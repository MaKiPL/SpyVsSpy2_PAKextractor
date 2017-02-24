#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>
#include <direct.h>

const char* path = "D:\\SPYSPY\\DATA.PAK"; //CHANGE ME
const char* outppath = "D:\\SPYSPY\\OUT\\"; //CHANGE ME
const unsigned int SECTOR_SIZE = 2048;
int GetDirectory(char* filename);

int main()
{
	FILE* f = fopen(path, "rb");
	unsigned int Fnct = 0;
	fread(&Fnct, sizeof(int), 1, f);
	unsigned int buffer = 0;
	fread(&buffer, sizeof(int), 1, f);
	fseek(f, (long)(buffer*SECTOR_SIZE), SEEK_SET);
	fseek(f, 96, SEEK_CUR); //logical pass
	char* myChar = malloc(256);
	int index = 0;
	int FSindex = 0;
	buffer = 0;
	for (int i = 0; i < Fnct; i++)
	{
		memset(myChar, 0x00, 256);
		buffer = 0;
		while (1)
		{
			char c = 0x00;
			fread(&c, 1, 1, f);
			if (c == 0x00)
				break;
			*(myChar + index++) = c;
		}
		fseek(f, 48-(index+1), SEEK_CUR);
		fseek(f, -8, SEEK_CUR);
		fread(&buffer, sizeof(unsigned int), 1, f);
		int sizebuf = 0;
		fread(&sizebuf, 4, 1, f);
		long storeLoc = ftell(f);
		GetDirectory(myChar);
		//while (1)
		//{
		//	if (*(myChar + FilenamePos + index) == 0x00 || *(myChar + FilenamePos + index) == EOF)
		//		break;
		//	c[index] = *(myChar + FilenamePos + index);
		//	index++;
		//}
		char AbsolutePath[512];
		sprintf(AbsolutePath, "%s%s", outppath, myChar);
		FILE* savF = fopen(AbsolutePath, "ab");
		fseek(f, buffer*SECTOR_SIZE, SEEK_SET);
		for (int i = 0; i < sizebuf; i++)
		{
			unsigned char byte = 0;
			fread(&byte, 1, 1, f);
			fwrite(&byte, 1, 1, savF);
		}
		fclose(savF);
		printf("%d:\t%s Unpacked!\n", i, myChar);
		fseek(f, storeLoc, SEEK_SET);
		index = 0;
	}

	return 0;
}

int GetDirectory(char * filename)
{
	char c[512] = { 0x00 };
	char d[256] = { 0x00 };
	int index = 1;
	int length = 0;
	while (1)
		if (*(filename + index) == 0x00 || *(filename + index) == EOF)
			break;
		else
		{
			length++;
			index++;
		}
	index = 0;
	while (1)
	{
		char cc = *(filename + length - 1 - index);
		if (cc == '/')
			break;
		else 
			index++;
	}

	for (int i = 0; i < length - index; i++)
		d[i] = *(filename + i);
	sprintf(c,"%s%s", outppath, d);
	_mkdir(c);
	return index;
}

