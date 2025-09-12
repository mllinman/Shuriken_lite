CREATE TABLE IF NOT EXISTS issues (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    projectId INTEGER NOT NULL,
    creatorEmail TEXT NOT NULL,
    title TEXT NOT NULL,
    body TEXT,
    status TEXT CHECK(status IN ('open','closed')) DEFAULT 'open',
    createdAt TEXT DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(projectId) REFERENCES projects(id),
    FOREIGN KEY(creatorEmail) REFERENCES users(email)
);

CREATE TABLE IF NOT EXISTS issue_comments (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    issueId INTEGER NOT NULL,
    commenterEmail TEXT NOT NULL,
    body TEXT NOT NULL,
    createdAt TEXT DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(issueId) REFERENCES issues(id),
    FOREIGN KEY(commenterEmail) REFERENCES users(email)
);
CREATE TABLE IF NOT EXISTS migrations (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    appliedAt TEXT DEFAULT CURRENT_TIMESTAMP
);