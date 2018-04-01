# -*- coding: utf-8 -*-

import unittest
from unittest import TestCase

from sklearn.tree import DecisionTreeClassifier

from tests.estimator.classifier.Classifier import Classifier
from tests.estimator.classifier.ExportedData import ExportedData
from tests.estimator.classifier.EmbeddedData import EmbeddedData
from tests.language.Java import Java


class DecisionTreeClassifierJavaTest(Java, Classifier, ExportedData,
                                     EmbeddedData, TestCase):

    def setUp(self):
        super(DecisionTreeClassifierJavaTest, self).setUp()
        self.estimator = DecisionTreeClassifier(random_state=0)

    def tearDown(self):
        super(DecisionTreeClassifierJavaTest, self).tearDown()

    @unittest.skip('The generated code would be too large.')
    def test_existing_features__digits_data__default(self):
        pass

    @unittest.skip('The generated code would be too large.')
    def test_random_features__digits_data__default(self):
        pass
