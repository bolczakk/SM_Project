const dgram = require('dgram');
const express = require('express');
const http = require('http');
const { Server } = require('socket.io');
const path = require('path');

// Konfiguracja serwerów
const app = express();
const webServer = http.createServer(app);
const io = new Server(webServer);
const udpServer = dgram.createSocket('udp4');

const UDP_PORT = 1234;
const WEB_PORT = 3000;

// Serwowanie plików statycznych (nasz frontend)
app.use(express.static(path.join(__dirname, 'public')));

// --- LOGIKA UDP ---
udpServer.on('message', (msg, rinfo) => {
    const rawData = msg.toString();
    const [temp, setpoint, duty] = rawData.split(';');

    const data = {
        temp: temp,
        setpoint: setpoint,
        duty: duty,
        time: new Date().toLocaleTimeString()
    };

    console.log(`Odebrano: ${rawData}`);

    // WYSYŁANIE DO FRONTENDU przez WebSocket
    io.emit('sensor-data', data);
});

udpServer.bind(UDP_PORT, '0.0.0.0', () => {
    console.log(`[UDP] Słucham Nucleo na porcie ${UDP_PORT}`);
});

// --- START SERWERA WWW ---
webServer.listen(WEB_PORT, () => {
    console.log(`[WEB] Dashboard dostępny pod: http://localhost:${WEB_PORT}`);
});