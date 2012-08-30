#ifndef ML_CLASSIFY_BAYES_HPP_BY_JADESOUL
#define ML_CLASSIFY_BAYES_HPP_BY_JADESOUL
/**
 * File: ml_classify_bayes.hpp
 * Description: 
 *
 * Copyright (c) 2011 Jadesoul (Home Page: http://jadesoul.org)
 * Licensed under the GNU General Public License:
 * http://www.gnu.org/licenses/gpl.html
 * 
 * Date: 2011-06-27 02:02:07.329000
 * Written In: Peking University, beijing, China
 */

#include "ml_classify_utils.hpp"


dt.meta.classMeta
dt.meta.classMeta.name
dt.meta.classMeta.ids
dt.meta.classMeta.uids
dt.meta.classMeta.probDist
dt.meta.classMeta.probDist.valueList		list<T>
dt.meta.classMeta.probDist.valueSet		set<T>
dt.meta.classMeta.probDist.uniquedValueList	list<T>
dt.meta.classMeta.probDist.freqMap		map<T, uint>
dt.meta.classMeta.probDist.probMap		map<T, float>

dt.meta.featureMetas[c]
dt.meta.featureMetas[c].name
dt.meta.featureMetas[c].weight
dt.meta.featureMetas[c].ids
dt.meta.featureMetas[c].uids
dt.meta.featureMetas[c].probDist

dt.samples[r].X		FeatureVector	list<Feature>
dt.samples[r].X[0]	Feature
dt.samples[r].X[1]
dt.samples[r].X[2]
dt.samples[r].y		Class

class Likehood {
public:
	Class C;
	uint	i;//the index of the feature in a feature vector of a sample
	Feature x;
	float P;//the prob
	
	friend ostream& operator<<(ostream& o, const Likehood& l) {
		return o<<"P(x_"<<i<<"="<<x<<"|C="<<C<<")";
	}
}

class FeatureLikehood {
public:
	Class C;
	uint	i;//the index of the feature in a feature vector of a sample
	//Likehood for each possible feature value
	//the size is featureMeta.uids.size()
	list<Likehood> likehoods;
	
	friend ostream& operator<<(ostream& o, const Likehood& l) {
		return o;
	}
}

class FeatureVecorLikehood {
public:
	Class C;
	//FeatureLikehood for each possible feature index
	//the size is features_cnt
	list<FeatureLikehood> featurelikehoods;
	
	friend ostream& operator<<(ostream& o, const Likehood& l) {
		return o;
	}
}

//FeatureVecorLikehood for each possible class C
//the size is class_size
typedef list<FeatureVecorLikehood> LikehoodMatrix;


class NaiveBayesClassifier : public Classifier {
private:
	uint class_size;
	list<float> class_prior_probs;
	LikehoodMatrix likehood_matrix;
public:
	void train() {
		DataTable& data=trainData;
		class_size=data.class_size();
		uint features_cnt=data.features_size();//feature size for each sample
		uint samples_cnt=data.samples_size();
		
		classes=data.class_uids();
		class_prior_probs=data.class_probs();
		
		likehood_matrix.clear();
		for_n(C, class_size) {
			// class_prior_probs[C]
			
			likehood_matrix.append(
			for_n(x, features_cnt) {
				list<str> likehood_names_for_one_feature;
				char buf[100];
				sprintf(buf,  
				
				data.feature_probdist(i);
				
			}
		}
		// data.class_probdist();
	}
	
	void test() {
		DataTable& data=testData;
	}
	
	Class predict(FeatureVector& X) {
	
	}
};


#endif /* ML_CLASSIFY_BAYES_HPP_BY_JADESOUL */

