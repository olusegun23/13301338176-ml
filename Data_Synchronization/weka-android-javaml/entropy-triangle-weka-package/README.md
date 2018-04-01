# entropy-triangle-weka-package
A set of information-theoretic tools for the assessment of classifiers in Weka.

http://apastor.github.io/entropy-triangle-weka-package/

This package is an implementation of the work of these papers:

* [Valverde-Albacete, F. J., & Peláez-Moreno, C. (2014).
 100% classification accuracy considered harmful:
 The normalized information transfer factor explains the accuracy paradox.
 PLoS ONE 9(1).]
 (http://dx.doi.org/10.1371/journal.pone.0084217)

* [Valverde-Albacete, F. J., & Peláez-Moreno, C. (2010).
 Two information-theoretic tools to assess the performance of multi-class classifiers.
 Pattern Recognition Letters, Volume 31, Issue 12, 1 September 2010, Pages 1665-1671.]
 (http://dx.doi.org/10.1016/j.patrec.2010.05.017)

### Requirements
- Java >=1.7
- [Weka](http://www.cs.waikato.ac.nz/~ml/weka/) >=3.7.8

## INSTALL
The zip package is cross-platform, provided you have Java 1.7 or later.
To install the plugin on Weka you can use either the Weka Package Manager GUI or the command line.

### GUI
1. In the Weka GUI Chooser, the main Weka window that opens on the program startup, go to the **Tools** menu and open the **Package manager**
2. On the top-right corner of the Package manager window click the **File/URL** button.
3. Click the **Browse...** button to select your already downloaded EntropyTriangle.zip file or paste the file URL from the *release* section.
4. Restart Weka before using the plugin.

### Command-line
```bash
# Add weka.jar to classpath
$ export CLASSPATH=${CLASSPATH}:<path-to>/weka.jar

$ java weka.core.WekaPackageManager -install-package EntropyTriangle.zip
```

## BUILD
To build the package from source use [Apache Ant](http://ant.apache.org/) from the project root directory.
It is necessary to specify the project build file (build_package.xml) and the weka.jar as library.
Optionally you can set the build command, make_package is the default one.

```bash
$ ant -f ./build_package.xml -lib <path-to>/weka.jar make_package
```
