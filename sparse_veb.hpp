#include <algorithm>
#include <iostream>
#include <type_traits>
#include <ext/pb_ds/assoc_container.hpp>
namespace Sparse_Veb_Implementation {
typedef long long ll;
template<int s,class placeholder=void>
class VEB {
};
template<int s,class arg_type>
class vEB {
	VEB<s/2> *tp;
	__gnu_pbds::gp_hash_table<int,VEB<(s+1)/2>*> bt;
	public:
	arg_type mi,mx;
	vEB(): tp(0),mi(-1),mx(-1) {
	}
	inline VEB<(s+1)/2>* get_bt(int c) {
		auto&u=bt[c];
		if(!u) u=new VEB<(s+1)/2>;
		return u;
	}
	inline VEB<(s+1)/2>* get_bt_noalloc(int c) {
		return bt[c];
	}
	inline VEB<s/2>* get_tp() {
		if(!tp) tp=new VEB<s/2>; return tp;
	}
	inline arg_type suc(arg_type x) {
		assert(x>=0&&x<(1LL<<s));
		if(x<mi) return mi;
		if(mi==-1||!tp) return -1;
		int c=x>>((s+1)/2),i=x&((1LL<<((s+1)/2))-1);
		auto btc=get_bt_noalloc(c);
		if(btc&&i<btc->mx) {
			int w=btc->suc(i);
			if(w==-1) return -1;
			return ((arg_type)c<<((s+1)/2))|w;
		}
		ll u=tp->suc(c);
		if(u==-1) return -1;
		return ((arg_type)u<<((s+1)/2))|bt[u]->mi;
	}
	inline bool ins(arg_type x) {
		assert(x>=0&&x<(1LL<<s));
		if(mi==-1) {
			mi=mx=x; return 1;
		}
		if(x==mi||x==mx) return 0;
		if(x<mi) std::swap(mi,x);
		if(x>mx) mx=x;
		int c=x>>((s+1)/2),i=x&((1LL<<((s+1)/2))-1);
		auto btc=get_bt(c);
		if(btc->mi==-1) get_tp()->ins(c);
		return btc->ins(i);
	}
	inline bool del(arg_type x) {
		assert(x>=0&&x<(1LL<<s));
		if(mi==-1||x<mi) return 0;
		VEB<(s+1)/2> *btc=0;
		if(x==mi) {
			if(!tp||tp->mi==-1) {
				mi=mx=-1; return 1;
			}
			int c=tp->mi; btc=bt[c];
			assert(btc->mi!=-1);
			x=mi=((arg_type)c<<((s+1)/2))|btc->mi;
		}
		if(!tp||tp->mi==-1) return 0;
		int c=x>>((s+1)/2),i=x&((1LL<<((s+1)/2))-1);
		if(!btc) btc=bt[c];
		if(!btc||btc->mi==-1) return 0;
		if(!btc->del(i)) return 0;
		if(btc->mi==-1) tp->del(c);
		if(tp->mi==-1) mx=mi;
		else {
			int u=tp->mx;
			mx=((arg_type)u<<((s+1)/2))|bt[u]->mx;
		}
		return 1;
	}
};
template<bool cond,typename U>
using resolvedType=typename std::enable_if<cond,U>::type;
template<int s>
class VEB<s,resolvedType<(s>5&&s<=30),void>>: public vEB<s,int> {
};
template<int s>
class VEB<s,resolvedType<(s>30),void>>: public vEB<s,ll> {
};
template<int s>
class VEB<s,resolvedType<(s<=5),void>> {
	public:
	unsigned u; int mi,mx;
	VEB():u(0),mi(-1),mx(-1) {
	}
	inline int suc(int x) const {
		if(x>=mx) return -1;
		return x+1+__builtin_ctz(u>>(x+1));
	}
	inline bool ins(int x) {
		if(u&(1u<<x)) return 0;
		u^=1u<<x;
		if(mi==-1||mi>x) mi=x;
		if(x>mx) mx=x;
		return 1;
	}
	inline bool del(int x) {
		if(!(u&(1u<<x))) return 0;
		u^=1u<<x;
		if(!u) mi=mx=-1;
		else {
			mi=__builtin_ctz(u);
			mx=31-__builtin_clz(u);
		}
		return 1;
	}
};
}
template<int s>
using sparseVeb=Sparse_Veb_Implementation::VEB<s>;
