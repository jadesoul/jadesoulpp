#ifndef RANGE_HPP_1324966597_26
#define RANGE_HPP_1324966597_26
/**
 * File: range.hpp
 * Description: 
 *
 * Copyright (c) 2011 Jadesoul (Home Page: http://jadesoul.org)
 * 
 * Date: 2011-12-27 14:16:37.261000
 * Written In: Peking University, beijing, China
 */

#include "includes.hpp"

/**
  * range -- Interval types implementation
  *
  * range use start, stop and step to stand for a range
  * it takes a range expression as parameter
  * the format of range expression is like python
  * example: [0:1], [3:], [10:-1], [1:10:2]
  */

class range {
public:
	int start;
	int stop;
	int step;
	const static int first=min_sint4;
	const static int last=max_sint4;

	range(int start, int stop, int step=1): start(start), stop(stop), step(step) {}

	range(int len=0) : start(0), stop(len), step(1) {}

	range(const string& range_expr) {
		construct_from_str(range_expr);
	}

	range& operator =(const string& range_expr) {
		range& me=*this;
		me.construct_from_str(range_expr);
		return me;
	}

	inline bool very_begin() const {
		return start==first;    //return if start is the very beginning of the range
	}
	inline bool very_end() const {
		return stop==last;    //return if stop is the very end of the range
	}

	void operator ()(const string& range_expr) {
		construct_from_str(range_expr);
	}

	friend ostream& operator<<(ostream& out, const range& r) {
		out<<"range("<<r.start<<":"<<r.stop<<":"<<r.step<<") = ";
		
		if ((r.start==r.stop) || (r.start<r.stop AND r.step<0)
			|| (r.start>r.stop AND r.step>0)) return out<<"[]";
		
		bool a= (r.start==first || r.start==last);
		bool b= (r.stop==last || r.stop==first);
		if (a || b) {
			out<<"[ ";
			
			if (r.start==first) out<<"-Limit";
			else if (r.start==last) out<<"+Limit";
			else out<<r.start;
			
			out<<" ... ";
			
			if (r.stop==first) out<<"-Limit";
			else if (r.stop==last) out<<"+Limit";
			else out<<r.stop;
			
			out<<" , "<<r.step;
			return out<<" )"<<endl;
		}
		out<<"[";
		if (r.stop>r.start AND r.step>0)
			for (int i=r.start; i<r.stop; i+=r.step) out<<i<<", ";
		else if (r.stop<=r.start AND r.step<0)
			for (int i=r.start; i>r.stop; i+=r.step) out<<i<<", ";
		out<<"]"<<endl;

		return out;
	}

	vec_int to_vec() {
		vec_int vec;
		if (stop>start AND step>0)
			for (int i=start; i<stop; i+=step) vec.push_back(i);
		else if (stop<=start AND step<0)
			for (int i=start; i>stop; i+=step) vec.push_back(i);
		return vec;
	}

private:
	void construct_from_str(const string& range_expr) {
		string deli=":";
		typedef vector<pair<string::const_iterator, string::const_iterator> > result_type;
		result_type nums(split(range_expr.begin(), range_expr.end(), deli.begin(), deli.end()));
		int nums_size=nums.size();
		assert( nums_size==2 || nums_size==3 );
		ptrdiff_t len_a=distance(nums[0].first, nums[0].second);
		if (len_a>0) {
			string s_start(nums[0].first, nums[0].second);
			start = atoi( s_start.c_str() );
		} else start=first;
		
		ptrdiff_t len_b=distance(nums[1].first, nums[1].second);
		if (len_b>0) {
			string s_stop(nums[1].first, nums[1].second);
			stop = atoi( s_stop.c_str() );
		} else stop=last;

		if (nums_size==2) step=1;
		else {
			string s_step(nums[2].first, nums[2].second);
			step = atoi( s_step.c_str() );
		}
		if (step==0) step=1;
		if (start==first && stop==last && step<0) {
			swap(start, stop);
		}
	}
} __tmp_range;

typedef range rg;
#define RG __tmp_range=

/**
 *  slice -- slice a big container by a range to get a smaller one (will copy a new one)
 *
 *  by jadesoul @ 2010-10-1
 *  return a new container
*/

//improve slice
template <  class Container >
inline Container slice(Container& c, const range& r) {
	return slice(c, r.start, r.stop, r.step);
}

#endif /* RANGE_HPP_1324966597_26 */
