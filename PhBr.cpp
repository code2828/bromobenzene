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
using namespace std;
using json = nlohmann::json;

json CR100J = {
	{"ID", "CR100J"},
	{"maximum_acceleration", 0.5},
	{"maximum_velocity", 28}
};

json j = {
	{"ID", -1},
	{"name", "random"},
	{"time_unit",30},
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
	}}}
};

decimal _basic_velocity(decimal tt, decimal len, decimal a_max, decimal v_max, json jj=j) // tt in time_units, result in meter per time_unit assuming a=1
{
	if(tt<0)throw("wth u tried to use a negative time!");
	decimal v_cap = (-3.0+sqrt(9.0+4*len))/2.0;
	v_max=min(v_max,v_cap);
	decimal ta=v_max+1.00, m=ta+3.00, n=m-4.00, tall=(len-2.0*(0.50*v_max*v_max+v_max))/v_max+2*ta+1.00, tsym=tall-ta+m-1.00;
	if(tsym-tt<tt)tt=tsym-tt;
	decimal ret=0;
	if(0<=tt&&tt<=1)ret=0;
	else if(1<tt&&tt<=2)ret=tt*tt*tt/6.0-0.5*tt*tt+0.5*tt-1/6.0;
	else if(2<tt&&tt<=3)ret=0-tt*tt*tt/6.0+1.5*tt*tt-3.5*tt+2.5;
	else if(3<tt&&tt<=m-3)ret=tt-2;
	else if(m-3<tt&&tt<=m-2)ret=n-(-(m-tt)*(m-tt)*(m-tt)/6.0+1.5*(m-tt)*(m-tt)-3.5*(m-tt)+2.5);
	else if(m-2<tt&&tt<=m-1)ret=n-((m-tt)*(m-tt)*(m-tt)/6.0-0.5*(m-tt)*(m-tt)+0.5*(m-tt)-1/6.0);
	else if(m-1<tt)ret=n;
	return ret;
}

decimal velocity(decimal t, long interval_start, long interval_end, json train, json j_=j)
{
	long interval_length=j_["stations"][interval_end].value("meter",0)-j_["stations"][interval_start].value("meter",0);
	decimal tu=j_.value("time_unit",0);
	decimal a_max=train.value("maximum_acceleration",0.0)*tu*tu;
	return _basic_velocity(t/tu,interval_length/a_max,a_max,train.value("maximum_velocity",0.0)*tu/a_max,j_)*a_max/tu;
}

int main()
{
	for(decimal d=0;d<=1000;d+=1)
	{
		cout<<"At time "<<d<<"s, the train's velocity is "<<velocity(d,0,3,CR100J)<<"m/s\n";
	}

	return 0;
}

