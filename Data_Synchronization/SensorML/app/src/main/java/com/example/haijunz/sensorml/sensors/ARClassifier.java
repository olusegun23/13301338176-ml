package com.example.haijunz.sensorml.sensors;

import android.util.Log;

/**
 * Created by haijunz on 18-3-27.
 */
public class ARClassifier {

    /*
     * Compute Peek to Peek for an array of data
     */
    public static double computePeakToPeak(Double[] smoothedData) {
        double peakToPeakVal = 0.0;
        double rmsVal = 0.0;
        double sumSqVal = 0.0;

        for (Double item : smoothedData) {
            double itemSqVal = Math.pow(item.doubleValue(), 2);
            sumSqVal += itemSqVal;
        }

        rmsVal = Math.sqrt(sumSqVal / smoothedData.length);
        peakToPeakVal = 2.8 * rmsVal;

        return peakToPeakVal;
    }

    /*
     * Compute Standard Deviation for an array of data
     *
     * @param smoothedData Data after linearization and 5 point smoothening
     * algorithm
     *
     * @param avgVal Average of the data
     *
     * @return double Standard Deviation of the data
     */
    public static double computeDiffSq(Double[] smoothedData, double avgVal) {
        double stddev = 0.0;
        double sumVal = 0.0;

        for (Double item : smoothedData) {
            double sqVal = Math.pow((item.doubleValue() - avgVal), 2);
            sumVal += sqVal;
        }

        stddev = Math.sqrt(sumVal / smoothedData.length);

        return stddev;
    }

    /*
     * Compute Average for an array of data
     *
     * @param smoothedData Data after data linearization and 5-point smootehning
     * algorithm
     *
     * @return double Average of the data
     */
    public static double computeAvg(Double[] smoothedData) {
        double sum = 0.0;
        double avg = 0.0;

        for (Double data : smoothedData) {
            sum += data.doubleValue();
        }

        avg = sum / smoothedData.length;
        return avg;
    }

    /*
     * kNN Classification Algorithm implementation 1NN Classification and no
     * voting
     *
     * @param name Name of the activity to compare the distance with
     *
     * @param avg Average of the data
     *
     * @param stdDev Standard Deviation of the data
     *
     * @param peekToPeek Peek To Peek value of the data
     *
     * @return double 1NN local distance between the default values of the
     * activity and the provided values.
     */
    private static double kNNClassifyDistance(
            ARFeatures.Name name, double avg, double stdDev,
            double peekToPeek) {

        double avgDist, stdDevDist, peekToPeekDist, globalDist;
        double avgAct, stdDevAct, peekToPeekAct;
        double maxAvg, minAvg, maxStdDev, minStdDev, maxPeekToPeek, minPeekToPeek;

        maxAvg = ARFeatures.maxAvg;
        minAvg = ARFeatures.minAvg;
        maxPeekToPeek = ARFeatures.maxPeekToPeek;
        minPeekToPeek = ARFeatures.minPeekToPeek;
        maxStdDev =  ARFeatures.maxStdDev;
        minStdDev = ARFeatures.minStdDev;

        if (name == ARFeatures.Name.ACTIVITY_RUN) {
            avgAct = ARFeatures.runAvg;
            stdDevAct = ARFeatures.runStdDev;
            peekToPeekAct = ARFeatures.runPeekToPeek;
        } else if (name == ARFeatures.Name.ACTIVITY_WALK) {
            avgAct = ARFeatures.walkAvg;
            stdDevAct = ARFeatures.walkStdDev;
            peekToPeekAct = ARFeatures.walkPeekToPeek;
        } else {
            avgAct = ARFeatures.standAvg;
            stdDevAct = ARFeatures.standStdDev;
            peekToPeekAct = ARFeatures.walkPeekToPeek;
        }

        // Local Distance of Avg
        avgDist = Math.abs((avg - avgAct) / (maxAvg - minAvg));

        // Local Distance of StdDev
        stdDevDist = Math.abs((stdDev - stdDevAct) / (maxStdDev - minStdDev));
        //stdDevDist = 0;

        // Local Distance of PeekToPeek
        peekToPeekDist = Math.abs((peekToPeek - peekToPeekAct) / (maxPeekToPeek - minPeekToPeek));

        System.out.println ("TweetSensor" + name + "avgDist" + avgDist + "stdDevDist" + stdDevDist+ "peekToPeekDist" + peekToPeekDist);
        globalDist = ARFeatures.weightAvg * avgDist
                + ARFeatures.weightStdDev * stdDevDist
                + ARFeatures.weightPeekToPeek * peekToPeekDist;

        return globalDist;
    }

    /*
     * Voting:
     *
     * We don't need voting for 1NN Just compare
     *
     * @param standDist Local distance of the stand activity
     *
     * @param walkDist Local distance of the walk activity
     *
     * @param runDist Local distance of the run activity
     */
    private static String kNNClassifyVoting(double standDist, double walkDist,
                                            double runDist) {
        if (standDist < walkDist && standDist < runDist) {
            return ARFeatures
                    .getName(ARFeatures.Name.ACTIVITY_STAND);
        } else if (runDist < walkDist) {
            return ARFeatures
                    .getName(ARFeatures.Name.ACTIVITY_RUN);
        } else {
            return ARFeatures
                    .getName(ARFeatures.Name.ACTIVITY_WALK);
        }

    }

    /*
     * Public method for kNN Classification and Voting Algorithm
     *
     * @smoothedData Data to be classified after linerization and 5-point
     * smoothening algorithm
     *
     * @return String The activity classified
     */
    public static String kNNClassifyActivity(Double[] smoothedData) {
        double avg, stdDev, peekToPeek;
        double distRun, distWalk, distStand;
        String act;

        avg = ARClassifier.computeAvg(smoothedData);

        stdDev = ARClassifier.computeDiffSq(smoothedData, avg);

        peekToPeek = ARClassifier.computePeakToPeak(smoothedData);

        Log.d(ARService.tag, ARService.tag
                + "_Activity" + "avg: " + avg + ", stdDev: " + stdDev
                + ", peek: " + peekToPeek);

        // local distance of run
        distRun = ARClassifier.kNNClassifyDistance(
                ARFeatures.Name.ACTIVITY_RUN, avg, stdDev,
                peekToPeek);

        // local distance of walk
        distWalk = ARClassifier.kNNClassifyDistance(
                ARFeatures.Name.ACTIVITY_WALK, avg, stdDev,
                peekToPeek);

        // local distance of stand
        distStand = ARClassifier.kNNClassifyDistance(
                ARFeatures.Name.ACTIVITY_STAND, avg, stdDev,
                peekToPeek);

        System.out.println ("TweetSensor" + "walk:" + distWalk + "stand:" + distStand + "run:" + distRun);
        // Compute global distance and classify
        String text = "avg: " + avg + ", stdDev: " + stdDev + ", peek: "
                + peekToPeek;
        //DataHandler.appendLog(text, "acc");
        act = ARClassifier.kNNClassifyVoting(distStand, distWalk,
                distRun);

        return act;
    }

}
