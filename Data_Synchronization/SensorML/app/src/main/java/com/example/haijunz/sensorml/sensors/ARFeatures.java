package com.example.haijunz.sensorml.sensors;

/**
 * Created by haijunz on 18-3-27.
 */


public class ARFeatures {
    public enum Name {
        ACTIVITY_STAND, ACTIVITY_WALK, ACTIVITY_RUN
    };

    public static double maxStdDev = 0.0;
    public static double minStdDev = 7.0;
    public static double minAvg = 8.0;
    public static double maxAvg = 20.0;
    public static double minPeekToPeek = 20.0;
    public static double maxPeekToPeek = 55.0;

    // Walk Standard Deviation values
    public static double walkStdDev = 4.814436111629097;

    // Walk Peek to Peek values
    public static double walkPeekToPeek = 35.28362518120852;

    // Walk Average values
    public static double walkAvg = 11.895184752921534;

    // Run Standard Deviation Values
    public static double runStdDev = 6.04963780127633523;

    // Run Peek to Peek Values
    public static double runPeekToPeek = 43.6701736221696024;

    // Run Average Values
    public static double runAvg = 14.374379100371538;

    // Stand Standard Deviation
    public static double standStdDev = 1.0472167012925262;

    // Stand Peek to Peek
    public static double standPeekToPeek = 25.1736325226226;

    // Stand Average
    public static double standAvg = 9.07368031875503;


    //Weights
    public static double weightStdDev = 1.0;
    public static double weightPeekToPeek = 1.0;
    public static double weightAvg = 1.0;

    /*
     * Return the String description of the activity Name
     *
     * @param Name Enumeration value of Activity
     *
     * @return String Returns the String description of the Activity
     */
    public static String getName(Name a) {

        if (a == Name.ACTIVITY_RUN) {
            return "Running";
        } else if (a == Name.ACTIVITY_STAND) {
            return "Standing";
        } else {
            return "Walking";
        }
    }
}
