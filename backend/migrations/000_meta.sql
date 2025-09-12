CREATE TABLE IF NOT EXISTS migrations (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    appliedAt TEXT DEFAULT CURRENT_TIMESTAMP
);
-- ============================================
-- End of Shuriken Hub Migrations