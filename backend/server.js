const fs = require("fs");
const sqlite3 = require("sqlite3").verbose();

const DB_FILE = "shuriken_hub.db";
const SCHEMA_FILE = "schema.sql";
const { execSync } = require("child_process");

try {
  console.log("📦 Running migrations...");
  execSync("node migrate.js", { stdio: "inherit" });
} catch (err) {
  console.error("❌ Migration step failed.");
  process.exit(1);
}
const db = new sqlite3.Database("shuriken_hub.db");
const express = require("express");
const app = express();
const bodyParser = require("body-parser");
const jwt = require("jsonwebtoken");
const cors = require("cors");

// Ensure DB exists
if (!fs.existsSync(DB_FILE)) {
  console.log("📦 Database not found, creating from schema.sql...");
  if (!fs.existsSync(SCHEMA_FILE)) {
    console.error("❌ schema.sql missing! Cannot initialize DB.");
    process.exit(1);
  }

  const schema = fs.readFileSync(SCHEMA_FILE, "utf-8");
  const dbInit = new sqlite3.Database(DB_FILE);

  dbInit.exec(schema, (err) => {
    if (err) {
      console.error("❌ Error initializing DB:", err.message);
      process.exit(1);
    } else {
      console.log("✅ Database initialized successfully from schema.sql");
    }
    dbInit.close();
  });
}
const http = require("http");
const { Server } = require("socket.io");

const server = http.createServer(app);
const io = new Server(server, { cors: { origin: "*" } });

// --- WebSocket Authenticated Connections ---
io.use((socket, next) => {
  const token = socket.handshake.auth.token;
  if (!token) return next(new Error("No token provided"));

  try {
    const user = jwt.verify(token, process.env.JWT_SECRET || "shuriken_secret");
    socket.user = user;
    next();
  } catch (err) {
    next(new Error("Invalid token"));
  }
});

io.on("connection", (socket) => {
  console.log(`🔌 User connected: ${socket.user.email}`);

  socket.join(socket.user.email); // join room = email

  socket.on("disconnect", () => {
    console.log(`❌ User disconnected: ${socket.user.email}`);
  });
});

// --- Utility: Push a notification to a user ---
function pushNotification(userEmail, message) {
  // Save to DB
  db.run(
    `INSERT INTO notifications (userEmail, message) VALUES (?, ?)`,
    [userEmail, message],
    function (err) {
      if (!err) {
        // Emit to connected clients
        io.to(userEmail).emit("notification", {
          id: this.lastID,
          message,
          readFlag: 0,
          createdAt: new Date().toISOString(),
        });
      }
    }
  );
}

app.use(cors());
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

module.exports = { app, server, db, pushNotification };

// E-commerce routes
const ecommerceRoutes = require('./ecommerce');
app.use('/api', ecommerceRoutes);

// Serve static files for frontend and product images
app.use(express.static('../frontend'));
app.use('/images', express.static('public/images'));

// Serve main page
app.get('/', (req, res) => {
  res.sendFile(require('path').join(__dirname, '../frontend/index.html'));
});

app.use(bodyParser.urlencoded({ extended: true }));
const SECRET = "your_jwt_secret";

// Start server
const PORT = process.env.PORT || 3000;
server.listen(PORT, () => {
  console.log(`🚀 CyberRanger E-commerce Server running on port ${PORT}`);
});

// Example route to demonstrate pushNotification usage
app.post("/star", (req, res) => {
  const { projectId, email } = req.body;
  // Logic to star the project...
  // Example: send a notification to the user
  pushNotification(email, `You starred project ${projectId}`);
  res.json({ success: true, message: "Project starred and notification sent." });
});