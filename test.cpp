#include <iostream>
#include <fstream>
#include "mnist_loader.hpp"
#include "util.hpp"

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
	for(int i=0; i<10; i++){
		char buf[128];
		sprintf(buf, "%d.pgm", i);
		Util::save_pgm(buf, data + w*h*i, w, h, true);
		cout << (int)labels[i] << endl;
	}
}