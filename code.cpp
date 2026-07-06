#include <WiFi.h>
#include <WebServer.h>

// Simulated Matrix Pinouts (Connect your MAX7219 module to these pins on the canvas)
#define DIN_PIN 47
#define CS_PIN  20
#define CLK_PIN 21

// REQUIRED SETTINGS FOR CIRKIT DESIGNER VIRTUAL NETWORK
const char* ssid = "CirkitWifi"; 
const char* password = NULL;     

WebServer server(80);

void writeMax7219(byte reg, byte data) {
  digitalWrite(CS_PIN, LOW);
  shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, reg);
  shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, data);
  digitalWrite(CS_PIN, HIGH);
}

void initMax7219() {
  pinMode(DIN_PIN, OUTPUT);
  pinMode(CLK_PIN, OUTPUT);
  pinMode(CS_PIN, OUTPUT);
  
  writeMax7219(0x0C, 0x01); // Normal Operation
  writeMax7219(0x0B, 0x07); // Scan-limit (All rows)
  writeMax7219(0x0A, 0x04); // Brightness
  writeMax7219(0x09, 0x00); // Matrix mode (No BCD decode)
  
  for (int i = 1; i <= 8; i++) {
    writeMax7219(i, 0x00); // Clear Display
  }
}

// Interactive Mobile Canvas HTML
const char HTML_PAGE[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 Sketchpad Simulator</title>
    <style>
        body { font-family: Arial, sans-serif; text-align: center; margin-top: 20px; background: #222; color: #fff; }
        .grid { display: grid; grid-template-columns: repeat(8, 40px); gap: 4px; justify-content: center; margin: 20px auto; }
        .cell { width: 40px; height: 40px; background: #444; border-radius: 4px; cursor: pointer; }
        .cell.active { background: #ff0055; box-shadow: 0 0 10px #ff0055; }
        button { padding: 12px 24px; font-size: 18px; margin: 10px; border: none; border-radius: 4px; cursor: pointer; font-weight: bold; }
        .btn-show { background: #28a745; color: white; }
        .btn-clear { background: #dc3545; color: white; }
    </style>
</head>
<body>
    <h2>Draw Letter on Virtual Screen</h2>
    <div class="grid" id="matrixGrid"></div>
    <button class="btn-show" onclick="sendMatrix()">SHOW</button>
    <button class="btn-clear" onclick="clearGrid()">CLEAR</button>

    <script>
        const grid = document.getElementById('matrixGrid');
        let matrix = Array(8).fill().map(() => Array(8).fill(0));
        let isDrawing = false;

        for (let r = 0; r < 8; r++) {
            for (let c = 0; c < 8; c++) {
                const cell = document.createElement('div');
                cell.className = 'cell';
                cell.dataset.row = r;
                cell.dataset.col = c;
                cell.addEventListener('mousedown', () => { isDrawing = true; toggleCell(cell, r, c); });
                cell.addEventListener('mouseenter', () => { if(isDrawing) toggleCell(cell, r, c); });
                cell.addEventListener('touchstart', (e) => { e.preventDefault(); isDrawing = true; toggleCell(cell, r, c); });
                grid.appendChild(cell);
            }
        }
        window.addEventListener('mouseup', () => isDrawing = false);
        window.addEventListener('touchend', () => isDrawing = false);

        function toggleCell(cell, r, c) {
            cell.classList.toggle('active');
            matrix[r][c] = cell.classList.contains('active') ? 1 : 0;
        }

        function clearGrid() {
            document.querySelectorAll('.cell').forEach(c => c.classList.remove('active'));
            matrix = Array(8).fill().map(() => Array(8).fill(0));
        }

        function sendMatrix() {
            let data = "";
            for(let r=0; r<8; r++) {
                let rowVal = 0;
                for(let c=0; c<8; c++) {
                    rowVal |= (matrix[r][c] << (7 - c));
                }
                data += "r" + r + "=" + rowVal + "&";
            }
            fetch('/draw?' + data.slice(0, -1), { method: 'POST' });
        }
    </script>
</body>
</html>
)=====";

void handleRoot() {
  server.send(200, "text/html", HTML_PAGE);
}

void handleDraw() {
  for (int r = 0; r < 8; r++) {
    String argName = "r" + String(r);
    if (server.hasArg(argName)) {
      byte rowValue = server.arg(argName).toInt();
      writeMax7219(r + 1, rowValue);
    }
  }
  server.send(200, "text/plain", "OK");
}

void setup() {
  Serial.begin(115200);
  initMax7219();

  // Initialize Connection to Virtual Network
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Virtual Network");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nConnected to Simulation Network!");
  Serial.print("Web Server Online at: http://");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, handleRoot);
  server.on("/draw", HTTP_POST, handleDraw);
  
  server.begin();
}

void loop() {
  server.handleClient();
}
