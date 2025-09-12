// migrate.js
const fs = require("fs");
const path = require("path");
const sqlite3 = require("sqlite3").verbose();

const DB_FILE = "shuriken_hub.db";
const MIGRATIONS_DIR = path.join(__dirname, "migrations");

const db = new sqlite3.Database(DB_FILE);

function runMigrations() {
  if (!fs.existsSync(MIGRATIONS_DIR)) {
    console.error("❌ Migrations folder missing!");
    process.exit(1);
  }

  db.serialize(() => {
    db.run(`CREATE TABLE IF NOT EXISTS migrations (
      id INTEGER PRIMARY KEY AUTOINCREMENT,
      name TEXT NOT NULL,
      appliedAt TEXT DEFAULT CURRENT_TIMESTAMP
    )`);

    db.all(`SELECT name FROM migrations`, (err, rows) => {
      if (err) throw err;

      const applied = rows.map(r => r.name);
      const files = fs.readdirSync(MIGRATIONS_DIR).sort();

      const pending = files.filter(f => !applied.includes(f));

      if (pending.length === 0) {
        console.log("✅ No new migrations.");
        db.close();
        return;
      }

      console.log("📦 Applying migrations:", pending);

      (function applyNext(i) {
        if (i >= pending.length) {
          console.log("✅ All migrations applied.");
          db.close();
          return;
        }

        const file = pending[i];
        const sql = fs.readFileSync(path.join(MIGRATIONS_DIR, file), "utf-8");

        db.exec(sql, (err) => {
          if (err) {
            console.error("❌ Migration failed:", file, err.message);
            process.exit(1);
          } else {
            db.run(`INSERT INTO migrations (name) VALUES (?)`, [file], () => {
              console.log("✅ Applied:", file);
              applyNext(i + 1);
            });
          }
        });
      })(0);
    });
  });
}

runMigrations();
