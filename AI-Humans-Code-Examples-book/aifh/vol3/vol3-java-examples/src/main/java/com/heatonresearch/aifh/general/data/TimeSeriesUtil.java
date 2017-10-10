/*
 * Artificial Intelligence for Humans
 * Volume 3: Deep Learning and Neural Networks
 * Java Version
 * http://www.aifh.org
 * http://www.jeffheaton.com
 *
 * Code repository:
 * https://github.com/jeffheaton/aifh
 *
 * Copyright 2014-2015 by Jeff Heaton
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * For more information on Heaton Research copyrights, licenses
 * and trademarks visit:
 * http://www.heatonresearch.com/copyright
 */
package com.heatonresearch.aifh.general.data;

import java.util.ArrayList;
import java.util.List;

/**
 * Utility for time series.
 */
public class TimeSeriesUtil {
    /**
     * Encode sliding window.
     * @param dataset The dataset.
     * @param inputWindow The size of the input window.
     * @param predictedWindow The size of the prediction window.
     * @param inputColumns The number of input columns.
     * @param predictedColumns The number of predicted columns.
     * @return The dataset.
     */
    public static List<BasicData> slidingWindow(
            double[][] dataset,
            int inputWindow,
            int predictedWindow,
            int[] inputColumns,
            int[] predictedColumns ) {

        List<BasicData> result = new ArrayList<>();

        int totalWindowSize = inputWindow+predictedWindow;

        int datasetIndex = 0;

        while((dataset.length - datasetIndex)>=totalWindowSize) {
            BasicData item = new BasicData(
                    inputWindow * inputColumns.length,
                    predictedWindow * predictedColumns.length);

            // input columns
            int inputIdx = 0;
            for (int i = 0; i < inputWindow; i++) {
                for (int j = 0; j < inputColumns.length; j++) {
                    item.getInput()[inputIdx++] = dataset[datasetIndex + i][inputColumns[j]];
                }
            }
            // predicted columns
            int predictIdx = 0;
            for (int i = 0; i < predictedWindow; i++) {
                for (int j = 0; j < predictedColumns.length; j++) {
                    item.getIdeal()[predictIdx++] = dataset[datasetIndex + inputWindow + i][predictedColumns[j]];
                }
            }

            datasetIndex++;
            // add the data item
            result.add(item);
        }

        return result;
    }
}
