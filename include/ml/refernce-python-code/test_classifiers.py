#!/usr/bin/env python
#coding=utf-8

import unittest
from numpy import *
from adaboost import AdaBoost
from decision_stump import DecisionStump

import pyroc

def load_bupa_dataset():
    """
    The BUPA dataset can be obtained from
    http://www.cs.huji.ac.il/~shais/datasets/ClassificationDatasets.html
    See description of this dataset at
    http://www.cs.huji.ac.il/~shais/datasets/bupa/bupa.names
    """
    data = loadtxt('bupa.data',delimiter = ',')
    X = data[:,:-1] # features
    X = X
    Y = data[:,-1]
    Y[Y==2] = -1    # labels <- {1, -1}
    return X, Y



class AdaBoostTestCase(unittest.TestCase):
    def setUp(self):
        pass

    def tearDown(self):
        pass

    def testBupaData(self):
        X, Y = load_bupa_dataset()
        classifier = AdaBoost(DecisionStump)
        for t in [100,200,300,400,500]:
            score = classifier.test_on_training_set(X,Y,t)
            roc = pyroc.ROCData(zip(Y,score))
            auc = roc.auc()
            print auc
            self.failUnless(auc > .9)

def main():
    suite = unittest.TestLoader().loadTestsFromTestCase(AdaBoostTestCase)
    unittest.TextTestRunner(verbosity=2).run(suite)

if __name__ == '__main__':
    main()





