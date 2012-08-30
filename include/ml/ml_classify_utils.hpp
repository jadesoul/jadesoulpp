#ifndef ML_CLASSIFY_UTILS_HPP_1335035059_15
#define ML_CLASSIFY_UTILS_HPP_1335035059_15
/**
 * File: ml_classify_utils.hpp
 * Description: 
 *
 * Copyright (c) 2012 Jadesoul (Home Page: http://jadesoul.sinaapp.com)
 * 
 * Date: 2012-04-22 03:04:19.149000
 * Written In: Peking University, beijing, China
 */

#include <jadesoul.hpp>
#include "ml_types.hpp"


template<class T>
class DistributionMapping {
private:
	DistributionMapping(const DistributionMapping& r) {}
	DistributionMapping& operator=(const DistributionMapping& r) const {}
public:
	list<T>* pValueList;
	set<T> valueSet;
	list<T> uniquedValueList;
	map<T, uint> distMap;//mapping value to id, so value==uniquedValueList[id]
	
	virtual void update()=0;
	
	// inline const list<str> getStringVersionOfUniquedValueList() {
		// list<str> ss;
		// uint n=uniquedValueList.size();
		// for_n(i, n) ss.append(uniquedValueList[i]);
		// return ss;
	// }
};

template<class T>
class DiscreteDistributionMapping : public DistributionMapping<T> {
private:
	DiscreteDistributionMapping(const DiscreteDistributionMapping& r) {}
	DiscreteDistributionMapping& operator=(const DiscreteDistributionMapping& r) const {}
public:
	DiscreteDistributionMapping(const list<T>& values) {
		this->pValueList=&values;
		update();
	}
	
	void update() {
		list<T>& valueList=*this->pValueList;
		uint n=valueList.size();
		for_n(i, n) this->valueSet.add(valueList[i]);
		uint i=0;
		for_iter(it, set<T>, this->valueSet) this->uniquedValueList.append(*it);
		n=this->uniquedValueList.size();
		for_n(i, n) this->distMap[this->uniquedValueList[i]]=i;
	}
	
	T& id_to_value(uint id) {
		assert(id>=0 AND id<this->uniquedValueList.size());
		return this->uniquedValueList[id];
	}
	
	uint value_to_id(const T& value) {
		assert(is_in(value, this->valueSet));
		return this->distMap[value];
	}
};

template<class T, class DiscretizationFunction>
class ContinuousDistributionMapping : public DistributionMapping<T> {
private:
	ContinuousDistributionMapping(const ContinuousDistributionMapping& r) {}
	ContinuousDistributionMapping& operator=(const ContinuousDistributionMapping& r) const {}
public:
	DiscretizationFunction func;
	
	ContinuousDistributionMapping(const list<T>& values, DiscretizationFunction func)
		: func(func) {
		this->pValueList=&values;
		update();
	}

	void update(const list<T>& values) {
		this->valueList=values;
		uint n=this->valueList.size();
		for_n(i, n) this->valueSet.add(func(this->valueList[i]));
		uint i=0;
		for_iter(it, set<T>, this->valueSet) this->uniquedValueList.append(*it);
		n=this->uniquedValueList.size();
		for_n(i, n) this->distMap[this->uniquedValueList[i]]=i;
	}
	
	//note this will not return the real value but the valueAfterDiscretization
	T& id_to_value(uint id) {
		assert(id>=0 AND id<this->uniquedValueList.size());
		return this->uniquedValueList[id];
	}
	
	uint value_to_id(const T& value) {
		T valueAfterDiscretization=func(value);
		assert(is_in(valueAfterDiscretization, this->valueSet));
		return this->distMap[valueAfterDiscretization];
	}
	
};

template<class T>
class FrequencyDistribution {
public:
	list<T> valueList;
	set<T> valueSet;
	list<T> uniquedValueList;
	map<T, uint> freqMap;
	
	FrequencyDistribution() {}
	FrequencyDistribution(const list<T>& data):valueList(data) { update_freq_dist(); }
	
	void update() {
		update_freq_dist();
	}
	
	template<class K, class V>
	struct compare_by_map_second {
		const bool operator() (typename map<K, V>::iterator a, typename map<K, V>::iterator b) const {
			return a->second < b.second;
		}
	};
	
	pair<T, uint> max() {
		return * find_iter_max(freqMap.begin(), freqMap.end(), compare_by_map_second<T, uint>());
	}
	
	pair<T, uint> min() {
		return *find_iter_min(freqMap.begin(), freqMap.end(), compare_by_map_second<T, uint>());
	}
protected:
	void update_freq_dist() {
		uint n=valueList.size();
		for_n(i, n) valueSet.add(valueList[i]);
		uint i=0;
		for_iter(it, set<T>, valueSet) uniquedValueList.append(*it);
		n=uniquedValueList.size();
		for_n(i, n) freqMap[uniquedValueList[i]]=0;
		n=valueList.size();
		for_n(i, n) ++freqMap[valueList[i]];
	}
};

template<class T>
class ProbabilityDistribution : public FrequencyDistribution<T> {
public:
	map<T, float> probMap;
	ProbabilityDistribution() {}
	ProbabilityDistribution(const list<T>& data):FrequencyDistribution<T>(data) {
		update_prob_dist();
	}
	
	void update() {
		this->update_freq_dist();
		this->update_prob_dist();
	}
	
private:
	void update_prob_dist() {
		uint n=this->uniquedValueList.size(), total=this->valueList.size();
		for_n(i, n) {
			T& v=this->uniquedValueList[i];
			probMap[v]=1.0*this->freqMap[v]/total;
		}
	}
	
};


enum DataType {
	DATA_TYPE_DISCRETE,
	DATA_TYPE_CONTINUOUS,
	TOTAL_DATA_TYPE
};

enum ValueType {
	VALUE_TYPE_INT,
	VALUE_TYPE_FLOAT,
	VALUE_TYPE_DOUBLE,
	VALUE_TYPE_STRING,
	TOTAL_VALUE_TYPE
};

template<class T>
class BasicDiscretizationFunction {
public:
	uint operator() (const T& v) {
		return uint(v);
	}
};

//mapping the each value in the data table to a uint id
class DataColumeIDsProvider {
public:
	str title;
	list<uint> ids;
	
	virtual void update_ids()=0;
};

template<class T=int, enum ValueType valueType=VALUE_TYPE_INT>
class DataColume : public DataColumeIDsProvider {
private:
	DataColume(const DataColume& r) {}
	DataColume& operator=(const DataColume& r) const {}
public:
	list<T> data;
	DistributionMapping<T>* pMapping;
	
	DataColume():pMapping(NULL) {}
	
	//shuold be called after the data is ready
	void update_ids() {
		free();
		if (VALUE_TYPE_FLOAT==valueType OR VALUE_TYPE_DOUBLE==valueType)
			pMapping=new ContinuousDistributionMapping<T, BasicDiscretizationFunction<T> >(data, BasicDiscretizationFunction<T>());
		else 
			pMapping=new DiscreteDistributionMapping<T>(data);
		
		ids.clear();
		uint n=data.size();
		for_n(i, n) ids.append(pMapping->value_to_id(data[i]));
	}
	
	void free() {
		if (NULL!=pMapping) {
			delete pMapping;
			pMapping=NULL;
		}
	}
	~DataColume() {
		free();
	}
};



class DataSource {
public:
	list<str> titles;
	list<str> types;
	list<ValueType> valueTypeList;
	list<list<string> > rawDataTable;
	uint rowCnt;
	uint colCnt;
	list<DataColumeIDsProvider*> dataColumeIDsProviderPtrList;
	
	DataSource(istream& in=cin) {
		istringstream iss;
		rowCnt=0;
		colCnt=0;
		
		const uint BUF_SIZE=8*1024;
		char* buf=new char[BUF_SIZE+1];
		assert(in.getline(buf, BUF_SIZE));
		iss.str(buf);

		string value;
		while (iss>>value) {
			++colCnt;
			titles.append(value);
		}
		assert(titles[-1]=="class");
		assert(in.getline(buf, BUF_SIZE));
		iss.str(buf);
		uint rcnt=0;
		while (iss>>value) {
			++rcnt;
			types.append(value);
		}
		assert(rcnt==colCnt);
		for_n(i, colCnt) {
			str type=types[i];
			if (type=="int") valueTypeList.append(VALUE_TYPE_INT);
			else if (type=="float") valueTypeList.append(VALUE_TYPE_FLOAT);
			else if (type=="double") valueTypeList.append(VALUE_TYPE_DOUBLE);
			else if (type=="string") valueTypeList.append(VALUE_TYPE_STRING);
			else assert(false);
		}
		while (in.getline(buf, BUF_SIZE)) {
			++rowCnt;
			iss.str(buf);
			list<string> rowValues;
			uint rcnt=0;
			while (iss>>value) {
				++rcnt;
				rowValues.append(value);
			}
			assert(rcnt==colCnt);
			rawDataTable.append(rowValues);
		}
		
		delete [] buf;
		update_data_colume_list();
	}
	
	void update_data_colume_list() {
		for_n(c, colCnt) {
			DataColumeIDsProvider* pdc;
			if (valueTypeList[c]==VALUE_TYPE_INT) {
				typedef DataColume<int, VALUE_TYPE_INT> DataColumeType;
				pdc=new DataColumeType();
				pdc->title=titles[c];
				for_n(r, rowCnt) {
					string& value=rawDataTable[r][c];
					istringstream iss(value);
					int v;
					iss>>v;
					(reinterpret_cast<DataColumeType>(pdc))->data.append(v);
				}
			} else if (valueTypeList[c]==VALUE_TYPE_FLOAT) {
				pdc=new DataColume<float, VALUE_TYPE_FLOAT>();
				pdc->title=titles[c];
				for_n(r, rowCnt) {
					string& value=rawDataTable[r][c];
					istringstream iss(value);
					float v;
					iss>>v;
					pdc->data.append(v);
				}
			} else if (valueTypeList[c]==VALUE_TYPE_DOUBLE) {
				pdc=new DataColume<double, VALUE_TYPE_DOUBLE>();
				pdc->title=titles[c];
				for_n(r, rowCnt) {
					string& value=rawDataTable[r][c];
					istringstream iss(value);
					double v;
					iss>>v;
					pdc->data.append(v);
				}
			} else if (valueTypeList[c]==VALUE_TYPE_STRING) {
				pdc=new DataColume<string, VALUE_TYPE_STRING>();
				pdc->title=titles[c];
				for_n(r, rowCnt) {
					string& value=rawDataTable[r][c];
					pdc->data.append(value);
				}
			} else assert(false);
			pdc->update_ids();
			dataColumeIDsProviderPtrList.append(pdc);
		}
	}
	
	str& getDataColumeName(uint i) {
		if (i==-1) i=colCnt-1;
		DataColumeIDsProvider& dp=*dataColumeIDsProviderPtrList[i];
		return dp.title;
	}
	
	inline list<uint>& getDataColumeIDs(uint i) {
		if (i==-1) i=colCnt-1;
		DataColumeIDsProvider& dp=*dataColumeIDsProviderPtrList[i];
		return dp.ids;
	}
	
	inline str getRawValue(uint row, uint col) {
		if (row<0) row+=rowCnt;
		if (col<0) col+=colCnt;
		return rawDataTable[row][col];
	}
	
	uint row() { return rowCnt; }
	uint col() { return colCnt; }
};

//note that the feature is already discreted by the DataSourceIDsProvider
typedef uint Feature;
typedef uint Class;
typedef list<Feature> FeatureVector;

class FeatureMeta {
public:
	str name;
	double weight;
	list<Feature> ids;
	list<Feature> uids;
	ProbabilityDistribution<Feature> probDist;
};

class ClassMeta {
public:
	str name;
	list<Class> ids;
	list<Class> uids;//uniqued class ids
	ProbabilityDistribution<Class> probDist;
};

typedef list<FeatureMeta> FeatureMetaVector; //should be the same size as FeatureVector

class Meta { //should be same size as Sample
public:
	FeatureMetaVector	featureMetas;
	ClassMeta		classMeta;
};
	
class Sample {
public:
	FeatureVector X;
	Class y;
};

typedef list<Sample> SampleList;

class DataTable;

class DataProvider {	
public:
	DataSource& dataSource;
	uint rowStart;
	uint rowStop;
	// uint colStart;
	// uint colStop;
	
	DataProvider(DataSource& dataSource, uint rowStart, uint rowStop):dataSource(dataSource), rowStart(rowStart), rowStop(rowStop) {}
	
	void provide(DataTable& dataTable);
	
	str getRawValue(uint row, uint col) {
		assert(row+rowStart<rowStop);
		dataSource.getRawValue(rowStart+row, col);
	}
};

class DataTable {
private:
	DataProvider* provider;
public:
	Meta meta;
	SampleList samples;
	
	DataTable():provider(NULL) {}
	DataTable(DataProvider& prov):provider(&prov) {
		refresh();
	}
	
	inline void refresh() {
		if (provider) provider->provide(*this);
	}
	
	inline void clear() {
		meta=Meta();
		samples=SampleList();
	}
	
	inline uint class_size() { return meta.classMeta.uids.size(); }
	inline uint samples_size() { return samples.size(); }
	inline uint features_size() { return meta.featureMetas.size(); }
	
	//return the ids of each class as list
	inline const list<Class>& class_uids() {
		return meta.classMeta.uids;
	}
	
	//return the probs of each class as list
	inline const list<float> class_probs() {
		list<float> probs;
		for_n(C, class_size()) probs.append(class_probdist().probMap[class_uids()[C]]);
	}
	
	//return the class probdist
	inline ProbabilityDistribution<Class>& class_probdist() {
		return meta.classMeta.probDist;
	}
	
	//return the feature probdist
	inline ProbabilityDistribution<Feature>& feature_probdist(uint col) {
		return meta.featureMetas[col].probDist;
	}
	
	// void from(datatable<Feature>& dt) {
		// samples.clear();
		// uint rows=dt.row();
		// for_n(i, rows) {
			// FeatureVector X(dt.rowbegin(i), dt.rowlast(i));
			// Class y=*dt.rowlast(i);
			// samples.append(Sample(X, y));
		// }
	// }
	
	// list<Class> getAllValuesOfClass() {
		// list<Class> ls;
		// uint n=samples.size();
		// for_n(i, n) ls.append(samples[i].y);
		// return ls;
	// }
	
	// list<Feature> getAllValuesOfFeature(uint id) {
		// list<Feature> fs;
		// uint n=samples.size();
		// for_n(i, n) fs.append(samples[i].X[id]);
		// return fs;
	// }
}

void DataProvider::provide(DataTable& dataTable) {
	DataTable& dt=dataTable;
	assert(rowStart<rowStop);
	assert(rowStart>=0 AND rowStart<dataSource.row());
	assert(rowStop>=0 AND rowStop<dataSource.row());
	
	uint colCnt=dataSource.col();
	assert(colCnt>1);
	
	ClassMeta& classMeta=dt.meta.classMeta;
	classMeta.name=dataSource.getDataColumeName(-1);
	classMeta.ids=dataSource.getDataColumeIDs(-1)(rowStart, rowStop+1);
	classMeta.probDist=ProbabilityDistribution<Class>(classMeta.ids);
	classMeta.uids=classMeta.probDist.uniquedValueList;
	for_n(c, colCnt-1) {
		FeatureMeta& featureMeta=dt.meta.featureMetas[c];
		featureMeta.name=dataSource.getDataColumeName(c);
		featureMeta.weight=1.0;
		featureMeta.ids=dataSource.getDataColumeIDs(c)(rowStart, rowStop+1);
		featureMeta.probDist=ProbabilityDistribution<Feature>(featureMeta.ids);
		featureMeta.uids=featureMeta.probDist.uniquedValueList;
	}
	
	uint rowCnt=rowStop-rowStart+1;
	assert(rowCnt==classMeta.ids.size());
	assert(rowCnt>0);
	dt.samples.clear();
	for_n(r, rowCnt) {
		FeatureVector X;
		for_n(c, colCnt-1) {
			FeatureMeta& featureMeta=dt.meta.featureMetas[c];
			// assert(rowCnt==featureMeta.ids.size());
			X.append(featureMeta.ids[r]);
		}
		Class y=classMeta.ids[r];
		dt.samples.append(Sample(X, y));
	}
}

class Classifier {
public:
	Classifier() {

	}
	
	virtual void train()=0;
	virtual void test()=0;
	virtual Class predict(FeatureVector& X)=0;
	
	void train(datatable<Feature>& dt) {
		this->trainData.from(dt);
		train();
	}
	
	void train(DataTable& data) {
		this->trainData=data;
		train();
	}
	
	void test(datatable<Feature>& dt) {
		this->testData.from(dt);
		test();
	}
	
	void test(DataTable& data) {
		this->testData=data;
		test();
	}

	bool validate(Sample& S) {
		return predict(S.X)==S.y;
	}
	
protected:
	DataTable trainData, testData;
};

#endif /* ML_CLASSIFY_UTILS_HPP_1335035059_15 */
