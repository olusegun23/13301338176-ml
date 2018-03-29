package com.example.haijunz.sensorml.models.maths;

public class RandomMock implements IRandom {

	@Override
	public double get(int x, int X) {
		assert x <= X : "X should be greater or equal to x";
		return ((double) x) / ((double) X);
	}

	@Override
	public double get() {
		return 1;
	}

}
