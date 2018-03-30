/*
 *   This file is part of entropy-triangle-weka-package.
 *   
 *   entropy-triangle-weka-package is free software: you can redistribute it
 *   and/or modify it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation, either version 3 of the License,
 *   or (at your option) any later version.
 *   
 *   entropy-triangle-weka-package is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *   
 *   You should have received a copy of the GNU General Public License
 *   along with entropy-triangle-weka-package.
 *   If not, see <http://www.gnu.org/licenses/>.
 */

/*
 *    TestTriangle.java
 *    Copyright (C) 2014-2015 Antonio A. Pastor Valles
 */

package weka.etplugin;

import javax.swing.JFrame;

import weka.classifiers.evaluation.Evaluation;
import weka.core.Instances;
import weka.core.converters.ConverterUtils.DataSource;

public class TestTriangle {

	public static void main(String[] args) {

		JFrame frame = new JFrame();
		EntropyTrianglePanel et = new EntropyTrianglePanel();

		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.add(et);
		frame.setVisible(true);
		frame.pack();

		try {
			DataSource source = new DataSource("/home/apastor/wekafiles/data/segment-challenge.arff");
			Instances train = source.getDataSet();
			Instances test = DataSource.read("/home/apastor/wekafiles/data/segment-test.arff");
			train.setClassIndex(train.numAttributes() - 1);
			test.setClassIndex(test.numAttributes() - 1);

			Evaluation eval = new Evaluation(train);
			weka.classifiers.rules.ZeroR zr = new weka.classifiers.rules.ZeroR();
			zr.buildClassifier(train);
			eval.evaluateModel(zr, test);
			et.addData(eval, zr, test.relationName(), null);

			eval = new Evaluation(train);
			weka.classifiers.rules.OneR oner = new weka.classifiers.rules.OneR();
			oner.buildClassifier(train);
			eval.evaluateModel(oner, test);
			et.addData(eval, oner, test.relationName(), null);

			eval = new Evaluation(train);
			weka.classifiers.bayes.NaiveBayes nb = new weka.classifiers.bayes.NaiveBayes();
			nb.buildClassifier(train);
			eval.evaluateModel(nb, test);
			et.addData(eval, nb, test.relationName(), null);

			weka.classifiers.trees.J48 j48 = new weka.classifiers.trees.J48();
			j48.buildClassifier(train);
			eval = new Evaluation(train);
			eval.evaluateModel(j48, test);
			et.addData(eval, j48, test.relationName(), null);

		} catch (Exception e) {
			System.out.println("Error on main");
			e.printStackTrace();
		}
	}
}
