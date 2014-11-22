"""
	http://sebastianraschka.com/Articles/2014_pca_step_by_step.html
	http://peekaboo-vision.blogspot.com/2012/12/another-look-at-mnist.html
"""

import numpy as np
import matplotlib.pyplot as plt
from sklearn.neighbors import KNeighborsClassifier
from sklearn.datasets import fetch_mldata
from sklearn.utils import shuffle
from sklearn.preprocessing import MinMaxScaler
from sklearn import cross_validation
from sklearn.decomposition import RandomizedPCA
from sklearn.decomposition import PCA
import time

print 'loading MNIST dataset...'
mnist = fetch_mldata("MNIST original")

x_all = mnist.data
y_all = mnist.target

x_all, y_all = shuffle(x_all, y_all)

x_train = x_all[:6000]
y_train = y_all[:6000]
x_test = x_all[6000:7000]
y_test = y_all[6000:7000]

knn = KNeighborsClassifier(n_neighbors = 3, metric = 'euclidean')

print "PCA fit...",
start = time.time()
#pca = RandomizedPCA(n_components = 40)
pca = PCA(n_components = 40)
pca.fit(x_train)
print '{0} seconds'.format(time.time() - start)
print np.sum(pca.explained_variance_ratio_)

plt.figure(1)
plt.title('Variance Exlained')
plt.plot(pca.explained_variance_ratio_)
plt.show(False)

print "PCA transform...",
start = time.time()
x_train_pca = pca.fit_transform(x_train)
x_test_pca = pca.transform(x_test)
print '{0} seconds'.format(time.time() - start)

print 'training...',
start = time.time()
knn.fit(x_train_pca, y_train)
print '{0} seconds'.format(time.time() - start)

print 'cross validation...',
start = time.time()
score = cross_validation.cross_val_score(knn, x_train_pca, y_train, cv = 5)
print '{0} seconds'.format(time.time() - start)
print score 

print 'testing...',
start = time.time()
y_predict = knn.predict(x_test_pca)
print '{0} seconds'.format(time.time() - start)
wrong = np.where(y_predict != y_test)[0]
print wrong

index = wrong[0]
print 'wrong example: {0} => {1}'.format(int(y_test[index]), int(y_predict[index]))
plt.rc("image", cmap="binary")
plt.subplot(1,4,1)
plt.imshow(x_test[index].reshape(28,28))
plt.title("test image")

# show three neighbors
neighbors = knn._tree.query(x_test_pca, k = 3)[1]
for i in [0, 1, 2]:
    plt.subplot(1, 4, 2 + i)
    plt.title("neighbor {0}".format(i))
    plt.imshow(x_train[neighbors[index, i]].reshape(28, 28))

plt.show()