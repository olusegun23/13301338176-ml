package com.example.haijunz.sensorml.Env;

/**
 * Created by haijunz on 18-3-27.
 */


public class EnvClassifier {

    /**
     *
     * Calculates the distance of the Different Activities with the acquired
     * values.
     *
     * @param name
     * @param zcrDiff
     * @param spectralRollOff
     * @param spectralFlux
     * @param spectralCentroid
     * @param bandwidth
     * @return
     */
    private static double kNNClassifyDistance(EnvFeatures.Name name,
                                              double zcrDiff, double lowEnergy, double spectralRollOff,
                                              double spectralFlux, double spectralCentroid, double bandwidth) {

        double defZcrDiff, defSpectralRollOff, defSpectralFlux, defSpectralCentroid, defBandwidth, defLowEnergyFrameRate;
        double zcrDist = 0.0, rollOffDist = 0.0, fluxDist = 0.0, centroidDist = 0.0, bandwidthDist = 0.0, lowEnergyFrameRateDist = 0.0;
        double globalDist;

        if (name == EnvFeatures.Name.ENVIRONMENT_SPEECH) {
            defZcrDiff = EnvFeatures.speechZCRDiff;
            defSpectralRollOff = EnvFeatures.speechSpectralRollOff;
            defSpectralFlux = EnvFeatures.speechSpectralFlux;
            defSpectralCentroid = EnvFeatures.speechSpectralCentroid;
            defBandwidth = EnvFeatures.speechBandwidth;
            defLowEnergyFrameRate = EnvFeatures.speechLowEnergyFrameRate;
        } else if (name == EnvFeatures.Name.ENVIRONMENT_MUSIC) {
            defZcrDiff = EnvFeatures.musicZCRDiff;
            defSpectralRollOff = EnvFeatures.musicSpectralRollOff;
            defSpectralFlux = EnvFeatures.musicSpectralFlux;
            defSpectralCentroid = EnvFeatures.musicSpectralCentroid;
            defBandwidth = EnvFeatures.musicBandwidth;
            defLowEnergyFrameRate = EnvFeatures.musicLowEnergyFrameRate;
        } else {
            defZcrDiff = EnvFeatures.ambientNoiseZCRDiff;
            defSpectralRollOff = EnvFeatures.ambientNoiseSpectralRollOff;
            defSpectralFlux = EnvFeatures.ambientNoiseSpectralFlux;
            defSpectralCentroid = EnvFeatures.ambientNoiseSpectralCentroid;
            defBandwidth = EnvFeatures.ambientNoiseBandwidth;
            defLowEnergyFrameRate = EnvFeatures.ambientNoiseLowEnergyFrameRate;
        }

        zcrDist = Math.abs(defZcrDiff - zcrDiff) / 16.0 - 0.0;
        lowEnergyFrameRateDist = Math.abs(defLowEnergyFrameRate - lowEnergy) / 20.0 - 0.0;
        // fluxDist = Math.abs(defSpectralFlux - spectralFlux) / 0.02 - 0.0;
        // rollOffDist = Math.abs(defSpectralRollOff - spectralRollOff) / 0.5 -
        // 0.0;
        // centroidDist = Math.abs(defSpectralCentroid - spectralCentroid);
        // bandwidthDist = Math.abs(defBandwidth - bandwidth);

        globalDist = zcrDist + lowEnergyFrameRateDist + rollOffDist + fluxDist
                + centroidDist + bandwidthDist;

        return globalDist;
    }

    /**
     *
     * @param musicDist
     *            global Distance of music
     * @param ambientNoiseDiff
     *            global Distance of ambient noise
     * @param speechDiff
     *            global Distance of speech
     * @return
     */
    private static String kNNClassifyVoting(double ambientNoiseDiff,
                                            double speechDiff, double musicDist) {

        if (musicDist < ambientNoiseDiff && musicDist < speechDiff) {
            return EnvFeatures
                    .getName(EnvFeatures.Name.ENVIRONMENT_MUSIC);
        } else if (speechDiff < ambientNoiseDiff) {
            return EnvFeatures
                    .getName(EnvFeatures.Name.ENVIRONMENT_SPEECH);
        } else {
            return EnvFeatures
                    .getName(EnvFeatures.Name.ENVIRONMENT_AMBIENT_NOISE);
        }

    }

    /**
     *
     * @param zcrDiff
     *            local distance of feature ZCR
     * @param spectralRollOff
     *            local distance of feature spectralRollOff
     * @param spectralFlux
     *            local distance of spectral flux
     * @param spectralCentroid
     *            local distance of spectral centroid.
     * @param bandwidth
     *            local distance of bandwidth
     * @return
     */
    public static String kNNClassifyActivity(double zcrDiff, double lowEnergy,
                                             double spectralRollOff, double spectralFlux,
                                             double spectralCentroid, double bandwidth) {
        double musicDist, speechDist, ambientNoiseDist;

        musicDist = kNNClassifyDistance(
                EnvFeatures.Name.ENVIRONMENT_MUSIC, zcrDiff,
                lowEnergy, spectralRollOff, spectralFlux, spectralCentroid,
                bandwidth);

        speechDist = kNNClassifyDistance(
                EnvFeatures.Name.ENVIRONMENT_SPEECH, zcrDiff,
                lowEnergy, spectralRollOff, spectralFlux, spectralCentroid,
                bandwidth);

        ambientNoiseDist = kNNClassifyDistance(
                EnvFeatures.Name.ENVIRONMENT_AMBIENT_NOISE, zcrDiff,
                lowEnergy, spectralRollOff, spectralFlux, spectralCentroid,
                bandwidth);

        System.out.println("Classifier dist:" + musicDist + "," + speechDist
                + "," + ambientNoiseDist);

        return kNNClassifyVoting(ambientNoiseDist, speechDist, musicDist);
    }
}
