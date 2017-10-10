// Artificial Intelligence for Humans
// Volume 3: Deep Learning and Neural Networks
// C# Version
// http://www.aifh.org
// http://www.jeffheaton.com
//
// Code repository:
// https://github.com/jeffheaton/aifh
//
// Copyright 2015 by Jeff Heaton
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// For more information on Heaton Research copyrights, licenses
// and trademarks visit:
// http://www.heatonresearch.com/copyright
//

namespace AIFH_Vol3.Core.Learning
{
    /// <summary>
    ///     A classification algorithm is used to classify input data into classes.
    /// </summary>
    public interface IClassificationAlgorithm : IMLMethod
    {
        /// <summary>
        ///     Classify the specified input into a group.
        /// </summary>
        /// <param name="input">The input data.</param>
        /// <returns>The group the data was classified into.</returns>
        int ComputeClassification(double[] input);
    }
}