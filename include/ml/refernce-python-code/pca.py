from PIL import Image
from numpy import *

http://www.janeriksolem.net/2009/01/pca-for-images-using-python.html

def pca(X):
  # Principal Component Analysis
  # input: X, matrix with training data as flattened arrays in rows
  # return: projection matrix (with important dimensions first),
  # variance and mean

  #get dimensions
  num_data,dim = X.shape

  #center data
  mean_X = X.mean(axis=0)
  for i in range(num_data):
      X[i] -= mean_X

  if dim>100:
      print 'PCA - compact trick used'
      M = dot(X,X.T) #covariance matrix
      e,EV = linalg.eigh(M) #eigenvalues and eigenvectors
      tmp = dot(X.T,EV).T #this is the compact trick
      V = tmp[::-1] #reverse since last eigenvectors are the ones we want
      S = sqrt(e)[::-1] #reverse since eigenvalues are in increasing order
  else:
      print 'PCA - SVD used'
      U,S,V = linalg.svd(X)
      V = V[:num_data] #only makes sense to return the first num_data

  #return the projection matrix, the variance and the mean
  return V,S,mean_X
  #projection:   dot(X,V.T) or dot(V,X.T)



def test():

    from PIL import Image
    import numpy
    import pylab

    import os
    p = r'C:\repository\research_code\gwb_cropped\gwb_cropped'

    imlist = [p+'/'+f for f in os.listdir(p) if 'jpg' in f]

    im = numpy.array(Image.open(imlist[0])) #open one image to get the size
    m,n = im.shape[0:2] #get the size of the images
    imnbr = len(imlist) #get the number of images

#create matrix to store all flattened images
    immatrix = numpy.array([numpy.array(Image.open(imlist[i])).flatten() for i in range(imnbr)],'f')

#perform PCA
    V,S,immean = pca(immatrix)

#mean image and first mode of variation
    immean = immean.reshape(m,n)
    mode = V[0].reshape(m,n)

#show the images
    pylab.figure()
    pylab.gray()
    pylab.imshow(immean)

    pylab.figure()
    pylab.gray()
    pylab.imshow(mode)

    pylab.show()



def test_on_coil():


    from PIL import Image
    import numpy
    import pylab

    import os
    p = r'd:\repository\datasets\coil-20-proc'

    imlist = [p+'/'+f for f in os.listdir(p) if 'png' in f]

    o = []
    for f in imlist:
        print f
        im = Image.open(f)
        im2 = numpy.array(im.resize((32,32),Image.BILINEAR))
        o.append(numpy.array(im2.flatten()))
    immatrix = numpy.array(o)

#perform PCA
    V,S,immean = pca(immatrix)

#mean image and first mode of variation
    m,n = 32,32
    immean = immean.reshape(m,n)
    mode = V[0].reshape(m,n)

#show the images
    pylab.figure()
    pylab.gray()
    pylab.imshow(immean)

    pylab.figure()
    pylab.gray()
    pylab.imshow(mode)

    pylab.show()


