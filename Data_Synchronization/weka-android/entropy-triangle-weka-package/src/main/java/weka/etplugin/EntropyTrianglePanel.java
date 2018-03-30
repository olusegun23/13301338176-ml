
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

import java.awt.BorderLayout;
import java.awt.Container;
import java.awt.Desktop;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.IOException;
import java.io.ObjectStreamClass;
import java.net.URISyntaxException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextPane;
import javax.swing.JToggleButton;
import javax.swing.event.HyperlinkEvent;
import javax.swing.event.HyperlinkListener;

import weka.classifiers.AbstractClassifier;
import weka.classifiers.evaluation.AbstractEvaluationMetric;
import weka.classifiers.evaluation.Evaluation;
import weka.classifiers.evaluation.InformationTheoreticEvaluationMetric;
import weka.classifiers.evaluation.StandardEvaluationMetric;
import weka.core.Attribute;
import weka.core.Capabilities;
import weka.core.Instance;
import weka.core.InstanceComparator;
import weka.core.Instances;
import weka.core.TechnicalInformation;
import weka.core.TechnicalInformation.Field;
import weka.core.TechnicalInformation.Type;
import weka.core.TechnicalInformationHandler;
import weka.core.Utils;
import weka.core.converters.AbstractFileLoader;
import weka.core.converters.AbstractFileSaver;
import weka.etplugin.metrics.Ema;
import weka.etplugin.metrics.InformationTransferFactor;
import weka.etplugin.metrics.Nit;
import weka.etplugin.metrics.RemainingPerplexity;
import weka.etplugin.metrics.VariationOfInformation;
import weka.gui.ConverterFileChooser;
import weka.gui.visualize.PrintablePanel;

/**
 * This panel coordinates the different elements of the Entropy Triangle plot.
 * <br><br>
 * Inherits from the {@link PrintablePanel} the possibility to print the panel to various file formats.
 * The Print dialog is accessible via Ctrl+Alt+Shift+"Left Mouse Click".
 * 
 * <br><br>
 * For more information about the Entropy Triangle, see<br>
 * <br>
 * <a href="http://dx.doi.org/10.1016/j.patrec.2010.05.017">
 * Valverde-Albacete, F. J., &amp; Pel&aacute;ez-Moreno, C. (2010).
 * Two information-theoretic tools to assess the performance of multi-class classifiers.
 * Pattern Recognition Letters, Volume 31, Issue 12, 1 September 2010, Pages 1665-1671.</a>
 * 
 * <br><br>
 * To use the Entropy Triangle from Java code it is needed to train and evaluate the classifiers before adding the data to the plot,
 * like in the following example:
 * <pre>
 * <code>
 * 	public static void main(String[] args) {
 *
 *		JFrame frame = new JFrame();
 *		EntropyTrianglePanel et = new EntropyTrianglePanel();
 *
 *		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
 *		frame.add(et);
 *		frame.setVisible(true);
 *		frame.pack();
 *
 *		try {
 *			DataSource source = new DataSource("./datasets/segment-challenge.arff");
 *			Instances train = source.getDataSet();
 *			Instances test = DataSource.read("./datasets/segment-test.arff");
 *			train.setClassIndex(train.numAttributes() - 1);
 *			test.setClassIndex(test.numAttributes() - 1);
 *
 *			weka.classifiers.rules.ZeroR zr = new weka.classifiers.rules.ZeroR();
 *			zr.buildClassifier(train);
 *			Evaluation eval = new Evaluation(train);
 *			eval.evaluateModel(zr, test);
 *			et.addData(eval, zr, test.relationName(), null);
 *
 *		} catch (Exception e) {
 *			System.out.println("Error on main");
 *			e.printStackTrace();
 *		}
 *	}
 *</code>
 * </pre>
 * @author Antonio Pastor
 *
 */
public class EntropyTrianglePanel extends PrintablePanel implements TechnicalInformationHandler {

	private static final long serialVersionUID = -7108681062395894449L;
	
	private static int PANELWIDTH = 730;
	
	private TernaryPlot plot;
	private ColorBar colorBar;
	
	private JPanel buttonPanel, plotPanel, colorBarPanel;
	private JComboBox<String> currentMetric;
	private JToggleButton toggleSplit;
	private JToggleButton toggleBaseline; // Baseline is H(Px) line
	private JButton saveButton;
	private JButton loadButton;
	private JButton helpButton;
	private ActionListener ioListener;
	
	private DataInstances dataList = null;
	private List<Baseline> bsline;
	
	/** Default constructor. */
	public EntropyTrianglePanel() {
		toggleSplit = new JToggleButton ("Split");
		toggleSplit.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				boolean state = ((JToggleButton)e.getSource()).isSelected();
				setSplit(state);
			}
		});
		toggleBaseline = new JToggleButton ("\u0394" + "H'(x) line");
		toggleBaseline.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				boolean state = ((JToggleButton)e.getSource()).isSelected();
				setBaseline(state);
			}
		});
		saveButton = new JButton ("Save to file");
		saveButton.setEnabled(false);
		saveButton.setActionCommand("save");
		ioListener = new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				try {
					ConverterFileChooser fileChooser = new ConverterFileChooser();
					if(dataList==null) {
						fileChooser.setCoreConvertersOnly(true);
					} else {
						fileChooser.setCapabilitiesFilter(Capabilities.forInstances(dataList, true));
					}
					if(e.getActionCommand()=="load") {
						int returnVal = fileChooser.showOpenDialog(EntropyTrianglePanel.this);
						if (returnVal == JFileChooser.APPROVE_OPTION) {
							AbstractFileLoader loader = fileChooser.getLoader();
							addInstancesFromFile(loader.getDataSet());
						}
					} else {
						int returnVal = fileChooser.showSaveDialog(EntropyTrianglePanel.this);
						if (returnVal == JFileChooser.APPROVE_OPTION) {
							AbstractFileSaver saver = fileChooser.getSaver();
							saver.setInstances(dataList);
							saver.writeBatch();
						}						
					}
				} catch (IOException e1) {
					e1.printStackTrace();
					System.out.println("Error saving to file");
				} catch (Exception e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
			}
		};
		saveButton.addActionListener(ioListener);
		loadButton = new JButton ("Load file");
		loadButton.setActionCommand("load");
		loadButton.addActionListener(ioListener);
		helpButton = new JButton ("Help"); // UIManager.getIcon("OptionPane.questionIcon")
		helpButton.addActionListener(new ActionListener() {
			JFrame helpFrame = null;
			final Desktop desktop = Desktop.getDesktop(); 
			@Override
			public void actionPerformed(ActionEvent e) {
				if (helpFrame == null || ! helpFrame.isDisplayable()) {
					JTextPane ta = new JTextPane();
					ta.setContentType("text/html");
					ta.addHyperlinkListener(new HyperlinkListener() {
				        @Override
				        public void hyperlinkUpdate(HyperlinkEvent e) {
				            if (HyperlinkEvent.EventType.ACTIVATED == e.getEventType()) {
				                try {
				                    desktop.browse(e.getURL().toURI());
				                } catch (IOException | URISyntaxException e1) {
				                    e1.printStackTrace();
				                }
				            }
				        }
					}
				    );
					ta.setBorder(BorderFactory.createEmptyBorder(5, 5, 5, 5));
//				    ta.setLineWrap(true);
//				    ta.setWrapStyleWord(true);
				    // ta.setBackground(getBackground());
				    ta.setEditable(false);
				    ta.setText("<h2>About</h2>"
				    		+ "<p>The Entropy Triangle is an exploratory data analysis method to evaluate and compare the performance of classifiers. It represents a balance equation of entropies by means of normalized values of the variation of information, the mutual information, and the increment of entropy from the uniform distribution in a ternary plot.</p>"
				    		+ "<p>For more information, see:</p>"
				    		+ "<ul>"
				    		+ "<li><p>Valverde-Albacete, F. J., &amp; Peláez-Moreno, C. (2014). <a href='http://dx.doi.org/10.1371/journal.pone.0084217'>100% classification accuracy considered harmful: The normalized information transfer factor explains the accuracy paradox.</a> PLoS ONE 9(1).</p></li>" 
				    		+ "<br>"
				    		+ "<li><p>Valverde-Albacete, F. J., &amp; Peláez-Moreno, C. (2010). <a href='http://dx.doi.org/10.1016/j.patrec.2010.05.017'>Two information-theoretic tools to assess the performance of multi-class classifiers.</a> Pattern Recognition Letters, Volume 31, Issue 12, 1 September 2010, Pages 1665-1671.</p></li>"
				    		+ "</ul>"
				    		+ "\n\n"
				    		+ "<h2>Help</h2>"
				    		+ "<a href='http://apastor.github.io/entropy-triangle-weka-package'>http://apastor.github.io/entropy-triangle-weka-package</a>"
				    		);
				    ta.setCaretPosition(0);
				    helpFrame = new JFrame("Entropy Triangle Information"); // PropertyDialog.getParentFrame(getTopLevelAncestor()),
				    helpFrame.addWindowListener(new WindowAdapter() {
				      @Override
				      public void windowClosing(WindowEvent e) {
				        helpFrame.dispose();
				      }
				    });
				    helpFrame.getContentPane().setLayout(new BorderLayout());
				    helpFrame.getContentPane().add(new JScrollPane(ta), BorderLayout.CENTER);
				    helpFrame.pack();
				    helpFrame.setSize(400, 600);
//				    helpFrame.setLocationRelativeTo(getTopLevelAncestor());
				    helpFrame.setLocation(((JButton)e.getSource()).getTopLevelAncestor().getLocationOnScreen().x
				      + ((JButton)e.getSource()).getTopLevelAncestor().getSize().width, ((JButton)e.getSource())
				      .getTopLevelAncestor().getLocationOnScreen().y);
				    helpFrame.setVisible(true);
				}
				else {
					helpFrame.toFront();
				}
			}
		});
		
		plotPanel = new JPanel(new BorderLayout());
		plotPanel.setBorder(BorderFactory.createTitledBorder("Plot"));
		plot = new TernaryPlot();
		plotPanel.add(plot, BorderLayout.CENTER);
		colorBarPanel = new JPanel(new BorderLayout());
		colorBarPanel.setBorder(BorderFactory.createTitledBorder("Metric colour"));
		colorBar = new ColorBar();
		colorBar.setBorder(BorderFactory.createEmptyBorder(15, 10, 10, 10));
		colorBarPanel.add(colorBar, BorderLayout.CENTER);
		buttonPanel = new JPanel ();
		buttonPanel.setBorder(BorderFactory.createEmptyBorder(10, 5, 10, 5));
		buttonPanel.add(toggleSplit);
		buttonPanel.add(toggleBaseline);
		buttonPanel.add(saveButton);
		buttonPanel.add(loadButton);
		buttonPanel.add(helpButton);
		
		this.setLayout(new BorderLayout());
		this.add(buttonPanel, BorderLayout.NORTH);
		this.add(plotPanel, BorderLayout.CENTER);
		this.add(colorBarPanel, BorderLayout.SOUTH);
		this.bsline = new ArrayList<Baseline> ();
		this.setPreferredSize(new Dimension(PANELWIDTH,PANELWIDTH));
	}
	
	/**
	 * Adds a set of instances to the data list of the plot,
	 * checks the compatibility of the instances with the 
	 * <code>checkInstance(Instance)</code> method of the 
	 * <code>Instances</code> class.
	 * <br>
	 * Initialize the data list if necessary.
	 * 
	 * @param instances the set of instances to add
	 * @return 0 if the instances are added correctly, -1 if the instances
	 * do not have the metrics needed to plot the data and -2 if the new instances
	 * are not compatible.
	 * @see Instances#checkInstance(Instance)
	 */
	public int addInstancesFromFile(Instances instances) {
		if (instances.isEmpty()) {
			return 0;
		}
		if (instances.attribute(InformationTransferFactor.METRIC_NAME)==null 
				|| instances.attribute(Nit.METRIC_NAME)==null 
				|| instances.attribute(VariationOfInformation.METRIC_NAME)==null
				|| instances.attribute(RemainingPerplexity.METRIC_NAME)==null) {
			return -1;
		}
		if (dataList==null) {
			dataList = new DataInstances(instances,instances.numInstances());
			updatePanels();
		}
		// Check instances compatibility before adding
		if (dataList.checkInstance(instances.firstInstance())) {
			for (Instance inst: instances) {
				addInstance(inst);
			}
		} else {
			return -2;
		}
		return 0;
	}

	/**
	 * Adds to the Entropy Triangle the data of a classifier evaluation process.
	 * 
	 * @param ev performed evaluation on the <code>classifier</code> for the relation named <code>str_dataser</code>  
	 * @param classifier the evaluated classifier
	 * @param str_dataset the name of the evaluated relation
	 * @param str_time a string for the time attribute. If <code>null</code>,
	 * a string with the current timestamp will be generated.
	 * @throws Exception on errors calculating metrics from the <code>Evaluation</code> object
	 */
	//str_time can be null, then current timestamp will be added
	public void addData(Evaluation ev, AbstractClassifier classifier, 
			String str_dataset, String str_time) throws Exception {
		if(dataList==null) {
			ArrayList<Attribute> attInfo;
			List<String> nullList = null;
			attInfo = new ArrayList<Attribute>(14);
			attInfo.add(new Attribute("Dataset", nullList)); // nullList make the Attribute type String
			attInfo.add(new Attribute("Classifier", nullList)); // No null List would make the Attribute type nominal
			attInfo.add(new Attribute("Classifier options", nullList));
			attInfo.add(new Attribute("Classifier version UID", nullList));
			attInfo.add(new Attribute("time", nullList)); // Type String for simplicity, no need of special handling
			attInfo.add(new Attribute("Correct"));
			attInfo.add(new Attribute("Incorrect"));
			attInfo.add(new Attribute("Kappa statistic"));
			attInfo.add(new Attribute("K&B information score"));
			attInfo.add(new Attribute("K&B relative info score"));
			attInfo.add(new Attribute("weighted F measure"));
			attInfo.add(new Attribute("Average cost"));
			attInfo.add(new Attribute("Total cost"));
			attInfo.add(new Attribute("weighted Area Under ROC"));
			List<AbstractEvaluationMetric>  pluginMetrics = ev.getPluginMetrics();
			for (AbstractEvaluationMetric metric : pluginMetrics) {
				if (metric instanceof StandardEvaluationMetric ||
						metric instanceof InformationTheoreticEvaluationMetric) {
					attInfo.add(new Attribute(metric.getStatisticNames().get(0)));
				}
			}
			dataList = new DataInstances(new SimpleDateFormat("dd-MM-yyyy HH:mm").format(new Date()), attInfo);
			updatePanels();
		}
		double[] values = new double[currentMetric.getItemCount()];

		for (int ind = 0; ind < currentMetric.getItemCount(); ind++){
			Attribute atr = dataList.attribute(currentMetric.getItemAt(ind));
			switch (atr.name()){
			case "Dataset":
				values[ind] = atr.addStringValue(str_dataset);
				break;
			case "Classifier":
				String str_classifier = classifier.getClass().getName().replaceFirst("weka.classifiers.", "");
				values[ind] = atr.addStringValue(str_classifier);
				break;
			case "Classifier options":
				String str_options = Utils.joinOptions(classifier.getOptions());
				if (str_options.isEmpty()){
					str_options = "NO OPTIONS";
				}
				values[ind] = atr.addStringValue(str_options);
				break;
			case "Classifier version UID":
				String str_uid = String.valueOf(ObjectStreamClass.lookup(classifier.getClass()).getSerialVersionUID());
				values[ind] = atr.addStringValue(str_uid);
				break;
			case "time":
				if(str_time==null) {
					str_time = new SimpleDateFormat("HH:mm:ss").format(new Date());
				}
				values[ind] = atr.addStringValue(str_time);
				break;
			case "Correct":
				values[ind] = ev.pctCorrect()/100;
				break;
			case "Incorrect":
				values[ind] = ev.pctIncorrect()/100;
				break;
			case "Kappa statistic":
				values[ind] = ev.kappa();
				break;
			case "K&B information score":
				values[ind] = ev.KBMeanInformation();
				break;
			case "K&B relative info score":
				values[ind] = ev.KBRelativeInformation();
				break;
			case "weighted F measure":
				values[ind] = ev.weightedFMeasure();
				break;
			case "Average cost":
				values[ind] = ev.avgCost();
				break;
			case "Total cost":
				values[ind] = ev.totalCost();
				break;
			case "weighted Area Under ROC":
				values[ind] = ev.weightedAreaUnderROC();
				break;
			default:
				AbstractEvaluationMetric pluginMetric = ev.getPluginMetric(atr.name());
				if (pluginMetric!=null) {
					values[ind] = pluginMetric.getStatistic(atr.name());
				} else {
					values[ind] = Utils.missingValue();
				}
			}
		}
		DataInstance newInst = new DataInstance(dataList, 1.0, values);
		addInstance(newInst);
	}
	
	/**
	 * Adds instances to the dataList and plot them accordingly to the current plot values.
	 * Duplicate instances are not added to avoid having multiple times the same points.
	 * 
	 * @param instance the instance to add to the graph
	 */
	private void addInstance(Instance inst) {
		if(dataList.add(inst)) {
			DataInstance newInst = (DataInstance) dataList.lastInstance();
			InstanceComparator instanceComparator = new InstanceComparator(true);
			for (int i = 0; i<dataList.size()-1; i++){
				Instance inList = dataList.get(i);
				if(instanceComparator.compare(inList, newInst)==0){
					dataList.delete(dataList.size()-1);
					return;
				}
			}
			newInst.setPopupMenus(this);
			colorBar.addActionListener(newInst.setColorChangeListener(colorBar));
			plot.add(newInst.joint);
			if(isSplit()) {
				plot.add(newInst.input);
				plot.add(newInst.output);
			}
			addBaseline(newInst);
			colorBar.repaint();
		}
	}

	/**
	 * Properly removes data from the Entropy Triangle. It removes the given instance
	 * and the corresponding label of the <code>Baseline</code> object.
	 * 
	 * @param instance the instance to remove from the graph
	 */
	protected void delInstance(DataInstance instance) {
		if (dataList==null){
			return;
		}
		dataList.remove(instance);
		plot.remove(instance.joint);
		if(isSplit()) {
			plot.remove(instance.input);
			plot.remove(instance.output);
		}
		for (Baseline bs: bsline) {
			if (bs.checkLabel(instance.label)) {
				if(bs.removeLabel(instance.label)==0) {
					bsline.remove(bs);
					plot.remove(bs);
				}
				break;
			}
		}
		if (dataList.isEmpty()){
			clearData();
		}
		colorBar.repaint();
	}
	
	/** Removes all data and resets the panel. */
	public void clearData() {
		plot.removeAllData();
		dataList = null;
		bsline.clear();
		if (currentMetric != null) {
			buttonPanel.remove(currentMetric);
		}
		currentMetric = null;
		colorBar.setInstances(null);
		toggleSplit.setSelected(false);
		toggleBaseline.setSelected(false);
		saveButton.setEnabled(false);
	}

	/**
	 * Checks if the graph is showing the split data.
	 * 
	 * @return true if the split data is visible
	 */
	public boolean isSplit() {
		return toggleSplit.isSelected();
	}
	
	/**
	 * Sets the graph to show separately the data from input and output of the classifier if true is passed,
	 * or just the information of the joint probability distribution if false.
	 * 
	 * @param split the new split state
	 */
	public void setSplit(boolean split) {
		if (dataList==null){
			return;
		}
		if (split) {
			for (Instance d : dataList) {
				plot.add(((DataInstance) d).input);
				plot.add(((DataInstance) d).output);
			}
		} else {
			for (Instance d : dataList) {
				plot.remove(((DataInstance) d).input);
				plot.remove(((DataInstance) d).output);
			}
		}
	}

	/**
	 * Sets the visibility of the H(Px) lines for the different datasets
	 * present in the graph.
	 * @param show wether to show or not the lines
	 */
	public void setBaseline(boolean show) {
		if (show){
			for (Baseline bs: bsline) {
				plot.add(bs);
			}
		}
		else {
			for (Baseline bs: bsline) {
				plot.remove(bs);
			}
		}
	}
	
	private void addBaseline(DataInstance d) {
		for (Baseline bs: bsline) {
			if (Math.abs(bs.getXPlotCoord()-d.normInc_H_Px)<=0.005) {
				bs.addLabel(d.label);
				return;
			}
		}
		// Avoiding the tenth color (White). ColorBar m_DefaultColors.
		Baseline bs = new Baseline(d.normInc_H_Px, d.label, colorBar.m_DefaultColors[bsline.size() % 9]); 
		bsline.add(bs);
		if (toggleBaseline.isSelected())
			plot.add(bs);
	}

	private void updatePanels() {
		plotPanel.setBorder(BorderFactory.createTitledBorder("Plot: "
				+ dataList.relationName()));
		int classIdx = dataList.classIndex();
		dataList.setClassIndex(-1);
		String[] str_att = new String[dataList.numAttributes()];
		for (int i=0; i<str_att.length; i++) {
			str_att[i] = dataList.attribute(i).name();
		}
		currentMetric = new JComboBox<String>(str_att);
		if (classIdx<0) {
			dataList.setClass(dataList.attribute(Ema.METRIC_NAME));
			if (dataList.classAttribute()==null) {
				dataList.setClass(dataList.attribute("Correct"));
			}
		} else {
			dataList.setClassIndex(classIdx);
		}
		
		currentMetric.setSelectedIndex(dataList.classIndex());
		colorBar.setInstances(dataList);
		saveButton.setEnabled(true);
		currentMetric.addActionListener(new ActionListener() {
			@SuppressWarnings("rawtypes")
			@Override
			public void actionPerformed(ActionEvent e) {
				if (dataList != null || !dataList.isEmpty()) {
					dataList.setClassIndex(((JComboBox)e.getSource()).getSelectedIndex());
					colorBar.setCindex(dataList.classIndex());
					colorBar.repaint();
				}
			}
		});
		buttonPanel.add(currentMetric, 0);
		buttonPanel.validate();
		this.setMinimumSize(new Dimension((int) buttonPanel.getPreferredSize().getWidth(), getHeight()));
		this.validate();
		Container window = getTopLevelAncestor();
		if (window instanceof JFrame) {
			JFrame jf = (JFrame) window;
			jf.pack();
			if (jf.getTitle().isEmpty()){
				jf.setTitle("Entropy Triangle");
			} 
		} else {
			window.setMinimumSize(getSize());;
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
