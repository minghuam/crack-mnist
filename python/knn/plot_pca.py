import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from itertools import product
from sklearn.decomposition import RandomizedPCA
from sklearn.decomposition import PCA
from sklearn.datasets import fetch_mldata
from sklearn.utils import shuffle
 
mnist = fetch_mldata("MNIST original")
x_train = mnist.data/255.0
y_train = mnist.target
 
x_train, y_train = shuffle(x_train, y_train)
x_train, y_train = x_train[:6000], y_train[:6000]

print "PCA...",
pca = RandomizedPCA(n_components = 2)
#pca = PCA(n_components = 2)
print "done..."

fig, plots = plt.subplots(10, 10)
plt.prism()
for i in xrange(10):
	for j in xrange(10):
		if i > j:
			continue
		
		print "transform and draw ({0},{1})".format(i, j)
		
		x = x_train[(y_train == i) + (y_train == j)]
		y = y_train[(y_train == i) + (y_train == j)]
		
		xt = pca.fit_transform(x)

		plots[i, j].scatter(xt[:, 0], xt[:, 1], c = y, marker = 'o', lw = 0, s = 2)
		plots[i, j].set_xticks(()); plots[i, j].set_yticks(())
		
		plots[j, i].scatter(xt[:, 1], xt[:, 0], c = y, marker = 'o', lw = 0, s = 2)
		plots[j, i].set_xticks(()); plots[j, i].set_yticks(())

		if i == 0:
			plots[i, j].set_title(j)
			plots[j, i].set_ylabel(j)

plt.show()