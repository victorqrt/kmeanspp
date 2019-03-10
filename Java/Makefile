all: jar runner

classes:
	javac src/*.java
	mv src/*.class .

jar: classes
	jar cvmf MANIFEST.MF kmeanspp.jar *.class
	rm -f *.class

runner:
	@printf '#!/bin/bash\nexec java -jar kmeanspp.jar "$$@"\n' > kmeanspp
	chmod +x kmeanspp

clean:
	rm -fr *.jar kmeanspp *.csv *.svg *.class
