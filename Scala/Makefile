all: jar bin
	cp $(shell find target/ -name kmeanspp.jar) .

jar:
	sbt assembly

bin:
	@printf '#!/bin/sh\nexec java -jar kmeanspp.jar "$$@"\n' > kmeanspp
	chmod +x kmeanspp

clean:
	rm -fr kmeanspp* target/ project/{target/,project/}
