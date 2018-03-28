package com.example.ravikiran357.mc_project;

import android.util.Log;

import java.io.FileWriter;
import java.io.PrintWriter;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;

import weka.classifiers.Classifier;
import weka.classifiers.Evaluation;
import weka.core.Instances;
import weka.core.converters.ConverterUtils.DataSource;

class MLTestDriver {
    public boolean test(int percent, String classifierName, String inputPath) {
		try {
            long startTime = System.currentTimeMillis();
            DataSource source = new DataSource(inputPath);
            Instances data = source.getDataSet();
            data.setClassIndex(data.numAttributes() - 1);
            int trainSize = Math.round(data.numInstances() * percent / 100);
            int testSize = data.numInstances() - trainSize;
            Instances trainingData = new Instances(data, 0, trainSize);
            Instances testingData = new Instances(data, trainSize, testSize);
            Classifier classifier = (Classifier) weka.core.SerializationHelper.
                    read(MainActivity.FILE_PATH + classifierName + ".model");
            Evaluation evaluateTrainingData = new Evaluation(testingData);
            evaluateTrainingData.evaluateModel(classifier, testingData);
            long totalTime = System.currentTimeMillis() - startTime;
            PrintWriter writerRes = new PrintWriter(new FileWriter(MainActivity.FILE_PATH +
                    "results.txt")); // result file

            // Populating data for result and logs
            float hter = (float) ((evaluateTrainingData.falsePositiveRate(1) +
                    evaluateTrainingData.falseNegativeRate(1))*100 / 2.0);
            String result = String.format(Locale.ENGLISH,"TAR:%.2f | TRR:%.2f | \n FAR:%.2f |" +
                    " FRR:%.2f | \n HTER:%.2f | \n Execution time:%d ms \n",evaluateTrainingData.
                    truePositiveRate(0)*100, evaluateTrainingData.
                    trueNegativeRate(1)*100, evaluateTrainingData.
                    falsePositiveRate(1)*100, evaluateTrainingData.
                    falseNegativeRate(0)*100, hter, totalTime);
            writerRes.print(result + " \n " + evaluateTrainingData.toSummaryString(false));
            PrintWriter writer = new PrintWriter(new FileWriter(MainActivity.LOG_FILE_PATH, true)); // log file
            writer.println(evaluateTrainingData.toClassDetailsString(" \n\n\n ******* RUN: " +
                    new SimpleDateFormat("yyyy/MM/dd-HH:mm:ss").format(new Date()) + " - " +
                    classifierName + " - " + percent + "% *******"));
            writer.printf(result);
            writer.print(evaluateTrainingData.toSummaryString(true));
            writer.close();
            writerRes.close();
        } catch (Exception e) {
		    Log.d("MLTestDriver", e.getMessage());
		    return false;
        }
        return true;
	}
}
