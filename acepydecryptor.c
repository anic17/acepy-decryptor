#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

char *strncpy_n(char *dest, const char *src, size_t count)
{
	// Better version that strncpy() because it always null terminates strings

	if (count)
	{
		memset(dest, 0, count);
		strncat(dest, src, count);
		return dest;
	}
	return NULL;
}

char *RemoveQuotes(char *dest, char *src)
{
	if (src[0] == '\"' && src[strlen(src) - 1] == '\"')
	{
		strncpy_n(dest, src + 1, strlen(src) - 2);
	}
	else
	{
		strncpy_n(dest, src, strlen(src));
	}
	return dest;
}

char *remove_extension(char *myStr) // https://stackoverflow.com/a/2736841
{
	char *retStr;
	char *lastExt;
	if (myStr == NULL)
		return NULL;
	if ((retStr = malloc(strlen(myStr) + 1)) == NULL)
		return NULL;
	strcpy(retStr, myStr);
	lastExt = strrchr(retStr, '.');
	if (lastExt != NULL)
		*lastExt = '\0';
	return retStr;
}

char *decrypt_string(char *s, char *key, size_t len)
{
	size_t keylen = strlen(key);
	for (size_t i = 0; i < len; i++)
	{
		s[i] ^= key[i % keylen]; // Decrypt using XOR
	}
}

int decrypt_file(char *file_name, char *key, char *original_name)
{
	FILE *file = fopen(file_name, "rb");
	if (!file)
	{
		perror("fopen()");
		return 0;
	}
	fseek(file, 0, SEEK_END);
	long file_size = ftell(file);
	fseek(file, 0, SEEK_SET);
	char *file_content = calloc(sizeof(char), file_size + 1);
	fread(file_content, file_size, 1, file);

	decrypt_string(file_content, key, file_size); // Decrypt the file

	freopen(original_name, "wb", file);
	fwrite(file_content, file_size, 1, file); // Write the decrypted file
	fclose(file);
	return 1;
}

int file_exists(char *filename)
{
	return _access(filename, 0) == 0;
}

int QuitProgram(int retval, int argc)
{
	if (argc < 2)
	{
		getchar();
	}
	exit(retval);
}

int main(int argc, char *argv[])
{
	char buf[MAX_PATH] = {0};
	char *buf_no_ext = {0};
	FILE *fp_file, fp_decrypt;
	printf("Acepy ransomware decryptor.\n\nThis tool is made to decrypt all files accidentally encrypted by Acepy with the extension '.acepy'.\nAcepy was meant as a Proof-of-Concept ransomware to learn about how modern ransomwares work.\n\n");

	if (argc > 1)
	{
		strncat(buf, argv[1], MAX_PATH);
		printf("Using folder path '%s'\n", buf);
	}
	else
	{
		printf("Folder containing encrypted files: ");
		fgets(buf, sizeof buf, stdin);
		buf[strcspn(buf, "\n")] = 0;
	}

	RemoveQuotes(buf, strdup(buf));

	if (!SetCurrentDirectory(buf))
	{
		fprintf(stderr, "Cannot change the current working directory.\nMake sure it is a valid path and you have the required permissions. (%d)\n", GetLastError());
		QuitProgram(1, argc);
	}
	size_t decrypt_count = 0, encrypted_count = 0;
	DWORD gle = 0;
	WIN32_FIND_DATA hFindData;
	HANDLE hFind = FindFirstFile("*.acepy", &hFindData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		gle = GetLastError();
		if (gle == 2)
		{
			fprintf(stderr, "This directory contains no '.acepy' files!\n");
			QuitProgram(1, argc);
		}
		fprintf(stderr, "Failed to get find data handle (%d)\n", gle);
		QuitProgram(1, argc);
	}
	else
	{
		printf("Starting decryption of '.acepy' files...\n");
	}
	do
	{
		if (hFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) // Don't decrypt directories
		{
			continue;
		}
		buf_no_ext = remove_extension(hFindData.cFileName);
		decrypt_file(hFindData.cFileName, "NewtroditIsTheBest", buf_no_ext); // This was the hard-coded key used by Acepy ransomware

		if (file_exists(buf_no_ext))
		{
			printf("Decrypted '%s' to '%s'\n", hFindData.cFileName, buf_no_ext);
			DeleteFileA(hFindData.cFileName);
			decrypt_count++;
		}
		else
		{
			printf("Failed to decrypt '%s'\n", hFindData.cFileName, buf_no_ext);
		}
		encrypted_count++;

	} while (FindNextFile(hFind, &hFindData));
	printf("Decrypted %d files out of %d encrypted files by Acepy Ransomware\n\nPress <ENTER> to quit...\n", decrypt_count, encrypted_count);
	getchar();
	return decrypt_count;
}