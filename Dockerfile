##########################################################
# Dockerfile to build mbs container images
# Based on ubuntu:16.04
##########################################################

FROM ubuntu:16.04

LABEL maintainer="liuzh <liuzh@llvision.com>"


COPY wst/llmbs /wst/bin/llmbs
COPY tools/AgoraCoreService /wst/tools/AgoraCoreService
COPY tools/convert.py /wst/tools/convert.py
COPY tools/ffmpeg /wst/tools/ffmpeg
COPY conf/llmbs.conf /wst/conf/llmbs.conf
RUN mkdir -p /wst/logs 
RUN mkdir -p /wst/data

RUN apt update && apt install -y python

ENV PATH $PATH:/wst/bin

# RUN apt -y update && apt install -y python 
# VOLUME [ "/wst/data" ]
WORKDIR /wst

EXPOSE 18005

CMD ["/wst/bin/llmbs"]