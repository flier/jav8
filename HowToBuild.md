## Prebuild Packages ##

If you want to try Jav8 ASAP, we suggest you use the prebuild packages for your platform.

Please [downloads](http://code.google.com/p/jav8/downloads/list) the package for the Windows, Linux/Debian or Android system.

## Prepare ##

Before build the Jav8, we should ensure the tool chain and 3rd party library is ready.

### Google V8 ###

Follow the following document to download and build v8 engine as static library

> [How to Download and Build V8](http://code.google.com/apis/v8/build.html)

set environment variable V8\_HOME to the v8 root folder

```
export V8_HOME=<v8_path> // Linux/Unix
```

or

```
set V8_HOME=<v8_path> // Windows
```

If you want to build v8 with GCC 4.x at x64 platform, you should compile v8 with PIC (Position-Independent Code) mode, and set the arch to x64 for scons.
```
export CCFLAGS=-fPIC
scons arch=x64
```

### Apache Ant ###

Jav8 use the Apache Ant as the build system, you should download [the binary distribution](http://ant.apache.org/bindownload.cgi) and [install it](http://ant.apache.org/manual/install.html#installing).

Please ensure your ant could be work.
```
$ ant -v
Apache Ant(TM) version 1.8.2 compiled on December 20 2010
Trying the default build file: build.xml
Buildfile: build.xml does not exist!
Build failed
```

### JDK ###
Please ensure you have installed the JDK before build the Jav8. You could [download it](http://www.oracle.com/technetwork/java/javase/downloads/index.html) or install it with the package manager.
```
#urmpi java-1.6.0-sun
```

## Build ##

Change the current directory to the Jav8 folder, use Ant to build the library
```
$ ant
Buildfile: /home/flier/jav8/build.xml

prepare:

compile:

jni:
     [echo] Generating JNI header ...
     [echo] Building JNI library ...
       [cc] 3 total files to be compiled.
       [cc] Starting link
     [copy] Copying 1 file to /home/flier/jav8/build/classes

jar:
      [jar] Building jar: /home/flier/jav8/dist/jav8-jsr223-linux-i386-0.3.jar

test:
    [mkdir] Created dir: /home/flier/jav8/build/test-classes
    [mkdir] Created dir: /home/flier/jav8/build/test-report
    [javac] Compiling 1 source file to /home/flier/jav8/build/test-classes
    [junit] Running lu.flier.script.V8ScriptEngineTest
    [junit] Testsuite: lu.flier.script.V8ScriptEngineTest
    [junit] Tests run: 11, Failures: 0, Errors: 0, Time elapsed: 1.056 sec
    [junit] Tests run: 11, Failures: 0, Errors: 0, Time elapsed: 1.056 sec
    [junit] ------------- Standard Output ---------------
    [junit] Hello, world!ScriptEngineFactory Info
    ...
```