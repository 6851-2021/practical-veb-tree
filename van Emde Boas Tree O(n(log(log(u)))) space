from math import ceil, log2
'''
van Emde Boas Tree is a data structure which gives O(log(log(u))
query time for operations like 
insert, search, delete, successor and predecessor

VEB class contains attribute
min, max, u, w, cluster and summary

initially min=max=NULL
u=size of universe (the range of total possible entries)
w=word length (number of bits in u)
w=log2(u)

cluster is an array of VEB structures of size of sqrt(u)
summary is a VEB of size sqrt(u)

when the size of VEB structure reaches we don't store clusters and summary vector
min and max are enough to store this structure.
'''

class VEB:

    '''index of x can be determined as the
    cluster number and the position inside the cluster

    for example lets consider 11
    in binary it is written as 1011
    so first half parts of the binary strinig give cluster number
    and 2nd half gives the postiton inside cluster

    cluster number= int(10)= 2
    position inside cluster= int(11)=3

    so 11 is in 2nd cluster at 3rd position
    where counting starts from 0th position

    0,1,2,3|4,5,6,7|8,9,10,11|12,13,14,15
                           ^
    here we use 'c' to denote cluster number
    and 'i' to denote index inside the cluster

    so x can be represented as <c,i>
    where x=c*sqrt(u)+i
    '''
    def high(self, x):
        # high(x)=x//int(sqrt(u))
        return x >> (self.w // 2)

    def low(self, x):
        # low(x)= x%int(sqrt(u))
        return x & (1 << (self.w // 2)) - 1

    def index(self, i, j):
        # return i*int(sqrt(self.u))+j
        return i << (self.w // 2) | j

    def __init__(self, u):
        '''
        this have been implemented using hash table
        to reduce the space complexity from O(U) to O(n*log(log(u))
        because u can be very large. for example if word size = 64 bits
        u= 2^64 = 16 million TB which can't be stored practically on ram.
        where as n*log*log*u can be O(3n) which can be easily stored.

        I have a different code for array implementation too.
        '''

        self.w = ceil(log2(u))
        # self.u = 2 ** self.w
        self.min = self.max = None

        if self.w >= 1:     #when u==2^w=2 min and max are enough so we stop recursion
            self.cluster = {}
            self.summary = None

    def member(self, x):
        '''function to check if x is present in tree or not'''
        if x == self.min or x == self.max:
            return True
        elif self.w == 1:
            return False
        else:
            c = self.high(x)
            i = self.low(x)
            if c in self.cluster:
                return self.cluster[c].member(i)
            else:
                return False

    def insert(self, x):

        if self.min is None:
            self.min = x
            self.max = x
            return
        else:
            if x < self.min:
                x, self.min = self.min, x
            c = self.high(x)
            i = self.low(x)
            if self.w > 1:
                if c not in self.cluster:
                    self.cluster[c]=VEB(2**(self.w//2))
                if self.cluster[c].min is None:
                    if self.summary is None:
                        self.summary = VEB(2**(self.w//2))
                    self.summary.insert(c)
                if c not in self.cluster:
                    self.cluster[c]=VEB(2**(self.w//2))
                self.cluster[c].insert(i)
            if x > self.max:
                self.max = x

    def succesor(self, x):
        if self.w == 1:
            if x == 0 and self.max == 1:
                return 1
            else:
                return None
        elif self.min is not None and x < self.min:
            return self.min
        else:
            c = self.high(x)
            i = self.low(x)
            if c in self.cluster:
                maxlow = self.cluster[c].max
            else:
                maxlow=None
            if maxlow is not None and i < maxlow:
                offset = self.cluster[c].succesor(i)
                return self.index(c, offset)
            else:
                if self.summary is not None:
                    succ_cluster = self.summary.succesor(self.high(x))
                else:
                    succ_cluster = None
                if succ_cluster is None:
                    return None
                else:
                    offset = self.cluster[succ_cluster].min
                    return self.index(succ_cluster, offset)

    def predecessor(self, x):
        if self.w == 1:
            if x == 1 and self.min == 0:
                return 0
            else:
                return None
        elif self.max is not None and x > self.max:
            return self.max
        else:
            c = self.high(x)
            i = self.low(x)
            if c in self.cluster:
                min_low = self.cluster[c].min
            else:
                min_low = None
            if min_low is not None and i > min_low:
                offset = self.cluster[c].predecessor(i)
                return self.index(c, offset)
            else:
                if self.summary is not None:
                    prev_cluster = self.summary.predecessor(c)
                else:
                    prev_cluster = None
                if prev_cluster is None:
                    if self.min is not None and x > self.min:
                        return self.min
                    else:
                        return None
                else:
                    offset = self.cluster[prev_cluster].max
                    return self.index(prev_cluster, offset)

    def delete(self, x):
        if self.min is None:
            return
        if x < self.min or x > self.max:
            return
        if self.min == self.max:
            self.min = self.max = None
        elif self.w == 1:
            if x == 0:
                self.min = 1
            else:
                self.min = 0
            self.max = self.min
        else:
            c = self.high(x)
            i = self.low(x)
            if x == self.min:
                if self.summary:
                    first_cluster = self.summary.min
                else:
                    first_cluster = None
                if first_cluster:
                    x = self.index(first_cluster, self.cluster[first_cluster].min)
                    self.min = x
            if c in self.cluster:
                self.cluster[c].delete(i)
                if self.cluster[c].min is None:
                    self.summary.delete(c)
                if x == self.max:
                    summary_max = self.summary.max
                    if summary_max is None:
                        self.max = self.min
                    else:
                        self.max = self.index(summary_max, self.cluster[summary_max].max)
            elif x == self.max:
                self.max = self.index(c, self.cluster[c].max)
