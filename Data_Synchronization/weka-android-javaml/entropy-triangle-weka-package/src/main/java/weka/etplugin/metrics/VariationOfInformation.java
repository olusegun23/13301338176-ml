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
 * The Variation of Information is the sum the mutually conditioned entropies between the input and the output of the classifier.
 * 
 * \[ VI = H_{X|Y} + H_{Y|X} \]
 * <br>
 * For more information, see<br>
 * <br>
 * <a href="http://dx.doi.org/10.1016/j.patrec.2010.05.017">
 * Valverde-Albacete, F. J., &amp; Pel&aacute;ez-Moreno, C. (2010).
 * Two information-theoretic tools to assess the performance of multi-class classifiers.
 * Pattern Recognition Letters, Volume 31, Issue 12, 1 September 2010, Pages 1665-1671.</a>
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
public class VariationOfInformation extends AbstractEvaluationMetric
		implements InformationTheoreticEvaluationMetric, TechnicalInformationHandler {

	/** For serialization */
	private static final long serialVersionUID = 7987268448342085068L;

	/** Constant string with the metric name */
	public static String METRIC_NAME = "Variation of Information";
	
	/**
	 * Returns a formatted string (suitable for displaying in console or GUI output) containing this metric.
	 * 
	 * @return a formatted string containing the metric
	 */
	@Override
	public String toSummaryString() {
		StringBuffer text = new StringBuffer();
		text.append("Variation of Information           ");
		text.append(Utils.doubleToString(getStatistic(VariationOfInformation.METRIC_NAME), 12, 4) + "\n");
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

	/** Returns false. */
	@Override
	public boolean statisticIsMaximisable(java.lang.String statName) {
		return false;
	}
	
	/**
	 * Get the name of this metric.
	 * 
	 * @return the name of this metric
	 */
	@Override
	public String getMetricName() {
		return VariationOfInformation.METRIC_NAME;
	}

	/**
	 * Get a short description of this metric.
	 * 
	 * @return a short description of this metric
	 */
	@Override
	public String getMetricDescription() {
		return "The sum of the mutually conditioned entropies between the input and the output of the classifier";
	}

	/**
	 * Get a list with the name of the metric.
	 * 
	 * @return the names of the metric
	 */
	@Override
	public List<String> getStatisticNames() {
		List<String> stNames = new ArrayList<String>();
		stNames.add(VariationOfInformation.METRIC_NAME);
		return stNames;
	}

	/**
	 * Get the value of the named statistic, should be "Variation of Information".
	 * 
	 * @param statName
	 *            the name of the statistic, should be "Variation of Information"
	 * @return the computed statistic
	 * @throws AbstractEvaluationMetric.UnknownStatisticException 
	 * 			if the statistic name is not "Variation of Information"
	 */
	@Override
	public double getStatistic(String statName) {
		if (!statName.contains(VariationOfInformation.METRIC_NAME)) {
			throw new UnknownStatisticException(statName + "statistic not known in class" + this.getClass().toString());
		}
		// Entropy of x Given y
		double entropy = ContingencyTables.entropyConditionedOnColumns(m_baseEvaluation.confusionMatrix());
		// Entropy of y Given x
		entropy += ContingencyTables.entropyConditionedOnRows(m_baseEvaluation.confusionMatrix()); 
		return entropy;
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
		result = new TechnicalInformation(Type.ARTICLE);
		result.setValue(Field.AUTHOR, "Meila, Marina");
		result.setValue(Field.TITLE, "Comparing clusterings—an information based distance");
		result.setValue(Field.JOURNAL, "Journal of multivariate analysis");
		result.setValue(Field.PUBLISHER, "Elsevier");
		result.setValue(Field.VOLUME, "98");
		result.setValue(Field.NUMBER, "5");
		result.setValue(Field.YEAR, "2007");
		result.setValue(Field.PAGES, "873--895");
		return result;
	}
	
}
