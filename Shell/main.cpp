#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>
using namespace std;

int convert(string q);
char* convert(int i);

int main()
{
	setlocale(LC_ALL, "rus");

	bool isActive = true;
	string command,s1,q;
	vector <string> nabor;
	chdir("/");

	while (isActive) 
	{
		//Получаем строку
		getline(cin, command);

		//Разбираем строку
		char *s=new char[command.length()+1];
		strcpy(s, command.c_str());
		char * pch = strtok( s , " ");
		while (pch != NULL) 
		{
			s1 = string(pch);
			nabor.insert(nabor.end(),s1);
			pch = strtok(NULL, " ");
		}
		free(pch);

		//Выбор действия
		s1 = nabor[0];
		switch (convert(s1)) 
		{
		//Передача системе неизвестной комманды
		case 0 :
		{
			system(s);
			nabor.clear();
			break;
		}
		//Комманда cd
		case 1: 
		{
			if (nabor.size() > 1) 
			{
				int i;
				char PathName[PATH_MAX];
				// Переменная, в которую буднт помещен указатель на PathName
				char *PN;
				// Определяем путь к текущей директории  
				PN = getwd(PathName);
				string q1 = string(PN);
				q = q1+"/"+nabor[1];
				char *path = new char[q.length() + 1];
				strcpy(path, q.c_str());
				if ((i=chdir(path))!=0)
				{
					fprintf(stderr, "Catalog is not exist\n");
					nabor.clear();
					break;
				}
				PN = getwd(PathName);
				printf("Directory : %s\n", PathName);
				nabor.clear();
				free(path);
			}
			else
			{
				fprintf(stdout, "Not enough arguments\n");
				nabor.clear();
			}
			break;
		}
		//Комманда dir
		case 2: 
		{
			DIR *dir;
			struct dirent *entry;

			// Буфер в который будет помещен путь
			// к текущей директории 
			char PathName[PATH_MAX];
			// Переменная, в которую буднт помещен указатель на PathName
			char *PN;
			// Определяем путь к текущей директории  
			PN = getwd(PathName);
			if (nabor.size() == 1)
			{
				// Вывод результата на консоль  
				if (PN == NULL) printf("Fail");
				else printf("Directory : %s\n", PathName);

				dir = opendir(PN);
				if (!dir) {
					perror("diropen");
					break;
				};

				while ((entry = readdir(dir)) != NULL) {
					printf("%s [%s]\n",entry->d_name, convert(entry->d_type));
				};
				closedir(dir);
				nabor.clear();
				break;
			}

			if (nabor.size() > 1) 
			{
				q = nabor[1];
				if(q==">>")
				{
					q = nabor[2];
					const char *s = q.c_str();
					FILE * file;
					file = fopen(s, "w+t");
					if (file == nullptr)
					{
						printf("Wrong file name or start catalog\n");
						nabor.clear();
						break;
					}
					else
					{
						fprintf(file, "Directory : %s\n", PathName);
						dir = opendir(PN);
						while ((entry = readdir(dir)) != NULL) {
							fprintf(file, "%s [%s]\n", entry->d_name, convert(entry->d_type));
						};
						closedir(dir);
						fclose(file);
						nabor.clear();
						break;
					}
				}
				else if (q == ">")
				{
					q = nabor[2];
					const char *s = q.c_str();
					FILE * file;
					file = fopen(s, "a+t");
					if (file == nullptr)
					{
						printf("Wrong file name or start catalog\n");
						nabor.clear();
						break;
					}
					else 
					{
					fprintf(file, "Directory : %s\n", PathName);
					dir = opendir(PN);
					while ((entry = readdir(dir)) != NULL) {
						fprintf(file, "%s [%s]\n", entry->d_name, convert(entry->d_type));
					};
					closedir(dir);
					fclose(file);
					nabor.clear();
					break;
					}
				}
				else 
				{
					printf("Wrong arguments\n");
					nabor.clear();
					break;
				}

			}
			nabor.clear();
			break;
		}
		//Комманда find
		case 3: 
		{

			DIR *dir;
			struct dirent *entry;

			char PathName[PATH_MAX];
			char *PN; 
			PN = getwd(PathName);
			if (nabor.size() == 2)
			{  
				//Строка для поиска
				q = nabor[1];
				char *path = new char[q.length() + 1];
				strcpy(path, q.c_str());
				//-----------------

				if (PN == NULL) printf("Fail");
				else printf("Directory : %s\n", PathName);

				dir = opendir(PN);
				if (!dir) {
					perror("diropen");
					break;
				};

				while ((entry = readdir(dir)) != NULL) 
				{
					string q1 = string(entry->d_name);
					if(q1.find(q)!=-1)
					{
						printf("%s [%s]\n", entry->d_name, convert(entry->d_type));
					}
					
				};
				closedir(dir);
				nabor.clear();
				break;
			}

			if (nabor.size() > 2)
			{
				q = nabor[2];
				if (q == ">>")
				{
					// Буфер в который будет помещен путь
			// к текущей директории 
					char PathName[PATH_MAX];
					// Переменная, в которую буднт помещен указатель на PathName
					char *PN;
					// Определяем путь к текущей директории  
					PN = getwd(PathName);

					s1 = nabor[3];
					const char *s = s1.c_str();
					FILE * file;
					file = fopen(s, "w+t");
					if (file == nullptr)
					{
						printf("Wrong file name or start catalog\n");
						nabor.clear();
						break;
					}

					q = nabor[1];
					char *path = new char[q.length() + 1];
					strcpy(path, q.c_str());

					dir = opendir(PN);
					if (!dir) {
						perror("diropen");
						break;
					};

					while ((entry = readdir(dir)) != NULL)
					{
						string q1 = string(entry->d_name);
						if (q1.find(q) != -1)
						{
							fprintf(file,"%s [%s]\n", entry->d_name, convert(entry->d_type));
						}

					};
					closedir(dir);
					fclose(file);
					nabor.clear();
					break;
					
				}
				else if (q == ">")
				{
					// Буфер в который будет помещен путь
			// к текущей директории 
					char PathName[PATH_MAX];
					// Переменная, в которую буднт помещен указатель на PathName
					char *PN;
					// Определяем путь к текущей директории  
					PN = getwd(PathName);

					s1 = nabor[3];
					const char *s = s1.c_str();
					FILE * file;
					file = fopen(s, "a+t");
					if (file == nullptr)
					{
						printf("Wrong file name or start catalog\n");
						nabor.clear();
						break;
					}

					q = nabor[1];
					char *path = new char[q.length() + 1];
					strcpy(path, q.c_str());

					dir = opendir(PN);
					if (!dir) {
						perror("diropen");
						break;
					};

					while ((entry = readdir(dir)) != NULL)
					{
						string q1 = string(entry->d_name);
						if (q1.find(q) != -1)
						{
							fprintf(file, "%s [%s]\n", entry->d_name, convert(entry->d_type));
						}

					};
					closedir(dir);
					fclose(file);
					nabor.clear();
					break;
				}
				else
				{
					printf("Wrong arguments\n");
					nabor.clear();
					break;
				}

			}
			nabor.clear();
			break;
		}
		//Комманда exit
		case 4:
		{
			isActive = false;
			nabor.clear();
			break;
		}
		}
		

		//isActive = false;
	}
    printf("hello from Shell!\n");
    return 0;
}

int convert(string q) 
{
	if (q == "cd") { return 1; }
	else if (q == "dir") { return 2; }
	else if (q == "find") { return 3; }
	else if (q == "exit") { return 4; }
	else { return 0; }
}

char* convert(int i) 
{
	if (i == 4) { return "Folder"; }
	else if (i == 8) { return "File"; }
	else return "XZ";
}