/*	
 * =====================================================================================
 *
 *       Filename:  PhBr.cpp
 *
 *    Description:  Main file for PhBr 
 *
 *        Version:  0.1
 *        Created:  23/12/10
 *       Compiler:  clang++
 *
 *         Author:  code2828
 *   Organization:  Tesserium
 *
 * =====================================================================================
 */
#include<iostream>
#include<iomanip>
#include<vector>
#include<fstream>
#include<cmath>
#include"json.hpp"
#define decimal long double
#define PI ((long double)(3.1415926535897932384))
using namespace std;
using json = nlohmann::json;

struct Dir
{
	int serial;
	string name;
	int nex;
	int nex_dir;
	int dis;
};

struct Station
{
	int serial;
	string id;
	int dir_num;
	Dir dirs[100];
	char dir_comp[100][100] = {""};
};

vector<Station> v;

decimal _basic_v(decimal x, decimal x00, decimal y00)
{
	return y00/2*sin(PI/x00*x-PI/2)+y00/2;
}

decimal vv(decimal t, json tr, int len)
{
	decimal y00=tr.value("maximum_velocity",0.0);
	y00=min(y00,len/(decimal)2.0);
	decimal x00=PI*y00/2/tr.value("maximum_acceleration",0.0);
	decimal total_time=len/y00-2+2*x00;
	if(t>=total_time || t<=0)return 0;
	else if(t>=total_time/2)t=total_time-t;
	if(t<=x00) return _basic_v(t,x00,y00);
	else return y00;
}

decimal place(decimal t, json tr, int len, bool only_get_time=false)
{
	if(len<=0)return 0;
	decimal y00=tr.value("maximum_velocity",0.0);
    decimal x00=PI*y00/2/tr.value("maximum_acceleration",0.0);
    if(x00*y00>len)
	{
		y00=sqrt(2*len*tr.value("maximum_acceleration",0.0)/PI);
		x00=len/y00;
	}
	decimal total_time=len/y00-2+x00;
	if(only_get_time)return total_time;
    if(t>=total_time)return len;
	else if(t<=0) return 0;
	if(t<=x00)return y00/2.0*t-y00*x00/2/PI*sin(PI/x00*t);	
	if(t>=total_time-x00)return (decimal)len-(y00/2.0*(total_time-t)-y00*x00/2/PI*sin(PI/x00*(total_time-t)));
	else return y00*(t-x00/2.0);
}

void read_stations()
{
	ifstream fin("dat/reality.stationlist");
	char c;
	while(fin.get(c))
	{
		if(c=='@')
		{
			Station new_stat;
			fin>>new_stat.serial>>new_stat.id>>new_stat.dir_num;
			for(int i=0;i<new_stat.dir_num;i++)
			{
				fin>>new_stat.dirs[i].serial>>new_stat.dirs[i].name>>new_stat.dirs[i].nex>>new_stat.dirs[i].nex_dir>>new_stat.dirs[i].dis;
				if(i!=new_stat.dirs[i].serial)throw("Serial number does not match index!!\n");
			}
			for(int j=0;j<new_stat.dir_num;j++)
			{
				for(int k=0;k<new_stat.dir_num;k++)
				{
					fin>>new_stat.dir_comp[j][k];
				}
			}
			v.push_back(new_stat);
		}
	}
}

bool used[100]={0};
string find_path(int st, int en)
{
	if(st>=v.size())
	{
		cout<<"st is too big!\n";
		return"";
	}
	if(en>=v.size())
	{
		cout<<"en is too big!\n";
		return "";
	}
	cout<<v[st].id<<"到"<<v[en].id<<endl;
	if(st==en)
	{
		cout<<"=====>\t好！\n";
		return v[st].id;
	}
	used[st]=1;
	for(int i=0;i<v[st].dir_num;i++)
	{
		if(v[st].dirs[i].dis>0 && !used[v[st].dirs[i].nex])
		{
			string ret=find_path(v[st].dirs[i].nex,en);
			if(ret!="")
			{
				ret.insert(0,"->");
				ret.insert(0,v[st].id);
				return ret;
			}
		}
	}
	used[st]=0;
	return "";
}

/*
void generate_simple_timetable(json line, json tr, pair<int,int> starting_time)
{
	pair<int,int> curtime=starting_time;int day=0;
    int last_meter=0;
    int quang=73;
    for(json::iterator it=line["stations"].begin(); it!=line["stations"].end(); it++)
    {
        if(rand()%100<quang&&it!=line["stations"].begin()&&(it+1)!=line["stations"].end())continue;
        decimal addtime=place(0,tr,(*it).value("meter",0)-last_meter,true);
        addtime/=60;
        curtime.second+=ceil(addtime);
        while(curtime.second>=60)curtime.second-=60,curtime.first++;
        while(curtime.first>=24)curtime.first-=24,day++;
		string pp=(*it).value("name","zero");
        cout<<pp;
		for(int j=pp.length()/3*2;j<=11;j++)cout<<' ';
		cout<<'+'<<day<<' '<<curtime.first<<':'<<curtime.second<<' ';
        curtime.second+=2;
        while(curtime.second>=60)curtime.second-=60,curtime.first++;
        cout<<'+'<<day<<' '<<curtime.first<<':'<<curtime.second<<'\n';
        last_meter=(*it).value("meter",0);
    }
}*/

int main()
{
	srand((unsigned)time(0));
	read_stations();
	cout<<find_path(1,4)<<endl;

	return 0;
}
