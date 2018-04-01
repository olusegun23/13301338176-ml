# -*- coding: utf-8 -*-

from unittest import TestCase

from sklearn.neighbors import KNeighborsClassifier

from tests.estimator.classifier.Classifier import Classifier
from tests.estimator.classifier.ExportedData import ExportedData
from tests.language.JavaScript import JavaScript


class KNeighborsClassifierJSTest(JavaScript, Classifier, ExportedData,
                                 TestCase):

    def setUp(self):
        super(KNeighborsClassifierJSTest, self).setUp()
        self.estimator = KNeighborsClassifier(n_neighbors=3)

    def tearDown(self):
        super(KNeighborsClassifierJSTest, self).tearDown()
