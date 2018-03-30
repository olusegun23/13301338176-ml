/*
 *   This file is part of entropy-triangle-weka-package.
 *   
 *   Copyright (C) 2015  Antonio Pastor
 *   
 *   This program is free software: you can redistribute it
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

package weka.etplugin.metrics;

import java.util.ArrayList;
import java.util.List;

import weka.classifiers.ConditionalDensityEstimator;
import weka.classifiers.evaluation.AbstractEvaluationMetric;
import weka.classifiers.evaluation.Evaluation;
import weka.classifiers.evaluation.InformationTheoreticEvaluationMetric;
import weka.core.ContingencyTables;
import weka.core.Instance;
import weka.core.Utils;

/**
 * Information theoretic evaluation metric for classifiers.
 * The class perplexity can be interpreted as the number of effective classes at the input of the classifier due to the likelihood of the input distribution.
 * This is defined as
 * 
 * \[ PP_X = 2^{H_X} \]
 *
 * where \( H_X \) is the entropy of the likelihood of the classes.
 * 
 * \( PP_X = k \), the number of classes,  when \( X \) is uniform. In general \(1 \leq PP_X \leq k \)
 * 
 * <br><br>
 * For more information, see
 * <br><br>
 * <a href="http://dx.doi.org/10.1371/journal.pone.0084217">
 * Valverde-Albacete, F. J., &amp; Pel&aacute;ez-Moreno, C. (2014).
 * 100% classification accuracy considered harmful:
 * The normalized information transfer factor explains the accuracy paradox.
 * PLoS ONE 9(1).</a>
 *
 * <br><br>
 * NOTE: This class needs to have an associated Evaluation object to compute the metric.
 * The Evaluation object can be set via {@link #setBaseEvaluation(Evaluation eval)}.
 * <br>
 * If the package is properly installed in Weka, a reference to the object of this class associated with every
 * Evaluation object can be obtained via the method {@link Evaluation#getPluginMetric(String metricName)}, or the method
 * {@link Evaluation#getPluginMetrics()} to get a list of all the plugin metrics associated with that Evaluation object.
 * 
 * @author Antonio Pastor
 * @see Evaluation
 *
 */
public class ClassPerplexity extends AbstractEvaluationMetric implements InformationTheoreticEvaluationMetric {

	/** For serialization */
	private static final long serialVersionUID = 5222153347263227310L;

	/** Constant string with the metric name */
	public static String METRIC_NAME = "Class perplexity";
	
	/**
	 * Returns a formatted string (suitable for displaying in console or GUI output) containing this metric.
	 * 
	 * @return a formatted string containing the metric
	 */
	@Override
	public String toSummaryString() {
		StringBuffer text = new StringBuffer();
		text.append("Class Perplexity                   ");
		text.append(Utils.doubleToString(getStatistic(ClassPerplexity.METRIC_NAME), 12, 4) + "\n");
		return text.toString();
	}

	/** Returns true. */
	@Override
	public boolean appliesToNominalClass() {
		return true;
	}

	/** Returns false. */
	@Override
	public boolean appliesToNumericClass() {
		return false;
	}

	/** Returns true.
	 * The maximum possible value is the number of classes if the classes distribution of the dataset is uniform.
	 * The minimum possible value is 1, for a dataset where all the instances are of the same class.
	 */
	@Override
	public boolean statisticIsMaximisable(java.lang.String statName) {
		return true;
	}
	
	/**
	 * Get the name of this metric.
	 * 
	 * @return the name of this metric
	 */
	@Override
	public String getMetricName() {
		return ClassPerplexity.METRIC_NAME;
	}

	/**
	 * Get a short description of this metric.
	 * 
	 * @return a short description of this metric
	 */
	@Override
	public String getMetricDescription() {
		return "The number of effective classes at the input of the classifier";
	}

	/**
	 * Get a list with the name of the metric.
	 * 
	 * @return the names of the metric
	 */
	@Override
	public List<String> getStatisticNames() {
		List<String> stNames = new ArrayList<String>();
		stNames.add(ClassPerplexity.METRIC_NAME);
		return stNames;
	}

	/**
	 * Get the value of the named statistic, should be "Class perplexity".
	 * 
	 * @param statName the only valid statistic is "Class perplexity"
	 * @return the computed statistic
	 * @throws AbstractEvaluationMetric.UnknownStatisticException 
	 * 			if the statistic name is not "Class perplexity"
	 */
	@Override
	public double getStatistic(String statName) {
		if (!statName.contains(ClassPerplexity.METRIC_NAME)) {
			throw new UnknownStatisticException(statName + "statistic not known in class" + this.getClass().toString());
		}
		// Entropy of X
		double entropy = ContingencyTables.entropyOverRows(m_baseEvaluation.confusionMatrix());
		return Math.pow(2, entropy);
	}

	/**
	 * Not used.
	 * This method is required to conform to the {@link InformationTheoreticEvaluationMetric}
	 * interface, but not implemented.
	 */
	@Override
	public void updateStatsForClassifier(double[] predictedDistribution, Instance instance) throws Exception {
		return;
	}

	/**
	 * Not used.
	 * This method is required to conform to the {@link InformationTheoreticEvaluationMetric}
	 * interface, but not implemented.
	 */
	@Override
	public void updateStatsForPredictor(double predictedValue, Instance instance) throws Exception {
		return;
	}

	/**
	 * Not used.
	 * This method is required to conform to the {@link InformationTheoreticEvaluationMetric}
	 * interface, but not implemented.
	 */
	@Override
	public void updateStatsForConditionalDensityEstimator(ConditionalDensityEstimator classifier, Instance classMissing,
			double classValue) throws Exception {
		return;
	}
	
}
