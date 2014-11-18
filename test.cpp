#include <iostream>
#include <fstream>
#include "mnist_loader.hpp"
#include "util.hpp"
#include "knn.hpp"

using namespace std;

const char* train_file = "./mnist/train-images.idx3-ubyte";
const char* train_labels = "./mnist/train-labels.idx1-ubyte";
const char* test_file = "./mnist/t10k-images.idx3-ubyte";
const char* test_labels = "./mnist/t10k-labels.idx1-ubyte";

int main(int argc, char **argv){
	MnistLoader loader;
	loader.load(train_file, train_labels, test_file, test_labels);

	const unsigned char* data = loader.train_data();
	const unsigned char* labels = loader.train_labels();
	int w = loader.width();
	int h = loader.height();

	//char buf[128];
	//sprintf(buf, "test%d.pgm", 0);
	//Util::save_pgm(buf, loader.test_data() + w*h*0, w, h, true);

	KNN knn(3);
	knn.batch_classify(12, loader.test_data(), loader.test_size(), \
		loader.train_data(), loader.train_size(), 
		loader.feature_size(), \
		loader.train_labels(), loader.test_labels());

}