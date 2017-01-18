== guide for installation of sedml (Mac OS X) ==

## compile sedml from https://github.com/fbergmann/libSEDML

important files:
libsedmlj.jnilib: default location is /usr/local/lib
libsedmlj.jar: default location is /usr/local/share/java

## create a java loader class

To correctly load the native libraries in matlab we need to create a wrapper class. For this create a file loadsedml.java with the following contents:

```
public class loadsedml {
static {
java.lang.System.loadLibrary("sedmlj");
}
}
```

compile this as loadsedml.class file by calling `javac -target 1.5 -source 1.5 loadsedml.java`

For me trying to load the java native library using  java.lang.System.loadLibrary or  java.lang.System.load directly from matlab did not work. Explicitely configuring target and source java version ensures that the class is loadable in the matlab JRE which can easily be older than the system JRE.

## configure matlab java paths

### java class path

edit $matlabroot/classpath.txt and add the absolute paths to libsedmlj.jar (e.g. /usr/local/share/java/libsedmlj.jar) and the compiled loadsedml.class

### java library path

edit $matlabroot/classpath.txt and add the folder that contains libsedmlj.jnilib (e.g. /usr/local/lib)

in order for the changes to take effect you need to restart matlab now

## load sedmlj.jnilib in matlab

call `loadsedml;`

after that the class org.sedml.libsedml.libsedml should be available






