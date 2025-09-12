-- Initial migration
CREATE TABLE IF NOT EXISTS users (
    email TEXT PRIMARY KEY,
    passwordHash TEXT NOT NULL,
    createdAt TEXT DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS projects (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    ownerEmail TEXT NOT NULL,
    name TEXT NOT NULL,
    visibility TEXT CHECK(visibility IN ('public','private')) DEFAULT 'private',
    createdAt TEXT DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(ownerEmail) REFERENCES users(email)
);

CREATE TABLE IF NOT EXISTS favorites (
    userEmail TEXT NOT NULL,
    projectId INTEGER NOT NULL,
    createdAt TEXT DEFAULT CURRENT_TIMESTAMP,
    UNIQUE(userEmail, projectId),
    FOREIGN KEY(userEmail) REFERENCES users(email),
    FOREIGN KEY(projectId) REFERENCES projects(id)
);

CREATE TABLE IF NOT EXISTS follows (
    followerEmail TEXT NOT NULL,
    followingEmail TEXT NOT NULL,
    createdAt TEXT DEFAULT CURRENT_TIMESTAMP,
    UNIQUE(followerEmail, followingEmail),
    FOREIGN KEY(followerEmail) REFERENCES users(email),
    FOREIGN KEY(followingEmail) REFERENCES users(email)
);
