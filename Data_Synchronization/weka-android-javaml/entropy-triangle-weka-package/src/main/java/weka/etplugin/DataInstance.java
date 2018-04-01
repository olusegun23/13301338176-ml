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

package weka.etplugin;

import java.awt.Color;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.geom.Point2D;

import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;

import weka.core.DenseInstance;
import weka.core.Instance;
import weka.core.Instances;
import weka.core.TechnicalInformation;
import weka.core.TechnicalInformation.Field;
import weka.core.TechnicalInformation.Type;
import weka.core.TechnicalInformationHandler;
import weka.core.Utils;
import weka.etplugin.metrics.InformationTransferFactor;
import weka.etplugin.metrics.Nit;
import weka.etplugin.metrics.RemainingPerplexity;
import weka.etplugin.metrics.VariationOfInformation;

/**
 * Wrapper class that stores the evaluation data of each classification performed
 * alongside the graphical components to represent that evaluation in the Entropy Triangle.
 * 
 * It handles the tooltip windows and the color of the <code>DataPoints</code> if the <code>ActionListener</code> is added,
 * i.e., to the <code>ColorBar</code>.
 * 
 * @author Antonio Pastor
 * @see DataPoint
 * @see DataInstances
 * @see ColorBar
 *
 */
public class DataInstance extends DenseInstance implements TechnicalInformationHandler {

	private static final long serialVersionUID = 7090787324664183875L;

	/** A string that resumes the classifier and dataset of the instance */
	public String label;
	
	private double normMI;
	
	private double normInc_H_Pxy;
	private double normVI;
	
	/** The normalized increment of entropy of the input,
	 * sets the upper bound of normalized MI.
	 * Used to plot the {@link Baseline}
	 */
	protected double normInc_H_Px;
	private double norm_H_xGivenY;

	private double normInc_H_Py;
	private double norm_H_yGivenX;
	
	private double joint_x, joint_y;
	private double input_x, output_x;
	
	/** Graphical object to represent this instance */
	public DataPoint joint, input, output;

	/**
	 * Constructor that initializes instance variable with given values for the given dataset.
	 * 
	 * @param dataset the Instances dataset that this instance belong
	 * @param weight the weight of this instance in the dataset
	 * @param attValues an array with the attributes values of this instance in double format
	 */
	public DataInstance(Instances dataset, double weight, double[] attValues) {
		super(weight, attValues);
		this.setDataset(dataset);
		setDataPoints();
	}
	
	/**
	 * Constructor that initializes instance variable with given values.
	 * 
	 * @param dataset the Instances dataset that this instance belong
	 * @param instance the instance from which the attribute values and the weight are to be copied
	 */
	public DataInstance(Instances dataset, Instance instance) {
		super(instance);
		this.setDataset(dataset);
		setDataPoints();
	}
	
	// Throws Exception 
	private void setDataPoints() {
		
		if(this.m_Dataset.attribute("Dataset")!=null) {
			label = String.format("%s - %s (%s dataset)", this.stringValue(this.m_Dataset.attribute("time")),
													this.stringValue(this.m_Dataset.attribute("Classifier")),
													this.stringValue(this.m_Dataset.attribute("Dataset")));
		
		} else if (this.m_Dataset.attribute("Date_time")!=null &&
				this.m_Dataset.attribute("Key_Scheme")!=null) {
			String tempDate = String.valueOf(this.value(this.m_Dataset.attribute("Date_time")));
			String tempScheme = this.stringValue(this.m_Dataset.attribute("Key_Scheme")).replaceFirst("weka.classifiers.", "");
			label = String.format("%s %s:%s - %s (%s dataset)", tempDate.substring(0, 8),
													tempDate.substring(9, 11),
													tempDate.substring(11, 13), tempScheme,
													this.stringValue(this.m_Dataset.attribute("Key_Dataset")));
			
		} else {
			label = "--NO LABEL --";
			// throw new UnsupportedAttributeTypeException();
		}
		
		double mu_xy = this.value(this.m_Dataset.attribute(InformationTransferFactor.METRIC_NAME));
		double nit = this.value(this.m_Dataset.attribute(Nit.METRIC_NAME));
		double vi = this.value(this.m_Dataset.attribute(VariationOfInformation.METRIC_NAME));
		
		double H_u = Utils.log2(Math.round(mu_xy / nit)); //  mu_xy / NIT
		double H_xGivenY = Utils.log2(this.value(this.m_Dataset.attribute(RemainingPerplexity.METRIC_NAME))); // Remaining perplexity = k_x|y
		double H_yGivenX = vi - H_xGivenY; // H_y|x = VI - H_x|y
		double MI = Utils.log2(mu_xy); // mu_xy = 2^MI_xy
		
		normMI = MI / H_u;
		
		normVI = vi / (2*H_u);
		normInc_H_Pxy = 1 - (normMI + normVI);
		joint_x = normInc_H_Pxy + TernaryPlot.COS60 * normMI;
		joint_y = TernaryPlot.SIN60 * normMI;
		
		norm_H_xGivenY = H_xGivenY / H_u;
		normInc_H_Px = 1 - (normMI + norm_H_xGivenY);
		input_x = normInc_H_Px + TernaryPlot.COS60 * normMI;
		
		norm_H_yGivenX = H_yGivenX / H_u; // H_y|x = VI - H_x|y
		normInc_H_Py = 1 - (normMI + norm_H_yGivenX);
		output_x = normInc_H_Py + TernaryPlot.COS60 * normMI;
		
		joint = new DataPoint(new Point2D.Double(joint_x,
				joint_y));
		input = new DataPoint(new Point2D.Double(input_x,
				joint_y));
		output = new DataPoint(new Point2D.Double(output_x,
				joint_y));
		
		setToolTips();
	}

	/**
	 * Sets and updates the tooltip windows of the <code>DataPoints</code> of this object.
	 * This method should be called when any of the information showed in the tooltip may have changed.
	 */
	public void setToolTips() {
		
		StringBuffer text = new StringBuffer();
		StringBuffer labelBuffer = new StringBuffer();
		StringBuffer labelClass = new StringBuffer();
		
		text.append("<html>");
		
		// If label is too long break it in multiple lines
		if (label!=null) {
			int labelLen = label.length();
			for (int i=0; i<labelLen; i+=70) {
				labelBuffer.insert(labelBuffer.length(), label, i, Math.min(i+70, labelLen));
				labelBuffer.append("<br/>");
			}
		}
		
		if (this.classIndex()>=0) {
			labelClass.append(this.classAttribute().name() + ": ");
			if (Utils.isMissingValue(this.classValue())) {
				labelClass.append("missing value");
			} else {
				if (this.classAttribute().isNumeric()) {
					labelClass.append(String.format("%.4f", this.classValue()));
				} else {
					labelClass.append(this.stringValue(this.classIndex()));
				}
			}
		}
		
		text.append(labelBuffer);
		
		text.append("<table>");
		text.append("<tr><td>norm. Inc. H(Pxy): </td>");
		text.append(String.format("<td>%.4f</td></tr>", normInc_H_Pxy));
		
		text.append("<tr><td>normalized MI: </td>");
		text.append(String.format("<td>%.4f</td></tr>", normMI));
		
		text.append("<tr><td>normalized VI: </td>");
		text.append(String.format("<td>%.4f</td></tr>", normVI));
		text.append("</table>");
		
		text.append(labelClass);
		
		text.append("</html>");
		joint.setToolTipText(text.toString());
		
		
		// text for Input point
		text = new StringBuffer();
		text.append("<html>");
		
		text.append(labelBuffer);
		
		text.append("<table>");
		text.append("<tr><td>norm. Inc. H(Px): </td>");
		text.append(String.format("<td>%.4f</td></tr>", normInc_H_Px));
		
		text.append("<tr><td>normalized MI: </td>");
		text.append(String.format("<td>%.4f</td></tr>", normMI));
		
		text.append("<tr><td>normalized H(x|y): </td>");
		text.append(String.format("<td>%.4f</td></tr>", norm_H_xGivenY));
		text.append("</table>");

		text.append(labelClass);
		
		text.append("</html>");
		input.setToolTipText(text.toString());
		
		// text for Output point
		text = new StringBuffer();
		text.append("<html>");
		
		text.append(labelBuffer);
		
		text.append("<table>");
		text.append("<tr><td>norm. Inc. H(Py): </td>");
		text.append(String.format("<td>%.4f</td></tr>", normInc_H_Py));

		text.append("<tr><td>normalized MI: </td>");
		text.append(String.format("<td>%.4f</td></tr>", normMI));

		text.append("<tr><td>normalized H(y|x): </td>");
		text.append(String.format("<td>%.4f</td></tr>", norm_H_yGivenX));
		text.append("</table>");

		text.append(labelClass);
		
		text.append("</html>");
		output.setToolTipText(text.toString());
	}
	
	/**
	 * Sets an <code>ActionListener</code> to sync the foreground color of the points of this instance
	 * in a graph with the given <code>ColorBar</code>.
	 * @param colorBar the object from where to get the new colors
	 * @return ActionListener the ActionListener that can be used by the <code>ColorBar</code> to
	 * change the color of the {@link DataPoint DataPoints} of this DataInstance.
	 * @see ColorBar
	 */
	public ActionListener setColorChangeListener(final ColorBar colorBar) {
		return new ActionListener () {
			@Override
			public void actionPerformed(ActionEvent e) {
				double value = classValue();
				Color color = colorBar.getColor(value);
				joint.setForeground(color);
				input.setForeground(color);
				output.setForeground(color);
			}
		};
	}


	/**
	 * Sets the right-click menu behavior for the points of this instance.
	 * 
	 * @param etPanel the panel where the points of this instance is being drawn
	 */
	public void setPopupMenus(final EntropyTrianglePanel etPanel){
			JPopupMenu menu = new JPopupMenu();
			JMenuItem delete = new JMenuItem("Delete");
			delete.addActionListener(new ActionListener () {

				@Override
				public void actionPerformed(ActionEvent e) {
					etPanel.delInstance(DataInstance.this);
				}
			});
			menu.add(delete);
			joint.setComponentPopupMenu(menu);
			input.setComponentPopupMenu(menu);
			output.setComponentPopupMenu(menu);
	}
	
	/**
	 * Produces a shallow copy of this instance. The copy has access to the same
	 * dataset.
	 * 
	 * @return the shallow copy
	 */
	// @ also ensures \result != null;
	// @ also ensures \result instanceof DenseInstance;
	// @ also ensures ((DenseInstance)\result).m_Dataset == m_Dataset;
	@Override
	public/* @pure@ */Object copy() {

		DataInstance result = new DataInstance(m_Dataset, this);
		result.m_Dataset = m_Dataset;
		return result;
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
		result.setValue(Field.TITLE, "Two information-theoretic tools to assess the performance of multi-class classifiers");
		result.setValue(Field.JOURNAL, "Pattern Recognition Letters");
		result.setValue(Field.PUBLISHER, "Elsevier");
		result.setValue(Field.VOLUME, "31");
		result.setValue(Field.NUMBER, "12");
		result.setValue(Field.YEAR, "2010");
		result.setValue(Field.ISSN, "0167-8655");
		result.setValue(Field.URL, "http://www.sciencedirect.com/science/article/pii/S0167865510001662");
		result.setValue(Field.PAGES, "1665 - 1671");
		result.setValue(Field.ABSTRACT, "We develop two tools to analyze the behavior of multiple-class, or multi-class,"
				+ " classifiers by means of entropic measures on their confusion matrix or contingency table."
				+ " First we obtain a balance equation on the entropies that captures interesting properties of"
				+ " the classifier. Second, by normalizing this balance equation we first obtain a 2-simplex"
				+ " in a three-dimensional entropy space and then the de Finetti entropy diagram or entropy"
				+ " triangle. We also give examples of the assessment of classifiers with these tools. ");
		result.setValue(Field.KEYWORDS, "Multi-class classifier, Confusion matrix, Contingency table, Contingency table, de Finetti diagram, Entropy triangle");
		return result;
	}
}
