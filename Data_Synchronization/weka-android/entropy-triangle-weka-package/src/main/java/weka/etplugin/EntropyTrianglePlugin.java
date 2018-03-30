/*
 *   This file is part of entropy-triangle-weka-package.
 *   
 *   Copyright (C) 2015-2017  Antonio Pastor
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

import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;

import javax.swing.JFrame;
import javax.swing.JList;
import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JPopupMenu;
import javax.swing.JScrollPane;
import javax.swing.JViewport;
import javax.swing.SwingUtilities;

import weka.classifiers.AbstractClassifier;
import weka.classifiers.evaluation.Evaluation;
import weka.core.Instance;
import weka.core.Instances;
import weka.core.TechnicalInformation;
import weka.core.TechnicalInformation.Field;
import weka.core.TechnicalInformation.Type;
import weka.core.TechnicalInformationHandler;
import weka.gui.MainMenuExtension;
import weka.gui.ResultHistoryPanel;
import weka.gui.visualize.plugins.ErrorVisualizePlugin;

/**
 * This class acts as a launcher for the Entropy Triangle Plugin from the visualization plugin context menu of the Weka explorer 
 * and from the Extensions menu in the Weka GUI Chooser window.
 * <br><br>
 * The same window, and {@code EntropyTrianglePanel}, is shared for both invoking sources. This way can be added instances from the explorer 
 * to a plot of a instances loaded from a file.
 * <br><br>
 * The panel data is cleared when the user closes the window.
 * <br><br>
 * For more information about the Entropy Triangle, see<br>
 * <br>
 * <a href="http://dx.doi.org/10.1016/j.patrec.2010.05.017">
 * Valverde-Albacete, F. J., &amp; Pel&aacute;ez-Moreno, C. (2010).
 * Two information-theoretic tools to assess the performance of multi-class classifiers.
 * Pattern Recognition Letters, Volume 31, Issue 12, 1 September 2010, Pages 1665-1671.</a>
 * 
 * @author Antonio Pastor
 * @see EntropyTrianglePanel
 *
 */
public class EntropyTrianglePlugin implements ErrorVisualizePlugin, MainMenuExtension, TechnicalInformationHandler {

	private static JFrame frame;
	private static EntropyTrianglePanel et;

	/**
	 * Get a JMenu or JMenuItem which contain action listeners that perform the
	 * visualization of the Entropy Triangle.
	 * <br>
	 * The actual class is the attribute declared as class attribute, the
	 * predicted class values is found in the attribute prior to the class
	 * attribute's position. In other words, if the <code>classIndex()</code>
	 * method returns 10, then the attribute position for the predicted class
	 * values is 9.
	 * <br>
	 * Exceptions thrown because of changes in Weka since compilation need to be
	 * caught by the implementer.
	 * 
	 * @see NoClassDefFoundError
	 * @see IncompatibleClassChangeError
	 * 
	 * @param predInst the instances with the actual and predicted class values
	 * @return the menu item for opening the visualization or add the data if the Entropy Triangle already exists.
	 */
	@Override
	public JMenuItem getVisualizeMenuItem(Instances predInst) {
		if (predInst.numClasses() == 1) {
			return null;
		}
		final Instances instances = predInst;
		JMenuItem result = new JMenuItem("Entropy Triangle");
		result.addActionListener(new ActionListener() {
			@SuppressWarnings("rawtypes")
			@Override
			public void actionPerformed(ActionEvent e) {

				/*
				 *  Eibe Frank (Waikato University) credit for the reversed path chain
				 *  to the result history list, panel and the classifier.
				 */
				JList jl = (JList)((JPopupMenu)((JMenu)((JPopupMenu)((JMenuItem)e.getSource())
						.getParent()).getInvoker()).getParent()).getInvoker();
				ResultHistoryPanel rhp = (ResultHistoryPanel)(((JScrollPane)(((JViewport)jl.getParent()).getParent())).getParent());
				final AbstractClassifier classifier = (AbstractClassifier)((ArrayList)rhp.getSelectedObject()).get(0);
				final String str_dataset = instances.relationName().replaceAll("_predicted", "");
				final String str_time = jl.getSelectedValue().toString().substring(0, 8);
				
				final Evaluation ev;
				try {
					ev = new Evaluation (instances);
					ev.setDiscardPredictions(true);
					int classIndex = instances.classIndex();
					
					for (Instance i: instances) {
						ev.evaluateModelOnce(i.value(classIndex-1), i);
					}
				} catch (Exception e1) {
					System.out.println("Error evaluating predictions, can't generate plot");
					e1.printStackTrace();
					return;
				}
				SwingUtilities.invokeLater(new Runnable() {
					@Override
					public void run() {
						updateWindow();
						try {
							et.addData(ev, classifier, str_dataset, str_time);
						} catch (Exception ex) {
							System.out.println("Error: instances don't have class atribute");
							ex.printStackTrace();
						}
					}
				});
				
			}
		});
		return result;
	}
	
	/**
	 * Get the minimum version of Weka, inclusive, the class is designed to work
	 * with. eg: <code>3.5.0</code>
	 * 
	 * @return the minimum version
	 */
	@Override
	public String getMinVersion() {
		return "3.7.8";
	}

	/**
	 * Get the maximum version of Weka, exclusive, the class is designed to work
	 * with. eg: <code>3.6.0</code>
	 * 
	 * @return the maximum version
	 */
	@Override
	public String getMaxVersion() {
		return "3.9.2";
	}

	/**
	 * Get the specific version of Weka the class is designed for. eg:
	 * <code>3.5.1</code>
	 * 
	 * @return the version the plugin was designed for
	 */
	@Override
	public String getDesignVersion() {
		return "3.7.12";
	}

	@Override
	public String getSubmenuTitle() {
		return null;
	}

	@Override
	public String getMenuTitle() {
		return "Entropy Triangle";
	}

	/**
	 * Gets an <code>ActionListener</code> to launch or update the Entropy Triangle window.
	 * 
	 * @param owner Not used, needed to conform the interface <code>MainMenuExtension</code>
	 * @return the <code>ActionListener</code> 
	 */
	@Override
	public ActionListener getActionListener(JFrame owner) {
		return new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				updateWindow();
			}
		};
	}

	/**
	 * Not used.
	 * This method is required to conform to the {@link MainMenuExtension}
	 * interface, but not implemented.
	 */
	@Override
	public void fillFrame(Component frame) {
		return;
	}

	@SuppressWarnings("static-method")
	private void updateWindow(){
		if (frame == null) {
			frame = new JFrame("Entropy Triangle");
			et = new EntropyTrianglePanel();
			frame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
			frame.add(et);
			frame.setVisible(true);
			frame.pack();
		}
		else if(!frame.isVisible()) {
			et.clearData();
			frame.setVisible(true);
		}
		else {
			et.requestFocusInWindow();
		}
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
