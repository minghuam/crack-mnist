/*
* MNIST Data loader
* http://yann.lecun.com/exdb/mnist/
*/

#include <iostream>
#include <fstream>

class MnistLoader{
private:
	int _train_size;
	int _test_size;
	int _width;
	int _height;
	bool _loaded;
	unsigned char *_train_data;
	unsigned char *_train_labels;
	unsigned char *_test_data;
	unsigned char *_test_labels;

	void init(){
		_train_size = 0;
		_test_size = 0;
		_width = 0;
		_height = 0;
		_loaded = false;
		_train_data = NULL;
		_train_labels = NULL;
		_test_data = NULL;
		_test_labels = NULL;
	}

	int bytes2int32(char *bytes){
		int ret = ((unsigned char)bytes[0] << 24) | \
					((unsigned char)bytes[1] << 16) | \
					((unsigned char)bytes[2] << 8) | \
					(unsigned char)bytes[3];
		return ret;
	}

	unsigned char* load_file(const char *file, int &size, int &width, int &height){
		size = 0;
		width = 0;
		height = 0;
		std::ifstream fs;
		fs.open(file, std::ios::in | std::ios::binary);
		if(!fs.is_open()){
			std::cout << "failed to open " << file << std::endl;
			return NULL;
		}

		char temp[4];
		// magic header: 4 bytes
		fs.read(temp, 4);
		int header = bytes2int32(temp);

		if(header != 2051 && header != 2049){
			std::cout << "invalid header number for " << file \
					<< ": " <<header << ". 2049 or 2051 expected" << std::endl;
			fs.close();
			return NULL;
		}

		// size
		fs.read(temp, 4);
		size = bytes2int32(temp);

		unsigned char* data = NULL;
		int data_size = 0;
		// width & heigt for image
		if(header == 2051){
			fs.read(temp, 4);
			width = bytes2int32(temp);
			fs.read(temp, 4);
			height = bytes2int32(temp);
			data_size = size * width * height;
		}else{
			data_size = size;
		}

		// data
		data = new unsigned char[data_size];
		fs.read((char*)data, data_size);

		fs.close();
		return data;
	}

public:
	MnistLoader(){
		init();
	}

	int width() const { return _width; }
	int height() const { return _height; }
	int feature_size() const { return _width * _height; }
	int test_size() const { return _test_size; }
	int train_size() const { return _train_size; }
	const unsigned char* train_data() const { return _train_data; }
	const unsigned char* train_labels() const { return _train_labels; }
	const unsigned char* test_data() const { return _test_data; }
	const unsigned char* test_labels() const {return _test_labels; }

	~MnistLoader(){
		// clean up
		if(_train_data != NULL){
			delete [] _train_data;
			_train_data = NULL;
		}
		if(_train_labels != NULL){
			delete [] _train_labels;
			_train_labels = NULL;
		}
		if(_test_data != NULL){
			delete [] _test_data;
			_test_data = NULL;
		}
		if(_test_labels != NULL){
			delete [] _test_labels;
			_test_labels = NULL;
		}

		init();
	}

	int load(const char *train_file, const char *train_label_file, \
		const char *test_file, const char *test_label_file){

		if(_loaded){
			return -1;
		}
		int dummy_size, dummy_width, dummy_height;
		_train_data = load_file(train_file, _train_size, _width, _height);
		_train_labels = load_file(train_label_file, dummy_size, dummy_width, dummy_height);
		_test_data = load_file(test_file, _test_size, dummy_width, dummy_height);
		_test_labels = load_file(test_label_file, dummy_size, dummy_width, dummy_height);
	
		if(_train_data == NULL || _train_labels == NULL ||\
			_test_data == NULL || _test_labels == NULL){
			return -1;
		}

		return 0;
	}
};