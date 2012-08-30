#!/usr/bin/env python
#coding=utf-8

import numpy
import math

#http://python.ca/nas/python/bpnn.py

def sigmoid(x):
    return math.tanh(x)

def sigmoid(t):
    try:
        return 1.0 / ( 1+ math.exp(-t))
    except:
        print t

sigmoid = numpy.vectorize(sigmoid)

def train(X,Y):
    if Y.ndim == 1:
        Y = Y.reshape(Y.shape[0],1)
    assert(Y.shape[1]==1)
    assert(X.ndim==2)
    N, d = X.shape
    assert(Y.shape[0]==N)
    assert(d>1)
    mean_vector = X.mean(axis=0)
    X = X - mean_vector
    scale = numpy.linalg.norm(X,'fro')/math.sqrt(X.shape[0])
    X = X/scale

    X = numpy.c_[X,numpy.ones((N,1))]
    d += 1
    w = numpy.random.rand(d,1) # random initialization
    _lambda = 0.0001
    tolerance = 1E-10
    # IRLS Newton Raphson Update
    iter = 0
    while True:
        f = numpy.dot(X,w)
        sigm = sigmoid(f)
        a = sigm * (1 - sigm)
        z = f + ((1-sigmoid(Y*f))*Y)/a
        P = numpy.linalg.inv( numpy.dot(X.T,a*X) + _lambda*numpy.eye(d))
        w_prev = w
        w = numpy.dot(numpy.dot(P,X.T),a*z)
        iter += 1
        dif = numpy.linalg.norm(w_prev - w)
        print iter, dif
        if iter >=10000 or  dif < tolerance:
            break
    return w, X
