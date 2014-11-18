/*
*	KNN classifier
*/

#include <iostream>
#include <cmath>
#include <vector>
#include <queue>
#include <map>
#include <thread>
#include <cfloat>
#include <chrono>

class KNN{

private:
	int _k;
	float euclidean_distance(const unsigned char* t1, \
		const unsigned char* t2, int len){
		int ret = 0;
		int temp;
		for(int i=0; i<len; i++){
			temp = (int)t1[i] - (int)t2[i];
			ret += temp*temp;
		}
		return sqrt((float)ret);
	}

	struct Neighbor
	{
		int label;
		int index;
		float distance;
		Neighbor(int lab, int ind, float dist): label(lab), index(ind), distance(dist) {}
	};

	struct NeighborCompare
	{
		bool operator()(const Neighbor &n1, const Neighbor &n2){
			return n1.distance < n2.distance;
		}
	};

	struct NeighborCluster
	{
		int count;
		int label;
		float min_distance;
		NeighborCluster(){
			count = 0;
			label = -1;
			min_distance = FLT_MAX;
		}
		NeighborCluster(int lab, float dist){
			count = 0;
			label = lab;
			min_distance = dist;
		}
	};

	typedef std::priority_queue<Neighbor, std::vector<Neighbor>, NeighborCompare> NeighborQueue;
	NeighborQueue _neighbors;

	void classify(int *correct_count, const unsigned char* test_data, int test_start, int test_size, \
		const unsigned char *train_data, int train_size, int feature_size, \
		const unsigned char *train_labels, const unsigned char *test_labels){

		*correct_count = 0;

		for(int i = test_start; i < test_start + test_size; i++){
			for(int j = 0; j < train_size; j++){
				float dist = euclidean_distance(test_data + i*feature_size, train_data + j*feature_size, feature_size);
				//std::cout << j << "," << dist << std::endl;
				Neighbor neighbor(train_labels[j], j, dist);
				_neighbors.push(neighbor);
				if(_neighbors.size() > _k){
					_neighbors.pop();
				}
			}

			// vote
			std::map<int, NeighborCluster> nm;
			while(_neighbors.size()){
				Neighbor n = _neighbors.top();
				_neighbors.pop();
				//std::cout << n.label << "," << n.index << "," << n.distance << std::endl;
				if(nm.find(n.label) == nm.end()){
					nm[n.label] = NeighborCluster(n.label, n.distance);
				}
				nm[n.label].count += 1;
				nm[n.label].min_distance = std::min(nm[n.label].min_distance, n.distance);
			}

			std::map<int, NeighborCluster>::iterator it = nm.begin();
			NeighborCluster nc = it->second;
			//std::cout << nc.label << "," << nc.min_distance << std::endl;
			++it;
			for(; it != nm.end(); ++it){

				//std::cout << it->second.label << "," << it->second.min_distance <<  std::endl;
				if(it->second.count > nc.count){
					nc = it->second;
				}else if(it->second.count == nc.count && \
					it->second.min_distance < nc.min_distance){
					nc = it->second;
				}
			}
			//std::cout << nc.label << "," << nc.count << "," << nc.min_distance << "," << (int)test_labels[i] << std::endl;

			if(nc.label == test_labels[i]){
				(*correct_count)++;
			}else{
				std::cout << "index/true/predicted " << i << "," \
				<< (int)test_labels[i] << "," << nc.label << std::endl;
			}

			//std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}

		//float accuracy = (float)(*correct_count)/test_size;
		//std::cout << "Accuracy: " << accuracy << std::endl;
	}

public:
	KNN() { KNN(1); }
	KNN(int k): _k(k) {}


	float batch_classify(int nbatch, const unsigned char* test_data, int test_size, \
		const unsigned char *train_data, int train_size, int feature_size, \
		const unsigned char *train_labels, const unsigned char *test_labels){

		using namespace std::chrono;
		high_resolution_clock::time_point start_time = high_resolution_clock::now();

		int patch_size = test_size/nbatch;
		std::vector<std::thread* > _workers;
		int *correct_count = new int[nbatch];
		int cnt;
		int test_start = 0;
		for(int i = 0; i < nbatch; i++){
			int size = patch_size;
			if(i == nbatch-1){
				size = test_size - patch_size*(nbatch - 1);
			}
			std::thread *t = new std::thread(&KNN::classify, *this, \
				&correct_count[i], test_data, test_start, size, \
				train_data, train_size, feature_size, \
				train_labels, test_labels \
				);
			test_start += size;

			_workers.push_back(t);
		}

		for(int i = 0; i < nbatch; i++){
			_workers[i]->join();
		}

  		high_resolution_clock::time_point end_time = high_resolution_clock::now();
  		duration<double> time_span = duration_cast<duration<double>>(end_time - start_time);

		for(int i = 0; i < nbatch; i++){
			std::cout << "batch: " << i << " , " << correct_count[i] \
			<< "/" << patch_size << std::endl;
		}
	
		int correct_sum = 0;
		for(int i = 0; i < nbatch; i++){
			correct_sum += correct_count[i];
		}
		delete [] correct_count;
		float accuracy = (float)correct_sum/test_size;

		std::cout << "total: " << correct_sum << "/" << test_size << std::endl;
		std::cout << "accuracy: " << accuracy << std::endl;
		std::cout << "time: " << time_span.count() << " seconds." << std::endl;

		return accuracy;
	}

};