# ACBr NFSe API

A Node.js REST API for integrating with ACBrLibNFSe to handle Brazilian electronic service invoices (NFSe).

## Features

- Create, sign, and validate RPS (Recibo Provisório de Serviços)
- Transmit RPS in batches
- Consult NFSe by RPS, number, or period
- Cancel NFSe
- Configure municipality and provider settings
- Support for multiple providers and municipalities
- SSL/TLS support for secure communication
- Comprehensive error handling and logging

## Prerequisites

- Node.js 20 or higher
- Docker (for containerized deployment)
- ACBrLibNFSe shared library
- SSL certificates for secure communication

## Installation

1. Clone the repository:
```bash
git clone <repository-url>
cd acbr-nfse-api
```

2. Install dependencies:
```bash
npm install
```

3. Configure the environment:
```bash
cp .env.example .env
# Edit .env with your settings
```

4. Configure ACBr:
```bash
# Edit acbr.ini with your provider and municipality settings
```

## Docker Deployment

1. Build the Docker image:
```bash
docker build -t acbr-nfse-api .
```

2. Run the container:
```bash
docker run -d \
  -p 3000:3000 \
  -v /path/to/certificates:/usr/src/app/certs \
  --name acbr-nfse-api \
  acbr-nfse-api
```

## API Endpoints

### RPS Operations

- `POST /rps`
  - Create a new RPS
  - Body: RPS data in JSON format

- `POST /rps/batch`
  - Submit multiple RPS in a batch
  - Body: Array of RPS data and batch settings

### NFSe Operations

- `GET /nfse/rps/:number/:series/:type`
  - Consult NFSe by RPS information

- `GET /nfse/number/:number`
  - Consult NFSe by number

- `GET /nfse/period`
  - Consult NFSe by period
  - Query params: startDate, endDate

- `POST /nfse/cancel`
  - Cancel an NFSe
  - Body: NFSe number, reason, and verification code

### Configuration

- `POST /config/municipality`
  - Set municipality code
  - Body: `{ "code": "string" }`

- `POST /config/provider`
  - Configure provider settings
  - Body: Provider configuration data

- `POST /config/environment`
  - Set environment (production/homologation)
  - Body: `{ "environment": "number" }`

## Error Handling

The API uses standard HTTP status codes and returns errors in the following format:

```json
{
  "error": "Error type",
  "message": "Detailed error message"
}
```

## Logging

Logs are stored in the `logs` directory:
- `error.log`: Error-level logs
- `combined.log`: All logs

In development, logs are also output to the console.

## SSL Configuration

1. Place your certificates in the `certs` directory
2. Update the SSL section in `acbr.ini`
3. Ensure proper permissions for certificate files

## Development

1. Start in development mode:
```bash
npm run dev
```

2. Run tests:
```bash
npm test
```

## Contributing

1. Fork the repository
2. Create your feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

## License

MIT License - see LICENSE file for details

## Support

For support, please open an issue in the repository or contact the maintainers. 