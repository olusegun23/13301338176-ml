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
 * The Remaining Perplexity represents the residual difficulty for the classification task after the learning process
 * 
 * \[ PP_{X|Y} = 2^{H_{X|Y}} \]
 * 
 * Where \( H_{X|Y} \) is the entropy of the input probability distribution given the output,
 * and measures the information that the classifier failed to learn.
 * 
 * The minimum \( PP_{X|Y} \) is one, meaning no remaining difficulty.
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
 * @see Ema
 * @see weka.classifiers.Evaluation
 */
public class RemainingPerplexity extends AbstractEvaluationMetric implements InformationTheoreticEvaluationMetric {

	/** For serialization */
	private static final long serialVersionUID = 506784502007343611L;

	/** Constant string with the metric name */
	public static String METRIC_NAME = "Remaining perplexity";
	
	/**
	 * Returns a formatted string (suitable for displaying in console or GUI output) containing this metric.
	 * 
	 * @return a formatted string containing the metric
	 */
	@Override
	public String toSummaryString() {
		StringBuffer text = new StringBuffer();
		text.append("Remaining Perplexity                ");
		text.append(Utils.doubleToString(getStatistic(RemainingPerplexity.METRIC_NAME), 12, 4) + "\n");
		return text.toString();
	}

	/**
	 * Get the name of this metric.
	 * 
	 * @return the name of this metric
	 */
	@Override
	public String getMetricName() {
		return RemainingPerplexity.METRIC_NAME;
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

	/** Returns false. */
	@Override
	public boolean statisticIsMaximisable(java.lang.String statName) {
		return false;
	}
	
	/**
	 * Get a short description of this metric.
	 * 
	 * @return a short description of this metric
	 */
	@Override
	public String getMetricDescription() {
		return "The mean number of possible classes can belong the predicted classes";
	}

	/**
	 * Get a list with the name of the metric.
	 * 
	 * @return the names of the metric
	 */
	@Override
	public List<String> getStatisticNames() {
		List<String> stNames = new ArrayList<String>();
		stNames.add(RemainingPerplexity.METRIC_NAME);
		return stNames;
	}

	/**
	 * Get the value of the named statistic, should be "Remaining perplexity".
	 * 
	 * @param statName
	 *            the name of the statistic, should be "Remaining perplexity"
	 * @return the computed statistic
	 * @throws AbstractEvaluationMetric.UnknownStatisticException 
	 * 			if the statistic name is not "Remaining perplexity"
	 */
	@Override
	public double getStatistic(String statName) {
		if (!statName.contains(RemainingPerplexity.METRIC_NAME)) {
			throw new UnknownStatisticException(statName + "statistic not known in class" + this.getClass().toString());
		}
		// Entropy of x Given y
		double entropy = ContingencyTables.entropyConditionedOnColumns(m_baseEvaluation.confusionMatrix()); 
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
