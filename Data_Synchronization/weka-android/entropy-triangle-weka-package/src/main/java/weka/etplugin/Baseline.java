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
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Point;
import java.awt.event.InputEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.geom.AffineTransform;
import java.awt.geom.Line2D;
import java.awt.geom.Point2D;
import java.util.LinkedList;
import java.util.List;

import javax.swing.JColorChooser;
import javax.swing.border.EmptyBorder;

/**
 * This class plots a line in a {@code TernaryPlot} that represents a value of the bottom axis.
 * <br><br>
 * The line shows a tooltip on mouse hover with info of its values and the stored labels,
 * and its color can be changed by clicking the line.
 * <br><br>
 * The parent object should have defined an {@link AffineTransform} with the
 * origin of the plot at the origin of coordinates to place the element correctly.
 * 
 * @author Antonio Pastor
 * @see TernaryPlot
 * @see EntropyTrianglePanel
 * @see AffineTransform
 */
public class Baseline extends PlotElement {

	private static final long serialVersionUID = 843256961391454059L;

	private double normInc_H;
	
	private List<String> labelList;

	private Line2D.Double line;
	
	/** 
	 * Constructor for a Baseline at the specified coordinate and the given text label in red.
	 * 
	 * @param normInc_H the value of the bottom axis of this line
	 * @param dataLabel the label of the first dataset this object represent
	 */
	public Baseline(double normInc_H, String dataLabel) {
		this(normInc_H, dataLabel, Color.RED);
	}
	
	/** 
	 * Constructor for a Baseline at the specified coordinate and the given text label in the given color.
	 * 
	 * @param normInc_H the value of the bottom axis of this line
	 * @param dataLabel the label of the first dataset this object represent
	 * @param color the color of the line in the graph
	 */
	public Baseline(double normInc_H, String dataLabel, Color color) {
		this.normInc_H = normInc_H;
		line = new Line2D.Double(normInc_H, 0,
			normInc_H+(1-normInc_H)*TernaryPlot.COS60, (1-normInc_H)*TernaryPlot.SIN60);
		this.setBorder(new EmptyBorder(0, 0, 0, 0));
		this.setForeground(color);
		labelList = new LinkedList<String>();
		this.addLabel(dataLabel);
		this.addMouseListener(new MouseAdapter() {
			@Override
			public void mouseClicked(MouseEvent e) {
				if ((e.getModifiers() & InputEvent.BUTTON1_MASK) == InputEvent.BUTTON1_MASK) {
					Color tmp = JColorChooser.showDialog(Baseline.this,
							"Select new Color", Baseline.this.getForeground());
					if (tmp != null) {
						Baseline.this.setForeground(tmp);
					}
				}
			}
		});
	}

	@Override
	public void paintComponent(Graphics g) {
		Graphics2D g2D = (Graphics2D) g;
		g2D.setStroke(TernaryPlot.BS_NORMAL);
		g2D.draw(line);
	}
	
	/**
	 *  Gets the coordinate of the line in the plot 
	 * @return the value of the line in the bottom axis
	 */
	public double getXPlotCoord() {
		return normInc_H;
	}
	
	@Override
	public boolean contains(Point p) {
		return this.contains(p.x, p.y);
	}
	
	@Override
	public boolean contains(int x, int y) {
		if (this.getParent() instanceof TernaryPlot) {
			Point2D pt1, pt2;
			AffineTransform af = ((TernaryPlot) this.getParent()).getPlotTransform();
			pt1 = af.transform(line.getP1(), null);
			pt2 = af.transform(line.getP2(), null);
//			System.out.println(pt1);
//			System.out.println(pt2);
//			System.out.println(Line2D.ptSegDist(pt1.getX(), pt1.getY(), pt2.getX(), pt2.getY(), x, y));
			if (Line2D.ptSegDist(pt1.getX(), pt1.getY(), pt2.getX(), pt2.getY(), x+getX(), y+getY())<2) {
				return true;
			}
			return false;
		}
		// The parent not being a Ternary Plot instance is not contemplated
		if (line.ptSegDist(x, y)<5) {
			return true;
		}
		return false;
	}
	
	/**
	 * Adds a label to the label list. Updates the tooltip to show information of the new label.
	 * 
	 * @param label a string that identifies the new data this label is going to represent
	 */
	public void addLabel(String label) {
		if (label==null) {
			return;
		}
		if (labelList.contains(label)==false) {
			labelList.add(label);
			updateToolTipText();
		}
	}

	/**
	 * Checks if the label list contains the given string.	 * 
	 * @param label the string to check if it is in the label list
	 * 
	 * @return true if there is an identical label string in the list, false if not
	 */
	public boolean checkLabel(String label) {
		return labelList.contains(label);
	}
	
	/**
	 * Deletes the given string from the label list.	 * 
	 * @param label the string to be removed from the list
	 * @return the number of labels remaining in the list after the operation, -1 if the label could not be removed
	 */
	public int removeLabel(String label) {
		if (labelList.remove(label)) {
			updateToolTipText();
			return labelList.size();
		} else {
			return -1;
		}
	}
	
 	private void updateToolTipText() {
		StringBuffer text = new StringBuffer();
		StringBuffer labelBuffer;
		text.append("<html>");
		
		if (labelList.isEmpty()==false) {
			text.append(" Baseline for:<br/>");
			text.append("<table>");

			for (String label: labelList) {
				if (label!=null) {
					labelBuffer = new StringBuffer();
					int labelLen = label.length();
					// If label is too long break it in multiple lines
					for (int i=0; i<labelLen; i+=70) {
						labelBuffer.insert(labelBuffer.length(), label, i, Math.min(i+70, labelLen));
						labelBuffer.append("<br/>");
					}
					text.append("<tr><td>" + labelBuffer + "</td></tr>");
				}
			}
			text.append("</table>");
		}
		text.append("\u0394" + "H'(Px): ");
		text.append(String.format("%.4f<br/>", normInc_H));
		text.append("max MI': ");
		text.append(String.format("%.4f<br/>", 1-normInc_H));
		text.append("</html>");
		this.setToolTipText(text.toString());
	}

 	/**
 	 * Gets the preference of the object to be drawn on top of elements without preference.
 	 * @return false
 	 */
	@Override
	public boolean isTopElement() {
		return false;
	}
	
	/**
	 * Gets the element type.
	 * @return {@link PlotElement#LINE_ELEMENT}
	 */
	@Override
	public int ElementType() {
		return PlotElement.LINE_ELEMENT;
	}

}
