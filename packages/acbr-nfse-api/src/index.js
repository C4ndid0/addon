require('dotenv').config();
const express = require('express');
const cors = require('cors');
const bodyParser = require('body-parser');
const winston = require('winston');
const NFSe = require('@acbr/nfse-addon');

// Configure logger
const logger = winston.createLogger({
    level: 'info',
    format: winston.format.combine(
        winston.format.timestamp(),
        winston.format.json()
    ),
    transports: [
        new winston.transports.File({ filename: 'logs/error.log', level: 'error' }),
        new winston.transports.File({ filename: 'logs/combined.log' })
    ]
});

if (process.env.NODE_ENV !== 'production') {
    logger.add(new winston.transports.Console({
        format: winston.format.simple()
    }));
}

const app = express();
const nfse = new NFSe();

// Initialize NFSe with config
try {
    nfse.initialize('acbr.ini');
    logger.info('NFSe initialized successfully');
} catch (error) {
    logger.error('Failed to initialize NFSe:', error);
    process.exit(1);
}

// Middleware
app.use(cors());
app.use(bodyParser.json());

// Error handling middleware
app.use((err, req, res, next) => {
    logger.error('Error:', err);
    res.status(500).json({
        error: 'Internal Server Error',
        message: err.message
    });
});

// Health check endpoint
app.get('/health', (req, res) => {
    res.json({ status: 'ok' });
});

// RPS Endpoints
app.post('/rps', async (req, res) => {
    try {
        const rpsData = req.body;
        await nfse.createRPS(rpsData);
        await nfse.signRPS();
        await nfse.validateRPS();
        const response = await nfse.getLastResponse();
        res.json({ success: true, data: response });
    } catch (error) {
        logger.error('Error creating RPS:', error);
        res.status(400).json({
            error: 'Failed to create RPS',
            message: error.message
        });
    }
});

app.post('/rps/batch', async (req, res) => {
    try {
        const { rpsList, batch, sync } = req.body;
        
        for (const rpsData of rpsList) {
            await nfse.addRPS(rpsData);
        }
        
        await nfse.signRPS();
        await nfse.validateRPS();
        const result = await nfse.transmitRPS(batch, false, sync);
        const response = await nfse.getLastResponse();
        
        res.json({
            success: true,
            data: {
                result,
                response
            }
        });
    } catch (error) {
        logger.error('Error processing RPS batch:', error);
        res.status(400).json({
            error: 'Failed to process RPS batch',
            message: error.message
        });
    }
});

// NFSe Consultation Endpoints
app.get('/nfse/rps/:number/:series/:type', async (req, res) => {
    try {
        const { number, series, type } = req.params;
        await nfse.consultNFSeByRPS(number, series, type);
        const response = await nfse.getLastResponse();
        res.json({ success: true, data: response });
    } catch (error) {
        logger.error('Error consulting NFSe by RPS:', error);
        res.status(400).json({
            error: 'Failed to consult NFSe',
            message: error.message
        });
    }
});

app.get('/nfse/number/:number', async (req, res) => {
    try {
        const { number } = req.params;
        await nfse.consultNFSeByNumber(number);
        const response = await nfse.getLastResponse();
        res.json({ success: true, data: response });
    } catch (error) {
        logger.error('Error consulting NFSe by number:', error);
        res.status(400).json({
            error: 'Failed to consult NFSe',
            message: error.message
        });
    }
});

app.get('/nfse/period', async (req, res) => {
    try {
        const { startDate, endDate } = req.query;
        await nfse.consultNFSeByPeriod(startDate, endDate);
        const response = await nfse.getLastResponse();
        res.json({ success: true, data: response });
    } catch (error) {
        logger.error('Error consulting NFSe by period:', error);
        res.status(400).json({
            error: 'Failed to consult NFSe',
            message: error.message
        });
    }
});

// NFSe Cancellation
app.post('/nfse/cancel', async (req, res) => {
    try {
        const { number, reason, verificationCode } = req.body;
        await nfse.cancelNFSe(number, reason, verificationCode);
        const response = await nfse.getLastResponse();
        res.json({ success: true, data: response });
    } catch (error) {
        logger.error('Error cancelling NFSe:', error);
        res.status(400).json({
            error: 'Failed to cancel NFSe',
            message: error.message
        });
    }
});

// Configuration Endpoints
app.post('/config/municipality', async (req, res) => {
    try {
        const { code } = req.body;
        await nfse.setMunicipality(code);
        res.json({ success: true });
    } catch (error) {
        logger.error('Error setting municipality:', error);
        res.status(400).json({
            error: 'Failed to set municipality',
            message: error.message
        });
    }
});

app.post('/config/provider', async (req, res) => {
    try {
        const providerData = req.body;
        await nfse.setProvider(providerData);
        res.json({ success: true });
    } catch (error) {
        logger.error('Error setting provider:', error);
        res.status(400).json({
            error: 'Failed to set provider',
            message: error.message
        });
    }
});

app.post('/config/environment', async (req, res) => {
    try {
        const { environment } = req.body;
        await nfse.setEnvironment(environment);
        res.json({ success: true });
    } catch (error) {
        logger.error('Error setting environment:', error);
        res.status(400).json({
            error: 'Failed to set environment',
            message: error.message
        });
    }
});

// Graceful shutdown
process.on('SIGTERM', () => {
    logger.info('SIGTERM received. Shutting down gracefully...');
    nfse.finalize();
    process.exit(0);
});

process.on('SIGINT', () => {
    logger.info('SIGINT received. Shutting down gracefully...');
    nfse.finalize();
    process.exit(0);
});

// Start server
const PORT = process.env.PORT || 3000;
app.listen(PORT, () => {
    logger.info(`Server running on port ${PORT}`);
}); 