FROM docker.io/ubuntu:impish-20211102
ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=America/Denver

RUN apt-get update && apt-get install -yqq \
	build-essential \
	cimg-dev \
	dumb-init \
	gnuplot \
	libopencv-dev \
	python3-pip \
	&& apt-get clean \
	&& rm -rf /var/lib/apt/lists/*

RUN pip install igraph

WORKDIR /app
RUN useradd -ms /bin/bash darwin
USER darwin

# Runs "/usr/bin/dumb-init -- /my/script --with --args"
ENTRYPOINT ["/usr/bin/dumb-init", "--"]
