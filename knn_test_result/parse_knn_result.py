import struct
import numpy as np
import matplotlib.pyplot as plt
from array import array


fail_file = "failed.txt"
mnist_test_file = "../mnist/t10k-images.idx3-ubyte"

def load_mnist_test_images(file):

	with open(file) as fr:
		magic, size, width, height = struct.unpack(">IIII", fr.read(16))
		if magic != 2051:
			raise ValueError('wrong magic numbre, 2501 expected!')

		image_data = array("B", fr.read())

	images = []
	for i in xrange(size):
		d = np.array(image_data[i*width*height : (i+1)*width*height], dtype=np.uint8)
		images.append(255 - d.reshape((width, height)))

	return images

def save_pgm(file, data, inverted = False):
	fout = open(file, 'wb')
	header = 'P5\n# created by minghuam\n' + str(data.shape[1]) + ' ' + str(data.shape[0]) + ' ' + str(255) + '\n'
	fout.write(header)
	data_to_draw = data;
	if inverted:
		data_to_draw = 255 - data
	fout.write(data_to_draw.tobytes())
	fout.close()

def tile_images(images, rows, cols):
	size = len(images)
	width = images[0].shape[1]
	height = images[0].shape[0]
	index = 0
	big_img = np.zeros((rows*height, cols*width))
	for r in xrange(rows):
		for c in xrange(cols):
			if index < size:
				big_img[r*height:(r+1)*height, c*width:(c+1)*width] = images[index]
				index += 1
			else:
				big_img[r*height:(r+1)*height, c*width:(c+1)*width] = zeros((height, width))
	return big_img

images = load_mnist_test_images(mnist_test_file)

#print images[0].dtype
#save_pgm("test.pgm", images[0]);

wrong_images = []
with open(fail_file) as fr:
	for line in fr.readlines():
		words = line.split(' ')[1].split(',')
		index = int(words[0])
		label = int(words[1])
		predicted = int(words[2])
		#file = "{0}_{1}_{2}.pgm".format(index, label, predicted)
		#save_pgm(file, images[index], width, height)
		wrong_images.append(images[index])

big_img = tile_images(wrong_images, 6, 5)

plt.imshow(big_img, cmap='gray')
plt.show()