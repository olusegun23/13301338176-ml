// Artificial Intelligence for Humans
// Volume 2: Nature-Inspired Algorithms
// C# Version
// http://www.aifh.org
// http://www.jeffheaton.com
//
// Code repository:
// https://github.com/jeffheaton/aifh
//
// Copyright 2014 by Jeff Heaton
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
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AIFH_Vol2.Core.Randomize
{
    /// <summary>
    /// A factory to create random number generators of a specific type.
    /// </summary>
    public interface IRandomFactory
    {
        /// <summary>
        /// Factor a random number generator.
        /// </summary>
        /// <returns>The random number generator.</returns>
        IGenerateRandom Factor();

        /// <summary>
        /// Factor a factory, just like this one.
        /// </summary>
        /// <returns></returns>
        IRandomFactory FactorFactory();
    }
}
