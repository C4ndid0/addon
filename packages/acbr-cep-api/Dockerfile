FROM sitespeedio/node:ubuntu-24-04-nodejs-20.15.1

ENV NODE_ENV=production
ENV DISPLAY=:99

WORKDIR /usr/src/app

# Instalando as dependências de build
RUN apt update && apt install -y --no-install-recommends \
    g++ \
    make \
    cmake \
    python3

# Instalando dependências do ACBrLib
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

# Configurar OpenSSL
COPY openssl.cnf /usr/lib/ssl/openssl.cnf

# Instalar prebuild globalmente
RUN npm install -g prebuild

# Copiar e construir o addon primeiro
COPY docker/addon ./addon
WORKDIR /usr/src/app/addon
RUN npm install && \
    npm run build

# Configurar a API
WORKDIR /usr/src/app/api
RUN mkdir -p lib/Linux logs

# Copiar arquivos da API e configuração ACBr
COPY docker/api/* ./
COPY acbr.ini ./

# Atualizar o package.json para usar o caminho correto do addon
RUN sed -i 's|"file:../acbr-cep-addon"|"file:../addon"|g' package.json

# Instalar dependências da API
RUN npm install

# Copiar a biblioteca do ACBr
RUN mkdir -p lib/Linux && \
    cp ../addon/lib/Linux/libacbrcep64.so lib/Linux/ && \
    chmod +x lib/Linux/libacbrcep64.so

# Criar script de inicialização
RUN echo '#!/bin/bash\nXvfb :99 -screen 0 1280x1024x24 -ac +extension GLX +render -nolisten tcp -nolisten unix &> xvfb_warning_dump.txt & npm test' > start.sh && \
    chmod +x start.sh

# Comando para executar o teste
CMD ["./start.sh"]

