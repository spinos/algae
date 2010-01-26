#include "SHelper.h"

#include <stdlib.h>
#include <time.h>

void SHelper::divideByFirstSpace(std::string& ab2a, std::string& b)
{
	b = ab2a;
	int found = ab2a.find(' ', 0);
	if(found < 0) return;
		
	ab2a.erase(found);
	b.erase(0, found);
}

void SHelper::trimByFirstSpace(std::string& res)
{
	int found = res.find(' ', 0);
	if(found < 0) return;
		
	res.erase(found);
}

void SHelper::getTime(std::string& log)
{
	time_t rawtime;
	struct tm * timeinfo;
	
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	log = std::string(asctime(timeinfo));
}

void SHelper::cutByFirstDot(std::string& res)
{
	int found = res.find('.', 0);
	if(found < 0) return;
		
	res.erase(found);
}

void SHelper::cutByLastDot(std::string& res)
{
	int found = res.rfind('.', res.size());
	if(found < 0) return;
		
	res.erase(found);
}

void SHelper::cutByLastSlash(std::string& res)
{
	int found = res.rfind('\\', res.size());
	if(found > 1) 
	{
		found++;
		res.erase(found);
		return;
	}	
	
	found = res.rfind('/', res.size());
	if(found > 1) 
	{
		found++;
		res.erase(found);
	}
	return;
}

void SHelper::changeFrameNumber(std::string& res, int frame)
{
	int first = res.find('.', 0);
	if(first < 0) return;
		
	int last = res.rfind('.', res.size());
	if(last < 0) return;
	
	char mid[8];
	sprintf(mid, ".%d.", frame);
	
	res.erase(first, last-first+1);
	res.insert(first, mid);
}

int SHelper::safeConvertToInt(const double a)
{
	double b = (a*100.0 + 0.5)/100.0;
	return (int)b;
}

int SHelper::getFrameNumber(std::string& name)
{
	int first_dot = name.find_first_of('.', 0);
	first_dot++;
	int last_dot = name.find_last_of('.', name.size()-1);
	last_dot--;
	
	if(last_dot < first_dot) return 0;
	
	std::string sub = name.substr(first_dot, last_dot - first_dot + 1);
	
	return atoi(sub.c_str());
}

int SHelper::compareFilenameExtension(std::string& name, const char* other)
{
	int found = name.find_last_of('.', name.size()-1);
	if(found < 0) return 0;
	found++;
	if(name.compare(found, 6, std::string(other)) == 0) return 1;
	
	return 0;
}

void SHelper::setFrameNumberAndExtension(std::string& name, int frame, const char* extension)
{
	int found = name.find_first_of('.', 0);
	if(found > 0) name.erase(found);
		
	char buf[8];
	sprintf(buf, ".%d.", frame);
	name.append(buf);
	name.append(extension);
}

int SHelper::getFrameNumber(const std::string& name)
{
	std::string sbuf = name;
	int found = sbuf.find_last_of('.', sbuf.size()-1);
	if(found < 0) return 0;
	sbuf.erase(found);
	
	found = sbuf.find_last_of('.', sbuf.size()-1);
	if(found < 0) return 0;
	sbuf.erase(0, found);

	int res;
	sscanf(sbuf.c_str(), "%i", &res);
	return res;
}

void SHelper::changeFilenameExtension(std::string& name, const char* ext)
{
	int found = name.find_last_of('.', name.size()-1);
	if(found < 0) return;
	found++;
	
	name.erase(found);
	name.append(ext);
}

void SHelper::validateFilePath(std::string& name)
{
	std::string str(name);

	int found = str.find('|', 0);
	
	while(found>-1)
	{
		str[found] = '_';
		found = str.find('|', found);
	}
	
	found = str.find(':', 0);
	
	while(found>-1)
	{
		str[found] = '_';
		found = str.find(':', found);
	}
		
	name = std::
	
	
	string(str.c_str());
}

void SHelper::replacefilename(std::string& res, std::string& toreplace)
{
	int founddot;
	int foundlash = res.rfind('\\', res.size());
	if(foundlash > 1) 
	{
		foundlash++;
		founddot = res.find('.', foundlash);
		if(founddot > foundlash)
		{
			res.erase(foundlash, founddot-foundlash);
			res.insert(foundlash, toreplace);
		}
		return;
	}	
	
	foundlash = res.rfind('/', res.size());
	if(foundlash > 1) 
	{
		foundlash++;
		founddot = res.find('.', foundlash);
		if(founddot > foundlash)
		{
			res.erase(foundlash, founddot-foundlash);
			res.insert(foundlash, toreplace);
		}
	}
}

void SHelper::cutfilepath(std::string& res)
{
	int found = res.rfind('\\', res.size());
	if(found > 1) 
	{
		found++;
		res.erase(0, found);
		return;
	}	
	
	found = res.rfind('/', res.size());
	if(found > 1) 
	{
		found++;
		res.erase(0, found);
	}
}
#ifdef WIN32
#include <windows.h>
#endif

void SHelper::saveFloatFile(const char* filename, const int nfloat, const float* data)
{
	std::string tmpfile = filename;
#ifdef WIN32
	cutfilepath(tmpfile);
#endif	
	//std::cout<<" saving "<<tmpfile<<std::endl;
	
	std::ofstream ffile;
	ffile.open(tmpfile.c_str(), std::ios::out | std::ios::binary);
	if(!ffile.is_open()) return;

	ffile.write((char*)data, nfloat*sizeof(float));
	ffile.close();
#ifdef WIN32	
	Sleep(1999);
	
	CopyFile(tmpfile.c_str(),filename,0);
	
	Sleep(1999);
	
	tmpfile = "del " + tmpfile;
	
	system(tmpfile.c_str());
#endif	
	return;
}

void SHelper::changeFrameNumberFistDot4Digit(std::string& res, int frame)
{
	int first = res.find('.', 0);
	if(first < 0) return;
	
	char mid[8];
	if(frame<10) sprintf(mid, ".000%d.", frame);
	else if(frame<100) sprintf(mid, ".00%d.", frame);
	else if(frame<1000) sprintf(mid, ".0%d.", frame);
	else sprintf(mid, ".%d.", frame);
	
	res.erase(first, 6);
	res.insert(first, mid);
}
//:~
