CREATE TABLE IF NOT EXISTS discussions (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    projectId INTEGER NOT NULL,
    creatorEmail TEXT NOT NULL,
    title TEXT NOT NULL,
    body TEXT,
    createdAt TEXT DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(projectId) REFERENCES projects(id),
    FOREIGN KEY(creatorEmail) REFERENCES users(email)
);

CREATE TABLE IF NOT EXISTS discussion_comments (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    discussionId INTEGER NOT NULL,
    commenterEmail TEXT NOT NULL,
    body TEXT NOT NULL,
    createdAt TEXT DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(discussionId) REFERENCES discussions(id),
    FOREIGN KEY(commenterEmail) REFERENCES users(email)
);
