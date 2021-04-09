#include <iostream>
#include <algorithm>
#include <assert.h>
#include <random>
#include <time.h>
#include <set>
using namespace std;
typedef long long ll;
#include "sparse_veb.hpp"
struct Test_Container {
	public:
	std::string name;
	virtual bool ins(ll) = 0;
	virtual bool del(ll) = 0;
	virtual ll suc(ll) = 0;
	ll op(int x,ll y) {
		assert(x>=0&&x<=2);
		if(x==0) return ins(y);
		if(x==1) return del(y);
		if(x==2) return suc(y);
	}
};
struct Test_Set: Test_Container {
	std::set<ll> s;
	public:
	Test_Set() {name="Set";}
	inline bool ins(ll x) {
		return s.insert(x).second;
	}
	inline bool del(ll x) {
		return s.erase(x);
	}
	inline ll suc(ll x) {
		auto w=s.upper_bound(x);
		if(w!=s.end()) return *w;
		return -1;
	}
	inline int size() {
		return s.size();
	}
};
#include<ext/pb_ds/assoc_container.hpp>
struct Test_Pbds: Test_Container {
	__gnu_pbds::tree<ll,__gnu_pbds::null_type> s;
	public:
	Test_Pbds() {name="Pbds";}
	inline bool ins(ll x) {
		return s.insert(x).second;
	}
	inline bool del(ll x) {
		return s.erase(x);
	}
	inline ll suc(ll x) {
		auto w=s.upper_bound(x);
		if(w!=s.end()) return *w;
		return -1;
	}
};
struct Test_Veb: Test_Container {
	sparseVeb<60> s;
	public:
	Test_Veb() {name="Veb";}
	inline bool ins(ll x) {
		return s.ins(x);
	}
	inline bool del(ll x) {
		return s.del(x);
	}
	inline ll suc(ll x) {
		return s.suc(x);
	}
};
const ll N=(1LL<<40);
const int S=(1<<20);
ll op[S][2],rst[S][3];
void gen_data() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> d0(0,99);
	std::uniform_int_distribution<long long> d1(0,N-1);
    for(int i=0;i<S;++i)
    {
    	int x=d0(gen);
    	if(x<30) op[i][0]=0;
    	else if(x<45) op[i][0]=1;
    	else op[i][0]=2;
		op[i][1]=d1(gen);
	}
}
void run() {
	for(int i=0;i<3;++i) {
		auto t0=clock();
		Test_Container *ct;
		if(i==0) ct=new Test_Set();
		else if(i==1) ct=new Test_Pbds();
		else if(i==2) ct=new Test_Veb();
		auto tm=clock();
		for(int j=0;j<S;++j)
			rst[j][i]=ct->op(op[j][0],op[j][1]);
		auto t1=clock();
		std::cerr<<ct->name<<" took "<<(t1-t0)*1./CLOCKS_PER_SEC<<"s ("<<(tm-t0)*1./CLOCKS_PER_SEC<<"s init)\n";
		if(i==0) {
			std::cerr<<" Final size: "<<((Test_Set*)ct)->size()<<"\n";
		}
		for(int j=0;j<S;++j)
			if(rst[j][0]!=rst[j][i]) std::cerr<<"fatal error: different answer\n";
		delete ct;
	}
}
int main()
{
	cout<<"Key space: "<<N<<"\n";
	cout<<"# of Operation: "<<S<<"\n";
	gen_data();
	cout<<"Data generation done...\n";
	run();
}

