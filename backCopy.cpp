/* 
 * =================================================================
 * APPLICATION NAME:
 * Back Copy
 * 
 * AUTHOR NAME:
 * Nac
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

string getDateS()
{
	//Get todays date from functions and return it as a string
	string date(to_string(getDay()) + "-" + to_string(getMonth()) + "-" + to_string(getYear()));
	return date;
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
	string localFolder;
	string targetFolder;
	//Get today's date and date from one month ago
	string todaysDate(getDateS());
	string purge(getDateRemove());
	//Setting values for DIR variables
	localFolder = "./";
	targetFolder = "testout/";
	//Check if current date already has DIR, remove if so
	//This prevents clutter and 'File exists' errors
	if(fs::exists(localFolder + targetFolder + todaysDate))
	{
		fs::remove_all(localFolder + targetFolder + todaysDate);
	}
	//Create directory with date
	fs::create_directory(localFolder + targetFolder + todaysDate);
	//Copy files to backup DIR
	fs::copy("./testin/", localFolder + targetFolder + todaysDate, fs::copy_options::recursive);
	cout << "Current date is " << todaysDate << endl;
	cout << "Removing backups from " << purge << " and earlier." << endl;
	//Check if there is a DIR that is 1 month old
	//Might move the date from a hard coded val to a config val
	if(fs::exists((localFolder + targetFolder + purge).c_str()))
	{
		//If folder from one month ago exists, delete it
		fs::remove_all((localFolder + targetFolder + purge).c_str());
	}
	else
	{
		//If folder from one month ago doesn't exist, notify user
		cout << "Folder with name " << purge << " does not exist in target DIR, nothing deleted." << endl;
	}
	return 0;
}
