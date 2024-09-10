FROM node:latest

LABEL "maintainer"="UniBook <systemprograming[at]unibook.com>"
LABEL "repository"="https://github.com/unibook/system-programing"
LABEL "homepage"="https://unibook.com/modern-cpp/"

# For Unicode
ENV LANG C.UTF-8
ENV LC_ALL C.UTF-8

WORKDIR /modern-cpp-tutorial

# Node & Npm & Python3 are from node:latest
# Install Texlive & Pandoc
RUN apt update && \
    apt install wget texlive-full -y && \
    wget https://github.com/jgm/pandoc/releases/download/2.7.3/pandoc-2.7.3-1-amd64.deb && \
    dpkg -i pandoc-2.7.3-1-amd64.deb
