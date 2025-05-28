#!/bin/bash

# Create lib directory if it doesn't exist
mkdir -p lib/Linux

# Copy ACBrNFSe library if it exists in the parent directory
if [ -f "../libacbrnfse64.so" ]; then
    cp ../libacbrnfse64.so lib/Linux/
fi

# Install dependencies
npm install

# Build the addon
npm run build

# Run tests if build is successful
if [ $? -eq 0 ]; then
    npm test
fi 