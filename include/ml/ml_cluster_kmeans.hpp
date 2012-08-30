#ifndef ML_CLUSTER_KMEANS_HPP_BY_JADESOUL
#define ML_CLUSTER_KMEANS_HPP_BY_JADESOUL
/**
 * File: ml_cluster.hpp
 * Description: 
 *
 * Copyright (c) 2011 Jadesoul (Home Page: http://jadesoul.org)
 * Licensed under the GNU General Public License:
 * http://www.gnu.org/licenses/gpl.html
 * 
 * Date: 2011-05-07 13:28:15.435000
 * Written In: Peking University, beijing, China$
 */


#include "ml_types.hpp"

enum {
	ML_DIST_PEARSON,
	ML_DIST_COSINE,
	ML_DIST_EULER,
	ML_DIST_TOTAL_COUNT
};

enum {
	ML_LINK_FULL,
	ML_LINK_SINGLE,
	ML_LINK_TOTAL_COUNT
};

template<class sample>
class cluster {
public:
	vector<sample> samples;
	int distype, linktype;
	cluster(int distype=0, int linktype=0):distype(distype), linktype(linktype) {
		
	}
	
	double distance(const cluster& r) {
		//TODO
		return 0.0;
	}
};

template<class type>
class kmeans {
public:
	typedef datatable<type> 	data_t;
	typedef type* 				iterator_t;
	typedef sequence<iterator_t> sample_t;
	typedef matrix2d<iterator_t> 	samples_t;
	typedef cluster<sample_t> 	cluster_t;
	typedef vector<cluster_t> 	clusters_t;
	typedef vector<sample_t> 	centers_t;
	typedef vector<uint> 		result_t;
	
	kmeans(data_t& data, uint k=2, int distype=0):samples(samples_t(data)), k(k), distype(distype) {
		assert(samples.row()>=k);
		total=samples.row();
		isassigned.assign(total, false);
		log("samples count=%d", total);
		buf=data_t(k*2, data.col());
		samples_t buf_samples(buf);
		for_n(i, k) {
			centers.push_back(buf_samples[i]);
			old_centers.push_back(buf_samples[i+k]);
		}
		iterationcnt=0;
	}
	
	void setk(uint k) {
		this->k=k;
	}
	
	void setdistype(int dtype) {
		distype=dtype;
	}
	
	result_t kcluster() {
		generate_random_centers();
		do {
			assign_samples_to_clusters();
			caculate_new_centers();
		} while (! is_centers_nearly_not_change());
		assign_samples_to_clusters();
		log("kmeans finished");
		result_t result(total, 0);
		for_n(i, total) {
			uint l=clusters[i].samples.size();
			for_n(j, l) result[ clusters[i].samples[j].row_diff(samples[0]) ]=i;
		}
		return result;
	}
	
	type distance(sample_t& a, sample_t& b) {
		if (distype==ML_DIST_PEARSON) return a.pearson(b);
		if (distype==ML_DIST_COSINE) return -a.cosine(b);//cosine the bigger the more similiar
		if (distype==ML_DIST_EULER) return a.euler_square(b);
	}
	
	//random generate k centers
	void generate_random_centers() {
		clusters.assign(k, cluster_t());
		for_n(j, k) {
			uint i;
			do {
				i=rand() % total;
			} while(is_in(samples[i], centers));
			centers[j].clone_from(samples[i]);
			clusters[j].samples.push_back(samples[i]);
			isassigned[i]=true;
		}
	}
	
	void assign_samples_to_clusters() {
		for_n(i, k) clusters[i].samples.clear();
		
		for_n(i, total) {
			if (isassigned[i]==false) {
				uint nearest_cluster_id=0;
				type dis=distance(samples[i], centers[0]);
				for (uint j=1; j<k; ++j) {
					type newdis=distance(samples[i], centers[j]);
					if (newdis<dis) {
						nearest_cluster_id=j;
						dis=newdis;
					}
				}
				clusters[nearest_cluster_id].samples.push_back(samples[i]);
				isassigned[i]=true;
			}
		}
		isassigned.assign(total, false);
	}
	
	void caculate_new_centers() {
		//if one cluster has no sample, then reset the randam centers
		bool all_has_samples=true;
		for_n(i, k) if (clusters[i].samples.size()==0) all_has_samples=false;
		if (all_has_samples==false) {
			generate_random_centers();
			return;
		}
		
		for_n(i, k) old_centers[i].clone_from(centers[i]);
		for_n(i, k) {
			centers[i].zero();
			uint l=clusters[i].samples.size();
			for_n(j, l) centers[i]+=clusters[i].samples[j];
			centers[i] /= l;
		}
	}
	
	bool is_centers_nearly_not_change() {
		++iterationcnt;
		type sum=0;
		for_n(i, k) sum+=centers[i].euler_square(old_centers[i]);
		log("after the %d th iteration, diff=%f", iterationcnt, sum);
		return sum<0.001;
	}
	
protected:
	samples_t samples;
	vector<bool> isassigned;
	uint k, total;
	int distype;
	clusters_t clusters;
	centers_t centers, old_centers;
	data_t buf;
	uint iterationcnt;
};


#endif /* ML_CLUSTER_KMEANS_HPP_BY_JADESOUL */
