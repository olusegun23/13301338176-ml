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

import weka.classifiers.evaluation.AbstractEvaluationMetric;
import weka.classifiers.evaluation.Evaluation;
import weka.classifiers.evaluation.StandardEvaluationMetric;
import weka.core.ContingencyTables;
import weka.core.Instance;
import weka.core.TechnicalInformation;
import weka.core.TechnicalInformation.Field;
import weka.core.TechnicalInformation.Type;
import weka.core.TechnicalInformationHandler;
import weka.core.Utils;

/**
 * Evaluation metric for classifiers.
 * The Entropy Modulated Accuracy is a pessimistic estimate of the accuracy
 * with the influence of the input distribution factored out.
 * It is defined mathematically as the inverse of the {@link RemainingPerplexity} after the learning process
 *
 * \[ EMA = \frac{1}{PP_{X|Y}} = \frac{1}{2^{H_{X|Y}}} \]
 * 
 * Its bounds are \( \frac{1}{PP_X} \leq EMA \leq 1 \), where \( PP_X \) is the {@link ClassPerplexity}.
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
 * @see RemainingPerplexity
 * @see weka.classifiers.Evaluation
 */
public class Ema extends AbstractEvaluationMetric implements StandardEvaluationMetric, TechnicalInformationHandler {

	/** For serialization */
	private static final long serialVersionUID = 7022909815264061552L;

	/** Constant string with the metric name */
	public static String METRIC_NAME = "EMA";

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

	/**
	 * Get a short description of this metric.
	 *
	 * @return a short description of this metric
	 */
	@Override
	public String getMetricDescription() {
		return "Entropy Modulated Accuracy";
	}

	/**
	 * Get the name of this metric.
	 *
	 * @return the name of this metric
	 */
	@Override
	public String getMetricName() {
		return Ema.METRIC_NAME;
	}

	/**
	 * Get the value of the named statistic, should be "EMA".
	 *
	 * @param statName
	 *            the name of the statistic, should be "EMA"
	 * @return the computed statistic
	 * @throws AbstractEvaluationMetric.UnknownStatisticException
	 * 			if the statistic name is not "EMA"
	 */
	@Override
	public double getStatistic(String statName) {
		if (!statName.contains(Ema.METRIC_NAME)) {
			throw new UnknownStatisticException(statName + "statistic not known in class" + this.getClass().toString());
		}
		double H_xGivenY = ContingencyTables.entropyConditionedOnColumns(m_baseEvaluation.confusionMatrix());
		return Math.pow(2, -H_xGivenY);
	}

	/**
	 * Get a list with the name of the metric.
	 *
	 * @return the names of the metric
	 */
	@Override
	public List<String> getStatisticNames() {
		List<String> stNames = new ArrayList<String>();
		stNames.add(Ema.METRIC_NAME);
		return stNames;
	}

	/**
	 * Returns a formatted string (suitable for displaying in console or GUI output) containing this metric.
	 *
	 * @return a formatted string containing the metric
	 */
	@Override
	public String toSummaryString() {
		StringBuffer text = new StringBuffer();
		text.append("Entropy Modulated Accuracy         ");
		text.append(Utils.doubleToString(getStatistic(Ema.METRIC_NAME), 12, 4) + "\n");
		return text.toString();
	}

	/** Returns true. */
	@Override
	public boolean statisticIsMaximisable(java.lang.String statName) {
		return true;
	}

	/**
	 * Not used.
	 * This method is required to conform to the {@link StandardEvaluationMetric}
	 * interface, but not implemented.
	 */
	@Override
	public void updateStatsForClassifier(double[] predictedDistribution, Instance instance) throws Exception {
		return;
	}

	/**
	 * Not used.
	 * This method is required to conform to the {@link StandardEvaluationMetric}
	 * interface, but not implemented.
	 */
	@Override
	public void updateStatsForPredictor(double predictedValue, Instance instance) throws Exception {
		return;
	}

	/**
	 * Returns an instance of a TechnicalInformation object, containing detailed
	 * information about the technical background of this class, e.g., paper
	 * reference or book this class is based on.
	 *
	 * @return the technical information about this class
	 */
	@Override
	public TechnicalInformation getTechnicalInformation() {
		TechnicalInformation result;
		result = new TechnicalInformation(Type.ARTICLE);
		result.setValue(Field.AUTHOR, "Francisco J. Valverde-Albacete and Carmen Peláez-Moreno");
		result.setValue(Field.TITLE, "100% Classification Accuracy Considered Harmful: "
				+ "The Normalized Information Transfer Factor Explains the Accuracy Paradox");
		result.setValue(Field.JOURNAL, "PLoS ONE");
		result.setValue(Field.PUBLISHER, "Public Library of Science");
		result.setValue(Field.YEAR, "2014");
		result.setValue(Field.MONTH, "01");
		result.setValue(Field.VOLUME, "9");
		result.setValue(Field.URL, "http://dx.doi.org/10.1371%2Fjournal.pone.0084217");
		result.setValue(Field.PAGES, "e84217");
		result.setValue(Field.NUMBER, "1");
		result.setValue(Field.ABSTRACT,
				"The most widely spread measure of performance, accuracy, suffers from a paradox:"
				+ " predictive models with a given level of accuracy may have greater predictive power than models"
				+ " with higher accuracy. Despite optimizing classification error rate, high accuracy models may fail"
				+ " to capture crucial information transfer in the classification task. We present evidence of this behavior"
				+ " by means of a combinatorial analysis where every possible contingency matrix of 2, 3 and 4 classes"
				+ " classifiers are depicted on the entropy triangle, a more reliable information-theoretic tool for"
				+ " classification assessment.</p><p>Motivated by this, we develop from first principles a measure of"
				+ " classification performance that takes into consideration the information learned by classifiers."
				+ " We are then able to obtain the entropy-modulated accuracy (EMA), a pessimistic estimate of the expected"
				+ " accuracy with the influence of the input distribution factored out, and the normalized information transfer"
				+ " factor (NIT), a measure of how efficient is the transmission of information from the input to the output set"
				+ " of classes.</p><p>The EMA is a more natural measure of classification performance than accuracy when the"
				+ " heuristic to maximize is the transfer of information through the classifier instead of classification error"
				+ " count. The NIT factor measures the effectiveness of the learning process in classifiers and also makes it"
				+ " harder for them to “cheat” using techniques like specialization, while also promoting the interpretability"
				+ " of results. Their use is demonstrated in a mind reading task competition that aims at decoding the identity"
				+ " of a video stimulus based on magnetoencephalography recordings. We show how the EMA and the NIT factor"
				+ " reject rankings based in accuracy, choosing more meaningful and interpretable classifiers.");
		return result;
	}

}
