package com.example.haijunz.sensorml.models.maths;

public class Random implements IRandom {

	@Override
	public double get(int x, int X) {
		return get();
	}

	@Override
	public double get() {
		return Math.random();
	}

}
