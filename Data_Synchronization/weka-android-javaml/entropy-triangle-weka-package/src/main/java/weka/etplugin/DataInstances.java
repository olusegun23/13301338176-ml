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

import java.util.ArrayList;
import java.util.List;

import weka.core.Attribute;
import weka.core.Instance;
import weka.core.Instances;
import weka.core.Utils;

/**
 * Class for handling a set of instances (of the type {@link DataInstance}) with information of evaluation of classification processes.
 * This class handles the update of the tooltips of the contained instances when the class attribute is changed.
 * 
 * @author Antonio Pastor
 * @see EntropyTrianglePanel
 */
public class DataInstances extends Instances {
	
	private static final long serialVersionUID = 6413844794789107504L;


	/**
	 * Constructor copying all instances and references to the header information from the given set of instances.
	 * 
	 * @param dataset the set to be copied
	 */
	public DataInstances(Instances dataset) {
		super(dataset);
	}

	/**
	 * Constructor creating an empty set of instances.
	 * Copies references to the header information from the given set of instances.
	 * Sets the capacity of the set of instances to 0 if its negative.
	 * 
	 * @param dataset the instances from which the header information is to be taken
	 * @param capacity the initial capacity of the new dataset
	 */
	public DataInstances(Instances dataset, int capacity) {
		super(dataset, capacity);
	}


	/**
	 * Creates an empty set of instances. Uses the given attribute information.
	 * Sets the capacity of the set of instances to 0 if its negative.
	 * Given attribute information must not be changed after this constructor has been used.
	 * 
	 * @param name the name of the relation
	 * @param attInfo the attribute information
	 */
	public DataInstances(String name, ArrayList<Attribute> attInfo) {
		super(name, attInfo, 1);
	}

	@Override
	public void setClass(Attribute att) {
		this.setClassIndex(att.index());
	}

	@Override
	public void setClassIndex(int classIndex) {
		super.setClassIndex(classIndex);
		this.updateInstances();
	}

	private void updateInstances() {
		for (Instance d: this.m_Instances) {
			((DataInstance) d).setToolTips();
		}
	}
	
	/**
	 * Adds one instance to the end of the set. Shallow copies instance before it
	 * is added. Increases the size of the dataset if it is not large enough. Does
	 * not check if the instance is compatible with the dataset. Note: String or
	 * relational values are not transferred.
	 * 
	 * @param instance the instance to be added
	 */
	@Override
	public boolean add(/* @non_null@ */Instance instance) {
		DataInstance newInstance;
		Instances inst_dataset = instance.dataset();
		if (inst_dataset!=null){
			double[] inst_values = new double[this.numAttributes()];
			for (int idx = 0; idx<inst_values.length; idx++){
				Attribute atr = this.attribute(idx);
				if (atr.isString()){
					inst_values[idx] = atr.addStringValue(inst_dataset.attribute(atr.name()), (int) instance.value(atr));
				} else {
					inst_values[idx] = instance.value(inst_dataset.attribute(atr.name()));
				}
			}
			newInstance = new DataInstance(this, 1.0, inst_values);
		} else {
			newInstance = new DataInstance(this, (Instance)instance.copy());
		}
		m_Instances.add(newInstance);
		return true;
	}
//	@Override
//	public boolean add(/* @non_null@ */Instance instance) {
//		DataInstance newInstance = new DataInstance(this, (Instance)instance.copy());
//		
//		m_Instances.add(newInstance);
//		return true;
//	}
	
	@Override
	public boolean remove(Object o) {
		boolean ret = super.remove(o);
		ArrayList<Attribute> atts = Utils.cast(m_Attributes.clone());
		for (Attribute att : atts) {
			if (att.isString()) {
				Attribute newAtt = new Attribute(att.name(), (List<String>) null, att.index());
				for (Instance inst : m_Instances) {
					inst.setValue(newAtt, inst.stringValue(att));
				}
				atts.set(att.index(), newAtt);
			}
		}
		this.m_Attributes = atts;
		return ret;
	}
}
