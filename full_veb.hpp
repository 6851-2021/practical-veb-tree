#include <algorithm>
#include <type_traits>
namespace Full_Veb_Implementation {
template<int s,class small_enough=void>
class vEB {
	vEB<s/2> *tp;
	vEB<(s+1)/2> *bt;
	public:
	int mi,mx;
	vEB() {
		mi=mx=-1;
		tp=new vEB<s/2>;
		bt=new vEB<(s+1)/2>[1<<(s/2)];
	}
	inline int suc(int x) const {
//		assert(x>=0&&x<(1<<s));
		if(x<mi) return mi;
		if(mi==-1) return -1;
		if(s==1) {
			if(x<mx) return mx;
			return -1;
		}
		int c=x>>((s+1)/2),i=x&((1<<((s+1)/2))-1);
		if(i<bt[c].mx) {
			int w=bt[c].suc(i);
			if(w==-1) return -1;
			return (c<<((s+1)/2))|w;
		}
		int u=tp->suc(c);
		if(u==-1) return -1;
		return (u<<((s+1)/2))|bt[u].mi;
	}
	inline bool ins(int x) {
//		assert(x>=0&&x<(1<<s));
		if(mi==-1) {
			mi=mx=x; return 1;
		}
		if(x==mi||x==mx) return 0;
		if(x<mi) std::swap(mi,x);
		if(x>mx) mx=x;
		if(s==1) {
//			assert(mi!=-1);
			return 1;
		}
		int c=x>>((s+1)/2),i=x&((1<<((s+1)/2))-1);
		if(bt[c].mi==-1) tp->ins(c);
		return bt[c].ins(i);
	}
	inline bool del(int x) {
//		assert(x>=0&&x<(1<<s));
		if(x==mi) {
			if(s==1) {
				if(mx==mi) mi=mx=-1;
				else mi=mx;
				return 1;
			}
			int c=tp->mi;
			if(c==-1) {
				mi=mx=-1; return 1;
			}
//			assert(bt[c].mi!=-1);
			x=mi=(c<<((s+1)/2))|bt[c].mi;
		}
//		assert(x>=0&&x<(1<<s));
		if(s==1) {
			if(x==mx) {
				mx=mi;
				return 1;
			}
			else return 0;
		}
		int c=x>>((s+1)/2),i=x&((1<<((s+1)/2))-1);
		if(bt[c].mi==-1) return 0;
		if(!bt[c].del(i)) {
			assert(bt[c].mi!=-1);
			return 0;
		}
		if(bt[c].mi==-1) tp->del(c);
		if(tp->mi==-1) mx=mi;
		else {
			int u=tp->mx;
			mx=(u<<((s+1)/2))|bt[u].mx;
		}
		return 1;
	}
};
template<bool cond,typename U>
using resolvedType=typename std::enable_if<cond,U>::type;
template<int s>
class vEB<s,resolvedType<(s<=5),void>> {
	public:
	unsigned u; int mi,mx;
	vEB():u(0),mi(-1),mx(-1) {
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
using fullVeb=Full_Veb_Implementation::vEB<s>;
