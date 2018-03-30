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
import weka.core.TechnicalInformation;
import weka.core.TechnicalInformation.Field;
import weka.core.TechnicalInformation.Type;
import weka.core.TechnicalInformationHandler;
import weka.core.Utils;

/**
 * Information theoretic evaluation metric for classifiers.
 * The Information Transfer factor measures the decrement in difficulty of the task as the perplexity of the
mutual information between the input and the output distributions of the classifier.
 * \[ \mu_{XY} = 2^{MI_{XY}} \]
 * 
 * where \[ MI_{XY} = H_X - H_{X|Y} =- \sum_{i,j}{p(x_i,y_j) log_2\frac{p(x_i,y_j)}{p(x_i)p(y_j)}} \]
 * 
 * <br>
 * For more information, see
 * <br><br>
 * <a href="http://dx.doi.org/10.1371/journal.pone.0084217">
 * Valverde-Albacete, F. J., &amp; Pel&aacute;ez-Moreno, C. (2014).
 * 100% classification accuracy considered harmful:
 * The normalized information transfer factor explains the accuracy paradox.
 * PLoS ONE 9(1).</a>
 * <br>
 * Fano MR (1961). Transmission of Information: A Statistical Theory of Communication. The MIT Press.
 * <br>
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
 * @see weka.classifiers.Evaluation
 */
public class InformationTransferFactor extends AbstractEvaluationMetric
		implements InformationTheoreticEvaluationMetric, TechnicalInformationHandler {

	/** For serialization */
	private static final long serialVersionUID = -8754649150868275087L;

	/** Constant string with the metric name */
	public static String METRIC_NAME = "Information Transfer factor";
	
	/**
	 * Returns a formatted string (suitable for displaying in console or GUI output) containing this metric.
	 * 
	 * @return a formatted string containing the metric
	 */
	@Override
	public String toSummaryString() {
		StringBuffer text = new StringBuffer();
		text.append("Information Transfer factor        ");
		text.append(Utils.doubleToString(getStatistic(InformationTransferFactor.METRIC_NAME), 12, 4) + "\n");
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

	/** Returns true. */
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
		return InformationTransferFactor.METRIC_NAME;
	}

	/**
	 * Get a short description of this metric.
	 * 
	 * @return a short description of this metric
	 */
	@Override
	public String getMetricDescription() {
		return "The perplexity of the mutual information (2^MI)";
	}

	/**
	 * Get a list with the name of the metric.
	 * 
	 * @return the names of the metric
	 */
	@Override
	public List<String> getStatisticNames() {
		List<String> stNames = new ArrayList<String>();
		stNames.add(InformationTransferFactor.METRIC_NAME);
		return stNames;
	}

	/**
	 * Get the value of the named statistic, should be "Information Transfer factor".
	 * 
	 * @param statName
	 *            the name of the statistic, should be "Information Transfer factor"
	 * @return the computed statistic
	 * @throws AbstractEvaluationMetric.UnknownStatisticException 
	 * 			if the statistic name is not "Information Transfer factor"
	 */
	@Override
	public double getStatistic(String statName) {
		if (!statName.contains(InformationTransferFactor.METRIC_NAME)) {
			throw new UnknownStatisticException(statName + "statistic not known in class" + this.getClass().toString());
		}
		double[][] cf = m_baseEvaluation.confusionMatrix();
		double H_xGivenY = ContingencyTables.entropyConditionedOnColumns(cf);
		double H_Px = ContingencyTables.entropyOverRows(cf);
		// MI(x,y) = H_Px - H_xGivenY
		double informationTransferFactor = Math.pow(2, H_Px - H_xGivenY);
		return informationTransferFactor;
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
		result = new TechnicalInformation(Type.BOOK);
		result.setValue(Field.AUTHOR, "Robert M. Fano");
		result.setValue(Field.TITLE, "Transmission of Information: A Statistical Theory of Communication");
		result.setValue(Field.PUBLISHER, "The MIT Press");
		result.setValue(Field.YEAR, "1961");
		return result;
	}
	
}
