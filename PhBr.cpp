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
	int rank;
	string id;
	int dir_num;
	Dir dirs[100];
	short dir_comp[100][100] = {{0}};
};
const Station stub = {-1,-1,"stub",0};

struct Train
{
	int serial;
	string name;
	decimal max_acc;
	decimal max_vel;
	bool distributed;
	bool highspeed_ok;	
};

inline bool cmp(Station _, Station __){return _.serial<__.serial;}

vector<Station> v;
vector<Train> trains;
map<string,int> stlist;
map<string,int> trlist;

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
			fin>>new_stat.serial>>new_stat.rank>>new_stat.id>>new_stat.dir_num;
			for(int i=0;i<new_stat.dir_num;i++)
			{
				fin>>new_stat.dirs[i].serial>>new_stat.dirs[i].name>>new_stat.dirs[i].nex>>new_stat.dirs[i].nex_dir>>new_stat.dirs[i].dis;
				for(int j=0;j<new_stat.dir_num;j++)
				{
					fin>>new_stat.dir_comp[i][j];
				}
			}
			while(v.size()<=new_stat.serial)
			{
				v.push_back(stub);
			}
			v[new_stat.serial]=new_stat;
			stlist.insert({new_stat.id,new_stat.serial});
		}
	}
}

void read_trains()
{
	ifstream fin("dat/reality.trainlist");
	char c;
	while(fin.get(c))
	{
		if(c=='@')
		{
			Train newtr;
			fin>>newtr.serial>>newtr.name>>newtr.max_acc>>newtr.max_vel>>newtr.distributed>>newtr.highspeed_ok;
			trains.push_back(newtr);
			trlist.insert({newtr.name,newtr.serial});
		}
	}
}

void find_path(int st, int en, int last_dir, vector<pair<int,bool> > curroute, vector<vector<pair<int,bool> > >& vec, bool* used)
{
	if(st>=v.size())
	{
		cout<<"st is too big!\n";
		return;
	}
	if(en>=v.size())
	{
		cout<<"en is too big!\n";
		return;
	}
	cout<<v[st].id<<"的"<<last_dir<<"到"<<v[en].id<<endl;
	if(st==en)
	{
		cout<<"=====>\t好！\n";
		curroute.push_back(pair<int,bool>(st,0));
		vec.push_back(curroute);
		curroute.pop_back();
		return;
	}
	used[st]=1;
	for(int i=0;i<v[st].dir_num;i++)
	{
		if(v[st].dirs[i].dis>0 && !used[v[st].dirs[i].nex])
		{
			bool needs_turning_around=0;
			if(last_dir>=0)
			{
				if(v[st].dir_comp[last_dir][i]==0 || v[st].dir_comp[last_dir][i]==9) continue;
				if(v[st].dir_comp[last_dir][i]==2) needs_turning_around=1;
			}
			curroute.push_back(pair<int,bool>(st,needs_turning_around));
			find_path(v[st].dirs[i].nex,en,v[st].dirs[i].nex_dir,curroute,vec,used);
			curroute.pop_back();
		}
	}
	used[st]=0;
	return;
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
	read_trains();
	string a,b,c;
	cout<<"从哪出发？（名称）  "<<flush;
	cin>>a;
	cout<<"到哪？（名称）  "<<flush;
	cin>>b;
	vector<vector<pair<int,bool> > > vec;
	bool used[100];
	vector<pair<int,bool> > curroute0;
	find_path(stlist.find(a)->second,stlist.find(b)->second,-05,curroute0,vec,used);
	for(vector<vector<pair<int,bool> > >::iterator it=vec.begin();it!=vec.end();it++)
	{
		for(vector<pair<int,bool> >::iterator jt=(*it).begin();jt!=(*it).end();jt++)
		{
			cout<<(jt!=(*it).begin()?"→":"")<<v[(*jt).first].id<<((*jt).second?"（调向）":"");
		}
		cout<<endl;
	}

	return 0;
}
