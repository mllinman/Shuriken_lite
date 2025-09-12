CREATE TABLE notifications (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    userEmail TEXT NOT NULL,
    message TEXT NOT NULL,
    readFlag INTEGER DEFAULT 0,
    createdAt TEXT DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT fk_user_email FOREIGN KEY (userEmail) REFERENCES users(email)
);
