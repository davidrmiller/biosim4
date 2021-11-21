FROM docker.io/ubuntu:impish-20211102
ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=America/New-York

RUN apt-get update && apt-get install -yqq \
	build-essential \
	gnuplot \
	cimg-dev \
	libopencv-dev \
	python3-pip \
	&& apt-get clean \
	&& rm -rf /var/lib/apt/lists/*

RUN pip install igraph

WORKDIR /app
RUN useradd -ms /bin/bash darwin
USER darwin
