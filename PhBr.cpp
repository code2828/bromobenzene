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
#include<cmath>
#include"json.hpp"
#define decimal long double
#define PI ((long double)(3.1415926535897932384))
using namespace std;
using json = nlohmann::json;

json CR1J = { 
    {"ID", "CR1J"},
    {"maximum_acceleration", 0.2},
    {"maximum_velocity", 0.278}
};

json CR10J = {
    {"ID", "CR10J"},
    {"maximum_acceleration", 0.3},
    {"maximum_velocity", 3}
};

json CR200J = {
	{"ID", "CR200J"},
	{"maximum_acceleration", 0.41},
	{"maximum_velocity", 34.3}
};

json CR400AF = {
    {"ID", "CR400AF"},
    {"maximum_acceleration", 0.53},
    {"maximum_velocity", 100}
};

json CR1000Z = {
    {"ID", "CR1000Z"},
    {"maximum_acceleration", 1},
    {"maximum_velocity", 277.78}
};

json CRNAN = {
    {"ID", "CRNAN"},
    {"maximum_acceleration", 100},
    {"maximum_velocity", 100000}
};

json j = {
	{"ID", -1},
	{"name", "random"},
	{"stations", {{
		{"name", "长亘"},
        {"platforms",1},
        {"meter",0}
    },{
		{"name", "山城"},
        {"platforms",1},
        {"meter",10000}
    },{
		{"name", "高粱"},
        {"platforms",1},
        {"meter",15000}
    },{
		{"name", "黄恩东"},
        {"platforms",1},
        {"meter",20000}
    },{
		{"name", "开阳"},
        {"platforms",1},
        {"meter",34000}
    },{
		{"name", "白市西"},
        {"platforms",1},
        {"meter",45000}
    },{
		{"name", "繁城"},
        {"platforms",1},
        {"meter",51000}
    },{
		{"name", "凉水江西"},
        {"platforms",1},
        {"meter",58091}
    },{
		{"name", "朝栾西"},
        {"platforms",1},
        {"meter",69819}
    },{
		{"name", "冷爱南"},
        {"platforms",1},
        {"meter",78822}
    },{
		{"name", "北城"},
        {"platforms",1},
        {"meter",87324}
    },{
		{"name", "氢北南"},
        {"platforms",1},
        {"meter",93929}
    },{
		{"name", "南城"},
	    {"platforms",1},
	    {"meter",97182}
	},{
		{"name", "广台东"},
		{"platforms",1},
		{"meter",102434}
	},{
		{"name", "艳隆西"},
		{"platforms",1},
		{"meter",112343}
	},{
		{"name", "平佥龙骨"},
		{"platforms",1},
		{"meter",122516}
	},{
		{"name", "龙驿南川"},
		{"platforms",1},
		{"meter",135670}
	},{
		{"name", "千兰北"},
		{"platforms",1},
		{"meter",150067}
	},{
		{"name", "随机市北"},
        {"platforms",1},
        {"meter",8127383}
	}}}
};

json jingjiu = {
    {"ID", 2},
    {"name", "jingjiuxian"},
    {"stations", {{
        {"name", "北京西"},
        {"platforms",1},
        {"meter",0}
    },{
        {"name", "任丘"},
        {"platforms",1},
        {"meter",147000}
    },{
        {"name", "衡水"},
        {"platforms",1},
        {"meter",274000}
    },{
        {"name", "九江"},
        {"platforms",1},
        {"meter",1314000}
    },{
        {"name", "南昌"},
        {"platforms",1},
        {"meter",1449000}
	}}}
};

decimal _basic_v(decimal x, decimal x00, decimal y00)
{
	return y00/2*sin(PI/x00*x-PI/2)+y00/2;
}

decimal v(decimal t, json tr, int len)
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

void generate_simple_timetable(json line, json tr, pair<int,int> starting_time)
{
	pair<int,int> curtime=starting_time;int day=0;
    int last_meter=0;
    cout<<"越站几率？（0～100）  "<<flush;
    int quang=0;cin>>quang;
    for(json::iterator it=line["stations"].begin(); it!=line["stations"].end(); it++)
    {
        if(rand()%100<=quang&&it!=line["stations"].begin()&&(it+1)!=line["stations"].end())continue;
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
}

int main()
{
	srand((unsigned)time(0));
	generate_simple_timetable(jingjiu,CR200J,pair<int,int>(19,59));
	generate_simple_timetable(j,CRNAN,pair<int,int>(8,0));
	
	return 0;
}
