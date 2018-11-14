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

ENV PATH $PATH:/wst/bin

# RUN apt -y update && apt install -y python 

WORKDIR /wst

EXPOSE 18005

CMD ["/wst/bin/llmbs"]