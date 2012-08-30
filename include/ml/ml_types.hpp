#ifndef ML_TYPES_HPP_BY_JADESOUL
#define ML_TYPES_HPP_BY_JADESOUL
/**
 * File: ml_types.hpp
 * Description: 
 *
 * Copyright (c) 2011 Jadesoul (Home Page: http://jadesoul.org)
 * Licensed under the GNU General Public License:
 * http://www.gnu.org/licenses/gpl.html
 * 
 * Date: 2011-06-26 14:18:09.480000
 * Written In: Peking University, beijing, China
 */

#include <jadesoul.hpp>

//range slicer
class ranger {
	int start;
	int stop;
	int step;
	int first;
	int last;
	int len;
	int now;
	bool empty;
	bool ended;
	void init() {
		empty=false;
		if (start<-len) start=0;
		else if (start<0) start+=len;
		else if (start<=len-1) {
			//good
		} else {
			if (step>0) empty=true;
			else start=len-1;
		}
		
		if (stop<-len) {
			if (step>0) empty=true;
			else stop=-1;
		}
		else if (stop<0) stop+=len;
		else if (stop<=len) {
			//good
		} else {
			if (step>0) stop=len;
			else empty=true;
		}
	
		if (start>stop && step>0) empty=true;
		if (start<stop && step<0) empty=true;
	}
	
public:
	ranger(int len, int start=min_sint4, int stop=max_sint4, int step=1, int first=0) {
		this->start=start;
		this->stop=stop;
		assert(step!=0);
		this->step=step;
		assert(len>0);
		this->first=first;
		this->last=first+len-1;
		this->len=len;
		init();
	}
	
	uint size() {
		if (empty) return 0;
		uint s=0;
		if (step>0) {
			for (int i=start; i<stop; i+=step) ++s;
		} else {
			for (int i=start; i>stop; i+=step) ++s;
		}
		return s;
	}
	
	int begin() {
		if (empty) return 0;
		ended=false;
		now=start;
		return now+first;
	}
	
	bool end() {
		return empty || ended;
	}
	
	int next() {
		now+=step;
		if (step>0 && now>=stop) {
			ended=true;
		} else if (step<0 && now<=stop) {
			ended=true;
		}
		return now+first;
	}
	
	// void print( ostream& out ) {
		// for(int i=begin(); !end(); i=next()) {
			// out<<i<<"\t";
		// }
		// out<<endl;
	// }
};

//2d data table
template<class type>
class datatable {
	vector<type> data;
	uint rows;
	uint cols;
	
	
public:
	typedef typename vector<type>::iterator iterator;
	datatable(uint row=1, uint col=1) {
		resize(row, col);
	}
	
	void resize() {
		ulong size=rows*cols;
		data=vector<type>(size);
	}
	
	void clear() {
		resize();
	}
	
	void resize(uint row, uint col) {
		rows=row;
		cols=col;
		resize();
	}
	
	type* begin() {
		return &data.front();
	}
	
	type* end() {
		return &data.back()+1;
	}
	
	type* rowbegin(uint r) {
		return addr(r, 0);
	}
	
	type* rowend(uint r) {
		return addr(r, cols);
	}
	
	type* rowlast(uint r) {
		return addr(r, cols-1);
	}
	
	inline type& at(uint i, uint j) {
		return data[i*cols+j];
	}
	
	inline type& addr(uint i, uint j) {
		return &data[0]+i*cols+j;
	}

	inline uint row() const {
		return rows;
	}

	inline uint col() const {
		return cols;
	}

	void shuffle(int times) {
		row_shuffle(times);
	}
	
	void shuffle() {
		row_shuffle(rows);
	}
	
	void row_shuffle(int times) {
		if(times==0) times=rows;
		for_n(i, times) {
			int a=rand() % rows;
			int b=rand() % rows;
			if (a!=b)
				for(uint j=0; j<cols; ++j)
					std::swap(at(a, j), at(b, j));
		}
	}
	
	void slice(datatable& dt, int rbegin=0, int rend=max_sint4, int rstep=1, int cbegin=0, int cend=max_sint4, int cstep=1) {
		ranger row(rows, rbegin, rend, rstep);
		ranger col(cols, cbegin, cend, cstep);
		dt.resize(row.size(), col.size());
		int m, n, i, j;
		for (m=0, i=row.begin(); !row.end(); i=row.next(), ++m)
			for (n=0, j=col.begin(); !col.end(); j=col.next(), ++n)
				dt.at(m, n)=at(i, j);
	}
	
	void head(datatable& dt, int nrow) {
		slice(dt, min_sint4, nrow);
	}
	
	void head(datatable& dt, float percent) {
		slice(dt, min_sint4, (int)(percent*rows));
	}
	
	void tail(datatable& dt, int nrow) {
		slice(dt, -nrow, max_sint4);
	}
	
	void tail(datatable& dt, float percent) {
		slice(dt, -(int)(percent*rows), max_sint4);
	}
	
	void save(ostream& out) {
		out<<rows<<"\t"<<cols<<endl;
		for(uint i=0; i<rows; ++i) {
			for(uint j=0; j<cols; ++j) out<<at(i, j)<<"\t";
			out<<endl;
		}
	}
	
	void save(const char* fn) {
		save(ofstream(fn));
	}

	void load(istream& in=cin) {
		in>>rows>>cols;
		resize();
		for(uint i=0; i<rows; ++i)
			for(uint j=0; j<cols; ++j) 
				in >> at(i, j);
	}
	
	void load(const char* fn) {
		load(ifstream(fn));
	}
	
	void input(istream& in=cin) {
		for(uint i=0; i<rows; ++i)
			for(uint j=0; j<cols; ++j) {
				type tmp;
				in >> tmp;
				at(i, j)=tmp;
			}
	}
	
	// void print(ostream& out) {
		// out<<"datatable: rows="<<rows<<" cols="<<cols<<endl;
		// for(uint i=0; i<rows; ++i) {
			// for(uint j=0; j<cols; ++j) out << data[i*cols+j]<<"\t";
			// out<<endl;
		// }
	// }
};


//pearson corelation
template <class InputIterator, class T, class TripleOperation>
T accumulate( InputIterator first, InputIterator last, InputIterator first2,T init, TripleOperation op) {
	while (first!=last)
		init=op(init, *first++, *first2++);
	return init;
}

//a sequence indicated by two iterators;
template<class iterator>
class sequence {
	iterator begin;
	iterator end;
public:
	typedef typename iterator_traits<iterator>::value_type type;
	sequence(iterator first, iterator last): begin(first), end(last) {
		assert(end-begin>0);
	}

	sequence(const sequence& r) {
		assert(r.end-r.begin>0);
		begin=r.begin;
		end=r.end;
	}
	
	void clone_from(const sequence& r) {
		if (r.begin==begin || r.end==end) return;
		std::copy(r.begin, r.end, begin);
	}
	
	void clone_to(sequence& r) {
		r.clone_from(*this);
	}
	
	void zero() {
		// for_each(begin, end, assign_zero());
		std::fill(begin, end, type());
	}
	
	sequence& operator =(const sequence& r) {
		if (this!=&r) {
			assert(r.end-r.begin>0);
			this->begin=r.begin;
			this->end=r.end;
		}
		return *this;
	}
	
	bool operator ==(const sequence& r) const {
		return this->begin==r.begin && this->end==r.end;
	}
	
	bool operator !=(const sequence& r) const {
		return !(*this == r);
	}
	
	
	type dot(sequence& r) {
		type sum=0;
		return accumulate(begin, end, r.begin, sum, multiply());
	}
	
	type operator *(sequence& r) {
		return this->dot(r);
	}
	
	#define macro_new_operator(op, op_name) 	\
	template<class T>						\
	struct op_name {						\
		const T& t;						\
		op_name(const T& t):t(t) {}			\
		inline void operator()(type& x) {		\
			x op t;						\
		}								\
	};									\
										\
	template<class T>						\
	sequence& operator op (const T& t) {		\
		std::for_each(begin, end, op_name<T>(t));	\
		return *this;						\
	}
	macro_new_operator(+=, op_addby)
	macro_new_operator(-=, op_subby)
	macro_new_operator(*=, op_multiplyby)
	macro_new_operator(/=, op_divideby)
	#undef macro_new_operator
	
	#define macro_new_operator(op1, op2, op_name)	\
	struct op_name {						\
		inline type operator()(type& x, type& y) {	\
			return x op1 y;					\
		}								\
	};									\
										\
	sequence& operator op2 (const sequence& r) {	\
		std::transform(begin, end, r.begin, begin, op_name());\
		return *this;						\
	}
	macro_new_operator (+, +=, op_eachaddby)
	macro_new_operator(-, -=, op_eachsubby)
	macro_new_operator(*, *=, op_eachmultiplyby)
	macro_new_operator(/, /=, op_eachdivideby)
	#undef macro_new_operator
	
	
	
	iterator first() {
		return this->beign;
	}
	
	iterator last() {
		return this->end;
	}
	
	type& at(int i) {
		uint len=size();
		assert(len!=0);
		while (i<0) i+=len;
		while (i>len-1) i-=len;
		return *(begin+i);
	}
	
	type& operator [] (int i) {
		return at(i);
	}
	
	int row_diff(const sequence& r) {
		return (begin-r.begin)/size();
	}
	
	type sum() {
		type init=0;
		return accumulate(begin, end, init);
	}
	
	type square_sum() {
		return pow_sum(2);
	}
	
	type cube_sum() {
		return pow_sum(3);
	}
	
	type pow_sum(int p) {
		type sum=0;
		return accumulate(begin, end, sum, power(p));
	}
	
	type sqrt_sum() {
		type sum=0;
		return accumulate(begin, end, sum, square_root());
	}
	
	struct power {
		double p;
		power(double p): p(p) {}
		inline type operator()(type& init, type& x) {
			return init+pow((double)x, p);
		}
	};
	
	struct assign_zero {
		inline void operator()(type& x) {
			x=type();
		}
	};
	
	
	struct square_root {
		inline type operator()(type& init, type& x) {
			return init+sqrt(x);
		}
	};
	
	struct multiply {
		inline type operator()(type& init, type& a, type& b) {
			return init+a*b;
		}
	};
	
	struct diff_square {
		inline type operator()(type& init, type& a, type& b) {
			return init+(a-b)*(a-b);
		}
	};
	
	uint size() {
		return end-begin;
	}
	
	double mo() {
		return sqrt(this->square_sum());
	}
	
	type cosine(sequence& y) {
		return 1.0 * this->dot(y) / this->mo() / y.mo();
	}
	
	type euler(sequence& r) {
		type sum=0;
		accumulate(begin, end, r.begin, sum, diff_square());
		return sqrt(sum);
	}
	
	type euler_square(sequence& r) {
		type sum=0;
		return accumulate(begin, end, r.begin, sum, diff_square());
	}
	
	type pearson(sequence& y) {
		sequence& x=*this;
		uint len=size();
		type sum1=x.sum();
		type sum2=y.sum();
		type sqsum1=x.square_sum();
		type sqsum2=y.square_sum();
		type dotsum=x.dot(y);
		type num=dotsum-(sum1*sum2/len);
		type den=sqrt((sqsum1-pow(sum1, 2)/len) * (sqsum2-pow(sum2, 2)/len));
		return den==0 ? 0 : 1.0-num/den;
	}
};

template<class iterator>
class matrix2d {
	iterator begin;
	iterator end;
	uint rows;
	uint cols;
public:
	typedef typename iterator_traits<iterator>::value_type type;
	matrix2d(iterator first, uint rows, uint cols): begin(first), end(rows*cols+first), rows(rows), cols(cols) {}
	matrix2d(datatable<type>& d): begin(d.begin()), end(d.end()), rows(d.row()), cols(d.col()) {}
	
	type& at(int row, int col) {
		uint len=size();
		assert(len!=0);
		while (row<0) row+=rows;
		while (row>rows-1) row-=rows;
		while (col<0) col+=cols;
		while (col>cols-1) col-=cols;
		return *(begin+cols*row+col);
	}
	
	void dot(matrix2d& y, matrix2d& res) {
		matrix2d& x=*this;
		assert(x.cols==y.rows && x.rows==res.rows && y.cols==res.cols);
		for (uint i=0; i<res.rows; ++i) {
			for (uint j=0; j<res.cols; ++j) {
				res.at(i, j)=0;
				for (uint k=0; k<x.cols; ++k) {
					res.at(i, j) += x.at(i, k) * y.at(k, j);
				}
			}	
		}
	}
	
	//¾ØÕóµÄÖÈ
	//ÌØÕ÷¾ØÕó
	//Õý¶¨Õó
	//µ¥Î»Õó
	//0¾ØÕó
	
	void zeros(type val=0) {
		for (uint i=0; i<rows; ++i) {
			for (uint j=0; j<cols; ++j) {
				at(i, j)=val;
			}
		}
	}
	
	void E() {
		assert(rows==cols);
		for (uint i=0; i<rows; ++i) {
			for (uint j=0; j<cols; ++j) {
				at(i, j)= ((i==j)?1:0);
			}
		}
	}
	
	void T() {
		assert(rows==cols);
		for (uint i=0; i<rows; ++i) {
			for (uint j=0; j<i; ++j) {
				std::swap(at(i, j), at(j, i));
			}
		}
	}
	
	bool reshape(uint width, uint height) {
		if (width*height!=size()) return false;
		rows=height;
		cols=width;
	}
	
	uint row() {
		return rows;
	}
	
	sequence<iterator> seq_row(int row) {
		while (row<0) row+=rows;
		while (row>rows-1) row-=rows;
		return sequence<iterator>(&at(row, 0), &at(row, -1)+1);
	}
	
	sequence<iterator> operator [](int row) {
		return seq_row(row);
	}
	
	vector<type> vec_row(int row) {
		while (row<0) row+=rows;
		while (row>rows-1) row-=rows;
		vector<type> tmp(&at(row, 0), &at(row, -1)+1);
		return tmp;
	}
	
	uint col() {
		return cols;
	}
	
	vector<type> vec_col(int col) {
		while (col<0) col+=cols;
		while (col>cols-1) col-=cols;
		vector<type> tmp(rows);
		for_n(i, rows) tmp.push_back(at(i, col));
		return tmp;
	}
	
	uint size() {
		return end-begin;
	}
};



#endif /* ML_TYPES_HPP_BY_JADESOUL */
