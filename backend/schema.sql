-- ============================================
-- Shuriken Hub Database Schema (schema.sql)
-- ============================================

-- Users
CREATE TABLE IF NOT EXISTS users (
    email TEXT PRIMARY KEY,
    passwordHash TEXT NOT NULL,
    createdAt TEXT DEFAULT CURRENT_TIMESTAMP
);

-- Projects
CREATE TABLE IF NOT EXISTS projects (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    ownerEmail TEXT NOT NULL,
    name TEXT NOT NULL,
    visibility TEXT CHECK(visibility IN ('public','private')) DEFAULT 'private',
    createdAt TEXT DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(ownerEmail) REFERENCES users(email)
);

-- Stars (Favorites)
CREATE TABLE IF NOT EXISTS favorites (
    userEmail TEXT NOT NULL,
    projectId INTEGER NOT NULL,
    createdAt TEXT DEFAULT CURRENT_TIMESTAMP,
    UNIQUE(userEmail, projectId),
    FOREIGN KEY(userEmail) REFERENCES users(email),
    FOREIGN KEY(projectId) REFERENCES projects(id)
);

-- Followers (User Follows)
CREATE TABLE IF NOT EXISTS follows (
    followerEmail TEXT NOT NULL,
    followingEmail TEXT NOT NULL,
    createdAt TEXT DEFAULT CURRENT_TIMESTAMP,
    UNIQUE(followerEmail, followingEmail),
    FOREIGN KEY(followerEmail) REFERENCES users(email),
    FOREIGN KEY(followingEmail) REFERENCES users(email)
);

-- Issues
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

-- Issue Comments
CREATE TABLE IF NOT EXISTS issue_comments (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    issueId INTEGER NOT NULL,
    commenterEmail TEXT NOT NULL,
    body TEXT NOT NULL,
    createdAt TEXT DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(issueId) REFERENCES issues(id),
    FOREIGN KEY(commenterEmail) REFERENCES users(email)
);

-- Releases
CREATE TABLE IF NOT EXISTS releases (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    projectId INTEGER NOT NULL,
    version TEXT NOT NULL,
    description TEXT,
    fileUrl TEXT,
    createdAt TEXT DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(projectId) REFERENCES projects(id)
);

-- Organizations
CREATE TABLE IF NOT EXISTS organizations (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    ownerEmail TEXT NOT NULL,
    createdAt TEXT DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(ownerEmail) REFERENCES users(email)
);

-- Org Members
CREATE TABLE IF NOT EXISTS org_members (
    orgId INTEGER NOT NULL,
    memberEmail TEXT NOT NULL,
    role TEXT CHECK(role IN ('owner','admin','member')) DEFAULT 'member',
    FOREIGN KEY(orgId) REFERENCES organizations(id),
    FOREIGN KEY(memberEmail) REFERENCES users(email),
    UNIQUE(orgId, memberEmail)
);

-- Org Projects
CREATE TABLE IF NOT EXISTS org_projects (
    orgId INTEGER NOT NULL,
    projectId INTEGER NOT NULL,
    FOREIGN KEY(orgId) REFERENCES organizations(id),
    FOREIGN KEY(projectId) REFERENCES projects(id),
    UNIQUE(orgId, projectId)
);

-- Discussions
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

-- Discussion Comments
CREATE TABLE IF NOT EXISTS discussion_comments (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    discussionId INTEGER NOT NULL,
    commenterEmail TEXT NOT NULL,
    body TEXT NOT NULL,
    createdAt TEXT DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(discussionId) REFERENCES discussions(id),
    FOREIGN KEY(commenterEmail) REFERENCES users(email)
);

-- Pull Requests
CREATE TABLE IF NOT EXISTS pull_requests (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    projectId INTEGER NOT NULL,
    contributorEmail TEXT NOT NULL,
    title TEXT NOT NULL,
    description TEXT,
    status TEXT CHECK(status IN ('open','merged','closed')) DEFAULT 'open',
    createdAt TEXT DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(projectId) REFERENCES projects(id),
    FOREIGN KEY(contributorEmail) REFERENCES users(email)
);

-- Notifications
CREATE TABLE IF NOT EXISTS notifications (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    userEmail TEXT NOT NULL,
    message TEXT NOT NULL,
    readFlag INTEGER DEFAULT 0,
    createdAt TEXT DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(userEmail) REFERENCES users(email)
);
CREATE TABLE IF NOT EXISTS migrations (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    appliedAt TEXT DEFAULT CURRENT_TIMESTAMP
);


-- ============================================
-- End of Shuriken Hub Schema
-- ============================================
