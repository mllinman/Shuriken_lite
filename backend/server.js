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
const express = require("express");
const http = require("http");
const { Server } = require("socket.io");
const sqlite3 = require("sqlite3").verbose();
const jwt = require("jsonwebtoken"); // already in use for auth

const app = express();
const server = http.createServer(app);
const io = new Server(server, { cors: { origin: "*" } });

const db = new sqlite3.Database("shuriken_hub.db");

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

module.exports = { app, server, db, pushNotification };
const bodyParser = require("body-parser");
const jwt = require("jsonwebtoken");
const SECRET = process.env.JWT_SECRET || "shuriken_secret";

app.use(bodyParser.json());
const db = new sqlite3.Database(DB_FILE);

function authenticateToken(req, res, next) {
  const authHeader = req.headers["authorization"];
  const token = authHeader && authHeader.split(" ")[1];
  if (!token) return res.sendStatus(401);
  jwt.verify(token, SECRET, (err, user) => {
    if (err) return res.sendStatus(403);
    req.user = user;
    next();
  });
}

db.serialize(() => {
  db.run(`CREATE TABLE IF NOT EXISTS issues (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    projectId INTEGER,
    creatorEmail TEXT,
    title TEXT,
    body TEXT,
    status TEXT DEFAULT 'open',
    createdAt TEXT DEFAULT CURRENT_TIMESTAMP
  )`);
});
    db.run(`CREATE TABLE IF NOT EXISTS comments (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    issueId INTEGER,
    creatorEmail TEXT,
    body TEXT,
    createdAt TEXT DEFAULT CURRENT_TIMESTAMP
  )`);
    db.run(`CREATE TABLE IF NOT EXISTS projects (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    creatorEmail TEXT,
    name TEXT,
    description TEXT,
    createdAt TEXT DEFAULT CURRENT_TIMESTAMP
  )`);
    db.run(`CREATE TABLE IF NOT EXISTS users (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    email TEXT,
    name TEXT,
    createdAt TEXT DEFAULT CURRENT_TIMESTAMP
  )`);
  app.post("/issues/create", authenticateToken, (req, res) => {
  const { projectId, title, body } = req.body;
  const email = req.user.email;

  db.run(
    `INSERT INTO issues (projectId, creatorEmail, title, body, status) 
     VALUES (?, ?, ?, ?, 'open')`,
    [projectId, email, title, body],
    function (err) {
      if (err) return res.status(500).json({ error: "DB insert failed" });
      res.json({ id: this.lastID, title, status: "open" });
    }
  );
});
    app.post("/comments/create", authenticateToken, (req, res) => {
  const { issueId, body } = req.body;
  const email = req.user.email;

  db.run(
    `INSERT INTO comments (issueId, creatorEmail, body) 
     VALUES (?, ?, ?)`,
    [issueId, email, body],
    function (err) {
      if (err) return res.status(500).json({ error: "DB insert failed" });
      res.json({ id: this.lastID, body });
    }
  );
});
    app.post("/projects/create", authenticateToken, (req, res) => {
  const { name, description } = req.body;
  const email = req.user.email;

  db.run(
    `INSERT INTO projects (creatorEmail, name, description) 
     VALUES (?, ?, ?)`,
    [email, name, description],
    function (err) {
      if (err) return res.status(500).json({ error: "DB insert failed" });
      res.json({ id: this.lastID, name, description });
    }
  );
});
    app.post("/users/create", (req, res) => {
  const { email, name } = req.body;

  db.run(
    `INSERT INTO users (email, name) 
     VALUES (?, ?)`,
    [email, name],
    function (err) {
      if (err) return res.status(500).json({ error: "DB insert failed" });
      res.json({ id: this.lastID, email, name });
    }
  );
});
app.get("/issues/:projectId", (req, res) => {
  const { projectId } = req.params;
  db.all(
    `SELECT * FROM issues WHERE projectId=? ORDER BY createdAt DESC`,
    [projectId],
    (err, rows) => {
      if (err) return res.status(500).json({ error: "DB query failed" });
      res.json(rows);
    }
  );
});
app.get("/comments/:issueId", (req, res) => {
  const { issueId } = req.params;
  db.all(
    `SELECT * FROM comments WHERE issueId=? ORDER BY createdAt DESC`,
    [issueId],
    (err, rows) => {
      if (err) return res.status(500).json({ error: "DB query failed" });
      res.json(rows);
    }
  );
});
app.post("/issues/update", authenticateToken, (req, res) => {
  const { issueId, status } = req.body;
  const email = req.user.email;

  db.run(
    `UPDATE issues SET status=? WHERE id=?`,
    [status, issueId],
    function (err) {
      if (err || this.changes === 0) {
        return res.status(400).json({ error: "Update failed" });
      }
      res.json({ success: true });
    }
  );
});
app.get("/projects", (req, res) => {
  db.all(`SELECT * FROM projects ORDER BY createdAt DESC`, [], (err, rows) => {
    if (err) return res.status(500).json({ error: "DB query failed" });
    res.json(rows);
  });
  db.serialize(() => {
  db.run(`CREATE TABLE IF NOT EXISTS issue_comments (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    issueId INTEGER,
    commenterEmail TEXT,
    body TEXT,
    createdAt TEXT DEFAULT CURRENT_TIMESTAMP
  )`);
});
app.post("/issues/comment", authenticateToken, (req, res) => {
  const { issueId, body } = req.body;
  const email = req.user.email;

  db.run(
    `INSERT INTO issue_comments (issueId, commenterEmail, body) VALUES (?, ?, ?)`,
    [issueId, email, body],
    function (err) {
      if (err) return res.status(500).json({ error: "DB insert failed" });
      res.json({ id: this.lastID, issueId, commenterEmail: email, body });
    }
  );
});
app.get("/issues/:issueId/comments", (req, res) => {
  const { issueId } = req.params;

  db.all(
    `SELECT * FROM issue_comments WHERE issueId=? ORDER BY createdAt ASC`,
    [issueId],
    (err, rows) => {
      if (err) return res.status(500).json({ error: "DB query failed" });
      res.json(rows);
    }
  );
});
