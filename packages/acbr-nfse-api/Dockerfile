FROM sitespeedio/node:ubuntu-24-04-nodejs-20.15.1

ENV NODE_ENV=production
ENV DISPLAY=:99

WORKDIR /usr/src/app

# Install build dependencies
RUN apt update && apt install -y --no-install-recommends \
    g++ \
    make \
    cmake \
    python3

# Install ACBrLib dependencies
RUN echo "deb http://deb.debian.org/debian buster main contrib\n" >> /etc/apt/sources.list && \
    apt install -y --no-install-recommends xvfb xauth

RUN echo ttf-mscorefonts-installer msttcorefonts/accepted-mscorefonts-eula select true | debconf-set-selections && \
    apt install -y \
    perl \
    ttf-mscorefonts-installer \
    openssl \
    libssl3 \
    libssl-dev \
    ca-certificates \
    libxml2 \
    libgtk2.0-0 && \
    ln -s /usr/lib/x86_64-linux-gnu/libxml2.so.2 /usr/lib/libxml2.so

# Configure OpenSSL
COPY openssl.cnf /usr/lib/ssl/openssl.cnf

# Install prebuild globally
RUN npm install -g prebuild

# Copy and build the addon first
COPY docker/addon ./addon
WORKDIR /usr/src/app/addon
RUN npm install && \
    npm run build

# Configure the API
WORKDIR /usr/src/app/api
RUN mkdir -p lib/Linux logs

# Copy API files and ACBr configuration
COPY docker/api/* ./
COPY acbr.ini ./

# Update package.json to use the correct addon path
RUN sed -i 's|"file:../acbr-nfse-addon"|"file:../addon"|g' package.json

# Install API dependencies
RUN npm install

# Copy ACBrNFSe library
RUN mkdir -p lib/Linux && \
    cp ../addon/lib/Linux/libacbrnfse64.so lib/Linux/ && \
    chmod +x lib/Linux/libacbrnfse64.so

# Create startup script
RUN echo '#!/bin/bash\nXvfb :99 -screen 0 1280x1024x24 -ac +extension GLX +render -nolisten tcp -nolisten unix &> xvfb_warning_dump.txt & npm start' > start.sh && \
    chmod +x start.sh

# Command to start the API
CMD ["./start.sh"] 