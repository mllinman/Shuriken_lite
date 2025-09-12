// initDB.js
const fs = require("fs");
const sqlite3 = require("sqlite3").verbose();

const DB_FILE = "shuriken_hub.db";
const SCHEMA_FILE = "schema.sql";

// Create or open database
const db = new sqlite3.Database(DB_FILE);

function runMigrations() {
  if (!fs.existsSync(SCHEMA_FILE)) {
    console.error("❌ schema.sql not found!");
    process.exit(1);
  }

  const schema = fs.readFileSync(SCHEMA_FILE, "utf-8");

  db.exec(schema, (err) => {
    if (err) {
      console.error("❌ Error running migrations:", err.message);
      process.exit(1);
    } else {
      console.log("✅ Database initialized with schema.sql");
    }
    db.close();
  });
}

runMigrations();
module.exports = db;

// db.js
const sqlite3 = require("sqlite3").verbose();
const db = new sqlite3.Database("shuriken_hub.db");
const express = require("express");
const app = express();
const bodyParser = require("body-parser");
const jwt = require("jsonwebtoken");

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));
const SECRET = "your_jwt_secret";