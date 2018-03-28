import java.io.File;
import java.util.Arrays;

import edu.wisc.jj.KNN;


public class Example {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		KNN mKNN = new KNN();
		String[] category=new String[]{"d","e"};
		double[][] features = new double[2][];
		features[0] = new double[]{-11.3, 14.5};
		features[1] = new double[]{-30.3, 40.5};
		
		//add multiple training samples at once
		mKNN.addTrainingItems(category, features);
		System.out.println(mKNN.toString());
		
		//classify multiple test data
		double[][] testData=new double[2][];
		testData[0] = new double[]{-1.0,2.0};
		testData[1] = new double[]{-35.0,42.0};		
		String[] result=mKNN.classifyMultiple(testData, 1);
		System.out.println(Arrays.toString(result));
		
		//add one single training sample
		String oneCat="f";
		double[] oneFeatureVector=new double[]{0,0};
		mKNN.addTrainingItem(oneCat,oneFeatureVector);
		System.out.println(mKNN.toString());
		//redo classify
		System.out.println("reclassify: ");
		result=mKNN.classifyMultiple(testData, 1);
		System.out.println(Arrays.toString(result));
		
		//save and load 
		mKNN.load(new File("./mtest/knnData.txt"));
		System.out.println("after load more data: ");
		System.out.println(mKNN.toString());		
		mKNN.save(new File("./mtest/knnData.txt"));
	}

}
