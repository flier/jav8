An implementation of the Java Scripting API (JSR223) base on the Google V8 Javascript engine.

Java Specification Request (JSR) 223 helps developers integrate Java technology and scripting languages by defining a standard framework and application programming interface (API) to do the following:

* Access and control Java technology-based objects from a scripting environment
* Create web content with scripting languages
* Embed scripting environments within Java technology-based applications

For more detail, please read the article [Scripting for the Java Platform](http://java.sun.com/developer/technicalArticles/J2SE/Desktop/scripting/)

To use the Jav8 Scripting Engine, place the file jav8-jsr223-xxx.jar to your classpath. Then you could get an instance of the engine this way:
```java
ScriptEngineManager factory = new ScriptEngineManager();
ScriptEngine engine = factory.getEngineByName("jav8");
```
And you execute the script with the ScriptEngine?.eval method
```java
  try {
    engine.eval("print('Hello, world!')");
  } catch (ScriptException ex) {
      ex.printStackTrace();
  }    
```
Please check the unit tests for more detail
