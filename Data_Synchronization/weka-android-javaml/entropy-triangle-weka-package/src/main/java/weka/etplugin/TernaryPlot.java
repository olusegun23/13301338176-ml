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

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.EventQueue;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.geom.AffineTransform;
import java.awt.geom.Point2D;
import java.util.Collections;
import java.util.LinkedList;
import java.util.List;

import javax.swing.JFrame;
import javax.swing.JPanel;



/**
 * This class plots the ternary plot for the entropy triangle.
 * To transform the Java coordinates to the plot coordinates it uses an {@link AffineTransform}.
 * The axes normalized to the range (0,1) and the children objects are plotted in the order specified by the 
 * {@link PlotElement} interface.
 * <br><br>
 * Children objects that do not implement the PlotElement interface are drawn at the bottom.
 * 
 * @author Antonio Pastor
 * @see AffineTransform
 * @see EntropyTrianglePanel
 *
 */
public class TernaryPlot extends JPanel {
	
	private static final long serialVersionUID = -3766376900858366982L;
	
	/** Sine of 60 degrees */
	public static double SIN60 = Math.sqrt(3)/2;
	
	/** Cosine of 60 degrees */
	public static double COS60 = 0.5;

	/** Minimum size of the panel for plot resizing in pixels */
	public static int MINPLOTWIDTH = 300;
	
	/** Default size of the this panel in pixels */
	public static int DEFAULTPLOTWIDTH =400;
	
	/** Distance betweem Axes grid */
	public static double GRID = 0.2;
	
	/** The normalized stroke for normal drawing in the plot */
	public static final BasicStroke BS_NORMAL= new BasicStroke(0.005f, BasicStroke.CAP_ROUND,
			BasicStroke.JOIN_MITER);
	
	/** The normalized dashed stroke for plot */
	public static final BasicStroke BS_DASHED = new BasicStroke(0.002f, BasicStroke.CAP_ROUND,
			BasicStroke.JOIN_MITER, 1.0f,new float[] {1f, 1f}, 2f);
	
	/** The normalized stroke for drawing borders in the plot */
	public static final BasicStroke BS_BORDER = new BasicStroke(0.002f, BasicStroke.CAP_BUTT,
			BasicStroke.JOIN_MITER);
	
	private AffineTransform aff_plot, aff_default;
	
	private TernaryAxes axes;
	private TernaryGrid grid;
	
	private int scale;
	
	
	private ComponentAdapter resizeListener = new ComponentAdapter (){
		@Override
		public void componentResized(ComponentEvent e){
			Component[] comps = getComponents();
			for (Component c: comps){
				if (c instanceof PlotElement)
					c.setBounds(getBounds());
			}
		}
		@Override
		public void componentMoved(ComponentEvent e){
			Component[] comps = getComponents();
			for (Component c: comps){
				if (c instanceof PlotElement)
					c.setBounds(getBounds());
			}
		}
		
	};

	/** Default constructor */
	public TernaryPlot() {
		this.setPreferredSize(new Dimension(DEFAULTPLOTWIDTH, DEFAULTPLOTWIDTH));
		this.setLayout(null);
		this.setBackground(Color.WHITE);
		this.addComponentListener(resizeListener);
		
		axes = new TernaryAxes();
		grid = new TernaryGrid();
		add(grid);
		add(axes);
	}
	
	@Override
	public void paint(Graphics g) {
		super.paintComponent(g);
		Graphics2D g2D = (Graphics2D) g;
		g2D.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
		g2D.setRenderingHint(RenderingHints.KEY_RENDERING, RenderingHints.VALUE_RENDER_QUALITY);
		g2D.setRenderingHint(RenderingHints.KEY_FRACTIONALMETRICS, RenderingHints.VALUE_FRACTIONALMETRICS_ON);
		
		scale = (int) Math.min(getWidth()/1.2, getHeight()/1.2);
		if (scale<TernaryPlot.MINPLOTWIDTH)
			scale = TernaryPlot.MINPLOTWIDTH;
		// Scale the panel to plot from 0 to 1
		aff_plot = AffineTransform.getScaleInstance(scale, scale);
		// translation to have some margin on origin of axes
		aff_plot.concatenate(new AffineTransform(1, 0, 0, -1, (getWidth()*0.5/scale)-0.5, 0.95));
		aff_default = g2D.getTransform();
		
		AffineTransform aff_g2D = (AffineTransform) aff_default.clone();
		aff_g2D.concatenate(aff_plot);
		g2D.setTransform(aff_g2D);
//		System.out.println(g2D.getTransform());
		
		List<PlotElement> list = new LinkedList<PlotElement>();
		// Paint first (beyond plot elements) elements that are not part of the graph
		for(Component c: getComponents()){
			if(c instanceof PlotElement){
				list.add((PlotElement) c);
			}
			else{
				c.paint(g2D);
			}
		}
		Collections.sort(list); // Plot Elements are Comparable, a point > axes > grid
		for (PlotElement pl: list){
			pl.paint(g2D);
		}
		g2D.setTransform(aff_default);
		super.paintBorder(g);
	}

	
	/**
	 * Gets the current AffineTransform of the Panel.
	 * @see AffineTransform
	 * @return The current AffineTransform of the Panel
	 */
	public AffineTransform getPlotTransform() {
		return aff_plot;
	}
	
	@Override
	public Component add(Component comp) {
		Component added;
		if (comp instanceof PlotElement){
			comp.setBounds(getBounds());
		}
			added = super.add(comp);
			this.repaint();
			return added;
	}
	
	@Override
	public void remove(Component comp) {
		super.remove(comp);
		this.repaint();
	}
	
	/**
	 * Removes all DataPoint and Baseline of the plot.
	 * @see DataPoint Baseline
	 */
	public void removeAllData () {
		for(Component c: getComponents()) {
			if(c instanceof DataPoint ||
					c instanceof Baseline) {
				remove(c);
			}
		}
	}
	
	/** Main method for testing this class */
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			@Override
			public void run() {
				JFrame window = new JFrame ("Test program");
				TernaryPlot plot = new TernaryPlot();
				Baseline bs = new Baseline(0.001, "test BSLine");
				DataPoint dp = new DataPoint(new Point2D.Double(0.5, 0.5), Color.BLUE);
				window.add(plot);
				window.setSize(400,400);
				window.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
				window.setVisible(true);
				plot.add(bs);
				plot.add(dp);
				
			}
		});
	}

	
}
