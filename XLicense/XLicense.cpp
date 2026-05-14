// XLicense.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//
#include <map>
#include <algorithm>
#include <vector>
#include <ctime>
#include <iostream>
#include "license.h"
#include <afx.h>
#include <stdio.h>
#include <tchar.h>
#include <conio.h>


// XLicence.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Das einzige Anwendungsobjekt


using namespace std;


using CommandList = std::map<char, std::string>;


#define MAKELENGTH(a) (2*(2*36+(a)+4))
#define GETLENGTH(a)  ((a)/2-4-2*36)
#define PASSWORD	"EC220704"


void clearbuffer()
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF) {}
}

void wait()
{
	printf(("\n\n*** Press enter to continue !!!***\n"));
	clearbuffer();
	getchar();
}


void clrscr()
{
	system("@cls||clear");
}


static void LicFile(const std::string& path)
{
	std::string filepath{ path };
	if (filepath.empty())
	{
		filepath = ".";
	}
	if (license::CreateLicFile(filepath) != L_OK)
	{
		printf(("\nError creating Licfile !!!\n"));
	}
	else
	{
		printf(("\nLicfile successfully created !"));
	}
}

static void DemoLicense(const std::string& path)
{
	std::string filepath{ path };
	if (filepath.empty())
	{
		filepath = ".";
	}
	if (license::CreateLicFile(filepath) != L_OK)
	{
		printf(("\nError creating Licfile !!!\n"));
		return;
	}
	printf(("\nLicfile successfully created !"));


}


static inline BOOL shift(const std::string& line)
{
	BOOL result = (line.size() >= 8);
	for (int32_t i = 0; i < line.size(); i++)
	{
		const int c = int(line[i]);
		switch (i)
		{
		case 0: if (result) result = c == int('E');
			break;
		case 1: if (result) result = c == int('C');
			break;
		case 2: if (result) result = c == int('2');
			break;
		case 3: if (result) result = c == int('2');
			break;
		case 4: if (result) result = c == int('0');
			break;
		case 5: if (result) result = c == int('7');
			break;
		case 6: if (result) result = c == int('0');
			break;
		case 7: if (result) result = c == int('4');
			break;
		}
	}
	return result;
}



void LicenceFile()
{
	char c;

	printf(("!!! Please remember to create lic-file first !!!\n\n"));
	while (TRUE)
	{
		clrscr();
		printf(("***************************************\n"));
		printf(("***                                 ***\n"));
		printf(("***  X-LicenceManager V.4.0         ***\n"));
		printf(("***  Copyright 2002 by Detlef Hafer ***\n"));
		printf(("***                                 ***\n"));
		printf(("***************************************\n\n\n\n"));
		printf(("Please select ... \n"));
		printf(("_______________________________________\n\n"));
		printf(("   1 : Create easyControl licence \n"));
		printf(("   2 : Create easyGraph licence \n"));
		printf(("   3 : exit menu \n"));

		LICENCETYPE iLicence = E_EASYCONTROL;

		cin >> c;
		switch (c)
		{
		case '1':  iLicence = E_EASYCONTROL;
			break;
		case '2':  iLicence = E_EASYGRAPH;
			break;
		default:   return;
		}

		char str[50];
		str[0] = 'P';
		str[1] = 'a';
		str[2] = 's';
		str[3] = 's';
		str[4] = 'w';
		str[5] = 'o';
		str[6] = 'r';
		str[7] = 'd';
		str[8] = ' ';
		str[9] = ':';
		str[10] = ' ';
		str[11] = '\0';

		printf("\n%s", str);

		/* Passworteingabe */
		std::string line;
		clearbuffer();
		getline(cin, line);

		auto result = shift(line);
		if (!result) 
		{
			printf(("\nUngültiges Passwort ! "));
			wait();
			continue;
		}

		printf(("Tage : "));
		long lDays;
		cin >> lDays;

		CTime aEndTime = CTime::GetCurrentTime();
		if (lDays == 0)
		{
			aEndTime = CTime(2099, 12, 31, 0, 0, 0);
		}
		else
		{
			aEndTime += CTimeSpan(lDays, 0, 0, 0);
		}

		if (license::CreateLicenceFile(".", iLicence, aEndTime.GetTime()) != L_OK)
		{
			printf(("\nFehler !!!\n"));
			wait();
			continue;
		}
		printf(("\nLicence-file successfully created !"));
		wait();
		break;;
	}
}




void Menu()
{
	char c;
	bool bDone = false;

	while (!bDone)
	{
		clrscr();
		printf(("***************************************\n"));
		printf(("***                                 ***\n"));
		printf(("***  X-LicenceManager V.2.0         ***\n"));
		printf(("***  Copyright 2022 by Detlef Hafer ***\n"));
		printf(("***                                 ***\n"));
		printf(("***************************************\n\n\n\n"));
		printf(("Please select ... \n"));
		printf(("_______________________________________\n\n"));
		printf(("   1 : Create lic   - file \n"));
		printf(("   2 : Create licence- file\n"));
		printf(("   3 : Check licence- file\n"));
		printf(("   4 : Exit program\n"));

		cin >> c;
		switch (c)
		{
		case '1':
		{
			LicFile("");
			wait();
		}
		break;

		case '2':
		{
			LicenceFile();
		}
		break;

		case '3':
		{
			LICENCETYPE iLicence = E_EASYCONTROL;
			time_t t = time(NULL);
			CString szLicense;
			auto result = license::CheckLicenceFile(".", E_EASYCONTROL, t);
			if (result == L_OK)
			{
				iLicence = E_EASYCONTROL;
				szLicense = "EASYCONTROL";
			}
			else
			{
				result = license::CheckLicenceFile(".", E_EASYGRAPH, t);
				if (result == L_OK)
				{
					iLicence = E_EASYGRAPH;
					szLicense = "EASYGRAPH";
				}
			}
			if (result == L_OK)
			{
				CTime aEndTime(t);
				CString sz = aEndTime.Format("%A, %B %d, %Y");
				printf("\nLicense = %s, Time left to expire : %s\n", LPCSTR(szLicense), LPCSTR(sz));
			}
			else
			{
				printf("\nNo valid license !");
			}
			wait();
		}
		break;

		case '4':
		{
			bDone = true;
		}
		break;
		}
	}
}



static CommandList CreateCommandList(int argc, char** argv)
{
	char cmd = 0;
	std::string attribute;
	CommandList CmdList;

	for (int k = 1; k < argc; k++)
	{
		if (strlen(argv[k]) > 1)
		{
			char b = argv[k][0];
			if ((b == '-') || (b == '/'))
			{
				if ( cmd)
				{
					CmdList.insert({ cmd, attribute });
					attribute = "";
				}
				cmd = toupper(argv[k][1]);
			}
			else
			{
				attribute = std::string(argv[k]);
			}
		}
	}
	if (cmd)
	{
		CmdList.insert({ cmd, attribute });
	}
	return CmdList;
}


static void PrintCommandList(const CommandList& rMap)
{
	printf("\n**** CommandList ***");
	printf("\n--------------------");
	for (const auto& rItem : rMap)
	{
		printf("\n%c - %s", rItem.first, rItem.second.c_str());
	}
}


static BOOL ParseCommandList(const CommandList& rMap)
{
	BOOL result = FALSE;

	std::string path;
	auto it = rMap.find('P');
	if (it != rMap.end())
	{
		path = it->second;
	}

	LICENCETYPE licenseType = LICENCETYPE::E_EASYCONTROL;
	it = rMap.find('T');
	if (it != rMap.end())
	{
		std::string str{ it->second };
		std::transform(str.begin(), str.end(), str.begin(), ::toupper);
		if (str == "EASYGRAPH")
		{
			licenseType = LICENCETYPE::E_EASYGRAPH;
		}
	}
	it = rMap.find('L');
	if (it != rMap.end())
	{
		// LicFile
		LicFile(path);
		result = TRUE;
	}
	else
	{
		it = rMap.find('D');
		if (it != rMap.end())
		{
			// // 30 tage Demo
			LicFile(path);
			CTime aEndTime = CTime::GetCurrentTime();
			aEndTime += CTimeSpan(30, 0, 0, 0);

			result = (license::CreateLicenceFile(path, licenseType, aEndTime.GetTime()) == L_OK);
			if (! result)
			{
				printf("\nError creating demo !");
			}
		}
	}
	return result;
}


int main(int argc, char** argv) 
{
	if (argc > 1)
	{
		auto cmdlist = CreateCommandList(argc, argv);
		if (!cmdlist.empty())
		{
			PrintCommandList(cmdlist);
			ParseCommandList(cmdlist);
		}
	}
	else
	{
		Menu();
	}
	return 0;
}

