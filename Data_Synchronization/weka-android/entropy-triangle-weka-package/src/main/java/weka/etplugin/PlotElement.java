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

import javax.swing.JComponent;

/**
 * Interface that allows ordering sorting the drawing preference of different types of elements of a plot.
 * It has two layers of preference, if the object of an implementing class is a {@link #TOP_ELEMENT}.
 * <br>
 * <br>
 * The order of elements from top to bottom is: <br><br>
 * <pre>
 * 		<code>POINT_ELEMENT &gt; LINE_ELEMENT &gt; AXES_ELEMENT &gt; GRID_ELEMENT</code>
 * </pre> 
 * All element types can be Top. The same sort is reproduced within top elements.<br><br>
 * Top elements of type top element are drawn above all, useful for non graph parts, i.e. a legend.
 * 
 * @author Antonio Pastor
 * @see TernaryPlot
 */
public abstract class PlotElement extends JComponent implements Comparable<PlotElement> {

	private static final long serialVersionUID = 881688448828612275L;

	/** Constant for element with preference to be drawn on top of regular elements */
	public static int TOP_ELEMENT = 4;
	
	/** Constant for plot elements of type grid */
	public static int GRID_ELEMENT = -4;
	
	/** Constant for plot elements that are axes */
	public static int AXES_ELEMENT = -3;
	
	/** Constant for plot elements of type that are lines */
	public static int LINE_ELEMENT = -2;
	
	/** Constant for punctual plot elements */
	public static int POINT_ELEMENT = -1;
	
	/**
	 * Gets the preference of the object to be drawn on top of elements without preference. 
	 * @return true if the plot element has to be drawn over elements of the same type.
	 * @see #ElementType
	 */
	public abstract boolean isTopElement();
	
	
	/**
	 * Gets the element type.
	 * @return the number of the constant that defines the object element type. 
	 */
	public abstract int ElementType();

	/** 
	 * Compares this object with the specified object for order.
	 * Returns a negative integer, zero, or a positive integer as this object is less than,
	 * equal to, or greater than the specified object.
	 * 
	 * @param o the object to be compared
	 * @return a negative integer, zero, or a positive integer as this object is less than, equal to, or greater than the specified object
	 */
	@Override
	public int compareTo(PlotElement o) {
		return typeSum()-o.typeSum();
	}

	// Auxiliary method to compute the precedence index of the PlotElement.
	private int typeSum() {
		int n = 0;
		if (isTopElement()) {
			n+=PlotElement.TOP_ELEMENT;
		}
		return n+ElementType();
	}
}
