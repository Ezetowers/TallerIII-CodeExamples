FROM centos:latest
WORKDIR /tmp

# Install wget to be able to download OpenMPI
RUN yum update -y && \
    yum groupinstall "Development Tools" -y && \
    yum install wget vim htop -y
