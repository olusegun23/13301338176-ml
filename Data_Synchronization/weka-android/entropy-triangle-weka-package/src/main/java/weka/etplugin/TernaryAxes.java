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
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.font.FontRenderContext;
import java.awt.font.GlyphVector;
import java.awt.geom.AffineTransform;
import java.awt.geom.Line2D;
import java.awt.geom.Path2D;
import java.awt.geom.Rectangle2D;
import javax.swing.border.EmptyBorder;

final class TernaryAxes extends PlotElement {

	/** Font for the ticks labels */
	public static Font FONT_TICK = new Font("Arial", Font.PLAIN, 6);
	
	/** Font for the axes labels */
	public static Font FONT_AXES = new Font("Arial", Font.PLAIN, 10);
	
	public static double FONT_SCALE_FACTOR = 1.0/200; // It must be between 0 and 1
	
	public static double TICK_SIZE = 0.02; // Axes tick size (Normalized to 1)
	
	Path2D axes = new Path2D.Double(Path2D.WIND_EVEN_ODD, 3);
	
	private String deltaH = new String("\u0394" + "H'");
	private String vI = new String("VI'");
	private String mI = new String("MI'");
	
	private double tick_dist = 0.1;
	private double tickLabels_dist = 0.1;
	
	private Font font_axes, font_axes60cw, font_axes60a;
	private Font font_tick, font_tick60cw, font_tick60a;
	
	public TernaryAxes (){
		this.setBorder(new EmptyBorder(0, 0, 0, 0));
		axes.moveTo(0, 0);
		axes.lineTo(1, 0);
		axes.lineTo(TernaryPlot.COS60, TernaryPlot.SIN60);
		axes.closePath();
		
		AffineTransform aff = AffineTransform.getScaleInstance(FONT_SCALE_FACTOR, -FONT_SCALE_FACTOR); // FONT_SCALE_FACTOR (0,1)
		
		font_axes = FONT_AXES.deriveFont(aff);
		font_tick = FONT_TICK.deriveFont(aff);
	}
	
	public TernaryAxes (double tick_dist, double tickLabels_dist){
		this();
		setTick_dist(tick_dist);
		setTickLabels_dist(tickLabels_dist);
	}
	
	/**
	 * @return the tick_dist
	 */
	protected double getTick_dist() {
		return tick_dist;
	}

	/**
	 * @return the tickLabels_dist
	 */
	protected double getTickLabels_dist() {
		return tickLabels_dist;
	}

	/**
	 * @param tick_dist the tick_dist to set
	 */
	protected void setTick_dist(double tick_dist) {
		if(tick_dist<=1 && tick_dist >= 0 && tick_dist%0.05==0)
			this.tick_dist = tick_dist;
	}

	/**
	 * @param tickLabels_dist the tickLabels_dist to set
	 */
	protected void setTickLabels_dist(double tickLabels_dist) {
		if(tickLabels_dist<=1 && tickLabels_dist >= 0 && tickLabels_dist%0.05==0)
			this.tickLabels_dist = tickLabels_dist;
	}

	protected void paintComponent(Graphics g){
		Graphics2D g2D =(Graphics2D) g;
		g2D.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
		g2D.setRenderingHint(RenderingHints.KEY_RENDERING, RenderingHints.VALUE_RENDER_QUALITY);
		g2D.setRenderingHint(RenderingHints.KEY_FRACTIONALMETRICS, RenderingHints.VALUE_FRACTIONALMETRICS_ON);
		
		
		FontRenderContext frc = g2D.getFontRenderContext();
		GlyphVector gv;
		Rectangle2D sw;
		double sw_x, sw_y;
		
		g2D.setColor(Color.BLACK);
		g2D.setStroke(TernaryPlot.BS_NORMAL);
		
		g2D.draw(axes);
		
//		draw tick lines
		if (tick_dist!=0){
			for (double i=tick_dist; i<0.99; i+=tick_dist) {
				g2D.draw(new Line2D.Double(i-TICK_SIZE*TernaryPlot.COS60, -TICK_SIZE*TernaryPlot.SIN60, i, 0)); // DeltaH axis tick lines
				g2D.draw(new Line2D.Double(1-i*TernaryPlot.COS60, i*TernaryPlot.SIN60, 1-i*TernaryPlot.COS60+TICK_SIZE, i*TernaryPlot.SIN60)); // MI axis tick lines
				g2D.draw(new Line2D.Double((1-i-TICK_SIZE)*TernaryPlot.COS60, (1-i+TICK_SIZE)*TernaryPlot.SIN60, (1-i)*TernaryPlot.COS60, (1-i)*TernaryPlot.SIN60)); // VI axis tick lines
			}
		}
		if(tickLabels_dist!=0){
			gv = font_tick.createGlyphVector(frc, "0.9");
			sw = gv.getVisualBounds();
			sw_x = sw.getWidth();
			sw_y = sw.getHeight();
			//			System.out.println(sw_x);

			// draw tick labels	
			for (double i=tickLabels_dist ; i<0.99; i+=tickLabels_dist) {
				// Rotated text for deltaH tick labels
				TernaryAxes.drawRotatedText(g2D ,font_tick.createGlyphVector(frc, String.format("%.1f",i)), (float) (i-(2*TICK_SIZE+sw_x)*TernaryPlot.COS60+sw_y*TernaryPlot.SIN60/2), (float) ((-2*TICK_SIZE-sw_x)*TernaryPlot.SIN60), 60);
				// Rotated graphics for VI tick labels
				TernaryAxes.drawRotatedText(g2D ,font_tick.createGlyphVector(frc, String.format("%.1f",i)), (float) ((1-i-2*TICK_SIZE-sw_x)*TernaryPlot.COS60-sw_y*TernaryPlot.SIN60/2), (float) ((1-i+2*TICK_SIZE+sw_x)*TernaryPlot.SIN60), -60);
				// Straight graphics for MI tick labels
				g2D.drawGlyphVector(font_tick.createGlyphVector(frc, String.format("%.1f",i)), (float) (1-i*TernaryPlot.COS60+2*TICK_SIZE), (float) (i*TernaryPlot.SIN60-sw_y/2));
			}
			// draw 1 tick labels
			
			// Rotated graphics for deltaH tick labels
			TernaryAxes.drawRotatedText(g2D ,font_tick.createGlyphVector(frc, String.format("%d",1)), (float) (1-(2*TICK_SIZE+sw_x)*TernaryPlot.COS60+sw_y*TernaryPlot.SIN60/2), (float) ((-2*TICK_SIZE-sw_x)*TernaryPlot.SIN60), 60);
			// Rotated graphics for VI tick labels
			TernaryAxes.drawRotatedText(g2D ,font_tick.createGlyphVector(frc, String.format("%d",1)), (float) ((-2*TICK_SIZE-sw_x)*TernaryPlot.COS60-sw_y*TernaryPlot.SIN60/2), (float) ((2*TICK_SIZE+sw_x)*TernaryPlot.SIN60), -60);
			// Straight font for MI tick labels
			g2D.drawGlyphVector(font_tick.createGlyphVector(frc, String.format("%d",1)), (float) (TernaryPlot.COS60+2*TICK_SIZE), (float) (TernaryPlot.SIN60-sw_y/2));
		}
		gv = font_axes.createGlyphVector(frc, deltaH);
		sw = gv.getVisualBounds();
		sw_x = sw.getWidth();
		sw_y = sw.getHeight();
		g2D.drawGlyphVector(gv, (float) (0.5*(1-sw_x)), (float) (-sw_y-7*TICK_SIZE));

		gv = font_axes.createGlyphVector(frc, vI);
		TernaryAxes.drawRotatedText(g2D, gv, 0.25-5*TICK_SIZE/TernaryPlot.COS60, (0.5+5*TICK_SIZE)*TernaryPlot.SIN60, 60);

		gv = font_axes.createGlyphVector(frc, mI);
		sw = gv.getVisualBounds();
		sw_x = sw.getWidth();
		sw_y = sw.getHeight();
		TernaryAxes.drawRotatedText(g2D, gv, 0.75+5*TICK_SIZE/TernaryPlot.COS60-sw_y*TernaryPlot.SIN60, (sw_x+0.5+5*TICK_SIZE)*TernaryPlot.SIN60, -60);
	}

	@Override
	public boolean isTopElement() {
		return false;
	}

	@Override
	public int ElementType() {
		return PlotElement.AXES_ELEMENT;
	}
	
	private static void drawRotatedText(Graphics g, GlyphVector gv, double x, double y, int rot){
		final Graphics2D g2 = (Graphics2D) g.create();
		g2.rotate(Math.toRadians(rot),x, y);
		g2.drawGlyphVector(gv, (float) x, (float) y);
		g2.dispose();
	}
	
}
