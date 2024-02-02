# Use the official Ubuntu 20.04 image as a base
FROM ubuntu:20.04

# Avoid warnings by switching to noninteractive
ENV DEBIAN_FRONTEND=noninteractive

# Update the apt package list
RUN apt-get update --fix-missing

# Install build-essential, cmake, libpcap-dev
RUN apt-get install -y git build-essential cmake libpcap-dev

# Install software-properties-common
RUN apt-get install -y software-properties-common

# Add deadsnakes PPA
RUN add-apt-repository ppa:deadsnakes/ppa

# Install python3.9 and python3.9-venv
RUN apt-get install -y python3.9 python3.9-venv

# Install pip for Python 3.9
RUN apt-get install -y curl
RUN curl https://bootstrap.pypa.io/get-pip.py | python3.9

# Install pipx
RUN python3.9 -m pip install pipx
RUN python3.9 -m pipx ensurepath

# Install poetry via pipx
RUN pipx install poetry

# Switch back to dialog for any ad-hoc use of apt-get
ENV DEBIAN_FRONTEND=dialog
