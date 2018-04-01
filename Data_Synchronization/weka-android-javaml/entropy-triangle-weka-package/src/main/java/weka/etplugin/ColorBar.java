/*
 *   This file is part of entropy-triangle-weka-package.
 *
 *   This program is free software: you can redistribute it
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 *    ColorBar.java (Modified version of the Weka source file ClassPanel.java)
 *    Copyright of the changes (C) 2015-2017 Antonio Pastor
 *
 *    ClassPanel.java
 *    Copyright (C) 2000-2012 University of Waikato, Hamilton, New Zealand
 *
 */

package weka.etplugin;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.LinearGradientPaint;
import java.awt.MultipleGradientPaint;
import java.awt.Paint;
import java.awt.PaintContext;
import java.awt.Rectangle;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.InputEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.geom.AffineTransform;
import java.awt.geom.Point2D;
import java.awt.image.ColorModel;
import java.awt.image.DataBuffer;
import java.util.ArrayList;

import javax.swing.JColorChooser;
import javax.swing.JLabel;
import javax.swing.JPanel;

import weka.core.Instances;
import weka.core.Utils;

/**
 * This panel displays colored labels for nominal or string attributes and a rainbow style gradient for numeric attributes.
 * <br><br>
 * This class is a modified version of the Weka's {@link weka.gui.visualize.ClassPanel} to fit the needs of the Entropy Triangle package.
 * It incorporates the {@link #getColor(double)} method to get the color of the gradient or labels for a given value.
 * 
 * As the original <code>ClassPanel</code> of Weka,
 * the ColorBar notifies to the objects on the repainters list and
 * to the registered ActionListeners when the class attribute is changed or the range of values is modified.
 * 
 * @author Mark Hall (mhall@cs.waikato.ac.nz)
 * @author Malcolm Ware (mfw4@cs.waikato.ac.nz)
 * @author Antonio Pastor
 * @see weka.gui.visualize.ClassPanel
 */
public class ColorBar extends JPanel {

	/** for serialization */
	private static final long serialVersionUID = -7969401840501661430L;

	private boolean wekaVersion_3_7_12_or_older = new weka.core.Version().isOlder("3.7.13");

	private DataBuffer colorBuffer = null;

	private float[] dist = {0f, 0.35f, 0.7f, 1f};
	private Color[] colors = {Color.BLUE, Color.GREEN, Color.YELLOW, Color.RED};

	private int bar_width = 0;

	/** Default maximum precision for the display of numeric values */
	protected static int MAX_PRECISION = 10;

	/** True if the colouring attribute is numeric */
	private boolean m_isNumeric = false;

	/** The height of the spectrum for numeric class */
	private final int m_spectrumHeight = 5;

	/** The maximum value for the colouring attribute */
	private double m_maxC;

	/** The minimum value for the colouring attribute */
	private double m_minC;

	/** The size of the ticks */
	private final int m_tickSize = 5;

	/** Font metrics */
	private FontMetrics m_labelMetrics = null;

	/** The font used in labeling */
	private Font m_labelFont = null;

	/** The amount of space to leave either side of the legend */
	private int m_HorizontalPad = 0;

	/** The precision with which to display real values */
	private int m_precisionC;

	/** The old width. */
	private int m_oldWidth = -9000;

	/** Instances being plotted */
	private Instances m_Instances = null;

	/** the list of colours to use for colouring nominal attribute labels */
	private ArrayList<Color> m_colorList;

	/**
	 * An optional list of Components that use the colour list maintained by this
	 * class. If the user changes a colour using the colour chooser, then these
	 * components need to be repainted in order to display the change
	 */
	private final ArrayList<Component> m_Repainters = new ArrayList<Component>();

	/**
	 * An optional list of listeners who want to know when a colour changes.
	 * Listeners are notified via an ActionEvent
	 */
	private final ArrayList<ActionListener> m_ColourChangeListeners = new ArrayList<ActionListener>();

	/** default colours for colouring discrete class */
	protected Color[] m_DefaultColors = { Color.blue, Color.red, Color.green,
			Color.cyan, Color.pink, new Color(255, 0, 255), Color.orange,
			new Color(255, 0, 0), new Color(0, 255, 0), Color.white };

	/**
	 * if set, it allows this panel to steer away from setting up a color in the
	 * color list that is equal to the background color
	 */
	protected Color m_backgroundColor = null;

	/**
	 * Inner Inner class used to create labels for nominal attributes so that
	 * there color can be changed.
	 */
	private class NomLabel extends JLabel {

		/** for serialization */
		private static final long serialVersionUID = -4686613106474820655L;

		private int m_index = 0;

		/**
		 * Creates a label with its name and class index value.
		 * 
		 * @param name The name of the nominal class value.
		 * @param id The index value for that class value.
		 */
		public NomLabel(String name, int id) {
			super(name);
			m_index = id;
			this.addMouseListener(new MouseAdapter() {
				@Override
				public void mouseClicked(MouseEvent e) {
					if ((e.getModifiers() & InputEvent.BUTTON1_MASK) == InputEvent.BUTTON1_MASK) {
						Color tmp = JColorChooser.showDialog(ColorBar.this,
								"Select new Color", m_colorList.get(m_index));
						if (tmp != null) {
							m_colorList.set(m_index, tmp);
							m_oldWidth = -9000;
							ColorBar.this.repaint();
							if (m_Repainters.size() > 0) {
								for (int i = 0; i < m_Repainters.size(); i++) {
									(m_Repainters.get(i)).repaint();
								}
							}
							if (m_ColourChangeListeners.size() > 0) {
								for (int i = 0; i < m_ColourChangeListeners.size(); i++) {
									(m_ColourChangeListeners.get(i))
									.actionPerformed(new ActionEvent(this, 0, ""));
								}
							}
						}
					}
				}
			});
		}
	}

	/** Default constructor, with default background color. */
	public ColorBar() {
		this(null);
	}

	/**
	 * Constructor specifying the background color. 
	 * @param background the background color of the panel
	 */
	public ColorBar(Color background) {
		m_backgroundColor = background;
		/** Set up some default colours */
		m_colorList = new ArrayList<Color>(10);
		for (int noa = m_colorList.size(); noa < 10; noa++) {
			Color pc = m_DefaultColors[noa % 10];
			int ija = noa / 10;
			ija *= 2;
			for (int j = 0; j < ija; j++) {
				pc = pc.darker();
			}
			m_colorList.add(pc);
		}
		this.setVisible(true);
	}

	/**
	 * Adds a component that will need to be repainted if the user changes the
	 * color of a label.
	 * 
	 * @param c the component to be repainted
	 */
	public void addRepaintNotify(Component c) {
		m_Repainters.add(c);
	}

	/**
	 * Add an action listener that will be notified if the user changes the color
	 * of a label
	 * 
	 * @param a an <code>ActionListener</code> value
	 */
	public void addActionListener(ActionListener a) {
		m_ColourChangeListeners.add(a);
	}

	/**
	 * Set up fonts and font metrics
	 * 
	 * @param gx the graphics context
	 */
	private void setFonts(Graphics gx) {
		if (m_labelMetrics == null) {
			m_labelFont = new Font("Monospaced", Font.PLAIN, 12);
			m_labelMetrics = gx.getFontMetrics(m_labelFont);
			int hf = m_labelMetrics.getAscent();
			if (this.getHeight() < (3 * hf)) {
				m_labelFont = new Font("Monospaced", Font.PLAIN, 11);
				m_labelMetrics = gx.getFontMetrics(m_labelFont);
			}
		}
		gx.setFont(m_labelFont);
	}

	/**
	 * Set the instances.
	 * 
	 * @param insts the instances
	 */
	public void setInstances(Instances insts) {
		m_Instances = insts;
		if(insts==null){
			this.m_isNumeric=false;
			this.m_oldWidth = -9000;
		} else {
			setCindex(insts.classIndex());
		}
	}

	/**
	 * Set the index of the attribute to display colored labels for
	 * 
	 * @param cIndex the index of the attribute to display colored labels for
	 */
	public void setCindex(int cIndex) {
		if (m_Instances != null && m_Instances.numAttributes() > 0) {
			if (m_Instances.classAttribute().isNumeric()) {
				setNumeric();
			} else {
				if (m_Instances.classAttribute().numValues() > m_colorList.size()) {
					extendColourMap();
				}
				setNominal();
			}
		}
	}

	/**
	 * Extends the list of colors if a new attribute with more values than the
	 * previous one is chosen
	 */
	private void extendColourMap() {
		for (int i = m_colorList.size(); i < m_Instances.classAttribute()
				.numValues(); i++) {
			Color pc = m_DefaultColors[i % 10];
			int ija = i / 10;
			ija *= 2;
			for (int j = 0; j < ija; j++) {
				pc = pc.brighter();
			}
			if (m_backgroundColor != null) {
				pc = ColorBar.checkAgainstBackground(pc, m_backgroundColor);
			}

			m_colorList.add(pc);
		}
	}

	protected void setDefaultColourList(Color[] list) {
		m_DefaultColors = list;
	}

	/**
	 * Set a list of colours to use for colouring labels
	 * 
	 * @param cols a list containing java.awt.Colors
	 */
	public void setColours(ArrayList<Color> cols) {
		m_colorList = cols;
	}

	/**
	 * Sets the legend to be for a nominal variable
	 */
	protected void setNominal() {
		m_isNumeric = false;
		m_HorizontalPad = 0;
		m_oldWidth = -9000;

		this.repaint();
	}

	/**
	 * Sets the legend to be for a numeric variable
	 */
	protected void setNumeric() {
		m_isNumeric = true;
		/*
		 * m_maxC = mxC; m_minC = mnC;
		 */

		double min = Double.POSITIVE_INFINITY;
		double max = Double.NEGATIVE_INFINITY;
		double value;

		int numInstances = 0;
		if (m_Instances != null) {
			numInstances = m_Instances.numInstances();
		}
		for (int i = 0; i < numInstances; i++) {
			if (!m_Instances.instance(i).classIsMissing()) {
				value = m_Instances.instance(i).classValue();
				if (value < min) {
					min = value;
				}
				if (value > max) {
					max = value;
				}
			}
		}

		// handle case where all values are missing
		if (min == Double.POSITIVE_INFINITY) {
			min = 0;
			max = 1;
		} else if (min==max) {
			if (min>0)
				min = 0;
			else
				max = 1;
		}

		m_minC = min;
		m_maxC = max;

		int whole = (int) Math.abs(m_maxC);
		double decimal = Math.abs(m_maxC) - whole;
		int nondecimal;
		nondecimal = (whole > 0) ? (int) (Math.log(whole) / Math.log(10)) : 1;

		m_precisionC = (decimal > 0) ? (int) Math
				.abs(((Math.log(Math.abs(m_maxC)) / Math.log(10)))) + 2 : 1;
		if (m_precisionC > MAX_PRECISION) {
			m_precisionC = 1;
		}

		String maxStringC = Utils.doubleToString(m_maxC, nondecimal + 1
				+ m_precisionC, m_precisionC);
		if (m_labelMetrics != null) {
			m_HorizontalPad = m_labelMetrics.stringWidth(maxStringC);
		}

		whole = (int) Math.abs(m_minC);
		decimal = Math.abs(m_minC) - whole;
		nondecimal = (whole > 0) ? (int) (Math.log(whole) / Math.log(10)) : 1;

		m_precisionC = (decimal > 0) ? (int) Math
				.abs(((Math.log(Math.abs(m_minC)) / Math.log(10)))) + 2 : 1;
		if (m_precisionC > MAX_PRECISION) {
			m_precisionC = 1;
		}

		maxStringC = Utils.doubleToString(m_minC, nondecimal + 1 + m_precisionC,
				m_precisionC);
		if (m_labelMetrics != null) {
			if (m_labelMetrics.stringWidth(maxStringC) > m_HorizontalPad) {
				m_HorizontalPad = m_labelMetrics.stringWidth(maxStringC);
			}
		}
	}

	/**
	 * Renders the legend for a nominal colouring attribute
	 * 
	 * @param gx the graphics context
	 */
	protected void paintNominal(Graphics gx) {
		setFonts(gx);
		
		int numClasses = m_Instances.classAttribute().numValues();
		int strOffset = 0;
		if (m_Instances.classAttribute().isString() && wekaVersion_3_7_12_or_older){
			strOffset = 1; // STRING ATTRIBUTE DUMMY STRING
		}

		int maxLabelLen = 0;
		int idx = 0;
		int legendHeight;
		int w = this.getWidth();
		int hf = m_labelMetrics.getAscent();

		for (int i = strOffset; i < numClasses; i++) {
			if (m_Instances.classAttribute().value(i).length() > maxLabelLen) {
				maxLabelLen = m_Instances.classAttribute().value(i).length();
				idx = i;
			}
		}

		maxLabelLen = m_labelMetrics.stringWidth(m_Instances.classAttribute().value(idx));

		if (((w - (2 * m_HorizontalPad)) / (maxLabelLen + 5)) >= numClasses-strOffset) {
			legendHeight = 1;
		} else {
			legendHeight = 2;
		}

		this.setSize(getWidth(), 20 + legendHeight * hf);

		int x = m_HorizontalPad;
		int y = 1 + hf;

		int numToDo = ((legendHeight == 1) ? numClasses-strOffset : ((numClasses-strOffset) / 2));
		for (int i = 0; i < numToDo; i++) {
			gx.setColor(m_colorList.get(i));
			// can we fit the full label or will each need to be trimmed?
			if ((numToDo * maxLabelLen) > (w - (m_HorizontalPad * 2))) {
				String val;
				val = m_Instances.classAttribute().value(i+strOffset);

				int sw = m_labelMetrics.stringWidth(val);
				int rm = 0;
				// truncate string if necessary
				if (sw > ((w - (m_HorizontalPad * 2)) / (numToDo))) {
					int incr = (sw / val.length());
					rm = (sw - ((w - (m_HorizontalPad * 2)) / numToDo)) / incr;
					if (rm <= 0) {
						rm = 0;
					}
					if (rm >= val.length()) {
						rm = val.length() - 1;
					}
					val = val.substring(0, val.length() - rm);
					sw = m_labelMetrics.stringWidth(val);
				}
				NomLabel jj = new NomLabel(val, i);
				jj.setFont(gx.getFont());

				jj.setSize(m_labelMetrics.stringWidth(jj.getText()),
						m_labelMetrics.getAscent() + 4);
				this.add(jj);
				jj.setLocation(x, y);
				jj.setForeground(m_colorList.get(i % m_colorList.size()));

				x += sw + 2;
			} else {

				NomLabel jj;
				jj = new NomLabel(m_Instances.classAttribute().value(i+strOffset), i);

				jj.setFont(gx.getFont());

				jj.setSize(m_labelMetrics.stringWidth(jj.getText()),
						m_labelMetrics.getAscent() + 4);
				this.add(jj);
				jj.setLocation(x, y);
				jj.setForeground(m_colorList.get(i % m_colorList.size()));

				x += ((w - (m_HorizontalPad * 2)) / numToDo);
			}
		}

		x = m_HorizontalPad;
		y = 1 + hf + 5 + hf;
		for (int i = numToDo; i < numClasses-strOffset; i++) {

			gx.setColor(m_colorList.get(i));
			if (((numClasses - numToDo + 1) * maxLabelLen) > (w - (m_HorizontalPad * 2))) {
				String val;
				val = m_Instances.classAttribute().value(i+strOffset);

				int sw = m_labelMetrics.stringWidth(val);
				int rm = 0;
				// truncate string if necessary
				if (sw > ((w - (m_HorizontalPad * 2)) / (numClasses - numToDo + 1))) {
					int incr = (sw / val.length());
					rm = (sw - ((w - (m_HorizontalPad * 2)) / (numClasses - numToDo)))
							/ incr;
					if (rm <= 0) {
						rm = 0;
					}
					if (rm >= val.length()) {
						rm = val.length() - 1;
					}
					val = val.substring(0, val.length() - rm);
					sw = m_labelMetrics.stringWidth(val);
				}
				// this is the clipped string
				NomLabel jj = new NomLabel(val, i);
				jj.setFont(gx.getFont());

				jj.setSize(m_labelMetrics.stringWidth(jj.getText()),
						m_labelMetrics.getAscent() + 4);

				this.add(jj);
				jj.setLocation(x, y);
				jj.setForeground(m_colorList.get(i % m_colorList.size()));

				x += sw + 2;
			} else {
				// this is the full string
				NomLabel jj;
				jj = new NomLabel(m_Instances.classAttribute().value(i+strOffset), i);

				jj.setFont(gx.getFont());

				jj.setSize(m_labelMetrics.stringWidth(jj.getText()),
						m_labelMetrics.getAscent() + 4);
				this.add(jj);
				jj.setLocation(x, y);
				jj.setForeground(m_colorList.get(i % m_colorList.size()));

				x += ((w - (m_HorizontalPad * 2)) / ((numClasses-strOffset) - numToDo));
			}
		}

	}

	/**
	 * Renders the legend for a numeric coloring attribute
	 * 
	 * @param g the graphics context
	 */
	protected void paintNumeric(Graphics g) {

		Graphics2D g2 = (Graphics2D) g;

		setFonts(g2);
		if (m_Instances == null) {
			m_HorizontalPad = 10;
		}
		if (m_HorizontalPad == 0) {
			setCindex(m_Instances.classIndex());
		}

		int w = this.getWidth();
		int hf = m_labelMetrics.getAscent();

		Paint paintBckup = g2.getPaint();

		bar_width = w - 2*m_HorizontalPad;

		g2.setPaint(setGradient(g2));
		g2.fillRect(m_HorizontalPad, 0, bar_width, 0 + m_spectrumHeight);

		g2.setPaint(paintBckup);

		int whole = (int) Math.abs(m_maxC);
		double decimal = Math.abs(m_maxC) - whole;
		int nondecimal;
		nondecimal = (whole > 0) ? (int) (Math.log(whole) / Math.log(10)) : 1;

		m_precisionC = (decimal > 0) ? (int) Math
				.abs(((Math.log(Math.abs(m_maxC)) / Math.log(10)))) + 2 : 1;
		if (m_precisionC > MAX_PRECISION) {
			m_precisionC = 1;
		}

		String maxStringC = Utils.doubleToString(m_maxC, nondecimal + 1
				+ m_precisionC, m_precisionC);

		int mswc = m_labelMetrics.stringWidth(maxStringC);
		int tmsc = mswc;
		if (w > (2 * tmsc)) {
			g2.setColor(Color.black);
			g2.drawLine(m_HorizontalPad, (m_spectrumHeight + 5), w - m_HorizontalPad,
					(m_spectrumHeight + 5));

			g2.drawLine(w - m_HorizontalPad, (m_spectrumHeight + 5), w
					- m_HorizontalPad, (m_spectrumHeight + 5 + m_tickSize));

			g2.drawString(maxStringC, (w - m_HorizontalPad) - (mswc / 2),
					(m_spectrumHeight + 5 + m_tickSize + hf));

			g2.drawLine(m_HorizontalPad, (m_spectrumHeight + 5), m_HorizontalPad,
					(m_spectrumHeight + 5 + m_tickSize));

			whole = (int) Math.abs(m_minC);
			decimal = Math.abs(m_minC) - whole;
			nondecimal = (whole > 0) ? (int) (Math.log(whole) / Math.log(10)) : 1;

			m_precisionC = (decimal > 0) ? (int) Math
					.abs(((Math.log(Math.abs(m_minC)) / Math.log(10)))) + 2 : 1;

			if (m_precisionC > MAX_PRECISION) {
				m_precisionC = 1;
			}

			maxStringC = Utils.doubleToString(m_minC, nondecimal + 1 + m_precisionC,
					m_precisionC);

			mswc = m_labelMetrics.stringWidth(maxStringC);
			g2.drawString(maxStringC, m_HorizontalPad - (mswc / 2), (m_spectrumHeight
					+ 5 + m_tickSize + hf));

			// draw the middle value if there is space
			if (w > (3 * tmsc)) {
				double mid = m_minC + ((m_maxC - m_minC) / 2.0);
				g2.drawLine(m_HorizontalPad + ((w - (2 * m_HorizontalPad)) / 2),
						(m_spectrumHeight + 5), m_HorizontalPad
						+ ((w - (2 * m_HorizontalPad)) / 2),
						(m_spectrumHeight + 5 + m_tickSize));

				whole = (int) Math.abs(mid);
				decimal = Math.abs(mid) - whole;
				nondecimal = (whole > 0) ? (int) (Math.log(whole) / Math.log(10)) : 1;

				m_precisionC = (decimal > 0) ? (int) Math
						.abs(((Math.log(Math.abs(mid)) / Math.log(10)))) + 2 : 1;
				if (m_precisionC > MAX_PRECISION) {
					m_precisionC = 1;
				}

				maxStringC = Utils.doubleToString(mid, nondecimal + 1 + m_precisionC,
						m_precisionC);

				mswc = m_labelMetrics.stringWidth(maxStringC);
				g2.drawString(maxStringC, m_HorizontalPad
						+ ((w - (2 * m_HorizontalPad)) / 2) - (mswc / 2), (m_spectrumHeight
								+ 5 + m_tickSize + hf));
			}
		}
		
		this.setSize(getWidth(), m_spectrumHeight + 5 + m_tickSize + hf + 20);
	}

	@Override
	public void paintComponent(Graphics gx) {
		super.paintComponent(gx);
		if (m_isNumeric) {
			m_oldWidth = -9000; // done so that if change back to nom, it will work
			//this.setEnabled(false);
			this.removeAll();
			setNumeric();
			paintNumeric(gx);
		} else {
			if (m_Instances != null && m_Instances.numInstances() > 0
					&& m_Instances.numAttributes() > 0) {
				int nClasses = m_Instances.classAttribute().numValues();
				if (m_Instances.classAttribute().isString() && wekaVersion_3_7_12_or_older) {
					nClasses--; // Value 0 of String attributes is a Dummy String that the colorBar eliminates in versions older than 3.7.13
				}
				if (m_oldWidth != this.getWidth() || nClasses != this.getComponentCount()) {
					this.removeAll();
					m_oldWidth = this.getWidth();
					paintNominal(gx);
				}
			} else {
				this.removeAll();
			}
		}
		if (m_ColourChangeListeners.size() > 0) {
			for (int i = 0; i < m_ColourChangeListeners.size(); i++) {
				(m_ColourChangeListeners.get(i))
				.actionPerformed(new ActionEvent(this, 0, ""));
			}
		}
	}



	/**
	 * Gets the color of the given value in the color bar for numeric classes
	 * and the corresponding label color for nominal or string classes.
	 * 
	 * @param value the value of the numeric attribute (the class) or the index of the label for nominal or string attributes.
	 * @return the corresponding color
	 */
	public Color getColor(double value){
		Color color = Color.BLACK;
		if (m_isNumeric) {
			int point = Math.round(standardizeValue(value)*(bar_width-1));
			try {
				if (colorBuffer==null) {
					return color;
				}
				color = new Color(colorBuffer.getElem(point));
			} catch (ArrayIndexOutOfBoundsException e) {
				color = Color.GRAY;
			}
		} else {
			if(wekaVersion_3_7_12_or_older){
				value--; // Value 0 of String attributes is a Dummy String that the colorBar eliminates in versions older than 3.7.13
			}
			color = this.m_colorList.get((int) Math.round(value));
		}
		return color;
	}

	// Method to calculate the colorBuffer index of a given a value.
	private float standardizeValue(double d) {
		// TODO d>max or d<min should throw exception
		if (m_maxC-m_minC<=0)
			return 1;
		else if (d>m_maxC)
			return 1;
		else if (d<m_minC)
			return 0;
		else
			return (float) ((d - m_minC)/ (m_maxC-m_minC));
	}

	/*
	 * Builds the linear gradient to paint the color bar for numerics attributes
	 * and stores the color values in the colorBuffer.
	 * 
	 * @see getColor(double value)
	 */
	private LinearGradientPaint setGradient(Graphics2D g){
		if (this.getWidth()<= 2*m_HorizontalPad) {
			colorBuffer = null;
			return null;
		}
		LinearGradientPaint gradient = new LinearGradientPaint (new Point2D.Double(m_HorizontalPad,0),new Point2D.Double(this.getWidth()-m_HorizontalPad,0),dist,colors,
				MultipleGradientPaint.CycleMethod.NO_CYCLE,MultipleGradientPaint.ColorSpaceType.LINEAR_RGB,new AffineTransform());

		if (colorBuffer == null || colorBuffer.getSize() != bar_width) {
			Rectangle box = new Rectangle(m_HorizontalPad, 0, bar_width+1, 1);
			PaintContext pc = gradient.createContext(ColorModel.getRGBdefault(), box, box,
					g.getTransform(),g.getRenderingHints());
			colorBuffer =  pc.getRaster(m_HorizontalPad, 0, bar_width, 1).getDataBuffer();
		}
		return gradient;
	}

	protected static Color checkAgainstBackground(Color c, Color background) {
		if (background == null) {
			return c;
		}

		if (c.equals(background)) {
			int red = c.getRed();
			int blue = c.getBlue();
			int green = c.getGreen();
			red += (red < 128) ? (255 - red) / 2 : -(red / 2);
			blue += (blue < 128) ? (blue - red) / 2 : -(blue / 2);
			green += (green < 128) ? (255 - green) / 2 : -(green / 2);
			c = new Color(red, green, blue);
		}
		return c;
	}

	/**
	 * Main method for testing this class.
	 * 
	 * @param args first argument must specify an arff file. Second can specify an
	 *          optional index to color labels on
	 */
	public static void main(String[] args) {
		try {
			if (args.length < 1) {
				System.err.println("Usage : weka.etplugin.ColorPanel <dataset> "
						+ "[class col]");
				System.exit(1);
			}
			final javax.swing.JFrame jf = new javax.swing.JFrame(
					"Color bar test");
			jf.setSize(500, 100);
			jf.getContentPane().setLayout(new BorderLayout());
			final ColorBar p2 = new ColorBar();
			jf.getContentPane().add(p2, BorderLayout.CENTER);
			jf.addWindowListener(new java.awt.event.WindowAdapter() {
				@Override
				public void windowClosing(java.awt.event.WindowEvent e) {
					jf.dispose();
					System.exit(0);
				}
			});
			
			if (args.length >= 1) {
				System.err.println("Loading instances from " + args[0]);
				java.io.Reader r = new java.io.BufferedReader(new java.io.FileReader(
						args[0]));
				Instances i = new Instances(r);
				i.setClassIndex(i.numAttributes() - 2);
				p2.setInstances(i);
			}
			jf.setVisible(true);
		} catch (Exception ex) {
			ex.printStackTrace();
			System.err.println(ex.getMessage());
		}
	}
}
