#ifndef STR_HPP_1324997558_33
#define STR_HPP_1324997558_33
/**
 * File: str.hpp
 * Description: 
 *
 * Copyright (c) 2011 Jadesoul (Home Page: http://jadesoul.org)
 * Licensed under the GNU General Public License:
 * http://www.gnu.org/licenses/gpl.html
 * 
 * Date: 2011-12-27 22:52:38.326000
 * Written In: Peking University, beijing, China
 */

#include "includes.hpp"
#include "object.hpp"
#include "range.hpp"

class str {
public:
	typedef std::vector<str> vecstr;
	typedef std::string container;
	typedef container::iterator iterator;
	typedef container::const_iterator citerator;
	typedef container::reverse_iterator riterator;
	typedef container::const_reverse_iterator criterator;
	
private:
	container s;
	
public:
	inline iterator begin() { return s.begin(); }
	inline iterator end() { return s.end(); }
	inline riterator rbegin() { return s.rbegin(); }
	inline riterator rend() { return s.rend(); }
	
	inline citerator begin() const { return s.begin(); }
	inline citerator end() const { return s.end(); }
	inline criterator rbegin() const { return s.rbegin(); }
	inline criterator rend() const { return s.rend(); }
	
	//for size query
	inline const uint size() const { return s.size(); }
	inline const bool empty() const { return s.empty(); }
	inline const bool equals(const str& r) const { return s==r.s; }
	
	/**************************************************
	constructors:
	**************************************************/
	inline str() {}
	inline str(const char* cstr):s(cstr) {}	//construction from c-style string
	inline str(const string& cs):s(cs) {}	//construction from std string
	template<class Iterator>
	inline str(Iterator begin, Iterator end):s(begin, end) {}	//construction from two iterators
	inline str(const char& c) {	// construction from char
		char buf[2];
		buf[0]=c;
		buf[1]=0;
		s=buf;
	}
	inline str(const uchar& c) {	// construction from uchar
		char buf[2];
		buf[0]=c;
		buf[1]=0;
		s=buf;
	}
	inline str(const short& n) {	// construction from short
		char buf[16];
		sprintf(buf, "%d", n);
		s=buf;
	}
	inline str(const ushort& n) {	// construction from ushort
		char buf[16];
		sprintf(buf, "%d", n);
		s=buf;
	}
	inline str(const int& n) {	// construction from int
		char buf[16];
		sprintf(buf, "%d", n);
		s=buf;
	}
	inline str(const uint& n) {	// construction from uint
		char buf[16];
		sprintf(buf, "%u", n);
		s=buf;
	}
	inline str(const long& n) {	// construction from long
		char buf[32];
		sprintf(buf, "%ld", n);
		s=buf;
	}
	str(const ulong& n) {	// construction from ulong
		char buf[32];
		sprintf(buf, fmtstr_ulong, n);
		s=buf;
	}
	inline str(const float& n) {	// construction from float
		char buf[32];
		sprintf(buf, "%f", n);
		s=buf;
	}
	inline str(const double& n) {	// construction from double
		char buf[32];
		sprintf(buf, "%g", n); 
		s=buf;
	}
	inline str(const str& s):s(s.s) {}	//copy construction
	
	/**************************************************
	bool expressions:	== != > >= < <= ! 
	**************************************************/
	inline const bool operator==(const str& r) const { return s==r.s; }
	inline const bool operator<(const str& r) const { return s<r.s; }
	inline const bool operator>(const str& r) const { return s>r.s; }
	inline const bool operator!=(const str& r) const { return s!=r.s; }
	inline const bool operator<=(const str& r) const { return s<=r.s; }
	inline const bool operator>=(const str& r) const { return s>=r.s; }
	inline const bool operator!() const { return empty(); }
	
	/**************************************************
	tostr
	output operator: <<
	**************************************************/
	inline friend ostream& operator <<(ostream& o, const str& s) {
		// return o<<'"'<<s.s<<'"';
		return o<<s.s;
	}
	
	inline const string tostr() const { return s; }
	inline const char* tocstr() const { return s.c_str(); }
	inline const string repr() const { return string("\"")+clone()
		.replace("\\", "\\\\").replace("\t", "\\t")
		.replace("\v", "\\v").replace("\r", "\\r")
		.replace("\n", "\\n").tostr()+"\""; }
		
	/**************************************************
	assign
	operator: =
	**************************************************/
	inline str& operator=(const str& r) {
		return assign(r);
	}
	inline str& assign(const str& r) {
		s=r.s;
		return *this;
	}
	
	/**************************************************
	operator: + += * *=
	**************************************************/
	inline str operator +(const str& r) {	//for connection
		return str(s+r.s);
	}
	inline str added(const str& r) { return (*this)+r; }
	inline str operator +(const char* r) {	//for connection
		return clone()+=r;
	}
	inline str& operator +=(const str& r) {
		s+=r.s;
		return *this;
	}
	inline str& add(const str& r) { return (*this)+=r; }
	inline str& operator +=(const string& r) {
		s+=r;
		return *this;
	}
	inline str& operator +=(const char* r) {
		s+=r;
		return *this;
	}
	inline str operator *(int n) {	//for multiply
		return repeated(n);
	}
	inline str& operator *=(int n) {
		return repeat(n);
	}
	
	/**************************************************
	operator: [](int)
	operator: [](string)
	at
	**************************************************/
	inline char& operator[](int i) {	//for element visiting
		if (i<0) i+=s.size();
		return s[i];
	}
	inline const char& operator[](int i) const {
		if (i<0) i+=s.size();
		return s[i];
	}
	inline str operator [](const char* cstr) {
		return str::slice(range(cstr));
	}
	inline char& at(int i) {	//for element visiting, much more safe
		uint len=s.size();
		assert(len>0);
		while(i<0) i+=len;
		while(i>=static_cast<int>(len)) i-=len;
		return s.at(i);
	}
	
	/**************************************************
	slice: substr 
	operator: ()(pos)
	operator: ()(start, stop, step)
	**************************************************/
	//for substr getter
	inline str operator()(int pos) const {
		uint l=size();		
		if (pos<0) pos+=l;
		assert(pos>=0 AND pos<static_cast<int>(l));
		return s.substr(pos);
	}
	
	//for substr
	str operator()(int start, int stop, int step=1) const {
		if (step==0) return "";
		uint l=size();
		if (start<0) start+=l;
		assert(start>=0 AND start<static_cast<int>(l));
		if (stop<0) stop+=l;
		if (step>0) {
			assert(stop>=0 AND stop<=static_cast<int>(l));
			int sl=stop-start;
			if (sl<=0) return "";
			if (step==1) return s.substr(start, sl);
			string ret;
			ret.reserve(sl);
			for (uint i=start; static_cast<int>(i)<stop; i+=step) ret.push_back(s[i]);
			return ret;
		} else {
			assert(stop>=-1 AND stop<static_cast<int>(l));
			int sl=start-stop;
			if (sl<=0) return "";
			if (step==-1) return string(s.rbegin()+start, s.rbegin()+(start+sl));
			string ret;
			ret.reserve(sl);
			for (uint i=start; static_cast<int>(i)>stop; i+=step) ret.push_back(s[i]);
			return ret;
		}
	}
	
	//for slice
	inline str slice(const range& r) {
		// cout<<r<<endl;
		return ::slice(s, r);
	}
	
	/*************************************************
	S.split([sep [,results]]) -> list of strings
		Return a list of the words in the string S, using sep as the
		delimiter string. If results is given, it will get all the 
		result list of strings. If sep is not specified, any
		whitespace string is a separator and empty strings are removed
		from the result.
	*************************************************/
	template<class Container>
	inline Container& split(const str& sep, Container& results) {	//faster version
		vector<string::iterator> o;
		::split(s, sep, o);
		for (uint i=0; i<o.size(); i+=2) results.insert(results.end(), typename Container::value_type(o[i], o[i+1]));
		return results;
	}
	
	inline const vecstr split(const str& sep) {	//slower version
		vecstr vs;
		vs.reserve(20);
		return this->split(sep, vs);
	}
	
	vecstr split() {
		vecstr vs;
		vs.reserve(20);
		//TODO
		return striped().split(" ", vs);
	}
	
	/*************************************************
	S.join(sequence) -> str
		Return a string which is the concatenation of the strings in the
		sequence.  The separator between elements is S.
	*************************************************/
	template<class Container>
	inline str join(const Container& con) const {
		string ret;
		ret.reserve(1024);
		return ::join(s, con, ret);
	}
	
	/*************************************************
	S.find(sub [,start [,end]]) -> int
		Search from left to right.
		Return the lowest index in S where substring sub is found,
		such that sub is contained within s[start:end].  Optional
		arguments start and end are interpreted as in slice notation.
		Return -1 on failure.
	*************************************************/
	inline int find(const str& sub, int start=0, int end=0) const {
		citerator a=(start<0?s.end():s.begin())+start, b=(end<=0?s.end():s.begin())+end;
		if (a>=b) return -1;
		citerator c=std::search(a, b, sub.begin(), sub.end());
		return (c==b)?-1:c-a;
	}
	
	/*************************************************
	S.rfind(sub [,start [,end]]) -> int
		Search from right to left.
		Return the highest index in S where substring sub is found,
		such that sub is contained within s[start:end].  Optional
		arguments start and end are interpreted as in slice notation.
		Return -1 on failure.
	*************************************************/
	inline int rfind(const str& sub, int start=0, int end=0) const {
		uint l=size();
		if (start<0) start+=l;
		if (end<=0) end+=l;
		start=l-start;
		end=l-end;
		assert(start>0);
		assert(end>=0);
		criterator a=s.rbegin()+end, b=s.rbegin()+start;
		if (a>=b) return -1;
		criterator c=std::search(a, b, sub.rbegin(), sub.rend());
		return (c==b)?-1:(b-c)-sub.size();
	}
	
	/*************************************************
	S.clone() -> new str
		Return a deep copy of string S, which is a clone
		of S.
	*************************************************/
	inline str clone() const {
		return s;
	}
	
	/*************************************************
	S.replace(old, new[, count]) -> S
		Return string S with all occurrences of substring old 
		replaced by new.  If the optional argument count is 
		given, only the first count occurrences are replaced.
	*************************************************/
	str& replace(const str& old, const str& new_, uint count=-1) {
		uint start=0, olen=old.size(), nlen=new_.size();
		while(count==static_cast<uint>(-1) OR count-->static_cast<uint>(0)) {
			start=s.find(old.s, start);
			if (start==string::npos) break;
			s.replace(start, olen, new_.s);
			start+=nlen;
		}
		return *this;
	}
	
	/*************************************************
	S.replaced(old, new[, count]) -> new str
		Return a copy of string S with all occurrences 
		of substring old replaced by new.  If the optional 
		argument count is given, only the first count 
		occurrences are replaced.
	*************************************************/
	inline str replaced(const str& old, const str& new_, uint count=-1) {
		return clone().replace(old, new_, count);
	}
	
	/*************************************************
	S.count(sub[, start[, end]]) -> uint
		Return the number of non-overlapping occurrences of substring sub in
		string S[start:end].  Optional arguments start and end are interpreted
		as in slice notation.
	*************************************************/
	inline uint count(const str& sub, int start=0, int end=0) {
		return ::count((start<0?s.end():s.begin())+start, (end<=0?s.end():s.begin())+end, sub.begin(), sub.end());
	}
	
	/*************************************************
	S.center(width[, fillchar]) -> string
		Return S centered in a string of length width. Padding is
		done using the specified fill character (default is a space)
	*************************************************/
	inline str center(uint width, char fillchar) {
		string ret(width, fillchar);
		uint l=size();
		if (width>=l) {
			int r=(width-l)/2;
			std::copy(s.begin(), s.end(), ret.begin()+r);
		} else {
			int r=(l-width)/2;
			std::copy(s.begin()+r, s.begin()+(width+r), ret.begin());
		}
		return ret;
	}
	
	/*************************************************
	S.startswith(prefix[, start[, end]]) -> bool
		Return True if S starts with the specified prefix, False otherwise.
		With optional start, test S beginning at that position.
		With optional end, stop comparing S at that position.
		prefix can also be a tuple of strings to try.
	*************************************************/
	bool startswith(const str& prefix, int start=0, int end=0) const {
		return ::startswith((start<0?s.end():s.begin())+start, (end<=0?s.end():s.begin())+end, prefix.begin(), prefix.end());
	}
	
	/*************************************************
	S.endswith(suffix[, start[, end]]) -> bool
		Return True if S ends with the specified suffix, False otherwise.
		With optional start, test S beginning at that position.
		With optional end, stop comparing S at that position.
		suffix can also be a tuple of strings to try.
	*************************************************/
	bool endswith(const str& prefix, int start=0, int end=0) const {
		return ::endswith((start<0?s.end():s.begin())+start, (end<=0?s.end():s.begin())+end, prefix.begin(), prefix.end());
	}
	
	/*************************************************
	S.expandtabs([tabsize]) -> string
		Return a copy of S where all tab characters are expanded using spaces.
		If tabsize is not given, a tab size of 8 characters is assumed.
	*************************************************/
	str expandtabs(uint tabsize=8) {
		return replaced("\t", str(" ").repeated(tabsize));
	}
	
	/*************************************************
	S.upper() -> S
	S.uppered() -> new string
		Return a copy of the string S converted to uppercase.
	*************************************************/
	str& upper() {
		for (uint l=size(), i=0; i<l; ++i) {
			char& c=s[i];
			if (c>='a' AND c<='z') c+='A'-'a';
		}
		return *this;
	}
	
	inline str uppered() {
		return clone().upper();
	}
	
	/*************************************************
	S.lower() -> S
	S.lowered() -> new string
		Return a copy of the string S converted to lowercase.
	*************************************************/
	str& lower() {
		for (uint l=size(), i=0; i<l; ++i) {
			char& c=s[i];
			if (c>='A' AND c<='Z') c+='a'-'A';
		}
		return *this;
	}
	
	inline str lowered() {
		return clone().lower();
	}
	
	/*************************************************
	S.swapcase() -> S
	S.swapcased() -> new string
		Return a copy of the string S with uppercase characters
		converted to lowercase and vice versa.
	*************************************************/
	str& swapcase() {
		for (uint l=size(), i=0; i<l; ++i) {
			char& c=s[i];
			if (c>='a' AND c<='z') c+='A'-'a';
			else if (c>='A' AND c<='Z') c+='a'-'A';
		}
		return *this;
	}
	
	inline str swapcased() {
		return clone().swapcase();
	}
	
	/*************************************************
	S.reverse() -> S
	S.reversed() -> new string
		Return a copy of the string S with all chars
		reversed.
	*************************************************/
	str& reverse() {
		std::reverse(s.begin(), s.end());
		return *this;
	}
	
	inline str reversed() {
		return string(s.rbegin(), s.rend());
	}
	
	/*************************************************
	S.repeat(n) -> S
	S.repeated(n) -> new string
		Return a copy of the string S with itself repeated
		for n times. if n<0, the reversed S will be repeated.
	*************************************************/
	str& repeat(int n) {
		str& me=*this;
		uint l=size();
		if (l==0) return me;
		if (n==0) {
			s.clear();
			return me;
		}
		else if (n<0) {
			me.reverse();
			n=-n;
		}
		for (uint i=1; static_cast<int>(i)<n; ++i) s.append(s.begin(), s.begin()+l);
		return me;
	}
	
	inline str repeated(int n) {
		return clone().repeat(n);
	}
	
	/*************************************************
	S.sort() -> S
	S.sorted() -> new string
		Return a copy of the string S with all chars
		sorted.
	*************************************************/
	str& sort() {
		std::sort(s.begin(), s.end());
		return *this;
	}
	
	inline str sorted() {
		return clone().sort();
	}
	
	/*************************************************
	S.strip([chars]) -> S
	S.lstrip([chars]) -> S
	S.rstrip([chars]) -> S
	S.striped([chars]) -> string
	S.lstriped([chars]) -> string
	S.rstriped([chars]) -> string
		Return a copy of the string S with leading and trailing
		whitespace removed.
		If chars is given, remove characters in chars instead.
	*************************************************/
	inline str& strip(const str& chars=" \t\v\r\n\f") {
		return rstrip(chars).lstrip(chars);
	}
	
	inline str& lstrip(const str& chars=" \t\v\r\n\f") {
		s.erase(0, s.find_first_not_of(chars.s));
		return *this;
	}
	
	inline str& rstrip(const str& chars=" \t\v\r\n\f") {
		uint i=s.find_last_not_of(chars.s);
		if (i==s.npos) return *this;
		s.erase(i+1);
		return *this;
	}
	
	inline str striped(const str& chars=" \t\v\r\n\f") {
		return clone().strip(chars);
	}
	
	inline str lstriped(const str& chars=" \t\v\r\n\f") {
		return clone().lstrip(chars);
	}
	
	inline str rstriped(const str& chars=" \t\v\r\n\f") {
		return clone().rstrip(chars);
	}
	
	/**************************************************
	tohash:	x.tohash() <==> Return DWORD hash
	**************************************************/
	inline const uint tohash() const {
		uint h=0, i=s.size();
		while(i>0) h=107*h+s[--i];
		return h;
	}
};

typedef vector<str> vecstr;

#endif /* STR_HPP_1324997558_33 */


