/* 
 * =================================================================
 * APPLICATION NAME:
 * Back Copy
 * 
 * AUTHOR NAME:
 * Nac/Nalal/Noah
 * 
 * AUTHOR GROUP/CORP:
 * FTC Consulting
 * 
 * INIT DATE+TIME:
 * 13th of May, 2019, 6:15PM
 * 
 * =================================================================
 */
#include <ctime>
#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include <sstream>
#include <unistd.h> 
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
namespace fs = std::experimental::filesystem;

int getDay()
{
	//Get current day (number in month) and return it as an int
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[3];
	time (&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buffer,sizeof(buffer),"%d",timeinfo);
	int dayNum(stoi(buffer));
	return dayNum;
}

int getMonth()
{
	//Get current month and return it as an int
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[3];
	time (&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buffer,sizeof(buffer),"%m",timeinfo);
	int dayNum(stoi(buffer));
	return dayNum;
}

int getYear()
{
	//Get current year and return it as an int
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[3];
	time (&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buffer,sizeof(buffer),"%y",timeinfo);
	int dayNum(stoi(buffer));
	return dayNum;
}
//Get current date in dateform
string getDateS()
{
	//Get todays date from functions and return it as a string
	string date(to_string(getDay()) + "-" + to_string(getMonth()) + "-" + to_string(getYear()));
	return date;
}
//Rip dateform string into individual ints based on '-' spacers
int* getDateFromString(string s)
{
	int dateDayInt;
	int dateMonthInt;
	int dateYearInt;
	
	
	int dateSeg(0);
	string holder;
	for(int i = 0; i <= s.length(); i++)
	{
		if(isdigit(s[i]) and i != s.length())
		{
			//cout << "todaysDate[i] is " << s[i] << endl;
			if(dateSeg == 0)
			{
				holder = holder + s[i];
			}
			else if(dateSeg == 1)
			{
				holder = holder + s[i];
			}
			else
			{
				holder = holder + s[i];
			}
		}
		else
		{
			if(dateSeg <= 2)
			{
				if (dateSeg == 0)
				{
					dateDayInt = stoi(holder);
				}
				else if (dateSeg == 1)
				{
					dateMonthInt = stoi(holder);
				}
				else if (dateSeg == 2)
				{
					dateYearInt = stoi(holder);
				}
				holder = "";
				dateSeg++;
			}
			else
			{
				dateSeg = 0;
			}
		}
	
	}
	int* segVals = new int[3];
	segVals[1] = dateDayInt;
	segVals[2] = dateMonthInt;
	segVals[3] =  dateYearInt;
	return segVals;
}
//Check if name is in proper date format
bool checkName(string s)
{
	if(s.length() > 6 and s.length() < 9)
	{
		for(int i = 0; i <= s.length() - 1; i++)
		{
			//cout << "i is " << s[i] << endl;
			if(!isdigit(s[i]) and s.find("-") == string::npos )
			{
				return false;
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}
//Madlad Standard C FileIO opperation to get UID for origin file
int getFSUID(string s)
{
	int ret;
	struct stat sb;
	stat(s.c_str(), &sb);
	ret = sb.st_uid;
	return ret;
}

string getDateRemove()
{
	//Get current date
	int iDay(getDay());
	int iMonth(getMonth());
	int iYear(getYear());
	//Catch if last month was in the previous year
	if(iMonth == 1)
	{
		iMonth = 12;
		iYear = iYear - 1;
	}
	else
	{
		iMonth = iMonth - 1;
	}
	//Return date to delete
	string removeDate(to_string(iDay) + "-" + to_string(iMonth) +  "-" + to_string(iYear));
	return removeDate;
}

int main(int argc, char *argv[])
{
	//DIR variable init
	string destFolder;
	string targetFolder;
	bool clean(true);
	bool clearExists(true);
	//Get today's date and date from one month ago
	string todaysDate(getDateS());
	string purge(getDateRemove());
	//Setting values for DIR variables
	destFolder = "INVALID";
	targetFolder = "INVALID";
	
	int* dates = getDateFromString("19-19-19");
	cout << "Day is " << dates[1] << endl;
	cout << "Month is " << dates[2] << endl;
	cout << "Year is " << dates[3] << endl;
	
	//Check if required args are supplied
	if(argc == 3)
	{
		destFolder = argv[1];
		targetFolder = argv[2];
		cout << "No args given, runing standard backup." << endl;
	}
	//Get args if there are more than 3 (C++ counts the line to run the program as an arg)
	else if(argc > 3)
	{
		destFolder = argv[1];
		targetFolder = argv[2];
		for(int a = 3; a <= argc; a++)
		{
			cout << "argv[a] is " << argv[a] << endl;
			if(argv[a] == "noclean")
			{ 
				clean = false;
				cout << "noclean given, will skip deleting past date backups." << endl;
			}
			else if(argv[a] == "safe")
			{
				clearExists = false;
				cout << "safe given, skipipng backup if current date folder exist in destFolder." << endl;
			}
			else
			{
				cout << argv[a] << " is an invalid argument, this may be a data entry error, halting for safety." << endl;
				return 0;
			}
		}
	}
	//If an insufficient ammount of args is given, halt the program.
	else
	{
		cout << "Not enough arguments supplied, terminating." << endl;
		return 0;
	}
	
	//Print directories to console
	cout << "Copying files from " << targetFolder << endl;
	cout << "Copying files to " << destFolder << todaysDate << endl;	
	
	//Check if given folders actually exist
	if(!fs::exists(destFolder) or !fs::exists(targetFolder))
	{
		cout << "Destination or Target folder does not exist, terminating." << endl;
		return 0;
	}
	//Check if current date already has DIR, remove if clearExists is true
	//This prevents clutter and 'File exists' errors but also prevents wasted I/O clearing data
	if(fs::exists(destFolder + todaysDate) and clearExists)
	{
		cout << "Folder exists with name " << todaysDate << " and execution is not flagged safe, removing existing folder." << endl;
		fs::remove_all(destFolder + todaysDate);
	}
	else if (fs::exists(destFolder + todaysDate) and !clearExists)
	{
		cout << "Destination folder contains folder with name " << todaysDate << ", skipping copy." << endl;
	}
	
	//Create directory with date
	if(!fs::exists(destFolder + todaysDate))
	{
		fs::create_directory(destFolder + todaysDate);
		//Copy files to backup DIR
		fs::copy(targetFolder, destFolder + todaysDate, fs::copy_options::recursive);
		//Get origin folder UID
		int real = getFSUID(targetFolder);
		cout << "Origin UID is " << real << ", setting output files to match." << endl;
		//Set initial forlder to correct perms, then recurse into folder and sub folders doing the same
		chown((destFolder + todaysDate).c_str(), real, real);
		for(auto& p: fs::recursive_directory_iterator(destFolder + todaysDate))
		{
			chown(p.path().c_str(), real, real);
		}
	}
	
	cout << "Current date is " << todaysDate << endl;
	cout << "If permitted, removing backups from " << purge << " and earlier." << endl;
	
	//Check if there is a DIR that is 1 month old and check if clean is true
	//Might move the date from a hard coded val to a config val
	if(fs::exists((destFolder + purge).c_str()) and clean)
	{
		//If folder from one month ago exists, delete it
		for(auto& p: fs::directory_iterator(destFolder))
		{	
			string pathName = p.path().filename().string();
			cout << "p is " << p.path().stem() << endl;
			if (checkName(pathName))
			{
				cout << "p is " << p.path().stem() << endl;
				int* dates = getDateFromString(pathName);
				if(dates[2] >= 1)
				{
					if(dates[2] < getMonth())
					{
						fs::remove_all((destFolder + pathName).c_str());
					}
				}
				else if(dates[2] == 1 and dates[3] < getYear())
				{
					fs::remove_all((destFolder + pathName).c_str());
				}
			}
		}
		cout << "Backups from " << purge << " have been cleaned." << endl;
	}
	//If the folder exists but the noclean flag was given, it will do nothing
	else if(fs::exists((destFolder + purge).c_str()) and !clean)
	{
		cout << "noclean flag was used, skipping old file clean regardless of existance." << endl;
	}
	//If no folder older than 1 month exists, do cout and move to final return
	else
	{
		//If folder from one month ago doesn't exist, notify user
		cout << "Folder with name " << purge << " does not exist in target DIR, nothing deleted." << endl;
	}
	return 0;
}
