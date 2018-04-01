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
import java.awt.geom.Line2D;

import javax.swing.border.EmptyBorder;

final class TernaryGrid extends PlotElement {
	
	private double dist = TernaryPlot.GRID;

	
	public TernaryGrid(){
		this.setBorder(new EmptyBorder(0, 0, 0, 0));
	}
	/**
	 * @return the distance between grid lines
	 */
	protected double getDist() {
		return dist;
	}

	/**
	 * @param dist the distance between grid lines to set
	 */
	protected void setDist(int dist) {
		if(dist<=1 && dist >= 0 && dist%0.05==0)
			this.dist = dist;
	}
	
	protected void paintComponent(Graphics g){
		Graphics2D g2D =(Graphics2D) g;
		g2D.setColor(Color.LIGHT_GRAY);
		g2D.setStroke(TernaryPlot.BS_DASHED);
		
		if (dist!=0)
			for (double i=dist; i<0.99; i+=dist) {
				g2D.draw(new Line2D.Double(i, 0, i+(1-i)*TernaryPlot.COS60, (1-i)*TernaryPlot.SIN60)); // DeltaH axis dashed lines
				g2D.draw(new Line2D.Double(1-i*TernaryPlot.COS60, i*TernaryPlot.SIN60, i*TernaryPlot.COS60, i*TernaryPlot.SIN60)); // MI axis dashed lines
				g2D.draw(new Line2D.Double((1-i)*TernaryPlot.COS60, (1-i)*TernaryPlot.SIN60, 1-i, 0)); // VI axis dashed lines
			}
	}

	@Override
	public boolean isTopElement() {
		return false;
	}

	@Override
	public int ElementType() {
		return PlotElement.GRID_ELEMENT;
	}
}
