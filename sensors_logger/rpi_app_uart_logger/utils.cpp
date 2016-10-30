

#include "utils.hpp"
#include <time.h>

//for file
#include <fstream>
#include <iostream>

using namespace std;

std::string TakeParseToLast(std::string &str,char sep)
{
	size_t first = str.find_last_of(sep);
	std::string Parsed = str.substr(0 , first);
	str = str.substr(first+1 ,str.length());
	return Parsed;
}

//take what's left if no separator found 
//important for robust parsing when lat delimiter is forgotten
std::string TakeParseTo(std::string &str,char sep)
{
	std::string Parsed;
	size_t first = str.find_first_of(sep);
	if(first != string::npos)
	{
		Parsed = str.substr(0 , first);
		str = str.substr(first+1 ,str.length());
	}
	else//not found, then take what's left
	{
		Parsed = str;
		str = "";
	}
	return Parsed;
}

int char2int(char input)
{
	if(input >= '0' && input <= '9')
		return input - '0';
	if(input >= 'A' && input <= 'F')
		return input - 'A' + 10;
	if(input >= 'a' && input <= 'f')
		return input - 'a' + 10;
}

void utl::hextext2data(const std::string &str, uint8_t *data)
{
	int i=0;
	while(i<str.length()-1)
	{
		*(data++) = char2int(str[i])*16 + char2int(str[i+1]);
		i += 2;
	}
}

std::string utl::remove_spaces(std::string &str)
{
	//remove_if not found
	//str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
	std::string res;
    for(int i=0; i<str.size(); i++)
	{
		if(str[i]!=' ')
			res+=str[i];
	}
	str = res;
    return res;	
}

std::string utl::remove_0x(std::string &str)
{
	//remove_if not found
	//str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
	std::string res;
    for(int i=0; i<str.size()-1; i++)
	{
		if((str[i]=='0') && (str[i+1]=='x'))
		{
			//not only skip, but skip two characters by incrementing the counter
			i++;
		}
		else
		{
			res+=str[i];			
		}
	}
	str = res;
    return res;	
}

void utl::remove(const std::string &substr, std::string &str)
{
	/*
	std::string resstr;
	size_t l = substr.length();

	size_t pos = str.find_first_of(substr);
	while(pos != string::npos)
	{
		resstr += str.substr(0 , first);
		resstr += str.substr(first+substr.length() ,str.length());
		pos = str.find_first_of(substr,pos+1);
	}
	*/
}

std::string utl::ParseRemTill(std::string &str,char sep,bool &found)
{
	size_t first = str.find_first_of(sep);
	if(first!=std::string::npos)
	{
		found = true;
	}
	else
	{
		found = false;
	}
	std::string Parsed = str.substr(0 , first);
	str = str.substr(first+1 ,str.length());
	return Parsed;
}

//here the parsed line should be as follows
//var1:valx;var3:valy;
//note that a safety limitation restricts the number of params to max 20
void utl::str2map(const std::string &str ,strmap &params)
{
	int max = 0;
	std::string vstr = str;
	//DBG_CMT std::cout << "START with : " << vstr << std::endl;
	while((!vstr.empty()) && (max < 20))
	{
		//DBG_CMT std::cout << "LOOP with : " << vstr << std::endl;
		std::string arg_name = TakeParseTo(vstr,':');
		params[arg_name] = TakeParseTo(vstr,';');
		max++;
	}
	//DBG_CMT std::cout << "DONE with : " << vstr << std::endl;
}

void utl::args2map( int argc, char** argv ,strmap &params)
{
	std::string exepath(*argv);//placed here before pointer is shifted
	while(argc--)
	{
		std::string argv_str(*argv++);
		//to debug parameters list
		//std::cout << "[" << argv_str << "]" << std::endl;
		std::string arg_name = TakeParseTo(argv_str,'=');
		params[arg_name] = argv_str;
	}
	//once all params in cmd line parsed check if a config file is available
	std::ifstream 	configfile;
	if(!utl::exists(params,"configfile"))//the parameter 'configfile' was not passed, so check the default one
	{
		exepath = TakeParseToLast(exepath,'/');
		params["configfile"] = exepath + "/configfile.txt";
	}
	configfile.open(params["configfile"].c_str());
	if(configfile.is_open())
	{
		std::cout << "configfile = "<< params["configfile"] << std::endl;
		string line;
		while ( getline (configfile,line) )
		{
			line = TakeParseTo(line,'#');//allow comment character
			std::string arg_name = TakeParseTo(line,'=');
			if(!utl::exists(params,arg_name))//if the new parameter from the file was not already provided in cmd line
			{
			params[arg_name] = TakeParseTo(line,'\r');//take the windows line ending out
			}//else ignore it and keep precedence for cmd line
			else
			{
				std::cout << arg_name << " => command line parameter takes over definition in file" << std::endl;
			}
		}
	}
	else
	{
		std::cout << "No Config file in cmd line and not found in exe dir: " <<  params["configfile"] << endl;
	}
	
}

bool utl::exists(const strmap &params,const std::string param)
{
	return (params.find(param) != params.end());
}

std::string	utl::getTime()
{
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time (&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer,80,"%Hh%Mmn%Ss",timeinfo);
	std::string str(buffer);
	return str;	
}
std::string	utl::getDay()
{
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time (&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer,80,"%Y.%m.%d",timeinfo);
	std::string str(buffer);
	return str;	
}
