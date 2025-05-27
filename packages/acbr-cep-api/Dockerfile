FROM node:20

RUN apt-get update && apt-get install -y \
    build-essential \
    python3 \
    libgtk2.0-0 \
    libx11-6 \
    libx11-xcb1 \
    libxcb1 \
    libxcomposite1 \
    libxcursor1 \
    libxdamage1 \
    libxext6 \
    libxfixes3 \
    libxi6 \
    libxrandr2 \
    libxrender1 \
    libxss1 \
    libxtst6 \
    ca-certificates \
    libssl-dev \
    openssl \
    xvfb \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /usr/src/app

COPY . .

# Create log directory with proper permissions
RUN mkdir -p logs && \
    touch logs/acbr_cep.log && \
    chmod 777 logs && \
    chmod 666 logs/acbr_cep.log

# Update SSL certificates
RUN update-ca-certificates

# Install Node.js dependencies
RUN npm install

# Create startup script
RUN echo '#!/bin/bash\n\
export DISPLAY=:99\n\
Xvfb :99 -screen 0 1024x768x16 &\n\
sleep 1\n\
echo "Testing network connectivity..."\n\
curl -v https://viacep.com.br/ws/37200645/json/\n\
echo "Starting ACBr test..."\n\
node test.js 2>logs/debug.log' > /usr/src/app/start.sh && \
chmod +x /usr/src/app/start.sh

CMD ["/usr/src/app/start.sh"]

