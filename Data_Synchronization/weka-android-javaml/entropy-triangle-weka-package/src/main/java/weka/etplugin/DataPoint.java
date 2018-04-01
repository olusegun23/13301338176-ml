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
import java.awt.geom.AffineTransform;
import java.awt.geom.Path2D;
import java.awt.geom.Point2D;

/**
 * This class plots a figure (a diamond) to represent a point in a graph.
 * 
 * The element is drawn with the center at the given plot coordinates.
 * The parent object should have defined an {@link AffineTransform} with the
 * origin of the plot at the origin of coordinates to place the element correctly.
 * 
 * @author Antonio Pastor
 * @see TernaryPlot
 * @see DataInstance
 * @see AffineTransform
 */
public class DataPoint extends PlotElement {

	private static final long serialVersionUID = -8544622101625666030L;

	private static double WIDTH = 0.02;
	private static double HEIGHT = 0.0324;
	
	private Path2D.Double shape;
	
	private Point2D.Double plotCoords;
	
	/**
	 * Constructor for a DataPoint at the given plot coordinates in black. 
	 * @param plotCoords the x and y coordinates of the point in a 2d graph
	 */
	public DataPoint(Point2D.Double plotCoords) {
		this(plotCoords, Color.BLACK);
	}
	
	/**
	 * Constructor for a DataPoint at the given plot coordinates with the specified foreground color. 
	 * @param coords the x and y coordinates of the point in a 2d graph
	 * @param color the foreground color for this point
	 */
	public DataPoint(Point2D.Double coords, Color color) {
		this.plotCoords = coords;
		this.setForeground(color);
		shape = DataPoint.getFigure(plotCoords);
	}
	
	@Override
	protected void paintComponent(Graphics g) {
		Graphics2D g2D = (Graphics2D) g;
		g2D.setColor(getForeground());
		g2D.fill(shape);
		g2D.setColor(Color.BLACK);
		g2D.setStroke(TernaryPlot.BS_BORDER);
		g2D.draw(shape);
	}
	
	/**
	 * Gets the coordinates of the point.
	 * @return the coordinates of the Point in the plot in double precision
	 */
	public Point2D.Double getPlotCoords() {
		return plotCoords;
	}
	
	@Override
	public boolean contains(Point p) {
		return contains(p.x,p.y);
	}
	
	@Override
	public boolean contains(int x, int y) {
		if (this.getParent() instanceof TernaryPlot) {
			AffineTransform af = ((TernaryPlot) this.getParent()).getPlotTransform();
			Path2D tf = (Path2D) shape.clone();
			tf.transform(af);
			return tf.contains(new Point(x+getX(),y+getY()));
		}
		return false;
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
	 * @return {@link PlotElement#POINT_ELEMENT}
	 */
	@Override
	public int ElementType() {
		return PlotElement.POINT_ELEMENT;
	}

	/* Builds the figure to plot the object */
	private static Path2D.Double getFigure(Point2D.Double p) {
		Path2D.Double fig = new Path2D.Double();
		fig.moveTo(p.x-WIDTH/2,	p.y);
		fig.lineTo(p.x, 		p.y-HEIGHT/2);
		fig.lineTo(p.x+WIDTH/2, p.y);
		fig.lineTo(p.x,			p.y+HEIGHT/2);
		fig.closePath();
		return fig;
	}

}
