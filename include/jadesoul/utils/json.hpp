#ifndef JSON_HPP_1326978838_97
#define JSON_HPP_1326978838_97
/**
 * File: json.hpp
 * Description: 
 *
 * Copyright (c) 2011 Jadesoul (Home Page: http://jadesoul.org)
 * Licensed under the GNU General Public License:
 * http://www.gnu.org/licenses/gpl.html
 * 
 * Date: 2012-01-19 21:13:58.976000
 * Written In: Peking University, beijing, China
 */

#include "includes.hpp"
#include "num.hpp"
#include "str.hpp"
#include "tuple.hpp"
#include "list.hpp"
#include "set.hpp"
#include "dict.hpp"

//the cpp json implementation
/*
example:
	json:{
		person:[
			{
				name:jadesoul,
				age:27
			},
			{
				name:hi,
				age:39
			}
		]
	}
*/

// dict {
	// name
	// age
// }



class json {
public:
	struct item {
		typedef list<item> array;
		typedef dict<str, item> dictionary;
		
		char type;	//array(A) or dictionary(D) or str(S) or num(N) or empty(E)
		void* ptr;
		// ptr=new array;
		// ptr=new dictionary;
		// ptr=new str;
		// ptr=new num;
		item():type('E'), ptr(NULL) {}
		item(const str& s):type('S'), ptr(new str(s)) {}
		item(const array& a):type('A'), ptr(new array(a)) {}
		item(const dictionary& d):type('D'), ptr(new dictionary(d)) {}
		item(const item& i):type(i.type), ptr(i.ptr) {}
		
		inline num& tonum() const { return *((num*)ptr); }
		inline str& tostr() const { return *((str*)ptr); }
		inline array& toarr() const { return *((array*)ptr); }
		inline dictionary& todict() const { return *((dictionary*)ptr); }
		
		inline const bool isnum() const { return type=='N'; }
		inline const bool isstr() const { return type=='S'; }
		inline const bool isarr() const { return type=='A'; }
		inline const bool isdict() const { return type=='D'; }
		inline const bool issimple() const { return isnum() OR isstr(); }
		inline const bool iscomplex() const { return isarr() OR isdict(); }
		
		inline const uint size() const {
			if (issimple()) return 1;
			if (isarr()) return toarr().size();
			else return todict().size();
		}
	} root;
	
	typedef list<item> array;
	typedef dict<str, item> dictionary;
	
	json() {
		root.type='A';
		root.ptr=new array();
		array& a=*((array*)root.ptr);
		a.append(item(str("hi")));
		a.append(item(str("hi")));
		
		dictionary d;
		// d["a"]=str("sit");
		a.append(item(d));
		array a2;
		a2.append(item(str("test")));
		a2.append(item(str("test")));
		a.append(item(a2));
		a.append(item(str("hi")));
		a.append(item(str("hi")));
		a.append(item(str("hi")));
		a.append(item(str("hi")));
	}
	inline void decode(const str& s) {
		
	}
	inline str encode() {
		return encode(root);
	}
	
	str encode(const item& i, str prefix="", const bool final=true) {
		if (i.isarr()) {
			array& a=i.toarr();
			list<str> ss;
			ss.append(prefix+"[");
			uint l=i.size(), cnt=0;
			for_iter(it, array, a) {
				++cnt;
				item& j=*it;
				ss.append(encode(j, prefix+"\t", cnt==l));
			}
			ss.append(prefix+(final?"]":"],"));
			return ss.glue("\n");
		} else if (i.isdict()) {
			dictionary& d=i.todict();
			list<str> ss;
			ss.append(prefix+"{");
			dictionary::cpelist items=d.pitems();
			uint l=i.size(), cnt=0;
			for_iter(it, dictionary::cpelist, items) {
				++cnt;
				const dictionary::entry& e=**it;
				const str& k=e.k;
				const item& v=e.v;
				if (v.isstr() OR v.isnum()) ss.append(str('"')+k+"\":"+encode(v, "", cnt==l));
				else ss.append(str('"')+k+"\":"+encode(v, prefix+'\t', cnt==l));
			}
			ss.append(prefix+(final?"}":"},"));
			return ss.glue("\n");
		} else if (i.isstr()) {
			return prefix+"\""+i.tostr()+(final?"\"":"\","); 
		} else {//if (i.isnum()) {
			return prefix+string(i.tonum())+(final?"":",");
		}
	}
	
	inline friend ostream& operator<<(ostream& o, json& j) {
		return o<<j.encode();
	}
};

#endif /* JSON_HPP_1326978838_97 */
